#include <stdio.h>
#include "../headers/manager.h"

Managerptr Manager_new(int size)
{
    Managerptr returnedPointer = (Managerptr)malloc(sizeof(Manager));
    Manager_init(returnedPointer, size);
    return returnedPointer;
}

void Manager_init(Managerptr self, int size)
{
    self->busy_list = List_new();
    self->free_list = List_new();
    self->size = size;
}


// TODO: Create the Manager_allocate() function here




void Manager_destroy(Managerptr self)
{
    if(self)
    {
        List_destroy(self->busy_list);
        List_destroy(self->free_list);
        free(self);
    }
}