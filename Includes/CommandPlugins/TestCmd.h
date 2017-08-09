#ifndef TESTCMD_H
#define TESTCMD_H

#include "ArgsLib/Reflection.h"

MANAGED_STRUCT( TestCmdArgs,
                /* commands */
                MEMBER( int, create );
                MEMBER( int, mine );
                MEMBER( int, move );
                MEMBER( int, remove );
                MEMBER( int, set );
                MEMBER( int, ship );
                MEMBER( int, shoot );
                /* options without arguments */
                MEMBER( int, drifting );
                MEMBER( int, help );
                MEMBER( int, moored );
                /* options with arguments */
                MEMBER( char *, speed );
                MEMBER( char *, name );
                MEMBER( char *, x );
                MEMBER( char *, y );
              )

#endif // TESTCMD_H
