#include <stdio.h>
#include "dslib.h"

typedef struct Manager
{
    Listptr free_list;
    Listptr busy_list;
    int size;
} Manager, * Managerptr;


Managerptr Manager_new(int size);
void Manager_init(Managerptr self, int size);
void Manager_allocate(void* data, int size, dataType type);
void Manager_destroy(Managerptr self);