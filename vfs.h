#ifndef __VFS__INCLUDE__
#define __VFS__INCLUDE__


#include <stdlib.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <ntddk.h>
#include <NTDDDISK.H>
#include <NTDDDISK.H>
#include <winddk.H>
#include <ntifs.H>

//#define DUPLICATE_SAME_ATTRIBUTES   0x00000004
//#pragma comment(lib,"ntdll.lib")

#ifdef __cplusplus
}
#endif

#include "time.h"
#include "io.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "types.h"

#ifndef true
#define true    TRUE
#endif

#ifndef false
#define false   FALSE
#endif


#define SECTOR_SIZE (VfsSys->DiskGeometry.BytesPerSector)

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
    DISK_GEOMETRY           DiskGeometry;
    PARTITION_INFORMATION   PartInfo;

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
#define VFS_ROOT_ENTRYS         1000
#define VFS_TSPACK_SIZE         0x24B80
#define VFS_REC_SIZE            0x24C00



#define O_RDONLY        0x0000  //! open file read_only
#define O_WRONLY        0x0001  //! open file write_only
#define O_RDWR          0x0002  //! open file read_write
#define O_ATTR_MASK     0x0003  //! mask for attrib

#define O_OPEN          0x0100  //! open an existing file
#define O_CREAT         0x0200  //! create an file
#define O_OPEN_MASK     0x0300  //! mask for open

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

}FAT_ERROR;

typedef enum {

       FILE_ENTRY = 0x10,
       DIR_ENTRY  = 0x20

}INODE_TYPE;

typedef enum {

       INODE_EOI   = 0xff,
       INODE_FREE  = 0x00,
       INODE_BUSSY = 0x01

}HD_VFS_INODE_STAT;

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

}HD_VFS_INODE_DESC;

typedef struct{

        INT32U *pBitmap;
        INT32U  Bitmapbytelen;
        INT32U  firstfree;
        INT32U  lastallocated;

}VFS_BITMAP_BUFF;

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

}HD_VFS_HANDLER;

typedef struct{

        INT8U                     type;
        INT8U                     U8OfsTsToPictHead;     // Offset from Rec.Head to PictureHeader (HW: PictType:CaseInfo)
        INT32U                    OfsTsHead;

}HD_VFS_INDEX_INFO;

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

}HD_VFS_RECORD_INFO;

typedef enum{

        FILE_DEL   = 0,
        FILE_STORE,
        FILE_CLOSE

}HD_VFS_FILECLOSE;

typedef struct{

        BOOL                      Init;
        PVFS_FILESYS              VfsSys;

}HD_VFS_INIT;

typedef enum{

        MARK_NON      = 0x00,
        MARK_POS      = 0x01,
        MARK_LOOP     = 0x02,
        MARK_JUMP     = 0x04,
        MARK_FSTART   = 0x08,
        MARK_FEND     = 0x10,
        MARK_END      = 0x20,
        MARK_LASTVIEW = 0x40

}HD_VFS_MARK_TYPE;

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

}HD_VFS_MARK_INFO;

#define VFS_MAX_MARK_BY_ENTRY         512
#define VFS_MARK_LBA_BY_ENTRY         64
#define VFS_PC_VERSION_STR_LEN        512
#define VFS_PC_ACTVERSION             "LaSAT TransportStream v.001"

typedef struct{

        INT8U             ver[VFS_PC_VERSION_STR_LEN];      // version
        INT8U             Epg[512];                         // EPG
        HD_VFS_MARK_INFO  MarkInfo[VFS_MAX_MARK_BY_ENTRY];  // info system

}HD_VFS_PC_HEADER;

#ifdef __cplusplus
extern "C" {
#endif
//! Disk.c
PUCHAR   VfsStatusToString (IN NTSTATUS Status );
NTSTATUS VfsReadDisk(PVFS_FILESYS  VfsSys,ULONGLONG Offset,ULONG   Length,PVOID Buffer);
NTSTATUS VfsWriteDisk(PVFS_FILESYS  VfsSys,ULONGLONG  Offset,ULONG  Length,PVOID Buffer);
NTSTATUS VfsGetMediaInfo(PVFS_FILESYS VfsSys );
NTSTATUS VfsLockVolume(PVFS_FILESYS VfsSys );
NTSTATUS VfsUnLockVolume(PVFS_FILESYS VfsSys );
NTSTATUS VfsDisMountVolume(PVFS_FILESYS VfsSys );
NTSTATUS VfsOpenDevice(PVFS_FILESYS VfsSys,PUCHAR DeviceName );
NTSTATUS VfsCloseDevice( PVFS_FILESYS VfsSys);
NTSTATUS VfsDevtoLetter(char *DevName, char *driver);
int      Unmount(char *driver);
void     VfsRemoveLetter(char *driver);
//! debug.c
void DEBUG_DumpData(U8 *ptData,U32 Size );
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
FAT_ERROR HD_VFS_GetEventIngobyFileIDX(INT32U EntryCluster,INT16U FileIdx,INT8U *pData);
FAT_ERROR HD_VFS_PutEventIngobyFileIDX(INT32U EntryCluster,INT16U FileIdx,INT8U *pData);
FAT_ERROR VFS_ReinitVFSRecordList(HD_VFS_HANDLER *pfile,INT16U MarkIdx);
//! main.cpp
int       main_test_dev();
FAT_ERROR main_StoreEventInfo(INT8U* pEpg);
int       main_init_vfs(void* text);
int       main_dir_vfs(void * list);
FAT_ERROR main_ItemSelectToStore(void* Item,double* LastRecord,char *pEpg);
INT8U*    main_GetRecord(FAT_ERROR *err);
INT8U*    main_PutRecord(void* input,FAT_ERROR *err);
FAT_ERROR main_CloseStore();
void      Maindebug(char* str);
void      print_to_output(char *str,void *output);
FAT_ERROR CreateVFSEntry(char* name);
__int64   FileSize64( const char * szFileName);
#ifdef __cplusplus
}
#endif


#endif //__VFS__INCLUDE__
