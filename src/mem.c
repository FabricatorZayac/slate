#include <assert.h>
#include <string.h>

#include "mem.h"
#include "slice.h"

static bool eql(void *left, void *right) {
    array_header_t *left_header = left - sizeof(array_header_t);
    array_header_t *right_header = right - sizeof(array_header_t);

    if (left_header->elem_size != right_header->elem_size || left_header->len != right_header->len) {
        return false;
    }

    const char *lhs = *(char **)left;
    const char *rhs = *(char **)right;

    return !memcmp(lhs, rhs, left_header->len * left_header->elem_size);
}

static void cpy(void *dest, void *source) {
    array_header_t *restrict dest_header = dest - sizeof(array_header_t);
    array_header_t *restrict source_header = source - sizeof(array_header_t);

    assert(dest_header->elem_size == source_header->elem_size && dest_header->len == source_header->len);

    char *restrict dest_ptr = *(char **)dest;
    const char *restrict source_ptr = *(char **)source;

    memcpy(dest_ptr, source_ptr, source_header->len * source_header->elem_size);
}

const struct mem_mod mem = {
    .cpy = cpy,
    .eql = eql,
};
