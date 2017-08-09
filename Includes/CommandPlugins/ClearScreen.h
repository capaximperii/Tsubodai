//
// Created by 38803 on 8/5/2017.
//

#ifndef DMCLI_CLEARSCREEN_H
#define DMCLI_CLEARSCREEN_H

#include "ArgsLib/Reflection.h"

#if !defined(_WIN32) && !defined(_WIN64)
#define CLEAR_COMMAND   "clear"
#else
#define CLEAR_COMMAND   "@cls"
#endif

MANAGED_STRUCT( ClearScreenArgs,
                /* commands */
                /* special */
                MEMBER( int, help );
              )


#endif //DMCLI_CLEARSCREEN_H
