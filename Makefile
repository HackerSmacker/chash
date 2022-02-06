CC ?= cc
CFLAGS ?= -ansi -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wdouble-promotion -Wno-unused-parameter -Wno-unused-variable -Wno-unused-but-set-variable -g

TESTS=tests/stack_init.out tests/init.out tests/stack_assign.out tests/assign.out \
	  tests/lookup.out tests/stack_lookup.out

.PHONY: all clean check

all: $(TESTS)

clean:
	$(RM) $(TESTS)

check: $(TESTS)
	./scripts/check.sh

.c.out:
	$(CC) $< $(CFLAGS) -o $@
