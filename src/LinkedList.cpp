#include <string.h>
#include <assert.h>
#include "PrettyDumpList.hpp"
#include "LinkedList.hpp"
#include "MinMax.hpp"

#ifdef NDEBUG
#define ERR_DUMP_RET(...)
#define ERR_DUMP_RET_RES(...)
#else
#define ERR_DUMP_RET(listPtr, error)                                    \
{                                                                       \
    ErrorCode _error = error;                                           \
    if (_error)                                                         \
    {                                                                   \
        DumpList(listPtr, _error);                                      \
        return _error;                                                  \
    }                                                                   \
}

#define ERR_DUMP_RET_RES(listPtr, error, value)                         \
{                                                                       \
    ErrorCode _error = error;                                           \
    if (_error)                                                         \
    {                                                                   \
        DumpList(listPtr, _error);                                      \
        return { value, _error };                                       \
    }                                                                   \
}
#endif

ErrorCode _listReallocUp(LinkedList* list);

ErrorCode LinkedList::Init(const char* logFolder)
{
    *this = {};

    ListElement_t* dataTemp = (ListElement_t*)calloc(DEFAULT_LIST_CAPACITY, sizeof(*dataTemp));
    if (!dataTemp) return ERROR_NO_MEMORY;

    size_t* nextTemp = (size_t*)calloc(DEFAULT_LIST_CAPACITY, sizeof(*nextTemp));
    if (!nextTemp) return ERROR_NO_MEMORY;

    size_t* prevTemp = (size_t*)calloc(DEFAULT_LIST_CAPACITY, sizeof(*prevTemp));
    if (!prevTemp) return ERROR_NO_MEMORY;

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

    this->logFolder = logFolder;

    return EVERYTHING_FINE;
}

ErrorCode LinkedList::Destructor()
{
    ERR_DUMP_RET(this, this->Verify());

    free(this->data);
    free(this->next);
    free(this->prev);

    this->capacity = FREE_ELEM;
    this->head     = NULL;
    this->tail     = NULL;

    return EVERYTHING_FINE;
}

ErrorCode LinkedList::Verify()
{
    if (!this->data || !this->next || !this->prev)
        return ERROR_NO_MEMORY;
    
    if (*this->head == FREE_ELEM || *this->tail == FREE_ELEM || this->freeHead == FREE_ELEM)
        return ERROR_INDEX_OUT_OF_BOUNDS;

    if (this->capacity <= *this->head || this->capacity <= *this->tail ||
                                         this->capacity < this->length)
        return ERROR_INDEX_OUT_OF_BOUNDS;

    return EVERYTHING_FINE;
}

ErrorCode LinkedList::InsertAfter(ListElement_t value, size_t index)
{
    if (index >= this->capacity) return ERROR_INDEX_OUT_OF_BOUNDS;
    if (this->prev[index] == FREE_ELEM) return ERROR_INDEX_OUT_OF_BOUNDS;

    ERR_DUMP_RET(this, this->Verify());

    RETURN_ERROR(_listReallocUp(this));

    size_t insertIndex = this->freeHead;
    this->freeHead     = this->next[this->freeHead];
    this->length++;

    this->data[insertIndex] = value;

    this->prev[insertIndex] = index;
    this->next[insertIndex] = this->next[index];

    this->prev[this->next[index]] = insertIndex;
    this->next[index]             = insertIndex;

    return EVERYTHING_FINE;
}

ErrorCode LinkedList::InsertBefore(ListElement_t value, size_t index)
{
    return this->InsertAfter(value, this->prev[index]);
}

ErrorCode LinkedList::PushBack(ListElement_t value)
{
    return this->InsertAfter(value, *this->tail);
}

ErrorCode LinkedList::PushFront(ListElement_t value)
{
    return this->InsertBefore(value, *this->head);
}

ListElemResult LinkedList::Pop(size_t index)
{
    if (index < 1 || index >= this->capacity) return { LIST_POISON, ERROR_INDEX_OUT_OF_BOUNDS };
    if (this->prev[index] == FREE_ELEM) return { LIST_POISON, ERROR_INDEX_OUT_OF_BOUNDS };

    ERR_DUMP_RET_RES(this, this->Verify(), LIST_POISON);

    ListElement_t value = this->data[index];
    this->data[index]   = LIST_POISON;
    this->length--;

    this->next[this->prev[index]] = this->next[index];
    this->prev[this->next[index]] = this->prev[index];

    this->prev[index] = FREE_ELEM;
    this->next[index] = this->freeHead;
    this->freeHead    = index;

    return { value, EVERYTHING_FINE };
}

ListElemResult LinkedList::Pop()
{
    return this->Pop(*this->tail);
}

ErrorCode _listReallocUp(LinkedList* list)
{
    MyAssertSoft(list, ERROR_NULLPTR);

    if (list->freeHead)
        return EVERYTHING_FINE;

    size_t newCapacity = list->capacity * LIST_GROW_FACTOR;

    ListElement_t* newData = (ListElement_t*)realloc(list->data, newCapacity * sizeof(*newData));
    if (!newData) return { ERROR_NO_MEMORY };

    size_t* newPrev = (size_t*)realloc(list->prev, newCapacity * sizeof(*newPrev));
    if (!newPrev) return { ERROR_NO_MEMORY };

    size_t* newNext = (size_t*)realloc(list->next, newCapacity * sizeof(*newNext));
    if (!newNext) return { ERROR_NO_MEMORY };

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

    return EVERYTHING_FINE;
}

ErrorCode LinkedList::ReallocDownAndUntangle()
{
    size_t newCapacity = max(DEFAULT_LIST_CAPACITY, this->length);

    ListElement_t* newData = (ListElement_t*)calloc(newCapacity, sizeof(*newData));
    if (!newData) return { ERROR_NO_MEMORY };

    size_t* newPrev = (size_t*)calloc(newCapacity, sizeof(*newPrev));
    if (!newPrev) return { ERROR_NO_MEMORY };

    size_t* newNext = (size_t*)calloc(newCapacity, sizeof(*newNext));
    if (!newNext) return { ERROR_NO_MEMORY };

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

    return EVERYTHING_FINE;
}

ListElemIndexResult LinkedList::GetByIndex(size_t index)
{
    if (index < 1 || index >= this->capacity) return { SIZET_POISON, ERROR_INDEX_OUT_OF_BOUNDS };
    if (this->prev[index] == FREE_ELEM) return { SIZET_POISON, ERROR_INDEX_OUT_OF_BOUNDS };

    ERR_DUMP_RET_RES(this, this->Verify(), 0);
    
    size_t curEl = *this->head;
    size_t i = 1;

    while (i < index && curEl)
    {
        curEl = this->next[curEl];
        i++;
    }

    return { curEl, curEl ? EVERYTHING_FINE : ERROR_NOT_FOUND };
}

ListElemIndexResult LinkedList::Find(const ListElement_t& value)
{
    ERR_DUMP_RET_RES(this, this->Verify(), 0);

    size_t curEl = *this->head;

    while (curEl && strcmp(this->data->name.buf, value.name.buf))
        curEl = this->next[curEl];

    return { curEl, curEl ? EVERYTHING_FINE : ERROR_NOT_FOUND };
}
