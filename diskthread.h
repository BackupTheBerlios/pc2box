/*
 * diskthread.h - pc2box disk thread header
 *
 *  This code implements the thread that performs the disk operations
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
#include <qthread.h>
#include <QMutexLocker>
#include <QFile>
#include "types.h"
#include "vfs.h"

#define LOAD_BAR_INFOST_LEN_MAX 200

typedef struct barinfo {
    U32 bar;
    U8 InfoStr[LOAD_BAR_INFOST_LEN_MAX];
} DownloadBarInfo;

typedef struct DownloadFiles {
    U8 EntryName[VFS_INODEN_NAME_LEN+5];  //! name
    struct DownloadFiles *next;
} Filesfordownload;

typedef struct {
    U32 fileCount;
    U32 type;
    Filesfordownload *FileList;
} FileDownload;

typedef struct {
    std::string a;
    char *strOutput;
    HD_VFS_HANDLER *pFileHandler;
    QMutex *pLock;
    U32 *fileCounter;
    FileDownload *pFileDownload;
    FileDownload *pPc2Box;
    DownloadBarInfo *pDownloadBar;
} THREAD_Params;

class Disk_Thread : public QThread{
    Q_OBJECT

public:
    Disk_Thread(THREAD_Params *pParams=0);
    virtual void    run();

    char             *strOutput;
    HD_VFS_HANDLER   *pActVfsHandler;
    QMutex           *lock;
    U32              *pFileCounter;
    FileDownload     *pFileDownload;
    FileDownload     *pPc2Box;
    DownloadBarInfo  *pDownloadBar;

    Filesfordownload *ActFile;
    Filesfordownload *ActPc2BoxFile;
    HD_VFS_HANDLER   *VFSHandler;
    QFile            *PCFile;
    HD_VFS_MARK_INFO MarkInfo;
    U8               RecordBuffer[VFS_REC_SIZE+500];
    U32              FileDownloadProcessing(void);
    U32              FileUploadProcessing(void);
    void             prepareFileName(char *Str);
    U32              CreateVFSEntry(char* name);

    int              init_vfs(int dev, bool *ping);
    int              Poll_vfs(int dev, bool *ping);

 signals:
    void      beep();
    void      FileListbeep();
    void      DiskRemovebeep();
    void      UpdateBarbeep();

    public slots:
    void      StartDownload();
    void      StartUpload();
    void      transferCancel();

 private:
    std::string     name;
    VFS_FILESYS     FileSys;
    HD_VFS_INIT     VfsInit;
    void printTextBrowser(const char *str);
    void AddNewVFSHandler(HD_VFS_HANDLER *pVfsHandler);
    void updateDownloadbar(DownloadBarInfo *pInfo);
    void OpenFilesForUpload(void);
    void UpdateFileListWidget(void);
};

typedef int (Disk_Thread::*fpPolling)(int, bool *);
