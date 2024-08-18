#include <assert.h>

#include "slice.h"
#include "allocator.h"
#include "heap.h"
#include "mem.h"

int main() {
    int *arr = ARRAY(int, 1, 2, 3, 4, 5, 6, 7, 8, 9);

    assert(len(arr) == 9);
    assert(arr[2] == 3);

    int **slice = SLICE(arr, 3, len(arr));
    assert(3 == len(arr) - len(slice));
    assert(len(slice) == 6);
    assert((*slice)[0] == 4);
    assert((*slice)[5] == 9);

    int **foo = SLICE(*slice, 2, len(slice) - 2);
    assert(len(foo) == 2);
    assert((*foo)[0] == 6);
    assert((*foo)[1] == 7);

    int *buf = Allocator.alloc(malloc_allocator, sizeof(int), len(arr));

    mem.cpy(TOSLICE(buf), TOSLICE(arr));
    assert(mem.eql(TOSLICE(buf), TOSLICE(arr)));

    Allocator.free(malloc_allocator, buf);
}
