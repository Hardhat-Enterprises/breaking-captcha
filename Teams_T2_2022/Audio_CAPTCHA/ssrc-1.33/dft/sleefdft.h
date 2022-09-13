#ifndef __SLEEFDFT_H__
#define __SLEEFDFT_H__

#include <stdlib.h>
#include <stdint.h>

#define SLEEF_MODE_FORWARD     (0 <<  0)
#define SLEEF_MODE_BACKWARD    (1 <<  0)
#define SLEEF_MODE_COMPLEX     (0 <<  1)
#define SLEEF_MODE_REAL        (1 <<  1)
#define SLEEF_MODE_ALT         (1 <<  2)
#define SLEEF_MODE_DEBUG       (1 <<  3)
#define SLEEF_MODE_VERBOSE     (1 <<  4)
#define SLEEF_MODE_DOUBLE      (0 <<  5)
#define SLEEF_MODE_FLOAT       (1 <<  5)
#define SLEEF_MODE_LONGDOUBLE  (2 <<  5)
#define SLEEF_MODE_HALF        (3 <<  5)
#define SLEEF_MODE_DD          (4 <<  5)
#define SLEEF_MODE_QD          (5 <<  5)
#define SLEEF_MODE_DF          (6 <<  5)
#define SLEEF_MODE_QF          (7 <<  5)
#define SLEEF_MODE_1D          (0 <<  9)
#define SLEEF_MODE_2D          (1 <<  9)
#define SLEEF_MODE_3D          (2 <<  9)
#define SLEEF_MODE_AUTO        (0 << 11)
#define SLEEF_MODE_PUREC       (1 << 11)
#define SLEEF_MODE_X86SSE3     (2 << 11)
#define SLEEF_MODE_X86AVX      (3 << 11)
#define SLEEF_MODE_X86AVX3     (4 << 11)
#define SLEEF_MODE_ARMNEON     (2 << 11)
#define SLEEF_MODE_ESTIMATE    (0 << 15)
#define SLEEF_MODE_MEASURE     (1 << 15)
#define SLEEF_MODE_PATIENT     (2 << 15)
#define SLEEF_MODE_EXHAUSTIVE  (3 << 15)

#ifdef SLEEFDFT_REAL_IS_FLOAT
typedef float SleefDFT_real;
#else
typedef double SleefDFT_real;
#endif

struct SleefDFT *SleefDFT_init(uint64_t mode, uint32_t n);
void SleefDFT_execute(struct SleefDFT *ptr, SleefDFT_real *out, const SleefDFT_real *in);
void SleefDFT_executePremult(struct SleefDFT *ptr, SleefDFT_real *out, const SleefDFT_real *in, const SleefDFT_real *premult);

void SleefDFT_setPath(struct SleefDFT *ptr, char *pathStr);

void SleefDFT_dispose(struct SleefDFT *ptr);
void *SleefDFT_malloc(size_t z);
void SleefDFT_free(void *ptr);

#endif
