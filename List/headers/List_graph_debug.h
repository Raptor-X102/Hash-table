#ifndef LIST_GD
#define LIST_GD

#include <stdlib.h>
#include <stdio.h>
#include "List_definitions.h"
#include "Debug_printf.h"

#define PRINTOUT(...) fprintf(output_file, __VA_ARGS__)

void List_graph_debug(List* list, const char* output_file_name, void (*Print_list_data) (void* data, FILE* file));
void Create_nodes(List* list, FILE* output_file, void (*Print_list_data) (void* data, FILE* file));
void Make_next_edges(List* list, FILE* output_file, int64_t start_index,
                     int weight, const char* color, bool constraint);
void Make_prev_edges(List* list, FILE* output_file, int64_t start_index,
                     int weight, const char* color, bool constraint);

#endif
