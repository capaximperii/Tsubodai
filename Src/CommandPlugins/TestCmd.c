#include<stdio.h>
#include <stdlib.h>

#include "ArgsLib/ArgsParse.h"
#include "CommandLib/CommandHelper.h"
#include "ArgsLib/Reflection.h"
#include "ArgsLib/ArgsDecl.h"
#include "CommandPlugins/TestCmd.h"

static const char help_message[] = "Test Command.\n"
"\n"
"Usage:\n"
"  Test ship create <name>...\n"
"  Test ship <name> move <x> <y> [--speed=<kn>]\n"
"  Test ship shoot <x> <y>\n"
"  Test mine (set|remove) <x> <y> [--moored|--drifting]\n"
"  Test --help\n"
"  Test --version\n"
"\n"
"Options:\n"
"  -h --help     Show this screen.\n"
"  --version     Show version.\n"
"  --speed=<kn>  Speed in knots [default: 10].\n"
"  --moored      Moored (anchored) mine.\n"
"  --drifting    Drifting mine.\n"
"\n"
"";

static const char usage_pattern[] =
"Usage:\n"
"  Test ship create <name>...\n"
"  Test ship <name> move <x> <y> [--speed=<kn>]\n"
"  Test ship shoot <x> <y>\n"
"  Test mine (set|remove) <x> <y> [--moored|--drifting]\n"
"  Test --help\n"
"  Test --version";


/*
 * Main command runner function.
 *
 */
static CommandResult runner(CommandLineArgs * commandLineArgs) {

    TestCmdArgs args = {
        .create = 0,
        .mine = 0,
        .move = 0,
        .remove = 0,
        .set = 0,
        .ship = 0,
        .shoot = 0,
        .name = (char *)"Default Name",
        .x = NULL,
        .y = NULL,
        .drifting = 0,
        .help = 0,
        .moored = 0,
        .speed = "10",
    };

    SubCommand commands[] = {
            {"create", 0},
            {"mine", 0},
            {"move", 0},
            {"remove", 0},
            {"set", 0},
            {"ship", 0},
            {"shoot", 0}
    };

    Option options[] = {
            {NULL, "--drifting", 0, 0, NULL},
            {"-h", "--help", 0, 0, NULL},
            {NULL, "--moored", 0, 0, NULL},
            {NULL, "--version", 0, 0, NULL},
            {NULL, "--speed", 1, 0, NULL},
            {"-n", "--name", 1, 0, NULL},
            {"-x", "--x", 1, 0, NULL},
            {"-y", "--y", 1, 0, NULL}
    };

    Elements elements = {
            .n_commands = sizeof(commands) / sizeof(commands[0]),
            .n_options = sizeof(options) / sizeof(options[0]),
            .commands = commands,
            .options = options
    };

    MAGIC_COMMANDLINE_PARSER(commandLineArgs, elements, TestCmdArgs, args);

    printf("Commands\n");
    printf("    mine == %s\n", args.mine ? "true" : "false");
    printf("    move == %s\n", args.move ? "true" : "false");
    printf("    create == %s\n", args.create ? "true" : "false");
    printf("    remove == %s\n", args.remove ? "true" : "false");
    printf("    set == %s\n", args.set ? "true" : "false");
    printf("    ship == %s\n", args.ship ? "true" : "false");
    printf("    shoot == %s\n", args.shoot ? "true" : "false");
    printf("Flags\n");
    printf("    --drifting == %s\n", args.drifting ? "true" : "false");
    printf("    --help == %s\n", args.help ? "true" : "false");
    printf("    --moored == %s\n", args.moored ? "true" : "false");
    printf("Options\n");
    printf("    --name == %s\n", args.name);
    printf("    --speed == %s\n", args.speed);
    printf("    --x == %s\n", args.x);
    printf("    --y == %s\n", args.y);
    return COMMAND_OK;
}

INITIALIZER(SetupTestCmd)
{
    CommandRegistry * command = (CommandRegistry *)malloc(sizeof(CommandRegistry));
    command->commandName = "Test";
    command->usagePattern = usage_pattern;
    command->CallBack = runner;
    RegisterCommand(command);
}

