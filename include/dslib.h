#include <stdlib.h>


/* Typedefs */

typedef enum {INT, CHAR, STRING, DOUBLE, POINTER} dataType;

typedef struct Node
{
    /* data */
    void* val;
    struct Node* next;
    struct Node* prev;
    dataType type;
    int size;
    //*Node_cleanup;
} Node, * Nodeptr;

typedef struct List
{
    /* data */
    Nodeptr head;
    Nodeptr tail;
    int len;
} List, * Listptr;

typedef struct Stack
{
    Listptr Stack_Backend;

} Stack, *Stackptr;

typedef struct Queue
{
    /* data */
    Listptr queueBackend;
} Queue, * Queueptr;


/* Function Declarations */

// Node Functions

Nodeptr Node_new(void* val, dataType type);
void Node_init(Nodeptr self, void* val, dataType type);
void* Node_getValue(Nodeptr self);
void Node_setValue(Nodeptr self, void* val);
void Node_reset(Nodeptr self);
void Node_destroy(Nodeptr self);
void Node_printVal(Nodeptr self);
void Node_printNode(Nodeptr self);

// List Functions

Listptr List_new();
void List_init(Listptr l);
void List_addValue(Listptr self, void* val, dataType type);
void List_printList(Listptr self);
void List_printNodes(Listptr self);
Nodeptr List_walkToIndex(Listptr self, int index);
Nodeptr List_findNodebyValue(Listptr self, void* val);
void List_insertBefore(Listptr self, int index, void* val, dataType type);
void List_insertAfter(Listptr self, int index, void* val, dataType type);
void List_removeAt(Listptr self, int index);
Nodeptr List_unlinkNodebyValue(Listptr self, void* val);
void List_valueSort(Listptr self);
void List_addressSort(Listptr self);
void List_clear(Listptr self);
void List_destroy(Listptr self);

//  Stack Functions

Stackptr Stack_new();
void Stack_init(Stackptr s);
void Stack_Push(Stackptr self, void* val, dataType type);
void* Stack_Pop(Stackptr self);
void* Stack_findNodebyValue(Stackptr self, void* val);
void Stack_Print(Stackptr self);

// Queue Functions

Queueptr Queue_new();
void Queue_init(Queueptr q);
void Queue_enqueue(Queueptr self, void* val, dataType type);
void* Queue_dequeue(Queueptr self);
void Queue_clear(Queueptr self);
void Queue_destroy(Queueptr self);
void Queue_print(Queueptr self);