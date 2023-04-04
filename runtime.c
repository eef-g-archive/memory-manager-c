#include <stdio.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <string.h>
#include "headers/manager.h"


/* -= Function Declarations =-*/

int ReadFile(char* file, char check);
bool file_exists(char* filename);
int get_char_index(char* str, char c);
char* substring(char* str, int start, int end);


/************/
/*   MAIN   */
/************/

int main( int argc, char *argv[])
{
    int size = 0;
    int type = -1;
    Managerptr manager;

    #pragma region Initial Setup
    // Check for arguments & decide whether to load config file or not
    if (argc == 1)
    {
        // Read txt file for memory size and type, if there is no file then ask for input and state that it is manual input mode. Otherwise, say reading from config
        printf("[-] No config file given...\n");
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
            size = ReadFile(argv[1], 'S');
            type = ReadFile(argv[1], 'T');
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

    // Create the manager object based off of the arguments given
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
    #pragma endregion

    #pragma region Main Program Loop
    // Main program loop
    int running = 1;
    while(running == 1)
    {
        int choice = -1;
        printf("\nEnter 1 to allocate memory, 2 to free memory, 3 to exit: ");
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
    #pragma endregion
}

#pragma region String Functions
/******************/
/*     CUSTOM     */
/*    FUNCTIONS   */
/******************/

/* Returns true if a file exists, false otherwise. */
// filename: the name of the file to check
bool file_exists(char* filename)
{
    struct stat buffer;
    return (stat(filename, &buffer) == 0);
}

// A function to read a file and return the int value of a specific config portion
// file: the file to read
// check: the char to check for in the file
int ReadFile(char* file, char check)
{
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
            if(line[0] == check)
            {
                return atoi(substring(line, index + 2, strlen(line) - 2));
            }
        }
    }
    fclose(fp);
}

// A function to get the index of a char in a string
// str: the string to search
// c: the char to search for
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

// A function to get a substring of a string
// str: the string to get the substring from
// start: the index to start at
// end: the index to end at
char* substring(char* str, int start, int end)
{
    // allocate memory for the substring
    char* sub = malloc(sizeof(char) * (end - start + 1));
    int i = 0;
    // copy characters from start to end into sub
    while(start <= end)
    {
        sub[i] = str[start];
        i++;
        start++;
    }
    // append a null character to the end of sub
    sub[i] = '\0';
    return sub;
}
#pragma endregion
