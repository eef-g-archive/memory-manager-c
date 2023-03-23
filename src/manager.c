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
    // Debug Stuff -- Will delete later
    Manager_printLists(self);
}

void Manager_free(Managerptr self, int address)
{
    printf("\n\n~~~~~~~~~~\nFreeing\n~~~~~~~~~~\n");
    Nodeptr freed_block = NULL;
    Nodeptr curr = self->busy_list->head;
    int index = 0;
    while(curr != NULL)
    {
        if(curr->val == address)
        {
            freed_block = curr;
            break;
        }
        curr = curr->next;
        index ++;
    }

    if(freed_block == NULL) { return; }
    int new_value = freed_block->val;
    List_addValue(self->free_list, new_value, INT);
    self->free_list->tail->size = freed_block->size;
    List_removeAt(self->busy_list, index);
    List_valueSort(self->free_list);
    List_valueSort(self->busy_list);

    // TODO: Coalesce Memory Here
    Manager_coalesce(self);

    // Debug Stuff -- Will delete later
    Manager_printLists(self);
}

void _FirstFitAlloc(Managerptr self, int size)
{
    int insert_slot, new_address;

    Nodeptr curr = self->free_list->head;
    int index = 0;
    while(curr != NULL)
    {
        if(size <= curr->size)
        {
            break;
        }
        curr = curr->next;
        index++;
    }

    if(size <= curr->size)
    {
        insert_slot = curr->val;

        List_addValue(self->busy_list, insert_slot, INT);
        self->busy_list->tail->size = size;

        if (size == curr->size)
        {
            List_removeAt(self->free_list, index);
        }
        else
        {
            new_address = insert_slot + size;
            curr->size -= size;
            curr->val = new_address;   
        }
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
    int min_diff = self->total_size;

    Nodeptr curr = self->free_list->head;
    int i = 0;
    block_location = i;    
    while(curr != NULL)
    {
        if((curr->size < min_diff) & (curr->size >= size))
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
    if(curr->size - size == 0)
    {
        List_removeAt(self->free_list, block_location);   
    }
    else
    {
        new_address = insert_slot + size;
        curr->size -= size;
        curr->val = new_address;   
    }
}



void Manager_coalesce(Managerptr self)
{
    // How to coalesce:
    /*
        1. Check to see if there are multiple free blocks next to eachother, and smush them together
        2. Only smush blocks next to eachother until you hit a busy block. Once you hit a busy block, then you cannot smush any further!
        3. So, check each free block and see if they start right after the current block and then combine them. If there is a gap between blocks, ignore coalescing.
        (i.e. We have five free list nodes with addresses from 0-150, 150-2000, 2000-2500, 4000-4040, 4050-5000 as well as busy list nodes from 2500-4000 and 4040-4050.
        We want to combine the first 3 free blocks to make a new free block from 0-2500, but cannot combine the new large block with either of the 2 remaining free blocks.
        This is because we hit a busy block and there are no free blocks right next to eachother.)
    */


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
        printf("| Block with address: %d\n", self->val);
        printf("| Next pointer: ");
        if(self->next != NULL)
        {
            printf("Block with address [%d]\n", self->next->val);
        }
        else
        {
            printf("NULL\n");
        }

        printf("| Prev pointer: ");
        if(self->prev != NULL)
        {
            printf("Block with address [%d]\n", self->prev->val);
        }
        else
        {
            printf("NULL\n");
        }
        printf("| Block size: %d\n", self->size);
        printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
}