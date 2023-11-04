#ifndef PRETTY_DUMP_LIST_HPP
#define PRETTY_DUMP_LIST_HPP

#include "Utils.hpp"
#include "LinkedList.hpp"

ErrorCode DumpList(LinkedList* list, const char* outTextPath, const char* outGraphPath);

ErrorCode DumpListText(LinkedList* list, const char* outTextPath);

ErrorCode DumpListGraph(LinkedList* list, const char* outGraphPath);

#endif
