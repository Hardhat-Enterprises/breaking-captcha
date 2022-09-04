#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <assert.h>

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795028842
#endif

#include "prng.h"
#include "shapercoefs.h"

//

#define MAGIC 0x81d80d2b
#define RANDBUFLEN (1 << 16)

typedef struct SSRCDither {
  uint32_t magic;

  int fs;
  int shaperIndex;
  int pdfID;
  double noisePeak;

  int shaperClipMin, shaperClipMax;
  double peakBottom, peakTop;

  struct SleefRNG *rng;
  int randPtr;

  double buf[SHAPERLENMAX];
  double randBuf[RANDBUFLEN];
} SSRCDither;

//

#define PDF_RECTANGULAR 0
#define PDF_TRIANGULAR 1
#define PDF_GAUSSIAN 2
#define PDF_TWOLEVEL 3

SSRCDither *SSRCDither_init(int32_t fs, int32_t min, int32_t max, int shaperID, int pdfID, double noisePeak, uint64_t seed) {
  if (pdfID < 0 || 2 < pdfID) return NULL;

  int shaperIndex = -1;
  for(int i=0;ditherCoef[i].fs != -1;i++) {
    if (fs == ditherCoef[i].fs && shaperID == ditherCoef[i].id) {
      shaperIndex = i;
      break;
    }
  }
  if (shaperIndex == -1) return NULL;

  SSRCDither *thiz = (SSRCDither *)calloc(sizeof(SSRCDither), 1);
  thiz->magic = MAGIC;

  thiz->fs = fs;

  thiz->shaperClipMin = min;
  thiz->shaperClipMax = max;
  thiz->peakBottom = thiz->peakTop = (min + max)*0.5;
  thiz->shaperIndex = shaperIndex;

  thiz->pdfID = pdfID;
  thiz->noisePeak = noisePeak;

  thiz->rng = SleefRNG_init(seed);

  switch(thiz->pdfID) {
  case PDF_RECTANGULAR:
    SleefRNG_fillRectangularDouble(thiz->rng, thiz->randBuf, RANDBUFLEN, -thiz->noisePeak, thiz->noisePeak);
    break;
  case PDF_TRIANGULAR:
    SleefRNG_fillTriangularDouble(thiz->rng, thiz->randBuf, RANDBUFLEN, thiz->noisePeak);
    break;
  case PDF_TWOLEVEL:
    SleefRNG_fillTwoLevelDouble(thiz->rng, thiz->randBuf, RANDBUFLEN, thiz->noisePeak);
    break;
  }

  return thiz;
}

void SSRCDither_getPeaks(SSRCDither *thiz, double *peaks) {
  assert(thiz != NULL && thiz->magic == MAGIC);
  peaks[0] = thiz->peakTop;
  peaks[1] = thiz->peakBottom;
}

double SSRCDither_getMaxNoiseStrength(SSRCDither *thiz) {
  assert(thiz != NULL && thiz->magic == MAGIC);

  double sum = 0;
  for(int j=0;j<ditherCoef[thiz->shaperIndex].len;j++) {
    sum += fabs(ditherCoef[thiz->shaperIndex].coefs[j]);
  }

  return (thiz->noisePeak + 0.5) * sum + thiz->noisePeak;
}

void SSRCDither_dispose(SSRCDither *thiz) {
  assert(thiz != NULL && thiz->magic == MAGIC);

  SleefRNG_dispose(thiz->rng);

  thiz->magic = 0;
  free(thiz);
}

const int *SSRCDither_getAllSupportedFS() { return freqs; }

int SSRCDither_getNumAvailableShapers(int32_t fs) {
  int cnt = 0;

  for(int i=0;ditherCoef[i].fs != -1;i++) {
    if (fs == ditherCoef[i].fs) cnt++;
  }

  return cnt;
}

int SSRCDither_getAvailableShaperIDs(int *ids, int maxnids, int32_t fs) {
  int cnt = 0;

  for(int i=0;cnt < maxnids && ditherCoef[i].fs != -1;i++) {
    if (fs == ditherCoef[i].fs) {
      ids[cnt++] = ditherCoef[i].id;
    }
  }

  return cnt;
}

const char *SSRCDither_getNameForShaperID(int32_t fs, int id) {
  for(int i=0;ditherCoef[i].fs != -1;i++) {
    if (fs == ditherCoef[i].fs && id == ditherCoef[i].id) {
      return ditherCoef[i].friendlyName;
    }
  }
  return NULL;
}

double SSRCDither_getStrengthForShaperID(int fs, int id) {
  for(int i=0;ditherCoef[i].fs != -1;i++) {
    if (fs == ditherCoef[i].fs && id == ditherCoef[i].id) {
      double sum = 0;
      for(int j=0;j<ditherCoef[i].len;j++) {
	sum += fabs(ditherCoef[i].coefs[j]);
      }
      return sum;
    }
  }
  return -1;
}

#define RINT(x) ((x) >= 0 ? ((int)((x) + 0.5)) : ((int)((x) - 0.5)))

void SSRCDither_quantizeDouble(SSRCDither *thiz, int32_t *out, double *in, int32_t nSamples, double gain) {
  assert(thiz != NULL && thiz->magic == MAGIC);

  const double *shaperCoefs = ditherCoef[thiz->shaperIndex].coefs;
  const int shaperLen = ditherCoef[thiz->shaperIndex].len;
  double *buf = thiz->buf, *randBuf = thiz->randBuf;
  double min = thiz->shaperClipMin, max = thiz->shaperClipMax;
  double peakBottom = thiz->peakBottom, peakTop = thiz->peakTop;
  int randPtr = thiz->randPtr;

  for(int p=0;p<nSamples;p++) {
    double h = shaperCoefs[shaperLen-1]*buf[shaperLen-1];
    for(int i=shaperLen-2;i>=0;i--) {
      h += shaperCoefs[i]*buf[i];
      buf[i+1] = buf[i];
    }

    double x = gain * in[p] + h;
    double y = x + randBuf[randPtr];
    if (y < peakBottom) peakBottom = y;
    if (y > peakTop) peakTop = y;

    double q = RINT(y);
    randPtr++;

    if (q < min || q > max) {
      if (q < min) q = min;
      if (q > max) q = max;
      buf[0] = q - x;
      if (buf[0] < -1) buf[0] = -1;
      if (buf[0] >  1) buf[0] =  1;
    } else {
      buf[0] = q - x;
    }

    out[p] = (int)q;
    
    //

    if (randPtr == RANDBUFLEN) {
      switch(thiz->pdfID) {
      case PDF_RECTANGULAR:
	SleefRNG_fillRectangularDouble(thiz->rng, randBuf, RANDBUFLEN, -thiz->noisePeak, thiz->noisePeak);
	break;
      case PDF_TRIANGULAR:
	SleefRNG_fillTriangularDouble(thiz->rng, randBuf, RANDBUFLEN, thiz->noisePeak);
	break;
      case PDF_TWOLEVEL:
	SleefRNG_fillTwoLevelDouble(thiz->rng, randBuf, RANDBUFLEN, thiz->noisePeak);
	break;
      }
      randPtr = 0;
    }
  }

  thiz->randPtr    = randPtr;
  thiz->peakTop    = peakTop;
  thiz->peakBottom = peakBottom;
}
