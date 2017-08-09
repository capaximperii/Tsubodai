/*!
 * Inspired from Java Reflection which makes it possible to inspect classes, interfaces, fields
 * and methods at runtime, without knowing the names of the classes, methods etc. at compile time.
 * Even though in C we do all the metadata collection using macros at compile time.
 *
 */
#ifndef REFLECTION_H
#define REFLECTION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"

/*!
 * Holds meta data information for each member of a managed struct.
 */
typedef struct {
  size_t size;
  size_t offset;
  char name[ARGV_TOKEN_LEN];
} MemberInfo;

/*!
 * A Struct consists of many Members, this is the corresponding Struct for member Info.
 */
typedef struct {
  MemberInfo memberInfo[ARGV_TOKEN_MAX];
} StructInfo;

/*!
 * Allows to get struct members to iterate over.
 * This is meant not to be called directly, instead using the helper macros.
 */
int GetStructMembers( const char *members, StructInfo *structInfo );

/*!
 * Macro routines to generate meta-data for structs.
 */
#define MEMBER(TYPE,NAME) TYPE NAME

/*!
 * A Structure defined via managed struct mechanism, allows storage of metadata.
 */

#define MANAGED_STRUCT(NAME,MEMBERS) \
  typedef struct NAME { \
    MEMBERS \
  } NAME; \
  const char* const NAME##_Members = #MEMBERS;

/*!
 * Helper macro for GetStructMembers
 */
#define GET_STRUCT_MEMBERS(NAME, buffer) GetStructMembers(NAME##_Members, buffer)

#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))

#endif // REFLECTION_H
