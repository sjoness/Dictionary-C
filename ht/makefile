ifdef NDEBUG
  ndebug=-DNDEBUG
else
  ndebug=
endif

ifndef INSTALLROOT
  INSTALLROOT=${HOME}
endif

sources = ht.h open_ht.c chained_ht.c any.h makefile README
derived = open_ht.o chained_ht.o
CLIST_DIR = ../clist

all: chained_ht.o

open_ht.o: ht.h open_ht.c
	gcc -c -g ${ndebug} open_ht.c

chained_ht.o: ht.h chained_ht.c
	gcc -c -g -I${CLIST_DIR} ${ndebug} chained_ht.c

clean:
	/bin/rm -f $(derived) *.o

install:
	make clean
	make all
	if [ ! -d "${INSTALLROOT}/include" ]; then mkdir ${INSTALLROOT}/include; fi
	if [ ! -f "${INSTALLROOT}/include/any.h" ]; then /bin/cp any.h ${INSTALLROOT}/include; fi
	if [ ! -d "${INSTALLROOT}/lib" ]; then mkdir ${INSTALLROOT}/lib; fi
	/bin/cp ht.h ${INSTALLROOT}/include
	ar rcs ${INSTALLROOT}/lib/libchained_hts.a chained_ht.o

zip:
	/bin/rm -rf ds
	mkdir ds
	mkdir ds/ht
	cp $(sources) ds/ht
	tar cvf ds.ht.tar ds
	/bin/rm -f ds.ht.tar.gz
	gzip ds.ht.tar
	/bin/rm -rf ds

