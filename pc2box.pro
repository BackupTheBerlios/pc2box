# pc2box.pro - Qt qmake makefile generator input file
#
#  This code provides the input options required for qmake makefile generation
#
#  See the AUTHORS file for the list of contributors
#
# This file is part of pc2box, a free C++ PVR file retrieval utility
#
#
# This program is free software; you can redistribute it and/or modify 
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += .
win32:LIBS  += -lntdll
RC_FILE      = win.rc

# The next two lines turn off optimization and enable debug mode (-g -O0)
#CONFIG -= release
CONFIG += debug
# QMAKE_CXXFLAGS += and QMAKE_CFLAGS += can set individual flags too

# Input
HEADERS += myform.h types.h vfs.h diskthread.h
SOURCES += disk.c \
           main.cpp \
           myform.cpp \
           diskthread.cpp \
           vfs.c
