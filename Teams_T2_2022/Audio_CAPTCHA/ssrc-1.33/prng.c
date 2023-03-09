// PRNG for SSRC, written by Naoki Shibata ( shibatch@users.sourceforge.net )
// This PRNG passes Big Crush TestU01. Period length = 2^128

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <float.h>
#include <assert.h>

//

typedef struct u128 {
  uint64_t l, u;
} u128;

static unsigned int add64c(uint64_t *ret, uint64_t *u, uint64_t *v) {
  uint64_t w;
  int c;

  w = *u + *v;
  c = w < *u;
  *ret = w;

  return c;
}

static void add128(u128 *r, u128 *u, u128 *v) {
  uint64_t rl, ru;

  ru = u->u + v->u + add64c(&rl, &u->l, &v->l);

  r->l = rl;
  r->u = ru;
}

static u128 nextState(u128 state) {
  u128 ret, u;
  u.u = 0xE7866D8CFFF116AAULL;
  u.l = 0xA933E07E1CB7963DULL;
  add128(&ret, &state, &u);
  return ret;
}

//

#define MAGIC 0xFEEDBABE

typedef struct SleefRNG {
  uint32_t magic;
  u128 state;

  uint64_t remain;
  int remainSize;
} SleefRNG;

SleefRNG *SleefRNG_init(uint64_t seed) {
  SleefRNG *thiz = (SleefRNG *)calloc(sizeof(SleefRNG), 1);
  thiz->magic = MAGIC;

  thiz->state.l = 0xAD62418D14EA8247ULL * seed + 0x01C4B4886CC66F59ULL;
  thiz->state.u = 0xFFD1390A0ADC2FB8ULL * seed + 0xDABBB8174D95C99BULL;

  return thiz;
}

void SleefRNG_dispose(SleefRNG *thiz) {
  assert(thiz->magic == MAGIC);
  thiz->magic = 0;
  free(thiz);
}

#define C0 0xf3dd0fb7820fde37ULL
#define C1 0xe6c6ac2c59e52811ULL
#define C2 0x2fc7871fff7c5b45ULL
#define C3 0x47c7e1f70aa4f7c5ULL
#define C4 0x094f02b7fb9ba895ULL
#define C5 0x89afda817e744570ULL
#define C6 0xc7277d052c7bf14bULL
#define C7 0x474f4433b10b081dULL
#define C8 0xa0a543d9f16196a5ULL
#define C9 0x06dce455629a8955ULL

static uint64_t xrandom64(SleefRNG *thiz) {
  assert(thiz->magic == MAGIC);
  int n;
  thiz->state = nextState(thiz->state);
  uint64_t s = thiz->state.u ^ thiz->state.l, t = s;
  n = (s >> 24) & 63; t = ((t << n) | (t >> (64 - n))); t ^= C0;
  n = (s >>  6) & 63; t = ((t << n) | (t >> (64 - n))); t ^= C1;
  n = (s >> 18) & 63; t = ((t << n) | (t >> (64 - n))); t ^= C2;
  n = (s >> 48) & 63; t = ((t << n) | (t >> (64 - n))); t ^= C3;
  n = (s >>  0) & 63; t = ((t << n) | (t >> (64 - n))); t ^= C4;
  n = (s >> 12) & 63; t = ((t << n) | (t >> (64 - n))); t ^= C5;
  n = (s >> 36) & 63; t = ((t << n) | (t >> (64 - n))); t ^= C6;
  n = (s >> 54) & 63; t = ((t << n) | (t >> (64 - n))); t ^= C7;
  n = (s >> 28) & 63; t = ((t << n) | (t >> (64 - n))); t ^= C8;
  n = (s >> 10) & 63; t = ((t << n) | (t >> (64 - n))); t ^= C9;
  return t;
}

//

uint64_t SleefRNG_next(SleefRNG *thiz, int nbits) {
  assert(thiz->magic == MAGIC);

  if (nbits > thiz->remainSize) {
    thiz->remain = xrandom64(thiz);
    thiz->remainSize = 64;
  }

  uint64_t ret = thiz->remain & ~(-1LL << nbits);
  thiz->remain >>= nbits;
  thiz->remainSize -= nbits;
  return ret;
}

uint64_t SleefRNG_next64(SleefRNG *thiz) {
  assert(thiz->magic == MAGIC);
  return xrandom64(thiz);
}

void SleefRNG_nextBytes(SleefRNG *thiz, uint8_t *ptr, size_t z) {
  assert(thiz->magic == MAGIC);

  size_t i;
  for(i=0;i<(z & ~7);i+=8) {
    *((uint64_t *)&(ptr[i])) = xrandom64(thiz);
  }

  for(;i<z;i++) {
    ptr[i] = SleefRNG_next(thiz, 8);
  }
}

double SleefRNG_nextDouble(SleefRNG *thiz) {
  assert(thiz->magic == MAGIC);
  return (double)xrandom64(thiz) * (1.0 / (1ULL << 32) / (1ULL << 32));
}

double SleefRNG_nextRectangularDouble(SleefRNG *thiz, double min, double max) {
  assert(thiz->magic == MAGIC && max >= min);
  return min + (double)xrandom64(thiz) * (1.0 / (1ULL << 32) / (1ULL << 32)) * (max-min);
}

void SleefRNG_fillRectangularDouble(SleefRNG *thiz, double *ptr, size_t z, double min, double max) {
  assert(thiz->magic == MAGIC && max >= min);
  size_t i;
  for(i=0;i<z;i++) {
    ptr[i] = min + (double)xrandom64(thiz) * (1.0 / (1ULL << 32) / (1ULL << 32)) * (max-min);
  }
}

double SleefRNG_nextTriangularDouble(SleefRNG *thiz, double peak) {
  assert(thiz->magic == MAGIC);
  return ((double)xrandom64(thiz) - (double)xrandom64(thiz)) * (1.0 / (1ULL << 32) / (1ULL << 32)) * peak;
}

void SleefRNG_fillTriangularDouble(SleefRNG *thiz, double *ptr, size_t z, double peak) {
  assert(thiz->magic == MAGIC);
  size_t i;
  for(i=0;i<z;i++) {
    ptr[i] = ((double)xrandom64(thiz) - (double)xrandom64(thiz)) * (1.0 / (1ULL << 32) / (1ULL << 32)) * peak;
  }
}

double SleefRNG_nextTwoLevelDouble(SleefRNG *thiz, double peak) {
  assert(thiz->magic == MAGIC);
  return (SleefRNG_next(thiz, 1) != 0) ? -peak : peak;
}

void SleefRNG_fillTwoLevelDouble(SleefRNG *thiz, double *ptr, size_t z, double peak) {
  assert(thiz->magic == MAGIC);
  size_t i;
  for(i=0;i<z;i++) {
    ptr[i] = (SleefRNG_next(thiz, 1) != 0) ? -peak : peak;
  }
}

//

#if 0
#include "bbattery.h"

struct SleefRNG *rng;

unsigned long getbits (void *param, void *state) { return SleefRNG_next(rng, 32); }
double getu01(void *param, void *state) { return SleefRNG_nextDouble(rng); }
void writestate(void *state) {}

int main (int argc, char **argv)
{
  unif01_Gen *gen = calloc(1, sizeof(unif01_Gen));

  gen->state = NULL;
  gen->param = NULL;
  gen->name = "xorrotate";
  gen->GetU01 = getu01;
  gen->GetBits = getbits;
  gen->Write = writestate;

  rng = SleefRNG_init(atoi(argv[1]));

  for(;;) bbattery_BigCrush(gen);

  return 0;
}
#endif
