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
#define chash_init(chash)                        \
    calloc(1, sizeof((*chash)));                 \
    (chash)->physical_size = CHASH_INITIAL_SIZE; \
    (chash)->buckets = calloc(CHASH_INITIAL_SIZE, sizeof((*(chash)).buckets))
#endif

/* chash

#endif
