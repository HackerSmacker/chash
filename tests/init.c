#include "common.h"

int main(void) {
    struct StringHashtable *hashtable = chash_init(hashtable, STRING_TABLE);

    assert(hashtable->length == 0);
    assert(hashtable->capacity == CHASH_INITIAL_SIZE);
    assert(hashtable->buckets != NULL);

    return EXIT_SUCCESS;
}
