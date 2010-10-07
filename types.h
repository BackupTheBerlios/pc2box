/*
 * types.h - pc2box type abstraction header
 *
 *  This code defines types required to access hardware-defined structures
 *
 *  See the AUTHORS file for the list of contributors
 *
 * This file is part of pc2box, a free C++ PVR file retrieval utility
 *
 *
 * This program is free software; you can redistribute it and/or modify 
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */
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
// TODO: check for 32 bitness on 64-bit targets
typedef	unsigned int	__u32;

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

#if !defined(_WIN32) && !defined(_WIN64)
typedef unsigned long long ULONGLONG;
typedef unsigned long ULONG;
typedef void * PVOID;
typedef char * PUCHAR;
typedef int NTSTATUS;
typedef int BOOLEAN;
typedef int BOOL;
typedef int HANDLE;
typedef unsigned long long __int64;
#define STATUS_SUCCESS 0
#define IN
#define NT_SUCCESS(x) (x!=-1)
#endif

#endif
