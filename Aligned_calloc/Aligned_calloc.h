#ifndef ALIGNED_CALLOC
#define ALIGNED_CALLOC

#include <stdlib.h>
#include <string.h>

void* aligned_calloc(size_t count, size_t size, size_t alignment);

#endif
