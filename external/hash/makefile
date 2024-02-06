ifeq ($(MINGW),1)
 COMPILER = x86_64-w64-mingw32-gcc
 LIBNAME = win/lib$(NAME).dll
 TESTNAME = win/tests.exe
 OBJDIR = win
else
 COMPILER = gcc
 LIBNAME = linux/lib$(NAME).so
 TESTNAME = linux/tests
 OBJDIR = linux
endif

ARGS = -Wall -Wextra -std=c11 -O3
PWD = $(shell pwd)
NAME = hash

linux:
	mkdir -p linux

win:
	mkdir -p win

all: $(LIBNAME)

$(LIBNAME): linux win $(OBJDIR)/sha1.o $(OBJDIR)/sha256_internal.o $(OBJDIR)/sha224.o $(OBJDIR)/sha256.o $(OBJDIR)/sha512_internal.o $(OBJDIR)/sha384.o $(OBJDIR)/sha512.o $(OBJDIR)/sha512_224.o $(OBJDIR)/sha512_256.o
	$(COMPILER) $(ARGS) -shared -Wl,-soname,$(LIBNAME) -o $(LIBNAME) $(OBJDIR)/sha1.o $(OBJDIR)/sha256_internal.o $(OBJDIR)/sha224.o $(OBJDIR)/sha256.o $(OBJDIR)/sha512_internal.o $(OBJDIR)/sha384.o $(OBJDIR)/sha512.o $(OBJDIR)/sha512_224.o $(OBJDIR)/sha512_256.o

$(OBJDIR)/sha1.o: sha1.c
	$(COMPILER) $(ARGS) -c -fPIC sha1.c -o $(OBJDIR)/sha1.o

$(OBJDIR)/sha256_internal.o: sha256_internal.c
	$(COMPILER) $(ARGS) -c -fPIC sha256_internal.c -o $(OBJDIR)/sha256_internal.o

$(OBJDIR)/sha512_internal.o: sha512_internal.c
	$(COMPILER) $(ARGS) -c -fPIC sha512_internal.c -o $(OBJDIR)/sha512_internal.o

$(OBJDIR)/sha224.o: sha224.c
	$(COMPILER) $(ARGS) -c -fPIC sha224.c -o $(OBJDIR)/sha224.o

$(OBJDIR)/sha256.o: sha256.c
	$(COMPILER) $(ARGS) -c -fPIC sha256.c -o $(OBJDIR)/sha256.o

$(OBJDIR)/sha384.o: sha384.c
	$(COMPILER) $(ARGS) -c -fPIC sha384.c -o $(OBJDIR)/sha384.o

$(OBJDIR)/sha512.o: sha512.c
	$(COMPILER) $(ARGS) -c -fPIC sha512.c -o $(OBJDIR)/sha512.o

$(OBJDIR)/sha512_224.o: sha512_224.c
	$(COMPILER) $(ARGS) -c -fPIC sha512_224.c -o $(OBJDIR)/sha512_224.o

$(OBJDIR)/sha512_256.o: sha512_256.c
	$(COMPILER) $(ARGS) -c -fPIC sha512_256.c -o $(OBJDIR)/sha512_256.o

clean:
	rm -f $(OBJDIR)/*.o $(LIBNAME) $(TESTNAME)

run-tests: build-tests
ifeq ($(MINGW),1)
	export LD_LIBRARY_PATH=$$(pwd):$$LD_LIBRARY_PATH; wine ./$(TESTNAME)
else
	export LD_LIBRARY_PATH=$$(pwd):$$LD_LIBRARY_PATH; ./$(TESTNAME)
endif

valgrind: build-tests
 export LD_LIBRARY_PATH=$$(pwd):$$LD_LIBRARY_PATH; valgrind --leak-check=full ./$(TESTNAME)

build-tests: $(TESTNAME)

$(TESTNAME): $(LIBNAME) $(OBJDIR)/tests.o
	$(COMPILER) $(ARGS) -o $(TESTNAME) -L$(PWD)/$(OBJDIR) $(OBJDIR)/tests.o -l$(NAME)

$(OBJDIR)/tests.o: tests.c
	$(COMPILER) $(ARGS) -c tests.c -o $(OBJDIR)/tests.o

commit: clean
	git add *
	git commit -a