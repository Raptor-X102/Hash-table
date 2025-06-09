#include "Aligned_calloc.h"

void* aligned_calloc(size_t count, size_t size, size_t alignment) {

    void* ptr = aligned_alloc(alignment, count * size);
    if (ptr != NULL)
        memset(ptr, 0, count * size);

    return ptr;
}
