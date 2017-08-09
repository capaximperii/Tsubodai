#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ArgsLib/ArgsParse.h>
#include <ArgsLib/Reflection.h>
#include <ArgsLib/ArgsDecl.h>

#include "ArgsLib/ArgsDecl.h"

Tokens tokens_new(int argc, char **argv) {
    Tokens ts = {argc, argv, 0, argv[0]};
    return ts;
}

Tokens* tokens_move(Tokens *ts) {
    if (ts->i < ts->argc) {
        ts->current = ts->argv[++ts->i];
    }
    if (ts->i == ts->argc) {
        ts->current = NULL;
    }
    return ts;
}


/*
 * ARGV parsing functions
 */

int parse_doubledash(Tokens *ts, Elements *elements) {
    //int n_commands = elements->n_commands;
    //Command *commands = elements->commands;
    // not implemented yet
    return 0;
}

int parse_long(Tokens *ts, Elements *elements) {
    int i;
    size_t len_prefix;
    int n_options = elements->n_options;
    char *eq = strchr(ts->current, '=');
    Option *option;
    Option *options = elements->options;

    len_prefix = (eq-(ts->current))/sizeof(char);
    for (i=0; i < n_options; i++) {
        option = &options[i];
        if (!strncmp(ts->current, option->olong, len_prefix))
            break;
    }
    if (i == n_options) {
        // TODO '%s is not a unique prefix
        fprintf(stderr, "%s is not recognized\n", ts->current);
        return 1;
    }
    tokens_move(ts);
    if (option->argcount) {
        if (eq == NULL) {
            if (ts->current == NULL) {
                fprintf(stderr, "%s requires argument\n", option->olong);
                return 1;
            }
            option->argument = ts->current;
            printf("found %s", ts->current);

            tokens_move(ts);
        } else {
            option->argument = eq + 1;
        }
    } else {
        if (eq != NULL) {
            fprintf(stderr, "%s must not have an argument\n", option->olong);
            return 1;
        }
        option->value = true;
    }
    return 0;
}

int parse_shorts(Tokens *ts, Elements *elements) {
    char *raw;
    int i;
    int n_options = elements->n_options;
    Option *option;
    Option *options = elements->options;

    raw = &ts->current[1];
    tokens_move(ts);
    while (raw[0] != '\0') {
        for (i=0; i < n_options; i++) {
            option = &options[i];
            if (option->oshort != NULL && option->oshort[1] == raw[0])
                break;
        }
        if (i == n_options) {
            // TODO -%s is specified ambiguously %d times
            fprintf(stderr, "-%c is not recognized\n", raw[0]);
            return 1;
        }
        raw++;
        if (!option->argcount) {
            option->value = true;
        } else {
            if (raw[0] == '\0') {
                if (ts->current == NULL) {
                    fprintf(stderr, "%s requires argument\n", option->oshort);
                    return 1;
                }
                raw = ts->current;
                tokens_move(ts);
            }
            option->argument = raw;
            break;
        }
    }
    return 0;
}

int parse_argcmd(Tokens *ts, Elements *elements) {
    int i;
    int n_commands = elements->n_commands;
    SubCommand *command;
    SubCommand *commands = elements->commands;

    for (i=0; i < n_commands; i++) {
        command = &commands[i];

        if (!strcmp(command->name, ts->current)){
            command->value = true;
            tokens_move(ts);
            return 0;
        }
    }
    tokens_move(ts);
    return 0;
}

int parse_args(Tokens *ts, Elements *elements) {
    int ret;
    while (ts->current != NULL) {
        if (strcmp(ts->current, "--") == 0) {
            ret = parse_doubledash(ts, elements);
            if (!ret) break;
        } else if (ts->current[0] == '-' && ts->current[1] == '-') {
            ret = parse_long(ts, elements);
        } else if (ts->current[0] == '-' && ts->current[1] != '\0') {
            ret = parse_shorts(ts, elements);
        } else
            ret = parse_argcmd(ts, elements);
        if (ret) return ret;
    }
    return 0;
}

void ParseCommandLine(CommandLineArgs * commandLineArgs, Elements * elements, StructInfo * structInfo, int structInfoLength, void * commandStruct) {

    Tokens ts = tokens_new(commandLineArgs->argc, commandLineArgs->argv);
    if (parse_args(&ts, elements))
        printf("Parsing Failed");

    SubCommand *command;
    Option *option;
    int i;
    int j = 0;
    char annotatedName[ARGV_TOKEN_LEN];
    /* options */
    for (i=0; i < elements->n_options; i++) {
        option = &elements->options[i];
        for(j=0; j < structInfoLength; j++ ) {
            sprintf(annotatedName, "--%s", structInfo->memberInfo[j].name);
            if(!strcmp(annotatedName, option->olong)) {
                if(option->argcount && option->argument)
                {
                        memcpy((char *)commandStruct + structInfo->memberInfo[j].offset,
                           (char *)&option->argument, sizeof(char *));
                }
                else if(!option->argcount)
                    *(bool *)((bool *)commandStruct + structInfo->memberInfo[j].offset) = option->value;

                else
                    fprintf(stderr, "%s Argument missing", option->olong);

                char *test = (char *) ((char *)commandStruct + structInfo->memberInfo[j].offset);
                printf("Found %d for option %s\n", option->value, option->olong);
                if(option->argument) printf("Argument %s\n", option->argument);
                break;
            }
        }
    }
    /* commands */
    for (i=0; i < elements->n_commands; i++) {
        command = &elements->commands[i];
        for(j=0; j < structInfoLength; j++ ) {
            if(!strcmp( structInfo->memberInfo[j].name, command->name)) {
                *(bool *)((bool *)commandStruct + structInfo->memberInfo[j].offset) = command->value;
                int *test = (int *) ((int *)commandStruct + structInfo->memberInfo[j].offset);
                break;
            }
        }
    }
}
