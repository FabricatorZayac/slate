#ifndef SLATE_VEC_H_
#define SLATE_VEC_H_

#include "allocator.h"
#include "slice.h"

const extern struct Vec_mt {
    void *(*init)(size_t typesize, allocator_t allocator);
    void (*deinit)(void *self);
    allocator_t (*get_allocator)(void *self);
    void (*ensure_capacity)(void *self, size_t new_capacity);
} Vec;

/// Initializes an empty vector and returns a slice into it with an additional
/// allocator header
#define Vec(T, allocator)                   \
&(struct {                                  \
    allocator_t allocator;                  \
    array_header_t header;                  \
    T *data;                                \
}) {                                        \
    .allocator = allocator,                 \
    .header = {                             \
        .elem_size = sizeof(T),             \
        .len = 0,                           \
    },                                      \
    .data = Vec.init(sizeof(T), allocator), \
}.data

// Naming subject to change
#define APPEND(VEC, ITEM) do {              \
    Vec.ensure_capacity(VEC, len(VEC) + 1); \
    (*VEC)[len(VEC)] = ITEM;                \
    len(VEC) += 1;                          \
} while(0)


#endif // !SLATE_VEC_H_
