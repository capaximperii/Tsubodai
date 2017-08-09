//
// Created by 38803 on 8/5/2017.
//

#include <CommandLib/CommandHelper.h>
#include "CommandLib/CommandHelper.h"

CommandRegistry * head = 0x0;

CommandResult RegisterCommand(CommandRegistry * newCommand)
{
    if(head == NULL) {
        head = newCommand;
        head->commandCount = 1;
        head->commandName = newCommand->commandName;
        head->next = NULL;
        head->head = head;
    }
    else {
        CommandRegistry * ptr = head;
        while (ptr->next) ptr = ptr->next;
        head->commandCount ++;
        ptr->next = newCommand;
        newCommand->next = NULL;
        ptr->head = head;
    }
    return COMMAND_OK;
}

CommandRegistry * findCommand(char * name)
{
    CommandRegistry * ptr = head;
    while( ptr && strcmp(name, ptr->commandName)) ptr = ptr->next;
    return ptr;
}

CommandRegistry * GetCommandRegistry()
{
    return head;
}

void PrintRegistry(char * startsWith)
{
    CommandRegistry * ptr = head;
    printf("\n");
    while(ptr)
    {
        if( startsWith == NULL || strlen(startsWith) == 0)
            printf("%-20s", ptr->commandName);
        else if(strncmp(startsWith, ptr->commandName, strlen(startsWith)) == 0)
            printf("%-20s", ptr->commandName);
        ptr = ptr->next;
    }
    printf("\n");
}

void CommandSuggestions(const char* usagePattern, unsigned int nSpaces)
{
    char *rest = strdup(usagePattern);
    unsigned int spacesInLine;
    char * line = strtok(rest, "\n");
    printf("\n");
    while (line != NULL)
    {
        spacesInLine = 0;
        char * ptr = line;
        while(*ptr)
        {
            if(isspace(*ptr) && !isspace(* (ptr - 1)) && !isspace(*(ptr + 1)))
                spacesInLine ++;
            ptr ++;
        }
        if(spacesInLine >= nSpaces)
            printf("%s\n", line);
        line = strtok(NULL, "\n");
    }
    free(rest);
    printf("\n");
}
