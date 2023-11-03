#include <string.h>
#include "Utils.hpp"
#include "LinkedList.hpp"
#include "MinMax.hpp"

LinkedListResult LinkedListInit()
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

ErrorCode LinkedListDestructor(LinkedList* list)
{
    MyAssertSoft(list, ERROR_NULLPTR);

    free(list->data);
    free(list->next);
    free(list->prev);

    list->capacity = POISON_SIZE_T;
    list->head     = POISON_SIZE_T;
    list->tail     = POISON_SIZE_T;

    return EVERYTHING_FINE;
}

ErrorCode LinkedListVerify(LinkedList* list)
{
    MyAssertSoft(list, ERROR_NULLPTR);
    
    if (!list->data || !list->next || !list->prev)
        return ERROR_NO_MEMORY;

    if (list->capacity <= max(list->head, list->tail) || list->capacity <= list->freeHead)
        return ERROR_INDEX_OUT_OF_BOUNDS;

    return EVERYTHING_FINE;
}

ErrorCode InsertAfter(LinkedList* list, ListElement_t value, size_t index)
{
    MyAssertSoft(list, ERROR_NULLPTR);
    MyAssertSoft(index < list->capacity, ERROR_INDEX_OUT_OF_BOUNDS);
    MyAssertSoft(list->freeHead != 0, ERROR_NO_MEMORY);

    RETURN_ERROR(LinkedListVerify(list));

    size_t insertIndex = list->freeHead;
    list->freeHead     = list->next[list->freeHead];

    list->data[insertIndex] = value;

    list->prev[insertIndex] = index;
    list->next[insertIndex] = list->next[index];

    list->prev[list->next[insertIndex]] = insertIndex;
    list->next[index] = insertIndex;

    if (index == list->tail)
    {
        list->tail = insertIndex;
        list->next[insertIndex] = 0;
        list->prev[0] = insertIndex;
    }

    if (list->head == 0)
        list->head = insertIndex;

    return EVERYTHING_FINE;
}

ErrorCode InsertBefore(LinkedList* list, ListElement_t value, size_t index)
{
    MyAssertSoft(list, ERROR_NULLPTR);
    MyAssertSoft(0 < index && index < list->capacity, ERROR_INDEX_OUT_OF_BOUNDS);
    MyAssertSoft(list->freeHead != 0, ERROR_NO_MEMORY);

    RETURN_ERROR(LinkedListVerify(list));

    size_t insertIndex = list->freeHead;
    list->freeHead     = list->next[list->freeHead];

    list->data[insertIndex] = value;

    list->next[insertIndex] = index;
    list->prev[insertIndex] = list->prev[index];

    list->next[list->prev[insertIndex]] = insertIndex;
    list->prev[index] = insertIndex;

    if (index == list->head)
    {
        list->head = insertIndex;
        list->prev[insertIndex] = 0;
        list->next[0] = insertIndex;
    }
    
    return EVERYTHING_FINE;
}
