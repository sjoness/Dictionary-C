ifdef NDEBUG
	ndebug=-DNDEBUG
else
	ndebug=
endif

ifndef INSTALLROOT
	INSTALLROOT=${HOME}
endif

CC=gcc
CFLAGS=-g -c
SOURCES=d_run.c dictionary.h dictionary.c
DOCS=d1.txt d2.txt makefile README.md
HT_DIR=ht
DIRS=ht clist
DERIVED=dictionary.o
EXECUTABLE=d_run

$(EXECUTABLE): $(SOURCES)
	$(CC) -g -Wall $< -o $@ -I${INSTALLROOT}/include -L${INSTALLROOT}/lib -ldictionary -llinked_clists -lchained_hts

all: $(EXECUTABLE)

$(DERIVED):
	$(CC) $(CFLAGS) -I${HT_DIR} dictionary.c

install:
	make clean
	make $(DERIVED)
	if [ ! -d "${INSTALLROOT}/include" ]; then mkdir ${INSTALLROOT}/include; fi
	if [ ! -d "${INSTALLROOT}/lib" ]; then mkdir ${INSTALLROOT}/lib; fi
	/bin/cp dictionary.h ${INSTALLROOT}/include
	ar rcs ${INSTALLROOT}/lib/libdictionary.a $(DERIVED)
	make all

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
	/bin/rm -f $(EXECUTABLE) $(DERIVED) *.o
