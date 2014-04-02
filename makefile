ifdef NDEBUG
	ndebug=-DNDEBUG
else
	ndebug=
endif

ifndef INSTALLROOT
	INSTALLROOT=${HOME}
endif

SOURCES=d_run.c dictionary.h dictionary.c makefile
DOCS=d1.txt d2.txt README
HT_DIR=ht
DIRS=ht clist

d_run: $(SOURCES)
	gcc -g -Wall $< -o $@ -I${INSTALLROOT}/include -L${INSTALLROOT}/lib \
	-ldictionary -lchained_hts -llinked_clists

all: d_run

dictionary.o:
	gcc -g -c -I${HT_DIR} dictionary.c

install:
	make clean
	make dictionary.o
	if [ ! -d "${INSTALLROOT}/include" ]; then mkdir ${INSTALLROOT}/include; fi
	if [ ! -d "${INSTALLROOT}/lib" ]; then mkdir ${INSTALLROOT}/lib; fi
	if [ ! -f "${INSTALLROOT}/lib/liblinked_clists.a" ]; then cd clist && make install; fi
	if [ ! -f "${INSTALLROOT}/lib/libchained_hts.a" ]; then cd ht && make install; fi
	/bin/cp dictionary.h ${INSTALLROOT}/include
	ar rcs ${INSTALLROOT}/lib/libdictionary.a dictionary.o
	make all
	if [ -d "d_run.dSYM" ]; then /bin/rm -r d_run.dSYM; fi

zip:
	/bin/rm -rf p12202749
	mkdir p12202749
	cp $(SOURCES) $(DOCS) p12202749
	cp -r $(DIRS) p12202749
	/bin/rm -f p12202749.tar.gz
	tar czf p12202749.tar.gz p12202749
	/bin/rm -rf p12202749

# Will execute the clean target even if there is a file named "clean" in the
# current working directory.
.PHONY: clean
clean:
	/bin/rm -f d_run *.o
	cd clist && make clean
	cd ht && make clean
