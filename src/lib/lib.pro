include ( ../config.pri )
TEMPLATE = lib
LANGUAGE = C++
#CONFIG += staticlib 
#exceptions stl
CONFIG += dll
VERSION=1.1.0
TARGET = qt$${QT_VER}-qdataschema
DESTDIR = ../../lib

QT +=sql xml
SOURCES += \
	qdataschema.cpp \
	qdataschemadriver.cpp \ 
	qdataschemaglobal.cpp
	
HEADERS += \
	qdataschema.h \
	qdataschemadriver.h \
	qdataschemaglobal.h

DEFINES +=QDATASCHEMA_EXPORT 

# DRIVERS
SOURCES += \
	qdataschemadriver-mysql.cpp \
	qdataschemadriver-postgresql.cpp \
	qdataschemadriver-sqlite.cpp 
HEADERS += \
	qdataschemadriver-mysql.h \
	qdataschemadriver-postgresql.h \
	qdataschemadriver-sqlite.h
