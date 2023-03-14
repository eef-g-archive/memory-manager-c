#include <stdio.h>
#include "../headers/manager.h"
// Things to keep track of with the memory manager:
/*
    1. We need to manually manipulate the size of the nodes
    2. The 'val' for the nodes is being used for their location in memory
    3. The 'size' for the nodes is being used to keep track of the size of the item in memory
*/


/*********************/
/*     CONSTRUCTOR   */
/*      FUNCTIONS    */
/*********************/


Managerptr Manager_new(int size, fitType mode)
{
    Managerptr returnedPointer = (Managerptr)malloc(sizeof(Manager));
    Manager_init(returnedPointer, size, mode);
    
    // Have to initialize the list in THIS function
    // If initialized in the Manager_init function, the value of the first node breaks, idk why
    int location = 0;
    List_addValue(returnedPointer->free_list, location, INT);
    returnedPointer->free_list->tail->size = size;
    return returnedPointer;
}

void Manager_init(Managerptr self, int size, fitType mode)
{
    self->mode = mode;
    self->busy_list = List_new();
    self->free_list = List_new();
    self->total_size = size;
}


/*********************/
/*     ALLOCATION    */
/*      FUNCTIONS    */
/*********************/


void Manager_allocate(Managerptr self, int size)
{

    printf("\n\n~~~~~~~~~~\nAllocating\n~~~~~~~~~~\n");

    // When allocating node, do the following:
    /*
        1. Make sure that the node's val is = the process's location in memory
            (i.e. At the beginning, val=0, then next allocation, val=free_list->head->val)
        2. Make sure that the node's size = process size
    */
   switch(self->mode)
   {
        case FIRST:
        {
            _FirstFitAlloc(self, size);
            break;
        }
        case BEST:
        {
            _BestFitAlloc(self, size);
            break;
        }
        default:
        {
            break;
        }

        // Sort the two lists by value to arrange them by the fake memory addresses
        List_valueSort(self->free_list);
        List_valueSort(self->busy_list);
   }
   // TODO: Automatically Coallesce the data after allocation (We want the free list to be as little as possible)

    // Debug Stuff -- Will delete later
    Manager_printLists(self);
}

void _FirstFitAlloc(Managerptr self, int size)
{
    int insert_slot, new_address;

    Nodeptr curr = self->free_list->head;
    while(curr != NULL)
    {
        if(size < curr->size)
        {
            break;
        }
        curr = curr->next;
    }

    if(size < curr->size)
    {
        insert_slot = curr->val;

        List_addValue(self->busy_list, insert_slot, INT);
        self->busy_list->tail->size = size;

        new_address = insert_slot + size;
        curr->size -= size;
        curr->val = new_address;
    }
    else
    {
        printf("No free block large enough for task of size %d", size);
        return;
    }
}

void _BestFitAlloc(Managerptr self, int size)
{
    int insert_slot, new_address, block_location;
    int min_diff = self->free_list->head->size;

    Nodeptr curr = self->free_list->head;
    int i = 0;
    block_location = i;    
    while(curr != NULL)
    {
        if(curr->size < min_diff)
        {
            min_diff = curr->size;
            block_location = i;
        }
        i++;
        curr = curr->next;
    }

    curr = List_walkToIndex(self->free_list, block_location);
    insert_slot = curr->val;

    List_addValue(self->busy_list, insert_slot, INT);
    self->busy_list->tail->size = size;
    new_address = insert_slot + size;
    curr->size -= size;
    curr->val = new_address;
}


/*********************/
/*  OUTPUT & DESTROY */
/*     FUNCTIONS     */
/*********************/


void Manager_printLists(Managerptr self)
{
    printf("##############\nFree List -- Length: %d\n", self->free_list->len);
    _print_List(self->free_list);
    printf("##############\nBusy List -- Length: %d\n", self->busy_list->len);
    _print_List(self->busy_list);
}


void Manager_destroy(Managerptr self)
{
    if(self)
    {
        List_destroy(self->busy_list);
        List_destroy(self->free_list);
        free(self);
    }
}


/*********************/
/*       DEBUG       */
/*      FUNCTIONS    */
/*********************/


void _print_List(Listptr self)
{
    if(self->len == 0)
    {
        printf("##############\n");
        return;
    }
    Nodeptr curr = self->head;
    while(curr != NULL)
    {
        _custom_Node_Print(curr);
        curr = curr->next;
    }
    printf("##############\n\n");
}

void _custom_Node_Print(Nodeptr self)
{
        printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
        printf("| Node with value: %d\n", self->val);
        printf("| Next pointer: ");
        if(self->next != NULL)
        {
            printf("Node with value [%d]\n", self->next->val);
        }
        else
        {
            printf("NULL\n");
        }

        printf("| Prev pointer: ");
        if(self->prev != NULL)
        {
            printf("Node with value [%d]\n", self->prev->val);
        }
        else
        {
            printf("NULL\n");
        }
        printf("| Node size: %d\n", self->size);
        printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
}