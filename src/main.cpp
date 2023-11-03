#include <stdio.h>
#include "LinkedList.hpp"
#include "Utils.hpp"

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

    {
        size_t i = list.head;
        size_t k = 1;
        while (i != 0)
        {
            printf("List[%zu] = %lg\n", k, list.data[i]);
            i = list.next[i];
            k++;
        }
    }
    printf("List head = %zu:%lg\n", list.head, list.data[list.head]);
    printf("List tail = %zu:%lg\n", list.tail, list.data[list.tail]);

    printf("\n\n");

    for (size_t i = 0; i < DEFAULT_LIST_CAPACITY; i++)
        printf("Data[%zu] = %lg\n", i, list.data[i]);


    return list.Destructor();
}