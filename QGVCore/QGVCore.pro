QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

TARGET = QGVCore
TEMPLATE = lib
CONFIG += shared

DESTDIR = ../lib
DLLDESTDIR = ../bin

!include(GraphViz.pri) {
     error("fail open GraphViz.pri")
 }

SOURCES += QGVScene.cpp \
    QGVNode.cpp \
    QGVEdge.cpp \
    QGVSubGraph.cpp \
		private/QGVCore.cpp \
		private/QGVGraphPrivate.cpp \
		private/QGVGvcPrivate.cpp \
		private/QGVEdgePrivate.cpp \
		private/QGVNodePrivate.cpp

HEADERS  += QGVScene.h \
    QGVNode.h \
    QGVEdge.h \
    QGVSubGraph.h \
		private/QGVCore.h \
		private/QGVGraphPrivate.h \
		private/QGVGvcPrivate.h \
		private/QGVEdgePrivate.h \
		private/QGVNodePrivate.h \
    qgv.h
