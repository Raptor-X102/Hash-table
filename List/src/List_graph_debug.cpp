#include "List_graph_debug.h"

void List_graph_debug(List* list, const char* output_file_name, void (*Print_list_data) (void* data, FILE* file)) {

    FILE* output_file = fopen(output_file_name, "wb+");
    if(!output_file) {

        DEBUG_PRINTF("ERROR: file %s was not opened\n", output_file_name);
        return;
    }

    //PRINTOUT("<pre>\n");
    //PRINTOUT("<graph>\n");
    PRINTOUT("digraph List{\n");
    PRINTOUT("rankdir=LR;\n");
    PRINTOUT("bgcolor=\"#f5cbaf\"\n");
    //PRINTOUT("node[fontsize=14, style=filled, fillcolor = \"#f56e31\", fontcolor=\"#000000\", shape=\"rectangle\",style=\"rounded\"];\n");
    PRINTOUT("edge[color=\"#ff05ff\",fontcolor=\"blue\",fontsize=12, splines = none];\n\n");

    Create_nodes(list, output_file, Print_list_data);

    PRINTOUT("node0 -> node%zd [weight = 1000, constraint = false, color = \"invis\"];\n",
                                                                 list->list_data[0].next);

    Make_next_edges(list, output_file, list->list_data[0].next, 1000, "invis", false);

    Make_next_edges(list, output_file, list->free_element, 1000, "invis", false);

    PRINTOUT("node0 -> node%zd [weight = 1000, constraint = true, color = \"#f257b2\"];\n",
                                                                  list->list_data[0].next);
    Make_next_edges(list, output_file, list->list_data[0].next, 100, "#f257b2", true);
    Make_next_edges(list, output_file, list->free_element, 100, "#610c0c", true);
    PRINTOUT("node0 -> node%zd [weight = 500, constraint = true, color = \"#9560f0\"];\n",
                                                                 list->list_data[0].prev);
    Make_prev_edges(list, output_file, list->list_data[0].prev, 50, "#9560f0", true);

    PRINTOUT("\n}\n");

    fclose(output_file);

    system("dot -Tpng C:\\Users\\mihai\\Documents\\VSCode\\List\\List_graph_debug.dot "
                                                              "-o List_graph_dump.png");
}

void Create_nodes(List* list, FILE* output_file, void (*Print_list_data) (void* data, FILE* file)) {

    PRINTOUT("node0[color=\"#000000\", "
             "fillcolor = \"#f5ef42\", "
             "shape = record, style=filled, "
             "label = \"{ <prev> | {");
    PRINTOUT("Index:0 | ");
    PRINTOUT("data: 0 | ");
    PRINTOUT("next: %d | ", list->list_data[0].next);
    PRINTOUT("prev: %d} | <next> }\"];\n", list->list_data[0].prev);

    int64_t curr_index = list->list_data[0].next;
    while(curr_index) {

        PRINTOUT("node%zd[color = \"#fc0505\", fillcolor = \"#f57e42\", "
                  "style=filled, shape = record,  label = \"{ <prev> | { ", curr_index);
        PRINTOUT("Index: %zd | data: ", curr_index);
        Print_list_data(&list->list_data[curr_index].element, output_file);
        PRINTOUT(" | next: %d | ", list->list_data[curr_index].next);
        PRINTOUT("prev: %d }  | <next> }\"];\n", list->list_data[curr_index].prev);

        curr_index = list->list_data[curr_index].next;
    }

    PRINTOUT("node%zd[color = \"#000000\", fillcolor = \"#d65858\", style=filled,"
              "shape = record,  label = \"{ <prev> f\\nr\\ne\\ne | { ", list->free_element);
    PRINTOUT("Index: %zd | data: ", list->free_element);
    Print_list_data(&list->list_data[list->free_element].element, output_file);
    PRINTOUT(" | next: %d | ", list->list_data[list->free_element].next);
    PRINTOUT("prev: %d }  | <next> }\"];\n", list->list_data[list->free_element].prev);
    PRINTOUT("\n");

    curr_index = list->list_data[list->free_element].next;
    while(curr_index) {

        PRINTOUT("node%zd[color = \"#fc0505\", fillcolor = \"#e67c7c\", "
                  "style=filled, shape = record,  label = \"{ <prev> | { ", curr_index);
        PRINTOUT("Index: %zd | data: ", curr_index);
        Print_list_data(&list->list_data[curr_index].element, output_file);
        PRINTOUT(" | next: %d | ", list->list_data[curr_index].next);
        PRINTOUT("prev: %d }  | <next> }\"];\n", list->list_data[curr_index].prev);

        curr_index = list->list_data[curr_index].next;
    }
}

void Make_next_edges(List* list, FILE* output_file, int64_t start_index, int weight, const char* color, bool constraint) {

    while (start_index) {

        int64_t tmp_index = start_index;
        start_index = list->list_data[start_index].next;
        int weight_copy = weight;

        if(start_index == 0)
            weight = 0;

        printf("%d", tmp_index);
        printf(" -> %d\n", start_index);
        PRINTOUT("node%zd:next", tmp_index);
        if (constraint) {

            PRINTOUT(" -> node%zd:prev [weight = ", start_index);
            PRINTOUT("%d, constraint = true, color = \"", weight);
            PRINTOUT("%s\"];\n", color);
        }
        else {

            PRINTOUT(" -> node%zd:prev [weight = ", start_index);
            PRINTOUT("%d, constraint = false, color = \"", weight);
            PRINTOUT("%s\"];\n", color);
        }

        weight = weight_copy;
    }
}

void Make_prev_edges(List* list, FILE* output_file, int64_t start_index,
                     int weight, const char* color, bool constraint) {

    while (start_index) {

        int64_t tmp_index = start_index;
        start_index = list->list_data[start_index].prev;
        int weight_copy = weight;

        if(start_index == 0)
            weight = 0;

        PRINTOUT("node%zd:prev", tmp_index);

        if (constraint) {

            PRINTOUT(" -> node%zd:next [weight = ", start_index);
            PRINTOUT("%d, constraint = true, color = \"", weight);
            PRINTOUT("%s\"];\n", color);
        }
        else {

            PRINTOUT(" -> node%zd:next [weight = ", start_index);
            PRINTOUT("%d, constraint = false, color = \"", weight);
            PRINTOUT("%s\"];\n", color);
        }

        weight = weight_copy;
    }
}
