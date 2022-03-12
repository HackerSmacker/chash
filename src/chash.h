#ifndef CWARE_LIBCHASH_H
#define CWARE_LIBCHASH_H

/* How big heap-allocated hashtables are by default */
#ifndef CHASH_INITIAL_SIZE
#define CHASH_INITIAL_SIZE 10
#elif CHASH_INITIAL_SIZE <= 0 
        "chash_init: default length must be greater than 0."
#endif

/* Calculates the next size of the hashtable. */
#ifndef CHASH_RESIZE
#define CHASH_RESIZE(size) \
    ((size) * 1.3)
#endif

/* The threshold that, when passed, will cause a resize */
#ifndef CHASH_LOAD_THRESHOLD
#define CHASH_LOAD_THRESHOLD 0.8
#endif

/* State enums */
#define CHASH_UNFILLED  0
#define CHASH_FILLED    1
#define CHASH_TOMBSTONE 2

#define chash_string_hash(key, hash)                          \
    5031;                                                     \
    do {                                                      \
        int index = 0;                                        \
                                                              \
        for(index = 0; (key)[index] != '\0'; index++) {       \
            (hash) = (((hash) << 5) + (hash)) + (key)[index]; \
        }                                                     \
    } while(0)

#define chash_string_compare(cmp_a, cmp_b) \
    (strcmp((cmp_a), (cmp_b)) == 0)

#define chash_init(hashtable, namespace)                       \
    NULL;                                                      \
                                                               \
    (hashtable) = calloc(1, sizeof((*(hashtable))));           \
    (hashtable)->capacity = CHASH_INITIAL_SIZE;                \
    (hashtable)->buckets = calloc(CHASH_INITIAL_SIZE,          \
                              sizeof(*((hashtable)->buckets))) \

#define chash_init_stack(hashtable, buffer, length, namespace)               \
    (*(hashtable));                                                               \
                                                                             \
    if((length) <= 0) {                                                      \
        fprintf(stderr, "chash_init_stack: hashtable cannot have a maximum " \
                        "length"                                             \
                        " of 0 or less (%s:%i)\n", __FILE__, __LINE__);      \
        exit(EXIT_FAILURE);                                                  \
    }                                                                        \
                                                                             \
    if((buffer) == NULL) {                                                    \
        fprintf(stderr, "chash_init_stack: hashtable block cannot be null "  \
                        "(%s:%i) \n", __FILE__, __LINE__);                   \
        exit(EXIT_FAILURE);                                                  \
    }                                                                        \
                                                                             \
    (hashtable)->capacity = length;                                              \
    (hashtable)->buckets = buffer 


#define __chash_resize(hashtable, namespace)                                                                \
do {                                                                                                \
    if((namespace ## _HEAP) == 0) {                                                                 \
        if((hashtable)->length == (hashtable)->capacity) {                                       \
            fprintf(stderr, "%s", "__chash_resize: hashtable is full. could not resize\n");         \
            exit(EXIT_FAILURE);                                                                     \
        }                                                                                           \
    } else {                                                                                        \
        namespace ## _BUCKET *__CHASH_BUCKETS = NULL;                                                                                                                          \
        size_t __CHASH_INDEX = 0;                                                                   \
        unsigned int __CHASH_NEXT_SIZE = (unsigned int) CHASH_RESIZE((hashtable)->capacity);         \
                                                                                                    \
        if((double) (hashtable)->length / (double) (hashtable)->capacity < CHASH_LOAD_THRESHOLD) \
            break;                                                                                  \
                                                                                                    \
        __CHASH_BUCKETS = calloc(__CHASH_NEXT_SIZE, sizeof(namespace ## _BUCKET));                          \
                                                                                                    \
        for(__CHASH_INDEX = 0; __CHASH_INDEX < (hashtable)->capacity; __CHASH_INDEX++) {           \
            __CHASH_KEY_BUCKET.key = (hashtable)->buckets[__CHASH_INDEX].key;                                      \
                                                                                                    \
            if((hashtable)->buckets[__CHASH_INDEX].state != CHASH_FILLED)                               \
                continue;                                                                           \
                                                                                                    \
            __CHASH_HASH = namespace ## _HASH(__CHASH_KEY_BUCKET.key, __CHASH_HASH);                                                                   \
            __CHASH_HASH = __CHASH_HASH % __CHASH_NEXT_SIZE;                                        \
                                                                                                    \
            while(1) {                                                                              \
                if(__CHASH_BUCKETS[__CHASH_HASH].state != CHASH_FILLED)                             \
                    break;                                                                          \
                                                                                                    \
                if(namespace ## _COMPARE((hashtable)->buckets[__CHASH_INDEX].key, __CHASH_BUCKETS[__CHASH_HASH].key) == 1)                                                                 \
                    break;                                                                          \
                                                                                                    \
                __CHASH_HASH = (__CHASH_HASH + 1) % __CHASH_NEXT_SIZE;                              \
            }                                                                                       \
                                                                                                    \
            __CHASH_BUCKETS[__CHASH_HASH].key = (hashtable)->buckets[__CHASH_INDEX].key;                \
            __CHASH_BUCKETS[__CHASH_HASH].value = (hashtable)->buckets[__CHASH_INDEX].value;            \
            __CHASH_BUCKETS[__CHASH_HASH].state = CHASH_FILLED;                                     \
        }                                                                                           \
                                                                                                    \
        free((hashtable)->buckets);                                                                     \
        (hashtable)->buckets = __CHASH_BUCKETS;                                                         \
        (hashtable)->capacity = __CHASH_NEXT_SIZE;                                                 \
    }                                                                                               \
} while(0)

#define chash_assign(hashtable, _key, _value, namespace) \
do {                                                                                                                                                \
    long __CHASH_HASH = 0;                                                                                                                          \
    namespace ## _BUCKET __CHASH_KEY_BUCKET;                                                                                                                          \
    __CHASH_KEY_BUCKET.key = (_key);              \
    __CHASH_KEY_BUCKET.value = (_value);          \
    __CHASH_KEY_BUCKET.state = CHASH_UNFILLED; \
                                                                                                                                                    \
    if((hashtable) == NULL) {                                                                                                                           \
        fprintf(stderr, "chash_assign: hashtable cannot be NULL. (%s:%i)\n", __FILE__, __LINE__);                                      \
        exit(EXIT_FAILURE);                                                                                                                         \
    }                                                                                                                                               \
                                                                                                                                                    \
    __chash_resize(hashtable, namespace);    \
                                                                                                                                                    \
    __CHASH_KEY_BUCKET.key = (_key);                                                                                                                           \
    __CHASH_HASH = namespace ## _HASH(__CHASH_KEY_BUCKET.key, __CHASH_HASH);                                                                                                                           \
    __CHASH_HASH = __CHASH_HASH % (hashtable)->capacity;                                                                                           \
                                                                                                                                                    \
    while(1) {                                                                                                                                      \
        if((hashtable)->buckets[__CHASH_HASH].state != CHASH_FILLED)                                                                                    \
            break;                                                                                                                                  \
                                                                                                                                                    \
        if(namespace ## _COMPARE((_key), (hashtable)->buckets[__CHASH_HASH].key ) == 1)                                                                                                                         \
            break;                                                                                                                                  \
                                                                                                                                                    \
        __CHASH_HASH = (__CHASH_HASH + 1) % (hashtable)->capacity;                                                                                 \
    }                                                                                                                                               \
                                                                                                                                                    \
    if((hashtable)->buckets[__CHASH_HASH].state == CHASH_FILLED) {                                                                                      \
        namespace ## _FREE_VALUE((hashtable)->buckets[__CHASH_HASH].value); \
    } else {                                                                                                                                        \
        (hashtable)->length++;                                                                                                                    \
    }                                                                                                                                               \
                                                                                                                                                    \
    (hashtable)->buckets[__CHASH_HASH].key = _key;                                                                                                      \
    (hashtable)->buckets[__CHASH_HASH].value = _value;                                                                                                  \
    (hashtable)->buckets[__CHASH_HASH].state = CHASH_FILLED;                                                                                            \
} while(0)

#define chash_lookup(hashtable, _key, storage, namespace)    \
    storage;                                                                                                                                           \
                                                                                                                                                        \
    do {                                                                                                                                                \
        int __CHASH_INDEX = 0;                                                                                                                 \
        long __CHASH_HASH = 0;                                                                                                                          \
        namespace ## _BUCKET __CHASH_KEY_BUCKET = {(_key), namespace ## _DEFAULT, CHASH_UNFILLED};                                                                                                                          \
                                                                                                                                                        \
        if((hashtable) == NULL) {                                                                                                                           \
            fprintf(stderr, "chash_lookup: hashtable cannot be NULL. (%s:%i)\n", __FILE__, __LINE__);                                      \
            exit(EXIT_FAILURE);                                                                                                                         \
        }                                                                                                                                               \
                                                                                                                                                        \
        __CHASH_HASH = namespace ## _HASH(__CHASH_KEY_BUCKET.key, __CHASH_HASH);                                                                   \
        __CHASH_HASH = __CHASH_HASH % (hashtable)->capacity;                                                                                           \
                                                                                                                                                        \
        while((unsigned int) __CHASH_INDEX < (hashtable)->capacity) {                                                                                  \
            if((hashtable)->buckets[__CHASH_HASH].state == CHASH_UNFILLED)                                                                                  \
                break;                                                                                                                                  \
                                                                                                                                                        \
            if(namespace ## _COMPARE((_key), (hashtable)->buckets[__CHASH_HASH].key) == 1) {                                                                                                                       \
                __CHASH_INDEX = -1;                                                                                                                     \
                break;                                                                                                                                  \
            }                                                                                                                                           \
                                                                                                                                                        \
            __CHASH_HASH = (__CHASH_HASH + 1) % (hashtable)->capacity;                                                                                 \
            __CHASH_INDEX++;                                                                                                                            \
        }                                                                                                                                               \
                                                                                                                                                        \
        if(((hashtable)->buckets[__CHASH_HASH].state == CHASH_UNFILLED) || __CHASH_INDEX != -1) {                                                                                    \
            if((namespace ## _USES_DEFAULT) == 1)                                                                                                                      \
                break;                                                                                                                                  \
                                                                                                                                                        \
            fprintf(stderr, "chash_lookup: could not find key '%s' in hashtable. (%s:%i)\n'", #_key, __FILE__, __LINE__);                               \
            exit(EXIT_FAILURE);                                                                                                                         \
        }                                                                                                                                               \
                                                                                                                                                        \
        storage = (hashtable)->buckets[__CHASH_HASH].value;                                                                                                 \
    } while(0)

#endif