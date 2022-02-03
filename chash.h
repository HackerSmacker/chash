#ifndef CWARE_HASH_H
#define CWARE_HASH_H

/* How big heap-allocated hashtables are by default */
#ifndef CHASH_INITIAL_SIZE
#define CHASH_INITIAL_SIZE 10
#endif

/* Calculates the next size of the hashtable. */
#ifndef CHASH_RESIZE
#define CHASH_RESIZE(size) \
    ((size) * 1.3)
#endif

/* The threshold that, when passed, will cause a resize */
#ifndef CHASH_LOAD_THRESHOLD
#define CHASH_LOAD_THRESHOLD 0.8F
#endif

/* State enums */
#define CHASH_UNFILLED  0
#define CHASH_FILLED    1
#define CHASH_TOMBSTONE 2

#define chash_string_hash()                                                           \
    5031;                                                                             \
    do {                                                                              \
        int index = 0;                                                                \
                                                                                      \
        for(index = 0; __CHASH_KEY[index] != '\0'; index++) {                         \
            __CHASH_HASH = ((__CHASH_HASH << 5) + __CHASH_HASH) + __CHASH_KEY[index]; \
        }                                                                             \
    } while(0)

#define chash_string_compare() \
    (strcmp(__CHASH_OPERAND_A, __CHASH_OPERAND_B) == 0)

/* chash_init */
#ifdef CHASH_STACKFUL
#define chash_init(chash, length, block) \
    {0};                                                                                                   \
                                                                                                           \
    if((length) <= 0) {                                                                                    \
        fprintf(stderr, "chash_init: hashtable '%s' cannot have a maximum length of 0 or less\n", #chash); \
        exit(EXIT_FAILURE);                                                                                \
    }                                                                                                      \
                                                                                                           \
    if((block) == NULL) {                                                                                  \
        fprintf(stderr, "chash_init: hashtable '%s' block cannot be null\n", #chash);                      \
        exit(EXIT_FAILURE);                                                                                \
    }                                                                                                      \
                                                                                                           \
    (chash)->physical_size = length;                                                                       \
    (chash)->buckets = block 
#else
#define chash_init(chash)                                                                               \
    NULL;                                                                                               \
                                                                                                        \
    if(CHASH_INITIAL_SIZE <= 0) {                                                                       \
        fprintf(stderr, "chash_init: hashtable '%s' default length must be greater than 0.\n", #chash); \
        exit(EXIT_FAILURE);                                                                             \
    }                                                                                                   \
                                                                                                        \
    (chash) = calloc(1, sizeof((*chash)));                                                              \
    (chash)->physical_size = CHASH_INITIAL_SIZE;                                                        \
    (chash)->buckets = calloc(CHASH_INITIAL_SIZE, sizeof((*(chash)).buckets))
#endif

/* chash_assign */
#define __chash_assign(chash, _key, _value, _key_type, _value_type, _bucket_type, _hash, _compare, _free_key, _free_value, _default, _uses_default) \
do {                                                                                                                                                \
    long __CHASH_HASH = 0;                                                                                                                          \
    _key_type __CHASH_KEY = (_key);                                                                                                                 \
                                                                                                                                                    \
    if((chash) == NULL) {                                                                                                                           \
        fprintf(stderr, "chash_assign: hashtable '%s' cannot be NULL. (%s:%i)\n", #chash, __FILE__, __LINE__);                                      \
        exit(EXIT_FAILURE);                                                                                                                         \
    }                                                                                                                                               \
                                                                                                                                                    \
    __CHASH_HASH = _hash;                                                                                                                           \
    __CHASH_HASH = __CHASH_HASH % (chash)->physical_size;                                                                                           \
                                                                                                                                                    \
    while(1) {                                                                                                                                      \
        _key_type __CHASH_OPERAND_A = (_key);                                                                                                       \
        _key_type __CHASH_OPERAND_B = (chash)->buckets[__CHASH_HASH].key;                                                                           \
                                                                                                                                                    \
        if((chash)->buckets[__CHASH_HASH].state != CHASH_FILLED)                                                                                    \
            break;                                                                                                                                  \
                                                                                                                                                    \
        if((_compare) == 1)                                                                                                                         \
            break;                                                                                                                                  \
                                                                                                                                                    \
        __CHASH_HASH = (__CHASH_HASH + 1) % (chash)->physical_size;                                                                                 \
    }                                                                                                                                               \
                                                                                                                                                    \
    if((chash)->buckets[__CHASH_HASH].state == CHASH_FILLED) {                                                                                      \
        _value_type __CHASH_VALUE = (chash)->buckets[__CHASH_HASH].value;                                                                           \
        (void) __CHASH_VALUE;                                                                                                                       \
                                                                                                                                                    \
        _free_value;                                                                                                                                \
    }                                                                                                                                               \
                                                                                                                                                    \
    (chash)->buckets[__CHASH_HASH].key = _key;                                                                                                      \
    (chash)->buckets[__CHASH_HASH].value = _value;                                                                                                  \
    (chash)->buckets[__CHASH_HASH].state = CHASH_FILLED;                                                                                            \
    (chash)->logical_size++;                                                                                                                        \
                                                                                                                                                    \
} while(0)

#define chash_assign(chash, key, value, settings) \
    __chash_assign(chash, key, value, settings)

#endif
