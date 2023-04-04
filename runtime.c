#include <stdio.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <string.h>
#include "headers/manager.h"

void ReadFile(char* file);
bool file_exists(char* filename);
int get_char_index(char* str, char c);

int main( int argc, char *argv[])
{
    int size = 0;
    int type = -1;
    Managerptr manager;

    if (argc == 1)
    {
        // Read txt file for memory size and type, if there is no file then ask for input and state that it is manual input mode. Otherwise, say reading from config
        printf("-= Manual Input Mode =-\n");
        printf("Enter size of memory: ");
        scanf("%d", &size);
        printf("Enter type of memory (0 for FIRST, 1 for BEST): "); 
        scanf("%d", &type);
    }
    else if (argc == 2)
    {
        printf("-= Config File Mode =-\n");
        printf("Creating manager from config file \"%s\"...\n", argv[1]);
        if(file_exists(argv[1]))
        {
            printf("\"%s\" exists!\n", argv[1]);
            ReadFile(argv[1]);  
        }
        else
        {
            printf("\"%s\" does not exist. Exiting...\n", argv[1]);
            return 0;
        }
    }
    else
    {
        printf("Invalid number of arguments. Exiting...\n");
        return 0;
    }




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

bool file_exists(char* filename)
{
    struct stat buffer;
    return (stat(filename, &buffer) == 0);
}

void ReadFile(char* file)
{
    char* size, *type;
    FILE *fp;
    fp = fopen(file, "r");
    if (fp == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }
    char line[256];
    while (fgets(line, sizeof(line), fp))
    {
        if(line[0] != '#')
        {
            int index = get_char_index(line, ':');
            if(line[0] == 'S')
            {
                // TODO: Get substring of line from index + 1 to end of line
                strcpy(size, line + (index + 1));
                printf("Size: %s", size);
            }
            //printf("%s | Index of ':' = %d | line[%d] = %c\n", line, index, index, line[index]);
        }
    }
    fclose(fp);
}

int get_char_index(char* str, char c)
{
    int i = 0;
    while(str[i] != '\0')
    {
        if(str[i] == c)
        {
            return i;
        }
        i++;
    }
    return -1;
}