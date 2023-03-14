#include <stdio.h>
#include "../headers/manager.h"
// Things to keep track of with the memory manager:
/*
    1. We need to manually manipulate the size of the nodes
    2. The 'val' for the nodes is being used for their location in memory
    3. The 'size' for the nodes is being used to keep track of the size of the item in memory
*/



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
            FirstFitAlloc(self, size);
            break;
        }
        case BEST:
        {
            FirstFitAlloc(self, size);
            break;
        }
        default:
        {
            break;
        }
   }
    // Debug Stuff -- Will delete later
    Manager_printLists(self);
}

void FirstFitAlloc(Managerptr self, int size)
{
    int available_size = self->free_list->tail->size - size;
    int insert_slot = self->free_list->tail->val;

    List_addValue(self->busy_list, insert_slot, INT);
    self->busy_list->tail->size = size;

    int new_address = insert_slot + size;

    self->free_list->tail->size -= size;

    self->free_list->tail->val = new_address;
}


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


// Functions for debugging
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