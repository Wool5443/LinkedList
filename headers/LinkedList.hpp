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

    LinkedListResult LinkedList::Init();

    ErrorCode LinkedList::Destructor();
    
    ErrorCode LinkedList::Verify();

    ErrorCode LinkedList::InsertAfter(ListElement_t value, size_t index);

    ErrorCode LinkedList::InsertBefore(ListElement_t value, size_t index);

    ListElemResult LinkedList::Pop(size_t index);
};

struct LinkedListResult
{
    LinkedList value;
    ErrorCode error;
};

struct ListElemResult
{
    ListElement_t value;
    ErrorCode error;

};

static const size_t DEFAULT_LIST_CAPACITY = 32;

static const size_t POISON_SIZE_T = 0xFFFFFFFFFFFFFFFF;

#endif
