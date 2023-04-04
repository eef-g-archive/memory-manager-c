#include <stdio.h>
#include "headers/manager.h"

int main()
{
    int size = 0;
    int type = -1;
    Managerptr manager;
    printf("Enter size of memory: ");
    scanf("%d", &size);
    printf("Enter type of memory (0 for FIRST, 1 for BEST): "); 
    scanf("%d", &type);

    switch(type)
    {
        case 0:
        {
            manager = Manager_new(size, FIRST);
            printf("Manager created with %d memory using FIRST type of allocation.\n", size);
            break;
        }
        case 1:
        {
            manager = Manager_new(size, BEST);
            printf("Manager created with %d memory using BEST type of allocation.\n", size);
            break;
        }
        default:
        {
            printf("Invalid type of memory. Exiting...");
            return 0;
        }
    }

    int running = 1;
    while(running == 1)
    {
        int choice = -1;
        printf("Enter 1 to allocate memory, 2 to free memory, 3 to exit: ");
        scanf("%d", &choice);
        switch(choice)
        {
            case 1:
            {
                int size = 0;
                printf("Enter size of memory to allocate: ");
                scanf("%d", &size);
                Manager_allocate(manager, size);
                break;
            }
            case 2:
            {
                int address = 0;
                printf("Enter address of memory to free: ");
                scanf("%d", &address);
                Manager_free(manager, address);
                break;
            }
            case 3:
            {
                running = 0;
                Manager_destroy(manager);
                break;
            }
            default:
            {
                printf("Invalid choice. Try again.\n");
                break;
            }
        }
    }

}