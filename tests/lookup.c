#include "common.h"

int main(void) {
    int value = -1;
    struct StringHashtable *hashtable = NULL;
    struct StringHashtable *default_hashtable = NULL;

    hashtable = chash_init(hashtable, STRING_TABLE);
    default_hashtable = chash_init(default_hashtable, DEFAULT_STRING_TABLE);

    chash_assign(hashtable, "foo", 0, STRING_TABLE);
    chash_assign(hashtable, "bar", 1, STRING_TABLE);
    chash_assign(hashtable, "baz", 2, STRING_TABLE);

    value = chash_lookup(hashtable, "bar", value, STRING_TABLE);
    assert(value == 1);

    value = chash_lookup(hashtable, "foo", value, STRING_TABLE);
    assert(value == 0);

    value = chash_lookup(hashtable, "baz", value, STRING_TABLE);
    assert(value == 2);

    /* Lookup with a default */
    value = chash_lookup(hashtable, "tuna", value, DEFAULT_STRING_TABLE);
    assert(value == -5);

    return EXIT_SUCCESS;
}
