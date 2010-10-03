/*
 * myform.h - pc2box user interface header
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
#ifndef MYFORM_H
#define MYFORM_H

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
#include <QWidget>
#include <QMessageBox>
#include <QProgressDialog>
#include "types.h"
#include "vfs.h"
#include "diskthread.h"

#define SW_INFO_ST " TL Version: pc2box v_2.0.9 "
#define TEXTBROWSER_STR_LEN 0x500


class MyForm : public QWidget
{
    Q_OBJECT

public:
    MyForm();
    ~MyForm();

signals:
    void StartDownloadbeep();
    void Startpc2boxbeep();
    void StartREC2TSbeep();

public slots:
    virtual void ShowApplicationInfo();
    virtual void LoadRecFiles();
    virtual void LoadTSFiles();
    virtual void pc2box();
    virtual void Rec2TS();

    void         gotBeep();
    void         updateFileListWidget();
    void         clearFileListWidget();
    void         DisplayLoadBar();

private:
    QGridLayout     *gridLayout;
    QSpacerItem     *spacerItem;
    QTextBrowser    *textBrowser;
    QPushButton     *pushButtonLoadTS;
    QPushButton     *pushButtonLoadREC;
    QPushButton     *pushButtonPc2box;
    QPushButton     *pushButtonREC2TS;
    QPushButton     *pushButtonInfo;
    QPushButton     *pushButtonQuit;
    QTreeWidget     *treeWidget;
    QSpacerItem     *spacerItem1;
    QProgressDialog *pd;
    Disk_Thread     *a;

    QMutex          lock;
    char            TextBrowserStr[TEXTBROWSER_STR_LEN];
    HD_VFS_HANDLER  ActVfsHandler;
    U32             FileCounter;
    FileDownload    FilestoDownload;
    DownloadBarInfo DownloadBar;
    FileDownload    Filespc2box;
    FileREC2TS      FilesREC2TS;

    U32 countSelectedFiles(void);
    QStringList getFileNames();
    void resize(QWidget *Form);
    void retranslateUi(QWidget *Form);
    void AddNewFileForDownload(void);
};

#endif
