#include <stdio.h>

#include "ArgsLib/ArgsParse.h"
#include "CommandLib/CommandHelper.h"
#include "CommandLib/Console.h"

int main()
{
    Console * console = SetupConsole();
    unsigned char ch;
    ConsoleAction status;
    bool shouldExit = false;
    while(shouldExit == false){
        // Read a user input character
        ch = _getch();
        // Process the input
        status = console->onKeyPress(ch);
        // If user wants to execute a command
        if(status == CONSOLE_EXEC)
        {
            // Unix like parsing for command line.
            CommandLineArgs * args = ShlexParse(console->buffer);
            if(args->argc > 0)
            {
                printArgs(args);
                // Find command from registry
                CommandRegistry * command = findCommand(args->argv[0]);
                if(command)
                {
                    // Add to history.
                    console->AddToHistory(console->buffer);
                    // Call command specific handler.
                    command->CallBack(args);
                    // TODO: Check for return code.
                }
                // Clean up arguments
                cleanArgs(args);
            }
            // Housekeeping after command has executed.
            console->onCommandExec();
        }
        // Break using Ctrl + C
        else if (status == CONSOLE_BREAK)
            shouldExit = true;
        // Handle Tabbed key
        else if (status == CONSOLE_COMMAND_SUGGEST)
        {
            // If the user has not yet typed first word.
            if(!console->isSpacePressed)
                PrintRegistry(console->buffer);
            // Command specific usage.
            else
            {
                CommandLineArgs * args = ShlexParse(console->buffer);
                if(args->argc > 0)
                {
                    CommandRegistry * command = findCommand(args->argv[0]);
                    if(command) CommandSuggestions(command->usagePattern, console->isSpacePressed);
                    else printf("\a"); // System bell.
                    fflush(stdout);
                }

            }
            // Command has not yet executed, so restore user typed buffer after showing suggestions.
            console->onHintShow();
        }
        // User is trying to input a file path, give suggestions.
        else if(status == CONSOLE_FILE_SUGGEST)
        {
            console->showCompletion();
            console->onHintShow();
        }
        // Load from history.
        else if (status == CONSOLE_HISTORY_LOAD )
        {
            console->LoadFromHistory();
            console->onHintShow();
        }
    }
    printf("\nPress any key to exit ...");
    getchar();
    return 0;
}
