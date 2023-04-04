#include <stdio.h>
#include "../headers/manager.h"

int main()
{
    Managerptr test_man = Manager_new(10000, FIRST);
    // Write unit tests for the Manager functions
    printf("Allocate test\n");
    Manager_allocate(test_man, 250);
    Manager_allocate(test_man, 250);
    Manager_allocate(test_man, 250);
    printf("Press enter to continue...");
    getchar();
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nFree test\n");
    Manager_free(test_man, 0);
    Manager_free(test_man, 250);

}
