/*!
 * ArgsDecl Export function to declare utilities that are to be used as common interface for parsing all command line options
 * from all commands.
 * The commands use a custom structure which is a combination of SubCommand, Option and Elements which are used by this library
 * to parse and provide information without the need for duplicate parsing in each module.
 *
 * It is based heavily on the docopt standards.
 *
 */
#ifndef ARGSDECL_H
#define ARGSDECL_H

#include "config.h"
#include "ArgsLib/Reflection.h"


/*!
 * A Command can have subcommands, those are described here.
 */
typedef struct {
  const char *name;     // subcommand name
  bool value;           // whether or not it was set by default.
} SubCommand;

/*!
 * Option that a Command accepts in the form of boolean (present or absent) or a string value.
 * If the expected argument is a number, then it has to be converted by the command plugin themselves.
 *
 */
typedef struct {
  const char *oshort;   // Short format such as ls -l
  const char *olong;    // Long format such as ls --list
  bool argcount;        // Number of arguments
  bool value;           // Whether this was set.
  char *argument;       // String value if any associated with this option.
} Option;

/*!
 * Structure to describe the command args in detail.
 *
 */
typedef struct {
  int n_commands;           // Number of sub commands
  int n_options;            // Number of options
  SubCommand *commands;     // Pointer to subcommands array
  Option *options;          // Pointer to options array
} Elements;


/*
 * Tokens object
 */
/*!
 * Structure used in the lib internally for command line parsing.
 *
 */
typedef struct Tokens {
  int argc;     // Number of command line arguments
  char **argv;  // Command line arguments in a character array
  int i;
  char *current;// Current token being parsed.
} Tokens;

/*!
 * Parses CommandLine arguments and places result into the commandStruct.
 */
void ParseCommandLine( CommandLineArgs *commandLineArgs, Elements *elements, StructInfo *structInfo, int structInfoLength, void *commandStruct );

/*!
 * Helper Macro that takes a custom plugin arg as opaque void * pointer and uses reflection to parse
 * command line struct and place values into the custom arg buffer depending on offsets.
 *
 */

#define MAGIC_COMMANDLINE_PARSER( commandLineArgs, elements, pluginCommandArgType, outBuffer) do {  \
    StructInfo structInfo;                                                                          \
    int structInfoLength = GET_STRUCT_MEMBERS(pluginCommandArgType, &structInfo);                   \
    ParseCommandLine(commandLineArgs, &elements, &structInfo, structInfoLength, &outBuffer);        \
  } while(0)

#endif // ARGSDECL_H
