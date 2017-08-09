#ifndef CONFIG_H
#define CONFIG_H

#include<stdbool.h>
#include<string.h>
#include<ctype.h>

/* Reflection requires no packing. */
#pragma pack(push, 1)
#pragma warning(disable: 4103)
#pragma warning(disable: 4996)

/* Args Parser Config */
#define ARGV_MAX  255
#define ARGV_TOKEN_MAX  255
#define ARGV_TOKEN_LEN  255

/* Commands Lib Config */
#define MAX_HISTORY 256

#define err(msg,...)

#endif // CONFIG_H
