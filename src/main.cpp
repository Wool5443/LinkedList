#include <stdio.h>
#include "LinkedList.hpp"
#include "Utils.hpp"

int main()
{
    LinkedListResult lRes = LinkedListInit();

    MyAssertSoft(!lRes.error, lRes.error);

    LinkedList list = lRes.value;

    InsertAfter(&list, 10, 0);
    InsertAfter(&list, 20, 1);
    InsertAfter(&list, 30, 2);
    InsertAfter(&list, 40, 3);
    
    InsertBefore(&list, 25, 3);
    InsertAfter(&list, 100, list.tail);

    size_t i = list.head;
    size_t k = 0;
    while (i != 0)
    {
        printf("List[%zu] = %lg\n", k, list.data[i]);
        i = list.next[i];
        k++;
    }

    printf("List head = %zu:%lg\n", list.head, list.data[list.head]);
    printf("List tail = %zu:%lg\n", list.tail, list.data[list.tail]);

    return LinkedListDestructor(&list);
}