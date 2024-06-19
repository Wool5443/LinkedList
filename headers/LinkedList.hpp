#pragma once

#include <stddef.h>
#include <math.h>
#include "Utils.hpp"

#include "ListSettings.hpp"

static const size_t FREE_ELEM = (size_t)-1;

struct ListElemResult
{
    ListElement_t value;
    Error error;
};

struct ListElemIndexResult
{
    size_t value;
    Error error;
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
     * @brief Initializes an empty list with default capacity
     * 
     * @return error which can happen during allocation
     */
    Error Init() noexcept;
    /**
     * @brief Destroys a list setting all fields to poisons
     * 
     * @return error if list is not valid
     */
    Error Destructor() noexcept;
    /**
     * @brief Checks the list for bad fields
     * 
     * @return list error
     */
    Error Verify() const noexcept;

    /**
     * @brief Inserts a value after a list element. May realloc. O(1)
     * 
     * @param value - what to insert
     * @param index - raw index in list. Not order
     * @return error. 
     */
    Error InsertAfter(ListElement_t value, size_t index) noexcept;
    /**
     * @brief Inserts a value before a list element. May realloc. O(1)
     * 
     * @param value - what to insert
     * @param index - raw index in list. Not in logical order
     * @return error. 
     */
    Error InsertBefore(ListElement_t value, size_t index) noexcept;

    /**
     * @brief Pushes a value in the end. May realloc. O(1)
     * 
     * @param value - what to push
     * @return error
     */
    Error PushBack(ListElement_t value) noexcept;
    /**
     * @brief Pushes a value in the start. May realloc. O(1)
     * 
     * @param value - what to push
     * @return error
     */
    Error PushFront(ListElement_t value) noexcept;

    /**
     * @brief Pops element at index and returns its value
     * 
     * @param index - which element to pop. Not order
     * @return ListElemResult containing value and error
     */
    ListElemResult Pop(size_t index) noexcept;
    /**
     * @brief Pops last element and returns its value
     * 
     * @return ListElemResult containing value and error
     */
    ListElemResult Pop() noexcept;

    /**
     * @brief Reallocs the list to its length and untangles it
     * 
     * @return error
     */
    Error ReallocDownAndUntangle() noexcept;

    /**
     * @brief Finds the element which is nth in order
     * 
     * @param [in] index 
     * @return ListElemIndexResult
     */
<<<<<<< HEAD
    ListElemIndexResult GetByIndex(size_t index);  

    /**
     * @brief finds the value in the list
     * 
     * @param [in] value
     * @return ListElemResult with a pointer to the value 
     */
    ListElemIndexResult Find(const ListElement_t& value);
=======
    ListElemIndexResult GetByIndex(size_t index) const noexcept;

    Error StartLogging(const char* logFolder) noexcept;
    Error EndLogging() noexcept;

    Error Dump() const noexcept;
    Error Dump(const Error& error) const noexcept;
>>>>>>> New-Error-System
};
