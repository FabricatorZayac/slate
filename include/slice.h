#ifndef SLATE_SLICE_H_
#define SLATE_SLICE_H_

#include <stddef.h>

typedef struct {
    size_t elem_size;
    size_t len;
} array_header_t;

/// Array object literal
#define ARRAY(T, ...)                                  \
(struct {                                              \
    array_header_t header;                             \
    T arr[sizeof((T[]){__VA_ARGS__}) / sizeof(T)];     \
}) {                                                   \
    .header = {                                        \
        .elem_size = sizeof(T),                        \
        .len = sizeof((T[]){__VA_ARGS__}) / sizeof(T), \
    },                                                 \
    .arr = {__VA_ARGS__}                               \
}.arr

#define SLICE(PTR, BEGIN, END)       \
(void *)&(struct {                   \
    array_header_t header;           \
    void *data;                      \
}) {                                 \
    .header = {                      \
        .elem_size = elem_size(PTR), \
        .len = END - BEGIN,          \
    },                               \
    .data = PTR + BEGIN,             \
}.data

// Naming subject to change. Might even somehow unify the 2 slice macros
// if I ever decide to sacrifice my undying soul in the name the Dark Gods
#define TOSLICE(ARR) SLICE(ARR, 0, len(ARR))

///
/// Dereferences `size_t len` field of array-lke object
/// 
/// Whatever you want the result of len(arr_like) be,
/// should be stored right behind your array-like object.
///
#define len(arr_like) (*((size_t *)arr_like - 1))

#define elem_size(arr_like) (*((size_t *)arr_like - 2))

/// String slice literal
#define str(cstr)                   \
&(struct {                          \
    array_header_t header;          \
    char *data;                     \
}) {                                \
    .header = {                     \
        .elem_size = sizeof(char),  \
        .len = sizeof(cstr) - 1,    \
    },                              \
    .data = cstr                    \
}.data

#endif // !SLATE_SLICE_H_
