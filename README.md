# Tsubodai

### What is Tsubodai?

Trick question! Its not a thing, its a [person](https://en.wikipedia.org/wiki/Subutai).

As one of the greatest commander in history, he lends his name to this command line framework.


Basic useful feature list:

 * C portable code that runs on windows and linux.
 * No external dependency
 * Tabbed completion
 * Options parsing abstracted into the framework
 * Commands register as plugins that are statically linked into the appllication but use compiler magic to stay decoupled from the main engine.
 * File name tabbed suggestions like on linux bash command shell.
 * History of commands in memory.


### How to write a Command plugin?

The following snippet shows a file added into CommandPlugins directory, which contains a function (SetupClearScreen) decorated with the INITIALIZER macro. This takes care of registering the command to the framework.

The command name is the name to type in the shell to access this command.

The usage pattern is unix style usage pattern string to help a user explain usage.

The callback is the entry point that is invoked.

```c
INITIALIZER(SetupClearScreen)
{
    CommandRegistry * command = (CommandRegistry *)malloc(sizeof(CommandRegistry));
    command->commandName = "Clear";
    command->usagePattern = help_message;
    command->CallBack = runner;
    RegisterCommand(command);
}
```

Clearly, each command has a different input parameter list and command line options. There are helper macros provided to ease the parsing pain too.
For example the above clear screen has a parameter of type boolean. This value is set or unset, depending on the command line parameters user typed in.

```c
MANAGED_STRUCT( ClearScreenArgs,
                /* commands */
                /* special */
                MEMBER( int, help );
                MEMBER ( char *, dummy );
              )

    ClearScreenArgs clearScreenArgs = {
            .help = 0,
            .dummy = (char *) "I should think of better examples."
    };

```
Notice the value dummy is assigned, is the default value in case the user has not supplied it and dummy is an optional parameter. 


But we have not yet got to explaining how to associate command line arguments to this struct, It is achieved simply by assigning a short-option, long option string to the command options like so:

```c
    SubCommand commands[] = {
            {"create", 0},
            {"shoot", 0}
    };

    Option options[] = {
            {"-h", "--help", 0, 0, NULL},
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


```

Subcommands are commands within commands, for very readable CLI codes, but they are not mandatory to have.

Options contain a short code (-h) and a long code (--help) and associated and they are automatically parsed using metadata.

Element is a sort of enclosure that contains the options and the subcommands to help with the parsing.

And we are done! :+1:



### Stuff used to make this:

 * [Docopt](https://github.com/docopt) for inspiration of really neat command line designs.
 * Nathan Miller for meta-data suggestion.
 * Extra long inter-continental flight with no movies to force me to sit down and do this.
 
