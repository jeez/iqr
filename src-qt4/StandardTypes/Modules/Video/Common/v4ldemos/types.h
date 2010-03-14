//---------------------------------------------------------------------------------
// types.h - Basic integer / byte types
//
// Author:  Ben Bridgwater
// History:
// 03/07/99 Initial version
//
//---------------------------------------------------------------------------------
#ifndef _TYPES_H
#define _TYPES_H
//---------------------------------------------------------------------------------

#include <sys/types.h>

//---------------------------------------------------------------------------------

#if (1)

typedef char      int1;
typedef short     int2;
typedef long      int4;
typedef long long int8;

typedef unsigned char      uint1;
typedef unsigned short     uint2;
typedef unsigned long      uint4;
typedef unsigned long long uint8;

#else

typedef int8_t    int1;
typede  int16_t   int2;
typedef int32_t   int4;
typedef int64_t   int8;

// Sun use uint8_t, etc.

typedef u_int8_t    uint1;
typede  u_int16_t   uint2;
typedef u_int32_t   uint4;
typedef u_int64_t   uint8;

#endif

#if (0)
typedef uint1 byte;
typedef uint2 word;
typedef uint4 dword;
typedef uint8 qword;
#endif

typedef uint1 bits8;
typedef uint2 bits16;
typedef uint4 bits32;
typedef uint8 bits64;

//---------------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------------
