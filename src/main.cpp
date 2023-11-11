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

    size_t length = list.length;
    for (size_t i = 1; i < length; i++)
    {
        list.Pop();
        char txtFile[32] = "";
        char dotFile[32] = "";
        sprintf(txtFile, "texts/afterPop%zu.txt", i);
        sprintf(dotFile, "dots/afterPop%zu.dot", i);
        DumpList(&list, list.Verify(), txtFile, dotFile);
    }

    list.PushBack(10);
    list.PushBack(20);
    list.PushBack(30);

    list.InsertAfter(15, 1);
    list.InsertBefore(13, 4);

    DumpList(&list, list.Verify(), "texts/afterAllPop.txt", "dots/afterAllPop.dot");

    list.ReallocDownAndUntangle();

    DumpList(&list, list.Verify(), "texts/afterUntangle.txt", "dots/afterUntangle.dot");

    list.PushBack(1);
    list.PushBack(2);
    list.PushBack(3);
    list.PushBack(4);
    list.PushBack(5);

    DumpList(&list, list.Verify(), "texts/afterAll.txt", "dots/afterAll.dot");

    return list.Destructor();
}
