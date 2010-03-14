CPPFLAGS=-Wall -g -O3
CFLAGS=-DPACKAGE_NAME=\"\" -DPACKAGE_TARNAME=\"\" -DPACKAGE_VERSION=\"\" -DPACKAGE_STRING=\"\" -DPACKAGE_BUGREPORT=\"\" -DSTDC_HEADERS=1 -DHAVE_INTELCPU=1 -DHAVE_MMX=1 
CXXFLAGS=-I$(QTDIR)/include -DPACKAGE_NAME=\"\" -DPACKAGE_TARNAME=\"\" -DPACKAGE_VERSION=\"\" -DPACKAGE_STRING=\"\" -DPACKAGE_BUGREPORT=\"\" -DSTDC_HEADERS=1 -DHAVE_INTELCPU=1 -DHAVE_MMX=1 
ASFLAGS=-g
LDFLAGS=-g 
LIBS=
QTLIBS=-L$(QTDIR)/lib -lqt
CC=gcc
CXX=g++
XLIBS=-L/usr/X11R6/lib -lX11

LIBOBJS=	ccvt_mmx.o ccvt_misc.o


all: 	libccvt.a

libccvt.a:	$(LIBOBJS)
	ar rcv $@ $(LIBOBJS)
	ranlib $@

bench:	bench_c1 bench_c2 bench_mmx
show:	show_c1 show_c2 show_mmx
.PHONY: bench show

bench_c1:	bench.o ccvt_c1.o
	g++ $(LDFLAGS) -o $@ bench.o ccvt_c1.o

bench_c2:	bench.o ccvt_c2.o
	g++ $(LDFLAGS) -o $@ bench.o ccvt_c2.o

bench_mmx:	bench.o ccvt_mmx.o
	g++ $(LDFLAGS) -o $@ bench.o ccvt_mmx.o

show_c1:	show.o ccvt_c1.o
	g++ $(LDFLAGS) -o $@ show.o ccvt_c1.o $(QTLIBS) $(XLIBS)

show_c2:	show.o ccvt_c2.o
	g++ $(LDFLAGS) -o $@ show.o ccvt_c2.o $(QTLIBS) $(XLIBS)

show_mmx:	show.o ccvt_mmx.o
	g++ $(LDFLAGS) -o $@ show.o ccvt_mmx.o $(QTLIBS) $(XLIBS)

.S.o:
	gcc -c $(CFLAGS) -o $@ $<

clean:
	rm -f *.o *.so *.a *~ core *.bak
	rm -f bench_c1 bench_c2 bench_mmx show_c1 show_c2 show_mmx

dep:
	# dummy

distclean:	clean
	rm -f config.cache config.status config.log Makefile
