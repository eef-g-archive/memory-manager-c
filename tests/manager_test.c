#include <stdio.h>
#include "../headers/manager.h"

int main()
{
    Managerptr test_man = Manager_new(10000, BEST);
    Manager_allocate(test_man, 1000);
    Manager_allocate(test_man, 10);
    Manager_allocate(test_man, 4000);
    Manager_free(test_man, 1000);
}
