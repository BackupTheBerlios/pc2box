/*
 * vfs.c - pc2box virtual file system
 *
 *  This code implements a virtual file system layer for PVR file systems
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

/* INCLUDES **************************************************************/
#include "vfs.h"

#if !defined(_WIN32) && !defined(_WIN64)
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#endif

/*-------------------------------------------------------------------*/
/* local VAR_S                                                               */
/*---------------------------------------------------------------------------*/
#define VFS_MAX_U32BITMAP_SIZE  0x2000
static VFS_VARS         Drive;
static INT8U            SectorBuffer[512];
static HD_VFS_HANDLER   VFSHandler[VFS_FILES_MAX_OPEN];
static INT32U           BitMapBuf[VFS_MAX_U32BITMAP_SIZE];

/*---------------------------------------------------------------------------*/
/* local defs                                                                */
/*---------------------------------------------------------------------------*/

#if defined(_WIN32) || defined(_WIN64)
#undef SECTOR_SIZE
#define SECTOR_SIZE Drive.VfsSys->DiskGeometry.BytesPerSector
#endif

#define READ_SECTOR(x)         NT_SUCCESS(VfsReadDisk(Drive.VfsSys, \
                                          (ULONGLONG)((ULONGLONG)(Drive.ActSectorinBuffer=x)<<9)/**(ULONGLONG)Drive.VfsSys->DiskGeometry.BytesPerSector)*/, \
                                          (ULONG) SECTOR_SIZE, \
                                           Drive.pSectorBuffer))


#define WRITE_SECTOR(x)        NT_SUCCESS(VfsWriteDisk(Drive.VfsSys, \
                                          (ULONGLONG)((ULONGLONG)(Drive.ActSectorinBuffer=x)<<9), \
                                          (ULONG) SECTOR_SIZE, \
                                           Drive.pSectorBuffer))

#define READ_SECTORS(x,d,n)    NT_SUCCESS(VfsReadDisk(Drive.VfsSys, \
                                          (ULONGLONG)((ULONGLONG)x<<9) /**(ULONGLONG)Drive.VfsSys->DiskGeometry.BytesPerSector)*/, \
                                          (ULONG)(n*SECTOR_SIZE), \
                                          d))

#define WRITE_SECTORS(x,d,n)   NT_SUCCESS(VfsWriteDisk(Drive.VfsSys, \
                                          (ULONGLONG)((ULONGLONG)x<<9), \
                                          (ULONG)(n*SECTOR_SIZE), \
                                          d))

#define FLUSH_SECTOR           NT_SUCCESS(VfsWriteDisk(Drive.VfsSys, \
                                          (ULONGLONG)((ULONGLONG)(Drive.ActSectorinBuffer)<<9), \
                                          (ULONG) SECTOR_SIZE, \
                                           Drive.pSectorBuffer))


#define CTRLE 0x05

void stripCtrlE(char *entryName)
{
    char *tmpBuffer = (char *)malloc(strlen(entryName)+1);
    size_t i;
    int j = 0;
    for (i=0; i<strlen(entryName); i++)
        if (entryName[i] != CTRLE) {
            tmpBuffer[j] = entryName[i];
            j++;
        }
    tmpBuffer[j] = 0;

    //printf("\nEntryName[0] <%x>, tmpBuffer[0] <%x>", entryName[0], tmpBuffer[0]);
    
    strcpy(entryName, tmpBuffer);

    free(tmpBuffer);
}

static INT8U VFS_Enter(void)
{
    INT8U err = 0;
     if(!NT_SUCCESS(VfsLockVolume(Drive.VfsSys))){
         err  = 1;
         printf("\nVolLockError!!");
        }
    return err;
}

static INT8U VFS_Exit(void)
{
    INT8U err = 0;
     if(!NT_SUCCESS(VfsUnLockVolume(Drive.VfsSys))){
            err  = 1;
            printf("\nVolunlockError!!");
          }
    return err;
}

static FAT_ERROR hd_vfs_setbit(INT8U *bmap, INT32U bit)
{
    INT32U  mask;
    mask  = bit >> 3;
    bmap += mask;
    mask  = bit & 0x07;
    SET_BIT(*bmap,mask);
    return FAT_OK;
}

static FAT_ERROR hd_vfs_clearbit(INT8U *bmap, INT32U bit)
{
    INT32U  mask;
    mask  = bit >> 3;
    bmap += mask;
    mask  = bit & 0x07;
    CLR_BIT(*bmap,mask);
    return FAT_OK;
}

static INT8U  nibblemap[] = {4, 3, 3, 2, 3, 2, 2, 1, 3, 2, 2, 1, 2, 1, 1, 0};
static INT32U hd_vfs_count_free(INT8U *bmap,INT32U len)
{
    INT32U i,sum = 0;
    if (!bmap)return 0;
    for (i = 0; i < len; i++)
        sum += nibblemap[bmap[i] & 0xf] + nibblemap[(bmap[i] >> 4) & 0xf];
    return sum;
}

static INT32U hd_vfs_find_first_zero(INT8U *bmap,INT32U Dwordlen,FAT_ERROR *err)
{
    INT32U i,ix;
    INT8U  tmp;
    *err = FAT_OK;
    if (!bmap)return 0xffffffff;
    for(ix = 0; ix < Dwordlen; ix++)
        {
            tmp = bmap[ix];
            //printf("\n 0x%x",tmp);
            for(i = 0; i<8;i++)
                {
                    if (!(tmp & (1 << i)))
                        {
                            return ((ix<<3)+i);
                        }
                }
        }
    *err = FAT_OUT_OF_SPACE;
    //! nothing is free
    return 0xffffffff;
}

static INT32U hd_vfs_find_first_one(INT8U *bmap,INT32U Startbit,INT32U Dwordlen,FAT_ERROR *err)
{
    INT32U i,ix;
    INT8U  tmp;
    *err = FAT_OK;

    if (!bmap){
        *err = FAT_OUT_OF_SPACE;
        return 0xffffffff;
    }
    for(ix = 0; ix < Dwordlen; ix++)
        {
            tmp       = bmap[ix];
            for(i = 0; i<8; i++)
                {
                    if (IS_BIT_SET(tmp,i))
                        {
                            if(((ix<<3)+i) > Startbit)
                                return ((ix<<3)+i);
                        }
                }
        }
    *err = FAT_OUT_OF_SPACE;
    //! nothing is free
    return 0xffffffff;
}

static INT32U hd_vfs_find_last_one(INT8U *bmap,INT32U Startbit,INT32U Dwordlen,FAT_ERROR *err)
{
    INT32U i,ix,is=0;
    INT8U  tmp;
    *err = FAT_OK;
    if (!bmap)return 0xffffffff;
    i    = Startbit;
    for(ix = 0; ix < Dwordlen; ix++)
        {
            tmp   = *bmap--;
            while(i--)
                {
                    is++;
                    if(IS_BIT_SET(tmp,i))
                        return is;
                }
            i = 8;
        }
    *err = FAT_OUT_OF_SPACE;
    //! nothing is free
    return 0xffffffff;
}

static FAT_ERROR hd_vfs_getnextcluster(HD_VFS_HANDLER *pfile)
{
    FAT_ERROR err = FAT_OK;
    INT8U *pU8;
    INT32U Cluster,len,Startbit,tmp;

    tmp      = pfile->ReadActCluster >> 3;
    pU8      = (INT8U*)(pfile->bmpfile.pBitmap);
    pU8      = &pU8[tmp];
    len      = pfile->bmpfile.Bitmapbytelen-tmp;
    tmp    <<= 3;
    Startbit = pfile->ReadActCluster & 0x07;
    Cluster  = hd_vfs_find_first_one(pU8,Startbit,len,&err);
    if(err  != FAT_OK)return err;
    Cluster              += tmp;
    pfile->ReadActCluster = Cluster;
    //printf("\n Cluster 0x%x\n",Cluster);

    return err;
}

static FAT_ERROR hd_vfs_getendcluster(HD_VFS_HANDLER *pfile)
{
    FAT_ERROR err = FAT_OK;
    INT8U *pU8;
    INT32U Cluster,len,Startbit,tmp;
    while(1)
        {
            tmp      = pfile->lastCluster >> 3;
            pU8      = (INT8U*)(pfile->bmpfile.pBitmap);
            pU8      = &pU8[tmp];
            len      = pfile->bmpfile.Bitmapbytelen-tmp;
            tmp    <<= 3;
            Startbit = pfile->lastCluster & 0x07;
            Cluster  = hd_vfs_find_first_one(pU8,Startbit,len,&err);
            if(err  != FAT_OK)return FAT_OK;
            Cluster += tmp;
            pfile->lastCluster = Cluster;
        }
    return FAT_EOC;
}

static FAT_ERROR hd_vfs_resetFileMarkList(INT16U idx)
{
    INT32U off,ix;
    if(!Drive.V_FAT.infoentryLBA){
        printf("\n mark not supported!!!");
        return FAT_DEVICENOTSUPPORTED;
    }
    printf("\n Reset MarkListInfo 0x%04x",idx);
    // byte by entry
    off = Drive.V_FAT.marksbyentry*sizeof(HD_VFS_MARK_INFO);
    // LBA by entry
    off = off>>9;
    // read first setor
    if (!READ_SECTOR(Drive.V_FAT.infoentryLBA+(off*idx)))
        return FAT_RW_ERROR;
    memset(Drive.pSectorBuffer,0x00,Drive.V_FAT.SectorSize);
    for(ix = 0;ix < off;ix++){
        if (!FLUSH_SECTOR)
            return FAT_RW_ERROR;
        Drive.ActSectorinBuffer++;
    }
    return FAT_OK;
}

static FAT_ERROR hd_vfs_getlastcluster(HD_VFS_HANDLER *pfile)
{
    FAT_ERROR err = FAT_OK;
    INT8U *pU8;
    INT32U Cluster,len,Startbit,tmp;

    tmp      = pfile->ReadActCluster >> 3;
    pU8      = (INT8U*)(pfile->bmpfile.pBitmap);
    pU8      = &pU8[tmp];
    len      = tmp+1;
    tmp    <<= 3;
    Startbit = pfile->ReadActCluster & 0x07;
    Cluster  = hd_vfs_find_last_one(pU8,Startbit,len,&err);
    if(err  != FAT_OK)
        return err;
    pfile->ReadActCluster -= Cluster;
    //DEB_PR_HEX("\nLastCluster -> 0x",pfile->ReadActCluster,5)
    return err;
}

static INT32U hd_vfs_allocCluster(VFS_BITMAP_BUFF *bmap,FAT_ERROR *err)
{
    INT32U Cluster = 0,len,tmp;
    INT8U  *pBuff;
    *err = FAT_OK;

    if(!bmap){
        *err = FAT_BADPARAM;
        return 0;
    }
    tmp      = bmap->firstfree >> 3;
    pBuff    = (INT8U*)(bmap->pBitmap);
    pBuff    = &pBuff[tmp];
    len      = bmap->Bitmapbytelen-tmp;
    tmp    <<= 3;
    Cluster  = hd_vfs_find_first_zero(pBuff,len,err);
    if(Cluster != 0xffffffff){
        hd_vfs_setbit((INT8U*)pBuff,Cluster);
        Cluster += tmp;
        if(*err != FAT_OK)return Cluster;
        bmap->firstfree  = Cluster;
    }else{printf("\n cluster END");}
    return Cluster;
}

static FAT_ERROR hd_vfs_getOffsetcluster(HD_VFS_HANDLER *pfile,INT32S NCluster)
{
    INT32S    ix;
    FAT_ERROR err = FAT_OK;

    if(NCluster>0)
        {
            for(ix = 0;ix<NCluster;ix++)
                {
                    if(hd_vfs_getnextcluster(pfile) != FAT_OK)
                        return FAT_OUT_OF_SPACE;
                }
        }else{
        for(ix = 0;ix>NCluster;ix--)
            {
                if(hd_vfs_getlastcluster(pfile) != FAT_OK)
                    return FAT_OUT_OF_SPACE;
            }
    }
    return err;
}

static INT8U *hd_vfs_ClusterRead(INT32U Cluster, INT32U offset)
{
    INT32U SectortoRead;

    if(offset > Drive.V_FAT.ClusterSize)
        return 0;

    offset          += Cluster * Drive.V_FAT.ClusterSize;
    SectortoRead     = Drive.V_FAT.rootentryLbaSect + offset;
    if(SectortoRead != Drive.ActSectorinBuffer){
        if (!READ_SECTOR(SectortoRead))
            return NULL;
    }
    return Drive.pSectorBuffer;
}

static FAT_ERROR hd_vfs_flush_FreeBitmap(void)
{
    INT8U NSectors = (Drive.V_FAT.freelistsizeinbyte/Drive.V_FAT.SectorSize)+1;
    printf("\nFlush FreeBitmap");
    if (!WRITE_SECTORS(Drive.V_FAT.freebitmapsec,Drive.FreeBitmap.pBitmap,NSectors))
        return FAT_RW_ERROR;
    return FAT_OK;
}

static FAT_ERROR hd_vfs_freefrom_FreeBitmap(VFS_BITMAP_BUFF *pBitMap)
{
    FAT_ERROR err = FAT_OK;
    INT8U *pU8;
    INT32U Cluster,ActCluster=0,len,Startbit,tmp;

    //DEB_PR_INFO("\nFree filebmp from FREE_LIST")
    if(!pBitMap->Bitmapbytelen){
        //DEB_PR_ERROR("\nFreeBitmap ERROR!!!! len == NULL")
    }

    while(1)
        {
            tmp      = ActCluster >> 3;
            pU8      = (INT8U*)pBitMap->pBitmap;
            pU8      = &pU8[tmp];
            len      = pBitMap->Bitmapbytelen-tmp;
            tmp    <<= 3;
            Startbit = ActCluster & 0x07;
            Cluster  = hd_vfs_find_first_one(pU8,Startbit,len,&err);
            if(err  != FAT_OK){
                break;
            }
            //! clear bit from free_list
            hd_vfs_clearbit(&((INT8U*)Drive.FreeBitmap.pBitmap)[pU8-(INT8U*)pBitMap->pBitmap],Cluster);
            //! go to next cluster
            ActCluster = Cluster + tmp;
            //DEB_PR_HEX("\nfreeC 0x",ActCluster,8)
            Drive.V_FAT.freecounter++;
            if(ActCluster < Drive.FreeBitmap.firstfree)
                Drive.FreeBitmap.firstfree = ActCluster;
        }
    //DEB_PR_HEX("\nfirstfree 0x",Drive.FreeBitmap.firstfree,8)
    //FM_DumpData((INT8U*)Drive.FreeBitmap.pBitmap,Drive.V_FAT.freelistsizeinbyte);
    return err;
}

static FAT_ERROR hd_vfs_flushFile(HD_VFS_HANDLER *pfile)
{
    FAT_ERROR err = FAT_OK;
    HD_VFS_INODE_DESC *pInode = 0;
    printf("\nFlush File_Head");
    //! store file_head
    if (!READ_SECTOR(pfile->fileinfosector))
        return FAT_RW_ERROR;
    printf("\n Head_LBA 0x%x",pfile->fileinfosector);
    pInode  = (HD_VFS_INODE_DESC *)(Drive.pSectorBuffer);
    pInode  = &pInode[pfile->fileinfosectorindex];
    *pInode = pfile->Inode;
    if (!FLUSH_SECTOR)
        return FAT_RW_ERROR;
    //! store bmap
    //DEB_PR_HEX("\n Bmp_LBA 0x",pfile->Inode.FileBitmapSector,8)
    if (!WRITE_SECTORS(pfile->Inode.FileBitmapSector,pfile->bmpfile.pBitmap,Drive.V_FAT.freelistsizeinlba))
        return FAT_RW_ERROR;
    return err;
}

static FAT_ERROR hd_vfs_deleteFile(HD_VFS_HANDLER *pfile)
{
    FAT_ERROR             err = FAT_OK;
    HD_VFS_INODE_DESC *pInode = 0;

    printf("\nDelete File_Head");
    //! store file_head
    if (!READ_SECTOR(pfile->fileinfosector))
        return FAT_RW_ERROR;
    //DEB_PR_HEX("\n Head_LBA 0x",pfile->fileinfosector,8)
    pInode  = (HD_VFS_INODE_DESC *)(Drive.pSectorBuffer);
    pInode  = &pInode[pfile->fileinfosectorindex];
    *pInode = pfile->Inode;
    memset(pInode,0x00,sizeof(HD_VFS_INODE_DESC));
    if (!FLUSH_SECTOR)
        return FAT_RW_ERROR;
    //! free bitmap
    hd_vfs_freefrom_FreeBitmap(&pfile->bmpfile);
    //! store bmap
    //DEB_PR_HEX("\nfree Bmp_LBA 0x",pfile->Inode.FileBitmapSector,8)
    //DEB_PR_HEX(" len 0x",pfile->bmpfile.Bitmapbytelen,8)
    memset((char*)pfile->bmpfile.pBitmap,0x00,pfile->bmpfile.Bitmapbytelen);
    if (!WRITE_SECTORS(pfile->Inode.FileBitmapSector,pfile->bmpfile.pBitmap,Drive.V_FAT.freelistsizeinlba))
        return FAT_RW_ERROR;
    return err;
}

static FAT_ERROR hd_vfs_allocNcluster(HD_VFS_HANDLER *pfile,INT32U NCluster)
{
    FAT_ERROR err = FAT_OK;
    INT32U Cluster,ix,mask;
    INT8U  *pU8;
    ix = NCluster;
    while(ix--){
        //! allocate a cluster after last alloc in file !!!
        Drive.FreeBitmap.firstfree = pfile->bmpfile.lastallocated;
        //! allocate a cluster in free list
        Cluster = hd_vfs_allocCluster(&Drive.FreeBitmap,&err);
        if(err  != FAT_OK){
            printf("\n allocError");
            return err;
        }
        //! mark this in file_bmp
        pfile->bmpfile.lastallocated = Cluster;
        pU8   = (INT8U*)(pfile->bmpfile.pBitmap);
        mask  = Cluster >> 3;   //! /8
        pU8  += mask;
        mask  = Cluster & 0x07; //! MOD8
        SET_BIT(*pU8,mask);
        pfile->WriteActCluster    = Cluster;
        pfile->lastCluster        = Cluster;
    }
    pfile->Inode.sizecluster += NCluster;
    return err;
}

static FAT_ERROR hd_vfs_printDir(INT32U Cluster)
{
    //       FAT_ERROR err = FAT_OK;
    INT16U    offset = 0,ix;
    HD_VFS_INODE_DESC *pInode = 0;
    INT8U  stat;
    INT32U filenumber = 0;

    printf("\nNo.            Filename");
    do{
        pInode = ((HD_VFS_INODE_DESC*)hd_vfs_ClusterRead(Cluster,offset++));
        if(pInode)
            {
                for(ix = 0;ix < (Drive.V_FAT.SectorSize / sizeof(HD_VFS_INODE_DESC));ix++)
                    {
                        if(filenumber++ >= Drive.V_FAT.rootentrycounter)
                            return FAT_OK;

                        stat = pInode[ix].status;
                        if(stat == INODE_BUSY)
                            {
                                if (pInode[ix].EntryName[0] == CTRLE)
                                    stripCtrlE((char *)pInode[ix].EntryName);
                                printf("\n%04d ",filenumber-1);
                                printf(" bmap lba 0x%x",pInode[ix].FileBitmapSector);
                                printf(" %s",pInode[ix].EntryName);
                                //Maindebug(out);
                                printf(" cs 0x%x",pInode[ix].sizecluster);
                                printf(" ls 0x%x",pInode[ix].sizeinlastcluster);
                            }else
                            if(stat == INODE_FREE)
                                {
                                    //printf("\n%d ",filenumber-1);
                                    //printf("\n-- entry free --");
                                }else
                                if(stat == INODE_EOI)
                                    {
                                        printf("\n-- EOI --");
                                        return FAT_OK;
                                    }else{
                                    printf("\nInode ERROR!! 0x%x",stat);
                                    //DEBUG_DumpData(Drive.pSectorBuffer,512);
                                    return FAT_BAD_CLUSTER;
                                }
                    }
            }
    }while(pInode);
    return FAT_OK;
}

static FAT_ERROR hd_vfs_createDirEntry(HD_VFS_HANDLER *phandler,INT32U Cluster,INT8U *name,INT8U attrib)
{
    INT32U    offset   = 0;
    FAT_ERROR err      = FAT_OK;
    HD_VFS_INODE_DESC *pInode = 0;
    INT16U    ix,file=0;
    printf("\nCreate a DirEntry");
    phandler->EntryIDX = 0;
    do{
        pInode = (HD_VFS_INODE_DESC*)hd_vfs_ClusterRead(Cluster,offset++);
        if(pInode)
            {
                for(ix = 0; ix < (Drive.V_FAT.SectorSize / sizeof(HD_VFS_INODE_DESC));ix++)
                    {
                        if(pInode->status == INODE_FREE){
                            memset((char*)pInode,0x00,sizeof(HD_VFS_INODE_DESC));
                            pInode->status               = INODE_BUSY;
                            pInode->type                 = FILE_ENTRY;
                            pInode->attrib               = attrib;
                            pInode->flServiceType        = 1;
                            pInode->sizecluster          = 0;
                            pInode->sizeinlastcluster    = 0;
                            //! compute bitmaplba
                            pInode->FileBitmapSector     = Drive.V_FAT.rootentryLbaSect;          //! offset to part
                            //! +1 == first cluster after root_entry
                            pInode->FileBitmapSector    += (Cluster+1 * Drive.V_FAT.ClusterSize); //! offset in fat
                            pInode->FileBitmapSector    +=  file*Drive.V_FAT.freelistsizeinlba;
                            pInode->Bitmapsizeinbyte     =  Drive.V_FAT.freelistsizeinbyte;
                            sprintf((char*)pInode->EntryName,"%s",name);
                            //! store this info to disk
                            if (!FLUSH_SECTOR)
                                return FAT_RW_ERROR;
                            phandler->Inode                = *pInode;
                            phandler->fileinfosector       = Drive.ActSectorinBuffer;
                            phandler->fileinfosectorindex  = ix;
                            phandler->ReadActCluster       = 0;
                            phandler->ReadClusterByteOffset= 0;
                            phandler->WriteActCluster      = 0;
                            phandler->WriteClusterOffset   = 0;
                            return err;
                        }else if(pInode->status == INODE_BUSY){
                            file++;
                            pInode++;
                        }else{
                            printf("\nOut of file entry's!!!!!");
                            err = FAT_OUT_OF_SPACE;
                            return err;
                        }
                        phandler->EntryIDX++;
                    }
            }
    }while(pInode);
    return err;
}

static FAT_ERROR hd_vfs_Seek(HD_VFS_HANDLER *file,INT32S SkipNbyte, BOOL flReadorWrite)
{
    INT32S          NCluster;
    INT32U          Rembyte;
    FAT_ERROR       err = FAT_OK;

    if(flReadorWrite)
        {
            if(!file->ReadClusterByteOffset){
                err = hd_vfs_getnextcluster(file);
                if(err == FAT_OUT_OF_SPACE){
                    printf("\nEOF!!!!");
                    return err;
                }
            }
            Rembyte   = file->ReadClusterByteOffset;
            NCluster  = Rembyte + SkipNbyte;
            if(NCluster > 0)
                {
                    NCluster /= (Drive.V_FAT.ClusterSize << 9);
                    file->ReadClusterByteOffset = (SkipNbyte+Rembyte) % (Drive.V_FAT.ClusterSize << 9);
                    //! test byteoffset
                    if(file->ReadActCluster == file->lastCluster){
                        if(file->ReadClusterByteOffset > file->Inode.sizeinlastcluster){
                            printf("\nLINODE!!!");
                            return FAT_EOC;
                        }
                    }
                }
            else{
                //! get absolute value
                NCluster  = abs(NCluster);
                //NCluster  = (0xffffffff - ((INT32U)NCluster));
                file->ReadClusterByteOffset = (Drive.V_FAT.ClusterSize << 9) - NCluster;
                //! get cluster
                NCluster /= (Drive.V_FAT.ClusterSize << 9);
                NCluster -= 1;
            }
            //! compute new cluster
            if(NCluster != 0){
                if(file->ReadClusterByteOffset)
                    err  = hd_vfs_getOffsetcluster(file,NCluster);
                if(err != FAT_OK){
                    printf("\nEOF!!!");
                    return err;
                }
            }
            //!file->ReadClusterByteOffset
        }else
        {
            /*
              Must be coded !!!!!
            */
        }
    return err;
}

FAT_ERROR VFS_SetVFSRecordInfo(HD_VFS_MARK_INFO *pInfo,INT8U flLinkenable)
{
    FAT_ERROR err = FAT_OK;
    INT32U    tmp,offset;
    HD_VFS_HANDLER      file;
    HD_VFS_RECORD_INFO *pRecInfo;

    printf("\n Set RecordInfo ->");
    VFS_Enter();
    //! set record mark
    file.ReadActCluster        = pInfo->MarkCluster;
    file.ReadClusterByteOffset = pInfo->MarkClusterByteOffset;
    err = hd_vfs_Seek(&file,-(VFS_REC_SIZE-VFS_TSPACK_SIZE),1);
    if(!err){
        tmp      = file.ReadActCluster;
        offset   = file.ReadClusterByteOffset >> 9;
        hd_vfs_ClusterRead(tmp,offset);
        offset   = file.ReadClusterByteOffset & 0x1FF;
        //printf("\n off -> %d size %d",512-offset,sizeof(HD_VFS_RECORD_INFO));
        pRecInfo = (HD_VFS_RECORD_INFO*)(&Drive.pSectorBuffer[offset]);
        if(pRecInfo->ActRecordNbr != pInfo->ActRecordNbr){
            printf("\n Wrong RegNbr ->");
            printf(" RecNbr  0x%x",pRecInfo->ActRecordNbr);
            printf(" InfoNbr 0x%x",pInfo->ActRecordNbr);
            VFS_Exit();
            return FAT_ERROR_GEN;
        }
        //! set the link to the associated MarkIDX
        pRecInfo->MarkLink  = pInfo->Idx;
        pRecInfo->Linkused  = flLinkenable;
        //! save the sector
        if (!FLUSH_SECTOR)
            return FAT_RW_ERROR;
    }else{printf("\nSet RecordMark SeekERROR!!!");}
    VFS_Exit();
    return err;
}

FAT_ERROR VFS_SetMarkInfoIDX(INT16U fileidx,INT16U markidx,HD_VFS_MARK_INFO *pInfo)
{
    INT32U off,lba;
    if(!Drive.V_FAT.infoentryLBA){
        printf("\n mark not supported!!!");
        return FAT_DEVICENOTSUPPORTED;
    }else
        if(Drive.V_FAT.numberoffilesinFS < fileidx)
            {
                printf("\n fileIDX not supported 0x%x",fileidx);
                return FAT_DEVICENOTSUPPORTED;
            }else
            if(Drive.V_FAT.marksbyentry < markidx)
                {
                    printf("\n fileIDX not supported!!!");
                    return FAT_DEVICENOTSUPPORTED;
                }

    VFS_Enter();
    //! compute LBA by one file
    off  = Drive.V_FAT.marksbyentry*sizeof(HD_VFS_MARK_INFO);
    off  = off>>9;
    //! offset to file-info (fileidx)
    lba  = Drive.V_FAT.infoentryLBA + (off*fileidx);
    //! offset to mark
    lba += ((markidx*sizeof(HD_VFS_MARK_INFO))>>9);
    //! read the LBA
    if(Drive.ActSectorinBuffer != lba)
        if (!READ_SECTOR(lba))
            return FAT_RW_ERROR;
    //! offset in LBA
    *(HD_VFS_MARK_INFO*)((INT8U*)&Drive.pSectorBuffer[((markidx<<6)&0x1ff)]) = *pInfo;
    if (!FLUSH_SECTOR)
        return FAT_RW_ERROR;
    VFS_Exit();
    return FAT_OK;
}

FAT_ERROR VFS_SetMarkInfo(INT16U fileidx,HD_VFS_MARK_INFO *pInfo)
{
    INT32U off,ix,lba;
    INT8U  State = 0;
    HD_VFS_MARK_INFO Tmp,Tmp1;

    if(!Drive.V_FAT.infoentryLBA){
        printf("\n mark not supported!!!");
        return FAT_DEVICENOTSUPPORTED;
    }else
        if(Drive.V_FAT.numberoffilesinFS < fileidx)
            {
                printf("\n fileIDX not supported!!!");
                printf("\n fileIDX 0x%04x",fileidx);
                return FAT_DEVICENOTSUPPORTED;
            }
    printf("\nSet MarkInfo");
    //!compute file IDX offset
    off  = Drive.V_FAT.marksbyentry<<6;
    off  = off>>9;
    off  = Drive.V_FAT.infoentryLBA + (off*fileidx);
    //!sort to mark list
    for(ix = 0;ix < Drive.V_FAT.marksbyentry;ix++)
        {
            //DEB_PR_DEZ("\n m ",ix,4)
            //!enter VFS
            VFS_Enter();
            //! offset to mark
            lba  = off + ((ix<<6)>>9);
            //DEB_PR_HEX(" lba->0x",lba,8)
            //! reload SectorBuffer
            if(Drive.ActSectorinBuffer != lba){
                //! read the new
                if (!READ_SECTOR(lba))
                    return FAT_RW_ERROR;
            }
            //! search for the correct place
            if(State == 0)
                {
                    //DEB_PR_HEX(" ->0x",((HD_VFS_MARK_INFO*)&Drive.pSectorBuffer[((ix<<6)&0x1ff)])->type,4)
                    if(!((HD_VFS_MARK_INFO*)&Drive.pSectorBuffer[((ix<<6)&0x1ff)])->type)
                        {
                            //! store the entry on a free entry
                            printf("\nSet Mark at the END");
                            //! create mark
                            if(!pInfo->type)pInfo->type = MARK_POS;
                            if(!strlen((const char*)pInfo->markname)){
                                sprintf((char*)pInfo->markname,"Mark%d",ix);
                            }
                            else{
                                printf("\n use special markname ->");
                                printf("%s", (const char*)pInfo->markname);
                            }
                            pInfo->Idx = ix;
                            *(HD_VFS_MARK_INFO*)&Drive.pSectorBuffer[((ix<<6)&0x1ff)] = *pInfo;
                            if (!FLUSH_SECTOR)
                                return FAT_RW_ERROR;
                            break;
                        }else
                        if(((HD_VFS_MARK_INFO*)&Drive.pSectorBuffer[((ix<<6)&0x1ff)])->ActRecordNbr > pInfo->ActRecordNbr)
                            {
                                printf("\nSet Mark to Pos ->0x%x",ix);
                                //! store ACT position to temp
                                Tmp = *(HD_VFS_MARK_INFO*)&Drive.pSectorBuffer[((ix<<6)&0x1ff)];
                                //! create mark
                                if(!pInfo->type)pInfo->type = MARK_POS;
                                if(!strlen((const char*)pInfo->markname)){
                                    sprintf((char*)pInfo->markname,"Mark%d",ix);
                                }
                                else{
                                    printf("\n use special markname ->");
                                    printf("%s", (const char*)pInfo->markname);
                                }
                                pInfo->Idx = ix;
                                *(HD_VFS_MARK_INFO*)&Drive.pSectorBuffer[((ix<<6)&0x1ff)] = *pInfo;
                                //! store LBA next time
                                State = 1;
                            }else
                            if(((HD_VFS_MARK_INFO*)&Drive.pSectorBuffer[((ix<<6)&0x1ff)])->ActRecordNbr == pInfo->ActRecordNbr) //set in same Block? do nothing!!!
                                {
                                    printf("\n do not set Mark in same Block !!!");
                                    VFS_Exit();
                                    return FAT_ERROR_GEN;
                                }else
                                if(((HD_VFS_MARK_INFO*)&Drive.pSectorBuffer[((ix<<6)&0x1ff)])->type == MARK_FEND)
                                    {
                                        printf("\n END !!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
                                        VFS_Exit();
                                        return FAT_ERROR_GEN;
                                    }
                }else
                //! resort until END
                if(State == 1)
                    {
                        //! save tmp
                        Tmp1  = *(HD_VFS_MARK_INFO*)&Drive.pSectorBuffer[((ix<<6)&0x1ff)];
                        //! resort
                        Tmp.Idx = ix;
                        *(HD_VFS_MARK_INFO*)&Drive.pSectorBuffer[((ix<<6)&0x1ff)] = Tmp;
                        //! check for end
                        if(!Tmp1.type){
                            if (!FLUSH_SECTOR)
                                return FAT_RW_ERROR;
                            break;
                        }
                        Tmp   = Tmp1;
                    }
            //!exit VFS
            if (!FLUSH_SECTOR)
                return FAT_RW_ERROR;
            VFS_Exit();
        }
    //!exit VFS
    //FLUSH_SECTOR;
    VFS_Exit();
    return FAT_OK;
}

FAT_ERROR VFS_Mount(INT32U SecLba)
{
    INT32U    free,tmp;
    FAT_ERROR err;
    INT8U     i=0;

    VFS_Enter();
    //! test MBR
    if (!READ_SECTOR(0)) {
        printf("Partition table can not be read!!!!\n");
        return FAT_ERROR_GEN;
    }

    tmp   = Drive.pSectorBuffer[511]<<8;
    tmp  += Drive.pSectorBuffer[510];
    if(tmp != 0xAA55){
        printf("Partition table not active!!!! 0x%x\n",tmp);
        return FAT_ERROR_GEN;
    }
    for(i = 0; i < 4;i++)
        {
            printf("\n test P->0x%x type 0x%x",i,Drive.pSectorBuffer[(0x1be + (i*0x10)) + 4]);
            if(Drive.pSectorBuffer[(0x1be + (i*0x10)) + 4] == 0xbb)
                break;
        }
    if(Drive.pSectorBuffer[(0x1be + (i*0x10)) + 4] != 0xbb){
        //printf("\n this is not LAFAT!!!!");
        VFS_Exit();
        return FAT_ERROR_GEN;
    }
    SecLba  = Drive.pSectorBuffer[(0x1be + (i*0x10)) +8+3]<<24;
    SecLba += Drive.pSectorBuffer[(0x1be + (i*0x10)) +8+2]<<16;
    SecLba += Drive.pSectorBuffer[(0x1be + (i*0x10)) +8+1]<<8;
    SecLba += Drive.pSectorBuffer[(0x1be + (i*0x10)) + 8];
    printf("\n mount VFS lba->0x%08x",SecLba);
    if (!READ_SECTOR(SecLba))
        return FAT_RW_ERROR;
    Drive.V_FAT = *(VFS_Table*)(Drive.pSectorBuffer);

    printf("\n FreeBitmapLba ->0x%x" ,Drive.V_FAT.freebitmapsec     );
    printf("\n BitmapbyteSize->0x%x" ,Drive.V_FAT.freelistsizeinbyte);
    printf("\n RootEntrys ->0x%x"    ,Drive.V_FAT.rootentrycounter  );
    printf("\n RootEntSect->0x%x"    ,Drive.V_FAT.rootentryLbaSect  );
    printf("\n freeInodes ->0x%x"    ,Drive.V_FAT.freecounter       );
    printf("\n---VFS info system------>>>");
    printf("\n LBA        ->0x%x"    ,Drive.V_FAT.infoentryLBA      );
    printf("\n Entrys     ->0x%x"    ,Drive.V_FAT.infoentrycounter  );
    printf("\n LBASize    ->0x%x"    ,Drive.V_FAT.infosize          );

    //! load free_bitmap
    Drive.FreeBitmap.pBitmap  = BitMapBuf;
    if((VFS_MAX_U32BITMAP_SIZE<<2) < Drive.V_FAT.freelistsizeinlba*Drive.V_FAT.SectorSize){
        printf("\n ERRORRRRRR");
    }
    if (!READ_SECTORS(Drive.V_FAT.freebitmapsec,Drive.FreeBitmap.pBitmap,Drive.V_FAT.freelistsizeinlba))
        return FAT_RW_ERROR;
    //DEBUG_DumpData((U8*)BitMapBuf,Drive.V_FAT.freelistsizeinbyte);

    free = hd_vfs_count_free((INT8U*)(Drive.FreeBitmap.pBitmap),Drive.V_FAT.freelistsizeinbyte);
    printf("\n FreeTest 0x%x"  ,free);
    free = hd_vfs_find_first_zero((INT8U*)(Drive.FreeBitmap.pBitmap),Drive.V_FAT.freelistsizeinbyte,&err);
    printf("\n First free 0x%x\n",free);
    //! update buffer
    Drive.FreeBitmap.firstfree     = free;
    Drive.FreeBitmap.Bitmapbytelen = Drive.V_FAT.freelistsizeinbyte;
    //! dump root
    hd_vfs_printDir(0);
    VFS_Exit();
    return FAT_OK;
}

INT32U VFS_PutNByte(HD_VFS_HANDLER **pfile,INT8U *pData,INT32U Size,FAT_ERROR *err)
{
    HD_VFS_HANDLER *file      = (*pfile);
    INT32U size               = Size;
    INT32U Cluster,offset,NSectors,Sectortowrite,tmp;

    if(*pfile == 0){
        printf("\n sorry invalid handler for this file");
        *err = FAT_ERROR_GEN;
        return 0;
    }
    //tmp = file->Inode.sizeinlastcluster+Size;
    //if(tmp > 0x80000)tmp = tmp - 0x80000;
    //DEB_PR_HEX("\nnext byte_offset 0x",tmp,8)
    VFS_Enter();
    *err = FAT_OK;
    //! fill last sector
    //DEB_PR_HEX(" b 0x",file->Inode.sizeinlastcluster,8)
    if(file->Inode.sizeinlastcluster % 512)
        {
            //! find last sector
            Cluster = file->WriteActCluster;
            //! sector offset
            offset  = file->Inode.sizeinlastcluster >> 9;
            //! read this sector
            hd_vfs_ClusterRead(Cluster,offset);
            //! fill sector
            offset  = file->Inode.sizeinlastcluster % 512;
            tmp     = 512-offset;
            if(tmp > size)tmp = size;
            memcpy(&Drive.pSectorBuffer[offset],pData,tmp);
            pData += tmp;
            if(tmp > size)
                tmp = size;
            file->WriteClusterOffset++;
            file->Inode.sizeinlastcluster += tmp;
            size                          -= tmp;
            //DEB_PR_HEX("\nWriteClusterOffset 0x",file->WriteClusterOffset,8)
            //DEB_PR_HEX(" ByteoffsetOffset 0x",file->Inode.sizeinlastcluster,8)
            //! -1 ????
            if((file->WriteClusterOffset ) >= Drive.V_FAT.ClusterSize){
                file->Inode.sizeinlastcluster  = 0;
                file->WriteClusterOffset       = 0;
            }
            if (!FLUSH_SECTOR)
                return FAT_RW_ERROR;
        }
    //! write the rest of data
    while(size > 0)
        {
            if(!file->WriteClusterOffset){
                //printf("\n PutNByte1 0x%x",size);
                if(hd_vfs_allocNcluster(file,1) != FAT_OK){
                    printf("\n HD_Space is emty");
                    *err = 1;
                    VFS_Exit();
                    return 0;
                }
            }

            //! compute sector to write
            Sectortowrite  = Drive.V_FAT.rootentryLbaSect + (file->WriteActCluster*Drive.V_FAT.ClusterSize);
            Sectortowrite += file->WriteClusterOffset;
            //printf("\nSectoWrite 0x%x",Sectortowrite);
            if(size >= ((Drive.V_FAT.ClusterSize - file->WriteClusterOffset) << 9)){
                //! fill act sector
                NSectors = Drive.V_FAT.ClusterSize - file->WriteClusterOffset;
                //! reset sector offset
                file->WriteClusterOffset = 0;
                file->Inode.sizeinlastcluster  = 0;
                if (!WRITE_SECTORS(Sectortowrite,pData,NSectors))
                    return FAT_RW_ERROR;
                tmp      = (NSectors << 9);
                size    -= tmp;
                pData   += tmp;
            }else{
                NSectors = size >> 9;
                tmp      = size % 512;
                size                           -= ((NSectors<<9)+tmp);
                if(tmp){
                    NSectors++;
                    file->WriteClusterOffset       += NSectors - 1;
                }else
                    file->WriteClusterOffset       += NSectors;
                file->Inode.sizeinlastcluster   = (file->WriteClusterOffset<<9)+tmp;
                if (!WRITE_SECTORS(Sectortowrite,pData,NSectors))
                    return FAT_RW_ERROR;
            }
        }
    //DEB_PR_HEX("\nbyte_offset 0x",file->Inode.sizeinlastcluster,8)
    //DEB_PR_HEX("\nActCluster 0x",file->WriteActCluster,8)
    VFS_Exit();
    return (Size-size);
}

INT32U  VFS_GetNByte(HD_VFS_HANDLER **pfile,INT8U *pData,INT32U Size,FAT_ERROR *err)
{
    HD_VFS_HANDLER *file      = (*pfile);
    INT32U NSectors,Sectortoread,tmp,offset;
    INT32S size = Size;

    if(*pfile == 0){
        printf("\n sorry invalide handler for this file");
        *err = FAT_ERROR_GEN;
        return 0;
    }
    VFS_Enter();
    *err    = FAT_OK;
    //! read last sector
    if(file->ReadClusterByteOffset % 512)
        {
            //DEB_PR_HEX("\nbyte_offset1 0x",file->ReadClusterByteOffset,8)
            //! find last sector
            tmp    = file->ReadActCluster;
            //! sector offset
            offset = file->ReadClusterByteOffset >> 9;
            //! read this sector
            hd_vfs_ClusterRead(tmp,offset);
            offset = file->ReadClusterByteOffset % 512;
            tmp    = 512-offset;
            if((INT32S)tmp > size)tmp = size;
            memcpy(pData,&Drive.pSectorBuffer[offset],tmp);
            pData += tmp;
            file->ReadClusterByteOffset += tmp;
            size  -= tmp;
            //DEB_PR_HEX("\nbyte_offset2 0x",file->ReadClusterByteOffset,8)
            if(file->ReadClusterByteOffset >= (Drive.V_FAT.ClusterSize << 9))
                file->ReadClusterByteOffset  = 0;
        }
    if(file->ReadActCluster == file->lastCluster){
        if(file->ReadClusterByteOffset > file->Inode.sizeinlastcluster){
            //DEB_PR_ERROR("\nLINODE!!!")
            *err = FAT_EOC;
            VFS_Exit();
            return (Size - size);
        }
    }
    //! read multi sector
    while(size > 0)
        {
            if(!file->ReadClusterByteOffset){
                if(hd_vfs_getnextcluster(file) == FAT_OUT_OF_SPACE){
                    printf("\nEOF!!!!");
                    VFS_Exit();
                    return (Size - size);
                }
            }
            tmp           = file->ReadClusterByteOffset >> 9;
            Sectortoread  = Drive.V_FAT.rootentryLbaSect;
            Sectortoread += (file->ReadActCluster*Drive.V_FAT.ClusterSize);
            Sectortoread += tmp;

            if(size >= (INT32S)((Drive.V_FAT.ClusterSize - tmp) << 9))
                {
                    NSectors = Drive.V_FAT.ClusterSize - tmp;
                    file->ReadClusterByteOffset = 0;
                    if (!READ_SECTORS(Sectortoread,pData,NSectors))
                        return FAT_RW_ERROR;
                    tmp      = (NSectors << 9);
                    size    -= tmp;
                    pData   += tmp;
                }else{
                NSectors = size >> 9;
                tmp      = size % 512;
                size    -= ((NSectors<<9)+tmp);
                file->ReadClusterByteOffset += (NSectors<<9)+tmp;
                if(tmp)NSectors++;
                if (!READ_SECTORS(Sectortoread,pData,NSectors))
                    return FAT_RW_ERROR;
            }
        }


    VFS_Exit();
    return (Size - size);
}

FAT_ERROR VFS_CloseFile(HD_VFS_HANDLER *pfile,HD_VFS_FILECLOSE store)
{
    FAT_ERROR err    = FAT_OK;

    if(!pfile)       return FAT_BADPARAM;
    if(!pfile->stat) return FAT_BADPARAM;
    printf("\n Close file");
    VFS_Enter();
    if(store == FILE_STORE)
        {
            hd_vfs_flushFile(pfile);
            hd_vfs_flush_FreeBitmap();
        }
    else if(store == FILE_DEL){
        hd_vfs_deleteFile(pfile);
        hd_vfs_flush_FreeBitmap();
    }else{
        printf(" without store");
    }
    pfile->stat                  = 0;
    pfile->fileinfosector        = 0;
    pfile->fileinfosectorindex   = 0;
    pfile->ReadActCluster        = 0;
    pfile->ReadClusterByteOffset = 0;
    pfile->WriteActCluster       = 0;
    pfile->WriteClusterOffset    = 0;
    printf("\n free -> 0x%p",pfile->bmpfile.pBitmap);
    free(pfile->bmpfile.pBitmap);
    pfile->bmpfile.pBitmap       = 0;
    pfile->bmpfile.Bitmapbytelen = 0;
    pfile->bmpfile.firstfree     = 0;
    pfile->bmpfile.lastallocated = 0;
    VFS_Exit();
    return err;
}

FAT_ERROR VFS_OpenFile(HD_VFS_HANDLER **pfile,INT8U *path,INT16U flag)
{
    FAT_ERROR err    = FAT_OK;
    HD_VFS_INODE_DESC *pInode  = 0;
    INT32U             Cluster = 0,
        offset  = 0,
        ix =0,i,is;

    if(!path)return FAT_BADPARAM;
    VFS_Enter();
    printf("\n Open File %x", path[0]);
    //! find a free file_handler
    for(i=0;i<VFS_FILES_MAX_OPEN;i++) {
        if(!VFSHandler[i].stat) {
            is = Drive.V_FAT.rootentrycounter;
            VFSHandler[i].EntryIDX = 0;
            while(is--) {
                pInode = (HD_VFS_INODE_DESC*)hd_vfs_ClusterRead(Cluster,offset++);
                if(!pInode)continue;
                for(ix = 0; ix < (Drive.V_FAT.SectorSize / sizeof(HD_VFS_INODE_DESC));ix++) {
                    if (pInode[ix].EntryName[0] == CTRLE)
                        stripCtrlE((char *)pInode[ix].EntryName);
                    if(pInode[ix].status == INODE_EOI) {
                        printf("\n Path not found (EOI)");
                        break;
                    } else
                        if(pInode[ix].status == INODE_BUSY) {
                            if(!strcmp((const char*)pInode[ix].EntryName,(const char*)path)) {
                                printf("\n Path found");
                                pInode = &pInode[ix];
                                is     = 0;
                                if (flag == (O_RDWR|O_CREAT)){
                                    VFS_Exit();
                                    return FAT_FILE_OPEN_ERROR;
                                } else
                                    break;
                            }
                        }
                    VFSHandler[i].EntryIDX++;
                }
            }
            if (flag == O_RDWR) {
                if(pInode->status == INODE_BUSY) {
                    printf("\nopen file OK "); printf("%s",(const char*)path); printf(" Idx ->0x%x",i);
                    VFSHandler[i].Inode                 = *pInode;
                    VFSHandler[i].fileinfosector        =  Drive.ActSectorinBuffer;
                    VFSHandler[i].fileinfosectorindex   =  ix;
                    VFSHandler[i].ReadActCluster        =  0;
                    VFSHandler[i].ReadClusterByteOffset =  0;
                    VFSHandler[i].WriteActCluster       =  0;
                    VFSHandler[i].WriteClusterOffset    =  0;
                    //! allocate buffer
                    printf("\n allocate buffer for bitmap ->");
                    VFSHandler[i].stat              = 1;
                    (*pfile)                        = &VFSHandler[i];
                    (*pfile)->bmpfile.pBitmap       = (INT32U *)malloc(Drive.V_FAT.freelistsizeinlba*Drive.V_FAT.SectorSize);
                    printf(" 0x%p" ,(*pfile)->bmpfile.pBitmap);
                    (*pfile)->bmpfile.Bitmapbytelen = Drive.V_FAT.freelistsizeinbyte;
                    printf("\n load bitmap");
                    if (!READ_SECTORS((*pfile)->Inode.FileBitmapSector,((*pfile)->bmpfile.pBitmap),Drive.V_FAT.freelistsizeinlba))
                        return FAT_RW_ERROR;
                    //DEBUG_DumpData((*pfile)->bmpfile.pBitmap,0x500);
                    VFSHandler[i].lastCluster = 0;
                    hd_vfs_getendcluster(&VFSHandler[i]);
                    break;
                } else {
                    printf("\n File not available!!!");
                    err = FAT_FILE_OPEN_ERROR;
                    break;
                }
            } else
                if(flag == (O_RDWR|O_CREAT)) {
                    //hd_vfs_printDir(0);
                    //DEB_PR_INFO("\n Create file-> ")DEB_PR_INFO((const char*)path)
                    hd_vfs_createDirEntry(&VFSHandler[i],Cluster,path,0x20);
                    //DEB_PR_INFOCOL(FM_BLUE,"\nopen file OK ") DEB_PR_INFO((const char*)path) DEB_PR_HEX(" Idx ->",i,8)
                    //! allocate buffer
                    //DEB_PR_INFO("\n allocate buffer for bitmap ->")
                    VFSHandler[i].stat              = 1;
                    (*pfile)                        = &VFSHandler[i];
                    (*pfile)->bmpfile.pBitmap       = (INT32U *)malloc(Drive.V_FAT.freelistsizeinlba*Drive.V_FAT.SectorSize);
                    //DEB_PR_HEX(" 0x" ,(*pfile)->bmpfile.pBitmap,8)
                    (*pfile)->bmpfile.Bitmapbytelen = Drive.V_FAT.freelistsizeinbyte;
                    (*pfile)->bmpfile.lastallocated = Drive.FreeBitmap.firstfree;
                    //DEB_PR_INFO("\n reset bitmap")
                    memset((*pfile)->bmpfile.pBitmap,0x00,Drive.V_FAT.freelistsizeinlba*Drive.V_FAT.SectorSize);
                    hd_vfs_flushFile(*pfile);
                    //! init file mark list
                    hd_vfs_resetFileMarkList((*pfile)->EntryIDX);
                    break;
                } else {
                    printf("\n Mask Not Supported!!!!");
                }

        }else{
            //DEB_PR_ERROR("\n handler busy ->")DEB_PR_INFO((const char*)VFSHandler[i].Inode.EntryName)
            if(!strcmp((const char*)VFSHandler[i].Inode.EntryName,(const char*)path))
                {
                    printf("\n file allready open!!!");
                    *pfile = &VFSHandler[i];
                    err    = FAT_ALREADY_INITIALIZED;
                    break;
                }
        }
    }
    VFS_Exit();
    return err;
}

FAT_ERROR VFS_GetFileInfobyIndex(INT16U idx,HD_VFS_HANDLER *pHandler)
{
    FAT_ERROR          err = FAT_OK;
    HD_VFS_INODE_DESC *pInode;
    INT32U             Cluster,offset,Idx;

    if(Drive.V_FAT.rootentrycounter < idx)
        return FAT_OUT_OF_SPACE;
    VFS_Enter();
    //! get target cluster
    Cluster = (idx*sizeof(HD_VFS_INODE_DESC)) / VFS_CLUSTER_SIZE;
    //! sector offset from one cluster
    offset  = (VFS_CLUSTER_SIZE / sizeof(HD_VFS_INODE_DESC))*Cluster;
    //! get sector in this cluster
    offset += (idx*sizeof(HD_VFS_INODE_DESC)) / Drive.V_FAT.SectorSize;
    //! read sector
    pInode  = ((HD_VFS_INODE_DESC*)hd_vfs_ClusterRead(Cluster,offset));
    Idx     = (idx*sizeof(HD_VFS_INODE_DESC)) % Drive.V_FAT.SectorSize;
    Idx    /=  sizeof(HD_VFS_INODE_DESC);
    // Strip leading CTRL-E character from the UI file list
    if(pInode)
        if (pInode[Idx].EntryName[0] == CTRLE)
            stripCtrlE((char *)pInode[Idx].EntryName);
    //! offset in this sector
    pInode  = &pInode[Idx];
    //DEB_PR_HEX("\nCluster 0x",Cluster,4)
    //DEB_PR_HEX(" SectorOffset 0x",offset,4)
    //DEB_PR_HEX(" InodeIdx 0x",Idx,4)
    //! file is not open
    pHandler->stat                  = 0;
    pHandler->bmpfile.pBitmap       = 0;
    pHandler->Inode                 = *pInode;
    pHandler->fileinfosector        = Drive.ActSectorinBuffer;
    pHandler->fileinfosectorindex   = Idx;
    pHandler->bmpfile.Bitmapbytelen = Drive.V_FAT.freelistsizeinbyte;
    //Maindebug((const char*)pHandler->Inode.EntryName);
    //DEBUG_DumpData(pInode,20);
    VFS_Exit();
    return err;
}

FAT_ERROR HD_VFS_GetEventInfobyFileIDX(INT16U FileIdx,INT8U *pData)
{
    FAT_ERROR         err;
    INT32U            Sectortoread;
    printf("\nGet EventInfo ->0x%x",FileIdx);
    VFS_Enter();
    Sectortoread  = Drive.V_FAT.rootentryLbaSect;
    //! offset to last dir_entry
    Sectortoread += ((Drive.V_FAT.rootentrycounter)*sizeof(HD_VFS_INODE_DESC)) / Drive.V_FAT.SectorSize;
    if(((Drive.V_FAT.rootentrycounter)*sizeof(HD_VFS_INODE_DESC)) % Drive.V_FAT.SectorSize)
        Sectortoread++;
    //! info_Idx offset (512 byte info by file)
    Sectortoread += FileIdx;

    printf(" at sector ->0x%x",Sectortoread);
    err = READ_SECTORS(Sectortoread,pData,1);
    VFS_Exit();
    return err;
}

FAT_ERROR HD_VFS_PutEventInfobyFileIDX(INT16U FileIdx,INT8U *pData)
{
    INT32U            Sectortoread;
    printf("\nPut EventInfo ->0x%x",FileIdx);
    VFS_Enter();
    Sectortoread  = Drive.V_FAT.rootentryLbaSect;
    //! offset to last dir_entry
    Sectortoread += ((Drive.V_FAT.rootentrycounter)*sizeof(HD_VFS_INODE_DESC)) / Drive.V_FAT.SectorSize;
    if(((Drive.V_FAT.rootentrycounter)*sizeof(HD_VFS_INODE_DESC)) % Drive.V_FAT.SectorSize)
        Sectortoread++;
    //! info_Idx offset (512 byte info by file)
    Sectortoread += FileIdx;
    memcpy(Drive.pSectorBuffer,pData,512);
    Drive.ActSectorinBuffer = Sectortoread;
    if (!FLUSH_SECTOR)
        return FAT_RW_ERROR;
    VFS_Exit();
    return FAT_OK;
}

FAT_ERROR VFS_Open(HD_VFS_INIT *pOpen)
{
    printf(" Init VFS support");
    Drive.VfsSys        = pOpen->VfsSys;
    Drive.pSectorBuffer = &SectorBuffer[0];
    Drive.ActSectorinBuffer = 0x00;
    Drive.ActLbaSector      = 0x00;
    return FAT_OK;
}

INT32U VFS_GetClusterSize()
{
    return Drive.V_FAT.ClusterSize;
}
