#include"Testing_funcs.h"

void Master_search(Hash_table_t* hash_table, Find_func_t hash_table_find, Compare_func_t compare_func,
                   char* buffer, int64_t max_symbols) {

    int words_found = 0;
    int words_not_found = 0;
    for(int i = 0; i < max_symbols; i += Stpride) {

        int64_t index = hash_table_find(hash_table, buffer + i, Stpride, compare_func);

    }
}

Test_data Single_test(Hash_table_t* hash_table, Find_func_t hash_table_find, Compare_func_t compare_func, char* buffer,
                      int64_t max_symbols, int test_num_of_cycles, FILE* output_file, const char* label) {

    Test_data test_data = {.frequency_end = 3.0E+9};
    test_data.start_cycles = Rdtscp();

    for(int i = 0; i < test_num_of_cycles; i++)
        Master_search(hash_table, hash_table_find, compare_func, buffer, max_symbols);

    test_data.end_cycles = Rdtscp();
    test_data.test_num_of_cycles = test_num_of_cycles;
    Print_test_data(output_file, &test_data, label);

    return test_data;
}

void Print_test_data(FILE* output_file, Test_data* test_data, const char* label) {

    double ticks = ((double)(test_data->end_cycles - test_data->start_cycles)) / test_data->test_num_of_cycles;
    double time_consumed_rdtsc = ticks / test_data->frequency_end;
    if(label)
        fprintf(output_file, "%s\n", label);

    fprintf(output_file, "Ticks;Time\n", ticks);
    fprintf(output_file, "%.2e;", ticks);
    fprintf(output_file, "\'%.2lf\'\n", time_consumed_rdtsc*1000);
}

bool Master_test(Hash_table_t* hash_table, char* words_buffer, const char* output_file_name, int64_t max_symbols,
                 int test_num_of_cycles) {

    FILE* output_file = fopen(output_file_name, "ab+");
    if(!output_file) {

        DEBUG_PRINTF("ERROR: file was not opened\n");
        return false;
    }

    time_t now = time(NULL);
    struct tm *curr_time = localtime(&now);
    char buffer[64];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", curr_time);
    fprintf(output_file, "Test start\nTime: %s\n", buffer);

    hash_table->hash_func = Hash_func_crc32_basic;
    Single_test(hash_table, Hash_table_find, Compare_strings, words_buffer,
                max_symbols, test_num_of_cycles, output_file, "Version 1");

    hash_table->hash_func = Hash_func_crc32_intrinsics;
    Single_test(hash_table, Hash_table_find, Compare_strings, words_buffer,
                max_symbols, test_num_of_cycles, output_file, "Version 2");

    Single_test(hash_table, Hash_table_find, Compare_strings_256, words_buffer,
                max_symbols, test_num_of_cycles, output_file, "Version 3");

    Single_test(hash_table, Hash_table_find_mod_div, Compare_strings_256, words_buffer,
                max_symbols, test_num_of_cycles, output_file, "Version 4");

    now = time(NULL);
    curr_time = localtime(&now);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", curr_time);
    fprintf(output_file, "Test end\nTime: %s\n", buffer);

    fclose(output_file);
}
