#include "common.h"

void read(struct StringHashtable table) {
    unsigned int index = 0;

    for(index = 0; index < table.physical_size; index++) {
        if(table.buckets[index].state != CHASH_FILLED) {
            printf("%i NULL: 0\n", index);

            continue;
        }

        printf("%i %s: %i\n", index, table.buckets[index].key, table.buckets[index].value);
    }
}

int main(void) {
    struct StringHashtable *hashtable = chash_init(hashtable);

    assert(hashtable->logical_size == 0);
    assert(hashtable->physical_size == CHASH_INITIAL_SIZE);

    /* Basic assignment */
    chash_assign(hashtable, "foo", 3, STRING_SETTINGS);
    assert(hashtable->logical_size == 1);
    assert(hashtable->physical_size == CHASH_INITIAL_SIZE);

    chash_assign(hashtable, "bar", 1, STRING_SETTINGS);
    assert(hashtable->logical_size == 2);
    assert(hashtable->physical_size == CHASH_INITIAL_SIZE);

    chash_assign(hashtable, "baz", 2, STRING_SETTINGS);
    assert(hashtable->logical_size == 3);
    assert(hashtable->physical_size == CHASH_INITIAL_SIZE);

    assert(strcmp(hashtable->buckets[2].key, "baz") == 0);
    assert(hashtable->buckets[2].value == 2);

    assert(strcmp(hashtable->buckets[4].key, "bar") == 0);
    assert(hashtable->buckets[4].value == 1);

    assert(strcmp(hashtable->buckets[9].key, "foo") == 0);
    assert(hashtable->buckets[9].value == 3);

    /* Replacing keys */
    chash_assign(hashtable, "foo", 2, STRING_SETTINGS);
    assert(hashtable->logical_size == 3);
    assert(hashtable->physical_size == CHASH_INITIAL_SIZE);

    assert(strcmp(hashtable->buckets[2].key, "baz") == 0);
    assert(hashtable->buckets[2].value == 2);
    assert(hashtable->physical_size == CHASH_INITIAL_SIZE);

    assert(strcmp(hashtable->buckets[4].key, "bar") == 0);
    assert(hashtable->buckets[4].value == 1);
    assert(hashtable->physical_size == CHASH_INITIAL_SIZE);

    assert(strcmp(hashtable->buckets[9].key, "foo") == 0);
    assert(hashtable->buckets[9].value == 2);
    assert(hashtable->physical_size == CHASH_INITIAL_SIZE);

    /* Make sure more items can be added */
    chash_assign(hashtable, "tuna", 4, STRING_SETTINGS);
    assert(hashtable->logical_size == 4);
    assert(hashtable->physical_size == CHASH_INITIAL_SIZE);

    chash_assign(hashtable, "spam", 5, STRING_SETTINGS);
    assert(hashtable->logical_size == 5);
    assert(hashtable->physical_size == CHASH_INITIAL_SIZE);

    chash_assign(hashtable, "thud", 5, STRING_SETTINGS);
    assert(hashtable->logical_size == 6);
    assert(hashtable->physical_size == CHASH_INITIAL_SIZE);

    chash_assign(hashtable, "quz", 6, STRING_SETTINGS);
    assert(hashtable->logical_size == 7);

    chash_assign(hashtable, "foobar", 7, STRING_SETTINGS);
    assert(hashtable->logical_size == 8);
    assert(hashtable->physical_size == CHASH_INITIAL_SIZE);

    /* Should trigger a resize */
    chash_assign(hashtable, "tuna", 7, STRING_SETTINGS);
    assert(hashtable->logical_size == 8);
    assert(hashtable->physical_size == CHASH_INITIAL_SIZE + 3);

    /* Previous keys should not be in the same places */
    assert(strcmp(hashtable->buckets[10].key, "foo") == 0);
    assert(hashtable->buckets[10].value == 2);

    assert(strcmp(hashtable->buckets[0].key, "baz") == 0);
    assert(hashtable->buckets[0].value == 2);

    assert(strcmp(hashtable->buckets[3].key, "foobar") == 0);
    assert(hashtable->buckets[3].value == 7);

    assert(strcmp(hashtable->buckets[1].key, "spam") == 0);
    assert(hashtable->buckets[1].value == 5);

    return EXIT_SUCCESS;
}
