/*
 * MyForm.h - pc2box user interface header
 *
 *  This code implements the QT-based user interface for pc2box
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
#ifndef UI_FORM1_H
#define UI_FORM1_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTextBrowser>
#include <QtGui/QTreeWidget>
#include <QtGui/QCheckBox>
#include <QtGui/QWidget>
#include <QObject>
#include <qwidget.h>
#include <qmessagebox.h>
#include <qthread.h>
#include <QMutexLocker>
#include <QReadWriteLock>
#include <QFile>
#include <QProgressDialog>
#include <QDir>
#include "types.h"
#include "vfs.h"

#define SW_INFO_ST " TL Version: pc2box v_2.0.3 "
#define TEXTBROWSER_STR_LEN 0x500

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
    char *str;
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

    char             *Str;
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

class MyForm : public QWidget
{
    Q_OBJECT

public:
    MyForm( QWidget* parent = 0, const char* name = 0);
    ~MyForm();

    QGridLayout     *gridLayout;
    QSpacerItem     *spacerItem;
    QTextBrowser    *textBrowser;
    QPushButton     *pushButton_2;
    QPushButton     *pushButton_4;
    QPushButton     *pushButton_3;
    QTreeWidget     *treeWidget;
    QSpacerItem     *spacerItem1;
    QPushButton     *pushButton;
    QPushButton     *pushButton_5;
    QProgressDialog *pd;
    Disk_Thread     *a;

    QMutex          lock;
    char            TextBrowserStr[TEXTBROWSER_STR_LEN];
    HD_VFS_HANDLER  ActVfsHandler;
    U32             FileCounter;
    FileDownload    FilestoDownload;
    DownloadBarInfo DownloadBar;
    FileDownload    Filespc2box;

signals:
    void StartDownloadbeep();
    void Startpc2boxbeep();

public slots:
    virtual void ShowApplicationInfo();
    virtual void LoadRecFiles();
    virtual void LoadTSFiles();
    virtual void pc2box();
    void         gotBeep();
    void         updateFileListWidget();
    void         clearFileListWidget();
    void         DisplayLoadBar();

private:
    U32 countSelectedFiles(void);
    void resize(QWidget *Form);
    void retranslateUi(QWidget *Form);
    void AddNewFileForDownload(void);
};


#endif
