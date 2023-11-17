#include <stdio.h>
#include "LinkedList.hpp"
#include "Utils.hpp"
#include "PrettyDumpList.hpp"

int main()
{
    StartHtmlLogging();
    
    LinkedList list = {};
    ErrorCode listError = list.Init();
    MyAssertSoft(!listError, listError);

    size_t n = 3;

    for (size_t i = 1; i < n; i++)
        list.PushBack((double)i * 10);

    DumpList(&list, list.Verify());

    list.InsertAfter(15, 1);
    DumpList(&list, list.Verify());
    list.InsertAfter(25, 2);
    DumpList(&list, list.Verify());
    list.InsertAfter(30, 4);
    DumpList(&list, list.Verify());

    size_t length = list.length;
    for (size_t i = 1; i < length; i++)
    {
        list.Pop();
        char txtFile[64] = "";
        char dotFile[64] = "";
        sprintf(txtFile, "texts/afterPop%zu.txt", i);
        sprintf(dotFile, "dots/afterPop%zu.dot", i);
        DumpList(&list, list.Verify());
    }

    list.PushBack(10);
    list.PushBack(20);
    list.PushBack(30);

    list.InsertAfter(15, 1);
    list.InsertBefore(13, 4);
    list.InsertBefore(5, 1);

    DumpList(&list, list.Verify());

    list.ReallocDownAndUntangle();

    DumpList(&list, list.Verify());

    list.PushBack(1);
    list.PushBack(2);
    list.PushBack(3);
    list.PushBack(4);
    list.PushBack(5);

    DumpList(&list, list.Verify());
    
    EndHtmlLogging();
    return list.Destructor();
}
