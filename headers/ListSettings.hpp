#pragma once

#include <math.h>
#include "String.hpp"

typedef void* (*function_t)(void* arg);

enum SymbolType
{
    ANY_SYMBOL = 0,
    VARIABLE_SYMBOL,
    CONST_SYMBOL,
    FUNCTION_SYMBOL,
};

struct SymbolTableEntry
{
    String name;
    SymbolType   type;

    union
    {
        double     value;
        function_t function;
    };

    Error Create(String* name, SymbolType type);
    void  Destructor();
};

constexpr size_t DEFAULT_LIST_CAPACITY = 8;
constexpr size_t LIST_GROW_FACTOR      = 2;

typedef SymbolTableEntry ListElement_t;
constexpr ListElement_t LIST_POISON = {};
#define LIST_EL_SPECIFIER ""
