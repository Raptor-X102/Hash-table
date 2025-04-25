#include"Text_processing.h"

int main(int argc, char* argv[]) {

    char* write_files[1] = {};
    char* read_files[1] = {};
    file_types files = {};
    files.write_files = write_files;
    files.read_files = read_files;
    if(!Check_r_w_flags(CHECK_R_W, argv, argc, &files)) {

        DEBUG_PRINTF("ERROR: flags verification failed");
        return 0;
    }

    Text_processing(files.read_files[0], files.write_files[0]);
}

bool Text_processing(char* input_file_name, char* output_file_name) {

    size_t file_size = get_file_size(input_file_name);

    FILE* input_file = fopen(input_file_name, "rb");
    if(!input_file) {

        DEBUG_PRINTF("ERROR: file was not opened\n");
        return false;
    }

    char* file_buffer = (char*) calloc(file_size, sizeof(char));
    if(!file_buffer) {

        DEBUG_PRINTF("ERROR: memory was not allocated\n");
        return false;
    }

    fread(file_buffer, sizeof(char), file_size, input_file);

    fclose(input_file);
    Buffer first_stage_buffer  = First_stage_processing(file_buffer, file_size);
    free(file_buffer);
    DEBUG_PRINTF("END\n");
    Buffer second_stage_buffer = Second_stage_processing(&first_stage_buffer);
    free(first_stage_buffer.buffer);
    FILE* output_file = fopen(output_file_name, "wb");
    if(!output_file) {

        DEBUG_PRINTF("ERROR: file was not opened\n");
        return false;
    }

    printf("second_stage_buffer.words = %d", second_stage_buffer.words);
    fwrite(second_stage_buffer.buffer, Stpride, second_stage_buffer.words,output_file);
    free(second_stage_buffer.buffer);
    fclose(output_file);

    return true;
}

Buffer First_stage_processing(char* file_buffer, size_t buffer_size) {

    size_t max_out_size = buffer_size * 2;
    char* first_stage_buffer = (char*) calloc(max_out_size, sizeof(char));
    Buffer buffer = {.buffer = first_stage_buffer, .buffer_size = 0, .words = 0};

    if (!first_stage_buffer) {
        DEBUG_PRINTF("ERROR: memory was not allocated\n");
        return buffer;
    }

    bool inword = false;
    size_t fs_buf_index = 0;

    DEBUG_PRINTF("buffer_size = %d\n", buffer_size);
    for (size_t buf_index = 0; buf_index < buffer_size; buf_index++) {

        //DEBUG_PRINTF("buf_index = %d\n", buf_index);
        char ch = file_buffer[buf_index];
        bool is_word_char = isalpha(ch) || (ch == '-');

        if (isalpha(ch) && !inword)
            inword = true;

        if (inword && is_word_char) {

            if (fs_buf_index < max_out_size) {

                //DEBUG_PRINTF("%c\n", ch);
                first_stage_buffer[fs_buf_index] = ch;
                fs_buf_index++;
            }

            else {

                DEBUG_PRINTF("ERROR: buffer overflow while copying word\n");
                break;
            }
        }

        else if (inword && !is_word_char) {

            if (fs_buf_index < max_out_size) {

                first_stage_buffer[fs_buf_index] = '\n';
                fs_buf_index++;
                buffer.words++;
            }

            else {

                DEBUG_PRINTF("ERROR: buffer overflow on word end\n");
                break;
            }

            inword = false;
        }
    }

    if (inword && fs_buf_index < max_out_size) {

        first_stage_buffer[fs_buf_index] = '\n';
        fs_buf_index++;
        buffer.words++;
    }

    buffer.buffer_size = fs_buf_index;
    return buffer;
}


Buffer Second_stage_processing(Buffer* first_stage_buffer) {

    char* second_stage_buffer = (char*) calloc(first_stage_buffer->words, Stpride);
    Buffer buffer = {.buffer = second_stage_buffer, .buffer_size = 0, .words = 0};
    if(!first_stage_buffer) {

        DEBUG_PRINTF("ERROR: memory was not allocated\n");
        return buffer;
    }

    char* first_buffer = first_stage_buffer->buffer;
    size_t words_limit = first_stage_buffer->buffer_size;
    size_t ss_buf_symbol = 0;
    for(int buf_word = 0; buf_word < words_limit;) {

        size_t word_len = strlen_nl(first_buffer + buf_word);
        if(word_len <= Stpride) {

            memcpy(second_stage_buffer + ss_buf_symbol, first_buffer + buf_word, word_len);
            ss_buf_symbol += Stpride;
        }

        buf_word += word_len + 1;
    }

    buffer.words = ss_buf_symbol / Stpride;
    buffer.buffer_size = ss_buf_symbol;

    return buffer;
}

size_t strlen_nl(char* string) {

    size_t len = 0;
    while(string[len] != '\n' && string[len] != '\r')
        len++;

    return len;
}
