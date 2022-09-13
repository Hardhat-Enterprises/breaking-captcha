#ifndef __SIMD_H__
#define __SIMD_H__

#if defined(__x86_64__) || defined(__i386__)
#include <x86intrin.h>
#endif

#if (defined(__arm__) || defined(__aarch64__)) && defined(__ARM_NEON__)
#include <arm_neon.h>
#endif

#include <stdint.h>

#include "sleefdft.h"

#define INLINE __attribute__((always_inline))

#if defined(PUREC_DOUBLE)
typedef double real;

#define VECWIDTH 1
#define LOG2VECWIDTH 0
#define PRIORITY 0
#define ISANAME "Pure C double"

typedef struct real2 {
  real x, y;
} real2;

static int available() { return 1; }

static INLINE real2 load(const real *ptr, int offset) { return ((real2 *)ptr)[offset]; }
static INLINE void store(real *ptr, int offset, real2 v) { ((real2 *)ptr)[offset] = v; }
static INLINE real2 loadu(const real *ptr, int offset) { return ((real2 *)ptr)[offset]; }
static INLINE void storeu(real *ptr, int offset, real2 v) { ((real2 *)ptr)[offset] = v; }

static INLINE void prefetch(real *ptr, int offset) {}
static INLINE void stream(real *ptr, int offset, real2 v) { ((real2 *)ptr)[offset] = v; }

static INLINE real2 r2(real x, real y) { real2 r; r.x = x; r.y = y; return r; }
static INLINE real2 loadc(real c) { return r2(c, c); }

static INLINE void scatter(real *ptr, int offset, int shift, real2 v) { ((real2 *)ptr)[offset] = v; }
static INLINE void scstream(real *ptr, int offset, int shift, real2 v) { ((real2 *)ptr)[offset] = v; }

static INLINE real2 plus(real2 d0, real2 d1) { return r2(d0.x+d1.x, d0.y+d1.y); }
static INLINE real2 minus(real2 d0, real2 d1) { return r2(d0.x-d1.x, d0.y-d1.y); }
static INLINE real2 uminus(real2 d0) { return r2(-d0.x, -d0.y); }

static INLINE real2 plusminus(real2 d0, real2 d1) { return r2(d0.x+d1.x, d0.y-d1.y); }
static INLINE real2 minusplus(real2 d0, real2 d1) { return r2(d0.x-d1.x, d0.y+d1.y); }
static INLINE real2 uplusminus(real2 d0) { return r2( d0.x, -d0.y); }
static INLINE real2 uminusplus(real2 d0) { return r2(-d0.x,  d0.y); }

static INLINE real2 times(real2 d0, real2 d1) { return r2(d0.x*d1.x, d0.y*d1.y); }
static INLINE real2 ctimes(real2 d0, real d) { return r2(d0.x*d, d0.y*d); }

static INLINE real2 reverse(real2 d0) { return r2(d0.y, d0.x); }
static INLINE real2 reverse2(real2 d0) { return d0; }
#elif defined(PUREC_FLOAT)
typedef float real;

#define VECWIDTH 1
#define LOG2VECWIDTH 0
#define PRIORITY 0
#define ISANAME "Pure C float"

typedef struct real2 {
  real x, y;
} real2;

static int available() { return 1; }

static INLINE real2 load(const real *ptr, int offset) { return ((real2 *)ptr)[offset]; }
static INLINE void store(real *ptr, int offset, real2 v) { ((real2 *)ptr)[offset] = v; }
static INLINE real2 loadu(const real *ptr, int offset) { return ((real2 *)ptr)[offset]; }
static INLINE void storeu(real *ptr, int offset, real2 v) { ((real2 *)ptr)[offset] = v; }

static INLINE void prefetch(real *ptr, int offset) {}
static INLINE void stream(real *ptr, int offset, real2 v) { ((real2 *)ptr)[offset] = v; }

static INLINE real2 r2(real x, real y) { real2 r; r.x = x; r.y = y; return r; }
static INLINE real2 loadc(real c) { return r2(c, c); }

static INLINE void scatter(real *ptr, int offset, int shift, real2 v) { ((real2 *)ptr)[offset] = v; }
static INLINE void scstream(real *ptr, int offset, int shift, real2 v) { ((real2 *)ptr)[offset] = v; }

static INLINE real2 plus(real2 d0, real2 d1) { return r2(d0.x+d1.x, d0.y+d1.y); }
static INLINE real2 minus(real2 d0, real2 d1) { return r2(d0.x-d1.x, d0.y-d1.y); }
static INLINE real2 uminus(real2 d0) { return r2(-d0.x, -d0.y); }

static INLINE real2 plusminus(real2 d0, real2 d1) { return r2(d0.x+d1.x, d0.y-d1.y); }
static INLINE real2 minusplus(real2 d0, real2 d1) { return r2(d0.x-d1.x, d0.y+d1.y); }
static INLINE real2 uplusminus(real2 d0) { return r2( d0.x, -d0.y); }
static INLINE real2 uminusplus(real2 d0) { return r2(-d0.x,  d0.y); }

static INLINE real2 times(real2 d0, real2 d1) { return r2(d0.x*d1.x, d0.y*d1.y); }
static INLINE real2 ctimes(real2 d0, real d) { return r2(d0.x*d, d0.y*d); }

static INLINE real2 reverse(real2 d0) { return r2(d0.y, d0.x); }
static INLINE real2 reverse2(real2 d0) { return d0; }
#elif defined(SSE3_DOUBLE)
typedef double real;

#define VECWIDTH 1
#define LOG2VECWIDTH 0
#define PRIORITY 1
#define ISANAME "x86 SSE3 double"

#include "sse.h"

typedef vdouble real2;

static int available() { return __builtin_cpu_supports("sse3"); }

static INLINE real2 minusplus(real2 d0, real2 d1) { return _mm_addsub_pd(d0, d1); }
static INLINE real2 uplusminus(real2 d0) { return _mm_xor_pd(d0, _mm_set_pd(-0.0, +0.0)); }
static INLINE real2 uminusplus(real2 d0) { return _mm_xor_pd(d0, _mm_set_pd(+0.0, -0.0)); }
static INLINE real2 reverse(real2 d0) { return _mm_shuffle_pd(d0, d0, 1); }
static INLINE real2 reverse2(real2 d0) { return d0; }

static INLINE real2 load(const real *ptr, int offset) { return vload_vd_p(&ptr[2*offset]); }
static INLINE void store(real *ptr, int offset, real2 v) { vstore_p_vd(&ptr[2*offset], v); }
static INLINE real2 loadu(const real *ptr, int offset) { return vloadu_vd_p(&ptr[2*offset]); }
static INLINE void storeu(real *ptr, int offset, real2 v) { vstoreu_p_vd(&ptr[2*offset], v); }
static INLINE real2 loadc(real c) { return vcast_vd_d(c); }

static INLINE void prefetch(real *ptr, int offset) { _mm_prefetch((char const *)&ptr[2*offset], _MM_HINT_T0); }
static INLINE void stream(real *ptr, int offset, real2 v) { _mm_stream_pd(&ptr[2*offset], v); }

static INLINE real2 plus(real2 d0, real2 d1) { return vadd_vd_vd_vd(d0, d1); }
static INLINE real2 minus(real2 d0, real2 d1) { return vsub_vd_vd_vd(d0, d1); }

static INLINE real2 uminus(real2 d0) { return vneg_vd_vd(d0); }

static INLINE real2 times(real2 d0, real2 d1) { return vmul_vd_vd_vd(d0, d1); }
static INLINE real2 ctimes(real2 d0, real d) { return vmul_vd_vd_vd(d0, vcast_vd_d(d)); }

static INLINE void scatter(real *ptr, int offset, int shift, real2 v) { vstore_p_vd((real *)(&ptr[2*offset]), v); }
static INLINE void scstream(real *ptr, int offset, int shift, real2 v) { _mm_stream_pd((real *)(&ptr[2*offset]), v); }

#elif defined(SSE3_FLOAT)
typedef float real;

#define VECWIDTH 2
#define LOG2VECWIDTH 1
#define PRIORITY 1
#define ISANAME "x86 SSE3 float"

#include "sse.h"

typedef vfloat real2;

static int available() { return __builtin_cpu_supports("sse3"); }

static INLINE real2 minusplus(real2 d0, real2 d1) { return _mm_addsub_ps(d0, d1); }
static INLINE real2 uplusminus(real2 d0) { return _mm_xor_ps(d0, _mm_set_ps(-0.0f, +0.0f, -0.0f, +0.0f)); }
static INLINE real2 uminusplus(real2 d0) { return _mm_xor_ps(d0, _mm_set_ps(+0.0f, -0.0f, +0.0f, -0.0f)); }
static INLINE real2 reverse(real2 d0)  { return _mm_shuffle_ps(d0, d0, (2 << 6) | (3 << 4) | (0 << 2) | (1 << 0)); }
static INLINE real2 reverse2(real2 d0) { return _mm_shuffle_ps(d0, d0, (1 << 6) | (0 << 4) | (3 << 2) | (2 << 0)); }

static INLINE real2 load(const real *ptr, int offset) { return vload_vf_p(&ptr[2*offset]); }
static INLINE void store(real *ptr, int offset, real2 v) { vstore_p_vf(&ptr[2*offset], v); }
static INLINE real2 loadu(const real *ptr, int offset) { return vloadu_vf_p(&ptr[2*offset]); }
static INLINE void storeu(real *ptr, int offset, real2 v) { vstoreu_p_vf(&ptr[2*offset], v); }
static INLINE void prefetch(real *ptr, int offset) { _mm_prefetch((char const *)&ptr[2*offset], _MM_HINT_T0); }
static INLINE real2 loadc(real c) { return vcast_vf_f(c); }

static INLINE void stream(real *ptr, int offset, real2 v) { _mm_stream_ps(&ptr[2*offset], v); }

static INLINE real2 plus(real2 d0, real2 d1) { return vadd_vf_vf_vf(d0, d1); }
static INLINE real2 minus(real2 d0, real2 d1) { return vsub_vf_vf_vf(d0, d1); }

static INLINE real2 uminus(real2 d0) { return vneg_vf_vf(d0); }

static INLINE real2 times(real2 d0, real2 d1) { return vmul_vf_vf_vf(d0, d1); }
static INLINE real2 ctimes(real2 d0, real d) { return vmul_vf_vf_vf(d0, vcast_vf_f(d)); }

static INLINE void scatter(real *ptr, int offset, const int step, real2 v) { // !!
  _mm_storel_pd((double *)(ptr+(offset + step * 0)*2), (__m128d)v);
  _mm_storeh_pd((double *)(ptr+(offset + step * 1)*2), (__m128d)v);
}

static INLINE void scstream(real *ptr, int offset, const int step, real2 v) {
  _mm_storel_pd((double *)(ptr+(offset + step * 0)*2), (__m128d)v);
  _mm_storeh_pd((double *)(ptr+(offset + step * 1)*2), (__m128d)v);
}
#elif defined(AVX_DOUBLE)
typedef double real;

#include "avx.h"

#define VECWIDTH 2
#define LOG2VECWIDTH 1
#define PRIORITY 2
#define ISANAME "x86 AVX double"

typedef vdouble real2;

//

static INLINE void scatter(real *ptr, int offset, const int step, real2 v) {
  _mm_store_pd(&ptr[(offset + step * 0)*2], _mm256_extractf128_pd(v, 0));
  _mm_store_pd(&ptr[(offset + step * 1)*2], _mm256_extractf128_pd(v, 1));
}

static INLINE void scstream(real *ptr, int offset, const int step, real2 v) {
  _mm_stream_pd(&ptr[(offset + step * 0)*2], _mm256_extractf128_pd(v, 0));
  _mm_stream_pd(&ptr[(offset + step * 1)*2], _mm256_extractf128_pd(v, 1));
}

static int available() { return __builtin_cpu_supports("avx"); }

static INLINE real2 minusplus(real2 d0, real2 d1) { return _mm256_addsub_pd(d0, d1); }
static INLINE real2 uplusminus(real2 d0) { return _mm256_xor_pd(d0, _mm256_set_pd(-0.0, +0.0, -0.0, +0.0)); }
static INLINE real2 uminusplus(real2 d0) { return _mm256_xor_pd(d0, _mm256_set_pd(+0.0, -0.0, +0.0, -0.0)); }
static INLINE real2 reverse(real2 d0) { return  _mm256_shuffle_pd(d0, d0, (0 << 3) | (1 << 2) | (0 << 1) | (1 << 0)); }
static INLINE real2 reverse2(real2 d0) { d0 = _mm256_permute2f128_pd(d0, d0, 1); return _mm256_shuffle_pd(d0, d0, (1 << 3) | (0 << 2) | (1 << 1) | (0 << 0)); };

//

static INLINE real2 load(const real *ptr, int offset) { return vload_vd_p(&ptr[2*offset]); }
static INLINE void store(real *ptr, int offset, real2 v) { vstore_p_vd(&ptr[2*offset], v); }
static INLINE real2 loadu(const real *ptr, int offset) { return vloadu_vd_p(&ptr[2*offset]); }
static INLINE void storeu(real *ptr, int offset, real2 v) { vstoreu_p_vd(&ptr[2*offset], v); }
static INLINE real2 loadc(real c) { return vcast_vd_d(c); }

static INLINE void stream(real *ptr, int offset, real2 v) { _mm256_stream_pd(&ptr[2*offset], v); }
static INLINE void prefetch(real *ptr, int offset) { _mm_prefetch((char const *)&ptr[2*offset], _MM_HINT_T0); }

static INLINE real2 plus(real2 d0, real2 d1) { return vadd_vd_vd_vd(d0, d1); }
static INLINE real2 minus(real2 d0, real2 d1) { return vsub_vd_vd_vd(d0, d1); }

static INLINE real2 uminus(real2 d0) { return vneg_vd_vd(d0); }

static INLINE real2 times(real2 d0, real2 d1) { return vmul_vd_vd_vd(d0, d1); }
static INLINE real2 ctimes(real2 d0, real d) { return vmul_vd_vd_vd(d0, vcast_vd_d(d)); }
#elif defined(AVX_FLOAT)
typedef float real;

#define VECWIDTH 4
#define LOG2VECWIDTH 2
#define PRIORITY 2
#define ISANAME "x86 AVX float"

#include "avx.h"

typedef vfloat real2;

static int available() { return __builtin_cpu_supports("avx"); }

static INLINE real2 minusplus(real2 d0, real2 d1) { return _mm256_addsub_ps(d0, d1); }
static INLINE real2 uplusminus(real2 d0) { return _mm256_xor_ps(d0, _mm256_set_ps(-0.0f, +0.0f, -0.0f, +0.0f, -0.0f, +0.0f, -0.0f, +0.0f)); }
static INLINE real2 uminusplus(real2 d0) { return _mm256_xor_ps(d0, _mm256_set_ps(+0.0f, -0.0f, +0.0f, -0.0f, +0.0f, -0.0f, +0.0f, -0.0f)); }
static INLINE real2 reverse(real2 d0) { return _mm256_shuffle_ps(d0, d0, (2 << 6) | (3 << 4) | (0 << 2) | (1 << 0)); }
static INLINE real2 reverse2(real2 d0) { d0 = _mm256_permute2f128_ps(d0, d0, 1); return _mm256_shuffle_ps(d0, d0, (1 << 6) | (0 << 4) | (3 << 2) | (2 << 0)); }

static INLINE real2 load(const real *ptr, int offset) { return vload_vf_p(&ptr[2*offset]); }
static INLINE void store(real *ptr, int offset, real2 v) { vstore_p_vf(&ptr[2*offset], v); }
static INLINE real2 loadu(const real *ptr, int offset) { return vloadu_vf_p(&ptr[2*offset]); }
static INLINE void storeu(real *ptr, int offset, real2 v) { vstoreu_p_vf(&ptr[2*offset], v); }
static INLINE void prefetch(real *ptr, int offset) { _mm_prefetch((char const *)&ptr[2*offset], _MM_HINT_T0); }
static INLINE real2 loadc(real c) { return vcast_vf_f(c); }

static INLINE void stream(real *ptr, int offset, real2 v) { _mm256_stream_ps(&ptr[2*offset], v); }

static INLINE real2 plus(real2 d0, real2 d1) { return vadd_vf_vf_vf(d0, d1); }
static INLINE real2 minus(real2 d0, real2 d1) { return vsub_vf_vf_vf(d0, d1); }

static INLINE real2 uminus(real2 d0) { return vneg_vf_vf(d0); }

static INLINE real2 times(real2 d0, real2 d1) { return vmul_vf_vf_vf(d0, d1); }
static INLINE real2 ctimes(real2 d0, real d) { return vmul_vf_vf_vf(d0, vcast_vf_f(d)); }

static INLINE void scatter(real *ptr, int offset, const int step, real2 v) {
  _mm_storel_pd((double *)(ptr+(offset + step * 0)*2), (__m128d)_mm256_extractf128_ps(v, 0));
  _mm_storeh_pd((double *)(ptr+(offset + step * 1)*2), (__m128d)_mm256_extractf128_ps(v, 0));
  _mm_storel_pd((double *)(ptr+(offset + step * 2)*2), (__m128d)_mm256_extractf128_ps(v, 1));
  _mm_storeh_pd((double *)(ptr+(offset + step * 3)*2), (__m128d)_mm256_extractf128_ps(v, 1));
}

static INLINE void scstream(real *ptr, int offset, const int step, real2 v) {
  _mm_storel_pd((double *)(ptr+(offset + step * 0)*2), (__m128d)_mm256_extractf128_ps(v, 0));
  _mm_storeh_pd((double *)(ptr+(offset + step * 1)*2), (__m128d)_mm256_extractf128_ps(v, 0));
  _mm_storel_pd((double *)(ptr+(offset + step * 2)*2), (__m128d)_mm256_extractf128_ps(v, 1));
  _mm_storeh_pd((double *)(ptr+(offset + step * 3)*2), (__m128d)_mm256_extractf128_ps(v, 1));
}
#elif defined(NEON_FLOAT)
typedef float real;

#define VECWIDTH 2
#define LOG2VECWIDTH 1
#define PRIORITY 1
#define ISANAME "ARM NEON float"

#include "neon.h"

typedef vfloat real2;

static int available() {
  static real2 detectionTmp;
  detectionTmp = vadd_vf_vf_vf(detectionTmp, detectionTmp);
  return 1;
}

static INLINE real2 uplusminus(real2 d0) {
  static const float32_t c[] = { +0.0f, -0.0f, +0.0f, -0.0f };
  return (vfloat)vxor_vm_vm_vm((vmask)vload_vf_p(c), (vmask)d0);
}

static INLINE real2 uminusplus(real2 d0) {
  static const float32_t c[] = { -0.0f, +0.0f, -0.0f, +0.0f };
  return (vfloat)vxor_vm_vm_vm((vmask)vload_vf_p(c), (vmask)d0);
}

static INLINE real2 minusplus(real2 d0, real2 d1) { return vadd_vf_vf_vf(d0, uminusplus(d1)); }
static INLINE real2 reverse(real2 d0)  { return vrev64q_f32(d0); }
static INLINE real2 reverse2(real2 d0) { return vcombine_f32(vget_high_f32(d0), vget_low_f32(d0)); }

static INLINE real2 load (const real *ptr, int offset) { return vload_vf_p(&ptr[2*offset]); }
static INLINE real2 loadu(const real *ptr, int offset) { return vloadu_vf_p(&ptr[2*offset]); }
static INLINE void store (real *ptr, int offset, real2 v) { vstore_p_vf(&ptr[2*offset], v); }
static INLINE void storeu(real *ptr, int offset, real2 v) { vstoreu_p_vf(&ptr[2*offset], v); }
static INLINE void prefetch(real *ptr, int offset) { }
static INLINE real2 loadc(real c) { return vcast_vf_f(c); }

static INLINE void stream(real *ptr, int offset, real2 v) { vstore_p_vf(&ptr[2*offset], v); }

static INLINE real2 plus(real2 d0, real2 d1) { return vadd_vf_vf_vf(d0, d1); }
static INLINE real2 minus(real2 d0, real2 d1) { return vsub_vf_vf_vf(d0, d1); }

static INLINE real2 uminus(real2 d0) { return vneg_vf_vf(d0); }

static INLINE real2 times(real2 d0, real2 d1) { return vmul_vf_vf_vf(d0, d1); }
static INLINE real2 ctimes(real2 d0, real d) { return vmul_vf_vf_vf(d0, vcast_vf_f(d)); }

static INLINE void scatter(real *ptr, int offset, const int step, real2 v) {
  vst1_f32((float *)(ptr+(offset + step * 0)*2), vget_low_f32(v));
  vst1_f32((float *)(ptr+(offset + step * 1)*2), vget_high_f32(v));
}

static INLINE void scstream(real *ptr, int offset, const int step, real2 v) {
  vst1_f32((float *)(ptr+(offset + step * 0)*2), vget_low_f32(v));
  vst1_f32((float *)(ptr+(offset + step * 1)*2), vget_high_f32(v));
}
#elif defined(NEON_DOUBLE)
typedef double real;

#define VECWIDTH 1
#define LOG2VECWIDTH 0
#define PRIORITY 1
#define ISANAME "ARM NEON double"

#include "neon.h"

typedef vdouble real2;

static int available() { return 1; }

static INLINE real2 uplusminus(real2 d0) {
  static const double c[] = { +0.0, -0.0 };
  return (vdouble)vxor_vm_vm_vm((vmask)vload_vd_p(c), (vmask)d0);
}

static INLINE real2 uminusplus(real2 d0) {
  static const double c[] = { -0.0, +0.0 };
  return (vdouble)vxor_vm_vm_vm((vmask)vload_vd_p(c), (vmask)d0);
}

static INLINE real2 minusplus(real2 d0, real2 d1) { return vadd_vd_vd_vd(d0, uminusplus(d1)); }
static INLINE real2 reverse(real2 d0)  { return vcombine_f64(vget_high_f64(d0), vget_low_f64(d0)); }
static INLINE real2 reverse2(real2 d0) { return d0; }

static INLINE real2 load (const real *ptr, int offset) { return vload_vd_p(&ptr[2*offset]); }
static INLINE real2 loadu(const real *ptr, int offset) { return vloadu_vd_p(&ptr[2*offset]); }
static INLINE void store (real *ptr, int offset, real2 v) { vstore_p_vd(&ptr[2*offset], v); }
static INLINE void storeu(real *ptr, int offset, real2 v) { vstoreu_p_vd(&ptr[2*offset], v); }
static INLINE void prefetch(real *ptr, int offset) { }
static INLINE real2 loadc(real c) { return vcast_vd_d(c); }

static INLINE void stream(real *ptr, int offset, real2 v) { vstore_p_vd(&ptr[2*offset], v); }

static INLINE real2 plus(real2 d0, real2 d1) { return vadd_vd_vd_vd(d0, d1); }
static INLINE real2 minus(real2 d0, real2 d1) { return vsub_vd_vd_vd(d0, d1); }

static INLINE real2 uminus(real2 d0) { return vneg_vd_vd(d0); }

static INLINE real2 times(real2 d0, real2 d1) { return vmul_vd_vd_vd(d0, d1); }
static INLINE real2 ctimes(real2 d0, real d) { return vmul_vd_vd_vd(d0, vcast_vd_d(d)); }

static INLINE void scatter(real *ptr, int offset, const int step, real2 v) {
  vstore_p_vd((real *)(&ptr[2*offset]), v);
}

static INLINE void scstream(real *ptr, int offset, const int step, real2 v) {
  vstore_p_vd((real *)(&ptr[2*offset]), v);
}
#else
#error No ISA specified
#endif

#endif
