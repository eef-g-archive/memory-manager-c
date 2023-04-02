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

// Function to create a new manager object
// size: the total amount of memory to manage
// mode: the fit mode to use (first fit or best fit)
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

// Function to initialize a manager object
// self: the manager object to initialize
// size: the total amount of memory to manage
// mode: the fit mode to use (first fit or best fit)
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


// Function to allocate a new block of memory
// self: the manager object to use
// size: the size of the block to allocate
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

// Function to free a task from memory
// self: the manager object to use
// address: the address of the block to free
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
            printf("Found the block to free\n");
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
    // This breaks if there are 2 or less items in the busy list
    List_valueSort(self->busy_list);

    Manager_coalesce(self);

    // Debug Stuff -- Will delete later
    Manager_printLists(self);
}

// Function to allocate a new block of memory using first fit
// self: the manager object to use
// size: the size of the block to allocate
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

// Function to allocate a new block of memory using best fit
// self: the manager object to use
// size: the size of the block to allocate
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


// Function to coalesce free blocks of memory
// self: the manager object to use
void Manager_coalesce(Managerptr self)
{
    Nodeptr curr = self->free_list->head;
    Nodeptr next = curr->next;
    int index = 0;
    while(next != NULL)
    {
        if(curr->val + curr->size == next->val)
        {
            curr->size += next->size;
            List_removeAt(self->free_list, index+1);
            next = curr->next;
        }
        else
        {
            curr = next;
            next = curr->next;
        }
        index++;
    }
}


// void Manager_coalesce(Managerptr self)
// {
//     // How to coalesce:
//     /*
//         1. Check to see if there are multiple free blocks next to eachother, and smush them together
//         2. Only smush blocks next to eachother until you hit a busy block. Once you hit a busy block, then you cannot smush any further!
//         3. So, check each free block and see if they start right after the current block and then combine them. If there is a gap between blocks, ignore coalescing.
//         (i.e. We have five free list nodes with addresses from 0-150, 150-2000, 2000-2500, 4000-4040, 4050-5000 as well as busy list nodes from 2500-4000 and 4040-4050.
//         We want to combine the first 3 free blocks to make a new free block from 0-2500, but cannot combine the new large block with either of the 2 remaining free blocks.
//         This is because we hit a busy block and there are no free blocks right next to eachother.)
//     */
//    //Goal is to make a loop that cycles through the manager until it hits a block thats being used
//     // make a list and push start and end index to it? then when re organizing, you can use the numbers for the addition/subtraction
//     Nodeptr curr = self->free_list->head;
//     int prev_size = curr->size;
//     int prev_address = curr->val;
//     int curr_size = curr->size;
//     int i = 0;
//     int is_combined = 0; // Treat this like a bool, please

//     while(is_combined == 0)
//     {
//         // Reset to be at the beginning
//         i = 0;
//         curr = self->free_list->head;
//         prev_size = curr->size;
//         prev_address = curr->val;

//         while(i < self->free_list->len)
//         {
//             //get block length, start, finish
//             if(prev_address + prev_size == curr->val)
//             {
//                 printf("New starting address: %d\nNew node size: %d\nCurrent index: %d",prev_address, prev_size + curr->size, i);
//                 curr_size = curr->size;
//                 List_removeAt(self->free_list, i - 1);
//                 List_addValue(self->free_list, prev_address, INT);
//                 self->free_list->tail->size = prev_size + curr_size;
//                 List_valueSort(self->free_list);
//                 // Flag for combination
//                 break;
//             }
//             //next block
//             i++;
//             curr = curr->next; // not sure why this doesnt work, considering the free list is a listptr
//             if(curr == NULL)
//             {
//                 is_combined = 1;
//             }
//         }
//     }

// }

/*********************/
/*  OUTPUT & DESTROY */
/*     FUNCTIONS     */
/*********************/


// Function to print the free and busy lists
// self: the manager object to use
void Manager_printLists(Managerptr self)
{
    printf("##############\nFree List -- Length: %d\n", self->free_list->len);
    _print_List(self->free_list);
    printf("##############\nBusy List -- Length: %d\n", self->busy_list->len);
    _print_List(self->busy_list);
}

// Function to destroy the manager object
// self: the manager object to destroy
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


// Function to print the contents of a list
// self: the list to print
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

// Function to print the contents of a node
// self: the node to print
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
