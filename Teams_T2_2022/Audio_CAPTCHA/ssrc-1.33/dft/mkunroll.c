#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <sys/stat.h>
#include <unistd.h>

char *replaceAll(const char *in, const char *pat, const char *replace) {
  const int replaceLen = strlen(replace);
  const int patLen = strlen(pat);

  char *str = malloc(strlen(in)+1);
  strcpy(str, in);

  for(;;) {
    char *p = strstr(str, pat);
    if (p == NULL) return str;

    int replace_pos = p - str;
    int tail_len = strlen(p + patLen);

    char *newstr = malloc(strlen(str) + (replaceLen - patLen) + 1);

    memcpy(newstr, str, replace_pos);
    memcpy(newstr + replace_pos, replace, replaceLen);
    memcpy(newstr + replace_pos + replaceLen, str + replace_pos + patLen, tail_len+1);

    free(str);
    str = newstr;
  }

  return str;
}

#define LEN 1024
char line[LEN+10];

int main(int argc, char **argv) {
  if (argc < 3) {
    fprintf(stderr, "Usage : %s <unrollmax> <unrollmax2> <isa> ...\n", argv[0]);
    exit(-1);
  }

  const int unrollmax = atoi(argv[1]);
  const int unrollmax2 = atoi(argv[2]);
  const int isastart = 3;
  const int isamax = argc - isastart;
  const int maxbutwidth = 6;

  for(int k=isastart;k<argc;k++) {
    FILE *fpin = fopen("unroll0.org", "r");

    sprintf(line, "unroll0_%s.c", argv[k]);
    FILE *fpout = fopen(line, "w");
    fputs("#include \"simd.h\"\n\n", fpout);
    fputs("extern SleefDFT_real ctbl[];\n\n", fpout);

    for(;;) {
      if (fgets(line, LEN, fpin) == NULL) break;
      char *s = replaceAll(line, "%ISA%", argv[k]);
      fputs(s, fpout);
      free(s);
    }
    
    fclose(fpin);
    fclose(fpout);
  }

  for(int k=isastart;k<argc;k++) {
    FILE *fpin = fopen("unroll0.org", "r");

    sprintf(line, "unroll0s_%s.c", argv[k]);
    FILE *fpout = fopen(line, "w");
    fputs("#include \"simd.h\"\n\n", fpout);
    fputs("extern SleefDFT_real ctbl[];\n\n", fpout);

    char isastring[20];
    sprintf(isastring, "%s_s", argv[k]);

    for(;;) {
      if (fgets(line, LEN, fpin) == NULL) break;
      char *s0 = replaceAll(line, "%ISA%", isastring);
      char *s1 = replaceAll(s0, "store(", "stream(");
      char *s2 = replaceAll(s1, "scatter(", "scstream(");
      fputs(s2, fpout);
      free(s0); free(s1); free(s2);
    }
    
    fclose(fpin);
    fclose(fpout);
  }

  for(int k=isastart;k<argc;k++) {
    sprintf(line, "unroll1_%s.c", argv[k]);
    FILE *fpout = fopen(line, "w");
    fputs("#include \"simd.h\"\n\n", fpout);
    fputs("extern SleefDFT_real ctbl[];\n\n", fpout);

    for(int shift=0;shift<=unrollmax;shift++) {
      FILE *fpin = fopen("unroll1.org", "r");

      for(;;) {
	if (fgets(line, LEN, fpin) == NULL) break;
	char *s0 = replaceAll(line, "%ISA%", argv[k]);

	char numstring[20];
	sprintf(numstring, "%d", shift);
	char *s1 = replaceAll(s0, "%SHIFT%", numstring);
	fputs(s1, fpout);
	free(s0); free(s1);
      }
      fclose(fpin);
    }
    
    fclose(fpout);
  }

  for(int k=isastart;k<argc;k++) {
    for(int outshift=0;outshift<=unrollmax2;outshift++) {
      sprintf(line, "unroll2_%d_%s.c", outshift, argv[k]);
      FILE *fpout = fopen(line, "w");
      fputs("#include \"simd.h\"\n\n", fpout);
      fputs("extern SleefDFT_real ctbl[];\n\n", fpout);

      for(int inshift=0;inshift<=unrollmax2;inshift++) {
	FILE *fpin = fopen("unroll2.org", "r");

	for(;;) {
	  if (fgets(line, LEN, fpin) == NULL) break;
	  char *s0 = replaceAll(line, "%ISA%", argv[k]);

	  char numstring[20];
	  sprintf(numstring, "%d", inshift);
	  char *s1 = replaceAll(s0, "%INSHIFT%", numstring);
	  sprintf(numstring, "%d", outshift);
	  char *s2 = replaceAll(s1, "%OUTSHIFT%", numstring);
	  fputs(s2, fpout);
	  free(s0); free(s1); free(s2);
	}
	fclose(fpin);
      }
      fclose(fpout);
    }
  }
}
