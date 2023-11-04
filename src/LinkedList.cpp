#include <string.h>
#include "LinkedList.hpp"
#include "MinMax.hpp"

static const size_t FREE_ELEM = (size_t)-1;

ErrorCode LinkedList::Init()
{
    *this = {};

    ListElement_t* dataTemp = (ListElement_t*)calloc(DEFAULT_LIST_CAPACITY, sizeof(*dataTemp));
    MyAssertSoft(dataTemp, ERROR_NO_MEMORY);

    size_t* nextTemp = (size_t*)calloc(DEFAULT_LIST_CAPACITY, sizeof(*nextTemp));
    MyAssertSoft(nextTemp, ERROR_NO_MEMORY);

    size_t* prevTemp = (size_t*)calloc(DEFAULT_LIST_CAPACITY, sizeof(*prevTemp));
    MyAssertSoft(prevTemp, ERROR_NO_MEMORY);

    for (size_t i = 0; i < DEFAULT_LIST_CAPACITY; i++)
        dataTemp[i] = LIST_POISON;
    
    for (size_t i = 0; i < DEFAULT_LIST_CAPACITY - 1; i++)
        nextTemp[i] = i + 1;

    for (size_t i = 1; i < DEFAULT_LIST_CAPACITY; i++)
        prevTemp[i] = FREE_ELEM;

    this->data     = dataTemp;
    this->capacity = DEFAULT_LIST_CAPACITY;
    this->head     = 0;
    this->tail     = 0;
    this->next     = nextTemp;
    this->prev     = prevTemp;
    this->freeHead = 1;

    return EVERYTHING_FINE;
}

ErrorCode LinkedList::Destructor()
{
    free(this->data);
    free(this->next);
    free(this->prev);

    this->capacity = FREE_ELEM;
    this->head     = FREE_ELEM;
    this->tail     = FREE_ELEM;

    return EVERYTHING_FINE;
}

ErrorCode LinkedList::Verify()
{
    if (!this->data || !this->next || !this->prev)
        return ERROR_NO_MEMORY;

    if (this->capacity <= max(this->head, this->tail) || this->capacity <= this->freeHead)
        return ERROR_INDEX_OUT_OF_BOUNDS;

    return EVERYTHING_FINE;
}

ErrorCode LinkedList::InsertAfter(ListElement_t value, size_t index)
{
    MyAssertSoft(index < this->capacity, ERROR_INDEX_OUT_OF_BOUNDS);
    MyAssertSoft(this->freeHead != 0, ERROR_NO_MEMORY);
    MyAssertSoft(this->prev[index] != FREE_ELEM, ERROR_INDEX_OUT_OF_BOUNDS);

    RETURN_ERROR(this->Verify());

    size_t insertIndex = this->freeHead;
    this->freeHead     = this->next[this->freeHead];

    this->data[insertIndex] = value;

    this->prev[insertIndex] = index;
    this->next[insertIndex] = this->next[index];

    this->prev[this->next[insertIndex]] = insertIndex;
    this->next[index] = insertIndex;

    if (index == this->tail)
        this->tail = insertIndex;

    return EVERYTHING_FINE;
}

ErrorCode LinkedList::InsertBefore(ListElement_t value, size_t index)
{
    MyAssertSoft(0 < index && index < this->capacity, ERROR_INDEX_OUT_OF_BOUNDS);
    MyAssertSoft(this->freeHead != 0, ERROR_NO_MEMORY);
    MyAssertSoft(this->prev[index] != FREE_ELEM, ERROR_INDEX_OUT_OF_BOUNDS);

    RETURN_ERROR(this->Verify());

    size_t insertIndex = this->freeHead;
    this->freeHead     = this->next[this->freeHead];

    this->data[insertIndex] = value;

    this->next[insertIndex] = index;
    this->prev[insertIndex] = this->prev[index];

    this->next[this->prev[insertIndex]] = insertIndex;
    this->prev[index] = insertIndex;

    if (index == this->head)
        this->head = insertIndex;
    
    return EVERYTHING_FINE;
}

ErrorCode LinkedList::PushBack(ListElement_t value)
{
    MyAssertSoft(this->freeHead != 0, ERROR_NO_MEMORY);

    RETURN_ERROR(this->Verify());

    size_t insertIndex = this->freeHead;
    this->freeHead     = this->next[this->freeHead];

    this->data[insertIndex] = value;
    this->next[insertIndex] = 0;
    this->prev[insertIndex] = this->tail;
    this->next[this->tail]  = insertIndex;

    this->prev[0] = insertIndex;
    this->tail    = insertIndex;

    if (this->head == 0)
        this->head = insertIndex;

    return EVERYTHING_FINE;
}

ErrorCode LinkedList::PushFront(ListElement_t value)
{
    MyAssertSoft(this->freeHead != 0, ERROR_NO_MEMORY);

    RETURN_ERROR(this->Verify());

    size_t insertIndex = this->freeHead;
    this->freeHead     = this->next[this->freeHead];

    this->data[insertIndex] = value;
    this->next[insertIndex] = this->head;
    this->prev[insertIndex] = 0;
    this->prev[this->head] = insertIndex;

    this->next[0] = insertIndex;
    this->head    = insertIndex;

    if (this->tail == 0)
        this->tail = insertIndex;

    return EVERYTHING_FINE;
}

ListElemResult LinkedList::Pop(size_t index)
{
    MyAssertSoftResult(1 <= index && index < this->capacity, LIST_POISON, ERROR_INDEX_OUT_OF_BOUNDS);
    MyAssertSoftResult(this->prev[index] != FREE_ELEM, LIST_POISON, ERROR_INDEX_OUT_OF_BOUNDS);

    ListElemResult result = {this->data[index], EVERYTHING_FINE};
    this->data[index] = LIST_POISON;
    this->next[this->prev[index]] = this->next[index];
    this->prev[this->next[index]] = this->prev[index];

    if (index == this->tail)
    {
        this->tail = this->prev[index];
        this->prev[0] = this->tail;
    }
    if (index == this->head)
    {
        this->head = this->next[index];
        this->next[0] = this->head;
    }

    this->prev[index] = FREE_ELEM;
    this->next[index] = this->freeHead;
    this->freeHead = index;

    return result;
}
