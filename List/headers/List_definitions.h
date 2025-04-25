#ifndef LIST_DEF
#define LIST_DEF

#include <stdint.h>

struct List_data {

    int64_t prev;
    int64_t next;
    char element[32];
};

struct List {

    List_data* list_data;
    int64_t free_element;
    size_t list_size;
    size_t elements_amount;
};

#endif
