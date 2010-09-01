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

#define SW_INFO_ST " TL Version: pc2box v_2.0.1 "
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

#if defined(_WIN32) || defined(_WIN64)
    int              init_vfs(int dev,int *ping);
    int              Poll_vfs(int dev,int *ping);
#else
    int              init_vfs(int dev, bool *ping);
    int              Poll_vfs(int dev, bool *ping);
#endif

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
    void printTextBrowser(const char *str){
        if(str){
            sprintf(this->Str,"%s",str);
        }else this->Str[0] = 0x00;
        emit beep();
        msleep(10); // update unsynced, wait a while to be sure

    }
    void AddNewVFSHandler(HD_VFS_HANDLER *pVfsHandler){
        if(pActVfsHandler){
            *pActVfsHandler = *pVfsHandler;
            emit FileListbeep();
        }else{printf("\n VFSHandler not init");}
    }
    void updateDownloadbar(DownloadBarInfo *pInfo){
        printf("\n bar -> %d ",(int)pInfo->bar);
        *pDownloadBar = *pInfo;
        emit UpdateBarbeep();
        while(pDownloadBar->bar != 0xffffffff){ // sync it
            lock->unlock();
            msleep(1);
            lock->lock();
        }
    }
    void OpenFilesForUpload(void);
    void UpdateFileListWidget(void);

};

#if defined(_WIN32) || defined(_WIN64)
typedef int (Disk_Thread::*fpPolling)(int, int *);
#else
typedef int (Disk_Thread::*fpPolling)(int, bool *);
#endif

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
    void         gotBeep() {
        if(!TextBrowserStr[0])
            textBrowser->clear();
        else textBrowser->insertPlainText(TextBrowserStr);
    }
    void         updateFileListWidget();
    void         clearFileListWidget();
    void         DisplayLoadBar();

private:
    U32 countSelectedFiles(void)
    {
        U32 ix,is=0;
        QTreeWidgetItem *item;
        //printf("\n load files as TS ");
        for(ix=0;ix<VFS_ROOT_ENTRYS;ix++){
            item = treeWidget->topLevelItem(ix);
#if defined(_WIN32) || defined(_WIN64)
            printf("\n item %x %x",(int)ix,(int)item);
#else
            printf("\n item %x",(int)ix);
#endif
            if(item){
                Qt::CheckState itemState = qobject_cast<QCheckBox*>(treeWidget->itemWidget(item, 0))->checkState();
                if(itemState == Qt::Checked){
                    is++;
                }
            }else{
                break;
            }
        }
        if(!is){
            QMessageBox::warning( this, tr(" ERROR "),tr(" nicht's selektiert? "));
            return 0;
        }else return is;
    }
    void resize(QWidget *Form)
    {
        QSize size(543, 363);
        size = size.expandedTo(Form->minimumSizeHint());
        Form->resize(size);
    }
    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(QApplication::translate("Form", "pc2Box", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("Form", "pc2box", 0, QApplication::UnicodeUTF8));
        pushButton_4->setText(QApplication::translate("Form", "TS laden", 0, QApplication::UnicodeUTF8));
        pushButton_3->setToolTip(QString());
        pushButton_3->setText(QApplication::translate("Form", "Rec laden", 0, QApplication::UnicodeUTF8));
        treeWidget->headerItem()->setText(0, QApplication::translate("Form", "Nbr", 0, QApplication::UnicodeUTF8));
        treeWidget->headerItem()->setText(1, QApplication::translate("Form", "Name", 0, QApplication::UnicodeUTF8));
        treeWidget->headerItem()->setText(2, QApplication::translate("Form", "Gr\303\266sse", 0, QApplication::UnicodeUTF8));
        treeWidget->headerItem()->setText(3, QApplication::translate("Form", "Zeit", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("Form", "Ende", 0, QApplication::UnicodeUTF8));
        pushButton_5->setText(QApplication::translate("Form", "Info", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(Form);
    } // retranslateUi
    void AddNewFileForDownload(void)
    {
        U32 is;
        for(is=0;is<VFS_ROOT_ENTRYS;is++){
            QTreeWidgetItem *item = treeWidget->topLevelItem(is);
            if(item){
                Qt::CheckState itemState = qobject_cast<QCheckBox*>(treeWidget->itemWidget(item, 0))->checkState();
                if(itemState == Qt::Checked){

                    Filesfordownload *File = (Filesfordownload*) malloc(sizeof(Filesfordownload));
                    Filesfordownload *ptr  =  FilestoDownload.FileList;

                    memset((char*)File,0x00,sizeof(Filesfordownload));
                    if(ptr){
                        while(ptr->next){
                            printf("\n next entry !!!!!");
                            ptr = ptr->next;
                        }
                        ptr->next = File;
                    }else{
                        printf("\n first entry !!!!!");
                        FilestoDownload.FileList = File;
                    }

                    printf("\n name ->");
                    QString  FileName = item->text(1);
                    U32      ix       = FileName.size();
                    QChar   *data     = FileName.data();
                    char  *Str        = (char*)&File->EntryName[0];
                    memset(Str,0x00,VFS_INODEN_NAME_LEN);
                    if(ix < VFS_INODEN_NAME_LEN){
                        while(ix--) {
                            *Str++ = data->unicode();
                            data++;
                        }
                    }else{printf(" VFS_INODEN_NAME_LEN ");}
                    printf("%s",(char*)File->EntryName);

                }
            }
        }
        emit StartDownloadbeep();

    }
};


#endif
