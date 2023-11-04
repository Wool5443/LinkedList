#include <stdio.h>
#include "LinkedList.hpp"
#include "Utils.hpp"
#include "PrettyDumpList.hpp"

int main()
{
    LinkedList list;
    ErrorCode listError = list.Init();
    MyAssertSoft(!listError, listError);

    for (size_t i = 1; i < 6; i++)
        list.PushBack((double)i * 10);
    list.PushFront(11);
    list.PushFront(12);
    list.PushFront(13);

    list.InsertAfter(35, 3);
    list.InsertBefore(25, 3);

    DumpList(&list, "beforePOP.txt", "beforePOP.dot");

    list.Pop(4);
    list.Pop(2);

    DumpList(&list, "afterPOP.txt", "afterPOP.dot");

    return list.Destructor();
}