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

    DumpList(&list, "texts/beforePOP.txt", "dots/beforePOP.dot");

    for (size_t i = 1; i < n; i++)
    {
        list.Pop();
        char outTextPath[32] = "";
        sprintf(outTextPath, "texts/pop%zu.txt", i);

        char outGraphPath[32] = "";
        sprintf(outGraphPath, "dots/pop%zu.dot", i);

        DumpList(&list, outTextPath, outGraphPath);
    }

    return list.Destructor();
}