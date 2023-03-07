#include <stdio.h>
#include <stdlib.h>
#include "dslib.h"




/*************************
 *     NODE FUNCTIONS    *
**************************/
/* Constructor Functions */

// Create a new node object on the heap and return a Nodeptr to that Node
Nodeptr Node_new(void* val, dataType type)
{
    Nodeptr returnedPointer = (Nodeptr)malloc(sizeof(Node));
    Node_init(returnedPointer, val, type);
    return returnedPointer;
}

// Initialize the values within the node object
void Node_init(Nodeptr self, void* val, dataType type)
{
    self->val = val;
    self->type = type;
    self->size = sizeof(val);
}


/* Getters & Setters */

// Returns the data stored in self->val
void* Node_getValue(Nodeptr self)
{
    void* output = NULL;
    switch (self->type)
    {
        case INT:
        {
            output = *(int *)self->val;
            break;
        }
        case STRING:
        {
            output = (char*)self->val;
            break;
        }
        case CHAR:
        {
            output = *(char *)self->val;
            break;
        }
        case DOUBLE:
        {
            //printf("%.2f", *(double*)self->val);
            output = self->val;
            break;
        }
        case POINTER:
        {
            output = self->val;
        }
        default:
        {
            break;
        }
    }
    return output;
}

// Changes the data stored in self->val and also updates the self->size value
void Node_setValue(Nodeptr self, void* val)
{
    
    // RN no wayh to set size for pointers`
    self->val = val;
    self->size = sizeof(val);
    // Know the data type, so if string then
    // Use strlen(val) + 1 <-- Gives the length of the string & Terminating NULL
}


/* Misc Functions */

// Prints the value of the node depending on the type variable
void Node_printVal(Nodeptr self)
{
    switch (self->type)
    {
        case INT:
        {
            /* code */
            printf("%d", *(int*)self->val); //casts void* val ptr to int*, then dereferences (tells me what the pointer is pointing at)
            break;
        }
        case STRING:
        {
            printf("%s", (char*)self->val);
            break;
        }
        case CHAR:
        {
            printf("%c", *(char*)self->val);
            break;
        }
        case DOUBLE:
        {
            double *temporaryDoublePointer = self->val;
            double temporaryDoubleStorage = *temporaryDoublePointer;
            printf("%.2f", temporaryDoubleStorage);
            break;
        }
        case POINTER:
        {
            printf("%p", self->val);
        }
        default:
        {
            break;
        }
    }
}

// Print out all the data within the node to the terminal. Mainly used for debugging purposes.
void Node_printNode(Nodeptr self)
{
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("| Node with value: ");
    Node_printVal(self);
    printf("\n| Next pointer: ");
    if(self->next != NULL)
    {
        printf("Node with value [");
        Node_printVal(self->next);
        printf("]\n");
    }
    else
    {
        printf("NULL\n");
    }

    printf("| Prev pointer: ");
    if(self->prev != NULL)
    {
        printf("Node with value [");
        Node_printVal(self->prev);
        printf("]\n");
    }
    else
    {
        printf("NULL\n");
    }
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
}

/* Deconstructor */

// Make all values of the node = NULL
void Node_reset(Nodeptr self)
{
    self->next = NULL;
    self->prev = NULL;
    self->size = NULL;
    // If val is actually a pointer, if you set it to NULL
    // You lose the pointer that points to the value itself, but not free the data somewhere else
    // Make sure that it manually frees the node->val just in case.

    // OTHER SOLUTION:
    /*
        Add a new variable into the node that is a pointer to the clearnup function
        In reset, call the clearnup function on self->val
    */
   if(self->type == POINTER)
   {
    free(self->val);
   }
    self->val = NULL;
} 

// Clear out the node's data and then free the struct from memory
void Node_destroy(Nodeptr self)
{
    if (self)
    {
        Node_reset(self);
        free(self);
    }
}


/*************************
 *     LIST FUNCTIONS    *
**************************/
/* Constructor */

// Make a new List object on the heap and return a Listptr to that object
Listptr List_new()
{
    Listptr returnedPointer = (Listptr)malloc(sizeof(List));
    List_init(returnedPointer);
    return returnedPointer;
}

// Initialize any variables that the list struct needs imediately
void List_init(Listptr l)
{
    l->len=0;
    l->head = NULL;
    l->tail = NULL;
}


/* Getters & Setters */

// Add a value at the END of the linked list
void List_addValue(Listptr self, void* val, dataType type)
{
    // First, make the node
    Nodeptr newNode = Node_new(val, type);

    if(self->len == 0)
    {
        self->head = newNode;
        self->tail = newNode;
    }
    else
    {
        self->tail->next = newNode;
        newNode->prev = self->tail;
        self->tail = newNode;
    }
    self->len++;
}


/* Other Methods */



// Returns the node object at a specific index within the Linked List
Nodeptr List_walkToIndex(Listptr self, int index)
{
    if( (index < 0) || (index > self->len) )
    {
        printf("Index %d out of bounds!\n", index);
        return NULL;
    }
    Nodeptr currentNode = self->head;
    if(index == 0 )
    {
        return currentNode;
    }
    for(int i = 0; i < index; i++)
    {
        currentNode = currentNode->next;
    }
    return currentNode;
}

/* Prints the entire Linked List to the terminal.
 Makes use of Node_printVal to ensure different types of data are 
 printed correctly.
*/
void List_printList(Listptr self)
{
    if(self->len > 0)
    {
        printf("[ ");
        Nodeptr n = self->head;
        for(int i = 0; i < self->len; i++)
        {
            if(i != self->len-1)
            {
                Node_printVal(n);
                printf(", ");
            }
            else
            {
                Node_printVal(n);
            }
            n = n->next;
        }
        printf("]\n");
    }
    else
    {
        printf("List is empty\n");
    }
}


/* Inserts a value at the index given in the Linked List.
   Pushes the existing value at index to index+1 in the list.
*/
void List_insertBefore(Listptr self, int index, void* val, dataType type)
{
    Nodeptr newNode = Node_new(val, type);
    Nodeptr nodeAtIndex = List_walkToIndex(self, index);
    if(nodeAtIndex == NULL)
    {
        Node_destroy(newNode);
        return;
    }

    newNode->next = nodeAtIndex;
    if(nodeAtIndex->prev == NULL)
    {
        self->head = newNode;
        nodeAtIndex->prev = newNode;
    }
    else
    {
        nodeAtIndex->prev->next = newNode;
        newNode->prev = nodeAtIndex->prev;
        nodeAtIndex->prev = newNode;
    }
    self->len++;
}


/* Inserts a value at the index AFTER the index given in the Linked List.
   Keeps the value at index where it is, but the given value will be in index+1
*/
void List_insertAfter(Listptr self, int index, void* val, dataType type)
{
    Nodeptr newNode = Node_new(val, type);
    Nodeptr nodeAtIndex = List_walkToIndex(self, index);
    if(nodeAtIndex == NULL)
    {
        Node_destroy(newNode);
        return;
    }

    newNode->next = nodeAtIndex->next;
    if(newNode->next != NULL)
    {
        newNode->next->prev = newNode;
    }
    else
    {
        self->tail = newNode;
    }
    nodeAtIndex->next = newNode;
    self->len++;
}

// Removes an item from the Linked List at index.
void List_removeAt(Listptr self, int index)
{
    // Check to make sure we're not removing the head of the list
    Nodeptr nodeToDelete = List_walkToIndex(self, index);
    if (nodeToDelete == NULL)
    {
        return;
    }

    if ((index == self->len - 1) & (index != 0))
    {
        Nodeptr nodeToDelete = List_walkToIndex(self, index);
        self->tail = nodeToDelete->prev;
        self->tail->next = NULL;
        Node_destroy(nodeToDelete);
    }
    else if((index > 0) & (index < self->len - 1))
    {
        Nodeptr previousNode = nodeToDelete->prev;
        previousNode->next = nodeToDelete->next;
        nodeToDelete->next->prev = previousNode;
        Node_destroy(nodeToDelete);

        if(index == self->len)
        {
            self->tail = previousNode;
        }
    }
    // If we are, then make the precautions needed
    else if (index == 0)
    {
        if(self->len != 1)
        {
            self->head = nodeToDelete->next;
            self->head->prev = NULL;
        }
        else
        {
            self->head = NULL;
            self->tail = NULL;
        }
        Node_destroy(nodeToDelete);
        nodeToDelete = NULL;
    }

    self->len--;
}

// Returns a Nodeptr object for the first node found with the given val in the Linked List. Returns NULL if nothing found.
Nodeptr List_findNodebyValue(Listptr self, void* val)
{
    // Main issues with this function:
    /*
        - The "val" argument is a void pointer
        - The "currentNode->val" is a void pointer but now we store it as an actual pointer address in the node
        - Need to convert from the address to the value, but need to know the type of value to properly convert it and compare. 
    */
    Nodeptr currentNode = self->head;
    for(int i = 0; i < self->len; i++)
    {
        if(Node_getValue(currentNode) == val)
        {
            return currentNode;
        }
        currentNode = currentNode->next;
    }
    return NULL;
}

// Finds a Node with the given value and unlinks it from the list. Returns the Nodeptr object.
Nodeptr List_unlinkNodebyValue(Listptr self, void* val)
{
    Nodeptr nodeToUnlink = List_findNodebyValue(self, val);
    if(nodeToUnlink != NULL)
    {
        printf("Unlinking node\n");
        nodeToUnlink->prev->next = nodeToUnlink->next;
        nodeToUnlink->prev = NULL;
        nodeToUnlink->next = NULL;
        self->len--;
        return nodeToUnlink;
    }
    return NULL;
}

// Takes 2 indexes and swaps their values in the Linked List.
void _shuffle(Listptr self, int firstIndex, int secondIndex)
{
    void* temporaryValueHolder = List_walkToIndex(self, firstIndex)->val;
    List_walkToIndex(self, firstIndex)->val = List_walkToIndex(self, secondIndex)->val;
    List_walkToIndex(self, secondIndex)->val = temporaryValueHolder;
}

// Sorts the given Linked List by the values it stores.
void List_valueSort(Listptr self)
{
    int sort_index;
    int isSorted = 0;
    int sortedCheck = 1;

    while(isSorted == 0)
    {
        for(int i = 0; i < self->len; i++)
        {
            sort_index = i;
            for(int j = i; j < self->len; j++)
            {
                if(Node_getValue(List_walkToIndex(self, j)) < Node_getValue(List_walkToIndex(self, i)))
                {
                    sort_index = j;
                }
            }
            _shuffle(self, i, sort_index);
        }

        // Need this last if-statement to error check for the shortcoming of the for-loop
        if(Node_getValue(self->head) > Node_getValue(self->head->next))
        {
            _shuffle(self, 0, 1);
        }

        for(int i = 0; i < self->len - 1; i++)
        {
            if(Node_getValue(List_walkToIndex(self, i)) > Node_getValue(List_walkToIndex(self, i + 1)))
            {
                sortedCheck = 0;
                break;
            }
        }
        if(sortedCheck != 1)
        {
            sortedCheck = 1;   
        }
        else
        {
            isSorted = 1;
            break;
        }

    }

}

// Sorts the given Linked List by the memory addresses of the values within it.
void List_addressSort(Listptr self)
{
    int sort_index;
    void* indexIAddress;
    void* indexJAddress;
    for(int i = 0; i < self->len; i++)
    {
        sort_index = i;
        for(int j = i; j < self->len; j++)
        {
            indexIAddress = (void *)List_walkToIndex(self, i)->val;
            indexJAddress = (void *)List_walkToIndex(self, j)->val;

            if(indexJAddress < indexIAddress)
            {
                sort_index = j;
            }
        }
        _shuffle(self, i, sort_index);
    }

    if ( (void *)self->head->val > (void *) self->head->next->val)
    {
        _shuffle(self, 0, 1);
    }
}


/* Deconstructors */

// Clears the values from the Linked List to make it empty.
void List_clear(Listptr self)
{
    while(self->head->next != NULL)
    {
        List_removeAt(self, 0);
    }
    List_removeAt(self, 0);
}

// Clears the Linked List and then frees it from memory.
void List_destroy(Listptr self)
{
    if(self)
    {
        if(self->head != NULL)
        {
            List_clear(self);
        }
        free(self->head);
        free(self->tail);
        free(self);
        self = NULL;
    }
}

/*************************
 *    STACK FUNCTIONS    *
**************************/
//Initializes the pointers and allocates memory
Stackptr Stack_new()
{
    Stackptr returnedPointer = (Stackptr)malloc(sizeof(Stack));
    Stack_init(returnedPointer);
    return returnedPointer;
}
//Constructor that uses List Class Functions
void Stack_init(Stackptr s)
{
    s->Stack_Backend = List_new(); //initializes a new stack, which uses the list function
}
//Push Function: uses List Class Add Value Function
void Stack_Push(Stackptr self, void* val, dataType type)
{
    List_addValue(self->Stack_Backend, val, type);
}
//Pop Function: uses List Class Walk To IDX and Remove At Functions
void* Stack_Pop(Stackptr self)
{
    int len = self->Stack_Backend->len;
    int pop_idx = len - 1;
    void* node_val = Node_getValue(List_walkToIndex(self->Stack_Backend, pop_idx));
    List_removeAt(self->Stack_Backend, pop_idx);
    if(self->Stack_Backend->len == len)
    {
        return NULL;
    }
    else
    {
        return node_val;
    }
}

//Find Node by Value Function: Uses List version of the function, but passes in a void* and a stack pointer(which is a list pointer)
void* Stack_findNodebyValue(Stackptr self, void* val)
{
    return List_findNodebyValue(self->Stack_Backend, val);
}

//Print function: Prints out the stack (in order from first value added to last)
void Stack_Print(Stackptr self)
{
    List_printList(self->Stack_Backend);
}

/*************************
 *    QUEUE FUNCTIONS    *
**************************/
/* Constructor */

Queueptr Queue_new()
{
    Queueptr returnedPointer = (Queueptr)malloc(sizeof(Queue));
    Queue_init(returnedPointer);
    return returnedPointer;
}

void Queue_init(Queueptr q)
{
    q->queueBackend = List_new();
}

void Queue_enqueue(Queueptr self, void* val, dataType type)
{
    List_addValue(self->queueBackend, val, type);
}

void* Queue_dequeue(Queueptr self)
{
    void* node_val = Node_getValue(self->queueBackend->head);
    int len = self->queueBackend->len;
    List_removeAt(self->queueBackend, 0);
    if(self->queueBackend->len == len)
    {
        return NULL;
    }
    else
    {
        return node_val;
    }
}

void Queue_clear(Queueptr self)
{
    List_clear(self->queueBackend);
}

void Queue_print(Queueptr self)
{
    List_printList(self->queueBackend);
}
