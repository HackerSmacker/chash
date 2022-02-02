#include "common.h"

int main(void) {
    struct StringHashtable *hashtable = chash_init(hashtable);

    assert(hashtable->logical_size == 0);
    assert(hashtable->physical_size == CHASH_INITIAL_SIZE);
    assert(hashtable->buckets != NULL);

    return EXIT_SUCCESS;
}
