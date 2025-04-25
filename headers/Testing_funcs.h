#ifndef TESTING_FUNC
#define TESTING_FUNC

#include"Hash_func.h"

#include"Rdtscp.h"
struct Test_data {

    clock_t timeh_t1;
    clock_t timeh_t2;
    uint64_t start_cycles;
    uint64_t end_cycles;
    double frequency_start;
    double frequency_end;
    int test_num_of_cycles;
};
void Master_search(Hash_table_t* hash_table, Find_func_t hash_table_find, Compare_func_t compare_func,
                   char* buffer, int64_t max_symbols);
Test_data Single_test(Hash_table_t* hash_table, Find_func_t hash_table_find, Compare_func_t compare_func, char* buffer,
                      int64_t max_symbols, int test_num_of_cycles, FILE* output_file, const char* label);
void Print_test_data(FILE* output_file, Test_data* test_data, const char* label);
bool Master_test(Hash_table_t* hash_table, char* words_buffer, const char* output_file_name, int64_t max_symbols,
                 int test_num_of_cycles);

#endif
