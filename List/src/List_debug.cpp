#include "List_debug.h"

void List_debug_console(List* list) {

    DEBUG_PRINTF("List:         free = %lld\n", list->free_element);
    DEBUG_PRINTF("INDEX     VALUE    NEXT    PREV\n");
    for(int64_t i = 0; i < list->list_size; i++) {

        DEBUG_PRINTF("%5d     ", i);
        DEBUG_PRINTF("%5.2lg   ", list->list_data[i].element);
        DEBUG_PRINTF("%5d   ", list->list_data[i].next);
        DEBUG_PRINTF("%5d\n", list->list_data[i].prev);
    }
    //
}
