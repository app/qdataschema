unix {
    UI_DIR = .ui
    MOC_DIR = .moc
    OBJECTS_DIR = .obj
    QT3DIR=$$system( echo $QTDIR | grep qt3 )
    QT4DIR=$$system( echo $QTDIR | grep qt4 )
}

win32 {
    UI_DIR = tmp\ui
    MOC_DIR = tmp\moc
    OBJECTS_DIR = obj
#    QT3DIR=$$system( echo $QTDIR | grep qt3 )
#    QT4DIR=$$system( echo $QTDIR | grep qt4 )
}

QMAKE_CLEAN += Makefile
CONFIG  += qt warn_on thread 
CONFIG  += debug
LANGUAGE = C++
!isEmpty(QT3DIR):QT_VER=3
!isEmpty(QT4DIR):QT_VER=4
message( QDataSchema BUILD FOR QT$$QT_VER )
