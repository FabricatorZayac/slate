#ifndef SLATE_ALLOCATOR_H_
#define SLATE_ALLOCATOR_H_

#include <stdbool.h>
#include <stddef.h>

/// buf is an array referece
typedef struct {
    void *ptr;
    const struct allocator_vtable {
        char *(*alloc)(void *ctx, size_t len);
        void (*free)(void *ctx, void *buf);
        bool (*resize)(void *ctx, void *buf, size_t new_len);
    } *vtable;
} allocator_t;

const extern struct allocator_mt {
    char *(*rawAlloc)(allocator_t, size_t len);
    bool (*rawResize)(allocator_t, void *buf, size_t new_len);
    void (*rawFree)(allocator_t, void *buf);

    void *(*create)(allocator_t, size_t size);
    void (*destroy)(allocator_t, void *ptr);

    void *(*alloc)(allocator_t, size_t size, size_t n);
    bool (*resize)(allocator_t, void *buf, size_t new_n);
    void *(*realloc)(allocator_t, void *buf, size_t new_n);
    void (*free)(allocator_t, void *buf);
} Allocator;

#endif // !SLATE_ALLOCATOR_H_
