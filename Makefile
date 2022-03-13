# This Makefile is the output of a template Makefile that was
# processed by the m4 macro preprocessor. It is not intended
# to be human readable.

CC=cc
PREFIX=/usr/local
# CFLAGS=-ansi -Wall -Wextra -Wshadow -Wdouble-promotion -fpic -Wno-unused-variable -Wno-unused-function -Wno-sign-compare
TESTS=tests/free.out tests/assign.out tests/struct_values.out tests/stack_init.out tests/stack_lookup.out tests/stack_contains.out tests/stack_assign.out tests/stack_free.out tests/lookup.out tests/delete.out tests/contains.out tests/stack_delete.out tests/init.out 
DOCS=doc/chash.cware doc/chash_init_stack.cware doc/chash_init.cware doc/chash_assign.cware 
MANNAMES=chash.cware chash_init_stack.cware chash_init.cware chash_assign.cware 
DEBUGGER=

.PHONY: all clean check install
.SUFFIXES: .c .out

all: $(TESTS) $(DOCS)

clean:
	rm -f $(TESTS)

check:
	./scripts/check.sh $(DEBUGGER)

install:
	install -d -m 755 $(PREFIX)/include/cware/chash
	install -m 755 src/chash.h $(PREFIX)/include/cware/chash
	install -d -m 755 $(PREFIX)/share/man/mancware
	for manual in $(MANNAMES); do \
		install -m 755 doc/$$manual $(PREFIX)/share/man/mancware; \
	done


uninstall:
	rm -f $(PREFIX)/include/cware/chash/chash.h
	for manual in $(MANNAMES); do  \
		rm -f $(PREFIX)/share/man/mancware/$$manual; \
	done
	rmdir $(PREFIX)/share/man/mancware
	rmdir $(PREFIX)/include/cware/chash

.c.out:
	$(CC) $< $(OBJS) $(CFLAGS) -o $@
