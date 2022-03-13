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

#define __chash_resize(hashtable, namespace)                                  \
do {                                                                          \
  if((namespace ## _HEAP) == 0) {                                             \
    if((hashtable)->length == (hashtable)->capacity) {                        \
        fprintf(stderr, "__chash_resize: hashtable is full. could not resize" \
                        " (%s:%i)\n", __FILE__, __LINE__);                    \
        exit(EXIT_FAILURE);                                                   \
    }                                                                         \
  } else {                                                                    \
    namespace ## _BUCKET *__CHASH_BUCKETS = NULL;                             \
    size_t __CHASH_INDEX = 0;                                                 \
    int __CHASH_NEXT_SIZE = CHASH_RESIZE((hashtable)->capacity);              \
                                                                              \
    if((double) (hashtable)->length /                                         \
       (double) (hashtable)->capacity < CHASH_LOAD_THRESHOLD)                 \
      break;                                                                  \
                                                                              \
    __CHASH_BUCKETS = calloc(__CHASH_NEXT_SIZE,                               \
                             sizeof(namespace ## _BUCKET));                   \
                                                                              \
    for(__CHASH_INDEX = 0; __CHASH_INDEX < (hashtable)->capacity;             \
                                                           __CHASH_INDEX++) { \
      __CHASH_KEY_BUCKET.key = (hashtable)->buckets[__CHASH_INDEX].key;       \
                                                                              \
      if((hashtable)->buckets[__CHASH_INDEX].state != CHASH_FILLED)           \
        continue;                                                             \
                                                                              \
      __CHASH_HASH = namespace ## _HASH(__CHASH_KEY_BUCKET.key,               \
                                        __CHASH_HASH);                        \
      __CHASH_HASH = __CHASH_HASH % __CHASH_NEXT_SIZE;                        \
                                                                              \
      while(1) {                                                              \
        if(__CHASH_BUCKETS[__CHASH_HASH].state != CHASH_FILLED)               \
          break;                                                              \
                                                                              \
        if((namespace ## _COMPARE((hashtable)->buckets[__CHASH_INDEX].key,    \
                                 __CHASH_BUCKETS[__CHASH_HASH].key)) == 1)    \
          break;                                                              \
                                                                              \
        __CHASH_HASH = (__CHASH_HASH + 1) % __CHASH_NEXT_SIZE;                \
      }                                                                       \
                                                                              \
      __CHASH_BUCKETS[__CHASH_HASH].key =                                     \
                                     (hashtable)->buckets[__CHASH_INDEX].key; \
      __CHASH_BUCKETS[__CHASH_HASH].value =                                   \
                                   (hashtable)->buckets[__CHASH_INDEX].value; \
      __CHASH_BUCKETS[__CHASH_HASH].state = CHASH_FILLED;                     \
    }                                                                         \
                                                                              \
    free((hashtable)->buckets);                                               \
    (hashtable)->buckets = __CHASH_BUCKETS;                                   \
    (hashtable)->capacity = __CHASH_NEXT_SIZE;                                \
  }                                                                           \
} while(0)

#define chash_init(hashtable, namespace)                       \
    NULL;                                                      \
                                                               \
    (hashtable) = calloc(1, sizeof((*(hashtable))));           \
    (hashtable)->capacity = CHASH_INITIAL_SIZE;                \
    (hashtable)->buckets = calloc(CHASH_INITIAL_SIZE,          \
                              sizeof(*((hashtable)->buckets))) \

#define chash_init_stack(hashtable, buffer, _length, namespace)               \
    (*(hashtable));                                                           \
                                                                              \
    if((_length) <= 0) {                                                      \
        fprintf(stderr, "chash_init_stack: hashtable cannot have a maximum "  \
                        "length of 0 or less (%s:%i)\n", __FILE__, __LINE__); \
        exit(EXIT_FAILURE);                                                   \
    }                                                                         \
                                                                              \
    if((buffer) == NULL) {                                                    \
        fprintf(stderr, "chash_init_stack: hashtable block cannot be null "   \
                        "(%s:%i) \n", __FILE__, __LINE__);                    \
        exit(EXIT_FAILURE);                                                   \
    }                                                                         \
                                                                              \
    (hashtable)->length = 0;                                                  \
    (hashtable)->capacity = _length;                                          \
    (hashtable)->buckets = buffer 

#define chash_assign(hashtable, _key, _value, namespace)                    \
do {                                                                        \
  long __CHASH_HASH = 0;                                                    \
  namespace ## _BUCKET __CHASH_KEY_BUCKET;                                  \
  __CHASH_KEY_BUCKET.key = (_key);                                          \
  __CHASH_KEY_BUCKET.value = (_value);                                      \
  __CHASH_KEY_BUCKET.state = CHASH_UNFILLED;                                \
                                                                            \
  if((hashtable) == NULL) {                                                 \
    fprintf(stderr, "chash_assign: hashtable cannot be NULL. (%s:%i)\n",    \
                    __FILE__, __LINE__);                                    \
    exit(EXIT_FAILURE);                                                     \
  }                                                                         \
                                                                            \
  __chash_resize(hashtable, namespace);                                     \
                                                                            \
  __CHASH_KEY_BUCKET.key = (_key);                                          \
  __CHASH_HASH = namespace ## _HASH(__CHASH_KEY_BUCKET.key, __CHASH_HASH);  \
  __CHASH_HASH = __CHASH_HASH % (hashtable)->capacity;                      \
                                                                            \
  while(1) {                                                                \
    if((hashtable)->buckets[__CHASH_HASH].state != CHASH_FILLED)            \
      break;                                                                \
                                                                            \
    if((namespace ## _COMPARE((_key),                                       \
                            (hashtable)->buckets[__CHASH_HASH].key)) == 1)  \
      break;                                                                \
                                                                            \
    __CHASH_HASH = (__CHASH_HASH + 1) % (hashtable)->capacity;              \
  }                                                                         \
                                                                            \
  if((hashtable)->buckets[__CHASH_HASH].state == CHASH_FILLED) {            \
     namespace ## _FREE_VALUE((hashtable)->buckets[__CHASH_HASH].value);    \
  } else {                                                                  \
     (hashtable)->length++;                                                 \
  }                                                                         \
                                                                            \
  (hashtable)->buckets[__CHASH_HASH].key = _key;                            \
  (hashtable)->buckets[__CHASH_HASH].value = _value;                        \
  (hashtable)->buckets[__CHASH_HASH].state = CHASH_FILLED;                  \
} while(0)

#define chash_lookup(hashtable, _key, storage, namespace)                     \
storage;                                                                      \
                                                                              \
do {                                                                          \
  int __CHASH_INDEX = 0;                                                      \
  long __CHASH_HASH = 0;                                                      \
  namespace ## _BUCKET __CHASH_KEY_BUCKET = {0};                              \
  __CHASH_KEY_BUCKET.key = _key;                                              \
                                                                              \
  if((hashtable) == NULL) {                                                   \
    fprintf(stderr, "chash_lookup: hashtable cannot be NULL. (%s:%i)\n",      \
                    __FILE__, __LINE__);                                      \
    exit(EXIT_FAILURE);                                                       \
  }                                                                           \
                                                                              \
  __CHASH_HASH = namespace ## _HASH(__CHASH_KEY_BUCKET.key, __CHASH_HASH);    \
  __CHASH_HASH = __CHASH_HASH % (hashtable)->capacity;                        \
                                                                              \
  while((unsigned int) __CHASH_INDEX < (hashtable)->capacity) {               \
    if((hashtable)->buckets[__CHASH_HASH].state == CHASH_UNFILLED)            \
      break;                                                                  \
                                                                              \
    if((namespace ## _COMPARE((_key),                                         \
                             (hashtable)->buckets[__CHASH_HASH].key)) == 1) { \
      __CHASH_INDEX = -1;                                                     \
      break;                                                                  \
    }                                                                         \
                                                                              \
    __CHASH_HASH = (__CHASH_HASH + 1) % (hashtable)->capacity;                \
    __CHASH_INDEX++;                                                          \
  }                                                                           \
                                                                              \
  if(((hashtable)->buckets[__CHASH_HASH].state == CHASH_UNFILLED) ||          \
                                                    __CHASH_INDEX != -1) {    \
    fprintf(stderr, "chash_lookup: failed to find key in hashtable (%s:%i)"   \
                    "\n", __FILE__, __LINE__);                                \
    exit(EXIT_FAILURE);                                                       \
  }                                                                           \
                                                                              \
  storage = (hashtable)->buckets[__CHASH_HASH].value;                         \
} while(0)

#define chash_delete(hashtable, _key, namespace)                              \
do {                                                                          \
  int __CHASH_INDEX = 0;                                                      \
  long __CHASH_HASH = 0;                                                      \
                                                                              \
  if((hashtable) == NULL) {                                                   \
    fprintf(stderr, "chash_delete: hashtable cannot be NULL (%s:%i)\n",       \
                     __FILE__, __LINE__);                                     \
    exit(EXIT_FAILURE);                                                       \
  }                                                                           \
                                                                              \
  __CHASH_HASH = namespace ## _HASH((_key), __CHASH_HASH);                    \
  __CHASH_HASH = __CHASH_HASH % (hashtable)->capacity;                        \
                                                                              \
  while((unsigned int) __CHASH_INDEX < (hashtable)->capacity) {               \
    if((hashtable)->buckets[__CHASH_HASH].state == CHASH_UNFILLED)            \
      break;                                                                  \
                                                                              \
    if((namespace ## _COMPARE((_key),                                         \
                             (hashtable)->buckets[__CHASH_HASH].key)) == 1) { \
      __CHASH_INDEX = -1;                                                     \
      break;                                                                  \
    }                                                                         \
                                                                              \
    __CHASH_HASH = (__CHASH_HASH + 1) % (hashtable)->capacity;                \
    __CHASH_INDEX++;                                                          \
  }                                                                           \
                                                                              \
  if(((hashtable)->buckets[__CHASH_HASH].state != CHASH_FILLED) ||            \
                                                  __CHASH_INDEX != -1) {      \
    fprintf(stderr, "chash_delete: failed to find key in hashtable (%s:%i)"   \
                    "\n", __FILE__, __LINE__);                                \
    exit(EXIT_FAILURE);                                                       \
  }                                                                           \
                                                                              \
  namespace ## _FREE_KEY((hashtable)->buckets[__CHASH_HASH].key);             \
  namespace ## _FREE_VALUE((hashtable)->buckets[__CHASH_HASH].value);         \
  (hashtable)->buckets[__CHASH_HASH].state = CHASH_TOMBSTONE;                 \
  (hashtable)->length--;                                                      \
} while(0)

#define chash_contains(hashtable, _key, storage, namespace)                   \
1;                                                                            \
                                                                              \
do {                                                                          \
  int __CHASH_INDEX = 0;                                                      \
  long __CHASH_HASH = 0;                                                      \
                                                                              \
  if((hashtable) == NULL) {                                                   \
    fprintf(stderr, "chash_contains: hashtable cannot be NULL (%s:%i)\n",     \
                     __FILE__, __LINE__);                                     \
    exit(EXIT_FAILURE);                                                       \
  }                                                                           \
                                                                              \
  __CHASH_HASH = namespace ## _HASH((_key), __CHASH_HASH);                    \
  __CHASH_HASH = __CHASH_HASH % (hashtable)->capacity;                        \
                                                                              \
  while((unsigned int) __CHASH_INDEX < (hashtable)->capacity) {               \
    if((hashtable)->buckets[__CHASH_HASH].state == CHASH_UNFILLED)            \
      break;                                                                  \
                                                                              \
    if((namespace ## _COMPARE((_key),                                         \
                             (hashtable)->buckets[__CHASH_HASH].key)) == 1) { \
      __CHASH_INDEX = -1;                                                     \
      break;                                                                  \
    }                                                                         \
                                                                              \
    __CHASH_HASH = (__CHASH_HASH + 1) % (hashtable)->capacity;                \
    __CHASH_INDEX++;                                                          \
  }                                                                           \
                                                                              \
  if(((hashtable)->buckets[__CHASH_HASH].state != CHASH_FILLED)               \
                                               || __CHASH_INDEX != -1) {      \
    storage = 0;                                                              \
  }                                                                           \
} while(0)

#define chash_free(hashtable, namespace)                                    \
do {                                                                        \
  (hashtable)->capacity--;                                                  \
                                                                            \
  while((hashtable)->capacity != -1) {                                      \
    if((hashtable)->buckets[(hashtable)->capacity].state != CHASH_FILLED) { \
      (hashtable)->capacity--;                                              \
      continue;                                                             \
    }                                                                       \
                                                                            \
    namespace ##_FREE_KEY(                                                  \
                    (hashtable)->buckets[(hashtable)->capacity].key);       \
    namespace ##_FREE_VALUE(                                                \
                    (hashtable)->buckets[(hashtable)->capacity].value);     \
    (hashtable)->capacity--;                                                \
    (hashtable)->length--;                                                  \
  }                                                                         \
                                                                            \
  if((namespace ## _HEAP) == 1) {                                           \
    free((hashtable)->buckets);                                             \
    free((hashtable));                                                      \
  }                                                                         \
} while(0);

#endif
