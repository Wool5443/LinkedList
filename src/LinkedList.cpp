#include <string.h>
#include "Utils.hpp"
#include "LinkedList.hpp"
#include "MinMax.hpp"

LinkedListResult LinkedList::Init()
{
    LinkedList list = {};

    ListElement_t* data = (ListElement_t*)calloc(DEFAULT_LIST_CAPACITY, sizeof(*data));
    MyAssertSoftResult(data, {}, ERROR_NO_MEMORY);

    size_t* next = (size_t*)calloc(DEFAULT_LIST_CAPACITY, sizeof(*next));
    MyAssertSoftResult(next, {}, ERROR_NO_MEMORY);

    size_t* prev = (size_t*)calloc(DEFAULT_LIST_CAPACITY, sizeof(*prev));
    MyAssertSoftResult(prev, {}, ERROR_NO_MEMORY);

    for (size_t i = 0; i < DEFAULT_LIST_CAPACITY; i++)
        data[i] = LIST_POISON;
    
    for (size_t i = 0; i < DEFAULT_LIST_CAPACITY - 1; i++)
        next[i] = i + 1;

    for (size_t i = 0; i < DEFAULT_LIST_CAPACITY; i++)
        prev[i] = -1;

    list.data     = data;
    list.capacity = DEFAULT_LIST_CAPACITY;
    list.head     = 0;
    list.tail     = 0;
    list.next     = next;
    list.prev     = prev;
    list.freeHead = 1;

    return {list, EVERYTHING_FINE};
}

ErrorCode LinkedList::Destructor()
{
    free(this->data);
    free(this->next);
    free(this->prev);

    this->capacity = POISON_SIZE_T;
    this->head     = POISON_SIZE_T;
    this->tail     = POISON_SIZE_T;

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

    RETURN_ERROR(this->Verify());

    size_t insertIndex = this->freeHead;
    this->freeHead     = this->next[this->freeHead];

    this->data[insertIndex] = value;

    this->prev[insertIndex] = index;
    this->next[insertIndex] = this->next[index];

    this->prev[this->next[insertIndex]] = insertIndex;
    this->next[index] = insertIndex;

    if (index == this->tail)
    {
        this->tail = insertIndex;
        this->next[insertIndex] = 0;
        this->prev[0] = insertIndex;
    }

    if (this->head == 0)
        this->head = insertIndex;

    return EVERYTHING_FINE;
}

ErrorCode LinkedList::InsertBefore(ListElement_t value, size_t index)
{
    MyAssertSoft(0 < index && index < this->capacity, ERROR_INDEX_OUT_OF_BOUNDS);
    MyAssertSoft(this->freeHead != 0, ERROR_NO_MEMORY);

    RETURN_ERROR(this->Verify());

    size_t insertIndex = this->freeHead;
    this->freeHead     = this->next[this->freeHead];

    this->data[insertIndex] = value;

    this->next[insertIndex] = index;
    this->prev[insertIndex] = this->prev[index];

    this->next[this->prev[insertIndex]] = insertIndex;
    this->prev[index] = insertIndex;

    if (index == this->head)
    {
        this->head = insertIndex;
        this->prev[insertIndex] = 0;
        this->next[0] = insertIndex;
    }
    
    return EVERYTHING_FINE;
}
