ifndef INSTALLROOT
	INSTALLROOT=${HOME}
endif

SOURCES=d_run.c dictionary.h dictionary.c makefile
DOCS=d1.txt d2.txt README
DIRS=ht clist

all: d_run

d_run: $(SOURCES)
	make clean
	if [ ! -d "${INSTALLROOT}/include" ]; then mkdir ${INSTALLROOT}/include; fi
	if [ ! -d "${INSTALLROOT}/lib" ]; then mkdir ${INSTALLROOT}/lib; fi
	cd clist && make install
	cd ht && make install
	if [ ! -f "${INSTALLROOT}/include" ]; then /bin/cp dictionary.h ${INSTALLROOT}/include; fi
	gcc -g -c -Iht dictionary.c
	ar rcs ${INSTALLROOT}/lib/libdictionary.a dictionary.o
	gcc -g -Wall $< -o $@ -I${INSTALLROOT}/include -L${INSTALLROOT}/lib \
	-ldictionary -lchained_hts -llinked_clists

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
