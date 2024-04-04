#ifndef LINKED_LIST_HPP
#define LINKED_LIST_HPP

#include <stddef.h>
#include <math.h>
#include "Utils.hpp"

#include "ListSettings.ini"

static const size_t FREE_ELEM = (size_t)-1;

struct ListElemResult
{
    ListElement_t value;
    ErrorCode error;
};

struct ListElemIndexResult
{
    size_t value;
    ErrorCode error;
};

struct LinkedList
{
    ListElement_t* data;
    size_t         length;
    size_t         capacity;
    size_t*        head;
    size_t*        tail;

    size_t*        next;
    size_t*        prev;

    size_t         freeHead;

    /**
     * @brief Initializes an empty list with default capacity.
     * 
     * @return error which can happen during allocation.
     */
    ErrorCode Init();
    /**
     * @brief Destroys a list setting all fields to poisons.
     * 
     * @return error if list is not valid.
     */
    ErrorCode Destructor();
    /**
     * @brief Checks the list for bad fields.
     * 
     * @return list error.
     */
    ErrorCode Verify();

    /**
     * @brief Inserts a value after a list element. May realloc. O(1)
     * 
     * @param value - what to insert.
     * @param index - raw index in list. Not order.
     * @return error. 
     */
    ErrorCode InsertAfter(ListElement_t value, size_t index);
    /**
     * @brief Inserts a value before a list element. May realloc. O(1)
     * 
     * @param value - what to insert.
     * @param index - raw index in list. Not in logical order.
     * @return error. 
     */
    ErrorCode InsertBefore(ListElement_t value, size_t index);

    /**
     * @brief Pushes a value in the end. May realloc. O(1)
     * 
     * @param value - what to push.
     * @return error. 
     */
    ErrorCode PushBack(ListElement_t value);
    /**
     * @brief Pushes a value in the start. May realloc. O(1)
     * 
     * @param value - what to push.
     * @return error. 
     */
    ErrorCode PushFront(ListElement_t value);

    /**
     * @brief Pops element at index and returns its value.
     * 
     * @param index - which element to pop. Not order.
     * @return ListElemResult containing value and error.
     */
    ListElemResult Pop(size_t index);
    /**
     * @brief Pops last element and returns its value.
     * 
     * @return ListElemResult containing value and error.
     */
    ListElemResult Pop();

    /**
     * @brief Reallocs the list to its length and untangles it.
     * 
     * @return error.
     */
    ErrorCode ReallocDownAndUntangle();

    /**
     * @brief Finds the element which is nth in order.
     * 
     * @param [in] index 
     * @return ListElemIndexResult 
     */
    ListElemIndexResult FindElement(size_t index);  
};

#endif
