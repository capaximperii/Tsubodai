#include <ctype.h>
#include <ArgsLib/Reflection.h>
#include "ArgsLib/Reflection.h"

int GetStructMembers(const char * members, StructInfo * structInfo) {
    int i = 0;
    size_t offset = 0;
    char *token;
    char * copy = strdup(members);

    /* get the first token */
    token = strtok(copy, ";");
    /* walk through other tokens */
    while( token != NULL )
    {
        char type[ARGV_TOKEN_LEN];
        while(isspace(token[0])) token ++;
        sscanf(token, "MEMBER( %[0-9a-zA-Z* ], %[0-9a-zA-Z] )", type, structInfo->memberInfo[i].name);
        if(strstr(type, "*"))
            structInfo->memberInfo[i].size = sizeof(void *);
        else if(strstr(type, "int"))
            structInfo->memberInfo[i].size = sizeof(int);
        else if(strstr(type, "char"))
            structInfo->memberInfo[i].size = sizeof(char);
        else if(strstr(type, "float"))
            structInfo->memberInfo[i].size = sizeof(float);
        else if(strstr(type, "double"))
            structInfo->memberInfo[i].size = sizeof(double);
        else
            structInfo->memberInfo[i].size = sizeof(void *);
        structInfo->memberInfo[i].offset = offset;
        offset += structInfo->memberInfo[i].size;

//        TODO: Struct has no way to declare array and therefore no parsing yet
//        if(strstr(type, "["))
//        {
//            int array_len = 0;
//            sscanf(type, "[%99[^]]]", &array_len);
//            structInfo->memberInfo[i].size = structInfo->memberInfo[i].size * array_len;
//        }

        i ++;
        token = strtok(NULL, ";");
    }
    free(copy);
    return i;
}
