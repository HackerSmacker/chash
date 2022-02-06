#ifndef CWARE_HASH_STACK_COMMON_H
#define CWARE_HASH_STACK_COMMON_H

#define CHASH_STACKFUL

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../chash.h"

#define STRING_SETTINGS \
    char *, int, struct StringBucket, chash_string_hash(), chash_string_compare(), NULL, NULL, {0}, 0

struct StringBucket {
    char *key;
    int value;
    int state;
};

struct StringHashtable {
    unsigned int logical_size;
    unsigned int physical_size;
    struct StringBucket *buckets;
};

#endif
