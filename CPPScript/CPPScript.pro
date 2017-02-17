#-------------------------------------------------
#
# Project created by QtCreator 2012-12-30T05:42:39
#
#-------------------------------------------------

QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CPPScript
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    cppscript.cpp \
    selecttablestogenscript.cpp

HEADERS  += mainwindow.h \
    cppscript.h \
    selecttablestogenscript.h

FORMS    += mainwindow.ui \
    selecttablestogenscript.ui
