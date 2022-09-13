ifeq ($(CC),cc)
CC=gcc
endif

CFLAGS = -Wall -Wno-attributes -Wno-unused -O3 -ffp-contract=off
#CFLAGS = -Wall -Wno-attributes -Wno-unused -O3 -ffast-math
#CFLAGS = -Wall -Wno-attributes -Wno-unused -g -I./dft
#CFLAGS += -DBIGENDIAN
#CFLAGS += -DNDEBUG

all : ssrc ssrc_hp

dft.a :
	cd dft;"$(MAKE)" -f Makefile.double CC=$(CC) CFLAGS="$(CFLAGS)" clean dft.a
	mv dft/dft.a .

dftsp.a :
	cd dft;"$(MAKE)" -f Makefile.float CC=$(CC) CFLAGS="$(CFLAGS)" clean dftsp.a
	mv dft/dftsp.a .

dither.o : dither.c
	$(CC) $(CFLAGS) -std=c99 -c dither.c -o dither.o

prng.o : prng.c
	$(CC) $(CFLAGS) -std=c99 -c prng.c -o prng.o

ssrc : ssrc.c dftsp.a dither.o prng.o
	$(CC) $(CFLAGS) -I./dft -DSLEEFDFT_REAL_IS_FLOAT ssrc.c dither.o prng.o dftsp.a -o ssrc -lm
	strip ssrc

ssrc_hp : ssrc.c dft.a dither.o prng.o
	$(CC) $(CFLAGS) -I./dft -DHIGHPREC ssrc.c dither.o prng.o dft.a -o ssrc_hp -lm
	strip ssrc_hp

clean :
	rm -f ssrc ssrc_hp ssrc.exe ssrc_hp.exe *.o *~ *.a *.wav *.pkf
	rm -rf testdir *.dSYM
	cd dft;"$(MAKE)" -f Makefile.float clean

test : ssrc ssrc_hp
	rm -rf testdir; mkdir testdir
	cp ../test.wav testdir
	./ssrc_hp --rate 48000 --bits 8 --dither 6 testdir/test.wav testdir/test0.wav
	./ssrc_hp --rate 44100 --bits 16 testdir/test0.wav testdir/test1.wav
	./ssrc_hp --rate 96000 --bits 24 testdir/test1.wav testdir/test2.wav
	./ssrc_hp --rate 88200 --bits 8 --dither 2 --bits 8 testdir/test2.wav testdir/test3.wav
	./ssrc_hp --rate 48000 --bits 24 testdir/test3.wav testdir/test4.wav
	./ssrc --rate 88200 --bits 16 testdir/test4.wav testdir/test5.wav
	./ssrc --rate 96000 testdir/test5.wav testdir/test6.wav
	./ssrc --rate 44100 --bits 8 --dither 16 testdir/test6.wav testdir/test7.wav
