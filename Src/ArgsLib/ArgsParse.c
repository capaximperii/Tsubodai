#include "ArgsLib/ArgsParse.h"

/* initialize empty argc/argv struct */
CommandLineArgs * ArgvInit()
{
  CommandLineArgs * commandLineArgs = (CommandLineArgs *) malloc(sizeof(CommandLineArgs));
  commandLineArgs->argc = 0;
  if ( ( commandLineArgs->_argv_token = calloc( ARGV_TOKEN_MAX, sizeof( char ) ) ) == NULL ) {
    err( 1, "argv_init: failed to calloc" );
  }
  memset( commandLineArgs->_argv_token, 0x0 ,ARGV_TOKEN_MAX * sizeof( char ) );
  return commandLineArgs;
}

/* add a character to the current token */
void ArgvTokenAddChar( CommandLineArgs *commandLineArgs, int c )
{
  size_t n;

  n = strlen( commandLineArgs->_argv_token );

  if ( n == ARGV_TOKEN_MAX - 1 ) {
    err( 1, "argv_token_addch: reached max token length (%d)", ARGV_TOKEN_MAX );
  }

  commandLineArgs->_argv_token[n] = c;
}

/* finish the current token: copy it into _argv and setup next token */
void ArgvTokenFinish(CommandLineArgs *commandLineArgs)
{
  if ( commandLineArgs->argc == ARGV_MAX ) {
    err( 1, "argv_token_finish: reached max argv length (%d)", ARGV_MAX );
  }
  /*STATUS("finishing token: '%s'\n", _argv_token);*/
  commandLineArgs->argv[ commandLineArgs->argc++ ] = commandLineArgs->_argv_token;
  if ( ( commandLineArgs->_argv_token = calloc( ARGV_TOKEN_MAX, sizeof( char ) ) ) == NULL ) {
    err( 1, "argv_token_finish: failed to calloc" );
  }
  memset( commandLineArgs->_argv_token, 0x0, ARGV_TOKEN_MAX * sizeof( char ) );
}

CommandLineArgs * ShlexParse(char *s)
{
  bool in_token;
  bool in_container;
  bool escaped;
  char container_start;
  char c;
  size_t  len;
  int  i;

  container_start = 0;
  in_token = false;
  in_container = false;
  escaped = false;

  len = strlen( s );

  CommandLineArgs *commandLineArgs = ArgvInit();

  for ( i = 0; i < len; i++ ) {
    c = s[i];

    switch ( c ) {
      /* handle whitespace */
      case ' ':
      case '\t':
      case '\n':
        if ( !in_token ) {
          continue;
        }

        if ( in_container ) {
          ArgvTokenAddChar( commandLineArgs, c );
          continue;
        }

        if ( escaped ) {
          escaped = false;
          ArgvTokenAddChar( commandLineArgs, c );
          continue;
        }

        /* if reached here, we're at end of token */
        in_token = false;
        ArgvTokenFinish(commandLineArgs);
        break;

      /* handle quotes */
      case '\'':
      case '\"':

        if ( escaped ) {
          ArgvTokenAddChar( commandLineArgs, c );
          escaped = false;
          continue;
        }

        if ( !in_token ) {
          in_token = true;
          in_container = true;
          container_start = c;
          continue;
        }

        if ( in_container ) {
          if ( c == container_start ) {
            in_container = false;
            in_token = false;
            ArgvTokenFinish(commandLineArgs);
            continue;
          } else {
            ArgvTokenAddChar( commandLineArgs, c );
            continue;
          }
        }

        /* XXX in this case, we:
         *    1. have a quote
         *    2. are in a token
         *    3. and not in a container
         * e.g.
         *    hell"o
         *
         * what's done here appears shell-dependent,
         * but overall, it's an error.... i *think*
         */
        printf( "Parse Error! Bad quotes\n" );
        break;

      case '\\':

        if ( in_container && s[i + 1] != container_start ) {
          ArgvTokenAddChar( commandLineArgs, c);
          continue;
        }

        if ( escaped ) {
          ArgvTokenAddChar( commandLineArgs, c );
          continue;
        }

        escaped = true;
        break;

      default:
        if ( !in_token ) {
          in_token = true;
        }

        ArgvTokenAddChar( commandLineArgs, c );
    }
  }

  if ( in_container ) {
    printf( "Parse Error! Still in container\n" );
  }

  if ( escaped ) {
    printf( "Parse Error! Unused escape (\\)\n" );
  }

  return commandLineArgs;
}

void printArgs(CommandLineArgs * commandLineArgs)
{
    int i;
    for(i=0; i< commandLineArgs->argc; i++ ) {
        printf("\n%s", commandLineArgs->argv[i]);
    }
    printf("\n");
}

void cleanArgs(CommandLineArgs * commandLineArgs) {
    int i;
    for(i=0; i< commandLineArgs->argc; i++ ) {
        free(commandLineArgs->argv[i]);
    }
    free(commandLineArgs->_argv_token);
    free(commandLineArgs);
  *(&commandLineArgs) = NULL;
}

