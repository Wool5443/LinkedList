#ifndef LINKED_LIST_HPP
#define LINKED_LIST_HPP

#include <stddef.h>
#include <math.h>
#include "Utils.hpp"

#include "ListSettings.ini"

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

#endif
