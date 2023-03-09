#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <setjmp.h>

#if defined(__MINGW32__) || defined(__MINGW64__)
#include <windows.h>
#include <sys/types.h>
#include <sys/timeb.h>
#elif defined(__APPLE__)
#include <sys/time.h>
#else
#include <malloc.h>
#endif

#ifndef M_PI
#define M_PI 3.141592653589793238462643383279502884
#endif

#ifndef min
#define min(x, y) ((x) < (y) ? (x) : (y))
#endif

#ifndef max
#define max(x, y) ((x) > (y) ? (x) : (y))
#endif

#ifndef abs
#define abs(x) ((x) < 0 ? -(x) : (x))
#endif

#define INLINE __attribute__((always_inline))

#include "sleefdft.h"

#include "dispatch.h"

//

SleefDFT_real ctbl[] = {
  0.7071067811865476, -0.7071067811865476, 0.38268343236508984, -0.38268343236508984, 
  0.9238795325112867, -0.9238795325112867, 0.5555702330196023, -0.5555702330196023, 
  0.8314696123025452, -0.8314696123025452, 0.19509032201612833, -0.19509032201612833, 
  0.9807852804032304, -0.9807852804032304, 0.773010453362737, -0.773010453362737, 
  0.6343932841636455, -0.6343932841636455, 0.4713967368259978, -0.4713967368259978, 
  0.881921264348355, -0.881921264348355, 0.9951847266721969, -0.9951847266721969, 
  0.09801714032956077, -0.09801714032956077, 0.9569403357322088, -0.9569403357322088, 
  0.29028467725446233, -0.29028467725446233, 
};

//

#if defined(__MINGW32__) || defined(__MINGW64__)
void *SleefDFT_malloc(size_t z) { return _aligned_malloc(z, 4096); }
void SleefDFT_free(void *ptr) { _aligned_free(ptr); }
#elif defined(__APPLE__)
void *SleefDFT_malloc(size_t z) { return malloc(z); }
void SleefDFT_free(void *ptr) { free(ptr); }
#else
void *SleefDFT_malloc(size_t z) { return memalign(4096, z); }
void SleefDFT_free(void *ptr) { free(ptr); }
#endif

static uint32_t ilog2(uint32_t q) {
  static const uint32_t tab[] = {0,1,2,2,3,3,3,3,4,4,4,4,4,4,4,4};
  uint32_t r = 0,qq;

  if (q & 0xffff0000) r = 16;

  q >>= r;
  qq = q | (q >> 1);
  qq |= (qq >> 2);
  qq = ((qq & 0x10) >> 4) | ((qq & 0x100) >> 7) | ((qq & 0x1000) >> 10);

  return r + tab[qq] * 4 + tab[q >> (tab[qq] * 4)] - 1;
}

INLINE static uint32_t perm(int nbits, uint32_t k, int s, int d) {
  s = min(max(s, 0), nbits);
  d = min(max(d, 0), nbits);
  uint32_t r;
  r = (((k & 0xaaaaaaaa) >> 1) | ((k & 0x55555555) << 1));
  r = (((r & 0xcccccccc) >> 2) | ((r & 0x33333333) << 2));
  r = (((r & 0xf0f0f0f0) >> 4) | ((r & 0x0f0f0f0f) << 4));
  r = (((r & 0xff00ff00) >> 8) | ((r & 0x00ff00ff) << 8));
  r = ((r >> 16) | (r << 16)) >> (32-nbits);

  return (((r << s) | (k & ~(-1 << s))) & ~(-1 << d)) |
    ((((k >> s) | (r & (-1 << (nbits-s)))) << d) & ~(-1 << nbits));
}

static double r2coefc(int i, int log2len, int level) {
  int a = ((i & (-1 << (log2len - level))) + (1 << (log2len-2))) & ~(-1 << log2len);
  return sin(a * (M_PI/(1 << (log2len-1))));
}

static double r2coefs(int i, int log2len, int level) {
  int a = i & ((-1 << (log2len - level)) & ~(-1 << log2len));
  return sin(a * (M_PI/(1 << (log2len-1))));
}

static double srcoefc(int i, int log2len, int level) {
  int a = (3*(i & (-1 << (log2len - level))) + (1 << (log2len-2))) & ~(-1 << log2len);
  return sin(a * (M_PI/(1 << (log2len-1))));
}

static double srcoefs(int i, int log2len, int level) {
  int a = (3*(i & (-1 << (log2len - level)))) & ~(-1 << log2len);
  return sin(a * (M_PI/(1 << (log2len-1))));
}

static double rtTable(int x, int y, int length) {
  x = x*2 + (y & 1);
  y >>= 1;
  switch(x) {
  case 0: return 0.5-0.5*sin(-y/(double)length*M_PI);
  case 1: return     0.5*cos(-y/(double)length*M_PI);
  case 2: return 0.5-0.5*sin( y/(double)length*M_PI);
  case 3: return     0.5*cos( y/(double)length*M_PI);
  }
  return 0;
}

static const int constK[] = {
  0, 2, 6, 14, 38, 94, 230, 542, 1254
};

static int makeTableRecurse(SleefDFT_real *x, int *p, const int log2len, const int levelorg, const int levelinc, const int sign, const int top, const int bot, const int N, int cnt) {
  if (levelinc >= N-1) return cnt;
  const int level = levelorg - levelinc;
  if (bot - top > 4) {
    const int bl = 1 << (N - levelinc);
    const int w = bl/4;
    for(int j=0;j<(bot-top)/bl;j++) {
      for(int i=0;i<w;i++) {
	x[cnt] =  r2coefs(-sign*(p[(levelinc << N) + top+bl*j+i] & (-1 << (log2len - level))), log2len, level); cnt++;
	x[cnt] = -r2coefc(      (p[(levelinc << N) + top+bl*j+i] & (-1 << (log2len - level))), log2len, level); cnt++;
	x[cnt] =  srcoefs(-sign*(p[(levelinc << N) + top+bl*j+i] & (-1 << (log2len - level))), log2len, level); cnt++;
	x[cnt] = -srcoefc(      (p[(levelinc << N) + top+bl*j+i] & (-1 << (log2len - level))), log2len, level); cnt++;
      }
      cnt = makeTableRecurse(x, p, log2len, levelorg, levelinc+1, sign, top+bl*j       , top+bl*j + bl/2, N, cnt);
      cnt = makeTableRecurse(x, p, log2len, levelorg, levelinc+2, sign, top+bl*j + bl/2, top+bl*j + bl  , N, cnt);
    }
  } else if (bot - top == 4) {
    x[cnt] =  r2coefs(-sign*(p[(levelinc << N) + top] & (-1 << (log2len - level))), log2len, level); cnt++;
    x[cnt] = -r2coefc(      (p[(levelinc << N) + top] & (-1 << (log2len - level))), log2len, level); cnt++;
    x[cnt] =  srcoefs(-sign*(p[(levelinc << N) + top] & (-1 << (log2len - level))), log2len, level); cnt++;
    x[cnt] = -srcoefc(      (p[(levelinc << N) + top] & (-1 << (log2len - level))), log2len, level); cnt++;
  }

  return cnt;
}

static SleefDFT_real **makeTable(int sign, int vecwidth, int log2len, const int N, const int K) {
  if (log2len < N) return NULL;

  SleefDFT_real **tbl = (SleefDFT_real **)calloc(sizeof(SleefDFT_real *), (log2len+1));

  for(int level=N;level<=log2len;level++) {
    if (level == log2len && (1 << (log2len-N)) < vecwidth) { tbl[level] = NULL; continue; }

    int tblOffset = 0;
    tbl[level] = (SleefDFT_real *)SleefDFT_malloc(sizeof(SleefDFT_real) * (K << (level-N)));

    for(int i0=0;i0 < (1 << (log2len-N));i0+=(1 << (log2len - level))) {
      int p[(N+1)<<N];

      for(int j=0;j<N+1;j++) {
	for(int i=0;i<(1 << N);i++) {
	  p[(j << N) + i] = perm(log2len, i0 + (i << (log2len-N)), log2len-level, log2len-(level-j));
	}
      }

      tbl[level][tblOffset++] =  r2coefc(      (p[((N-1) << N) + 0] & (-1 << (log2len - level))), log2len, level-N+1);
      tbl[level][tblOffset++] =  r2coefs(-sign*(p[((N-1) << N) + 0] & (-1 << (log2len - level))), log2len, level-N+1);

      tblOffset = makeTableRecurse(tbl[level], p, log2len, level, 0, sign, 0, 1 << N, N, tblOffset);
    }

    if (level == log2len) {
      SleefDFT_real *atbl = (SleefDFT_real *)SleefDFT_malloc(sizeof(SleefDFT_real)*(K << (log2len-N))*2);
      tblOffset = 0;
      while(tblOffset < (K << (log2len-N))) {
	for(int k=0;k < K;k++) {
	  for(int v = 0;v < vecwidth;v++) {
	    assert((tblOffset + k * vecwidth + v)*2 + 1 < (K << (log2len-N))*2);
	    atbl[(tblOffset + k * vecwidth + v)*2 + 0] = tbl[log2len][tblOffset + v * K + k];
	    atbl[(tblOffset + k * vecwidth + v)*2 + 1] = tbl[log2len][tblOffset + v * K + k];
	  }
	}
	tblOffset += K * vecwidth;
      }
      SleefDFT_free(tbl[log2len]);
      tbl[log2len] = atbl;
    }
  }

  return tbl;
}

#define MAGIC 0x12345678

typedef struct SleefDFT {
  uint32_t magic, mode, log2len, dimension;

  SleefDFT_real **tbl[MAXBUTWIDTH+1];
  SleefDFT_real *rtCoef0, *rtCoef1;
  uint32_t **perm;

  SleefDFT_real *x0, *x1;

  int dataType;
  int isa;
  int planMode;

  int vecwidth, log2vecwidth;

  uint64_t tm[(MAXBUTWIDTH+1)*32], tm2[(MAXBUTWIDTH+1)*32];
  uint64_t bestTime;
  int *bestPath, pathLen;
} SleefDFT;

//

static void freeTables(SleefDFT *p) {
  for(int N=1;N<=MAXBUTWIDTH;N++) {
    for(uint32_t level=N;level<=p->log2len;level++) {
      SleefDFT_free(p->tbl[N][level]);
    }
    free(p->tbl[N]);
    p->tbl[N] = NULL;
  }
}

void SleefDFT_dispose(SleefDFT *p) {
  assert(p != NULL && p->magic == MAGIC);

  if ((p->mode & SLEEF_MODE_REAL) != 0) {
    SleefDFT_free(p->rtCoef1);
    SleefDFT_free(p->rtCoef0);
    p->rtCoef0 = p->rtCoef1 = NULL;
  }

  SleefDFT_free(p->x1);
  SleefDFT_free(p->x0);
  p->x0 = p->x1 = NULL;

  for(int level = p->log2len;level >= 1;level--) {
    SleefDFT_free(p->perm[level]);
  }
  free(p->perm);
  p->perm = NULL;

  free(p->bestPath);
  p->bestPath = NULL;

  freeTables(p);

  p->magic = 0;
  free(p);
}

static void dispatch(SleefDFT *p, const int N, SleefDFT_real *d, const SleefDFT_real *s, const int level, const int stream) {
  const int K = constK[N], log2len = p->log2len;
  if (level == N) {
    if ((p->mode & SLEEF_MODE_BACKWARD) == 0) {
      if (UNROLLMAX >= 0 && log2len-N <= UNROLLMAX) {
	void (*func)(SleefDFT_real *, const SleefDFT_real *) = dftf[p->isa][N][log2len-N];
	(*func)(d, s);
      } else {
	void (*func)(SleefDFT_real *, const SleefDFT_real *, const int) = stream ? nudftfs[p->isa][N] : nudftf[p->isa][N];
	(*func)(d, s, log2len-N);
      }
    } else {
      if (UNROLLMAX >= 0 && log2len-N <= UNROLLMAX) {
	void (*func)(SleefDFT_real *, const SleefDFT_real *) = dftb[p->isa][N][log2len-N];
	(*func)(d, s);
      } else {
	void (*func)(SleefDFT_real *, const SleefDFT_real *, const int) = stream ? nudftbs[p->isa][N] : nudftb[p->isa][N];
	(*func)(d, s, log2len-N);
      }
    }
  } else if (level == log2len) {
    assert(p->vecwidth <= (1 << N));
    if ((p->mode & SLEEF_MODE_BACKWARD) == 0) {
      if (UNROLLMAX >= 0 && log2len-N <= UNROLLMAX && !stream) {
	void (*func)(SleefDFT_real *, uint32_t *, const SleefDFT_real *, const SleefDFT_real *, const int) = tbutf[p->isa][N][log2len-N];
	(*func)(d, p->perm[level], s, p->tbl[N][level], K);
      } else {
	void (*func)(SleefDFT_real *, uint32_t *, const SleefDFT_real *, const int, const SleefDFT_real *, const int) = stream ? nutbutfs[p->isa][N] : nutbutf[p->isa][N];
	(*func)(d, p->perm[level], s, log2len-N, p->tbl[N][level], K);
      }
    } else {
      if (UNROLLMAX >= 0 && log2len-N <= UNROLLMAX && !stream) {
	void (*func)(SleefDFT_real *, uint32_t *, const SleefDFT_real *, const SleefDFT_real *, const int) = tbutb[p->isa][N][log2len-N];
	(*func)(d, p->perm[level], s, p->tbl[N][level], K);
      } else {
	void (*func)(SleefDFT_real *, uint32_t *, const SleefDFT_real *, const int, const SleefDFT_real *, const int) = stream ? nutbutbs[p->isa][N] : nutbutb[p->isa][N];
	(*func)(d, p->perm[level], s, log2len-N, p->tbl[N][level], K);
      }
    }
  } else {
    if ((p->mode & SLEEF_MODE_BACKWARD) == 0) {
      if (UNROLLMAX2 >= 0 && log2len-level <= UNROLLMAX2 && log2len-N <= UNROLLMAX2 && !stream) {
	void (*func)(SleefDFT_real *, uint32_t *, const SleefDFT_real *, const SleefDFT_real *, const int) = butf[p->isa][log2len-level][N][log2len-N];
	(*func)(d, p->perm[level], s, p->tbl[N][level], K);
      } else {
	void (*func)(SleefDFT_real *, uint32_t *, const int, const SleefDFT_real *, const int, const SleefDFT_real *, const int) = stream ? nubutfs[p->isa][N] : nubutf[p->isa][N];
	(*func)(d, p->perm[level], log2len-level, s, log2len-N, p->tbl[N][level], K);
      }
    } else {
      if (UNROLLMAX2 >= 0 && log2len-level <= UNROLLMAX2 && log2len-N <= UNROLLMAX2 && !stream) {
	void (*func)(SleefDFT_real *, uint32_t *, const SleefDFT_real *, const SleefDFT_real *, const int) = butb[p->isa][log2len-level][N][log2len-N];
	(*func)(d, p->perm[level], s, p->tbl[N][level], K);
      } else {
	void (*func)(SleefDFT_real *, uint32_t *, const int, const SleefDFT_real *, const int, const SleefDFT_real *, const int) = stream ? nubutbs[p->isa][N] : nubutb[p->isa][N];
	(*func)(d, p->perm[level], log2len-level, s, log2len-N, p->tbl[N][level], K);
      }
    }
  }
}

void SleefDFT_execute(SleefDFT *p, SleefDFT_real *d, const SleefDFT_real *s) {
  assert(p != NULL && p->magic == MAGIC);

  SleefDFT_real *t[] = { p->x1, p->x0, d };
  const SleefDFT_real *lb = s;
  int nb = 0;
  if ((p->mode & SLEEF_MODE_REAL) != 0 && (p->pathLen & 1) == 0 &&
      ((p->mode & SLEEF_MODE_BACKWARD) != 0) != ((p->mode & SLEEF_MODE_ALT) != 0)) nb = -1;
  if ((p->mode & SLEEF_MODE_REAL) == 0 && (p->pathLen & 1) == 1) nb = -1;

  if ((p->mode & SLEEF_MODE_REAL) != 0 &&
      ((p->mode & SLEEF_MODE_BACKWARD) != 0) != ((p->mode & SLEEF_MODE_ALT) != 0)) {
    (*realSub1[p->isa])(t[nb+1], s, p->log2len, p->rtCoef0, p->rtCoef1);
    if ((p-> mode & SLEEF_MODE_ALT) == 0) t[nb+1][(1 << p->log2len)+1] = -s[(1 << p->log2len)+1];
    lb = t[nb+1];
    nb = (nb + 1) & 1;
  }

  for(int level = p->log2len;level >= 1;) {
    int N = abs(p->bestPath[level]), st = p->bestPath[level] < 0;
    dispatch(p, N, t[nb+1], lb, level, st);
    level -= N;
    lb = t[nb+1];
    nb = (nb + 1) & 1;
  }

  if ((p->mode & SLEEF_MODE_REAL) != 0 && 
      ((p->mode & SLEEF_MODE_BACKWARD) == 0) != ((p->mode & SLEEF_MODE_ALT) != 0)) {
    (*realSub0[p->isa])(d, lb, p->log2len, p->rtCoef0, p->rtCoef1);
    if ((p-> mode & SLEEF_MODE_ALT) == 0) d[(1 << p->log2len)+1] = -d[(1 << p->log2len)+1];
  }
}

#if defined(__MINGW32__) || defined(__MINGW64__)
static uint64_t gettime() {
  struct __timeb64 t;
  _ftime64(&t);
  return t.time * 1000LL + t.millitm;
}
#elif defined(__APPLE__)
static uint64_t gettime() {
  struct timeval time;
  gettimeofday(&time, NULL);
  return (uint64_t)((time.tv_sec * 1000) + (time.tv_usec / 1000));
}
#else
static uint64_t gettime() {
  struct timespec tp;
  clock_gettime(CLOCK_MONOTONIC, &tp);
  return (uint64_t)tp.tv_sec * 1000000000 + ((uint64_t)tp.tv_nsec);
}
#endif

static void searchForRandomPathRecurse(SleefDFT *p, int level, int *path, uint64_t tm) {
  if (level == 0) {
    p->bestTime = tm;
    for(uint32_t j = 0;j < p->log2len+1;j++) p->bestPath[j] = path[j];
    return;
  }

  if (level < 1) return;

  do {
    int N;

    do {
      N = 1 + rand() % MAXBUTWIDTH;
    } while(p->tm[level*(MAXBUTWIDTH+1)+N] >= 1ULL << 60);

    path[level] = ((rand() & 1) == 0) ? N : -N;
    for(int j = level-1;j >= 0;j--) path[j] = 0;
    searchForRandomPathRecurse(p, level - N, path, 0);
  } while(p->bestTime >= 1ULL << 60);
}

static void searchForBestPathRecurse(SleefDFT *p, int level, int *path, uint64_t tm) {
  if (level == 0 && tm < p->bestTime) {
    p->bestTime = tm;
    for(uint32_t j = 0;j < p->log2len+1;j++) p->bestPath[j] = path[j];
    return;
  }

  if (level < 1 || tm >= p->bestTime) return;

  for(int N=MAXBUTWIDTH;N>0;N--) {
    if (p->tm[level*(MAXBUTWIDTH+1)+N] < p->tm2[level*(MAXBUTWIDTH+1)+N]) {
      path[level] = N;
      searchForBestPathRecurse(p, level - N, path, tm + p->tm[level*(MAXBUTWIDTH+1)+N]);
    } else {
      path[level] = -N;
      searchForBestPathRecurse(p, level - N, path, tm + p->tm2[level*(MAXBUTWIDTH+1)+N]);
    }
  }
  path[level] = 0;
}

static uint64_t estimate(int level, int N) {
  return N * 1000 + abs(N-3) * 1000;
}

static int measure(SleefDFT *p, int randomize) {
  SleefDFT_real *s = (SleefDFT_real *)memset(p->x0, 0, sizeof(SleefDFT_real) * (2 << p->log2len));
  SleefDFT_real *d = (SleefDFT_real *)memset(p->x1, 0, sizeof(SleefDFT_real) * (2 << p->log2len));

  for(uint32_t level=1;level<=p->log2len;level++) {
    for(int N=1;N<=MAXBUTWIDTH;N++) {
      p->tm[level*(MAXBUTWIDTH+1)+N] = 1ULL << 60;
      p->tm2[level*(MAXBUTWIDTH+1)+N] = 1ULL << 60;
    }
  }

  const int niter =  1 + 100000000 / (p->log2len << p->log2len);

  if (p->planMode != 0) {
    uint64_t tm = gettime();
    while(gettime()-tm < 100000000LL) ;
  }

  for(uint32_t level = p->log2len;level >= 1;level--) {
    for(uint32_t N=1;N<=MAXBUTWIDTH;N++) {
      if (level < N || p->log2len <= N) continue;
      if (level == N) {
	if ((int)p->log2len - (int)level < p->log2vecwidth) continue;
	if (!randomize && p->planMode != 0) {
	  const int pl = level >= p->log2len-2 ? 1 : 1;

	  p->tm[level*(MAXBUTWIDTH+1)+N] = 0;
	  for(int i=0;i<niter;i++) {
	    dispatch(p, pl, d, s, level+pl, 0);
	    uint64_t tm = gettime();
	    dispatch(p, N, d, s, level, 0);
	    p->tm[level*(MAXBUTWIDTH+1)+N] += gettime() - tm;
	  }

	  p->tm2[level*(MAXBUTWIDTH+1)+N] = 0;
	  for(int i=0;i<niter;i++) {
	    dispatch(p, pl, d, s, level+pl, 0);
	    uint64_t tm = gettime();
	    dispatch(p, N, d, s, level, 1);
	    p->tm2[level*(MAXBUTWIDTH+1)+N] += gettime() - tm;
	  }
	} else {
	  p->tm[level*(MAXBUTWIDTH+1)+N] = estimate(level, N);
	}
      } else if (level == p->log2len) {
	if (p->tbl[N] == NULL || p->tbl[N][level] == NULL) continue;
	if (p->vecwidth > (1 << N)) continue;
	if (!randomize && p->planMode != 0) {
	  for(int i0=0, i1=0;i0 < (1 << (p->log2len-N));i0+=p->vecwidth, i1++) {
	    p->perm[level][i1] = 2*perm(p->log2len, i0, p->log2len-level, p->log2len-(level-N));
	  }

	  uint64_t tm = gettime();
	  for(int i=0;i<niter;i++) {
	    dispatch(p, N, d, s, level, 0);
	  }
	  p->tm[level*(MAXBUTWIDTH+1)+N] = gettime() - tm;
	} else {
	  p->tm[level*(MAXBUTWIDTH+1)+N] = estimate(level, N);
	}
      } else {
	if (p->tbl[N] == NULL || p->tbl[N][level] == NULL) continue;
	if (p->vecwidth > 2 && p->log2len <= N+2) continue;
	if ((int)p->log2len - (int)level < p->log2vecwidth) continue;
	if (!randomize && p->planMode != 0) {
	  for(int i0=0, i1=0;i0 < (1 << (p->log2len-N));i0+=p->vecwidth, i1++) {
	    p->perm[level][i1] = 2*perm(p->log2len, i0, p->log2len-level, p->log2len-(level-N));
	  }
	  const int pl = level >= p->log2len-2 ? 1 : 1;

	  p->tm[level*(MAXBUTWIDTH+1)+N] = 0;
	  for(int i=0;i<niter;i++) {
	    dispatch(p, pl, d, s, level+pl, 0);
	    uint64_t tm = gettime();
	    dispatch(p, N, d, s, level, 0);
	    p->tm[level*(MAXBUTWIDTH+1)+N] += gettime() - tm;
	  }

	  p->tm2[level*(MAXBUTWIDTH+1)+N] = 0;
	  for(int i=0;i<niter;i++) {
	    dispatch(p, pl, d, s, level+pl, 0);
	    uint64_t tm = gettime();
	    dispatch(p, N, d, s, level, 1);
	    p->tm2[level*(MAXBUTWIDTH+1)+N] += gettime() - tm;
	  }
	} else {
	  p->tm[level*(MAXBUTWIDTH+1)+N] = estimate(level, N);
	}
      }
    }
  }

  int executable = 0;
  for(int i=1;i<=MAXBUTWIDTH && !executable;i++) {
    if (p->tm[p->log2len*(MAXBUTWIDTH+1)+i] < (1ULL << 60)) executable = 1;
  }

  if (!executable) return 0;

  p->bestPath = (int *)malloc(sizeof(int) * (p->log2len+1));
  p->bestTime = 1ULL << 60;

  int path[p->log2len+1];
  for(int j = p->log2len;j >= 0;j--) path[j] = 0;

  if (!randomize) {
    searchForBestPathRecurse(p, p->log2len, path, 0);
  } else {
    searchForRandomPathRecurse(p, p->log2len, path, 0);
  }

  p->pathLen = 0;
  for(int j = p->log2len;j >= 0;j--) if (p->bestPath[j] != 0) p->pathLen++;

  //for(int j = p->log2len;j >= 0;j--) if (p->bestPath[j] != 0) printf("%d ", p->bestPath[j]);
  //printf("\n");

  return 1;
}

static jmp_buf sigjmp;
static void sighandler(int signum) { longjmp(sigjmp, 1); }

static int checkISAAvailability(int isa) {
  signal(SIGILL, sighandler);

  if (setjmp(sigjmp) == 0) {
    int ret = getInt[isa] != NULL && (*getInt[isa])(0);
    signal(SIGILL, SIG_DFL);
    return ret;
  }

  signal(SIGILL, SIG_DFL);
  return 0;
}

SleefDFT *SleefDFT_init(uint64_t mode, uint32_t n) {
  SleefDFT *p = (SleefDFT *)calloc(1, sizeof(SleefDFT));
  p->magic = MAGIC;
  p->mode = mode;

  p->dataType  = (mode >> 5) & 15;
  p->dimension = ((mode >> 9) & 3) + 1;
  p->isa       = (mode >> 11) & 15;
  p->planMode  = (mode >> 15) &  7;

  if (p->isa == 0) {
    int bestPriority = -1;
    for(int i=0;i<ISAMAX;i++) {
      if (checkISAAvailability(i) && bestPriority < (*getInt[i])(2)) {
	bestPriority = (*getInt[i])(2);
	p->isa = i;
      }
    }
  } else {
    p->isa--;
    if (checkISAAvailability(p->isa)) {
      if ((p->mode & SLEEF_MODE_VERBOSE) != 0) printf("Specified ISA not available\n");
      p->magic = 0;
      free(p);
      return NULL;
    }
  }

  if ((mode & SLEEF_MODE_REAL) != 0) n /= 2;

  assert(n >= 4);

  if ((mode & SLEEF_MODE_ALT) != 0) p->mode = mode = mode ^ SLEEF_MODE_BACKWARD;
  int sign = (mode & SLEEF_MODE_BACKWARD) != 0 ? -1 : 1;
  p->log2len = ilog2(n);

  p->perm = (uint32_t **)calloc(sizeof(uint32_t *), p->log2len+1);
  for(int level = p->log2len;level >= 1;level--) {
    p->perm[level] = (uint32_t *)SleefDFT_malloc(sizeof(uint32_t) * ((1 << p->log2len) + 8));
  }

  p->x0 = (SleefDFT_real *)SleefDFT_malloc(sizeof(SleefDFT_real) * n * 2);
  p->x1 = (SleefDFT_real *)SleefDFT_malloc(sizeof(SleefDFT_real) * n * 2);

  if ((mode & SLEEF_MODE_REAL) != 0) {
    p->rtCoef0 = (SleefDFT_real *)SleefDFT_malloc(sizeof(SleefDFT_real) * n);
    p->rtCoef1 = (SleefDFT_real *)SleefDFT_malloc(sizeof(SleefDFT_real) * n);

    int ts = (mode & SLEEF_MODE_BACKWARD) != 0;

    for(uint32_t i=0;i<n/2;i++) {
      p->rtCoef0[i*2+0] = p->rtCoef0[i*2+1] = rtTable(ts, i*2+0, n);
      p->rtCoef1[i*2+0] = p->rtCoef1[i*2+1] = rtTable(ts, i*2+1, n);
    }
  }

  for(;;) {
    p->vecwidth = (*getInt[p->isa])(1);
    p->log2vecwidth = ilog2(p->vecwidth);

    for(int i=1;i<=MAXBUTWIDTH;i++) {
      p->tbl[i] = makeTable(sign, p->vecwidth, p->log2len, i, constK[i]);
    }

    if (measure(p, (mode & SLEEF_MODE_DEBUG))) break;

    if (p->isa == 0) { fprintf(stderr, "SleefDFT : Initialization failed.\n"); exit(-1); }

    freeTables(p);

    p->isa = 0;
  }

  for(int level = p->log2len;level >= 1;) {
    int N = abs(p->bestPath[level]);
    if (level == N) { level -= N; continue; }

    int i1 = 0;
    for(int i0=0;i0 < (1 << (p->log2len-N));i0+=p->vecwidth, i1++) {
      p->perm[level][i1] = 2*perm(p->log2len, i0, p->log2len-level, p->log2len-(level-N));
    }
    for(;i1 < (1 << p->log2len) + 8;i1++) p->perm[level][i1] = 0;

    level -= N;
  }  

  if ((p->mode & SLEEF_MODE_VERBOSE) != 0) printf("ISA : %s\n", (char *)(*getPtr[p->isa])(0));

  return p;
}

void SleefDFT_setPath(struct SleefDFT *p, char *pathStr) {
  assert(p != NULL && p->magic == MAGIC);

  int path[32];

  int pathLen = sscanf(pathStr, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d", 
		       &path[ 0], &path[ 1], &path[ 2], &path[ 3], &path[ 4], &path[ 5], &path[ 6], &path[ 7], 
		       &path[ 8], &path[ 9], &path[10], &path[11], &path[12], &path[13], &path[14], &path[15], 
		       &path[16], &path[17], &path[18], &path[19], &path[20], &path[21], &path[22], &path[23], 
		       &path[24], &path[25], &path[26], &path[27], &path[28], &path[29], &path[30], &path[13]);

  for(uint32_t j = 0;j <= p->log2len;j++) p->bestPath[j] = 0;

  for(int level = p->log2len, j=0;level > 0 && j < pathLen;) {
    p->bestPath[level] = level < abs(path[j]) ? level : path[j];
    level -= abs(path[j]);
    j++;
  }

  p->pathLen = 0;
  for(int j = p->log2len;j >= 0;j--) if (p->bestPath[j] != 0) p->pathLen++;

  if ((p->mode & SLEEF_MODE_VERBOSE) != 0) {
    printf("set path : ");
    for(int j = p->log2len;j >= 0;j--) if (p->bestPath[j] != 0) printf("%d ", p->bestPath[j]);
    printf("\n");
  }
}
