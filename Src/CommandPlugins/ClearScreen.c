//
// Created by 38803 on 8/5/2017.
//

#include<stdio.h>
#include <stdlib.h>
#include <ArgsLib/ArgsParse.h>
#include <CommandLib/CommandHelper.h>
#include <ArgsLib/Reflection.h>

#include "ArgsLib/ArgsDecl.h"
#include "CommandPlugins/ClearScreen.h"

/*
 * Main command runner function.
 *
 */

static const char help_message[] = "Clear Command.\n"
        "Usage:\n"
        "   Clear [--help] \n"
        "";

static CommandResult runner(CommandLineArgs * commandLineArgs) {

    ClearScreenArgs clearScreenArgs = {
            .help = 0,
    };

    system(CLEAR_COMMAND);

    return COMMAND_OK;
}

INITIALIZER(SetupClearScreen)
{
    CommandRegistry * command = (CommandRegistry *)malloc(sizeof(CommandRegistry));
    command->commandName = "Clear";
    command->usagePattern = help_message;
    command->CallBack = runner;
    RegisterCommand(command);
}

