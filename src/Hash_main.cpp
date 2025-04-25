#include"Hash_func.h"
#include"Check_r_w_flags.h"
#include"Testing_funcs.h"

const int Words = 62393;
const int Unique_words = 8085;
const int words_to_check = 100;
const int64_t Max_symbols = Words * Stpride;
const int Test_num_of_cycles = 200;
const int Hash_table_size = 512;
int main(int argc, char* argv[]) {

    char* read_files[1] = {};
    char* write_files[2] = {};
    file_types files = {};
    files.read_files = read_files;
    files.write_files = write_files;
    if(!Check_r_w_flags(CHECK_R_W, argv, argc, &files)) {

        DEBUG_PRINTF("ERROR: flags verification failed");
        return 0;
    }

    Hash_table_t hash_table = Hash_table_create(Hash_table_size, Hash_func_crc32_intrinsics);
    char* buffer = Hash_table_file_insert(&hash_table, files.read_files[0], Compare_strings);

    int words_found = 0;
    int words_not_found = 0;
    for(int i = 0; i < Max_symbols; i += Stpride) {

        int64_t index = Hash_table_find(&hash_table, buffer + i, Stpride, Compare_strings);
        if(index == Not_found_value) {

            //printf("ERROR:   value '%s' was not found\n", buffer + i);
            words_not_found++;
        }

        else {

            //printf("SUCCESS: value '%s' was found\n", buffer + i);
            words_found++;
        }
    }

    printf("TOTAL:\n FOUND: %d\n NOT_FOUND: %d\n", words_found, words_not_found);

    if(!Hash_table_plot_hist(&hash_table, files.write_files[0]))
       DEBUG_PRINTF("ERROR: histogram was not created\n");

    //Master_test(&hash_table, buffer, files.write_files[1], Max_symbols, Test_num_of_cycles);
    free(buffer);
}
