#include"Hash_func.h"

uint64_t Hash_func_stack_version(const void * buffer_start, const void * buffer_end) {

    const char* byte_buffer = (const char*) buffer_start;
    uint64_t len = (const char*)buffer_end - byte_buffer+1;
    uint64_t h_sum = +7952812;

    for(int commands_amount = 0; commands_amount < len; commands_amount++){

        h_sum = (h_sum*10 + h_sum % 10 + commands_amount) ^ (byte_buffer[commands_amount]);
    }

    return h_sum;
}

uint64_t Hash_func_crc32_intrinsics(const void * buffer_start, const void * buffer_end) {

    //ALIGN32_ASSERT(buffer_start);
    uint64_t crc = Crc32_Init_Value;
    __m256i vec = _mm256_load_si256((const __m256i*)buffer_start);

    crc = _mm_crc32_u64(crc, _mm256_extract_epi64(vec, 0));
    crc = _mm_crc32_u64(crc, _mm256_extract_epi64(vec, 1));
    crc = _mm_crc32_u64(crc, _mm256_extract_epi64(vec, 2));
    crc = _mm_crc32_u64(crc, _mm256_extract_epi64(vec, 3));

    return crc ^ Crc32_Init_Value;
}

uint64_t Hash_func_crc32_basic(const void* buffer_start, const void* buffer_end) {

    const uint8_t* start = (const uint8_t*)buffer_start;
    const uint8_t* end = (const uint8_t*)buffer_end;
    size_t len = end - start;

    const uint8_t* bytes = start;
    uint32_t crc = Crc32_Init_Value;

    for (size_t i = 0; i < len; i++) {

        uint8_t index = (uint8_t)((crc ^ bytes[i]) & 0xFF);
        crc = (crc >> 8) ^ crc32_table[index];
    }

    return crc ^ Crc32_Init_Value;
}

Hash_table_t Hash_table_create(int hash_table_size, Hash_func_t hash_func) {

    Hash_table_t hash_table = {};
    if(hash_table_size >= Hash_table_min_size)
        hash_table.table_size = hash_table_size;

    else
        hash_table.table_size = Hash_table_min_size;

    hash_table.lists_array = (List*) calloc(hash_table.table_size, sizeof(List));
    if(!hash_table.lists_array)
        DEBUG_PRINTF("ERROR: memory was not allocated");

    for(int i = 0; i < hash_table.table_size; i++)
        List_Ctor(hash_table.lists_array + i, Chunk_size);

    hash_table.hash_func = hash_func;
    return hash_table;
}

bool Hash_table_insert(Hash_table_t* hash_table, void* element, int element_size, Compare_func_t compare_func) {

    size_t element_hash = hash_table->hash_func(element, (char*)element + element_size);
    int64_t mask = hash_table->table_size - 1;

    __asm__ __volatile__ (
        "and %[mask], %[hash]\n\t"
        : [hash] "+r" (element_hash)
        : [mask] "r" (mask)
    );

    List* list_ptr = &hash_table->lists_array[element_hash];
    int64_t element_index = List_find_element_c(list_ptr, element, element_size, compare_func);
    //DEBUG_PRINTF("element_index = %d\n", element_index);
    if(element_index == Not_found_value) {

        int64_t last_index = List_get_last(list_ptr);
        //DEBUG_PRINTF("last_index = %d\n", last_index);
        if(!List_insert(list_ptr, last_index, element, element_size))
            return false;
    }

    return true;
}

int64_t Hash_table_find_mod_div(Hash_table_t* hash_table, void* element, int element_size, Compare_func_t compare_func) {

    size_t element_hash = hash_table->hash_func(element, (char*)element + element_size);
    int64_t mask = hash_table->table_size - 1;

    __asm__ __volatile__ (
        "and %[mask], %[hash]\n\t"
        : [hash] "+r" (element_hash)
        : [mask] "r" (mask)
    );

    List* list_ptr = &hash_table->lists_array[element_hash];

    return List_find_element_c(list_ptr, element, element_size, compare_func);
}

int64_t Hash_table_find(Hash_table_t* hash_table, void* element, int element_size, Compare_func_t compare_func) {

    size_t element_hash = hash_table->hash_func(element, (char*)element+element_size);
    int64_t list_index = element_hash % hash_table->table_size;
    List* list_ptr = &hash_table->lists_array[list_index];

    return List_find_element_c(list_ptr, element, element_size, compare_func);
}

/*int64_t Hash_table_find_c(Hash_table_t* hash_table, void* element, int element_size, Compare_func_t compare_func) {

    size_t element_hash = hash_table->hash_func(element, (char*)element + element_size);
    int64_t mask = hash_table->table_size - 1;

    __asm__ __volatile__ (
        "and %[mask], %[hash]\n\t"
        : [hash] "+r" (element_hash)
        : [mask] "r" (mask)
    );

    List* list_ptr = &hash_table->lists_array[element_hash];
    return List_find_element_c(list_ptr, element, element_size, compare_func);
}*/

char* Hash_table_file_insert(Hash_table_t* hash_table, char* file_name, Compare_func_t compare_func) {

    size_t file_size = get_file_size(file_name);

    FILE* input_file = fopen(file_name, "rb");
    if(!input_file) {

        DEBUG_PRINTF("ERROR: file was not opened\n");
        return NULL;
    }

    char* file_buffer = (char*) aligned_calloc(file_size, sizeof(char), Memory_alignment);
    if(!file_buffer) {

        DEBUG_PRINTF("ERROR: memory was not allocated\n");
        return NULL;
    }

    fread(file_buffer, sizeof(char), file_size, input_file);

    fclose(input_file);
    size_t max_symbol = file_size - (file_size % Stpride);
    DEBUG_PRINTF("file_size = %d\n", file_size);
    DEBUG_PRINTF("max_symbol = %d\n", max_symbol);
    DEBUG_PRINTF("words = %d\n", max_symbol/Stpride);
    //getchar();
    for(int i = 0; i < max_symbol; i += Stpride) {

        //DEBUG_PRINTF("i = %d\n", i);
        if(!Hash_table_insert(hash_table, file_buffer + i, Stpride, compare_func))
            DEBUG_PRINTF("Value '%s' was not inserted\n", (char*) file_buffer + i);

    }

    return file_buffer;
}

bool Hash_table_plot_hist(Hash_table_t* hash_table, const char* output_file_name1, const char* output_file_name2) {

    FILE* output_file = fopen(output_file_name1, "wb");
    if(!output_file) {

        DEBUG_PRINTF("ERROR: file was not opened\n");
        return false;
    }

    size_t buffer_size = hash_table->table_size * (2 * Max_digits + 5 + 2) * Max_bucket_elements; // bucket number and elements <= 2^64,
    char* output_buffer = (char*) calloc(buffer_size, sizeof(char));    // 2 = ' ' (space) + '\n', 5 = max_float
    if(!output_buffer) {

        DEBUG_PRINTF("ERROR: memory was not allocated\n");
        return false;
    }

    size_t offset = 0;

    int64_t table_size = hash_table->table_size;
    int64_t unique_elements = 0;
    int buckets_sizes[Buckets_sizes_array_size] = {};
    //List list_of_sizes = {};
    //List_Ctor(&list_of_sizes, List_of_sizes_size);
    //DEBUG_PRINTF("table_size = %d\n", table_size);
    for (int bucket = 0; bucket < table_size; bucket++) {

        int elements_to_write = hash_table->lists_array[bucket].elements_amount;
        if(elements_to_write < Buckets_sizes_array_size)
            buckets_sizes[elements_to_write]++;
        //int list_of_sizes_index = List_find_element_c(&list_of_sizes, &elements_to_write, sizeof(int), Compare_sizes);

        for (int i = 0; i < elements_to_write; i++) {
            int written = snprintf(output_buffer + offset, buffer_size - offset, "%d %d %.4lf\n",
                                bucket, i + 1, (double)i / (double)Max_bucket_elements);

            if (written < 0 || written >= buffer_size - offset) {
                DEBUG_PRINTF("ERROR: buffer overflow or write error\n");
                free(output_buffer);
                return false;
            }
            offset += written;
        }

        unique_elements += hash_table->lists_array[bucket].elements_amount;
    }

    fwrite(output_buffer, sizeof(char), buffer_size, output_file);

    DEBUG_PRINTF("unique_elements = %d\n", unique_elements);
    fclose(output_file);

    output_file = fopen(output_file_name2, "wb");
    if(!output_file) {

        DEBUG_PRINTF("ERROR: file was not opened\n");
        return false;
    }
    offset = 0;
    int max_size = 0;
    memset(output_buffer, 0, buffer_size);
    for (int i = 0; i < Buckets_sizes_array_size; i++) {

        int bucket_size = buckets_sizes[i];
        if(bucket_size > max_size) {

            max_size = bucket_size;

        }
        if(bucket_size > 0){

            for(int j = 0; j < bucket_size; j++) {
                int written = snprintf(output_buffer + offset, buffer_size - offset, "%d %d %.4lf\n",
                                    i, j + 1, (double)j / (double)Max_amount_of_bucket_size);

                if (written < 0 || written >= buffer_size - offset) {
                    DEBUG_PRINTF("ERROR: buffer overflow or write error\n");
                    free(output_buffer);
                    return false;
                }
                offset += written;
            }
        }
    }

    fwrite(output_buffer, sizeof(char), buffer_size, output_file);
    fclose(output_file);
    printf("max_size = %d\n", max_size);
    system("gnuplot hist.gnuplot");
    system("gnuplot hist2.gnuplot");
    free(output_buffer);
    return true;
}
