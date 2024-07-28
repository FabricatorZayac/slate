#include "allocator.h"
#include "mem.h"
#include "slice.h"

static char *rawAlloc(allocator_t self, size_t len) {
    return self.vtable->alloc(self.ptr, len);
}
static bool rawResize(allocator_t self, void *buf, size_t new_len) {
    return self.vtable->resize(self.ptr, buf, new_len);
}
static void rawFree(allocator_t self, void *buf) {
    self.vtable->free(self.ptr, buf);
}

static void *create(allocator_t self, size_t typesize) {
    return Allocator.rawAlloc(self, typesize);
}
static void destroy(allocator_t self, void *ptr) {
    Allocator.rawFree(self, ptr);
}

static void *alloc(allocator_t self, size_t size, size_t n) {
    array_header_t *header = (array_header_t *)Allocator.rawAlloc(self, size * n + sizeof(array_header_t));
    header->elem_size = size;
    header->len = n;
    return header + 1;
}
static bool resize(allocator_t self, void *buf, size_t new_n) {
    if (new_n == 0) {
        Allocator.free(self, buf);
        return true;
    }
    if (len(buf) == 0) {
        return false;
    }
    return Allocator.rawResize(self, buf, new_n * elem_size(buf));
}
static void *realloc(allocator_t self, void *buf, size_t new_n) {
    if (Allocator.resize(self, buf, new_n)) {
        return buf;
    }

    void *new_mem = Allocator.alloc(self, elem_size(buf), new_n);
    mem.cpy(new_mem, buf);
    Allocator.free(self, buf);

    return new_mem;
}
static void free(allocator_t self, void *buf) {
    Allocator.rawFree(self, (array_header_t *)buf - 1);
}

const struct allocator_mt Allocator = {
    .rawAlloc = rawAlloc,
    .rawResize = rawResize,
    .rawFree = rawFree,

    .create = create,
    .destroy = destroy,

    .alloc = alloc,
    .resize = resize,
    .realloc = realloc,
    .free = free,
};
