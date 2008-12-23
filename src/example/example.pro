include (../config.pri)
TEMPLATE    = app
LANGUAGE    = C++

CONFIG +=console

SOURCES     = main.cpp

TARGET      = q$${QT_VER}ds-example

INCLUDEPATH     +=$(QTDIR)/include/qdataschema 
INCLUDEPATH     +=../lib 

LIBS    += -L../../lib -lqt$${QT_VER}-qdataschema
