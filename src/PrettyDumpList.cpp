#include <stdio.h>
#include "ListSettings.ini"
#include "PrettyDumpList.hpp"

#define FONT_SIZE "10"
#define FONT_NAME "\"Fira Code Bold\""
#define BACK_GROUND_COLOR "\"#de97d4\""
#define NODE_COLOR "\"#fae1f6\""
#define NODE_FRAME_COLOR "\"#000000\""
#define ROOT_COLOR "\"#c95b90\""
#define FREE_HEAD_COLOR "\"#b9e793\""

ErrorCode DumpList(LinkedList* list, const char* outTextPath, const char* outGraphPath)
{
    MyAssertSoft(outTextPath, ERROR_BAD_FILE);
    MyAssertSoft(outGraphPath, ERROR_BAD_FILE);

    RETURN_ERROR(DumpListText(list, outTextPath));
    RETURN_ERROR(DumpListGraph(list, outGraphPath));
    
    return EVERYTHING_FINE;
}

ErrorCode DumpListText(LinkedList* list, const char* outTextPath)
{
    MyAssertSoft(outTextPath, ERROR_BAD_FILE);

    FILE* outTextFile = fopen(outTextPath, "w");
    MyAssertSoft(outTextFile, ERROR_BAD_FILE);

    {
        size_t i = list->head;
        size_t k = 1;
        while (i != 0)
        {
            fprintf(outTextFile, "List[%zu] = %lg\n", k, list->data[i]);
            i = list->next[i];
            k++;
        }
    }
    fprintf(outTextFile, "List head = %zu:%lg\n", list->head, list->data[list->head]);
    fprintf(outTextFile, "List tail = %zu:%lg\n", list->tail, list->data[list->tail]);
    fprintf(outTextFile, "List free head = %zu\n", list->freeHead);

    fprintf(outTextFile, "\n\n");

    for (size_t i = 0; i < DEFAULT_LIST_CAPACITY; i++)
        fprintf(outTextFile, "Data[%zu] = %lg\n", i, list->data[i]);
    fprintf(outTextFile, "\n\n");
    
    for (size_t i = 0; i < DEFAULT_LIST_CAPACITY; i++)
        fprintf(outTextFile, "Prev[%zu] = %zu\n", i, list->prev[i]);
    fprintf(outTextFile, "\n\n");

    for (size_t i = 0; i < DEFAULT_LIST_CAPACITY; i++)
        fprintf(outTextFile, "Next[%zu] = %zu\n", i, list->next[i]);
    fprintf(outTextFile, "\n\n");
        
    fclose(outTextFile);

    return EVERYTHING_FINE;
}

ErrorCode DumpListGraph(LinkedList* list, const char* outGraphPath)
{
    MyAssertSoft(outGraphPath, ERROR_BAD_FILE);

    FILE* outGraphFile = fopen(outGraphPath, "w");
    MyAssertSoft(outGraphFile, ERROR_BAD_FILE);

    fprintf(outGraphFile, 
    "digraph\n"
    "{\n"
    "rankdir = LR;\n"
    "node[shape = record, color = " NODE_FRAME_COLOR ", fontname = " FONT_NAME ", fontsize = " FONT_SIZE "]"
    "bgcolor = " BACK_GROUND_COLOR ";\n"

    "ROOT[style = \"filled\", fillcolor = " ROOT_COLOR ", "
    "label = \"ROOT|{<head>head = %zu|<tail>tail = %zu}\"];\n",
    list->head, list->tail
    );
    
    for (size_t i = 1; i < list->capacity; i++)
    {
        fprintf(outGraphFile, 
        "CELL_%zu[style = \"filled\", fillcolor = " NODE_COLOR ", "
        "label = \"index = %zu|value\\n%lg|{prev = %zu|next = %zu}\"];\n",
        i, i, list->data[i], list->prev[i], list->next[i]
        );
    }

    fprintf(outGraphFile, "ROOT->CELL_1");
    for (size_t i = 2; i < DEFAULT_LIST_CAPACITY; i++)
        fprintf(outGraphFile, "->CELL_%zu", i);

    fprintf(outGraphFile, " [weight = 1000000000, color = " BACK_GROUND_COLOR "];\n");

    if (list->head != 0)
        fprintf(outGraphFile, "ROOT:head->CELL_%zu [style = \"bold\", color = white];\n", list->head);
    
    if (list->tail != 0)
        fprintf(outGraphFile, "ROOT:tail->CELL_%zu [style = \"bold\", color = white];\n", list->tail);

    if (list->head != list->tail)
    {
        fprintf(outGraphFile, "CELL_%zu", list->head);
        size_t index = list->next[list->head];
        while (index != 0)
        {
            fprintf(outGraphFile, "->CELL_%zu", index);
            index = list->next[index];
        }
        fprintf(outGraphFile, "[style = \"bold\", color = white];\n");
    }

    fprintf(outGraphFile,
    "FREE_HEAD[style = \"filled\", fillcolor = " FREE_HEAD_COLOR ", "
    "label = \"FREE HEAD|<freeHead>freeHead = %zu\"];\n",
    list->freeHead
    );

    fprintf(outGraphFile, "FREE_HEAD:freeHead->CELL_%zu[style = \"bold\", color = white];\n", list->freeHead);

    fprintf(outGraphFile, "}\n");

    fclose(outGraphFile);

    return EVERYTHING_FINE;
}
