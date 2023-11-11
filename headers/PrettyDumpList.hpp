#ifndef PRETTY_DUMP_LIST_HPP
#define PRETTY_DUMP_LIST_HPP

#include "Utils.hpp"
#include "LinkedList.hpp"

static size_t DUMP_ITER = 0;
static FILE* HTML_FILE = NULL;

ErrorCode _dumpList(LinkedList* list, ErrorCode error, SourceCodePosition* caller);
#define DumpList(listPtr, error)                                            \
({                                                                          \
    SourceCodePosition _caller = { __FILE__, __LINE__, __func__ };          \
    ErrorCode _dumpError = error;                                           \
    _dumpList(listPtr, _dumpError, &_caller);                               \
})

ErrorCode _dumpListText(LinkedList* list, ErrorCode error, SourceCodePosition* caller, const char* outTextPath);
#define DumpListText(listPtr, error, outTextPath)                           \
({                                                                          \
    SourceCodePosition _caller = { __FILE__, __LINE__, __func__ };          \
    ErrorCode _dumpError = error;                                           \
    _dumpListText(listPtr, _dumpError, &_caller, outTextPath);              \
})

ErrorCode DumpListGraph(LinkedList* list, const char* outGraphPath);

ErrorCode StartHtmlLogging();

ErrorCode EndHtmlLogging();

#endif
