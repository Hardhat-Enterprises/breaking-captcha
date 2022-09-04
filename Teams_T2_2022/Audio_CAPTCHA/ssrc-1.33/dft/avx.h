#ifndef __AVX__
#error Please specify -mavx.
#endif

#include <immintrin.h>
#include <stdint.h>

#include "nonnumber.h"

typedef __m256d vdouble;
typedef __m128i vint;
typedef __m256i vmask;

typedef __m256 vfloat;
typedef struct { vint x, y; } vint2;

//

static INLINE vdouble vload_vd_p(const double *ptr) { return _mm256_load_pd(ptr); }
static INLINE vdouble vloadu_vd_p(const double *ptr) { return _mm256_loadu_pd(ptr); }

static INLINE void vstore_p_vd(double *ptr, vdouble v) { _mm256_store_pd(ptr, v); }
static INLINE void vstoreu_p_vd(double *ptr, vdouble v) { _mm256_storeu_pd(ptr, v); }

static INLINE vfloat vload_vf_p(const float *ptr) { return _mm256_load_ps(ptr); }
static INLINE vfloat vloadu_vf_p(const float *ptr) { return _mm256_loadu_ps(ptr); }

static INLINE void vstore_p_vf(float *ptr, vfloat v) { _mm256_store_ps(ptr, v); }
static INLINE void vstoreu_p_vf(float *ptr, vfloat v) { _mm256_storeu_ps(ptr, v); }

//

static INLINE vint vrint_vi_vd(vdouble vd) { return _mm256_cvtpd_epi32(vd); }
static INLINE vint vtruncate_vi_vd(vdouble vd) { return _mm256_cvttpd_epi32(vd); }
static INLINE vdouble vcast_vd_vi(vint vi) { return _mm256_cvtepi32_pd(vi); }
static INLINE vdouble vcast_vd_d(double d) { return _mm256_set1_pd(d); }
static INLINE vint vcast_vi_i(int i) { return _mm_set_epi32(i, i, i, i); }

static INLINE vmask vreinterpret_vm_vd(vdouble vd) { return (__m256i)vd; }
static INLINE vdouble vreinterpret_vd_vm(vmask vm) { return (__m256d)vm;  }

static INLINE vmask vreinterpret_vm_vf(vfloat vf) { return (__m256i)vf; }
static INLINE vfloat vreinterpret_vf_vm(vmask vm) { return (__m256)vm; }

//

static INLINE vint vadd_vi_vi_vi(vint x, vint y) { return _mm_add_epi32(x, y); }
static INLINE vint vsub_vi_vi_vi(vint x, vint y) { return _mm_sub_epi32(x, y); }
static INLINE vint vneg_vi_vi(vint e) { return vsub_vi_vi_vi(vcast_vi_i(0), e); }

static INLINE vint vand_vi_vi_vi(vint x, vint y) { return _mm_and_si128(x, y); }
static INLINE vint vandnot_vi_vi_vi(vint x, vint y) { return _mm_andnot_si128(x, y); }
static INLINE vint vor_vi_vi_vi(vint x, vint y) { return _mm_or_si128(x, y); }
static INLINE vint vxor_vi_vi_vi(vint x, vint y) { return _mm_xor_si128(x, y); }

static INLINE vint vsll_vi_vi_i(vint x, int c) { return _mm_slli_epi32(x, c); }
static INLINE vint vsrl_vi_vi_i(vint x, int c) { return _mm_srli_epi32(x, c); }
static INLINE vint vsra_vi_vi_i(vint x, int c) { return _mm_srai_epi32(x, c); }

//

static INLINE vmask vand_vm_vm_vm(vmask x, vmask y) { return (vmask)_mm256_and_pd((__m256d)x, (__m256d)y); }
static INLINE vmask vandnot_vm_vm_vm(vmask x, vmask y) { return (vmask)_mm256_andnot_pd((__m256d)x, (__m256d)y); }
static INLINE vmask vor_vm_vm_vm(vmask x, vmask y) { return (vmask)_mm256_or_pd((__m256d)x, (__m256d)y); }
static INLINE vmask vxor_vm_vm_vm(vmask x, vmask y) { return (vmask)_mm256_xor_pd((__m256d)x, (__m256d)y); }

static INLINE vmask veq_vm_vd_vd(vdouble x, vdouble y) { return (__m256i)_mm256_cmp_pd(x, y, _CMP_EQ_OQ); }
static INLINE vmask vneq_vm_vd_vd(vdouble x, vdouble y) { return (__m256i)_mm256_cmp_pd(x, y, _CMP_NEQ_UQ); }
static INLINE vmask vlt_vm_vd_vd(vdouble x, vdouble y) { return (__m256i)_mm256_cmp_pd(x, y, _CMP_LT_OQ); }
static INLINE vmask vle_vm_vd_vd(vdouble x, vdouble y) { return (__m256i)_mm256_cmp_pd(x, y, _CMP_LE_OQ); }
static INLINE vmask vgt_vm_vd_vd(vdouble x, vdouble y) { return (__m256i)_mm256_cmp_pd(x, y, _CMP_GT_OQ); }
static INLINE vmask vge_vm_vd_vd(vdouble x, vdouble y) { return (__m256i)_mm256_cmp_pd(x, y, _CMP_GE_OQ); }

static INLINE vmask veq_vm_vf_vf(vfloat x, vfloat y) { return (__m256i)_mm256_cmp_ps(x, y, _CMP_EQ_OQ); }
static INLINE vmask vneq_vm_vf_vf(vfloat x, vfloat y) { return (__m256i)_mm256_cmp_ps(x, y, _CMP_NEQ_UQ); }
static INLINE vmask vlt_vm_vf_vf(vfloat x, vfloat y) { return (__m256i)_mm256_cmp_ps(x, y, _CMP_LT_OQ); }
static INLINE vmask vle_vm_vf_vf(vfloat x, vfloat y) { return (__m256i)_mm256_cmp_ps(x, y, _CMP_LE_OQ); }
static INLINE vmask vgt_vm_vf_vf(vfloat x, vfloat y) { return (__m256i)_mm256_cmp_ps(x, y, _CMP_GT_OQ); }
static INLINE vmask vge_vm_vf_vf(vfloat x, vfloat y) { return (__m256i)_mm256_cmp_ps(x, y, _CMP_GE_OQ); }

//

static INLINE vfloat vcast_vf_f(float f) { return _mm256_set_ps(f, f, f, f, f, f, f, f); }

static INLINE vfloat vadd_vf_vf_vf(vfloat x, vfloat y) { return _mm256_add_ps(x, y); }
static INLINE vfloat vsub_vf_vf_vf(vfloat x, vfloat y) { return _mm256_sub_ps(x, y); }
static INLINE vfloat vmul_vf_vf_vf(vfloat x, vfloat y) { return _mm256_mul_ps(x, y); }
static INLINE vfloat vdiv_vf_vf_vf(vfloat x, vfloat y) { return _mm256_div_ps(x, y); }
static INLINE vfloat vrec_vf_vf(vfloat x) { return vdiv_vf_vf_vf(vcast_vf_f(1.0f), x); }
static INLINE vfloat vsqrt_vf_vf(vfloat x) { return _mm256_sqrt_ps(x); }
static INLINE vfloat vmax_vf_vf_vf(vfloat x, vfloat y) { return _mm256_max_ps(x, y); }
static INLINE vfloat vmin_vf_vf_vf(vfloat x, vfloat y) { return _mm256_min_ps(x, y); }

static INLINE vfloat vmla_vf_vf_vf_vf(vfloat x, vfloat y, vfloat z) { return vadd_vf_vf_vf(vmul_vf_vf_vf(x, y), z); }
static INLINE vfloat vmlanp_vf_vf_vf_vf(vfloat x, vfloat y, vfloat z) { return vsub_vf_vf_vf(z, vmul_vf_vf_vf(x, y)); }
static INLINE vfloat vabs_vf_vf(vfloat f) { return (vfloat)vandnot_vm_vm_vm((vmask)vcast_vf_f(-0.0f), (vmask)f); }
static INLINE vfloat vneg_vf_vf(vfloat d) { return (vfloat)vxor_vm_vm_vm((vmask)vcast_vf_f(-0.0f), (vmask)d); }

//

static INLINE vdouble vadd_vd_vd_vd(vdouble x, vdouble y) { return _mm256_add_pd(x, y); }
static INLINE vdouble vsub_vd_vd_vd(vdouble x, vdouble y) { return _mm256_sub_pd(x, y); }
static INLINE vdouble vmul_vd_vd_vd(vdouble x, vdouble y) { return _mm256_mul_pd(x, y); }
static INLINE vdouble vdiv_vd_vd_vd(vdouble x, vdouble y) { return _mm256_div_pd(x, y); }
static INLINE vdouble vrec_vd_vd(vdouble x) { return _mm256_div_pd(_mm256_set_pd(1, 1, 1, 1), x); }
static INLINE vdouble vsqrt_vd_vd(vdouble x) { return _mm256_sqrt_pd(x); }
static INLINE vdouble vabs_vd_vd(vdouble d) { return (__m256d)_mm256_andnot_pd(_mm256_set_pd(-0.0,-0.0,-0.0,-0.0), d); }
static INLINE vdouble vneg_vd_vd(vdouble d) { return (__m256d)_mm256_xor_pd(_mm256_set1_pd(-0.0), d); }
static INLINE vdouble vmla_vd_vd_vd_vd(vdouble x, vdouble y, vdouble z) { return vadd_vd_vd_vd(vmul_vd_vd_vd(x, y), z); }
static INLINE vdouble vmlapn_vd_vd_vd_vd(vdouble x, vdouble y, vdouble z) { return vsub_vd_vd_vd(vmul_vd_vd_vd(x, y), z); }

static INLINE vdouble vmax_vd_vd_vd(vdouble x, vdouble y) { return _mm256_max_pd(x, y); }
static INLINE vdouble vmin_vd_vd_vd(vdouble x, vdouble y) { return _mm256_min_pd(x, y); }

//

static INLINE vmask veq_vm_vi_vi(vint x, vint y) {
  __m256d r = _mm256_cvtepi32_pd(_mm_and_si128(_mm_cmpeq_epi32(x, y), _mm_set_epi32(1, 1, 1, 1)));
  return veq_vm_vd_vd(r, _mm256_set_pd(1, 1, 1, 1));
}

static INLINE vdouble vsel_vd_vm_vd_vd(vmask mask, vdouble x, vdouble y) {
  return (__m256d)vor_vm_vm_vm(vand_vm_vm_vm(mask, (__m256i)x), vandnot_vm_vm_vm(mask, (__m256i)y));
}

static INLINE vfloat vsel_vf_vm_vf_vf(vmask mask, vfloat x, vfloat y) {
  return (vfloat)vor_vm_vm_vm(vand_vm_vm_vm(mask, (vmask)x), vandnot_vm_vm_vm(mask, (vmask)y));
}

static INLINE vint vsel_vi_vd_vd_vi_vi(vdouble d0, vdouble d1, vint x, vint y) {
  __m128i mask = _mm256_cvtpd_epi32(_mm256_and_pd(_mm256_cmp_pd(d0, d1, _CMP_LT_OQ), _mm256_set_pd(1.0, 1.0, 1.0, 1.0)));
  mask = _mm_cmpeq_epi32(mask, _mm_set_epi32(1, 1, 1, 1));
  return vor_vi_vi_vi(vand_vi_vi_vi(mask, x), vandnot_vi_vi_vi(mask, y));
}

//

static INLINE vint2 vcast_vi2_vm(vmask vm) {
  vint2 r;
  r.x = _mm256_castsi256_si128(vm);
  r.y = _mm256_extractf128_si256(vm, 1);
  return r;
}

static INLINE vmask vcast_vm_vi2(vint2 vi) {
  vmask m = _mm256_castsi128_si256(vi.x);
  m = _mm256_insertf128_si256(m, vi.y, 1);
  return m;
}

static INLINE vint2 vrint_vi2_vf(vfloat vf) { return vcast_vi2_vm((vmask)_mm256_cvtps_epi32(vf)); }
static INLINE vint2 vtruncate_vi2_vf(vfloat vf) { return vcast_vi2_vm((vmask)_mm256_cvttps_epi32(vf)); }
static INLINE vfloat vcast_vf_vi2(vint2 vi) { return _mm256_cvtepi32_ps((vmask)vcast_vm_vi2(vi)); }
static INLINE vint2 vcast_vi2_i(int i) { vint2 r; r.x = r.y = vcast_vi_i(i); return r; }

static INLINE vint2 vadd_vi2_vi2_vi2(vint2 x, vint2 y) { vint2 r; r.x = vadd_vi_vi_vi(x.x, y.x); r.y = vadd_vi_vi_vi(x.y, y.y); return r; }
static INLINE vint2 vsub_vi2_vi2_vi2(vint2 x, vint2 y) { vint2 r; r.x = vsub_vi_vi_vi(x.x, y.x); r.y = vsub_vi_vi_vi(x.y, y.y); return r; }
static INLINE vint2 vneg_vi2_vi2(vint2 e) { return vsub_vi2_vi2_vi2(vcast_vi2_i(0), e); }

static INLINE vint2 vand_vi2_vi2_vi2(vint2 x, vint2 y) { vint2 r; r.x = vand_vi_vi_vi(x.x, y.x); r.y = vand_vi_vi_vi(x.y, y.y); return r; }
static INLINE vint2 vandnot_vi2_vi2_vi2(vint2 x, vint2 y) { vint2 r; r.x = vandnot_vi_vi_vi(x.x, y.x); r.y = vandnot_vi_vi_vi(x.y, y.y); return r; }
static INLINE vint2 vor_vi2_vi2_vi2(vint2 x, vint2 y) { vint2 r; r.x = vor_vi_vi_vi(x.x, y.x); r.y = vor_vi_vi_vi(x.y, y.y); return r; }
static INLINE vint2 vxor_vi2_vi2_vi2(vint2 x, vint2 y) { vint2 r; r.x = vxor_vi_vi_vi(x.x, y.x); r.y = vxor_vi_vi_vi(x.y, y.y); return r; }

static INLINE vint2 vsll_vi2_vi2_i(vint2 x, int c) { vint2 r; r.x = vsll_vi_vi_i(x.x, c); r.y = vsll_vi_vi_i(x.y, c); return r; }
static INLINE vint2 vsrl_vi2_vi2_i(vint2 x, int c) { vint2 r; r.x = vsrl_vi_vi_i(x.x, c); r.y = vsrl_vi_vi_i(x.y, c); return r; }
static INLINE vint2 vsra_vi2_vi2_i(vint2 x, int c) { vint2 r; r.x = vsra_vi_vi_i(x.x, c); r.y = vsra_vi_vi_i(x.y, c); return r; }

static INLINE vmask veq_vm_vi2_vi2(vint2 x, vint2 y) {
  vint2 r;
  r.x = _mm_cmpeq_epi32(x.x, y.x);
  r.y = _mm_cmpeq_epi32(x.y, y.y);
  return vcast_vm_vi2(r);
}

static INLINE vmask vgt_vm_vi2_vi2(vint2 x, vint2 y) {
  vint2 r;
  r.x = _mm_cmpgt_epi32(x.x, y.x);
  r.y = _mm_cmpgt_epi32(x.y, y.y);
  return vcast_vm_vi2(r);
}

static INLINE vint2 vgt_vi2_vi2_vi2(vint2 x, vint2 y) {
  vint2 r;
  r.x = _mm_cmpgt_epi32(x.x, y.x);
  r.y = _mm_cmpgt_epi32(x.y, y.y);
  return r;
}

static INLINE vint2 vsel_vi2_vm_vi2_vi2(vmask m, vint2 x, vint2 y) {
  vint2 r, m2 = vcast_vi2_vm(m);
  r.x = vor_vi_vi_vi(vand_vi_vi_vi(m2.x, x.x), vandnot_vi_vi_vi(m2.x, y.x));
  r.y = vor_vi_vi_vi(vand_vi_vi_vi(m2.y, x.y), vandnot_vi_vi_vi(m2.y, y.y));
  return r;
}

//

static INLINE double vcast_d_vd(vdouble v) {
  double s[4];
  _mm256_storeu_pd(s, v);
  return s[0];
}

static INLINE float vcast_f_vf(vfloat v) {
  float s[8];
  _mm256_storeu_ps(s, v);
  return s[0];
}

static INLINE vmask vsignbit_vm_vd(vdouble d) {
  return (vmask)_mm256_and_pd(d, _mm256_set_pd(-0.0,-0.0,-0.0,-0.0));
}

static INLINE vdouble vsign_vd_vd(vdouble d) {
  return _mm256_or_pd(_mm256_set_pd(1.0, 1.0, 1.0, 1.0), (vdouble)vsignbit_vm_vd(d));
}

static INLINE vdouble vmulsign_vd_vd_vd(vdouble x, vdouble y) {
  return (__m256d)vxor_vm_vm_vm((__m256i)x, vsignbit_vm_vd(y));
}

static INLINE vmask visinf_vm_vd(vdouble d) {
  return (vmask)_mm256_cmp_pd(vabs_vd_vd(d), _mm256_set_pd(INFINITY, INFINITY, INFINITY, INFINITY), _CMP_EQ_OQ);
}

static INLINE vmask vispinf_vm_vd(vdouble d) {
  return (vmask)_mm256_cmp_pd(d, _mm256_set_pd(INFINITY, INFINITY, INFINITY, INFINITY), _CMP_EQ_OQ);
}

static INLINE vmask visminf_vm_vd(vdouble d) {
  return (vmask)_mm256_cmp_pd(d, _mm256_set_pd(-INFINITY, -INFINITY, -INFINITY, -INFINITY), _CMP_EQ_OQ);
}

static INLINE vmask visnan_vm_vd(vdouble d) {
  return (vmask)_mm256_cmp_pd(d, d, _CMP_NEQ_UQ);
}

static INLINE vdouble visinf(vdouble d) {
  return _mm256_and_pd((vdouble)visinf_vm_vd(d), vsign_vd_vd(d));
}

static INLINE vdouble visinf2(vdouble d, vdouble m) {
  return _mm256_and_pd((vdouble)visinf_vm_vd(d), _mm256_or_pd((vdouble)vsignbit_vm_vd(d), m));
}

static INLINE vdouble vupper_vd_vd(vdouble d) {
  return (__m256d)_mm256_and_pd(d, (vdouble)_mm256_set_epi32(0xffffffff, 0xf8000000, 0xffffffff, 0xf8000000, 0xffffffff, 0xf8000000, 0xffffffff, 0xf8000000));
}

static INLINE vfloat vupper_vf_vf(vfloat d) {
  return (vfloat)vand_vm_vm_vm((vmask)d, _mm256_set_epi32(0xfffff000, 0xfffff000, 0xfffff000, 0xfffff000,0xfffff000, 0xfffff000, 0xfffff000, 0xfffff000));
}
