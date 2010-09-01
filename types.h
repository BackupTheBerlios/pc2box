#ifndef _TYPES_H
#define _TYPES_H

//! linux_type
/*
typedef unsigned __int8         __u8;
typedef signed   __int8         __s8;
typedef signed   __int64        __s64;
typedef unsigned __int64        __u64;
typedef	signed   __int16	__s16;
typedef	unsigned __int16	__u16;
typedef	signed   __int32	__s32;

typedef	signed   __int64	__s64;
typedef	unsigned __int64	__u64;
*/
typedef	unsigned __int32	__u32;
//! SmartMpeg_type
typedef unsigned char  U8;
typedef unsigned short U16;
typedef unsigned long  U32;
typedef signed char    S8;
typedef signed short   S16;
typedef signed long    S32;

//! ucos_type
typedef unsigned char  INT8U;                    /* Unsigned  8 bit quantity                           */
typedef signed   char  INT8S;                    /* Signed    8 bit quantity                           */
typedef unsigned short INT16U;                   /* Unsigned 16 bit quantity                           */
typedef signed   short INT16S;                   /* Signed   16 bit quantity                           */
typedef unsigned int   INT32U;                   /* Unsigned 32 bit quantity                           */
typedef signed   int   INT32S;                   /* Signed   32 bit quantity                           */
typedef float          FP32;                     /* Single precision floating point                    */
typedef double         FP64;                     /* Double precision floating point                    */

#endif
