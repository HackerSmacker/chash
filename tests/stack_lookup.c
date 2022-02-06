#include "stack_common.h"

int main(void) {
    int value = -1;
    struct StringBucket buckets[4] = {0};
    struct StringHashtable hashtable = chash_init(&hashtable, 4, buckets);

    chash_assign(&hashtable, "foo", 1, STRING_SETTINGS);
    chash_assign(&hashtable, "bar", 2, STRING_SETTINGS);
    chash_assign(&hashtable, "baz", 3, STRING_SETTINGS);
    chash_assign(&hashtable, "tuna", 4, STRING_SETTINGS);

    value = chash_lookup(&hashtable, "foo", value, STRING_SETTINGS);
    assert(value == 1);

    value = chash_lookup(&hashtable, "spam", value, STRING_SETTINGS);
    assert(value == 1);


    return EXIT_SUCCESS;
}
