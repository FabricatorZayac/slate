#include "allocator.h"
#include "slice.h"
#include <stdlib.h>

static char *alloc(void *ctx, size_t len) {
    (void)ctx;
    return malloc(len);
}

static void _free(void *ctx, void *buf) {
    (void)ctx;
    free(buf);
}

static bool resize(void *ctx, void *buf, size_t new_len) {
    (void)ctx;
    if (new_len == len(buf)) return true;
    return false;
}

const allocator_t malloc_allocator = {
    .ptr = NULL,
    .vtable = &(const struct allocator_vtable) {
        .alloc = alloc,
        .free = _free,
        .resize = resize,
    },
};
