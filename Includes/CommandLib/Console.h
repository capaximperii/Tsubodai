//
// Created by 38803 on 8/5/2017.
//

#ifndef DMCLI_CONSOLE_H_H
#define DMCLI_CONSOLE_H_H

#include <stdarg.h>

#include "config.h"
#include "CommandHelper.h"

#if !defined(_WIN32) && !defined(_WIN64)
#include<termios.h>
int _getch();
#define FILE_SEPARATOR '/'
#define LS_COMMAND      "ls %s"
#else
#include <conio.h>
#define FILE_SEPARATOR '\\'
#define LS_COMMAND      "@dir /b %s"
#endif



#define CTRL(c) (c - '@')
#define ESCAPE 27
#define NUMLOCK_OFF 0
#define DEDICATED_ARROW_KEYS_ESCAPE_CHAR -32 //224?
#define BACKSPACE '\b'
#define LINUX_BACKSPACE 127

#define ARROW_UP_AFTER_ESCAPE 72
#define ARROW_LEFT_AFTER_ESCAPE 75
#define ARROW_RIGHT_AFTER_ESCAPE 77
#define ARROW_DOWN_AFTER_ESCAPE 80

/*!
 * Console action return values for main loop.
 */
typedef enum {
  CONSOLE_CONTINUE,
  CONSOLE_BREAK,
  CONSOLE_COMMAND_SUGGEST,
  CONSOLE_FILE_SUGGEST,
  CONSOLE_CLEAR,
  CONSOLE_HISTORY_LOAD,
  CONSOLE_EXEC
} ConsoleAction;

/*!
 * Callback types for different events supported by this console.
 */
typedef ConsoleAction( *OnKeyPress )( char c );
typedef void ( *OnCommandExec )();
typedef void ( *OnHintShow )();
typedef void ( *FileAutoComplete )();
typedef void ( *HistoryAppend )();
typedef void ( *HistoryLoad )();

/*!
 * Declares Console and bindings to callbacks.
 */
typedef struct  {
  char *banner;
  char *history[MAX_HISTORY];
  char *promptchar;
  char *buffer;
  unsigned short isSpacePressed;
  OnKeyPress onKeyPress;
  OnCommandExec onCommandExec;
  OnHintShow onHintShow;
  FileAutoComplete showCompletion;
  HistoryAppend AddToHistory;
  HistoryLoad LoadFromHistory;
  unsigned int cursorPosition;
  unsigned int bufCount;
  unsigned int historyCount;
  unsigned int historyIndex;
} Console;
/*!
 * Initializes console with specific banner, prompt and event callbacks.
 */
Console *SetupConsole();

#endif //DMCLI_CONSOLE_H_H
