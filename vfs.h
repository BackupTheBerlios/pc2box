/*
 * vfs.h - pc2box virtual file system header
 *
 *  This code holds definitions required for the virtual file system layer for PVR file systems
 *
 *  This code was developed by tibor/LaSat and ported to Linux by metterschling
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
#ifndef __VFS__INCLUDE__
#define __VFS__INCLUDE__


#include <stdlib.h>
#include <stdio.h>

#if defined(_WIN32) || defined(_WIN64)

#ifdef __cplusplus
extern "C" {
#endif

#include <ddk/ntddk.h>
#include <ddk/ntdddisk.h>
#include <ddk/winddk.h>
#include <ddk/ntifs.h>

//#define DUPLICATE_SAME_ATTRIBUTES   0x00000004
//#pragma comment(lib,"ntdll.lib")

#ifdef __cplusplus
}
#endif

#include <io.h>

#endif // windows only

#ifndef true
#define true    TRUE
#endif

#ifndef false
#define false   FALSE
#endif

#include <time.h>
#include <string.h>
#include "types.h"

#if defined(_WIN32) || defined(_WIN64)
#define SECTOR_SIZE (VfsSys->DiskGeometry.BytesPerSector)
#else
// hard-code sector size on linux -> unlikely to change
#define SECTOR_SIZE 0x0200UL
#endif

typedef struct _vfs_filesys
{
    int                 flags;
    int                 blocksize;
    __u32               stride;
    __u32               umask;
    __u32               reserved[8];

    void *                  priv_data;

    BOOLEAN                 bFile;
    HANDLE                  MediaHandle;
#if defined(_WIN32) || defined(_WIN64)
    DISK_GEOMETRY           DiskGeometry;
    PARTITION_INFORMATION   PartInfo;
#endif
} VFS_FILESYS, *PVFS_FILESYS;


#define CREATE_NEW          1
#define CREATE_ALWAYS       2
#define OPEN_EXISTING       3
#define OPEN_ALWAYS         4
#define TRUNCATE_EXISTING   5
//#define INVALID_HANDLE_VALUE ((HANDLE)(LONG_PTR)-1)
//#define INVALID_FILE_SIZE ((DWORD)0xFFFFFFFF)
//#define INVALID_SET_FILE_POINTER ((DWORD)-1)
//#define INVALID_FILE_ATTRIBUTES ((DWORD)-1)

//! vfs stuff

#define VFS_INODEN_NAME_LEN     32
#define VFS_ROOT_ENTRIES        1000
#define VFS_TSPACK_SIZE         0x24B80
#define VFS_REC_SIZE            0x24C00


#if defined(_WIN32) || defined(_WIN64)
#define O_RDONLY        0x0000  //! open file read_only
#define O_WRONLY        0x0001  //! open file write_only
#define O_RDWR          0x0002  //! open file read_write
#define O_ATTR_MASK     0x0003  //! mask for attrib

#define O_OPEN          0x0100  //! open an existing file
#define O_CREAT         0x0200  //! create an file
#define O_OPEN_MASK     0x0300  //! mask for open
#endif

typedef enum{
    FAT_OK         = 0,
    FAT_BADPARAM,
    FAT_DEVICENOTSUPPORTED,
    FAT_ERROR_GEN,
    FAT_ALREADY_INITIALIZED,
    FAT_FILE_OPEN_ERROR,
    FAT_EOC,
    FAT_TABLE_ERROR,
    FAT_OUT_OF_SPACE,
    FAT_BAD_CLUSTER,
    FAT_RW_ERROR
} FAT_ERROR;

typedef enum {
    FILE_ENTRY = 0x10,
    DIR_ENTRY  = 0x20
} INODE_TYPE;

typedef enum {
    INODE_EOI  = 0xff,
    INODE_FREE = 0x00,
    INODE_BUSY = 0x01
} HD_VFS_INODE_STAT;

/* On-disk structure describing a file */
typedef struct {
    INT8U                    status;                          //! status
    INT8U                    type;                            //! dir file system
    INT8U                    attrib;                          //! --rw-- --ro--
    INT8U                    flServiceType;
    
    INT8U                    EntryName[VFS_INODEN_NAME_LEN];  //! name
//! file size
    INT32U                   sizecluster;                     //! size in cluster
    INT32U                   sizeinlastcluster;               //! bytesize in last cluster
//! bitmap pos
    INT32U                   FileBitmapSector;                //! pos of bitmap
    INT32U                   U8Offset;
    INT32U                   Bitmapsizeinbyte;
    
    INT32U                   FileEventMjd;
    INT32U                   LastRecTime;
} HD_VFS_INODE_DESC;

/* define maximum partition size (0x10 per GB partition size)
   old default v1:   0x1000 (256 GB)
   new default v2:   0x2000 (512 GB)
   Firmware >= 0139: 0x4000 (1 TB)
   now increasing to 4 TB max. partition size */
#define VFS_MAX_U32BITMAP_SIZE  0x10000

typedef struct{
    INT32U *pBitmap;
    INT32U  Bitmapbytelen;
    INT32U  firstfree;
    INT32U  lastallocated;
} VFS_BITMAP_BUFF;

typedef struct{
    INT16U                    stat;
    HD_VFS_INODE_DESC         Inode;
    VFS_BITMAP_BUFF           bmpfile;
    INT16U                    EntryIDX;
    INT32U                    lastCluster;
    
    //! location of fileinfo
    INT32U                    fileinfosector;
    INT16U                    fileinfosectorindex;
    //! act_read
    INT32U                    ReadActCluster;                 //! cluster offset
    INT32U                    ReadClusterByteOffset;          //! byte offset in cluster
    //! act_write
    INT32U                    WriteActCluster;                //! cluster offset
    INT32U                    WriteClusterOffset;             //! sector offset in cluster
} HD_VFS_HANDLER;

typedef struct{
    INT8U                     type;
    INT8U                     U8OfsTsToPictHead;     // Offset from Rec.Head to PictureHeader (HW: PictType:CaseInfo)
    INT32U                    OfsTsHead;
} HD_VFS_INDEX_INFO;

#define PIC_IDX_MAX               10
typedef struct{
    INT32U                    ActRecordNbr;          // 4
    INT32U                    RecHdTimeStamp;        // 4
    INT16U                    ActChId;               // 2
    INT16U                    ActVideoPid;           // 2
    INT16U                    ActAudioPid;           // 2
    INT16U                    ActAc3Pid;             // 2
    INT8U                     NbrOfHeads;            // 1
    HD_VFS_INDEX_INFO         IdxTable[PIC_IDX_MAX]; // old or new
    INT16U                    MarkLink;              // 2
    INT8U                     Linkused;              // 1
} HD_VFS_RECORD_INFO;

typedef enum{
    FILE_DEL   = 0,
    FILE_STORE,
    FILE_CLOSE
} HD_VFS_FILECLOSE;

typedef struct{
    BOOL                      Init;
    PVFS_FILESYS              VfsSys;
} HD_VFS_INIT;

typedef enum{
    MARK_NON      = 0x00,
    MARK_POS      = 0x01,
    MARK_LOOP     = 0x02,
    MARK_JUMP     = 0x04,
    MARK_FSTART   = 0x08,
    MARK_FEND     = 0x10,
    MARK_END      = 0x20,
    MARK_LASTVIEW = 0x40
} HD_VFS_MARK_TYPE;

/* On-disk structure */
#define VFS_MARKINFO_NAME_LEN     40
typedef struct{
    INT32U                    Idx;                                //! markIDX
    INT32U                    type;                               //! type of this mark
    //8
    INT8U                     markname[VFS_MARKINFO_NAME_LEN];    //! name
    //40
    INT32U                    ActRecordNbr;                       //! nbr of this record
    INT32U                    MarkCluster;                        //! vfs possition
    INT32U                    MarkClusterByteOffset;
    INT32U                    RecHdTimeStamp;                     //! recTime
    //16
} HD_VFS_MARK_INFO;

#define VFS_MAX_MARK_BY_ENTRY         512
#define VFS_MARK_LBA_BY_ENTRY         64
#define VFS_PC_VERSION_STR_LEN        512
#define VFS_PC_ACTVERSION             "LaSAT TransportStream v.001"

/* REC file header structure */
typedef struct{
    INT8U             ver[VFS_PC_VERSION_STR_LEN];      // version
    INT8U             Epg[512];                         // EPG
    HD_VFS_MARK_INFO  MarkInfo[VFS_MAX_MARK_BY_ENTRY];  // info system
} HD_VFS_PC_HEADER;


#define VFS_SIG                0x14233241

/* Original source: #define VFS_INFOSTR_SIZE 30
   So we are in trouble here: either support one structure or the other,
   but not both at the same time
*/
#define VFS_INFOSTR_SIZE       32

/* Original source: #define VFS_VERSION 0x00000100
   100 -> MarkInfo has random content and is being ignored
   200 -> MarkInfo must be filled with content */
#define VFS_VERSION            0x00000200

/* Original source: #define VFS_INFO_STR "LaSAT VideoFS V.100"
   But: This was never used by pc2box */
#define VFS_INFO_STR           "LaSAT VideoFS V.201"

/* On recordings from receiver, VFS_Table::ClusterSize 
   contains 0x000800 - what does this mean? */
#define VFS_CLUSTER_SIZE       0x100000
#define VFS_RESEVED_SECT       0x80
#define VFS_FILES_MAX_OPEN     2

#define SET_BIT(val,bitnum)    ((val) |=  (1<<(bitnum)))
#define CLR_BIT(val,bitnum)    ((val) &= ~(1<<(bitnum)))
#define IS_BIT_SET(val,bitnum) ((((val) & (1<<(bitnum))) == 0) ? 0 : 1 )
#define IS_BIT_CLR(val,bitnum) ((((val) & (1<<(bitnum))) == 0) ? 1 : 0 )

/* On-disk structure, similar to FAT boot sector */
typedef struct{
    INT32U                   vfs_sig;                             //! VFS_Sig
    INT8U                    FSInfo[VFS_INFOSTR_SIZE];            //! InfoString
    INT32U                   VFS_Version;                         //! Version
    INT32U                   SectorSize;                          //! LbaSectorsize in byte
    INT32U                   ClusterSize;                         //! size of each cluster in lba
    INT32U                   LbaSize;                             //! size of complete partition
    INT32U                   RsvLbaSect;                          //! lba's free after VFS_Table
    INT32U                   rootentrycounter;                    //! counter of entrys in root
    INT32U                   rootentryLbaSect;                    //! root entry sector
    //! freelist stuff
    INT32U                   freebitmapsec;                       //! lba of bitmap
    INT32U                   freelistsizeinbyte;                  //! size of this bitmap
    INT32U                   freelistsizeinlba;                   //! size of this bitmap in lba
    //! bitmap Info
    INT32U                   firstfreebyteoffset;                 //! byteoffset of first free inode
    INT8U                    Bitmaskoffirstfree;                  //! bitoffset
    INT32U                   freecounter;                         //! counter of free inodes
    INT32U                   numberoffilesinFS;                   //! conter of all files in partition
    INT32U                   rootentryLBA;                        //! lba of root_dir
    //! info system
    INT32U                   infoentryLBA;
    INT32U                   infoentrycounter;
    INT32U                   infosize;
    INT16U                   marksbyentry;
} VFS_Table, MBR;

typedef struct{
    VFS_Table                V_FAT;
    VFS_BITMAP_BUFF          FreeBitmap;
    INT8U                   *pSectorBuffer;
    INT32U                   ActSectorinBuffer;
    INT32U                   ActLbaSector;
    PVFS_FILESYS             VfsSys;
} VFS_VARS;

/* The following is the "magic" driver string required for raw
   disk access on Windows. It is of no use for Linux, but for the
   sake of common code, a dummy string is used
*/
#if defined(_WIN32) || defined(_WIN64)
#define DRIVER_MAGIC "0xdeadbeef"
#else
#define DRIVER_MAGIC "Not required for Linux"
#endif

#ifdef __cplusplus
extern "C" {
#endif
//! disk.c
PUCHAR   VfsStatusToString (IN NTSTATUS Status );
NTSTATUS VfsReadDisk(PVFS_FILESYS  VfsSys,ULONGLONG Offset,ULONG   Length,PVOID Buffer);
NTSTATUS VfsWriteDisk(PVFS_FILESYS  VfsSys,ULONGLONG  Offset,ULONG  Length,PVOID Buffer);
NTSTATUS VfsGetMediaInfo(PVFS_FILESYS VfsSys );
NTSTATUS VfsLockVolume(PVFS_FILESYS VfsSys );
NTSTATUS VfsUnLockVolume(PVFS_FILESYS VfsSys );
NTSTATUS VfsDisMountVolume(PVFS_FILESYS VfsSys );
NTSTATUS VfsOpenDevice(PVFS_FILESYS VfsSys,PUCHAR DeviceName );
NTSTATUS VfsCloseDevice( PVFS_FILESYS VfsSys);
int      VfsDevtoLetter(const char *DevName, const char *driver);
int      Unmount(const char *driver);
//! vfs.c
FAT_ERROR VFS_Open(HD_VFS_INIT *pOpen);
FAT_ERROR VFS_Mount(INT32U SecLba);
FAT_ERROR VFS_GetFileInfobyIndex(INT16U idx,HD_VFS_HANDLER *pHandler);
FAT_ERROR VFS_OpenFile(HD_VFS_HANDLER **pfile,INT8U *path,INT16U flag);
INT32U    VFS_GetNByte(HD_VFS_HANDLER **pfile,INT8U *pData,INT32U Size,FAT_ERROR *err);
FAT_ERROR VFS_SetMarkInfo(INT16U fileidx,HD_VFS_MARK_INFO *pInfo);
FAT_ERROR VFS_SetMarkInfoIDX(INT16U fileidx,INT16U markidx,HD_VFS_MARK_INFO *pInfo);
FAT_ERROR VFS_SetVFSRecordInfo(HD_VFS_MARK_INFO *pInfo,INT8U flLinkenable);
INT32U    VFS_PutNByte(HD_VFS_HANDLER **pfile,INT8U *pData,INT32U Size,FAT_ERROR *err);
FAT_ERROR VFS_CloseFile(HD_VFS_HANDLER *pfile,HD_VFS_FILECLOSE store);
INT32U    VFS_GetClusterSize();
// Read EPG from device
FAT_ERROR HD_VFS_GetEventInfobyFileIDX(INT16U FileIdx,INT8U *pData);
// Store EPG to device
FAT_ERROR HD_VFS_PutEventInfobyFileIDX(INT16U FileIdx,INT8U *pData);
// Remove CRTL-E (0x05) characters from text strings
void      stripCtrlE(char *entryName);
#ifdef __cplusplus
}
#endif


#endif //__VFS__INCLUDE__
