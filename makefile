ifndef INSTALLROOT
	INSTALLROOT=${HOME}
endif

SOURCES=d_run.c dictionary.h dictionary.c makefile
DOCS=d1.txt d2.txt README
HT_DIR=ht
DIRS=ht clist

all: dictionary.o d_run

d_run: $(SOURCES)
	gcc -g -Wall $< -o $@ -I${INSTALLROOT}/include -L${INSTALLROOT}/lib \
	-ldictionary -lchained_hts -llinked_clists

dictionary.o:
	gcc -g -c -I${HT_DIR} dictionary.c

install:
	make clean
	make all
	if [ ! -d "${INSTALLROOT}/include" ]; then mkdir ${INSTALLROOT}/include; fi
	if [ ! -d "${INSTALLROOT}/lib" ]; then mkdir ${INSTALLROOT}/lib; fi
	cd clist && make install
	cd ht && make install
	/bin/cp dictionary.h ${INSTALLROOT}/include
	ar rcs ${INSTALLROOT}/lib/libdictionary.a dictionary.o
	if [ -d "d_run.dSYM" ]; then /bin/rm -r d_run.dSYM; fi # For OS X systems

zip:
	/bin/rm -rf p12202749
	mkdir p12202749
	cp $(SOURCES) $(DOCS) p12202749
	cp -r $(DIRS) p12202749
	/bin/rm -f p12202749.tar.gz
	tar czf p12202749.tar.gz p12202749
	/bin/rm -rf p12202749

.PHONY: clean
clean:
	/bin/rm -f d_run *.o
	cd clist && make clean
	cd ht && make clean
