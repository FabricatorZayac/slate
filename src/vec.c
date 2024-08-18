#include <assert.h>

#include "vec.h"
#include "mem.h"
#include "slice.h"

static void *init(size_t typesize, allocator_t allocator) {
    return Allocator.alloc(allocator, typesize, 0);
}

static void deinit(void *self) {
    Allocator.free(Vec.get_allocator(self), *(void **)self);
}

static allocator_t get_allocator(void *self) {
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

static void append_slice(void *self, void *items) {
    Vec.ensure_capacity(self, len(*(void **)self) + len(items));

    size_t new_len = len(self) + len(items);
    mem.cpy(
        SLICE(*(void **)self, len(self), new_len),
        items
    );

    len(self) = new_len;
}

static void expand_to_cap(void *self) {
    len(self) = len(*(void **)self);
}

static void shrink_and_free(void *self, size_t new_len) {
    assert(new_len <= len(self));
    void **this = self;
    allocator_t allocator = Vec.get_allocator(this);
    
    void **old_memory = SLICE(*this, 0, len(*this));
    if (Allocator.resize(allocator, old_memory, new_len)) {
        len(*this) = new_len;
        len(this) = new_len;
        return;
    }

    void *new_memory = Allocator.alloc(allocator, elem_size(this), new_len);
    void **new_slice = SLICE(new_memory, 0, len(new_memory));
    mem.cpy(new_slice, SLICE(*this, 0, new_len));
    Allocator.free(allocator, *old_memory);
    // *this = new_memory;
    // len(*this) = len(new_memory);
}

static size_t append_write(void *ctx, char **bytes) {
    if (elem_size(ctx) != 1) return 0;
    Vec.append_slice(ctx, bytes);
    return len(bytes);
}
static writer_t writer(void *self) {
    return (writer_t) {
        .context = self,
        .write_fn = append_write,
    };
}

const struct Vec_mt Vec = {
    .init = init,
    .deinit = deinit,
    .ensure_capacity = ensure_capacity,
    .expand_to_cap = expand_to_cap,
    .get_allocator = get_allocator,
    .append_slice = append_slice,

    .writer = writer,
};
