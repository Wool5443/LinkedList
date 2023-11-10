#include <stdio.h>
#include "LinkedList.hpp"
#include "Utils.hpp"
#include "PrettyDumpList.hpp"

int main()
{
    LinkedList list = {};
    ErrorCode listError = list.Init();
    MyAssertSoft(!listError, listError);

    size_t n = 3;

    for (size_t i = 1; i < n; i++)
        list.PushBack((double)i * 10);

    DumpList(&list, list.Verify(), "texts/before.txt", "dots/before.dot");

    list.InsertAfter(15, 1);
    DumpList(&list, list.Verify(), "texts/after1.txt", "dots/after1.dot");
    list.InsertAfter(25, 2);
    DumpList(&list, list.Verify(), "texts/after2.txt", "dots/after2.dot");
    list.InsertAfter(30, 4);
    DumpList(&list, list.Verify(), "texts/after3.txt", "dots/after3.dot");

    // size_t length = list.length;
    // for (size_t i = 1; i < length; i++)
    //     list.Pop(i);


    list.ReallocDownAndUntangle();

    DumpList(&list, list.Verify(), "texts/afterUntangle.txt", "dots/afterUntangle.dot");

    list.PushBack(1);
    list.PushBack(2);
    list.PushBack(3);
    list.PushBack(4);

    DumpList(&list, list.Verify(), "texts/afterAll.txt", "dots/afterAll.dot");

    return list.Destructor();
}
