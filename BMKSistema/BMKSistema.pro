#-------------------------------------------------
#
# Project created by QtCreator 2017-07-12T21:19:42
#
#-------------------------------------------------
CONFIG += c++14
QT       += core gui opengl sql webengine webenginewidgets charts


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BMKSistema
TEMPLATE = app

DEFINES += MYLIBRARY_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    mylibrary/comboboxdelegate.cpp \
    mylibrary/database_connection.cpp \
    mylibrary/doublespinboxdelegate.cpp \
    mylibrary/funcionalidad.cpp \
    mylibrary/lineedit.cpp \
    mylibrary/lineeditdelegate.cpp \
    mylibrary/lineeditnumberdelegate.cpp \
    mylibrary/mylibrary.cpp \
    mylibrary/reniecconsultadni.cpp \
    mylibrary/session.cpp \
    mylibrary/sistema.cpp \
    mylibrary/snackbarinfo.cpp \
    mylibrary/sqlstatements.cpp \
    mylibrary/sunatconsultaruc.cpp \
    mylibrary/sunattipocambio.cpp \
    mylibrary/widgetnumber.cpp \    
    mylibrary/widgetevents.cpp \
    mylibrary/_startprogram.cpp \
    mylibrary/configuracion.cpp \
    mylibrary/login.cpp \
    mylibrary/managersystem.cpp \
    mylibrary/opcionesdb.cpp \
    mylibrary/toolbar.cpp \
    mylibrary/vercambio.cpp \
    mylibrary/verigv.cpp \
    main.cpp \
    comprobante/comprobante.cpp \
    persona/persona.cpp \
    producto/producto.cpp \
    comprobante/familia.cpp \
    comprobante/plato.cpp \
    comprobante/cliente.cpp \
    comprobante/comprobantebuscar.cpp \
    mylibrary/adminpass.cpp \
    mylibrary/starmicronics.cpp

HEADERS += \
    mylibrary/comboboxdelegate.h \
    mylibrary/database_connection.h \
    mylibrary/doublespinboxdelegate.h \
    mylibrary/funcionalidad.h \
    mylibrary/lineedit.h \
    mylibrary/lineeditdelegate.h \
    mylibrary/lineeditnumberdelegate.h \
    mylibrary/mylibrary.h \
    mylibrary/mylibrary_global.h \
    mylibrary/reniecconsultadni.h \
    mylibrary/session.h \
    mylibrary/share_include.h \
    mylibrary/share_static_include.h \
    mylibrary/share_typedef.h \
    mylibrary/singleton.h \
    mylibrary/sistema.h \
    mylibrary/snackbarinfo.h \
    mylibrary/sqlstatements.h \
    mylibrary/sunatconsultaruc.h \
    mylibrary/sunattipocambio.h \
    mylibrary/tablemoves.h \
    mylibrary/widgetnumber.h \
    mylibrary/widgetevents.h \
    mylibrary/_startprogram.h \
    mylibrary/configuracion.h \
    mylibrary/login.h \
    mylibrary/managersystem.h \
    mylibrary/modules_library.h \
    mylibrary/opcionesdb.h \
    mylibrary/shares.h \
    mylibrary/toolbar.h \
    mylibrary/vercambio.h \
    mylibrary/verigv.h \
    persona/persona_headers.h \
    producto/producto_headers.h \
    comprobante/comprobante_headers.h \
    comprobante/comprobante.h \
    persona/persona.h \
    producto/producto.h \
    comprobante/familia.h \
    comprobante/plato.h \
    comprobante/cliente.h \
    comprobante/comprobantebuscar.h \
    mylibrary/adminpass.h \
    mylibrary/starmicronics.h

FORMS += \
    mylibrary/snackbarinfo.ui \
    mylibrary/configuracion.ui \
    mylibrary/login.ui \
    mylibrary/managersystem.ui \
    mylibrary/opcionesdb.ui \
    mylibrary/toolbar.ui \
    mylibrary/vercambio.ui \
    mylibrary/verigv.ui \
    comprobante/comprobante.ui \
    persona/persona.ui \
    producto/producto.ui \
    comprobante/familia.ui \
    comprobante/plato.ui \
    comprobante/cliente.ui \
    comprobante/comprobantebuscar.ui \
    mylibrary/adminpass.ui

RESOURCES += \
    iconos.qrc

win32:contains(QMAKE_HOST.arch, x86_64) {
    win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../VS2015_Tesseract-master/x64/release/ -lzlib
    else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../VS2015_Tesseract-master/x64/debug/ -lzlib
    else:unix: LIBS += -L$$PWD/../../VS2015_Tesseract-master/x64/ -lzlib

    win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../VS2015_Tesseract-master/x64/release/ -lgiflib
    else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../VS2015_Tesseract-master/x64/debug/ -lgiflib
    else:unix: LIBS += -L$$PWD/../../VS2015_Tesseract-master/x64/ -lgiflib

    win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../VS2015_Tesseract-master/x64/release/ -llibjpeg
    else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../VS2015_Tesseract-master/x64/debug/ -llibjpeg
    else:unix: LIBS += -L$$PWD/../../VS2015_Tesseract-master/x64/ -llibjpeg

    win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../VS2015_Tesseract-master/x64/release/ -llibpng
    else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../VS2015_Tesseract-master/x64/debug/ -llibpng
    else:unix: LIBS += -L$$PWD/../../VS2015_Tesseract-master/x64/ -llibpng

    win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../VS2015_Tesseract-master/x64/release/ -llibtiff
    else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../VS2015_Tesseract-master/x64/debug/ -llibtiff
    else:unix: LIBS += -L$$PWD/../../VS2015_Tesseract-master/x64/ -llibtiff

    win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../VS2015_Tesseract-master/x64/release/ -llibwebp
    else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../VS2015_Tesseract-master/x64/debug/ -llibwebp
    else:unix: LIBS += -L$$PWD/../../VS2015_Tesseract-master/x64/ -llibwebp

    win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../VS2015_Tesseract-master/x64/release/ -lopenjpeg
    else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../VS2015_Tesseract-master/x64/debug/ -lopenjpeg
    else:unix: LIBS += -L$$PWD/../../VS2015_Tesseract-master/x64/ -lopenjpeg

    win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../VS2015_Tesseract-master/x64/release/ -lliblept
    else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../VS2015_Tesseract-master/x64/debug/ -lliblept
    else:unix: LIBS += -L$$PWD/../../VS2015_Tesseract-master/x64/ -lliblept

    win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../VS2015_Tesseract-master/x64/release/ -llibtesseract
    else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../VS2015_Tesseract-master/x64/debug/ -llibtesseract
    else:unix: LIBS += -L$$PWD/../../VS2015_Tesseract-master/x64/ -llibtesseract

    win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-QtXlsxWriter-Desktop_Qt_5_9_1_MSVC2015_64bit-Release/release/ -lQtXlsxWriter
    else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-QtXlsxWriter-Desktop_Qt_5_9_1_MSVC2015_64bit-Debug/debug/ -lQtXlsxWriter
    else:unix: LIBS += -L$$PWD/../build-QtXlsxWriter-Desktop_Qt_5_9_1_MSVC2015_64bit-Debug/ -lQtXlsxWriter

    win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-BMKSistema-Desktop_Qt_5_9_1_MSVC2015_64bit-Release/release/ -lStarIOPort
    else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-BMKSistema-Desktop_Qt_5_9_1_MSVC2015_64bit-Release/debug/ -lStarIOPort
    else:unix: LIBS += -L$$PWD/../build-BMKSistema-Desktop_Qt_5_9_1_MSVC2015_64bit-Release/ -lStarIOPort
} else {
    win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../VS2015_Tesseract-master/release/ -lzlib
    else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../VS2015_Tesseract-master/debug/ -lzlib
    else:unix: LIBS += -L$$PWD/../../VS2015_Tesseract-master/ -lzlib

    win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../VS2015_Tesseract-master/release/ -lgiflib
    else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../VS2015_Tesseract-master/debug/ -lgiflib
    else:unix: LIBS += -L$$PWD/../../VS2015_Tesseract-master/x64/ -lgiflib

    win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../VS2015_Tesseract-master/release/ -llibjpeg
    else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../VS2015_Tesseract-master/debug/ -llibjpeg
    else:unix: LIBS += -L$$PWD/../../VS2015_Tesseract-master/ -llibjpeg

    win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../VS2015_Tesseract-master/release/ -llibpng
    else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../VS2015_Tesseract-master/debug/ -llibpng
    else:unix: LIBS += -L$$PWD/../../VS2015_Tesseract-master/ -llibpng

    win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../VS2015_Tesseract-master/release/ -llibtiff
    else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../VS2015_Tesseract-master/debug/ -llibtiff
    else:unix: LIBS += -L$$PWD/../../VS2015_Tesseract-master/ -llibtiff

    win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../VS2015_Tesseract-master/release/ -llibwebp
    else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../VS2015_Tesseract-master/debug/ -llibwebp
    else:unix: LIBS += -L$$PWD/../../VS2015_Tesseract-master/ -llibwebp

    win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../VS2015_Tesseract-master/release/ -lopenjpeg
    else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../VS2015_Tesseract-master/debug/ -lopenjpeg
    else:unix: LIBS += -L$$PWD/../../VS2015_Tesseract-master/ -lopenjpeg

    win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../VS2015_Tesseract-master/release/ -lliblept
    else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../VS2015_Tesseract-master/debug/ -lliblept
    else:unix: LIBS += -L$$PWD/../../VS2015_Tesseract-master/ -lliblept

    win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../VS2015_Tesseract-master/release/ -llibtesseract
    else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../VS2015_Tesseract-master/debug/ -llibtesseract
    else:unix: LIBS += -L$$PWD/../../VS2015_Tesseract-master/ -llibtesseract

    win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-QtXlsxWriter-Desktop_Qt_5_9_1_MSVC2015_32bit-Release/release/ -lQtXlsxWriter
    else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-QtXlsxWriter-Desktop_Qt_5_9_1_MSVC2015_32bit-Debug/debug/ -lQtXlsxWriter
    else:unix: LIBS += -L$$PWD/../build-QtXlsxWriter-Desktop_Qt_5_9_1_MSVC2015_32bit-Debug/ -lQtXlsxWriter
}
    INCLUDEPATH += $$PWD/../../VS2015_Tesseract-master/zlib/include
    DEPENDPATH += $$PWD/../../VS2015_Tesseract-master/zlib/include

    INCLUDEPATH += $$PWD/../../VS2015_Tesseract-master/giflib
    DEPENDPATH += $$PWD/../../VS2015_Tesseract-master/giflib

    INCLUDEPATH += $$PWD/../../VS2015_Tesseract-master/libjpeg
    DEPENDPATH += $$PWD/../../VS2015_Tesseract-master/libjpeg

    INCLUDEPATH += $$PWD/../../VS2015_Tesseract-master/libpng
    DEPENDPATH += $$PWD/../../VS2015_Tesseract-master/libpng

    INCLUDEPATH += $$PWD/../../VS2015_Tesseract-master/libtiff
    DEPENDPATH += $$PWD/../../VS2015_Tesseract-master/libtiff

    INCLUDEPATH += $$PWD/../../VS2015_Tesseract-master/libwebp/src
    DEPENDPATH += $$PWD/../../VS2015_Tesseract-master/libwebp/src

    INCLUDEPATH += $$PWD/../../VS2015_Tesseract-master/openjpeg
    DEPENDPATH += $$PWD/../../VS2015_Tesseract-master/openjpeg

    INCLUDEPATH += $$PWD/../../VS2015_Tesseract-master/liblept
    DEPENDPATH += $$PWD/../../VS2015_Tesseract-master/liblept

    INCLUDEPATH += $$PWD/../../VS2015_Tesseract-master/leptonica/src
    DEPENDPATH += $$PWD/../../VS2015_Tesseract-master/leptonica/src

    INCLUDEPATH += $$PWD/../../VS2015_Tesseract-master\tesseract_3.04\api
    INCLUDEPATH += $$PWD/../../VS2015_Tesseract-master\tesseract_3.04\ccmain
    INCLUDEPATH += $$PWD/../../VS2015_Tesseract-master\tesseract_3.04\ccstruct
    INCLUDEPATH += $$PWD/../../VS2015_Tesseract-master\tesseract_3.04\ccutil
    INCLUDEPATH += $$PWD/../../VS2015_Tesseract-master\tesseract_3.04\classify
    INCLUDEPATH += $$PWD/../../VS2015_Tesseract-master\tesseract_3.04\cube
    INCLUDEPATH += $$PWD/../../VS2015_Tesseract-master\tesseract_3.04\cutil
    INCLUDEPATH += $$PWD/../../VS2015_Tesseract-master\tesseract_3.04\dict
    INCLUDEPATH += $$PWD/../../VS2015_Tesseract-master\tesseract_3.04\neural_networks\runtime
    INCLUDEPATH += $$PWD/../../VS2015_Tesseract-master\tesseract_3.04\opencl
    INCLUDEPATH += $$PWD/../../VS2015_Tesseract-master\tesseract_3.04\textord
    INCLUDEPATH += $$PWD/../../VS2015_Tesseract-master\tesseract_3.04\training
    INCLUDEPATH += $$PWD/../../VS2015_Tesseract-master\tesseract_3.04\viewer
    INCLUDEPATH += $$PWD/../../VS2015_Tesseract-master\tesseract_3.04\vs2010\port
    INCLUDEPATH += $$PWD/../../VS2015_Tesseract-master\tesseract_3.04\wordrec

    DEPENDPATH += $$PWD/../../VS2015_Tesseract-master\liblept
    DEPENDPATH += $$PWD/../../VS2015_Tesseract-master\leptonica\src
    DEPENDPATH += $$PWD/../../VS2015_Tesseract-master\tesseract_3.04\api
    DEPENDPATH += $$PWD/../../VS2015_Tesseract-master\tesseract_3.04\ccmain
    DEPENDPATH += $$PWD/../../VS2015_Tesseract-master\tesseract_3.04\ccmain
    DEPENDPATH += $$PWD/../../VS2015_Tesseract-master\tesseract_3.04\ccutil
    DEPENDPATH += $$PWD/../../VS2015_Tesseract-master\tesseract_3.04\classify
    DEPENDPATH += $$PWD/../../VS2015_Tesseract-master\tesseract_3.04\cube
    DEPENDPATH += $$PWD/../../VS2015_Tesseract-master\tesseract_3.04\cutil
    DEPENDPATH += $$PWD/../../VS2015_Tesseract-master\tesseract_3.04\dict
    DEPENDPATH += $$PWD/../../VS2015_Tesseract-master\tesseract_3.04\neural_networks\runtime
    DEPENDPATH += $$PWD/../../VS2015_Tesseract-master\tesseract_3.04\opencl
    DEPENDPATH += $$PWD/../../VS2015_Tesseract-master\tesseract_3.04\textord
    DEPENDPATH += $$PWD/../../VS2015_Tesseract-master\tesseract_3.04\training
    DEPENDPATH += $$PWD/../../VS2015_Tesseract-master\tesseract_3.04\viewer
    DEPENDPATH += $$PWD/../../VS2015_Tesseract-master\tesseract_3.04\vs2010\port
    DEPENDPATH += $$PWD/../../VS2015_Tesseract-master\tesseract_3.04\wordrec

    INCLUDEPATH += $$PWD/../QtXlsxWriter
    DEPENDPATH += $$PWD/../QtXlsxWriter

INCLUDEPATH += $$PWD/../StarIO_Cpp_SDK_Win_V210/Software/Dependencies
DEPENDPATH += $$PWD/../StarIO_Cpp_SDK_Win_V210/Software/Dependencies
