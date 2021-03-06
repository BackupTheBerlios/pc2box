/*
 * myform.cpp - pc2box user interface
 *
 *  This code implements the QT-based user interface for pc2box
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
#include <QFileDialog>
#include <QString>
#include "myform.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>
#include "vfs.h"

MyForm::MyForm()
{
    setObjectName( "MyForm" );
    setWindowIcon(QIcon(QString::fromUtf8("gs_t.xpm")));

    gridLayout  = new QGridLayout(this);
    gridLayout->setSpacing(6);
    gridLayout->setMargin(9);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    spacerItem = new QSpacerItem(20, 71, QSizePolicy::Minimum, QSizePolicy::Expanding);
    gridLayout->addItem(spacerItem, 3, 0, 1, 1);

    textBrowser = new QTextBrowser(this);
    textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
    textBrowser->setMinimumSize(QSize(250, 0));

    QPalette palette;
    QBrush brush(QColor(0, 255, 0, 255));
    brush.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
    QBrush brush1(QColor(212, 208, 200, 255));
    brush1.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::Button, brush1);
    QBrush brush2(QColor(0, 0, 0, 255));
    brush2.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::Light, brush2);
    QBrush brush3(QColor(234, 229, 220, 255));
    brush3.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::Midlight, brush3);
    QBrush brush4(QColor(128, 128, 128, 255));
    brush4.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::Dark, brush4);
    QBrush brush5(QColor(141, 139, 133, 255));
    brush5.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::Mid, brush5);
    QBrush brush6(QColor(0, 255, 0, 255));
    brush6.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::Text, brush6);
    QBrush brush7(QColor(255, 255, 255, 255));
    brush7.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::BrightText, brush7);
    QBrush brush8(QColor(0, 0, 0, 255));
    brush8.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::ButtonText, brush8);
    QBrush brush9(QColor(0, 0, 0, 255));
    brush9.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::Base, brush9);
    QBrush brush10(QColor(212, 208, 200, 255));
    brush10.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::Window, brush10);
    QBrush brush11(QColor(64, 64, 64, 255));
    brush11.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::Shadow, brush11);
    QBrush brush12(QColor(10, 36, 106, 255));
    brush12.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::Highlight, brush12);
    QBrush brush13(QColor(255, 255, 255, 255));
    brush13.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::HighlightedText, brush13);
    QBrush brush14(QColor(0, 0, 255, 255));
    brush14.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::Link, brush14);
    QBrush brush15(QColor(255, 0, 255, 255));
    brush15.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::LinkVisited, brush15);
    QBrush brush16(QColor(232, 232, 232, 255));
    brush16.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::AlternateBase, brush16);
    QBrush brush17(QColor(0, 255, 0, 255));
    brush17.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush17);
    QBrush brush18(QColor(212, 208, 200, 255));
    brush18.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Inactive, QPalette::Button, brush18);
    QBrush brush19(QColor(0, 0, 0, 255));
    brush19.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Inactive, QPalette::Light, brush19);
    QBrush brush20(QColor(234, 229, 220, 255));
    brush20.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Inactive, QPalette::Midlight, brush20);
    QBrush brush21(QColor(128, 128, 128, 255));
    brush21.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Inactive, QPalette::Dark, brush21);
    QBrush brush22(QColor(141, 139, 133, 255));
    brush22.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Inactive, QPalette::Mid, brush22);
    QBrush brush23(QColor(0, 255, 0, 255));
    brush23.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Inactive, QPalette::Text, brush23);
    QBrush brush24(QColor(255, 255, 255, 255));
    brush24.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Inactive, QPalette::BrightText, brush24);
    QBrush brush25(QColor(0, 0, 0, 255));
    brush25.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Inactive, QPalette::ButtonText, brush25);
    QBrush brush26(QColor(0, 0, 0, 255));
    brush26.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Inactive, QPalette::Base, brush26);
    QBrush brush27(QColor(212, 208, 200, 255));
    brush27.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Inactive, QPalette::Window, brush27);
    QBrush brush28(QColor(64, 64, 64, 255));
    brush28.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Inactive, QPalette::Shadow, brush28);
    QBrush brush29(QColor(212, 208, 200, 255));
    brush29.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Inactive, QPalette::Highlight, brush29);
    QBrush brush30(QColor(0, 0, 0, 255));
    brush30.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Inactive, QPalette::HighlightedText, brush30);
    QBrush brush31(QColor(0, 0, 255, 255));
    brush31.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Inactive, QPalette::Link, brush31);
    QBrush brush32(QColor(255, 0, 255, 255));
    brush32.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Inactive, QPalette::LinkVisited, brush32);
    QBrush brush33(QColor(232, 232, 232, 255));
    brush33.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush33);
    QBrush brush34(QColor(106, 104, 100, 255));
    brush34.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush34);
    QBrush brush35(QColor(212, 208, 200, 255));
    brush35.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Disabled, QPalette::Button, brush35);
    QBrush brush36(QColor(0, 0, 0, 255));
    brush36.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Disabled, QPalette::Light, brush36);
    QBrush brush37(QColor(233, 231, 227, 255));
    brush37.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Disabled, QPalette::Midlight, brush37);
    QBrush brush38(QColor(128, 128, 128, 255));
    brush38.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Disabled, QPalette::Dark, brush38);
    QBrush brush39(QColor(141, 139, 133, 255));
    brush39.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Disabled, QPalette::Mid, brush39);
    QBrush brush40(QColor(106, 104, 100, 255));
    brush40.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Disabled, QPalette::Text, brush40);
    QBrush brush41(QColor(255, 255, 255, 255));
    brush41.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Disabled, QPalette::BrightText, brush41);
    QBrush brush42(QColor(106, 104, 100, 255));
    brush42.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush42);
    QBrush brush43(QColor(212, 208, 200, 255));
    brush43.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Disabled, QPalette::Base, brush43);
    QBrush brush44(QColor(212, 208, 200, 255));
    brush44.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Disabled, QPalette::Window, brush44);
    QBrush brush45(QColor(0, 0, 0, 255));
    brush45.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Disabled, QPalette::Shadow, brush45);
    QBrush brush46(QColor(10, 36, 106, 255));
    brush46.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Disabled, QPalette::Highlight, brush46);
    QBrush brush47(QColor(255, 255, 255, 255));
    brush47.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Disabled, QPalette::HighlightedText, brush47);
    QBrush brush48(QColor(0, 0, 255, 255));
    brush48.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Disabled, QPalette::Link, brush48);
    QBrush brush49(QColor(255, 0, 255, 255));
    brush49.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Disabled, QPalette::LinkVisited, brush49);
    QBrush brush50(QColor(232, 232, 232, 255));
    brush50.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush50);
    textBrowser->setPalette(palette);
    textBrowser->setCursor(QCursor(static_cast<Qt::CursorShape>(10)));

    gridLayout->addWidget(textBrowser, 0, 1, 5, 2);

    pushButtonPc2box = new QPushButton(this);
    pushButtonPc2box->setObjectName(QString::fromUtf8("pushButtonPc2box"));

    gridLayout->addWidget(pushButtonPc2box, 2, 0, 1, 1);

    pushButtonLoadTS = new QPushButton(this);
    pushButtonLoadTS->setObjectName(QString::fromUtf8("pushButtonLoadTS"));

    gridLayout->addWidget(pushButtonLoadTS, 1, 0, 1, 1);

    pushButtonLoadREC = new QPushButton(this);
    pushButtonLoadREC->setObjectName(QString::fromUtf8("pushButtonLoadREC"));
    pushButtonLoadREC->setCursor(QCursor(static_cast<Qt::CursorShape>(13)));

    gridLayout->addWidget(pushButtonLoadREC, 0, 0, 1, 1);

    treeWidget = new QTreeWidget(this);
    treeWidget->setObjectName(QString::fromUtf8("treeWidget"));
    treeWidget->setFrameShape(QFrame::HLine);
    treeWidget->setUniformRowHeights(false);
    treeWidget->setSortingEnabled(true);
    treeWidget->setColumnCount(4);

    gridLayout->addWidget(treeWidget, 5, 0, 1, 3);
    spacerItem1 = new QSpacerItem(401, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout->addItem(spacerItem1, 6, 0, 1, 2);

    pushButtonQuit = new QPushButton(this);
    pushButtonQuit->setObjectName(QString::fromUtf8("pushButton"));
    pushButtonQuit->setCursor(QCursor(static_cast<Qt::CursorShape>(13)));

    gridLayout->addWidget(pushButtonQuit, 6, 2, 1, 1);

    pushButtonInfo = new QPushButton(this);
    pushButtonInfo->setObjectName(QString::fromUtf8("pushButtonInfo"));

    gridLayout->addWidget(pushButtonInfo, 4, 0, 1, 1);

    pushButtonREC2TS = new QPushButton(this);
    pushButtonREC2TS->setObjectName(QString::fromUtf8("pushButtonREC2TS"));

    gridLayout->addWidget(pushButtonREC2TS, 3, 0, 1, 1);

    QWidget::setTabOrder(treeWidget  , textBrowser);
    QWidget::setTabOrder(textBrowser , pushButtonLoadREC);
    QWidget::setTabOrder(pushButtonLoadREC, pushButtonQuit);
    QWidget::setTabOrder(pushButtonQuit  , pushButtonLoadTS);
    QWidget::setTabOrder(pushButtonLoadTS, pushButtonPc2box);
    QWidget::setTabOrder(pushButtonPc2box, pushButtonREC2TS);
    QWidget::setTabOrder(pushButtonREC2TS, pushButtonInfo);

    resize(this);

    QAction* epgAction = new QAction("&EPG...", treeWidget);
    treeWidget->addAction(epgAction);
    QObject::connect(epgAction, SIGNAL(triggered()), this, SLOT(showEPGInfo()));
    treeWidget->setContextMenuPolicy(Qt::ActionsContextMenu);

    QObject::connect(pushButtonQuit   , SIGNAL(clicked(bool)), this, SLOT(close()));
    QObject::connect(pushButtonInfo , SIGNAL(clicked(bool)), this, SLOT(ShowApplicationInfo()));
    QObject::connect(pushButtonPc2box , SIGNAL(clicked(bool)), this, SLOT(pc2box()));
    QObject::connect(pushButtonLoadREC , SIGNAL(clicked(bool)), this, SLOT(LoadRecFiles()));
    QObject::connect(pushButtonLoadTS , SIGNAL(clicked(bool)), this, SLOT(LoadTSFiles()));
    QObject::connect(pushButtonREC2TS , SIGNAL(clicked(bool)), this, SLOT(Rec2TS()));

    retranslateUi(this);

    // Mandatory initializations - without these, download does not start on Linux
    this->FilestoDownload.FileList = 0;
    this->Filespc2box.FileList = 0;

    FilesREC2TS.index = 0;
    FilesREC2TS.fileList.clear();

    // Setup data structures that are shared with the disk thread
    THREAD_Params ThreadParams;
    ThreadParams.strOutput      = this->TextBrowserStr;
    ThreadParams.pFileHandler   = &this->ActVfsHandler;
    ThreadParams.pLock          = &this->lock;
    ThreadParams.fileCounter    = &this->FileCounter;
    ThreadParams.pFileDownload  = &this->FilestoDownload;
    ThreadParams.pDownloadBar   = &this->DownloadBar;
    ThreadParams.pPc2Box        = &this->Filespc2box;
    ThreadParams.pREC2TS        = &this->FilesREC2TS;

    pd = new QProgressDialog(" nothing ", "Cancel", 0 ,100, this );
    pd->setAutoReset(0);
    pd->setWindowModality(Qt::WindowModal);

    //! Start threading
    a = new Disk_Thread(&ThreadParams);

    // thread->myform communication
    QObject::connect( a, SIGNAL( beep() ), this, SLOT( gotBeep() ) );                       // update Textbrowser
    QObject::connect( a, SIGNAL( FileListbeep() ), this, SLOT( updateFileListWidget() ) );  // update FileListWidget
    QObject::connect( a, SIGNAL( DiskRemovebeep() ), this, SLOT( clearFileListWidget() ) ); // clear  FileListWidget
    QObject::connect( a, SIGNAL( UpdateBarbeep() ), this, SLOT( DisplayLoadBar() ) );       // update progress bar
    // myform->thread communication
    QObject::connect( this, SIGNAL( StartDownloadbeep() ), a, SLOT( StartDownload() ) );    // start  download
    QObject::connect( this, SIGNAL( Startpc2boxbeep() )  , a, SLOT( StartUpload() ) );      // start  upload/pc2box
    QObject::connect( this, SIGNAL( StartREC2TSbeep() )  , a, SLOT( StartREC2TS() ) );      // start  REC->TS
    // progress dialog->thread communication
    QObject::connect(pd, SIGNAL(canceled()), a, SLOT(transferCancel()));
    a->start();
}

MyForm::~MyForm(){
    //! thread wait fin
    a->quit();
}

void MyForm::ShowApplicationInfo(){
    QMessageBox::information( this, tr(" Info "),tr(SW_INFO_ST));
}

void MyForm::clearFileListWidget(){
    printf("----------- clear FileListWidget --------------\n");
    lock.lock();
    treeWidget->clear();
    FileCounter = 0xffffffff;
    lock.unlock(); // free mutex
}

void MyForm::DisplayLoadBar(){
    lock.lock();
    //printf("LoadBar %s %d\n",DownloadBar.InfoStr,(int)DownloadBar.bar);
    if(!DownloadBar.bar){
        // start popup
        pd->setLabelText( (char*)DownloadBar.InfoStr);
        //pd->sizeHint();
        pd->show();
        pd->setValue(0);
    }else
        if(DownloadBar.bar == 0xffff0000){
            // close popup
            U32 ix;
            QTreeWidgetItem *item=0;

            // uncheck all items
            for(ix=0;ix<VFS_ROOT_ENTRIES;ix++){
                item = treeWidget->topLevelItem(ix);
                printf(" item %i %p\n",(int)ix,item);
                if(item){
                    Qt::CheckState itemState = qobject_cast<QCheckBox*>(treeWidget->itemWidget(item, 0))->checkState();
                    if(itemState == Qt::Checked){
                        qobject_cast<QCheckBox*>(treeWidget->itemWidget(item, 0))->setCheckState(Qt::Unchecked);
                    }
                }else{
                    break;
                }
            }

            QObject::disconnect(pd, SIGNAL(canceled()), a, SLOT(transferCancel()));
            pd->close();
            QObject::connect(pd, SIGNAL(canceled()), a, SLOT(transferCancel()));

        }else{
            // update popup
            if(DownloadBar.bar > 100)
                pd->setValue(100);
            else pd->setValue(DownloadBar.bar);
        }
    DownloadBar.bar = 0xffffffff; // release Disk_Thread
    lock.unlock();
}

// Add a single entry to the file list, triggered by FileListbeep
void MyForm::updateFileListWidget(){
    char *Str;
    U32 Flen;
    QTreeWidgetItem *item   = new QTreeWidgetItem(treeWidget);
    QCheckBox *itemCheckBox = new QCheckBox(treeWidget);

    lock.lock();
    Str = (char *)malloc(20);
    Str[0] = 0;
    printf(" item %lu %p\n",FileCounter,item);
    // Internal representation requires leading zeroes so that rows get correctly sorted
    snprintf(Str,20,"%04lu",FileCounter);
    // Disable tree widget row selection to prevent duplicate values from
    // re-appearing in first column
    item->setFlags(Qt::ItemIsEnabled);
    item->setText( 0,(const char*)Str);
    // Make first column values invisible. Still required for row sorting 
    item->setForeground(0, QBrush(Qt::transparent, Qt::NoBrush));
    // GUI representation->without sorting
    snprintf(Str,20,"%lu",FileCounter);
    itemCheckBox->setText(Str);
    treeWidget->setItemWidget(item, 0, itemCheckBox);

    item->setText( 1,tr((const char*)ActVfsHandler.Inode.EntryName));

    Str[0] = 0x00;
    Flen   = ActVfsHandler.Inode.sizecluster;
    if(Flen>0){
        Flen--; // use sizeinlastcluster instead
        Flen *= VFS_GetClusterSize();
        Flen  = Flen>>1;
        Flen += ActVfsHandler.Inode.sizeinlastcluster>>10;
        Flen += sizeof(HD_VFS_PC_HEADER)>>10;
    }
    snprintf(Str,20,"%d Kbyte",(int)Flen);
    item->setText( 2,(const char*)Str);

    Str[0] = 0x00;
    snprintf(Str,20,"%d:%02d",ActVfsHandler.Inode.LastRecTime/60,ActVfsHandler.Inode.LastRecTime%60);
    item->setText( 3,(const char*)Str);

    FileCounter = 0xffffffff;

    free(Str);
    lock.unlock(); // free mutex
}

void MyForm::LoadRecFiles(){
    U32 is = countSelectedFiles();
    printf(" load files as REC\n");
    printf(" %d files selected\n",(int)is);
    if(!is)
        return;

    //lock.lock();

    FilestoDownload.fileCount = is;
    FilestoDownload.type      = REC_DOWNLOAD;
    if(!FilestoDownload.FileList) {
        AddNewFileForDownload();
    } else {
        printf(" download in progress\n");
    }
    //lock.unlock();
}

void MyForm::LoadTSFiles(){
    U32 is = countSelectedFiles();
    printf(" load files as TS\n");
    printf(" %d files selected\n",(int)is);
    if(!is)
        return;

    //lock.lock();

    FilestoDownload.fileCount = is;
    FilestoDownload.type      = TS_DOWNLOAD;
    if(!FilestoDownload.FileList){
        AddNewFileForDownload();
    } else {
        printf(" download in progress\n");
    }

    //lock.unlock();
}

void MyForm::resize(QWidget *Form)
{
    QSize size(543, 363);
    size = size.expandedTo(Form->minimumSizeHint());
    Form->resize(size);
}

void MyForm::retranslateUi(QWidget *Form)
{
    Form->setWindowTitle(QApplication::translate("Form", "pc2Box", 0, QApplication::UnicodeUTF8));
    pushButtonPc2box->setText(QApplication::translate("Form", "pc2box...", 0, QApplication::UnicodeUTF8));
    pushButtonPc2box->setToolTip(QString("Rohformat-Datei auf Receiver Uebertragen"));
    pushButtonLoadTS->setText(QApplication::translate("Form", "TS laden", 0, QApplication::UnicodeUTF8));
    pushButtonLoadTS->setToolTip(QString("Stream im MPEG-TS Format speichern"));
    pushButtonLoadREC->setText(QApplication::translate("Form", "Rec laden", 0, QApplication::UnicodeUTF8));
    pushButtonLoadREC->setToolTip(QString("Stream im Rohformat speichern"));
    pushButtonREC2TS->setText(QApplication::translate("Form", "REC->TS...", 0, QApplication::UnicodeUTF8));
    pushButtonREC2TS->setToolTip(QString("Rohformat-Datei in MPEG-TS wandeln"));
    pushButtonInfo->setText(QApplication::translate("Form", "Info...", 0, QApplication::UnicodeUTF8));
    pushButtonQuit->setText(QApplication::translate("Form", "Ende", 0, QApplication::UnicodeUTF8));
    treeWidget->headerItem()->setText(0, QApplication::translate("Form", "Nummer", 0, QApplication::UnicodeUTF8));
    treeWidget->headerItem()->setText(1, QApplication::translate("Form", "Name", 0, QApplication::UnicodeUTF8));
    treeWidget->headerItem()->setText(2, QApplication::translate("Form", "Gr\303\266sse", 0, QApplication::UnicodeUTF8));
    treeWidget->headerItem()->setText(3, QApplication::translate("Form", "Zeit", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(Form);
}

void MyForm::AddNewFileForDownload(void)
{
    U32 is;
    for(is=0;is<VFS_ROOT_ENTRIES;is++){
        QTreeWidgetItem *item = treeWidget->topLevelItem(is);
        if(item){
            Qt::CheckState itemState = qobject_cast<QCheckBox*>(treeWidget->itemWidget(item, 0))->checkState();
            if(itemState == Qt::Checked){

                Filesfordownload *File = (Filesfordownload*) malloc(sizeof(Filesfordownload));
                Filesfordownload *ptr  =  FilestoDownload.FileList;

                memset((char*)File,0x00,sizeof(Filesfordownload));
                if(ptr){
                    while(ptr->next){
                        printf(" next entry !!!!!\n");
                        ptr = ptr->next;
                    }
                    ptr->next = File;
                }else{
                    printf(" first entry !!!!!\n");
                    FilestoDownload.FileList = File;
                }

                printf(" name ->\n");
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

void MyForm::gotBeep() {
    textBrowser->clear();
    if(TextBrowserStr[0])
        textBrowser->insertPlainText(TextBrowserStr);
}

U32 MyForm::countSelectedFiles(void)
{
    U32 ix,is=0;
    QTreeWidgetItem *item;
    //printf("\n load files as TS ");
    for(ix=0;ix<VFS_ROOT_ENTRIES;ix++){
        item = treeWidget->topLevelItem(ix);
        printf(" item 0x%x %p\n",(int)ix,item);
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
        QMessageBox::warning( this, tr(" ERROR "),tr(" nothing selected? "));
        return 0;
    } else
        return is;
}

QStringList MyForm::getFileNames()
{
    int ix;
    QStringList resultList;
    QTreeWidgetItem *item;
    
    for(ix=0; ix<VFS_ROOT_ENTRIES; ix++) {
        item = treeWidget->topLevelItem(ix);
        if(item) {
            printf(" item 0x%x %s\n",(int)ix,item->text(1).toStdString().c_str());
            resultList.append(item->text(1));
        } else {
            break;
        }
    }
    return resultList;
}

void MyForm::pc2box(){
    printf(" pc2box 0x%p\n",Filespc2box.FileList);

    if(Filespc2box.FileList){
        QMessageBox::warning( this, tr(" ERROR "),tr(" upload running "));
        return;
    }
    
    QStringList fileList = QFileDialog::getOpenFileNames(this,
                                                         tr("Open REC file"), 
                                                         ".", 
                                                         tr("Rec files (*.rec *.REC)"));
    if(fileList.isEmpty()) {
        QMessageBox::warning( this, tr(" ERROR "),tr(" no suitable *.rec file found "));
        return;
    } else {
        QStringList existingFiles = getFileNames();
        
        for(int i = 0; i < fileList.size(); ++i) {
            QFileInfo fileInfo(fileList.at(i));
            QByteArray asciiFileName = fileInfo.fileName().toAscii();
            const char *asciiFileNameRaw = asciiFileName.constData();

            // increment main structure list size
            Filespc2box.fileCount++;

            // allocate linked list entry
            Filesfordownload *File = (Filesfordownload*) malloc(sizeof(Filesfordownload));
            // convenient pointer to linked list
            Filesfordownload *ptr  =  Filespc2box.FileList;

            memset((char*)File,0x00,sizeof(Filesfordownload));
            // check if list is non-empty
            if(ptr){
                // seek to free entry
                while(ptr->next){
                    printf(" next entry !!!!!\n");
                    ptr = ptr->next;
                }
                // set entry to new one
                ptr->next = File;
            }else{
                // list is empty -> initialize first one
                printf(" first entry !!!!!\n");
                Filespc2box.FileList = File;
            }
            // copy (and cut) file name
            strncpy((char *)File->EntryName, asciiFileNameRaw, VFS_INODEN_NAME_LEN);
        }
    }

    if(!Filespc2box.fileCount){
        QMessageBox::warning( this, tr(" ERROR "),tr(" no suitable *.rec file found "));
        return;
    }else{
        emit Startpc2boxbeep();
    }
}

void MyForm::Rec2TS()
{
    FilesREC2TS.index = 0;
    FilesREC2TS.fileList.clear();

    FilesREC2TS.fileList = QFileDialog::getOpenFileNames(this,
                                                tr("Open REC file"), 
                                                ".", 
                                                tr("Rec files (*.rec *.REC)"));
    
    if(FilesREC2TS.fileList.isEmpty()) {
        QMessageBox::warning( this, tr(" ERROR "),tr(" No suitable *.rec file found "));
        return;
    } else {
        emit StartREC2TSbeep();
    }
}

void MyForm::showEPGInfo()
{
    QTreeWidgetItem *item = treeWidget->currentItem();
    HD_VFS_HANDLER *pLocalVFSHandler;
    char Epg[512] = {0};

    VFS_OpenFile(&pLocalVFSHandler, (INT8U *)(item->text(1).toStdString().c_str()) ,O_RDWR);
    HD_VFS_GetEventInfobyFileIDX(pLocalVFSHandler->EntryIDX, (INT8U *)Epg);
    VFS_CloseFile(pLocalVFSHandler, FILE_CLOSE);
    stripCtrlE(Epg);
    strcat(Epg, "\n");
    // TBD: sync access to TextBrowserStr for potential future Disk_Thread use
    if (strlen(TextBrowserStr) + strlen(Epg) > TEXTBROWSER_STR_LEN)
        strcpy(TextBrowserStr, Epg);
    else
        strcat(TextBrowserStr, Epg);
    gotBeep();
}
