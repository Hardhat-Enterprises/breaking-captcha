#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <math.h>
#include <complex.h>
#include <time.h>

#include "sleefdft.h"

#ifndef M_PI
#define M_PI 3.141592653589793238462643383279502884
#endif

#define THRES 1e-4

#define max(x, y) ((x) > (y) ? (x) : (y))

typedef double complex cmpl;

cmpl omega(double n, double kn) {
  return cexp((-2 * M_PI * _Complex_I / n) * kn);
}

void forward(cmpl *ts, cmpl *fs, int len) {
  int k, n;

  for(k=0;k<len;k++) {
    fs[k] = 0;

    for(n=0;n<len;n++) {
      fs[k] += ts[n] * omega(len, n*k);
    }
  }
}

void backward(cmpl *fs, cmpl *ts, int len) {
  int k, n;

  for(k=0;k<len;k++) {
    ts[k] = 0;

    for(n=0;n<len;n++) {
      ts[k] += fs[n] * omega(-len, n*k);
    }
  }
}

// complex forward
int check_cf(int n) {
  int i;

  SleefDFT_real *sx = (SleefDFT_real *)SleefDFT_malloc(n*2 * sizeof(SleefDFT_real));
  SleefDFT_real *sy = (SleefDFT_real *)SleefDFT_malloc(n*2 * sizeof(SleefDFT_real));

  cmpl *ts = (cmpl *)malloc(sizeof(cmpl)*n);
  cmpl *fs = (cmpl *)malloc(sizeof(cmpl)*n);

  //

  for(i=0;i<n;i++) {
    ts[i] = (2.0 * (rand() / (double)RAND_MAX) - 1) + (2.0 * (rand() / (double)RAND_MAX) - 1) * _Complex_I;
    sx[(i*2+0)] = creal(ts[i]);
    sx[(i*2+1)] = cimag(ts[i]);
  }

  //

  forward(ts, fs, n);

  struct SleefDFT *p = SleefDFT_init(SLEEF_MODE_DEBUG | SLEEF_MODE_VERBOSE, n);

  SleefDFT_execute(p, sy, sx);

  //

  int success = 1;
  double maxError = 0;

  for(i=0;i<n;i++) {
    if ((fabs(sy[(i*2+0)] - creal(fs[i])) > THRES) ||
	(fabs(sy[(i*2+1)] - cimag(fs[i])) > THRES)) {
      success = 0;
    }
    
    //printf("%g, %g\n", sy[(2*i+0)], creal(fs[i]));
    //printf("%g, %g\n", sy[(2*i+1)], cimag(fs[i]));
    
    maxError = max(maxError, fabs((sy[(i*2+0)] - creal(fs[i]))));
    maxError = max(maxError, fabs((sy[(i*2+1)] - cimag(fs[i]))));
  }

  printf("maxError = %g\n", maxError);

  //

  free(fs);
  free(ts);

  SleefDFT_free(sx);
  SleefDFT_free(sy);
  SleefDFT_dispose(p);

  //

  return success;
}

// complex backward
int check_cb(int n) {
  int i;

  SleefDFT_real *sx = (SleefDFT_real *)SleefDFT_malloc(sizeof(SleefDFT_real)*n*2);
  SleefDFT_real *sy = (SleefDFT_real *)SleefDFT_malloc(sizeof(SleefDFT_real)*n*2);

  cmpl *ts = (cmpl *)malloc(sizeof(cmpl)*n);
  cmpl *fs = (cmpl *)malloc(sizeof(cmpl)*n);

  //

  for(i=0;i<n;i++) {
    fs[i] = (2.0 * (rand() / (double)RAND_MAX) - 1) + (2.0 * (rand() / (double)RAND_MAX) - 1) * _Complex_I;
    sx[(i*2+0)] = creal(fs[i]);
    sx[(i*2+1)] = cimag(fs[i]);
  }

  backward(fs, ts, n);

  struct SleefDFT *p = SleefDFT_init(SLEEF_MODE_DEBUG | SLEEF_MODE_BACKWARD, n);

  SleefDFT_execute(p, sy, sx);

  //

  int success = 1;

  for(i=0;i<n;i++) {
    if ((fabs(sy[(i*2+0)] - creal(ts[i])) > THRES) ||
	(fabs(sy[(i*2+1)] - cimag(ts[i])) > THRES)) {
      success = 0;
    }
  }

  //

  free(fs);
  free(ts);

  SleefDFT_free(sx);
  SleefDFT_free(sy);
  SleefDFT_dispose(p);

  //

  return success;
}

int check_rf(int n) {
  int i;

  SleefDFT_real *sx = (SleefDFT_real *)SleefDFT_malloc(n * sizeof(SleefDFT_real));
  SleefDFT_real *sy = (SleefDFT_real *)SleefDFT_malloc(n * sizeof(SleefDFT_real));

  cmpl *ts = (cmpl *)malloc(sizeof(cmpl)*n);
  cmpl *fs = (cmpl *)malloc(sizeof(cmpl)*n);

  //

  for(i=0;i<n;i++) {
    ts[i] = (2.0 * (rand() / (double)RAND_MAX) - 1);
    sx[i] = creal(ts[i]);
  }

  //

  forward(ts, fs, n);

  struct SleefDFT *p = SleefDFT_init(SLEEF_MODE_DEBUG | SLEEF_MODE_REAL, n);

  SleefDFT_execute(p, sy, sx);

  //

  int success = 1;

  for(i=0;i<n/2;i++) {
    if (i == 0) {
      if (fabs(sy[(2*0+0)] - creal(fs[0  ])) > THRES) success = 0;
      if (fabs(sy[(2*0+1)] - creal(fs[n/2])) > THRES) success = 0;
    } else {
      if (fabs(sy[(2*i+0)] - creal(fs[i])) > THRES) success = 0;
      if (fabs(sy[(2*i+1)] - cimag(fs[i])) > THRES) success = 0;
    }
  }

  //

  free(fs);
  free(ts);

  SleefDFT_free(sx);
  SleefDFT_free(sy);
  SleefDFT_dispose(p);

  //

  return success;
}

int check_rb(int n) {
  int i;

  SleefDFT_real *sx = (SleefDFT_real *)SleefDFT_malloc(n * sizeof(SleefDFT_real));
  SleefDFT_real *sy = (SleefDFT_real *)SleefDFT_malloc(n * sizeof(SleefDFT_real));

  cmpl *ts = (cmpl *)malloc(sizeof(cmpl)*n);
  cmpl *fs = (cmpl *)malloc(sizeof(cmpl)*n);

  //

  for(i=0;i<n/2;i++) {
    if (i == 0) {
      fs[0  ] = (2.0 * (rand() / (double)RAND_MAX) - 1);
      fs[n/2] = (2.0 * (rand() / (double)RAND_MAX) - 1);
    } else {
      fs[i  ] = (2.0 * (rand() / (double)RAND_MAX) - 1) + (2.0 * (rand() / (double)RAND_MAX) - 1) * _Complex_I;
      fs[n-i] = conj(fs[i]);
    }
  }

  for(i=0;i<n/2;i++) {
    if (i == 0) {
      sx[2*0+0] = creal(fs[0  ]);
      sx[2*0+1] = creal(fs[n/2]);
    } else {
      sx[2*i+0] = creal(fs[i]);
      sx[2*i+1] = cimag(fs[i]);
    }
  }

  //

  backward(fs, ts, n);

  struct SleefDFT *p = SleefDFT_init(SLEEF_MODE_DEBUG | SLEEF_MODE_REAL | SLEEF_MODE_BACKWARD, n);

  SleefDFT_execute(p, sy, sx);

  //

  int success = 1;

  for(i=0;i<n;i++) {
    if (fabs(cimag(ts[i])) > THRES) {
      success = 0;
    }

    if ((fabs(sy[i]*2 - creal(ts[i])) > THRES)) {
      success = 0;
    }
  }

  //

  free(fs);
  free(ts);

  SleefDFT_free(sx);
  SleefDFT_free(sy);
  SleefDFT_dispose(p);

  //

  return success;
}

int check_arf(int n) {
  int i;

  SleefDFT_real *sx = (SleefDFT_real *)SleefDFT_malloc(n * sizeof(SleefDFT_real));
  SleefDFT_real *sy = (SleefDFT_real *)SleefDFT_malloc(n * sizeof(SleefDFT_real));

  cmpl *ts = (cmpl *)malloc(sizeof(cmpl)*n);
  cmpl *fs = (cmpl *)malloc(sizeof(cmpl)*n);

  //

  for(i=0;i<n;i++) {
    ts[i] = 2 * (rand() / (SleefDFT_real)RAND_MAX) - 1;
    sx[i] = creal(ts[i]);
  }

  //

  backward(ts, fs, n);

  struct SleefDFT *p = SleefDFT_init(SLEEF_MODE_REAL | SLEEF_MODE_ALT, n);

  SleefDFT_execute(p, sy, sx);

  //

  int success = 1;

  for(i=0;i<n/2;i++) {
    if (i == 0) {
      if (fabs(sy[(2*0+0)] - creal(fs[0  ])) > THRES) success = 0;
      if (fabs(sy[(2*0+1)] - creal(fs[n/2])) > THRES) success = 0;
    } else {
      if (fabs(sy[(2*i+0)] - creal(fs[i])) > THRES) success = 0;
      if (fabs(sy[(2*i+1)] - cimag(fs[i])) > THRES) success = 0;
    }
  }

  //

  SleefDFT_free(sx);
  SleefDFT_free(sy);
  SleefDFT_dispose(p);

  //

  return success;
}

int check_arb(int n) {
  int i;

  SleefDFT_real *sx = (SleefDFT_real *)SleefDFT_malloc(n * sizeof(SleefDFT_real));
  SleefDFT_real *sy = (SleefDFT_real *)SleefDFT_malloc(n * sizeof(SleefDFT_real));

  cmpl *ts = (cmpl *)malloc(sizeof(cmpl)*n);
  cmpl *fs = (cmpl *)malloc(sizeof(cmpl)*n);

  //

  for(i=0;i<n/2;i++) {
    if (i == 0) {
      fs[0  ] = (2.0 * (rand() / (double)RAND_MAX) - 1);
      fs[n/2] = (2.0 * (rand() / (double)RAND_MAX) - 1);
    } else {
      fs[i  ] = (2.0 * (rand() / (double)RAND_MAX) - 1) + (2.0 * (rand() / (double)RAND_MAX) - 1) * _Complex_I;
      fs[n-i] = conj(fs[i]);
    }
  }

  for(i=0;i<n/2;i++) {
    if (i == 0) {
      sx[2*0+0] = creal(fs[0  ]);
      sx[2*0+1] = creal(fs[n/2]);
    } else {
      sx[2*i+0] = creal(fs[i]);
      sx[2*i+1] = cimag(fs[i]);
    }
  }

  //

  forward(fs, ts, n);

  struct SleefDFT *p = SleefDFT_init(SLEEF_MODE_REAL | SLEEF_MODE_BACKWARD | SLEEF_MODE_ALT, n);

  SleefDFT_execute(p, sy, sx);

  //

  int success = 1;

  for(i=0;i<n;i++) {
    if (fabs(cimag(ts[i])) > THRES) {
      success = 0;
    }

    if ((fabs(sy[i]*2 - creal(ts[i])) > THRES)) {
      success = 0;
    }
  }

  //

  free(fs);
  free(ts);

  SleefDFT_free(sx);
  SleefDFT_free(sy);
  SleefDFT_dispose(p);

  //

  return success;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "%s <log2n>\n", argv[0]);
    exit(-1);
  }

  const int n = 1 << atoi(argv[1]);

  srand(time(NULL));

  //

  printf("complex  forward   : %s\n", check_cf(n)  ? "OK" : "NG");
  printf("complex  backward  : %s\n", check_cb(n)  ? "OK" : "NG");
  printf("real     forward   : %s\n", check_rf(n)  ? "OK" : "NG");
  printf("real     backward  : %s\n", check_rb(n)  ? "OK" : "NG");
  printf("real alt forward   : %s\n", check_arf(n) ? "OK" : "NG");
  printf("real alt backward  : %s\n", check_arb(n) ? "OK" : "NG");

  exit(0);
}
