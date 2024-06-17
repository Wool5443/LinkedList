#pragma once

#include <math.h>
#include "String.hpp"

enum SymbolType
{
    VARIABLE_SYMBOL,
    CONST_SYMBOL,
    FUNCTION_SYMBOL,
};

struct SymbolTableEntry
{
    String*    entry;
    SymbolType type;
};

constexpr size_t DEFAULT_LIST_CAPACITY = 8;
constexpr size_t LIST_GROW_FACTOR      = 2;

typedef double ListElement_t;
constexpr ListElement_t LIST_POISON = {};
#define LIST_EL_SPECIFIER "%lg"
