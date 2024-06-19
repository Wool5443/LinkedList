#include <stdio.h>
#include "LinkedList.hpp"
#include "Utils.hpp"

static const char* LOG_FOLDER = "../log";

int main()
{
    LinkedList list = {};
    list.Init();

    list.StartLogging(LOG_FOLDER);

    for (int i = 0; i < 4; i++)
        list.PushBack((double)i);

    list.InsertBefore(0.5, 2);
    list.InsertAfter(0.75, list.prev[2]);
    list.InsertAfter(1.5, 2);

    list.PushFront(-1); // { -1, 0, 0.5, 0.75, 1, 1.5, 2, 3 }

    ListElemResult firstEl = list.Pop(1);
    list.Dump().Print();

    list.ReallocDownAndUntangle();
    list.Dump().Print();


    return list.Destructor();
}
