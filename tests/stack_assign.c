#include "stack_common.h"

void read(struct StringHashtable table) {
    int index = 0;

    for(index = 0; index < table.physical_size; index++) {
        if(table.buckets[index].state != CHASH_FILLED) {
            printf("%i NULL: 0\n", index);

            continue;
        }

        printf("%i %s: %i\n", index, table.buckets[index].key, table.buckets[index].value);
    }
}

int main(void) {
    struct StringBucket buckets[10] = {0};
    struct StringHashtable hashtable = chash_init(&hashtable, 10, buckets);

    /* Basic assignment */
    chash_assign(&hashtable, "foo", 3, STRING_SETTINGS);
    chash_assign(&hashtable, "bar", 1, STRING_SETTINGS);
    chash_assign(&hashtable, "baz", 2, STRING_SETTINGS);

    assert(strcmp(hashtable.buckets[2].key, "baz") == 0);
    assert(hashtable.buckets[2].value == 2);

    assert(strcmp(hashtable.buckets[4].key, "bar") == 0);
    assert(hashtable.buckets[4].value == 1);

    assert(strcmp(hashtable.buckets[9].key, "foo") == 0);
    assert(hashtable.buckets[9].value == 3);

    /* Replacing keys */
    chash_assign(&hashtable, "foo", 2, STRING_SETTINGS);

    assert(strcmp(hashtable.buckets[2].key, "baz") == 0);
    assert(hashtable.buckets[2].value == 2);

    assert(strcmp(hashtable.buckets[4].key, "bar") == 0);
    assert(hashtable.buckets[4].value == 1);

    assert(strcmp(hashtable.buckets[9].key, "foo") == 0);
    assert(hashtable.buckets[9].value == 2);

    return EXIT_SUCCESS;
}
