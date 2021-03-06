/*
 * diskthread.cpp - pc2box threaded operations
 *
 *  This code implements the thread that perform the disk operations
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
#include "diskthread.h"

#if !defined(_WIN32) && !defined(_WIN64)
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#endif

Disk_Thread::Disk_Thread(THREAD_Params *pParams)
{
    this->strOutput      = pParams->strOutput;
    this->pActVfsHandler = pParams->pFileHandler;
    this->lock           = pParams->pLock;
    this->pFileCounter   = pParams->fileCounter;
    this->pFileDownload  = pParams->pFileDownload;
    this->pDownloadBar   = pParams->pDownloadBar;
    this->pPc2Box        = pParams->pPc2Box;
    this->pREC2TS        = pParams->pREC2TS;
}

int Disk_Thread::Poll_vfs(int dev, bool *ping)
{
    PVFS_FILESYS        VfsSys       = &FileSys;

    /* Try to get media info for device; most probably not possible
       under Windows in case the device is locked / opened!
       -> Remove files from list in case device is/gets mounted
    */
    if(!NT_SUCCESS(VfsGetMediaInfo(VfsSys))){
        lock->lock();

        *this->pFileCounter = 0;
        // DiskRemovebeep is connected to clearFileListWidget which sets the counter to 0xffffffff
        emit DiskRemovebeep();
        while(*this->pFileCounter != 0xffffffff) { // sync thread
            lock->unlock();
            // Give the myform thread the chance to update the variable (task switch)
            msleep(1);
            lock->lock();
        }

        lock->unlock();

        VfsCloseDevice(VfsSys);
        Unmount(DRIVER_MAGIC);
        *ping = true;
    }
    return dev;
}

// Note: This code needs to be called from a context where the lock mutex is locked
void Disk_Thread::UpdateFileListWidget(void){
    U32 idx;
    HD_VFS_HANDLER *File = (HD_VFS_HANDLER*)malloc(sizeof(HD_VFS_HANDLER));

    *this->pFileCounter = 0;
    emit DiskRemovebeep();
    while(*this->pFileCounter != 0xffffffff){ // sync thread
        lock->unlock();
        msleep(1);
        lock->lock();
    }

    *this->pFileCounter =0;
    for(idx = 0;idx < VFS_ROOT_ENTRIES;idx++){
        if(VFS_GetFileInfobyIndex(idx,File) == FAT_OK){
            if(File->Inode.status == INODE_BUSY){
                *this->pFileCounter = idx+1;
                printf(" VFS loop 0x%x\n",(int)*this->pFileCounter);
                AddNewVFSHandler(File);
                while(*this->pFileCounter != 0xffffffff){ // sync thread
                    lock->unlock();
                    msleep(1);
                    lock->lock();
                }

            }
        }
    }
    free(File);
}

void Disk_Thread::printTextBrowser(const char *str){
    if(str){
        strcat(this->strOutput,str);
    } else
        this->strOutput[0] = 0x00;
    emit beep();
}

void Disk_Thread::AddNewVFSHandler(HD_VFS_HANDLER *pVfsHandler){
    if(pActVfsHandler){
        *pActVfsHandler = *pVfsHandler;
        emit FileListbeep();
    }else{
        printf(" VFSHandler not initialized\n");
    }
}

void Disk_Thread::updateDownloadbar(DownloadBarInfo *pInfo){
    //printf(" bar -> %d\n",(int)pInfo->bar);
    *pDownloadBar = *pInfo;
    emit UpdateBarbeep();
    while(pDownloadBar->bar != 0xffffffff){ // sync it
        lock->unlock();
        msleep(1);
        lock->lock();
    }
}

int Disk_Thread::init_vfs(int dev, bool *ping)
{
    char DevName[50] = {0};
    char textMessage[100] = {0};
    PVFS_FILESYS VfsSys = &FileSys;
    int devNr = dev;
    
#if defined(_WIN32) || defined(_WIN64)
    snprintf(DevName, 50, "\\Device\\Harddisk%d\\Partition0",dev);
#else
    snprintf(DevName, 50, "/dev/sd%c",devNr+'b');
#endif
    printTextBrowser(0);
    snprintf(textMessage, 100, "Test device %s\n", DevName);
    printTextBrowser(textMessage);

    if(VfsDevtoLetter(DevName, DRIVER_MAGIC) == -1) {
        printTextBrowser("Dev not ready!\n");
        dev = 0;
    } else {
#if defined(_WIN32) || defined(_WIN64)
        sprintf(DevName, "%s", DRIVER_MAGIC);
#endif
        if(!NT_SUCCESS(VfsOpenDevice(VfsSys, (PUCHAR)DevName))){
            printTextBrowser("vfs:  Volume does not exist!\n");
#if defined(_WIN32) || defined(_WIN64)
            devNr = 0;
#else
            devNr++;
#endif
        }else{
            qDebug(" DEV OPEN OK");
            VfsGetMediaInfo(VfsSys);
            VfsInit.VfsSys = VfsSys;
            VfsInit.Init   = false;
            VFS_Open(&VfsInit);
            if(VFS_Mount(0) == FAT_OK){
                U32 idx;
                HD_VFS_HANDLER *File = (HD_VFS_HANDLER*)malloc(sizeof(HD_VFS_HANDLER));
            
                printTextBrowser(" Device mounted OK!\n");
                    
                VfsInit.Init   = true;
                *ping          = true;
                    
                *this->pFileCounter =0;
                for(idx = 0;idx < VFS_ROOT_ENTRIES;idx++){
                    if(VFS_GetFileInfobyIndex(idx,File) == FAT_OK){
                        if(File->Inode.status == INODE_BUSY){

                            lock->lock();
                            *this->pFileCounter = idx+1;
                            printf(" VFS loop 0x%x\n",(int)*this->pFileCounter);
                            AddNewVFSHandler(File);
                            while(*this->pFileCounter != 0xffffffff){ // sync thread
                                lock->unlock();
                                msleep(1);
                                lock->lock();
                            }
                            lock->unlock();
                                
                        }
                    }
                }
                free(File);
                return devNr;
            }
            VfsCloseDevice(VfsSys);
            devNr++;
        }
    }
    Unmount(DRIVER_MAGIC);
    return devNr;
}

void Disk_Thread::prepareFileName(char *Str)
{
    const char wrongchar[] = {0x05, 0x22,0x2f,0x5c,0x3a,0x2a,0x3c,0x3e,0x3f};
    U32 is,ix;
    for(is = 0;is < sizeof(wrongchar); is++){
        //printf("\n ");
        for(ix=0;ix<VFS_INODEN_NAME_LEN+5;ix++){
            //printf(" t 0x%x",Str[ix]);
            if(Str[ix] == wrongchar[is]){
                Str[ix] = ' ';
            }
            if(!Str[ix])break;
        }
    }
}

void Disk_Thread::transferCancel(void)
{
    static Filesfordownload DummyActFile={{0},0};
    printf(" transfer cancel request!\n");

    lock->lock();

    printf(" cancel granted!\n");

    if(VFSHandler){
        VFS_CloseFile(VFSHandler,FILE_CLOSE);
        VFSHandler = 0;
    }

    if(PCFile){
        PCFile->close();
        delete PCFile;
        PCFile =0;
    }

    if (REC2TSDest){
        REC2TSDest->close();
        delete REC2TSDest;
        REC2TSDest = 0;
    }
    
    if(ActFile)       
        ActFile = &DummyActFile;
    if(ActPc2BoxFile) 
        ActPc2BoxFile = &DummyActFile;

    if (pREC2TS)
        if (pREC2TS->index > 0)
            pREC2TS->index = 0;
    
    lock->unlock();
}

U32 Disk_Thread::CreateVFSEntry(char* name)
{
    HD_VFS_MARK_INFO  Info;
    FAT_ERROR err = FAT_FILE_OPEN_ERROR;
    U16 ix,len;
    U8  FileName[VFS_INODEN_NAME_LEN];

    memset(FileName,0x00,VFS_INODEN_NAME_LEN);
    strncpy((char*)FileName,name,VFS_INODEN_NAME_LEN-4);

    len = strlen((const char*)FileName);

    for(ix = 0;ix < 999; ix++){
        printf(" create_file -> %s\n",FileName);
        err = VFS_OpenFile(&VFSHandler,FileName,O_RDWR|O_CREAT);
        if(err == FAT_OUT_OF_SPACE){
            break;
        }else if(err != FAT_OK){
            //! find a new name
            printf(" try to find a new name %s\n",FileName);
            if(len>VFS_INODEN_NAME_LEN-4){
                FileName[len] = 0x00;
                FileName[len-1] = ((ix%10)+0x30);
                if(ix/10)
                    FileName[len-2] = (((ix/10)%10)+0x30);
                if(ix/100)
                    FileName[len-3] = ((ix/100)+0x30);
            } else {
                FileName[len] = ' ';
                if(ix<10){
                    FileName[len+1] = ((ix%10)+0x30);
                    FileName[len+2] = 0x00;
                }else if(ix<100){
                    FileName[len+1] = ((ix/10)+0x30);
                    FileName[len+2] = ((ix%10)+0x30);
                    FileName[len+3] = 0x00;
                }else{
                    FileName[len+1] = ((ix/100)+0x30);
                    FileName[len+2] = (((ix/10)%10)+0x30);
                    FileName[len+3] = ((ix%10)+0x30);
                    FileName[len+4] = 0x00;
                }
            }
        }else{
            printf(" VFS_open file -> OK!!!!\n");
            VFSHandler->Inode.FileEventMjd = 0;
            //! set start mark
            snprintf((char*)Info.markname,VFS_MARKINFO_NAME_LEN,"Start");
            Info.type                  = MARK_FSTART;
            Info.ActRecordNbr          = 0;
            Info.RecHdTimeStamp        = 0;
            Info.MarkCluster           = 0;
            Info.MarkClusterByteOffset = 0;
            VFS_SetMarkInfo(VFSHandler->EntryIDX,&Info);
            break;
        }
    }
    return (U32)err;
}

void Disk_Thread::OpenFilesForUpload(void)
{
    printf(" try to open %s\n",(char*)ActPc2BoxFile->EntryName);
    PCFile = new QFile((char*)ActPc2BoxFile->EntryName);
    if(!PCFile->open(QIODevice::ReadOnly)){
        delete PCFile;
        ActPc2BoxFile = 0;
        printf(" can't open file\n");
    }else{
        HD_VFS_PC_HEADER* pPC_Header = (HD_VFS_PC_HEADER*) malloc(sizeof(HD_VFS_PC_HEADER));
        PCFile->read((char*)pPC_Header,sizeof(HD_VFS_PC_HEADER));
        printf(" SI-EPGData-> %s\n",(char*)pPC_Header->Epg);
        if(strcmp((char*)pPC_Header->ver,VFS_PC_ACTVERSION)){
            PCFile->close();
            delete PCFile;
            ActPc2BoxFile = 0;
        }else{
            if(CreateVFSEntry((char*)ActPc2BoxFile->EntryName)){
                PCFile->close();
                delete PCFile;
                ActPc2BoxFile = 0;
                printf(" can't open VFS_file\n");
            }else{
                memset(&MarkInfo,0x00,sizeof(MarkInfo));
                HD_VFS_PutEventInfobyFileIDX(VFSHandler->EntryIDX,pPC_Header->Epg);
            }
        }
        free(pPC_Header);
    }
}

void Disk_Thread::OpenFilesForDownload(void)
{
    char Str[VFS_INODEN_NAME_LEN+5]={0};

    VFS_OpenFile(&VFSHandler, ActFile->EntryName ,O_RDWR);
    if(pFileDownload->type == TS_DOWNLOAD){
        snprintf(Str,VFS_INODEN_NAME_LEN+5,"%s.mpg",ActFile->EntryName);
    }else if(pFileDownload->type == REC_DOWNLOAD){
        snprintf(Str,VFS_INODEN_NAME_LEN+5,"%s.rec",ActFile->EntryName);
    }
    stripCtrlE((char *)Str);
    printf(" try to open %s\n",Str);
    prepareFileName(Str);
    PCFile = new QFile(Str);
    if(!PCFile->open(QIODevice::ReadWrite)){
        delete PCFile;
        ActFile = 0;
        printf(" can't open file\n");
    } else if (pFileDownload->type == REC_DOWNLOAD) {
        // Write REC file header
        char   Epg[512] = {0};
        HD_VFS_PC_HEADER* pPC_Header = (HD_VFS_PC_HEADER*) malloc(sizeof(HD_VFS_PC_HEADER));
        memset(pPC_Header->ver,0x00,sizeof(pPC_Header->ver));
        memset(pPC_Header->Epg,0x00,sizeof(pPC_Header->Epg));
        HD_VFS_GetEventInfobyFileIDX(VFSHandler->EntryIDX, (INT8U *)Epg);
        memcpy(pPC_Header->Epg,Epg,512);
        memset(pPC_Header->MarkInfo,0x00,sizeof(pPC_Header->MarkInfo));
        snprintf((char*)pPC_Header->ver,VFS_PC_VERSION_STR_LEN,"%s",VFS_PC_ACTVERSION);
        printf(" -> %s \n",pPC_Header->ver);
        PCFile->write((const char*)pPC_Header,sizeof(HD_VFS_PC_HEADER));
        free(pPC_Header);
    }
}

void Disk_Thread::OpenFilesForTS2REC()
{
    QString fileName = pREC2TS->fileList.at(pREC2TS->index);
    QString outFileName = fileName;
    char *buffer;
    size_t numread;
    
    outFileName.replace(".rec", ".ts", Qt::CaseInsensitive);
    printf("Converting <%s> to <%s>\n", fileName.toStdString().c_str(), outFileName.toStdString().c_str());
    PCFile = new QFile(fileName);
    REC2TSDest = new QFile(outFileName);
    
    if (!PCFile->open(QIODevice::ReadOnly)) {
        printf("Opening <%s> failed\n", fileName.toStdString().c_str());
        delete PCFile;
        delete REC2TSDest;
        return;
    }
    
    buffer = (char *)malloc(sizeof(char) * sizeof(HD_VFS_PC_HEADER));
    printf("Skipping header (%lu bytes)...\n", (unsigned long int)(sizeof(char) * sizeof(HD_VFS_PC_HEADER)));
    numread = PCFile->read(buffer, sizeof(char) * sizeof(HD_VFS_PC_HEADER));

    if ((numread < sizeof(HD_VFS_PC_HEADER)) || strncmp(buffer, VFS_PC_ACTVERSION, sizeof(VFS_PC_ACTVERSION))) {
        PCFile->close();
        delete PCFile;
        delete REC2TSDest;
        printf("<%s> does not have a record file header\n", fileName.toStdString().c_str());
        return;
    }
    
    free(buffer);

    if (!REC2TSDest->open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        PCFile->close();
        delete PCFile;
        delete REC2TSDest;
        printf("Opening <%s> failed", outFileName.toStdString().c_str());
        return;
    }
    pREC2TS->index++;
}

void Disk_Thread::StartUpload(void)
{
    lock->lock();                                             // called from MyFormThread

    printf(" ---------- start upload -----------------\n");
    ActPc2BoxFile = pPc2Box->FileList;
    if(ActPc2BoxFile){
        OpenFilesForUpload();
    }else{
        printf(" nothing available\n");
    }
    lock->unlock();
}

void Disk_Thread::StartDownload(void)
{
    lock->lock();

    printf(" ---------- start download ---------------\n");   // called from MyFormThread
    ActFile = pFileDownload->FileList;
    if(ActFile){
        OpenFilesForDownload();
    }else{
        printf(" nothing available\n");
    }
    lock->unlock();
}

void Disk_Thread::StartREC2TS(void)
{
    lock->lock();                                             // called from MyFormThread
    printf(" ---------- start REC->TS ---------------\n");
    if (!pREC2TS->fileList.isEmpty()){
        OpenFilesForTS2REC();
    }else{
        printf(" nothing available\n");
    }
    lock->unlock();
}

void Disk_Thread::FileUploadProcessing(void)
{
    lock->lock();
    if(ActPc2BoxFile){
        //printf("*\n");
        if(PCFile){
            FAT_ERROR           fat_err;
            HD_VFS_RECORD_INFO *pInfo;

            if(PCFile->read((char*)RecordBuffer,VFS_REC_SIZE) <= 0){
                printf(" VFS_EOF!!!-> set end\n");
                snprintf((char*)MarkInfo.markname,VFS_MARKINFO_NAME_LEN,"End");

                MarkInfo.type                 = MARK_FEND;
                MarkInfo.Idx                  = 1;
                VFS_SetMarkInfoIDX(VFSHandler->EntryIDX,1,&MarkInfo);
                MarkInfo.Idx = 1;
                VFS_SetVFSRecordInfo(&MarkInfo,1);
                VFSHandler->Inode.LastRecTime = MarkInfo.RecHdTimeStamp;
                VFS_CloseFile(VFSHandler,FILE_STORE);
                VFSHandler = 0;

                PCFile->close();
                delete PCFile;
                PCFile = 0;

            }else{

                pInfo = (HD_VFS_RECORD_INFO *)&RecordBuffer[VFS_TSPACK_SIZE];

                // qDebug("Rec -> %d t-> %d ",pInfo->ActRecordNbr,pInfo->RecHdTimeStamp);
                if(pInfo->ActRecordNbr == 1)pInfo->Linkused = 1;
                else                        pInfo->Linkused = 0;
                pInfo->MarkLink          = 0;

                if(!VFS_PutNByte(&VFSHandler,RecordBuffer,VFS_REC_SIZE,&fat_err)){
                    printf(" PutByte ERROR\n");
                }
                if(fat_err){
                    printf(" disk full !!!!\n");
                    snprintf((char*)MarkInfo.markname,VFS_MARKINFO_NAME_LEN,"End");
                    MarkInfo.type               = MARK_FEND;
                    MarkInfo.Idx                = 1;
                    VFS_SetMarkInfoIDX(VFSHandler->EntryIDX,1,&MarkInfo);
                    MarkInfo.Idx = 1;
                    VFS_SetVFSRecordInfo(&MarkInfo,1);
                    VFSHandler->Inode.LastRecTime = MarkInfo.RecHdTimeStamp;
                    //! close VFS
                    VFS_CloseFile(VFSHandler,FILE_STORE);

                    PCFile->close();
                    delete PCFile;
                    PCFile = 0;
                    static Filesfordownload DummyActFile={{0},0};
                    ActPc2BoxFile = &DummyActFile;

                }
                if(MarkInfo.ActRecordNbr+1 == pInfo->ActRecordNbr){
                    MarkInfo.ActRecordNbr          = pInfo->ActRecordNbr;
                    MarkInfo.RecHdTimeStamp        = pInfo->RecHdTimeStamp;
                    MarkInfo.MarkCluster           = VFSHandler->WriteActCluster;
                    MarkInfo.MarkClusterByteOffset = VFSHandler->WriteClusterOffset<<9;
                }

                DownloadBarInfo Bar;
                memset(&Bar,0x00,sizeof(DownloadBarInfo));
                snprintf((char*)Bar.InfoStr,LOAD_BAR_INFOST_LEN_MAX,"%s",(char*)VFSHandler->Inode.EntryName);
                if(pInfo->ActRecordNbr == 1){
                    updateDownloadbar(&Bar);
                }

                Bar.bar  = pInfo->ActRecordNbr*100;
                Bar.bar /= PCFile->size()/VFS_REC_SIZE;
                updateDownloadbar(&Bar);

                lock->unlock();
                return;
            }

        }else{
            ActPc2BoxFile = ActPc2BoxFile->next;
            if(ActPc2BoxFile){
                // open next file
                OpenFilesForUpload();
            }else{
                // no more files in queue
                Filesfordownload *ptr = pPc2Box->FileList,*tmp;
                while(ptr){
                    tmp = ptr;
                    ptr = ptr->next;
                    free(tmp);
                }
                pPc2Box->FileList = 0;

                DownloadBarInfo Bar;
                memset(&Bar,0x00,sizeof(DownloadBarInfo));
                Bar.bar = 0xffff0000;
                updateDownloadbar(&Bar);

                UpdateFileListWidget();
                printf(" ---------- END   upload -----------------\n");
            }
        }
    }
    lock->unlock();
    return;
}

void Disk_Thread::FileDownloadProcessing(void)
{
    lock->lock();
    if(ActFile){
        //printf("*\n");
        if(VFSHandler){
            U32 size;
            FAT_ERROR  err;
            size  = VFS_GetNByte(&VFSHandler,RecordBuffer,VFS_REC_SIZE,&err);
            if(size != VFS_REC_SIZE){
                printf("EOF\n");
                VFS_CloseFile(VFSHandler,FILE_CLOSE);
                // The next line terminates the entire single file transaction
                VFSHandler = 0;
                PCFile->close();
                delete PCFile;
            }else{
                // Copy a single record
                HD_VFS_RECORD_INFO *pInfo = (HD_VFS_RECORD_INFO *)&RecordBuffer[VFS_TSPACK_SIZE];
                //qDebug("Rec -> %d t-> %d ",pInfo->ActRecordNbr,pInfo->RecHdTimeStamp);
                if(pFileDownload->type == TS_DOWNLOAD){
                    // mpeg PART
                    PCFile->write((const char*)RecordBuffer,VFS_TSPACK_SIZE);
                }else if(pFileDownload->type == REC_DOWNLOAD){
                    // rec PART
                    PCFile->write((const char*)RecordBuffer,VFS_REC_SIZE);
                }

                DownloadBarInfo Bar;
                memset(&Bar,0x00,sizeof(DownloadBarInfo));
                Bar.bar  = pInfo->RecHdTimeStamp*100;
                Bar.bar /= VFSHandler->Inode.LastRecTime;
                snprintf((char*)Bar.InfoStr,LOAD_BAR_INFOST_LEN_MAX,"%s",(char*)VFSHandler->Inode.EntryName);
                updateDownloadbar(&Bar);

                lock->unlock();
                return;

            }
        }else{
            ActFile = ActFile->next;
            
            if (ActFile) { 
                // open the next file
                OpenFilesForDownload();
            }else{
                // del downloadlist
                Filesfordownload *ptr = pFileDownload->FileList,*tmp;
                while(ptr){
                    tmp = ptr;
                    ptr = ptr->next;
                    free(tmp);
                }
                pFileDownload->FileList = 0;
                DownloadBarInfo Bar;
                memset(&Bar,0x00,sizeof(DownloadBarInfo));
                Bar.bar = 0xffff0000;
                updateDownloadbar(&Bar);
                printf(" ---------- END   download ---------------\n");
            }
        }
    }
    lock->unlock();
    return;
}

void Disk_Thread::REC2TSProcessing(void)
{
    size_t numread, numwritten;
    char *buffer;
    
    lock->lock();
    if (pREC2TS == NULL) {
        lock->unlock();
        return;
    }
    
    if (pREC2TS->index > 0) {
        DownloadBarInfo Bar;
        memset(&Bar,0x00,sizeof(DownloadBarInfo));
        snprintf((char*)Bar.InfoStr,LOAD_BAR_INFOST_LEN_MAX,"%s",(char*)pREC2TS->fileList.at(pREC2TS->index-1).toStdString().c_str());
        Bar.bar  = PCFile->pos()*100 / PCFile->size();
        updateDownloadbar(&Bar);

        // Handle case that transfer was cancelled
        if (PCFile) {
            buffer = (char *)malloc(sizeof(char) * VFS_REC_SIZE);
            numwritten = VFS_TSPACK_SIZE;
            printf("Copying");
            
            numread = PCFile->read(buffer, sizeof(char) * VFS_REC_SIZE);
            if( numread > VFS_TSPACK_SIZE ) 
                numread = VFS_TSPACK_SIZE; // truncate to VFS_TSPACK_SIZE
        
            if (numread == VFS_TSPACK_SIZE) {
                numwritten = REC2TSDest->write(buffer, sizeof(char) * numread);
                free(buffer);
            } else {
                // file end
                free(buffer);
                PCFile->close();
                REC2TSDest->close();
                delete PCFile;
                PCFile = 0;
                delete REC2TSDest;
                REC2TSDest = 0;
                if (pREC2TS->index == pREC2TS->fileList.size()) {
                    pREC2TS->index = 0;
                    DownloadBarInfo Bar;
                    memset(&Bar,0x00,sizeof(DownloadBarInfo));
                    Bar.bar = 0xffff0000;
                    updateDownloadbar(&Bar);
                    printf(" ---------- END   REC->TS -----------------\n");
                } else
                    OpenFilesForTS2REC();
            }
        }
    }
    lock->unlock();
    return;
}


void Disk_Thread::run(void)
{
    fpPolling   fpDiskPoll = &Disk_Thread::init_vfs;
    int count      = 0;
#if !defined(_WIN32) && !defined(_WIN64)
    int countPrev  = count;
#endif
    bool pingpong= false;

    printf("start thread\n");
    VfsInit.Init = false;

    ActFile       = 0;
    ActPc2BoxFile = 0;
    VFSHandler    = 0;
    
#if !defined(_WIN32) && !defined(_WIN64)
    // Linux only: single poll, no re-open as files/devices are not locked
    count = (this->*fpDiskPoll)(countPrev,&pingpong);
    while (count != countPrev) {
        countPrev = count % 25;
        count = (this->*fpDiskPoll)(countPrev,&pingpong);
        msleep(1);
    }
#endif

    for(;;){
#if defined(_WIN32) || defined (_WIN64)
        //---disk_mount_unmount-----
        // Windows only: constant re-polling
        count = (this->*fpDiskPoll)(count,&pingpong);
        if(pingpong){
            if(fpDiskPoll == &Disk_Thread::init_vfs){
                fpDiskPoll = &Disk_Thread::Poll_vfs;
            }else{
                fpDiskPoll = &Disk_Thread::init_vfs;
            }
            pingpong = false;
        }
#endif
        //--------------------------
        //---file processing--------
        //--------------------------
        if(ActFile){
            FileDownloadProcessing();
        } else if (ActPc2BoxFile) {
            FileUploadProcessing();
        } else if (pREC2TS->index > 0) {
            REC2TSProcessing();
        }
        // Give up timeslice for main thread
        msleep(1);
    }
}
