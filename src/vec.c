#include "vec.h"
#include "mem.h"
#include "slice.h"

static void *init(size_t typesize, allocator_t allocator) {
    array_header_t *header = (array_header_t *)Allocator.rawAlloc(allocator, sizeof(array_header_t));
    header->elem_size = typesize;
    header->len = 0;

    return header + 1;
}

static void deinit(void *self) {
    array_header_t *header = *(void **)self - sizeof(array_header_t);
    Allocator.rawFree(Vec.get_allocator(self), header);
}

allocator_t get_allocator(void *self) {
    return *(allocator_t *)(self - sizeof(array_header_t) - sizeof(allocator_t));
}

// Would be nice to make this a bit more readable if possible
/// self is a slice into the vector's memory
static void ensure_capacity(void *self, size_t new_capacity) {
    array_header_t *header = *(void **)self - sizeof(array_header_t);
    allocator_t allocator = Vec.get_allocator(self);

    if (header->len >= new_capacity) return;

    size_t better_capacity = header->len;
    do {
        better_capacity += better_capacity / 2 + 8;
    } while(better_capacity < new_capacity);

    array_header_t *new_header = (array_header_t *)Allocator.rawAlloc(
        allocator,
        sizeof(array_header_t) + better_capacity * elem_size(self)
    );

    new_header->elem_size = header->elem_size;
    new_header->len = better_capacity;
    char *new_mem = (char *)(new_header + 1);
    char *old_mem = (char *)(header + 1);
    mem.cpy(
        SLICE(new_mem, 0, len(self)),
        TOSLICE(old_mem)
    );

    Allocator.rawFree(allocator, header);
    *(void **)self = new_mem;
}

const struct Vec_mt Vec = {
    .init = init,
    .deinit = deinit,
    .ensure_capacity = ensure_capacity,
    .get_allocator = get_allocator,
};
