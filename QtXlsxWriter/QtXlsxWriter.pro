#-------------------------------------------------
#
# Project created by QtCreator 2017-03-08T00:42:57
#
#-------------------------------------------------

QT       += core gui gui-private

TARGET = QtXlsxWriter
TEMPLATE = lib

DEFINES += QT_BUILD_XLSX_LIB

QMAKE_DOCS = $$PWD/doc/qtxlsx.qdocconf

#Define this macro if you want to run tests, so more AIPs will get exported.
#DEFINES += XLSX_TEST

QMAKE_TARGET_COMPANY = "Debao Zhang"
QMAKE_TARGET_COPYRIGHT = "Copyright (C) 2013-2014 Debao Zhang <hello@debao.me>"
QMAKE_TARGET_DESCRIPTION = ".Xlsx file wirter for Qt5"
# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

win32:contains(QMAKE_HOST.arch, x86_64) {
    CONFIG(debug, debug|release) {
        DLLDESTDIR += "../build-ManagerSystem-Desktop_Qt_5_8_0_MSVC2015_64bit-Debug/debug"
    }

    CONFIG(release, debug|release) {
        DLLDESTDIR += "../build-ManagerSystem-Desktop_Qt_5_8_0_MSVC2015_64bit-Release/release"
    }
}else{
    CONFIG(debug, debug|release) {
        DLLDESTDIR += "../build-ManagerSystem-Desktop_Qt_5_8_0_MSVC2015_32bit-Debug/debug"
    }

    CONFIG(release, debug|release) {
        DLLDESTDIR += "../build-ManagerSystem-Desktop_Qt_5_8_0_MSVC2015_32bit-Release/release"
    }
}

SOURCES += qtxlsxwriter.cpp \
    xlsxabstractooxmlfile.cpp \
    xlsxabstractsheet.cpp \
    xlsxcell.cpp \
    xlsxcellformula.cpp \
    xlsxcellrange.cpp \
    xlsxcellreference.cpp \
    xlsxchart.cpp \
    xlsxchartsheet.cpp \
    xlsxcolor.cpp \
    xlsxconditionalformatting.cpp \
    xlsxcontenttypes.cpp \
    xlsxdatavalidation.cpp \
    xlsxdocpropsapp.cpp \
    xlsxdocpropscore.cpp \
    xlsxdocument.cpp \
    xlsxdrawing.cpp \
    xlsxdrawinganchor.cpp \
    xlsxformat.cpp \
    xlsxmediafile.cpp \
    xlsxnumformatparser.cpp \
    xlsxrelationships.cpp \
    xlsxrichstring.cpp \
    xlsxsharedstrings.cpp \
    xlsxsimpleooxmlfile.cpp \
    xlsxstyles.cpp \
    xlsxtheme.cpp \
    xlsxutility.cpp \
    xlsxworkbook.cpp \
    xlsxworksheet.cpp \
    xlsxzipreader.cpp \
    xlsxzipwriter.cpp

HEADERS += qtxlsxwriter.h\
        qtxlsxwriter_global.h \
    xlsxabstractooxmlfile.h \
    xlsxabstractooxmlfile_p.h \
    xlsxabstractsheet.h \
    xlsxabstractsheet_p.h \
    xlsxcell.h \
    xlsxcell_p.h \
    xlsxcellformula.h \
    xlsxcellformula_p.h \
    xlsxcellrange.h \
    xlsxcellreference.h \
    xlsxchart.h \
    xlsxchart_p.h \
    xlsxchartsheet.h \
    xlsxchartsheet_p.h \
    xlsxcolor_p.h \
    xlsxconditionalformatting.h \
    xlsxconditionalformatting_p.h \
    xlsxcontenttypes_p.h \
    xlsxdatavalidation.h \
    xlsxdatavalidation_p.h \
    xlsxdocpropsapp_p.h \
    xlsxdocpropscore_p.h \
    xlsxdocument.h \
    xlsxdocument_p.h \
    xlsxdrawing_p.h \
    xlsxdrawinganchor_p.h \
    xlsxformat.h \
    xlsxformat_p.h \
    xlsxglobal.h \
    xlsxmediafile_p.h \
    xlsxnumformatparser_p.h \
    xlsxrelationships_p.h \
    xlsxrichstring.h \
    xlsxrichstring_p.h \
    xlsxsharedstrings_p.h \
    xlsxsimpleooxmlfile_p.h \
    xlsxstyles_p.h \
    xlsxtheme_p.h \
    xlsxutility_p.h \
    xlsxworkbook.h \
    xlsxworkbook_p.h \
    xlsxworksheet.h \
    xlsxworksheet_p.h \
    xlsxzipreader_p.h \
    xlsxzipwriter_p.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
