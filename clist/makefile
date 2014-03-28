ifdef NDEBUG
  ndebug=-DNDEBUG
else
  ndebug=
endif

ifndef INSTALLROOT
  INSTALLROOT=${HOME}
endif

sources = clist.h clist.c makefile any.h README
derived = clist.o

all: clist.o

clist.o: clist.h clist.c any.h
	gcc -g -c ${ndebug} clist.c

clean:
	/bin/rm -f $(derived) *.o

install:
	make clean
	make all
	if [ ! -d "${INSTALLROOT}/include" ]; then mkdir ${INSTALLROOT}/include; fi
	if [ ! -f "${INSTALLROOT}/include/any.h" ]; then /bin/cp any.h ${INSTALLROOT}/include; fi
	if [ ! -d "${INSTALLROOT}/lib" ]; then mkdir ${INSTALLROOT}/lib; fi
	/bin/cp clist.h ${INSTALLROOT}/include
	ar rcs ${INSTALLROOT}/lib/liblinked_clists.a clist.o
	
zip:
	/bin/rm -rf ds
	mkdir ds
	mkdir ds/clist
	cp $(sources) ds/clist
	tar cvf ds.clist.tar ds
	/bin/rm -f ds.clist.tar.gz
	gzip ds.clist.tar
	/bin/rm -rf ds

