#ifndef COMMANDHELPER_H
#define COMMANDHELPER_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "ArgsLib/ArgsParse.h"

/*!
 * Helper macros, that allow automatic initialization code for command plugins to run before main.
 * Typically used for command registration to the framework.
 */
#ifdef __cplusplus
#define INITIALIZER(f) \
  static void f(void); \
  struct f##_t_ { f##_t_(void) { f(); } }; static f##_t_ f##_; \
  static void f(void)
#elif defined(_MSC_VER)
#pragma section(".CRT$XCU",read)
#define INITIALIZER2_(f,p) \
  static void f(void); \
  __declspec(allocate(".CRT$XCU")) void (*f##_)(void) = f; \
  __pragma(comment(linker,"/include:" p #f "_")) \
  static void f(void)
#ifdef _WIN64
#define INITIALIZER(f) INITIALIZER2_(f,"")
#else
#define INITIALIZER(f) INITIALIZER2_(f,"_")
#endif
#else
#define INITIALIZER(f) \
  static void f(void) __attribute__((constructor)); \
  static void f(void)
#endif


typedef enum {
  COMMAND_OK,
  BAD_ARGUMENTS,
  COMMAND_FAILED
} CommandResult;
/*!
 * Command main routine callback.
 */
typedef CommandResult( *CommandRunner )( CommandLineArgs *args );

/*!
 * Command Registry holds list of all commands registered.
 */
typedef struct CommandRegistry {
  struct CommandRegistry *head;
  char *commandName;
  const char *usagePattern;
  unsigned int commandCount;
  CommandRunner CallBack;
  struct CommandRegistry *next;
} CommandRegistry;

/*!
 * Register Command to the framework.
 */
CommandResult RegisterCommand( CommandRegistry *newCommand );
/*!
 * Find a Command by name.
 */
CommandRegistry *findCommand( char *name );
/*!
 * Get Command registry list to iterate over.
 */
CommandRegistry *GetCommandRegistry();
/*!
 * Print all registered commands, optionally matching the startsWith filter string.
 */
void PrintRegistry( char *startsWith );
/*!
 * Each command themselves can have suggestion for usages gleaned from usagePattern in command registry.
 */
void CommandSuggestions( const char *usagePattern, unsigned int nSpaces );

#endif // COMMANDHELPER_H
