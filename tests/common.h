#ifndef CWARE_HASH_COMMON_H
#define CWARE_HASH_COMMON_H

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
    int logical_size;
    int physical_size;
    struct StringBucket *buckets;
};

#endif
