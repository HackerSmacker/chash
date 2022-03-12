# This Makefile is the output of a template Makefile that was
# processed by the m4 macro preprocessor. It is not intended
# to be human readable.

CC=cc
PREFIX=/usr/local
CFLAGS=-ansi -Wall -Wextra -Wshadow -Wdouble-promotion -fpic -Wno-unused-variable -Wno-unused-function -Wno-sign-compare
TESTS=tests/free.out tests/assign.out tests/stack_init.out tests/stack_lookup.out tests/stack_contains.out tests/stack_assign.out tests/stack_free.out tests/lookup.out tests/contains.out tests/init.out 
DOCS=./doc/chash.cware 
MANNAMES=chash.cware 
DEBUGGER=

.PHONY: all clean check install
.SUFFIXES: .c .out

all: $(TESTS) $(DOCS)

clean:
	rm -f $(TESTS)

check:
	./scripts/check.sh $(DEBUGGER)

install:
	cp $(DOCS) $(PREFIX)/share/man/mancware

uninstall:
	for manual in $(MANNAMES); do  \
		rm -f $(PREFIX)/share/man/mancware/$$manual; \
	done

.c.out:
	$(CC) $< $(OBJS) $(CFLAGS) -o $@
