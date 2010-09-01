/*
 * main.cpp - pc2box main program launcher
 *
 *  This code implements the QT application launcher
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
#include <QApplication>
#include "myform.h"


int main(int argc, char* argv[])
{
  printf("Start application->>>>");
  QApplication app(argc, argv);

  //QApplication::addLibraryPath(QApplication::applicationDirPath() + "/plugins");
  //QString Path = QApplication::applicationDirPath() + "/dll";
  //QByteArray byteArray = Path.toAscii();
  //const char *str      = byteArray.constData();
  //printf("Start application->>>> %s ",str);

  MyForm* form = new MyForm;
  form->show();
  return app.exec();
}

