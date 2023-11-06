#include <stdio.h>
#include "LinkedList.hpp"
#include "Utils.hpp"
#include "PrettyDumpList.hpp"

int main()
{
    LinkedList list;
    ErrorCode listError = list.Init();
    MyAssertSoft(!listError, listError);

    size_t n = 16;

    for (size_t i = 1; i < n; i++)
        list.PushBack((double)i * 10);

    DumpList(&list, "texts/before.txt", "dots/before.dot");

    list.InsertAfter(15, 1);
    list.InsertAfter(35, 3);
    list.InsertAfter(65, 6);
    list.InsertAfter(75, 7);

    list.InsertBefore(13, 16);

    size_t length = list.length;
    for (size_t i = 1; i < length; i++)
        list.Pop(i);

    DumpList(&list, "texts/after.txt", "dots/after.dot");

    list.ReallocDown();

    DumpList(&list, "texts/after2.txt", "dots/after2.dot");

    list.PushBack(1);
    list.PushBack(2);
    list.PushBack(3);

    DumpList(&list, "texts/after3.txt", "dots/after3.dot");

    return list.Destructor();
}
