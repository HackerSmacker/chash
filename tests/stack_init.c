#include "stack_common.h"

int main(void) {
    struct StringBucket buckets[10] = {0};
    struct StringHashtable hashtable = chash_init(&hashtable, 10, buckets);

    assert(hashtable.logical_size == 0);
    assert(hashtable.physical_size == 10);
    assert(hashtable.buckets == buckets);

    return EXIT_SUCCESS;
}
