/*!
 * ArgsParse library implements Unix shell style parsing of arguments into argc and argv[]
 * to be consumed by each command plugin.
 *
 */
#ifndef ARGSPARSE_H
#define ARGSPARSE_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "config.h"

/*!
 * CommandLineArgs struct that contain references to argc and argv.
 *
 */

typedef struct {
  int    argc;
  char  *argv[ARGV_MAX];
  char  *_argv_token;
} CommandLineArgs;


/*!
 * Helper routine to parse a line into unix style argc argv values.
 */
CommandLineArgs *ShlexParse( char *s );

/*!
 * Prints command line arguments to the stdout.
 */
void printArgs( CommandLineArgs *commandLineArgs );

/*!
 * Memory freeing for the parsed args, called after command execution.
 */
void cleanArgs( CommandLineArgs *commandLineArgs );

#endif // ARGSPARSE_H
