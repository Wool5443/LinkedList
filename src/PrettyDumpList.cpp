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

ErrorCode DumpList(LinkedList* list, ErrorCode error, const char* outTextPath, const char* outGraphPath)
{
    MyAssertSoft(outTextPath, ERROR_BAD_FILE);
    MyAssertSoft(outGraphPath, ERROR_BAD_FILE);

    RETURN_ERROR(DumpListText(list, error, outTextPath));
    RETURN_ERROR(DumpListGraph(list, outGraphPath));
    
    return EVERYTHING_FINE;
}

ErrorCode _dumpListText(LinkedList* list, ErrorCode error, SourceCodePosition* caller, const char* outTextPath)
{
    MyAssertSoft(list, ERROR_NULLPTR);

    MyAssertSoft(outTextPath, ERROR_BAD_FILE);

    FILE* outTextFile = fopen(outTextPath, "w");
    MyAssertSoft(outTextFile, ERROR_BAD_FILE);

    fprintf(outTextFile, "List[%p] called from %s(%zu) %s()\n", list, caller->fileName,
                                                                       caller->line,
                                                                       caller->name);
    fprintf(outTextFile, "List condition - %s[%d]\n", ERROR_CODE_NAMES[error], error);

    fprintf(outTextFile, "{\n");
    fprintf(outTextFile, "%3s length = %zu\n", "",    list->length);
    fprintf(outTextFile, "%3s capacity = %zu\n", "",  list->capacity);
    fprintf(outTextFile, "%3s head = %zu\n", "",     *list->head);
    fprintf(outTextFile, "%3s tail = %zu\n", "",     *list->tail);
    fprintf(outTextFile, "%3s free head = %zu\n", "", list->freeHead);
    fprintf(outTextFile, "%3s list:\n", "");
    {
        size_t curEl    = *list->head;
        size_t orderNum = 1;
        while (curEl != 0 && orderNum <= list->length * 2)
        {
            fprintf(outTextFile, "%4s", "");
            if (list->data[curEl] != LIST_POISON)
                fprintf(outTextFile, "*[%zu] = " LIST_EL_SPECIFIER "\n", orderNum, list->data[curEl]);
            else
                fprintf(outTextFile, " [%zu] = POISON\n", orderNum);

            curEl = list->next[curEl];
            orderNum++;
        }
    }

    fprintf(outTextFile, "\n%3s data[%p]\n", "", list->data);
    for (size_t i = 0; i < list->capacity; i++)
    {
        fprintf(outTextFile, "%4s", "");
        if (list->data[i] != LIST_POISON)
            fprintf(outTextFile, "*[%zu] = " LIST_EL_SPECIFIER "\n", i, list->data[i]);
        else
            fprintf(outTextFile, " [%zu] = POISON\n", i);
    }

    fprintf(outTextFile, "\n%3s prev[%p]\n", "", list->prev);
    for (size_t i = 0; i < list->capacity; i++)
    {
        fprintf(outTextFile, "%4s", "");
        if (list->prev[i] != FREE_ELEM)
            fprintf(outTextFile, "*[%zu] = %zu\n", i, list->prev[i]);
        else
            fprintf(outTextFile, " [%zu] = FREE\n", i);
    }

    fprintf(outTextFile, "\n%3s next[%p]\n", "", list->next);
    for (size_t i = 0; i < list->capacity; i++)
    {
        fprintf(outTextFile, "%4s", "");
        if (list->next[i] != FREE_ELEM)
            fprintf(outTextFile, "*[%zu] = %zu\n", i, list->next[i]);
        else
            fprintf(outTextFile, " [%zu] = BAD\n", i);
    }
        
    fprintf(outTextFile, "}\n");

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
    "node[shape = record, color = " NODE_FRAME_COLOR ", fontname = " FONT_NAME ", fontsize = " FONT_SIZE "];\n"
    "bgcolor = " BACK_GROUND_COLOR ";\n"

    "ROOT[style = \"filled\", fillcolor = " ROOT_COLOR ", "
    "label = \"ROOT|{<head>head = %zu|<tail>tail = %zu}\"];\n"

    "FREE_HEAD[style = \"filled\", fillcolor = " FREE_HEAD_COLOR ", "
    "label = \"FREE HEAD|<freeHead>freeHead = %zu\"];\n",
    *list->head, *list->tail, list->freeHead
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
    for (size_t i = 2; i < list->capacity; i++)
        fprintf(outGraphFile, "->CELL_%zu", i);

    fprintf(outGraphFile, " [weight = 1000000000, color = " BACK_GROUND_COLOR "];\n");
    
    if (*list->head)
        fprintf(outGraphFile, "ROOT:head->CELL_%zu [style = \"bold\", color = white];\n", *list->head);
    
    if (*list->tail)
        fprintf(outGraphFile, "ROOT:tail->CELL_%zu [style = \"bold\", color = white];\n", *list->tail);

    if (*list->head != *list->tail)
    {
        fprintf(outGraphFile, "CELL_%zu", *list->head);
        size_t index = list->next[*list->head];
        while (index != 0)
        {
            fprintf(outGraphFile, "->CELL_%zu", index);
            index = list->next[index];
        }
        fprintf(outGraphFile, "[style = \"bold\", color = white];\n");
    }

    if (list->freeHead)
        fprintf(outGraphFile, "FREE_HEAD:freeHead->CELL_%zu[style = \"bold\", color = white];\n", list->freeHead);

    fprintf(outGraphFile, "}\n");

    fclose(outGraphFile);

    return EVERYTHING_FINE;
}
