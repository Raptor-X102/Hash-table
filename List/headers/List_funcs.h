#ifndef LIST_FUNCS
#define LIST_FUNCS

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <xmmintrin.h>
#include <immintrin.h>
#include <nmmintrin.h>
#include "Debug_printf.h"
#include "List_definitions.h"
#ifndef NO_DEBUG
#define CHECK_ELEMENTS(element1, element2) if(!element1 || !element2) {\
                                                    \
                                                DEBUG_PRINTF("ERROR: null ptr\n");\
                                                return Error_value;\
                                            }
#else
#define CHECK_ELEMENTS(element1, element2)
#endif

const size_t Realloc_shrink_limit = 4;
const size_t Realloc_coeff = 2;
const int64_t Error_value = 0xDEAD;
const int64_t Not_found_value = -0x404;
const int Elements_equal = 0xFFFFFFFF;
const int Elements_not_equal = 0xFACE;
typedef int (*Compare_func_t) (void* element1, void* element2, int el_size);
bool List_Ctor(List* list, size_t list_size);
static void List_connector(List* list, int64_t start_index);
void List_Dtor(List* list);
bool List_insert(List* list, int64_t index, void* value, size_t value_size);
bool List_erase(List* list, int64_t index);
bool List_expand(List* list, unsigned int coeff);
int64_t List_find_element_c(List* list, void* element, int size, Compare_func_t compare_func);
extern "C" int64_t List_find_element(List* list, void* element, int size,
                         int (*compare_func)(void*, void*, int));
int Compare_strings_256(void* element1, void* element2, int el_size);
int Compare_strings(void* element1, void* element2, int el_size);
int64_t List_get_first(List* list);
int64_t List_get_last(List* list);
extern "C" {int Strcmp_256(const char* string1, const char* string2);}
int Strcmp_ymm(const char* string1, const char* string2);
int Strcmp_256_inline(const char* s1, const char* s2);

#endif
