CC ?= cc
CFLAGS ?= -ansi -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wdouble-promotion -Wno-unused-parameter

TESTS=tests/stack_init.out tests/init.out tests/stack_assign.out

.PHONY: all clean check

all: $(TESTS)

clean:
	$(RM) $(TESTS)

check: $(TESTS)
	./scripts/check.sh

.c.out:
	$(CC) $< $(CFLAGS) -o $@
