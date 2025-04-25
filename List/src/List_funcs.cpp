#include "List_funcs.h"
#include "List_debug.h"
#include"Hash_def_and_decl.h"

bool List_Ctor(List* list, size_t list_size) {

    List_data* tmp_ptr = (List_data*) calloc(list_size, sizeof(List_data));
    if(tmp_ptr) {

        list->list_data = tmp_ptr;
        tmp_ptr = NULL;
        list->list_size = list_size;
    }

    else {

        DEBUG_PRINTF("ERROR: memory was not allocated\n");
        return false;
    }

    List_connector(list, 0);

    list->list_data[0].next = 0;
    list->list_data[0].prev = 0;
    list->free_element = 1;
    list->elements_amount = 0;

    return true;
}

static void List_connector(List* list, int64_t start_index) {

    for(int64_t i = start_index; i < list->list_size; i++) {

        list->list_data[i].next = (i + 1) % list->list_size;
        list->list_data[i].prev = -1;
    }
}

void List_Dtor(List* list) {

    if(list && list->list_data)
        free(list->list_data);

    else
        DEBUG_PRINTF("ERROR: null pointer\n");
}

bool List_insert(List* list, int64_t index, void* value, size_t value_size) {

    if(list->list_data[index].prev == -1 || index >= list->list_size) {

        DEBUG_PRINTF("ERROR: can't insert after element out of list\n");
        return false;
    }

    int64_t free_index_copy = list->list_data[list->free_element].next;

    memcpy(list->list_data[list->free_element].element, value, value_size);

    list->list_data[list->list_data[index].next].prev = list->free_element;
    list->list_data[list->free_element].next = list->list_data[index].next;
    list->list_data[list->free_element].prev = index;
    list->list_data[index].next = list->free_element;
    list->free_element = free_index_copy;

    if(!list->free_element) {

        if(!List_expand(list, Realloc_coeff))
            return false;

        List_connector(list, list->list_size / 2);
        list->free_element = list->list_size / 2;
        DEBUG_PRINTF("List was expanded\n");
    }

    list->elements_amount++;
    LIST_DEBUG_CONSOLE(list);

    return true;
}

bool List_erase(List* list, int64_t index) {

    if(list->list_data[index].prev == -1) {

        DEBUG_PRINTF("ERROR: cant' erase element out of list\n");
        return false;
    }

    memset(&list->list_data[index].element, 0, sizeof(int64_t));

    list->list_data[list->list_data[index].next].prev = list->list_data[index].prev;
    list->list_data[list->list_data[index].prev].next = list->list_data[index].next;
    list->list_data[index].prev = -1;
    list->list_data[index].next = list->free_element;
    list->free_element = index;
    list->elements_amount--;
    LIST_DEBUG_CONSOLE(list);

    return true;
}

bool List_expand(List* list, unsigned int coeff) {

    if(!list) {

        DEBUG_PRINTF("ERROR: null pointer\n");
        return false;
    }

    if(coeff < 1) {

        DEBUG_PRINTF("ERROR: wrong expansion coefficient\n");
        return false;
    }

    List_data* tmp_struct_ptr = (List_data*) realloc(list->list_data, sizeof(List_data) * list->list_size * coeff);
    if(tmp_struct_ptr) {

        list->list_data = tmp_struct_ptr;
        tmp_struct_ptr = NULL;
        list->list_size *= coeff;
        return true;
    }

    else {

        DEBUG_PRINTF("ERROR: memory was not reallocated\n")
        return false;
    }
}

int64_t List_find_element_c(List* list, void* element, int size, Compare_func_t compare_func) {

    int64_t index = 0;
    do {

        List_data tmp_list_el = list->list_data[index];
        if(compare_func(&tmp_list_el.element, element, size) == Elements_equal)
            return index;

        index = tmp_list_el.next;

    } while(index != 0);

    return Not_found_value;
}

/*int64_t List_find_element(List* list, void* element, int size, Compare_func_t compare_func) {
//                              r13            r12        ebp                   rbx
    int64_t index_next = 0;  // rax
    int64_t index_prev = list->list_data[index_next].prev;
    //int64_t stop_prev_index = index_prev;
    do {

        List_data tmp_list_el_next = list->list_data[index_next];         // r14
        List_data tmp_list_el_prev = list->list_data[index_prev];
        if(compare_func(&tmp_list_el_next.element, element, size) == Elements_equal)
            return index_next;

        if(compare_func(&tmp_list_el_prev.element, element, size) == Elements_equal)
            return index_prev;

        index_next = tmp_list_el_next.next;       // r15
        index_prev = tmp_list_el_prev.prev;

    } while(index_next < index_prev);

    return Not_found_value;
}*/

//function for strings
int Compare_strings_256(void* element1, void* element2, int el_size) {

    CHECK_ELEMENTS(element1, element2)
    return Strcmp_256((const char*)element1, (const char*)element2);
}

int Compare_strings(void* element1, void* element2, int el_size) {

    CHECK_ELEMENTS(element1, element2)
    return strcmp((const char*)element1, (const char*)element2) - 1;
}

int Strcmp_256_inline(const char* s1, const char* s2) {
    int result;

    __asm__ __volatile__ (
        "vmovdqu (%1), %%ymm0        \n\t"  // загрузка из element1
        "vpcmpeqb (%2), %%ymm0, %%ymm2\n\t" // сравнение с element2
        "vpmovmskb %%ymm2, %0        \n\t"  // битовая маска
        "vzeroupper                  \n\t"
        : "=r"(result)                   // выход: %0
        : "r"(s1), "r"(s2)   // входы: %1, %2
        : "ymm0", "ymm2"                 // разрушаемые регистры
    );

    return result;
}

int Strcmp_ymm(const char* string1, const char* string2) {

    __m256i vec1 = _mm256_loadu_si256((const __m256i*)string1);
    __m256i vec2 = _mm256_loadu_si256((const __m256i*)string2);

    __m256i cmp_result = _mm256_cmpeq_epi8(vec1, vec2);

    return _mm256_movemask_epi8(cmp_result);
}

int64_t List_get_first(List* list) {

    return list->list_data[0].next;
}

int64_t List_get_last(List* list) {

    return list->list_data[0].prev;
}
