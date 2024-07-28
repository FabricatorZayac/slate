#include <assert.h>

#include "heap.h"
#include "vec.h"

int main() {
    int **vec = Vec(int, malloc_allocator);

    APPEND(vec, 5);
    assert(len(vec) == 1);
    assert((*vec)[0] == 5);

    APPEND(vec, 8);
    assert(len(vec) == 2);
    assert((*vec)[0] == 5);
    assert((*vec)[1] == 8);

    Vec.deinit(vec);
}
