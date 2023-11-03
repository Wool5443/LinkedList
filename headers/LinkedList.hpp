#ifndef LINKED_LIST_HPP
#define LINKED_LIST_HPP

#include <stddef.h>
#include <math.h>
#include "Utils.hpp"

typedef double ListElement_t;

static const ListElement_t LIST_POISON = NAN;

struct ListElemResult
{
    ListElement_t value;
    ErrorCode error;
};

struct LinkedList
{
    ListElement_t* data;
    size_t         capacity;
    size_t         head;
    size_t         tail;
    size_t*        next;
    size_t*        prev;
    size_t         freeHead;

    ErrorCode Init();
    ErrorCode Destructor();
    ErrorCode Verify();
    ErrorCode InsertAfter(ListElement_t value, size_t index);
    ErrorCode InsertBefore(ListElement_t value, size_t index);
    ErrorCode PushBack(ListElement_t value);
    ErrorCode PushFront(ListElement_t value);
    ListElemResult Pop(size_t index);
};

static const size_t DEFAULT_LIST_CAPACITY = 32;

static const size_t POISON_SIZE_T = 0xFFFFFFFFFFFFFFFF;

#endif
