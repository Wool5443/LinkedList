#include <string.h>
#include <assert.h>
#include "LinkedList.hpp"
#include "MinMax.hpp"

#ifdef NDEBUG
#define ERR_DUMP_RET(...)
#define ERR_DUMP_RET_RES(...)
#else
#define ERR_DUMP_RET(listPtr)               \
do                                          \
{                                           \
    Error _error = listPtr->Verify();       \
    if (_error)                             \
    {                                       \
        listPtr->Dump(_error);              \
        return _error;                      \
    }                                       \
} while(0)
#define ERR_DUMP_RET_RES(listPtr, poison)   \
do                                          \
{                                           \
    Error _error = listPtr->Verify();       \
    if (_error)                             \
    {                                       \
        listPtr->Dump(_error);              \
        return { poison, _error };          \
    }                                       \
} while(0)
#endif

static Error _printListElement(FILE* file, ListElement_t* listEl);
static Error _dumpListText(const LinkedList* list, const Error& error, const char* outTextPath);
static Error _dumpListGraph(const LinkedList* list, const char* outGraphPath);
static Error _listReallocUp(LinkedList* list);

Error LinkedList::Init() noexcept
{
    *this = {};

    ListElement_t* dataTemp = (ListElement_t*)calloc(DEFAULT_LIST_CAPACITY, sizeof(*dataTemp));
    if (!dataTemp)
        return CREATE_ERROR(ERROR_NO_MEMORY);

    size_t* nextTemp = (size_t*)calloc(DEFAULT_LIST_CAPACITY, sizeof(*nextTemp));
    if (!nextTemp)
        return CREATE_ERROR(ERROR_NO_MEMORY);

    size_t* prevTemp = (size_t*)calloc(DEFAULT_LIST_CAPACITY, sizeof(*prevTemp));
    if (!prevTemp)
        return CREATE_ERROR(ERROR_NO_MEMORY);

    for (size_t i = 0; i < DEFAULT_LIST_CAPACITY; i++)
        dataTemp[i] = LIST_POISON;

    for (size_t i = 1; i < DEFAULT_LIST_CAPACITY - 1; i++)
        nextTemp[i] = i + 1;

    for (size_t i = 1; i < DEFAULT_LIST_CAPACITY; i++)
        prevTemp[i] = FREE_ELEM;

    this->data      = dataTemp;
    this->next      = nextTemp;
    this->prev      = prevTemp;

    this->length    = 1;
    this->capacity  = DEFAULT_LIST_CAPACITY;

    this->head      = &nextTemp[0];
    this->tail      = &prevTemp[0];
    this->freeHead  = 1;

    return Error();
}

Error LinkedList::Destructor() noexcept
{
    ERR_DUMP_RET(this);

    size_t curEl = *this->head;

    while (curEl)
    {
        this->data[curEl].Destructor();
        curEl = this->next[curEl];
    }

    free(this->data);
    free(this->next);
    free(this->prev);

    this->capacity = FREE_ELEM;
    this->head     = NULL;
    this->tail     = NULL;

    return Error();
}

Error LinkedList::Verify() const noexcept
{
    if (!this->data || !this->next || !this->prev)
        return CREATE_ERROR(ERROR_NO_MEMORY);
    
    if (*this->head == FREE_ELEM || *this->tail == FREE_ELEM || this->freeHead == FREE_ELEM)
        return CREATE_ERROR(ERROR_INDEX_OUT_OF_BOUNDS);

    if (this->capacity <= *this->head || this->capacity <= *this->tail ||
                                         this->capacity < this->length)
        return CREATE_ERROR(ERROR_INDEX_OUT_OF_BOUNDS);

    return Error();
}

Error LinkedList::InsertAfter(ListElement_t value, size_t index) noexcept
{
    if (index >= this->capacity)
        return CREATE_ERROR(ERROR_INDEX_OUT_OF_BOUNDS);
    if (this->prev[index] == FREE_ELEM)
        return CREATE_ERROR(ERROR_INDEX_OUT_OF_BOUNDS);

    ERR_DUMP_RET(this);

    RETURN_ERROR(_listReallocUp(this));

    size_t insertIndex = this->freeHead;
    this->freeHead     = this->next[this->freeHead];
    this->length++;

    this->data[insertIndex] = value;

    this->prev[insertIndex] = index;
    this->next[insertIndex] = this->next[index];

    this->prev[this->next[index]] = insertIndex;
    this->next[index]             = insertIndex;

    return Error();
}

Error LinkedList::InsertBefore(ListElement_t value, size_t index) noexcept
{
    return this->InsertAfter(value, this->prev[index]);
}

Error LinkedList::PushBack(ListElement_t value) noexcept
{
    return this->InsertAfter(value, *this->tail);
}

Error LinkedList::PushFront(ListElement_t value) noexcept
{
    return this->InsertBefore(value, *this->head);
}

ListElemResult LinkedList::Pop(size_t index) noexcept
{
    if (index < 1 || index >= this->capacity)
        return { LIST_POISON, CREATE_ERROR(ERROR_INDEX_OUT_OF_BOUNDS) };
    if (this->prev[index] == FREE_ELEM)
        return { LIST_POISON, CREATE_ERROR(ERROR_INDEX_OUT_OF_BOUNDS) };

    ERR_DUMP_RET_RES(this, LIST_POISON);

    ListElement_t value = this->data[index];
    this->data[index]   = LIST_POISON;
    this->length--;

    this->next[this->prev[index]] = this->next[index];
    this->prev[this->next[index]] = this->prev[index];

    this->prev[index] = FREE_ELEM;
    this->next[index] = this->freeHead;
    this->freeHead    = index;

    return { value, Error() };
}

ListElemResult LinkedList::Pop() noexcept
{
    return this->Pop(*this->tail);
}

Error _listReallocUp(LinkedList* list)
{
    SoftAssert(list, ERROR_NULLPTR);

    if (list->freeHead)
        return Error();

    size_t newCapacity = list->capacity * LIST_GROW_FACTOR;

    ListElement_t* newData = (ListElement_t*)realloc(list->data, newCapacity * sizeof(*newData));
    if (!newData) return CREATE_ERROR(ERROR_NO_MEMORY);

    size_t* newPrev = (size_t*)realloc(list->prev, newCapacity * sizeof(*newPrev));
    if (!newPrev) return CREATE_ERROR(ERROR_NO_MEMORY);

    size_t* newNext = (size_t*)realloc(list->next, newCapacity * sizeof(*newNext));
    if (!newNext) return CREATE_ERROR(ERROR_NO_MEMORY);

    for (size_t i = list->capacity; i < newCapacity - 1; i++)
        newNext[i] = i + 1;
    newNext[newCapacity - 1] = 0;

    for (size_t i = list->capacity; i < newCapacity; i++)
    {
        newData[i] = LIST_POISON;
        newPrev[i] = FREE_ELEM;
    }

    list->data = newData;
    list->next = newNext;
    list->prev = newPrev;
    
    list->head = &newNext[0];
    list->tail = &newPrev[0];
    list->freeHead = list->capacity;

    list->capacity = newCapacity;

    return Error();
}

Error LinkedList::ReallocDownAndUntangle() noexcept
{
    size_t newCapacity = max(DEFAULT_LIST_CAPACITY, this->length);

    ListElement_t* newData = (ListElement_t*)calloc(newCapacity, sizeof(*newData));
    if (!newData) return CREATE_ERROR(ERROR_NO_MEMORY);

    size_t* newPrev = (size_t*)calloc(newCapacity, sizeof(*newPrev));
    if (!newPrev) return CREATE_ERROR(ERROR_NO_MEMORY);

    size_t* newNext = (size_t*)calloc(newCapacity, sizeof(*newNext));
    if (!newNext) return CREATE_ERROR(ERROR_NO_MEMORY);

    for (size_t i = 0; i < newCapacity; i++)
        newData[i] = LIST_POISON;

    for (size_t i = 0; i < newCapacity - 1; i++)
        newNext[i] = i + 1;

    for (size_t i = 1; i < newCapacity; i++)
        newPrev[i] = FREE_ELEM;

    size_t newInd = 1;
    {
        size_t oldInd = *this->head;

        if (*this->head)
            newNext[0] = 1;

        while (oldInd)
        {
            newData[newInd] = this->data[oldInd];
            newPrev[newInd] = newInd - 1;
            oldInd          = this->next[oldInd];
            newInd++;
        }
    }
    newInd--;

    newNext[newInd] = 0;
    newPrev[0] = newInd;

    free(this->data);
    free(this->next);
    free(this->prev);

    this->data = newData;
    this->next = newNext;
    this->prev = newPrev;

    this->capacity = newCapacity;

    this->head = &newNext[0];
    this->tail = &newPrev[0];
    this->freeHead = this->length % newCapacity;

    return Error();
}

ListElemIndexResult LinkedList::GetByIndex(size_t index) const noexcept
{
    if (index < 1 || index >= this->capacity)
        return { SIZET_POISON, CREATE_ERROR(ERROR_INDEX_OUT_OF_BOUNDS) };
    if (this->prev[index] == FREE_ELEM)
        return { SIZET_POISON, CREATE_ERROR(ERROR_INDEX_OUT_OF_BOUNDS) };

    ERR_DUMP_RET_RES(this, 0);
    
    size_t curEl = *this->head;
    size_t i = 1;

    while (i < index && curEl)
    {
        curEl = this->next[curEl];
        i++;
    }

    return { curEl, curEl ? Error() : CREATE_ERROR(ERROR_NOT_FOUND) };
}

static const char*  LOG_FOLDER = nullptr;
static FILE*        HTML_FILE  = NULL;
static const size_t MAX_PATH_LENGTH    = 256;
static const size_t MAX_COMMAND_LENGTH = 640;

#define PRINT_LOG(...)                                                          \
do                                                                              \
{                                                                               \
    fprintf(outTextFile, __VA_ARGS__);                                          \
    if (HTML_FILE)                                                              \
        fprintf(HTML_FILE, __VA_ARGS__);                                        \
} while (0)

static Error _printListElement(FILE* file, ListElement_t* listEl)
{
    SoftAssert(file, ERROR_BAD_FILE);
    SoftAssert(listEl, ERROR_NULLPTR);

    switch (listEl->type)
    {
        case VARIABLE_SYMBOL:
            fprintf(file, "var %s = %lg", listEl->name.buf, listEl->value);
            break;
        case CONST_SYMBOL:
            fprintf(file, "cont %s = %lg", listEl->name.buf, listEl->value);
            break;
        case FUNCTION_SYMBOL:
            fprintf(file, "function %s = %p", listEl->name.buf, listEl->function);
            break;
        default:
            return Error();
    }

    return Error();
}

#define FONT_SIZE "10"
#define FONT_NAME "\"Fira Code Bold\""
#define BACK_GROUND_COLOR "\"#de97d4\""
#define TREE_COLOR "\"#ff7be9\""
#define NODE_COLOR "\"#fae1f6\""
#define NODE_FRAME_COLOR "\"#000000\""
#define ROOT_COLOR "\"#c95b90\""
#define FREE_HEAD_COLOR "\"#b9e793\""
Error LinkedList::Dump() const noexcept
{
    return this->Dump(this->Verify());
}
Error LinkedList::Dump(const Error& error) const noexcept
{
    static size_t DUMP_ITERATION = 0;

    char outTextPath[MAX_PATH_LENGTH] = "";
    sprintf(outTextPath, "%s/txt/iter%zu.txt", LOG_FOLDER, DUMP_ITERATION);

    char outGraphPath[MAX_PATH_LENGTH] = "";
    sprintf(outGraphPath, "%s/dot/iter%zu.dot", LOG_FOLDER, DUMP_ITERATION);

    if (HTML_FILE)
        fprintf(HTML_FILE, "<h1>Iteration %zu</h1>\n<pre>\n", DUMP_ITERATION);

    RETURN_ERROR(_dumpListText(this, error, outTextPath));

    if (HTML_FILE)
        fprintf(HTML_FILE, "</pre>\n");

    RETURN_ERROR(_dumpListGraph(this, outGraphPath));

    char outImgPath[MAX_PATH_LENGTH] = "";
    sprintf(outImgPath, "%s/img/iter%zu.png", LOG_FOLDER, DUMP_ITERATION);
    
    char command[MAX_COMMAND_LENGTH] = "";
    sprintf(command, "dot %s -T png -o %s", outGraphPath, outImgPath);

    system(command);

    if (HTML_FILE)
        fprintf(HTML_FILE, "<img src = \"%s\"/>\n", outImgPath);

    DUMP_ITERATION++;

    return Error();
}

Error _dumpListText(const LinkedList* list, const Error& error, const char* outTextPath)
{
    SoftAssert(list, ERROR_NULLPTR);

    FILE* outTextFile = fopen(outTextPath, "w");
    if (!outTextFile)
        return CREATE_ERROR(ERROR_BAD_FILE);

    PRINT_LOG("List[%p]\n", list);
    PRINT_LOG("List condition - %s[%d]\n", error.GetErrorName(), error.code);

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
            PRINT_LOG("%4s [%zu] = ", "", curEl);
            RETURN_ERROR(_printListElement(outTextFile, &list->data[curEl]));
            fputc('\n', outTextFile);

            curEl = list->next[curEl];
            orderNum++;
        }
    }

    PRINT_LOG("\n%3s data[%p]\n", "", list->data);
    for (size_t i = 0; i < list->capacity; i++)
    {
        PRINT_LOG("%4s [%zu] = ", "", i);
        RETURN_ERROR(_printListElement(outTextFile, &list->data[i]));
        fputc('\n', outTextFile);
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

    return Error();
}

Error _dumpListGraph(const LinkedList* list, const char* outGraphPath)
{
    SoftAssert(list,         ERROR_NULLPTR);
    SoftAssert(outGraphPath, ERROR_NULLPTR);

    FILE* outGraphFile = fopen(outGraphPath, "w");
    if (!outGraphFile)
        return CREATE_ERROR(ERROR_BAD_FILE);

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

        fprintf(outGraphFile, "value\\n");
        RETURN_ERROR(_printListElement(outGraphFile, &list->data[i]));
        fputc('|', outGraphFile);

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

    return Error();
}
#undef FONT_SIZE
#undef FONT_NAME
#undef BACK_GROUND_COLOR
#undef NODE_COLOR
#undef NODE_FRAME_COLOR
#undef ROOT_COLOR
#undef FREE_HEAD_COLOR

Error LinkedList::StartLogging(const char* logFolder) noexcept
{
    SoftAssert(logFolder, ERROR_BAD_FILE);
    LOG_FOLDER = logFolder;

    char HTML_FILE_PATH[MAX_PATH_LENGTH];
    sprintf(HTML_FILE_PATH, "%s/log.html", logFolder);

    HTML_FILE = fopen(HTML_FILE_PATH, "w");
    SoftAssert(HTML_FILE, ERROR_BAD_FILE);

    fprintf(HTML_FILE, 
        "<style>\n"
        ".content {\n"
        "max-width: 500px;\n"
        "margin: auto;\n"
        "}\n"
        "</style>,\n"
        "<body>\n"
        "<div class=\"content\">");

    return Error();
}

Error LinkedList::EndLogging() noexcept
{
    if (HTML_FILE)
    {
        fprintf(HTML_FILE, "</div>\n</body>\n");
        fclose(HTML_FILE);
    }
    HTML_FILE = NULL;

    return Error();
}

ListElemIndexResult LinkedList::Find(const ListElement_t& value) const noexcept
{
    ERR_DUMP_RET_RES(this, SIZET_POISON);

    size_t curEl = *this->head;

    while (curEl && strcmp(this->data[curEl].name.buf, value.name.buf))
        curEl = this->next[curEl];

    return { curEl, curEl ? Error() : CREATE_ERROR(ERROR_NOT_FOUND) };
}
