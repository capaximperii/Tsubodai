//
// Created by 38803 on 8/5/2017.
//

#include "config.h"
#include "CommandLib/Console.h"

static Console * console = NULL;

// Callback after command.
void PostCommand()
{
    printf("\n%s ", console->promptchar);
    fflush(stdout);
    memset(console->buffer, 0x0, ARGV_TOKEN_LEN * ARGV_TOKEN_MAX);
    console->isSpacePressed = 0;
    console->bufCount = 0;
    console->cursorPosition = 0;
    console->historyIndex = 0;
}

// Callback after tabbed completion suggestions.
void PostSuggestion()
{
    printf("\r%s %s", console->promptchar, console->buffer);
    fflush(stdout);
}

// Add command to history.
void AddToHistory(char * command)
{
    if(console->historyCount == MAX_HISTORY)
    {
        int i = 0;
        free(console->history[0]);
        for(i=0;i<MAX_HISTORY - 1; i ++)
            console->history[i] = console->history[i+1];
        console->historyCount --;
    }
    char * copy = strdup(command);
    size_t i = 0, j = strlen(copy);
    while (j > 0 && isspace((unsigned char)copy[j - 1])) copy[--j] = '\0';
    while (isspace((unsigned char)copy[i])) i++;
    if (i > 0) memmove(copy, copy + i, j - i + 1);
    console->history[ console->historyCount ++ ] = copy;
}

// Load index from history.
void LoadFromHistory()
{
    Console * c = console;
    unsigned int index = console->historyCount - console->historyIndex;
    if(index >=0 && index < console->historyCount) {
        strcpy(console->buffer, console->history[index]);
        console->cursorPosition = console->bufCount = (int)strlen(console->buffer);
    }
}

// Get last word so far of user input.
static char * GetLastWord()
{
    static char buf[ ARGV_TOKEN_LEN ];
    char *first;
    memset(buf, 0x0, ARGV_TOKEN_LEN);
    strcpy(buf, console->buffer);
    if(console->isSpacePressed)
    {
        while((first = strchr((const char *)buf, ' ')) != NULL)
        {
            strcpy(buf, first + 1 ); // +1 to skip the leading space.
        }
    }
    return buf;
}

// Use commands to list directory that user has typed so far.
void PrintMatchingPaths()
{
    int index = 0;
    char * pathSoFar = GetLastWord();
    size_t len = strlen(pathSoFar);
    char buf[ ARGV_TOKEN_LEN ];
    char path[ ARGV_TOKEN_LEN ];
    printf("\n");
    char * dirListCmd = LS_COMMAND;
    memset(buf, 0x0, ARGV_TOKEN_LEN);
    memset(path, 0x0, ARGV_TOKEN_LEN);
    int i;
    for(i = 0; i < len; i ++ )
    {
        if(pathSoFar[i] == FILE_SEPARATOR)
            index = i;
    }
    strncpy(path, pathSoFar, index + 1);
    sprintf(buf, dirListCmd, path);
    system(buf);
    printf("\n");
}

// Is the user trying to input a path?
bool IsUserAccessingPath()
{
    bool ret = true;
    char * word = GetLastWord();
    size_t len = strlen(console->buffer);
    // Last character is space, so its next command argument and not a file path.
    if( len && isspace(console->buffer[ len - 1 ]) ) return false;

    #if !defined(_WIN32) && !defined(_WIN64)
        if ( word[0] != FILE_SEPARATOR && !strstr(word, "./") && !strstr(word, "../"))
            ret = false;
    #else
        if ( !strstr(word, ":\\") && !strstr(word, ".\\") && !strstr(word, "..\\"))
            ret = false;
    #endif

    return ret;
}

// Each character is analyzed and if it does not create an event like ctrl+c then put into buffer.
void AddToBuffer(unsigned char ch)
{
    memmove( console->buffer + console->cursorPosition + 1, console->buffer + console->cursorPosition, console->bufCount - console->cursorPosition );
    *(console->buffer + console->cursorPosition) = ch;
    console->bufCount ++;
    console->cursorPosition ++;
}

// Backspace to remove character from cursor position.
// TODO: There is no portable way to place a cursor, but we track cursor position internally, bad UI
void RemoveFromBuffer()
{
    if(console->bufCount && console->cursorPosition) {
        memmove( console->buffer + console->cursorPosition - 1, console->buffer + console->cursorPosition, console->bufCount - console->cursorPosition );
        *(console->buffer + console->bufCount - 1) = '\0';
        console->bufCount --;
        console->cursorPosition --;
    }
}

// Special key combination handler.
ConsoleAction KeyHandler(char c)
{
    unsigned char tmp;
    switch(c) {
        case '\n':
        case '\r':
        case EOF:
            console->buffer[console->bufCount++] = '\n';
            return CONSOLE_EXEC;
        case CTRL('c'):
        case CTRL('C'):
            return CONSOLE_BREAK;
        case CTRL('u'):
        case CTRL('U'):
            return CONSOLE_CLEAR;
        case '\t':
            if (IsUserAccessingPath()) return CONSOLE_FILE_SUGGEST;
            return CONSOLE_COMMAND_SUGGEST;
        case BACKSPACE:
        case LINUX_BACKSPACE:
            RemoveFromBuffer();
            break;
        case ESCAPE:
            if(feof(stdin))
                return CONSOLE_BREAK;
            tmp = _getch();
            tmp = _getch();
            if (tmp == 'A') {
                if(console->historyIndex < console->historyCount) console->historyIndex ++;
                return CONSOLE_HISTORY_LOAD;
            }
            else if(tmp == 'B') {
                if(console->historyIndex) console->historyIndex --;
                return CONSOLE_HISTORY_LOAD;
            }
            else if (tmp == 'C') {
            if(console->cursorPosition < console->bufCount) {
                    console->cursorPosition ++;
                }
            }
            else if(tmp == 'D') {
                if(console->cursorPosition > 0) {
                    console->cursorPosition --;
                }
            }
            break;
        case NUMLOCK_OFF:
        case DEDICATED_ARROW_KEYS_ESCAPE_CHAR:
            if(!feof(stdin)) {
                tmp = _getch();
                if (tmp == ARROW_UP_AFTER_ESCAPE) {
                    if(console->historyIndex < console->historyCount) console->historyIndex ++;
                    return CONSOLE_HISTORY_LOAD;
                }
                else if(tmp == ARROW_DOWN_AFTER_ESCAPE) {
                    if(console->historyIndex) console->historyIndex --;
                    return CONSOLE_HISTORY_LOAD;
                }
                else if (tmp == ARROW_RIGHT_AFTER_ESCAPE) {
                    if(console->cursorPosition < console->bufCount) {
                        console->cursorPosition ++;
                    }
                }
                else if(tmp == ARROW_LEFT_AFTER_ESCAPE) {
                    if(console->cursorPosition > 0) {
                        console->cursorPosition --;
                    }
                }
            }
            break;
        case ' ':
            if(!console->bufCount) break; // Dont encourage leading spaces.
                console->isSpacePressed ++;
        default:
            if(isprint(c)) {
                  AddToBuffer(c);
            }
        }
    printf("\r% *c", 80, ' '); // Clears line.
    fflush(stdout);
    printf("\r%s %s", console->promptchar, console->buffer);
    // Position the cursor.
    for(int cursor=0; cursor < console->bufCount - console->cursorPosition && console->bufCount > console->cursorPosition; cursor ++)
        CursorBackward();
    fflush(stdout);
    return CONSOLE_CONTINUE;
}

Console * SetupConsole()
{
    if(console)
        return console;
    console = (Console *) malloc(sizeof(Console));
    console->banner = "DM CLI for command line access to HDM and DI Commands."
                       "\n\tPress Tab for list of available commands and Completions.\n";
    console->buffer = (char *)malloc(ARGV_TOKEN_LEN * ARGV_TOKEN_MAX);
    console->promptchar = (char *) "DMI > ";
    console->isSpacePressed = false;
    console->onCommandExec = PostCommand;
    console->onKeyPress = KeyHandler;
    console->onHintShow = PostSuggestion;
    console->showCompletion = PrintMatchingPaths;
    console->AddToHistory = AddToHistory;
    console->LoadFromHistory = LoadFromHistory;
    console->bufCount = 0;
    console->historyCount = 0;
    console->historyIndex = 0;
    printf("%s\n", console->banner);
    console->onCommandExec();
    return console;
}

#if !defined(_WIN32) && !defined(_WIN64)
#warning "Overriding non standard getch."
// Non buffered STDIN for linux. Windows has getch.
int _getch()
{
  struct termios initial_settings, new_settings;
  unsigned char ch;
  tcgetattr(0,&initial_settings);
  new_settings = initial_settings;
  new_settings.c_lflag &= ~ICANON;
  new_settings.c_lflag &= ~ECHO;
  new_settings.c_lflag &= ~ISIG;
  new_settings.c_cc[VMIN] = 1;
  new_settings.c_cc[VTIME] = 0;
  tcsetattr(0, TCSANOW, &new_settings);
  ch = getchar();
  tcsetattr(0, TCSANOW, &initial_settings);
  return ch;
}

#endif

