#ifndef CWARE_HASH_STACK_COMMON_H
#define CWARE_HASH_STACK_COMMON_H

#define CHASH_STACKFUL

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../chash.h"

struct StringBucket {
    char *key;
    int value;
    int tombstone;
};

struct StringHashtable {
    int logical_size;
    int physical_size;
    struct StringBucket *buckets;
};

#endif
