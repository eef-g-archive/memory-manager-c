#include <stdio.h>
#include "../headers/manager.h"

int main()
{
    Managerptr test_man = Manager_new(10000, FIRST);
    // Write unit tests for the Manager functions
    printf("Successful Allocation tests\n");
    Manager_allocate(test_man, 250);
    Manager_allocate(test_man, 250);
    Manager_allocate(test_man, 250);
    printf("Press enter to continue...");
    getchar();
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nSuccessful Freeing tests\n");
    Manager_free(test_man, 0);
    Manager_free(test_man, 250);
    Manager_free(test_man, 500);
    printf("Press enter to continue...");
    getchar();
    printf("\n\n\n\nPurposely Failed Allocation test\n");
    Manager_allocate(test_man, 5000000);
    printf("Press enter to continue...");
    getchar();
    printf("\n\n\n\nPurposely failed Free test\n");
    Manager_free(test_man, 5000000);
    
}
