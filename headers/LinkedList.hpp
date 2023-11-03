#ifndef LINKED_LIST_HPP
#define LINKED_LIST_HPP

#include <stddef.h>
#include <math.h>
#include "Utils.hpp"

typedef double ListElement_t;

static const ListElement_t LIST_POISON = NAN;

struct LinkedList
{
    ListElement_t* data;
    size_t         capacity;
    size_t         head;
    size_t         tail;
    size_t*        next;
    size_t*        prev;
    size_t         freeHead;
};

struct LinkedListResult
{
    LinkedList value;
    ErrorCode error;
};

static const size_t DEFAULT_LIST_CAPACITY = 32;

static const size_t POISON_SIZE_T = 0xFFFFFFFFFFFFFFFF;

LinkedListResult LinkedListInit();

ErrorCode LinkedListDestructor(LinkedList* list);

ErrorCode LinkedListVerify(LinkedList* list);

/**
 * @brief Insert value in list after index.
*/
ErrorCode InsertAfter(LinkedList* list, ListElement_t value, size_t index);

ErrorCode InsertBefore(LinkedList* list, ListElement_t value, size_t index);

#endif
