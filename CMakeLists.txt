cmake_minimum_required(VERSION 3.8)
project(DmCli)

set(CMAKE_CXX_STANDARD 11)

include_directories(Includes)

set(SOURCE_FILES
        Src/ArgsLib/ArgsDecl.c
        Src/ArgsLib/ArgsParse.c
        Src/ArgsLib/Reflection.c
        Src/CommandPlugins/TestCmd.c
        Src/CommandLib/CommandHelper.c
        main.c Includes/CommandLib/Console.h Src/CommandLib/Console.c Src/CommandPlugins/ClearScreen.c Includes/CommandPlugins/ClearScreen.h)

add_executable(DmCli ${SOURCE_FILES})