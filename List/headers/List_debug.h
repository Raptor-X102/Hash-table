#ifndef LIST_DEBUG
#define LIST_DEBUG

#include <stdio.h>
#include "List_definitions.h"
#include "Debug_printf.h"

void List_debug_console(List* list);

#ifndef NO_DEBUG
#define LIST_DEBUG_CONSOLE(list) List_debug_console(list)
#else
#define LIST_DEBUG_CONSOLE(list)
#endif
#endif
