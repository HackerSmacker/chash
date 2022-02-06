#include "stack_common.h"

int main(void) {
    struct StringBucket buckets[10] = {0};
    struct StringHashtable hashtable = chash_init(&hashtable, 10, buckets);

    assert(hashtable.logical_size == 0);

    /* Basic assignment */
    chash_assign(&hashtable, "foo", 3, STRING_SETTINGS);
    assert(hashtable.logical_size == 1);

    chash_assign(&hashtable, "bar", 1, STRING_SETTINGS);
    assert(hashtable.logical_size == 2);

    chash_assign(&hashtable, "baz", 2, STRING_SETTINGS);
    assert(hashtable.logical_size == 3);

    assert(strcmp(hashtable.buckets[2].key, "baz") == 0);
    assert(hashtable.buckets[2].value == 2);

    assert(strcmp(hashtable.buckets[4].key, "bar") == 0);
    assert(hashtable.buckets[4].value == 1);

    assert(strcmp(hashtable.buckets[9].key, "foo") == 0);
    assert(hashtable.buckets[9].value == 3);

    /* Replacing keys */
    chash_assign(&hashtable, "foo", 2, STRING_SETTINGS);
    assert(hashtable.logical_size == 3);

    assert(strcmp(hashtable.buckets[2].key, "baz") == 0);
    assert(hashtable.buckets[2].value == 2);

    assert(strcmp(hashtable.buckets[4].key, "bar") == 0);
    assert(hashtable.buckets[4].value == 1);

    assert(strcmp(hashtable.buckets[9].key, "foo") == 0);
    assert(hashtable.buckets[9].value == 2);

    /* Make sure more items can be added */
    chash_assign(&hashtable, "tuna", 4, STRING_SETTINGS);
    assert(hashtable.logical_size == 4);

    chash_assign(&hashtable, "spam", 5, STRING_SETTINGS);
    assert(hashtable.logical_size == 5);

    chash_assign(&hashtable, "thud", 5, STRING_SETTINGS);
    assert(hashtable.logical_size == 6);

    return EXIT_SUCCESS;
}
