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

    DumpList(&list, "beforePOP.txt", "beforePOP.dot");

    for (size_t i = 0; i < DEFAULT_LIST_CAPACITY; i++)
    {
        list.Pop(i);
        char outTextPath[16] = "";
        sprintf(outTextPath, "pop%zu.txt", i);

        char outGraphPath[16] = "";
        sprintf(outGraphPath, "pop%zu.dot", i);

        DumpList(&list, outTextPath, outGraphPath);
    }

    return list.Destructor();
}