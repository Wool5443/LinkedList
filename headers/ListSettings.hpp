#pragma once

#include <math.h>

constexpr size_t DEFAULT_LIST_CAPACITY = 8;
constexpr size_t LIST_GROW_FACTOR      = 2;

typedef double ListElement_t;
static const double LIST_POISON = NAN;
#define LIST_EL_SPECIFIER "%lg"
