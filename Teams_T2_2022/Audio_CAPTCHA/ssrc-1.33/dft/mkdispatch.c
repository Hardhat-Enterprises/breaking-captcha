#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

int main(int argc, char **argv) {
  if (argc < 3) {
    fprintf(stderr, "Usage : %s <unrollmax> <unrollmax2> <maxbutwidth> <isa> ...\n", argv[0]);
    exit(-1);
  }

  const int unrollmax = atoi(argv[1]);
  const int unrollmax2 = atoi(argv[2]);
  const int maxbutwidth = atoi(argv[3]);
  const int isastart = 4;
  const int isamax = argc - isastart;

  printf("#define ISAMAX %d\n", isamax);
  printf("#define MAXBUTWIDTH %d\n", maxbutwidth);
  printf("#define UNROLLMAX %d\n", unrollmax);
  printf("#define UNROLLMAX2 %d\n", unrollmax2);
  printf("\n");

  for(int k=isastart;k<argc;k++) {
    for(int j=1;j<=maxbutwidth;j++) {
      printf("void dft%df_%s(SleefDFT_real *, const SleefDFT_real *, const int);\n", 1 << j, argv[k]);
      printf("void dft%db_%s(SleefDFT_real *, const SleefDFT_real *, const int);\n", 1 << j, argv[k]);
      printf("void tbut%df_%s(SleefDFT_real *, uint32_t *, const SleefDFT_real *, const int, const SleefDFT_real *, const int);\n", 1 << j, argv[k]);
      printf("void tbut%db_%s(SleefDFT_real *, uint32_t *, const SleefDFT_real *, const int, const SleefDFT_real *, const int);\n", 1 << j, argv[k]);
      printf("void but%df_%s(SleefDFT_real *, uint32_t *, const int, const SleefDFT_real *, const int, const SleefDFT_real *, const int);\n", 1 << j, argv[k]);
      printf("void but%db_%s(SleefDFT_real *, uint32_t *, const int, const SleefDFT_real *, const int, const SleefDFT_real *, const int);\n", 1 << j, argv[k]);

      printf("void dft%df_%s_s(SleefDFT_real *, const SleefDFT_real *, const int);\n", 1 << j, argv[k]);
      printf("void dft%db_%s_s(SleefDFT_real *, const SleefDFT_real *, const int);\n", 1 << j, argv[k]);
      printf("void tbut%df_%s_s(SleefDFT_real *, uint32_t *, const SleefDFT_real *, const int, const SleefDFT_real *, const int);\n", 1 << j, argv[k]);
      printf("void tbut%db_%s_s(SleefDFT_real *, uint32_t *, const SleefDFT_real *, const int, const SleefDFT_real *, const int);\n", 1 << j, argv[k]);
      printf("void but%df_%s_s(SleefDFT_real *, uint32_t *, const int, const SleefDFT_real *, const int, const SleefDFT_real *, const int);\n", 1 << j, argv[k]);
      printf("void but%db_%s_s(SleefDFT_real *, uint32_t *, const int, const SleefDFT_real *, const int, const SleefDFT_real *, const int);\n", 1 << j, argv[k]);
    }
    printf("void realSub0_%s(SleefDFT_real *, const SleefDFT_real *, const int, const SleefDFT_real *, const SleefDFT_real *);\n", argv[k]);
    printf("void realSub1_%s(SleefDFT_real *, const SleefDFT_real *, const int, const SleefDFT_real *, const SleefDFT_real *);\n", argv[k]);
    printf("int getInt_%s(int);\n", argv[k]);
    printf("const void *getPtr_%s(int);\n", argv[k]);
  }

  printf("\n");

  printf("void (*nudftf[ISAMAX][MAXBUTWIDTH+1])(SleefDFT_real *, const SleefDFT_real *, const int) = {\n");
  for(int k=isastart;k<argc;k++) {
    printf("  {NULL, ");
    for(int i=1;i<=maxbutwidth;i++) {
      printf("dft%df_%s, ", 1 << i, argv[k]);
    }
    printf("},\n");
  }
  printf("};\n\n");

  printf("void (*nudftb[ISAMAX][MAXBUTWIDTH+1])(SleefDFT_real *, const SleefDFT_real *, const int) = {\n");
  for(int k=isastart;k<argc;k++) {
    printf("  {NULL, ");
    for(int i=1;i<=maxbutwidth;i++) {
      if (i == 1) {
	printf("dft%df_%s, ", 1 << i, argv[k]);
      } else {
	printf("dft%db_%s, ", 1 << i, argv[k]);
      }
    }
    printf("},\n");
  }
  printf("};\n\n");

  printf("void (*nutbutf[ISAMAX][MAXBUTWIDTH+1])(SleefDFT_real *, uint32_t *, const SleefDFT_real *, const int, const SleefDFT_real *, const int) = {\n");
  for(int k=isastart;k<argc;k++) {
    printf("  {NULL, ");
    for(int i=1;i<=maxbutwidth;i++) {
      printf("tbut%df_%s, ", 1 << i, argv[k]);
    }
    printf("},\n");
  }
  printf("};\n\n");

  printf("void (*nutbutb[ISAMAX][MAXBUTWIDTH+1])(SleefDFT_real *, uint32_t *, const SleefDFT_real *, const int, const SleefDFT_real *, const int) = {\n");
  for(int k=isastart;k<argc;k++) {
    printf("  {NULL, ");
    for(int i=1;i<=maxbutwidth;i++) {
      printf("tbut%db_%s, ", 1 << i, argv[k]);
    }
    printf("},\n");
  }
  printf("};\n\n");

  printf("void (*nubutf[ISAMAX][MAXBUTWIDTH+1])(SleefDFT_real *, uint32_t *, const int, const SleefDFT_real *, const int, const SleefDFT_real *, const int) = {\n");
  for(int k=isastart;k<argc;k++) {
    printf("  {NULL, ");
    for(int i=1;i<=maxbutwidth;i++) {
      printf("but%df_%s, ", 1 << i, argv[k]);
    }
    printf("},\n");
  }
  printf("};\n\n");

  printf("void (*nubutb[ISAMAX][MAXBUTWIDTH+1])(SleefDFT_real *, uint32_t *, const int, const SleefDFT_real *, const int, const SleefDFT_real *, const int) = {\n");
  for(int k=isastart;k<argc;k++) {
    printf("  {NULL, ");
    for(int i=1;i<=maxbutwidth;i++) {
      printf("but%db_%s, ", 1 << i, argv[k]);
    }
    printf("},\n");
  }
  printf("};\n\n");

  //

  printf("void (*nudftfs[ISAMAX][MAXBUTWIDTH+1])(SleefDFT_real *, const SleefDFT_real *, const int) = {\n");
  for(int k=isastart;k<argc;k++) {
    printf("  {NULL, ");
    for(int i=1;i<=maxbutwidth;i++) {
      printf("dft%df_%s_s, ", 1 << i, argv[k]);
    }
    printf("},\n");
  }
  printf("};\n\n");

  printf("void (*nudftbs[ISAMAX][MAXBUTWIDTH+1])(SleefDFT_real *, const SleefDFT_real *, const int) = {\n");
  for(int k=isastart;k<argc;k++) {
    printf("  {NULL, ");
    for(int i=1;i<=maxbutwidth;i++) {
      if (i == 1) {
	printf("dft%df_%s_s, ", 1 << i, argv[k]);
      } else {
	printf("dft%db_%s_s, ", 1 << i, argv[k]);
      }
    }
    printf("},\n");
  }
  printf("};\n\n");

  printf("void (*nutbutfs[ISAMAX][MAXBUTWIDTH+1])(SleefDFT_real *, uint32_t *, const SleefDFT_real *, const int, const SleefDFT_real *, const int) = {\n");
  for(int k=isastart;k<argc;k++) {
    printf("  {NULL, ");
    for(int i=1;i<=maxbutwidth;i++) {
      printf("tbut%df_%s_s, ", 1 << i, argv[k]);
    }
    printf("},\n");
  }
  printf("};\n\n");

  printf("void (*nutbutbs[ISAMAX][MAXBUTWIDTH+1])(SleefDFT_real *, uint32_t *, const SleefDFT_real *, const int, const SleefDFT_real *, const int) = {\n");
  for(int k=isastart;k<argc;k++) {
    printf("  {NULL, ");
    for(int i=1;i<=maxbutwidth;i++) {
      printf("tbut%db_%s_s, ", 1 << i, argv[k]);
    }
    printf("},\n");
  }
  printf("};\n\n");

  printf("void (*nubutfs[ISAMAX][MAXBUTWIDTH+1])(SleefDFT_real *, uint32_t *, const int, const SleefDFT_real *, const int, const SleefDFT_real *, const int) = {\n");
  for(int k=isastart;k<argc;k++) {
    printf("  {NULL, ");
    for(int i=1;i<=maxbutwidth;i++) {
      printf("but%df_%s_s, ", 1 << i, argv[k]);
    }
    printf("},\n");
  }
  printf("};\n\n");

  printf("void (*nubutbs[ISAMAX][MAXBUTWIDTH+1])(SleefDFT_real *, uint32_t *, const int, const SleefDFT_real *, const int, const SleefDFT_real *, const int) = {\n");
  for(int k=isastart;k<argc;k++) {
    printf("  {NULL, ");
    for(int i=1;i<=maxbutwidth;i++) {
      printf("but%db_%s_s, ", 1 << i, argv[k]);
    }
    printf("},\n");
  }
  printf("};\n\n");

  //

  printf("void (*realSub0[ISAMAX])(SleefDFT_real *, const SleefDFT_real *, const int, const SleefDFT_real *, const SleefDFT_real *) = {\n  ");
  for(int k=isastart;k<argc;k++) printf("realSub0_%s, ", argv[k]);
  printf("\n};\n\n");

  printf("void (*realSub1[ISAMAX])(SleefDFT_real *, const SleefDFT_real *, const int, const SleefDFT_real *, const SleefDFT_real *) = {\n  ");
  for(int k=isastart;k<argc;k++) printf("realSub1_%s, ", argv[k]);
  printf("\n};\n\n");

  printf("int (*getInt[16])(int) = {\n  ");
  for(int k=isastart;k<argc;k++) printf("getInt_%s, ", argv[k]);
  for(int k=0;k<16-(argc-isastart);k++) printf("NULL, ");
  printf("\n};\n\n");

  printf("const void *(*getPtr[16])(int) = {\n  ");
  for(int k=isastart;k<argc;k++) printf("getPtr_%s, ", argv[k]);
  for(int k=0;k<16-(argc-isastart);k++) printf("NULL, ");
  printf("\n};\n\n");

  //

  for(int k=isastart;k<argc;k++) {
    for(int i=0;i<=unrollmax;i++) {
      for(int j=1;j<=maxbutwidth;j++) {
	printf("void dft%df_%d_%s(SleefDFT_real *, const SleefDFT_real *);\n", 1 << j, i, argv[k]);
	printf("void dft%db_%d_%s(SleefDFT_real *, const SleefDFT_real *);\n", 1 << j, i, argv[k]);
	printf("void tbut%df_%d_%s(SleefDFT_real *, uint32_t *, const SleefDFT_real *, const SleefDFT_real *, const int);\n", 1 << j, i, argv[k]);
	printf("void tbut%db_%d_%s(SleefDFT_real *, uint32_t *, const SleefDFT_real *, const SleefDFT_real *, const int);\n", 1 << j, i, argv[k]);
      }
    }
    for(int i=0;i<=unrollmax2;i++) {
      for(int m=0;m<=unrollmax2;m++) {
	for(int j=1;j<=maxbutwidth;j++) {
	  printf("void but%df_%d_%d_%s(SleefDFT_real *, uint32_t *, const SleefDFT_real *, const SleefDFT_real *, const int);\n", 1 << j, i, m, argv[k]);
	  printf("void but%db_%d_%d_%s(SleefDFT_real *, uint32_t *, const SleefDFT_real *, const SleefDFT_real *, const int);\n", 1 << j, i, m, argv[k]);
	}
      }
    }
  }

  printf("\n");

  printf("void (*dftf[ISAMAX][MAXBUTWIDTH+1][UNROLLMAX+1])(SleefDFT_real *, const SleefDFT_real *) = {\n");
  for(int m=isastart;m<argc;m++) {
    printf("  {\n");
    printf("    {},\n");
    for(int i=1;i<=maxbutwidth;i++) {
      printf("    {\n");
      for(int j=0;j<=unrollmax;j++) {
	if ((j & 3) == 0) printf("      ");
	printf("dft%df_%d_%s, ", 1 << i, j, argv[m]);
	if ((j & 3) == 3) printf("\n");
      }
      printf("\n    },\n");
    }
    printf("  },\n");
  }
  printf("};\n");
  printf("\n");

  printf("void (*dftb[ISAMAX][MAXBUTWIDTH+1][UNROLLMAX+1])(SleefDFT_real *, const SleefDFT_real *) = {\n");
  for(int m=isastart;m<argc;m++) {
    printf("  {\n");
    printf("    {},\n");
    for(int i=1;i<=maxbutwidth;i++) {
      printf("    {\n");
      for(int j=0;j<=unrollmax;j++) {
	if ((j & 3) == 0) printf("      ");
	if (i == 1) {
	  printf("dft%df_%d_%s, ", 1 << i, j, argv[m]);
	} else {
	  printf("dft%db_%d_%s, ", 1 << i, j, argv[m]);
	}
	if ((j & 3) == 3) printf("\n");
      }
      printf("\n    },\n");
    }
    printf("  },\n");
  }
  printf("};\n");
  printf("\n");

  printf("void (*tbutf[ISAMAX][MAXBUTWIDTH+1][UNROLLMAX+1])(SleefDFT_real *, uint32_t *, const SleefDFT_real *, const SleefDFT_real *, const int) = {\n");
  for(int m=isastart;m<argc;m++) {
    printf("  {\n");
    printf("    {},\n");
    for(int i=1;i<=maxbutwidth;i++) {
      printf("    {\n");
      for(int j=0;j<=unrollmax;j++) {
	if ((j & 3) == 0) printf("      ");
	printf("tbut%df_%d_%s, ", 1 << i, j, argv[m]);
	if ((j & 3) == 3) printf("\n");
      }
      printf("\n    },\n");
    }
    printf("  },\n");
  }
  printf("};\n");
  printf("\n");

  printf("void (*tbutb[ISAMAX][MAXBUTWIDTH+1][UNROLLMAX+1])(SleefDFT_real *, uint32_t *, const SleefDFT_real *, const SleefDFT_real *, const int) = {\n");
  for(int m=isastart;m<argc;m++) {
    printf("  {\n");
    printf("    {},\n");
    for(int i=1;i<=maxbutwidth;i++) {
      printf("    {\n");
      for(int j=0;j<=unrollmax;j++) {
	if ((j & 3) == 0) printf("      ");
	printf("tbut%db_%d_%s, ", 1 << i, j, argv[m]);
	if ((j & 3) == 3) printf("\n");
      }
      printf("\n    },\n");
    }
    printf("  },\n");
  }
  printf("};\n");
  printf("\n");

  printf("void (*butf[ISAMAX][UNROLLMAX2+1][MAXBUTWIDTH+1][UNROLLMAX2+1])(SleefDFT_real *, uint32_t *, const SleefDFT_real *, const SleefDFT_real *, const int) = {\n");
  for(int m=isastart;m<argc;m++) {
    printf("  {\n");
    for(int k=0;k<=unrollmax2;k++) {
      printf("    {\n");
      printf("      {},\n");
      for(int i=1;i<=maxbutwidth;i++) {
	printf("      {\n");
	for(int j=0;j<=unrollmax2;j++) {
	  if ((j & 3) == 0) printf("        ");
	  printf("but%df_%d_%d_%s, ", 1 << i, k, j, argv[m]);
	  if ((j & 3) == 3) printf("\n");
	}
	printf("\n      },\n");
      }
      printf("    },\n");
    }
    printf("  },\n");
  }
  printf("};\n");
  printf("\n");

  printf("void (*butb[ISAMAX][UNROLLMAX2+1][MAXBUTWIDTH+1][UNROLLMAX2+1])(SleefDFT_real *, uint32_t *, const SleefDFT_real *, const SleefDFT_real *, const int) = {\n");
  for(int m=isastart;m<argc;m++) {
    printf("  {\n");
    for(int k=0;k<=unrollmax2;k++) {
      printf("    {\n");
      printf("      {},\n");
      for(int i=1;i<=maxbutwidth;i++) {
	printf("      {\n");
	for(int j=0;j<=unrollmax2;j++) {
	  if ((j & 3) == 0) printf("        ");
	  printf("but%db_%d_%d_%s, ", 1 << i, k, j, argv[m]);
	  if ((j & 3) == 3) printf("\n");
	}
	printf("\n      },\n");
      }
      printf("    },\n");
    }
    printf("  },\n");
  }
  printf("};\n");
  printf("\n");
}
