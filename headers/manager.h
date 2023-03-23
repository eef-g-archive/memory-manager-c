#include <stdio.h>
#include "../include/dslib.h"

typedef enum {FIRST, BEST} fitType;

typedef struct Manager
{
    Listptr free_list;
    Listptr busy_list;
    int total_size;
    fitType mode;
} Manager, * Managerptr;


Managerptr Manager_new(int size, fitType mode);
void Manager_init(Managerptr self, int size, fitType mode);
void Manager_allocate(Managerptr self, int size);
void Manager_free(Managerptr self, int address);
void Manager_coalesce(Managerptr self);
void Manager_printLists(Managerptr self);
void Manager_destroy(Managerptr self);
