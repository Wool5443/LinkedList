# LinkedList

This repo is an implementation of double linked list using arrays to store nodes. Since it does not allocate memory for each node separately it is much faster than classic implementations you can find in standard libraries. Also all indexes used here are raw meaning that you need to watch where you insert your elements. It is possible to get nth element by order using LinkedList::FindElement() but it is O(n).

## Installation

```bash
git clone https://github.com/Wool5443/LinkedList
make release
```
## Usage

In [ListSettings.ini](headers/ListSettings.ini) you can choose data type of the list, its default capacity, grow factor, default log paths.

```c++
LinkedList list = {};
list.Init();
```

We can destroy it.
```c++
list.Destructor();
```

We created a list, initialized its fields and allocated memory for it.

```c++
for (int i = 0; i < 4; i++)
    list.PushBack((double)i);
```
We added { 0, 1, 2, 3 } to our list. Now lets insert some values.

```c++
list.InsertBefore(0.5, 2);            // {0, 0.5, 1, 2, 3}
list.InsertAfter(0.75, list.prev[2]); // {0, 0.5, 0.75, 1, 2, 3}
list.InsertAfter(1.5, 2);             // {0, 0.5, 0.75, 1, 1.5, 2, 3}
```

We can also push elements to front.
```c++
list.PushFront(-1); // {-1, 0, 0.5, 0.75, 1, 1.5, 2, 3}
```
We can pop elements.
```c++
ListElemResult firstEl = list.Pop(1);
if (!firstEl.error)
    printf("firstEl = "LIST_EL_SPECIFIER "\n", firstEl.value); // 0
else
    printf("%s[%d]\n", ERROR_CODE_NAMES[firstEl.error], firstEl.error);
// list : {-1, 0.5, 0.75, 1, 1.5, 2, 3}
```

We can validate list.
```c++
ErrorCode listError = list.Verify();
```
We can also dump our list in text or graphics.
```c++
DumpList(&list, list.Verify(), "textDump.txt", "graphDumpt.dot");
```
[beforeUntangle.txt](examples/beforeUntangle.txt)
```txt
List[0x7ffcd5832b30] called from src/main.cpp(73) main()
List condition - EVERYTHING_FINE[0]
{
    length = 8
    capacity = 16
    head = 8
    tail = 4
    free head = 1
    list:
    *[1] = -1
    *[2] = 0.5
    *[3] = 0.75
    *[4] = 1
    *[5] = 1.5
    *[6] = 2
    *[7] = 3

    data[0x60c000000040]
    *[0] = nan
    *[1] = nan
    *[2] = 1
    *[3] = 2
    *[4] = 3
    *[5] = 0.5
    *[6] = 0.75
    *[7] = 1.5
    *[8] = -1
    *[9] = nan
    *[10] = nan
    *[11] = nan
    *[12] = nan
    *[13] = nan
    *[14] = nan
    *[15] = nan

    prev[0x60c000000100]
    *[0] = 4
     [1] = FREE
    *[2] = 6
    *[3] = 7
    *[4] = 3
    *[5] = 8
    *[6] = 5
    *[7] = 2
    *[8] = 0
     [9] = FREE
     [10] = FREE
     [11] = FREE
     [12] = FREE
     [13] = FREE
     [14] = FREE
     [15] = FREE

    next[0x60c0000001c0]
    *[0] = 8
    *[1] = 9
    *[2] = 7
    *[3] = 4
    *[4] = 0
    *[5] = 6
    *[6] = 2
    *[7] = 3
    *[8] = 5
    *[9] = 10
    *[10] = 11
    *[11] = 12
    *[12] = 13
    *[13] = 14
    *[14] = 15
    *[15] = 0
}
```
Here not nonexistent nodes are nan in data, FREE in prev. next is a single linked list of free nodes used for adding new elements.

You can see elements in data are not in order. We can fix this by calling ReallocDownWithUntangle().

[afterUntangle.txt](examples/afterUntangle.txt)
```txt
List[0x7ffcd5832b30] called from src/main.cpp(77) main()
List condition - EVERYTHING_FINE[0]
{
    length = 8
    capacity = 8
    head = 1
    tail = 7
    free head = 0
    list:
    *[1] = -1
    *[2] = 0.5
    *[3] = 0.75
    *[4] = 1
    *[5] = 1.5
    *[6] = 2
    *[7] = 3

    data[0x606000000140]
    *[0] = nan
    *[1] = -1
    *[2] = 0.5
    *[3] = 0.75
    *[4] = 1
    *[5] = 1.5
    *[6] = 2
    *[7] = 3

    prev[0x6060000001a0]
    *[0] = 7
    *[1] = 0
    *[2] = 1
    *[3] = 2
    *[4] = 3
    *[5] = 4
    *[6] = 5
    *[7] = 6

    next[0x606000000200]
    *[0] = 1
    *[1] = 2
    *[2] = 3
    *[3] = 4
    *[4] = 5
    *[5] = 6
    *[6] = 7
    *[7] = 0
}
```

Graph dumps are in [images](images/). Example ones are in [examples](examples/)

You can use dot to draw your own graph dumps.
```bash
make draw
```
It draws all dot files in [dots](dots/).