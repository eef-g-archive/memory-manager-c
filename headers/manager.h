#include <stdio.h>
#include "../include/dslib.h"

typedef struct Manager
{
    Listptr free_list;
    Listptr busy_list;
    int total_size;
} Manager, * Managerptr;


Managerptr Manager_new(int size);
void Manager_init(Managerptr self, int size);
void Manager_allocate(Managerptr self, int size);
void Manager_printLists(Managerptr self);
void Manager_destroy(Managerptr self);
