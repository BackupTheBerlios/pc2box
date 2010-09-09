/*
 * MyForm.cpp - pc2box user interface
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
#include "myform.h"

#if !defined(_WIN32) && !defined(_WIN64)
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#endif


MyForm::MyForm( QWidget* parent, const char* name){

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

    pushButton_2 = new QPushButton(this);
    pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

    gridLayout->addWidget(pushButton_2, 2, 0, 1, 1);

    pushButton_4 = new QPushButton(this);
    pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));

    gridLayout->addWidget(pushButton_4, 1, 0, 1, 1);

    pushButton_3 = new QPushButton(this);
    pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
    pushButton_3->setCursor(QCursor(static_cast<Qt::CursorShape>(13)));

    gridLayout->addWidget(pushButton_3, 0, 0, 1, 1);

    treeWidget = new QTreeWidget(this);
    treeWidget->setObjectName(QString::fromUtf8("treeWidget"));
    treeWidget->setFrameShape(QFrame::HLine);
    treeWidget->setUniformRowHeights(false);
    treeWidget->setSortingEnabled(true);
    treeWidget->setColumnCount(4);

    gridLayout->addWidget(treeWidget, 5, 0, 1, 3);
    spacerItem1 = new QSpacerItem(401, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout->addItem(spacerItem1, 6, 0, 1, 2);

    pushButton = new QPushButton(this);
    pushButton->setObjectName(QString::fromUtf8("pushButton"));
    pushButton->setCursor(QCursor(static_cast<Qt::CursorShape>(13)));

    gridLayout->addWidget(pushButton, 6, 2, 1, 1);

    pushButton_5 = new QPushButton(this);
    pushButton_5->setObjectName(QString::fromUtf8("pushButton_5"));

    gridLayout->addWidget(pushButton_5, 4, 0, 1, 1);

    QWidget::setTabOrder(treeWidget  , textBrowser);
    QWidget::setTabOrder(textBrowser , pushButton_3);
    QWidget::setTabOrder(pushButton_3, pushButton);
    QWidget::setTabOrder(pushButton  , pushButton_4);
    QWidget::setTabOrder(pushButton_4, pushButton_2);
    QWidget::setTabOrder(pushButton_2, pushButton_5);

    resize(this);

    QObject::connect(pushButton   , SIGNAL(clicked(bool)), this, SLOT(close()));
    QObject::connect(pushButton_5 , SIGNAL(clicked(bool)), this, SLOT(ShowApplicationInfo()));
    QObject::connect(pushButton_2 , SIGNAL(clicked(bool)), this, SLOT(pc2box()));
    QObject::connect(pushButton_3 , SIGNAL(clicked(bool)), this, SLOT(LoadRecFiles()));
    QObject::connect(pushButton_4 , SIGNAL(clicked(bool)), this, SLOT(LoadTSFiles()));

    retranslateUi(this);
    //! start threating
    THREAD_Params ThreadParams;
    ThreadParams.a              = "MyTead";
    ThreadParams.str            = this->TextBrowserStr;
    ThreadParams.pFileHandler   = &this->ActVfsHandler;
    ThreadParams.pLock          = &this->lock;
    ThreadParams.fileCounter    = &this->FileCounter;
    ThreadParams.pFileDownload  = &this->FilestoDownload;
    ThreadParams.pDownloadBar   = &this->DownloadBar;
    ThreadParams.pPc2Box        = &this->Filespc2box;

    pd = new QProgressDialog(" nothing ", "Cancel", 0 ,100, this );
    pd->setAutoReset(0);
    pd->setWindowModality(Qt::WindowModal);

    a = new Disk_Thread(&ThreadParams);
    QObject::connect( a, SIGNAL( beep() ), this, SLOT( gotBeep() ) );                       // update Textbrowser
    QObject::connect( a, SIGNAL( FileListbeep() ), this, SLOT( updateFileListWidget() ) );  // update FileListWidget
    QObject::connect( a, SIGNAL( DiskRemovebeep() ), this, SLOT( clearFileListWidget() ) ); // clear  FileListWidget
    QObject::connect( a, SIGNAL( UpdateBarbeep() ), this, SLOT( DisplayLoadBar() ) );       // clear  FileListWidget
    QObject::connect( this, SIGNAL( StartDownloadbeep() ), a, SLOT( StartDownload() ) );    // start  download
    QObject::connect( this, SIGNAL( Startpc2boxbeep() )  , a, SLOT( StartUpload() ) );

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
    printf("\n----------- clear FileListWidget --------------");
    lock.lock();
    treeWidget->clear();
    FileCounter = 0xffffffff;
    lock.unlock(); // free mutex
}

void MyForm::DisplayLoadBar(){
    lock.lock();
    printf("\nLoadBar %s %d",DownloadBar.InfoStr,(int)DownloadBar.bar);
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
            for(ix=0;ix<VFS_ROOT_ENTRIES;ix++){
                item = treeWidget->topLevelItem(ix);
                printf("\n item %x %p",(int)ix,item);
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

void MyForm::updateFileListWidget(){
    char *Str = (char *)malloc(20);
    QTreeWidgetItem *item   = new QTreeWidgetItem(treeWidget);
    QCheckBox *itemCheckBox = new QCheckBox(treeWidget);

    Str[0] = 0;
    
    lock.lock();

    printf("\n item %lu %p",FileCounter,item);
    snprintf(Str,20,"%04lu",FileCounter);

    item->setText( 0,(const char*)Str);
    item->setForeground(0, QBrush(Qt::transparent, Qt::NoBrush));
    itemCheckBox->setText(Str);
    treeWidget->setItemWidget(item, 0, itemCheckBox);

    item->setText( 1,tr((const char*)ActVfsHandler.Inode.EntryName));

    Str[0]   = 0x00;
    U32 Flen = ActVfsHandler.Inode.sizecluster * VFS_GetClusterSize();
    Flen   = Flen>>1;
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
    printf("\n load files as REC ");
    printf("\n %d files selected ",(int)is);
    if(!is)return;

    //lock.lock();

    FilestoDownload.fileCount = is;
    FilestoDownload.type      = 1;
    if(!FilestoDownload.FileList){
        AddNewFileForDownload();
    }else{printf("\n download in progress ");}
    //lock.unlock();
}

void MyForm::LoadTSFiles(){
    U32 is = countSelectedFiles();
    printf("\n load files as TS ");
    printf("\n %d files selected ",(int)is);
    if(!is)return;

    //lock.lock();

    FilestoDownload.fileCount = is;
    FilestoDownload.type      = 2;
    if(!FilestoDownload.FileList){
        AddNewFileForDownload();
    }else{printf("\n download in progress ");}

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

void MyForm::gotBeep() {
    if(!TextBrowserStr[0])
        textBrowser->clear();
    else textBrowser->insertPlainText(TextBrowserStr);
}

U32 MyForm::countSelectedFiles(void)
{
    U32 ix,is=0;
    QTreeWidgetItem *item;
    //printf("\n load files as TS ");
    for(ix=0;ix<VFS_ROOT_ENTRIES;ix++){
        item = treeWidget->topLevelItem(ix);
        printf("\n item %x %p",(int)ix,item);
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

void MyForm::pc2box(){
    printf("\n pc2box 0x%p",Filespc2box.FileList);
    //QMessageBox::warning( this, tr(" ERROR "),tr(" muss noch eingebaut werden "));

    if(Filespc2box.FileList){
        QMessageBox::warning( this, tr(" ERROR "),tr(" upload running "));
        return;
    }

    QDir dir;
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setSorting(QDir::Size | QDir::Reversed);

    QFileInfoList list = dir.entryInfoList();

    Filespc2box.fileCount = 0;
    Filespc2box.type      = 3;
    Filespc2box.FileList  = 0;

    for(int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo   = list.at(i);
        QString name         = fileInfo.fileName();
        QByteArray byteArray = name.toAscii();
        const char *str      = byteArray.constData();
        U32 ix               = strlen(str);
        while(ix--){
            if(!strncmp(str,".rec",4)){
                str      = byteArray.constData();
                printf("\n %s ",str);
                Filespc2box.fileCount++;

                Filesfordownload *File = (Filesfordownload*) malloc(sizeof(Filesfordownload));
                Filesfordownload *ptr  =  Filespc2box.FileList;

                memset((char*)File,0x00,sizeof(Filesfordownload));
                if(ptr){
                    while(ptr->next){
                        printf("\n next entry !!!!!");
                        ptr = ptr->next;
                    }
                    ptr->next = File;
                }else{
                    printf("\n first entry !!!!!");
                    Filespc2box.FileList = File;
                }
                ix = strlen(str);
                if(ix<(VFS_INODEN_NAME_LEN+4)){
                    char *Str = (char*)&File->EntryName[0];
                    while(ix--)*Str++ = *str++;
                }else{
                    printf("\n VFS_INODEN_NAME_LEN+5 ");
                }
                break;
            }
            str++;
        }
    }

    if(!Filespc2box.fileCount){
        QMessageBox::warning( this, tr(" ERROR "),tr(" kein *.rec gefunden "));
        return;
    }else{
        emit Startpc2boxbeep();
    }
}

Disk_Thread::Disk_Thread(THREAD_Params *pParams)
{
    this->name           = pParams->a;
    this->Str            = pParams->str;
    this->pActVfsHandler = pParams->pFileHandler;
    this->lock           = pParams->pLock;
    this->pFileCounter   = pParams->fileCounter;
    this->pFileDownload  = pParams->pFileDownload;
    this->pDownloadBar   = pParams->pDownloadBar;
    this->pPc2Box        = pParams->pPc2Box;
}

int Disk_Thread::Poll_vfs(int dev, bool *ping)
{
    PVFS_FILESYS        VfsSys       = &FileSys;

    // Try to get media info for device; most probably not possible
    // under Windows in case the device is locked / opened!
    // Potential bug here: returns 0 in case of success, so should be
    // negative logic!!??
    if(VfsGetMediaInfo(VfsSys)){
        lock->lock();

        *this->pFileCounter = 0;
        // DiskRemovebeep is connected to clearFileListWidget which sets the counter to 0xffffffff
        emit DiskRemovebeep();
        while(*this->pFileCounter != 0xffffffff) { // sync thread
            lock->unlock();
            msleep(1);
            lock->lock();
        }

        lock->unlock();

        VfsCloseDevice(VfsSys);
        Unmount("0xdeadbeef");
        *ping = true;
    }
    return dev;
}

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
                printf("\n VFS loop %x",(int)*this->pFileCounter);
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
        sprintf(this->Str,"%s",str);
    }else this->Str[0] = 0x00;
    emit beep();
    msleep(10); // update unsynced, wait a while to be sure

}

void Disk_Thread::AddNewVFSHandler(HD_VFS_HANDLER *pVfsHandler){
    if(pActVfsHandler){
        *pActVfsHandler = *pVfsHandler;
        emit FileListbeep();
    }else{printf("\n VFSHandler not init");}
}

void Disk_Thread::updateDownloadbar(DownloadBarInfo *pInfo){
    printf("\n bar -> %d ",(int)pInfo->bar);
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
    char                DevName[100] = {0};
    PVFS_FILESYS        VfsSys       = &FileSys;
    int devNr = dev;
    
    sprintf(DevName," Test dev  %d \n",devNr);
    printTextBrowser(DevName);
    
#if defined(_WIN32) || defined(_WIN64)
    sprintf(DevName,"\\Device\\Harddisk%d\\Partition0",dev);
#else
    sprintf(DevName,"/dev/sd%c",devNr+'b');
#endif
    if(VfsDevtoLetter(DevName,"0xdeadbeef")){
        printTextBrowser("Dev not ready!\n");
        dev = 0;
    }else{
        qDebug("\n DEV OK ");
#if defined(_WIN32) || defined(_WIN64)
        sprintf(DevName, "%s", "0xdeadbeef");
#endif
        if(!NT_SUCCESS(VfsOpenDevice(VfsSys, (PUCHAR)DevName))){
            printTextBrowser("vfs:  Volume does not exist!\n");
            printTextBrowser(0);
#if defined(_WIN32) || defined(_WIN64)
            devNr = 0;
#else
            devNr++;
#endif
        }else{
            qDebug(" DEV OPEN OK ");
            VfsGetMediaInfo(VfsSys);
            VfsInit.VfsSys = VfsSys;
            VfsInit.Init   = false;
            VFS_Open(&VfsInit);
            if(VFS_Mount(0) == FAT_OK){
                U32 idx;
                HD_VFS_HANDLER *File = (HD_VFS_HANDLER*)malloc(sizeof(HD_VFS_HANDLER));
            
                printTextBrowser(0);
                printTextBrowser(" mount OK!!! \n");
                    
                VfsInit.Init   = true;
                *ping          = true;
                    
                *this->pFileCounter =0;
                for(idx = 0;idx < VFS_ROOT_ENTRIES;idx++){
                    if(VFS_GetFileInfobyIndex(idx,File) == FAT_OK){
                        if(File->Inode.status == INODE_BUSY){

                            lock->lock();
                            *this->pFileCounter = idx+1;
                            printf("\n VFS loop %x",(int)*this->pFileCounter);
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
            devNr += 1;
        }
    }
    Unmount("0xdeadbeef");
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
    printf("\n transfer cancel !!!!!!!!!!!!!!!!!!!!!");

    lock->lock();

    if(VFSHandler){
        VFS_CloseFile(VFSHandler,FILE_CLOSE);
        VFSHandler = 0;
    }

    if(PCFile){
        PCFile->close();
        delete PCFile;
        PCFile =0;
    }

    if(ActFile)       ActFile = &DummyActFile;
    if(ActPc2BoxFile) ActPc2BoxFile = &DummyActFile;

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
        printf("\n create_file -> %s",FileName);
        err = VFS_OpenFile(&VFSHandler,FileName,O_RDWR|O_CREAT);
        if(err == FAT_OUT_OF_SPACE){
            break;
        }else if(err != FAT_OK){
            //! find a new name
            printf("\n try to find a new name %s",FileName);
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
            printf("\n VFS_open file -> OK!!!!");
            VFSHandler->Inode.FileEventMjd = 0;
            //! set start mark
            sprintf((char*)Info.markname,"Start");
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

void Disk_Thread::OpenFilesForUpload(void){
    printf("\n try  to open %s",(char*)ActPc2BoxFile->EntryName);
    PCFile = new QFile((char*)ActPc2BoxFile->EntryName);
    if(!PCFile->open(QIODevice::ReadOnly)){
        delete PCFile;
        ActPc2BoxFile = 0;
        printf("\n can't open file ");
    }else{
        HD_VFS_PC_HEADER* pPC_Header = (HD_VFS_PC_HEADER*) malloc(sizeof(HD_VFS_PC_HEADER));
        PCFile->read((char*)pPC_Header,sizeof(HD_VFS_PC_HEADER));
        printf("\n SI-EPGData-> %s",(char*)pPC_Header->Epg);
        if(strcmp((char*)pPC_Header->ver,VFS_PC_ACTVERSION)){
            PCFile->close();
            delete PCFile;
            ActPc2BoxFile = 0;
        }else{
            if(CreateVFSEntry((char*)ActPc2BoxFile->EntryName)){
                PCFile->close();
                delete PCFile;
                ActPc2BoxFile = 0;
                printf("\n can't open VFS_file ");
            }else{
                memset(&MarkInfo,0x00,sizeof(MarkInfo));
                HD_VFS_PutEventInfobyFileIDX(VFSHandler->EntryIDX,pPC_Header->Epg);
            }
        }
        free(pPC_Header);
    }
}

void Disk_Thread::StartUpload(void)
{
    lock->lock();                                             // called from MyFormThread

    printf("\n ---------- start upload -----------------");
    ActPc2BoxFile = pPc2Box->FileList;
    if(ActPc2BoxFile){
        OpenFilesForUpload();
    }else{printf("\n nothing available");}
    printf("\n ---------- END   upload -----------------");

    lock->unlock();
}

void Disk_Thread::StartDownload(void)
{
    char Str[VFS_INODEN_NAME_LEN+5]={0};

    lock->lock();

    printf("\n ---------- start download ---------------");   // called from MyFormThread
    ActFile = pFileDownload->FileList;
    if(ActFile){
        VFS_OpenFile(&VFSHandler, ActFile->EntryName ,O_RDWR);
        if(pFileDownload->type == 2){
            sprintf(Str,"%s.mpg",ActFile->EntryName);
        }else if(pFileDownload->type == 1){
            sprintf(Str,"%s.rec",ActFile->EntryName);
        }
        stripCtrlE((char *)Str);
        printf("\n try to open %s",Str);
        prepareFileName(Str);
        PCFile = new QFile(Str);
        if(!PCFile->open(QIODevice::ReadWrite)){
            delete PCFile;
            ActFile = 0;
            printf("\n can't open file ");
        }else{
            if(pFileDownload->type == 1){
                char   Epg[512] = {0};
                HD_VFS_PC_HEADER* pPC_Header = (HD_VFS_PC_HEADER*) malloc(sizeof(HD_VFS_PC_HEADER));
                memset(pPC_Header->ver,0x00,sizeof(pPC_Header->ver));
                memset(pPC_Header->Epg,0x00,sizeof(pPC_Header->Epg));
                memcpy(pPC_Header->Epg,Epg,512);
                memset(pPC_Header->MarkInfo,0x00,sizeof(pPC_Header->MarkInfo));
                sprintf((char*)pPC_Header->ver,"%s",VFS_PC_ACTVERSION);
                printf(" -> %s \n",pPC_Header->ver);
                PCFile->write((const char*)pPC_Header,sizeof(HD_VFS_PC_HEADER));
                free(pPC_Header);
            }
        }
    }else{printf("\n nothing available");}
    printf("\n ---------- END   download ---------------");

    lock->unlock();
}

U32 Disk_Thread::FileUploadProcessing(void)
{
    lock->lock();
    if(ActPc2BoxFile){
        printf("*");
        if(PCFile){
            FAT_ERROR           fat_err;
            HD_VFS_RECORD_INFO *pInfo;

            if(PCFile->read((char*)RecordBuffer,VFS_REC_SIZE) <= 0){
                printf("\n VFS_EOF!!!-> set end");
                sprintf((char*)MarkInfo.markname,"End");

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

                qDebug("Rec -> %d t-> %d ",pInfo->ActRecordNbr,pInfo->RecHdTimeStamp);
                if(pInfo->ActRecordNbr == 1)pInfo->Linkused = 1;
                else                        pInfo->Linkused = 0;
                pInfo->MarkLink          = 0;

                if(!VFS_PutNByte(&VFSHandler,RecordBuffer,VFS_REC_SIZE,&fat_err)){
                    printf("\n PutByte ERROR");
                }
                if(fat_err){
                    printf("\n disc full !!!!");
                    sprintf((char*)MarkInfo.markname,"End");
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
                sprintf((char*)Bar.InfoStr,"%s",(char*)VFSHandler->Inode.EntryName);
                if(pInfo->ActRecordNbr == 1){
                    updateDownloadbar(&Bar);
                }

                Bar.bar  = pInfo->ActRecordNbr*100;
                Bar.bar /= PCFile->size()/VFS_REC_SIZE;
                updateDownloadbar(&Bar);

                lock->unlock();
                return 10;
            }

        }else{
            ActPc2BoxFile = ActPc2BoxFile->next;
            if(ActPc2BoxFile){
                // open next file
                OpenFilesForUpload();
            }else{
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
                printf("ENDE");
            }
        }
    }
    lock->unlock();
    return 100;
}

U32 Disk_Thread::FileDownloadProcessing(void)
{
    lock->lock();
    if(ActFile){
        printf("\n*");
        if(VFSHandler){
            U32 size;
            FAT_ERROR  err;
            size  = VFS_GetNByte(&VFSHandler,RecordBuffer,VFS_REC_SIZE,&err);
            if(size != VFS_REC_SIZE){

                printf("\nEOF");
                VFS_CloseFile(VFSHandler,FILE_CLOSE);
                VFSHandler = 0;
                PCFile->close();
                delete PCFile;

            }else{
                HD_VFS_RECORD_INFO *pInfo = (HD_VFS_RECORD_INFO *)&RecordBuffer[VFS_TSPACK_SIZE];
                qDebug("Rec -> %d t-> %d ",pInfo->ActRecordNbr,pInfo->RecHdTimeStamp);
                if(pFileDownload->type == 2){
                    // mpeg PART
                    PCFile->write((const char*)RecordBuffer,VFS_TSPACK_SIZE);
                }else if(pFileDownload->type == 1){
                    // rec PART
                    PCFile->write((const char*)RecordBuffer,VFS_REC_SIZE);
                }

                DownloadBarInfo Bar;
                memset(&Bar,0x00,sizeof(DownloadBarInfo));
                Bar.bar  = pInfo->RecHdTimeStamp*100;
                Bar.bar /= VFSHandler->Inode.LastRecTime;
                sprintf((char*)Bar.InfoStr,"%s",(char*)VFSHandler->Inode.EntryName);
                updateDownloadbar(&Bar);

                lock->unlock();
                return 10;

            }
        }else{
            ActFile = ActFile->next;
            if(ActFile){ // open the next file
                VFS_OpenFile(&VFSHandler, ActFile->EntryName ,O_RDWR);
                if(pFileDownload->type == 2){
                    sprintf(Str,"%s.mpg",ActFile->EntryName);
                }else if(pFileDownload->type == 1){
                    sprintf(Str,"%s.rec",ActFile->EntryName);
                }
                stripCtrlE((char *)Str);
                printf("\n try to open %s",Str);
                prepareFileName(Str);
                PCFile = new QFile(Str);
                if(!PCFile->open(QIODevice::ReadWrite)){
                    delete PCFile;
                    ActFile = 0;
                    printf("\n can't open file ");
                }else{
                    if(pFileDownload->type == 1){
                        char   Epg[512] = {0};
                        HD_VFS_PC_HEADER* pPC_Header = (HD_VFS_PC_HEADER*) malloc(sizeof(HD_VFS_PC_HEADER));
                        memset(pPC_Header->ver,0x00,sizeof(pPC_Header->ver));
                        memset(pPC_Header->Epg,0x00,sizeof(pPC_Header->Epg));
                        memcpy(pPC_Header->Epg,Epg,512);
                        memset(pPC_Header->MarkInfo,0x00,sizeof(pPC_Header->MarkInfo));
                        sprintf((char*)pPC_Header->ver,"%s",VFS_PC_ACTVERSION);
                        printf(" -> %s \n",pPC_Header->ver);
                        PCFile->write((const char*)pPC_Header,sizeof(HD_VFS_PC_HEADER));
                        free(pPC_Header);
                    }
                }
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
                printf("ENDE");
            }
        }
    }
    lock->unlock();
    return 100;
}

void Disk_Thread::run(void)
{
    fpPolling   fpDiskPoll = &Disk_Thread::init_vfs;
    int count      = 0;
#if !defined(_WIN32) && !defined(_WIN64)
    int countPrev  = count;
#endif
    bool pingpong= false;

    printf("start thread");
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
    }
#endif

    for(;;){

        //---disk_mount_unmount-----
#if defined(_WIN32) || defined (_WIN64)
        // Windows only: constant re-polling
        count = (this->*fpDiskPoll)(count,&pingpong);
        if(pingpong){
            if(fpDiskPoll == &Disk_Thread::init_vfs){
                fpDiskPoll = &Disk_Thread::Poll_vfs;
            }else{
                fpDiskPoll = &Disk_Thread::init_vfs;
            }
            pingpong = 0;
        }
#endif
        //--------------------------
        //---file processing--------
        U32 timeout;
        //--------------------------
        if(ActFile){
            timeout = FileDownloadProcessing();
        }else if(ActPc2BoxFile){
            timeout = FileUploadProcessing();
        }else timeout = 100;
        msleep(timeout);

    }
}
