include ( ../config.pri )

TEMPLATE    = app
LANGUAGE    = C++

CONFIG +=console

SOURCES     = main.cpp

TARGET      = q$${QT_VER}dsadm

QT +=sql xml

INCLUDEPATH     +=../lib 

LIBS    += -L../../lib -lqt$${QT_VER}-qdataschema
DESTDIR = ../../bin
