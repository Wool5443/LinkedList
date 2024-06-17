#include <stdio.h>
#include <stdlib.h>
#include "ListSettings.ini"
#include "PrettyDumpList.hpp"

#define FONT_SIZE "10"
#define FONT_NAME "\"Fira Code Bold\""
#define BACK_GROUND_COLOR "\"#de97d4\""
#define NODE_COLOR "\"#fae1f6\""
#define NODE_FRAME_COLOR "\"#000000\""
#define ROOT_COLOR "\"#c95b90\""
#define FREE_HEAD_COLOR "\"#b9e793\""

static size_t DUMP_ITER = 0;
static FILE*  HTML_FILE = NULL;

static const size_t MAX_LEN_PATH    = 256;
static const size_t MAX_COMMAND_LEN = 640;

#define PRINT_LOG(...)                                                          \
do                                                                              \
{                                                                               \
    fprintf(outTextFile, __VA_ARGS__);                                          \
    if (HTML_FILE)                                                              \
        fprintf(HTML_FILE, __VA_ARGS__);                                        \
} while (0)             

ErrorCode StartHtmlLogging(const char* logFolder)
{
    char htmlLogPath[MAX_LEN_PATH] = "";
    sprintf(htmlLogPath, "%s/log.html", logFolder);

    HTML_FILE = fopen(htmlLogPath, "w");
    if (!HTML_FILE) return ERROR_BAD_FILE;

    return EVERYTHING_FINE;
}

ErrorCode EndHtmlLogging()
{
    if (HTML_FILE)
        fclose(HTML_FILE);
    HTML_FILE = NULL;

    return EVERYTHING_FINE;
}

ErrorCode _dumpList(LinkedList* list, ErrorCode error, SourceCodePosition* caller)
{
    MyAssertHard(list,   ERROR_NULLPTR);
    MyAssertHard(caller, ERROR_NULLPTR);

    char outTextPath[MAX_LEN_PATH] = "";
    sprintf(outTextPath, "%s/txt/iter%zu.txt", list->logFolder, DUMP_ITER);

    char outGraphPath[MAX_LEN_PATH] = "";
    sprintf(outGraphPath, "%s/dot/iter%zu.dot", list->logFolder, DUMP_ITER);

    if (HTML_FILE)
        fprintf(HTML_FILE, "<h1>Iteration %zu</h1>\n<pre>\n", DUMP_ITER);

    RETURN_ERROR(_dumpListText(list, error, caller, outTextPath));

    if (HTML_FILE)
        fprintf(HTML_FILE, "</pre>\n");

    RETURN_ERROR(DumpListGraph(list, outGraphPath));

    char outImgPath[MAX_LEN_PATH] = "";
    sprintf(outImgPath, "%s/img/iter%zu.png", list->logFolder, DUMP_ITER);
    
    char command[MAX_COMMAND_LEN] = "";
    sprintf(command, "dot %s -T png -o %s", outGraphPath, outImgPath);

    system(command);

    if (HTML_FILE)
        fprintf(HTML_FILE, "<img src = \"%s\"/>\n", outImgPath);
    DUMP_ITER++;
    return EVERYTHING_FINE;
}

ErrorCode _dumpListText(LinkedList* list, ErrorCode error, SourceCodePosition* caller, const char* outTextPath)
{
    MyAssertSoft(list,        ERROR_NULLPTR);
    MyAssertSoft(outTextPath, ERROR_BAD_FILE);

    FILE* outTextFile = fopen(outTextPath, "w");
    if (!outTextFile) return ERROR_BAD_FILE;

    PRINT_LOG("List[%p] called from %s(%zu) %s()\n", list, caller->fileName,
                                                           caller->line,
                                                           caller->name);
    PRINT_LOG("List condition - %s[%d]\n", ERROR_CODE_NAMES[error], error);

    PRINT_LOG("{\n");
    PRINT_LOG("%3s length = %zu\n", "",    list->length);
    PRINT_LOG("%3s capacity = %zu\n", "",  list->capacity);
    PRINT_LOG("%3s head = %zu\n", "",     *list->head);
    PRINT_LOG("%3s tail = %zu\n", "",     *list->tail);
    PRINT_LOG("%3s free head = %zu\n", "", list->freeHead);
    PRINT_LOG("%3s list:\n", "");
    {
        size_t curEl    = *list->head;
        size_t orderNum = 1;
        while (curEl != 0 && orderNum <= list->length * 2)
        {
            PRINT_LOG("%4s", "");
            if (list->data[curEl] == LIST_POISON || !isfinite((double)list->data[curEl]))
                PRINT_LOG(" [%zu] = POISON\n", orderNum);
            else
                PRINT_LOG("*[%zu] = " LIST_EL_SPECIFIER "\n", orderNum, list->data[curEl]);

            curEl = list->next[curEl];
            orderNum++;
        }
    }

    PRINT_LOG("\n%3s data[%p]\n", "", list->data);
    for (size_t i = 0; i < list->capacity; i++)
    {
        PRINT_LOG("%4s", "");
        if (list->data[i] == LIST_POISON || !isfinite((double)list->data[i]))
            PRINT_LOG(" [%zu] = POISON\n", i);
        else
            PRINT_LOG("*[%zu] = " LIST_EL_SPECIFIER "\n", i, list->data[i]);
    }

    PRINT_LOG("\n%3s prev[%p]\n", "", list->prev);
    for (size_t i = 0; i < list->capacity; i++)
    {
        PRINT_LOG("%4s", "");
        if (list->prev[i] != FREE_ELEM)
            PRINT_LOG("*[%zu] = %zu\n", i, list->prev[i]);
        else
            PRINT_LOG(" [%zu] = FREE\n", i);
    }

    PRINT_LOG("\n%3s next[%p]\n", "", list->next);
    for (size_t i = 0; i < list->capacity; i++)
    {
        PRINT_LOG("%4s", "");
        if (list->next[i] != FREE_ELEM)
            PRINT_LOG("*[%zu] = %zu\n", i, list->next[i]);
        else
            PRINT_LOG(" [%zu] = BAD\n", i);
    }
        
    PRINT_LOG("}\n");

    fclose(outTextFile);

    return EVERYTHING_FINE;
}

ErrorCode DumpListGraph(LinkedList* list, const char* outGraphPath)
{
    if (!list)         return ERROR_NULLPTR;
    if (!outGraphPath) return ERROR_NULLPTR;

    FILE* outGraphFile = fopen(outGraphPath, "w");
    if (!outGraphFile) return ERROR_BAD_FILE;

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
        "CELL_%zu[style = \"filled\", fillcolor = " NODE_COLOR ", ", i);
        fprintf(outGraphFile, "label = \"index = %zu|", i);

        if (list->data[i] == LIST_POISON || !isfinite((double)list->data[i]))
            fprintf(outGraphFile, "value\\nPOISON|");
        else
            fprintf(outGraphFile, "value\\n" LIST_EL_SPECIFIER "|", list->data[i]);

        if (list->prev[i] == FREE_ELEM)
            fprintf(outGraphFile, "{prev = FREE|");
        else
            fprintf(outGraphFile, "{prev = %zu|", list->prev[i]);

        if (list->next[i] == FREE_ELEM)
            fprintf(outGraphFile, "next = FREE}\"];\n");
        else
            fprintf(outGraphFile, "next = %zu}\"];\n", list->next[i]);
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
