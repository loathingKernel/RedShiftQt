#-------------------------------------------------
#
# Project created by QtCreator 2015-12-19T21:15:05
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RedShiftQt
TEMPLATE = app


SOURCES += \
    main.cpp \
    redshiftqttray.cpp \
    redshiftqtprefs.cpp \
    redshiftqtlog.cpp \
    redshiftqt.cpp \
    qdoublespinboxlist.cpp

HEADERS += \
    redshiftqttray.h \
    redshiftqtprefs.h \
    redshiftqtlog.h \
    redshiftqt.h \
    qdoublespinboxlist.h

FORMS   += \
    redshiftqtprefs.ui \
    redshiftqtlog.ui

RESOURCES += \
    redshiftqt.qrc

RC_FILE += \
    redshiftqt.rc

DISTFILES +=
