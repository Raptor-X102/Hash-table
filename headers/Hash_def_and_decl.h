#ifndef HASH_DEF_AND_DECL
#define HASH_DEF_AND_DECL

#include "List_funcs.h"
#include "List_debug.h"
#include "List_graph_debug.h"
#include "Check_r_w_flags.h"
#include <xmmintrin.h>
#include <immintrin.h>
#include <nmmintrin.h>

typedef uint64_t (*Hash_func_t)(const void * buffer_start, const void * buffer_end);
struct Hash_table_t {

    List* lists_array;
    int64_t table_size;
    Hash_func_t hash_func;
};

struct String_info {

    char* string;
    int64_t counter;
};

const int Hash_table_min_size = 20;
const int Stpride = 32;
const int Chunk_size = 20;
const uint64_t Crc32_Init_Value = 0xFFFFFFFF;
const int Max_digits = 20;
const int Max_bucket_elements = 30;
#endif
