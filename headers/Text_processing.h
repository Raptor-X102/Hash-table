#ifndef TEXT_PROCESSING
#define TEXT_PROCESSING

#include<stdio.h>
#include<ctype.h>
#include"Check_r_w_flags.h"
#include "GetFileSize2.h"
#include"Hash_def_and_decl.h"
struct Buffer {

    char* buffer;
    size_t buffer_size;
    size_t words;
};

bool Text_processing(char* input_file_name, char* output_file_name);
Buffer First_stage_processing(char* file_buffer, size_t buffer_size);
Buffer Second_stage_processing(Buffer* first_stage_buffer);
size_t strlen_nl(char* string);

#endif
