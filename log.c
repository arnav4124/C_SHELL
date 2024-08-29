#include "log.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int line_counter()
{
    FILE *file = fopen("/mnt/c/Users/dell/Documents/OSN/Mini_Projects/MiniProject-1/log.txt", "r");
    if (file == NULL)
    {
        printf(RED"Error in opening file\n"RESET);
        return 0;
    }

    char line[1024];
    int i = 0;
    while (fgets(line, sizeof(line), file))
    {
        i++;
    }
    fclose(file);
    return i;
}

int check_log(char* str)
{
    // Make a copy to avoid modifying the original string
    char temp[1024];
    strncpy(temp, str, 1024);
    char* token = strtok(temp, " ");
    return (strcmp(token, "log") == 0) ? 1 : 0;
}

void log_entry(char str[1024])
{
  
    // printf("Count: %d\n", count);
    FILE *file = fopen("/mnt/c/Users/dell/Documents/OSN/Mini_Projects/MiniProject-1/log.txt", "a+");
    if (file == NULL)
    {
        printf(RED"Error in opening file\n"RESET);
        return;
    }

    

    // Check the last line in the file
    char last_line[1024];
    FILE *file_read = fopen("/mnt/c/Users/dell/Documents/OSN/Mini_Projects/MiniProject-1/log.txt", "r");
   
    if (file_read == NULL)
    {
        printf(RED"Error in opening file\n"RESET);
        fclose(file);
        return;
    }

    while (fgets(last_line, sizeof(last_line), file_read))
    {
        // Loop until the last line
    }

    fclose(file_read);
    // printf("Last line: %s", last_line);
    last_line[strlen(last_line) - 1] = '\0';  // Remove newline character
   // printf("Current line: %s\n", str);
    // Only add the entry if it's not a log and doesn't match the last line
    if (!check_log(str) && strcmp(last_line, str) != 0)
    {
        fprintf(file, "%s\n", str);
        // count++;
    }
    fclose(file);
      int count = line_counter();
    // Handle log rotation if more than 15 lines
    if (count >= 16)
    {
        FILE *file_read = fopen("/mnt/c/Users/dell/Documents/OSN/Mini_Projects/MiniProject-1/log.txt", "r+");
        FILE *file_temp = fopen("/mnt/c/Users/dell/Documents/OSN/Mini_Projects/MiniProject-1/temp.txt", "w+");

        if (file_read == NULL || file_temp == NULL)
        {
            printf(RED"Error in opening file\n"RED);
            fclose(file_read);
            fclose(file_temp);
            return;
        }

        char line[1024];
        int i = 0;
        while (fgets(line, sizeof(line), file_read))
        {
            if (i > 0)  // Skip the first line
            {
                fprintf(file_temp, "%s", line);
            }
            i++;
        }
        fclose(file_temp);
         fclose(file_read);
        file_temp = fopen("/mnt/c/Users/dell/Documents/OSN/Mini_Projects/MiniProject-1/temp.txt", "r");
        file_read = fopen("/mnt/c/Users/dell/Documents/OSN/Mini_Projects/MiniProject-1/log.txt", "w");
        while(fgets(line, sizeof(line), file_temp))
        {
           fprintf(file_read, "%s", line);
        }
        fclose(file_read);
        fclose(file_temp);

        remove("log.txt");
        rename("temp.txt", "log.txt");
    }
}

void log_print()
{
    FILE *file = fopen("/mnt/c/Users/dell/Documents/OSN/Mini_Projects/MiniProject-1/log.txt", "r");
    if (file == NULL)
    {
        printf(RED"Error in opening file\n"RESET);
        return;
    }

    char line[1024];
    while (fgets(line, sizeof(line), file) != NULL)
    {
        printf("%s", line);
    }

    fclose(file);
}

void log_call(char* args)
{
    char exe[23];
    int linearg;

    if (strcmp(args, "purge") == 0)
    {
        FILE *file = fopen("/mnt/c/Users/dell/Documents/OSN/Mini_Projects/MiniProject-1/log.txt", "w+");
        if (file == NULL)
        {
            printf(RED"Error in opening file\n"RESET);
            return;
        }
        fclose(file);
        return;
    }

    int sc = sscanf(args, "%s %d", exe, &linearg);
    if (sc < 1 || strcmp(exe, "execute") != 0)
    {
        printf(RED"Invalid command\n"RESET);
        return;
    }

    if (sc == 2)
    {
        if(linearg<1 || linearg>15){
            printf(RED"Invalid line number\n"RESET);
            return;
        }
        int count = line_counter();
        if (linearg > count)
        {
            printf(RED"Invalid line number\n"RESET);
            return;
        }

        FILE *file = fopen("/mnt/c/Users/dell/Documents/OSN/Mini_Projects/MiniProject-1/log.txt", "r");
        if (file == NULL)
        {
            printf(RED"Error in opening file\n"RESET);
            return;
        }

        char line[1024];
        int i = 0;
        while (fgets(line, sizeof(line), file))
        {
            if (i == count - linearg)
            {
                // Execute the line content if necessary
                // tok(line); // Assuming tok is a function to process the line
                line[strlen(line) - 1] = '\0'; 
                if(linearg!=1){
                    log_entry(line);
                } // Remove newline character
                tok(line);
                // printf("Executing: %s", line); // Placeholder for tok(line)
            }
            i++;
        }
        fclose(file);
    }
}
