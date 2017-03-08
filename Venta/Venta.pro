#-------------------------------------------------
#
# Project created by QtCreator 2017-01-04T17:40:48
#
#-------------------------------------------------

QT       += widgets qml quick network opengl sql script scripttools svg webengine webenginecore webenginewidgets xml xmlpatterns multimedia testlib dbus
QT       += charts texttospeech
greaterThan(QT_MAJOR_VERSION, 4): QT += printsupport

TARGET = Venta
TEMPLATE = lib

DEFINES += VENTA_LIBRARY

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

SOURCES += venta.cpp \
    a_venta.cpp \
    ventaamarres.cpp \
    ventaboleta.cpp \
    ventabuscar.cpp \
    ventacliente.cpp \
    ventaconfiguracion.cpp \
    ventacotizacion.cpp \
    ventafactura.cpp \
    ventamenu.cpp \
    ventanotacredito.cpp \
    ventanotadebito.cpp \
    ventanotapedido.cpp \
    ventaproducto.cpp \
    ventaregistrosindoc.cpp \
    ventathreadsetproducto.cpp \
    ventaguiarr.cpp \
    ventaconfighoja.cpp \
    ventasnselector.cpp \
    ventachartprecio.cpp

HEADERS += venta.h\
        venta_global.h \
    a_venta.h \
    ventaamarres.h \
    ventaboleta.h \
    ventabuscar.h \
    ventacliente.h \
    ventaconfiguracion.h \
    ventacotizacion.h \
    ventafactura.h \
    ventamenu.h \
    ventanotacredito.h \
    ventanotadebito.h \
    ventanotapedido.h \
    ventaproducto.h \
    ventaregistrosindoc.h \
    ventathreadsetproducto.h \
    ventaguiarr.h \
    ventaconfighoja.h \
    ventasnselector.h \
    ventachartprecio.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    a_venta.ui \
    ventaamarres.ui \
    ventaboleta.ui \
    ventabuscar.ui \
    ventaconfiguracion.ui \
    ventacotizacion.ui \
    ventafactura.ui \
    ventamenu.ui \
    ventanotacredito.ui \
    ventanotadebito.ui \
    ventanotapedido.ui \
    ventaregistrosindoc.ui \
    ventaguiarr.ui \
    ventaconfighoja.ui \
    ventasnselector.ui \
    ventachartprecio.ui

RESOURCES += \
    ../ManagerSystem/Iconos.qrc

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

    win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-MyLibrary-Desktop_Qt_5_8_0_MSVC2015_64bit-Release/release/ -lMyLibrary
    else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-MyLibrary-Desktop_Qt_5_8_0_MSVC2015_64bit-Debug/debug/ -lMyLibrary
    else:unix: LIBS += -L$$PWD/../build-MyLibrary-Desktop_Qt_5_8_0_MSVC2015_64bit-Debug/ -lMyLibrary

    win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-Producto-Desktop_Qt_5_8_0_MSVC2015_64bit-Release/release/ -lProducto
    else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-Producto-Desktop_Qt_5_8_0_MSVC2015_64bit-Debug/debug/ -lProducto
    else:unix: LIBS += -L$$PWD/../build-Producto-Desktop_Qt_5_8_0_MSVC2015_64bit-Debug/ -lProducto

    win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-Persona-Desktop_Qt_5_8_0_MSVC2015_64bit-Release/release/ -lPersona
    else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-Persona-Desktop_Qt_5_8_0_MSVC2015_64bit-Debug/debug/ -lPersona
    else:unix: LIBS += -L$$PWD/../build-Persona-Desktop_Qt_5_8_0_MSVC2015_64bit-Debug/ -lPersona

    win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-Compra-Desktop_Qt_5_8_0_MSVC2015_64bit-Release/release/ -lCompra
    else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-Compra-Desktop_Qt_5_8_0_MSVC2015_64bit-Debug/debug/ -lCompra
    else:unix: LIBS += -L$$PWD/../build-Compra-Desktop_Qt_5_8_0_MSVC2015_64bit-Debug/ -lCompra

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

    win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-MyLibrary-Desktop_Qt_5_8_0_MSVC2015_32bit-Release/release/ -lMyLibrary
    else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-MyLibrary-Desktop_Qt_5_8_0_MSVC2015_32bit-Debug/debug/ -lMyLibrary
    else:unix: LIBS += -L$$PWD/../build-MyLibrary-Desktop_Qt_5_8_0_MSVC2015_32bit-Debug/ -lMyLibrary

    win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-Producto-Desktop_Qt_5_8_0_MSVC2015_32bit-Release/release/ -lProducto
    else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-Producto-Desktop_Qt_5_8_0_MSVC2015_32bit-Debug/debug/ -lProducto
    else:unix: LIBS += -L$$PWD/../build-Producto-Desktop_Qt_5_8_0_MSVC2015_32bit-Debug/ -lProducto

    win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-Persona-Desktop_Qt_5_8_0_MSVC2015_32bit-Release/release/ -lPersona
    else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-Persona-Desktop_Qt_5_8_0_MSVC2015_32bit-Debug/debug/ -lPersona
    else:unix: LIBS += -L$$PWD/../build-Persona-Desktop_Qt_5_8_0_MSVC2015_32bit-Debug/ -lPersona

    win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-Compra-Desktop_Qt_5_8_0_MSVC2015_32bit-Release/release/ -lCompra
    else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-Compra-Desktop_Qt_5_8_0_MSVC2015_32bit-Debug/debug/ -lCompra
    else:unix: LIBS += -L$$PWD/../build-Compra-Desktop_Qt_5_8_0_MSVC2015_32bit-Debug/ -lCompra
}
    INCLUDEPATH += $$PWD/../../../VS2015_Tesseract-master/zlib/include
    DEPENDPATH += $$PWD/../../../VS2015_Tesseract-master/zlib/include

    INCLUDEPATH += $$PWD/../../../VS2015_Tesseract-master/giflib
    DEPENDPATH += $$PWD/../../../VS2015_Tesseract-master/giflib

    INCLUDEPATH += $$PWD/../../../VS2015_Tesseract-master/libjpeg
    DEPENDPATH += $$PWD/../../../VS2015_Tesseract-master/libjpeg

    INCLUDEPATH += $$PWD/../../../VS2015_Tesseract-master/libpng
    DEPENDPATH += $$PWD/../../../VS2015_Tesseract-master/libpng

    INCLUDEPATH += $$PWD/../../../VS2015_Tesseract-master/libtiff
    DEPENDPATH += $$PWD/../../../VS2015_Tesseract-master/libtiff

    INCLUDEPATH += $$PWD/../../../VS2015_Tesseract-master/libwebp/src
    DEPENDPATH += $$PWD/../../../VS2015_Tesseract-master/libwebp/src

    INCLUDEPATH += $$PWD/../../../VS2015_Tesseract-master/openjpeg
    DEPENDPATH += $$PWD/../../../VS2015_Tesseract-master/openjpeg

    INCLUDEPATH += $$PWD/../../../VS2015_Tesseract-master/liblept
    DEPENDPATH += $$PWD/../../../VS2015_Tesseract-master/liblept

    INCLUDEPATH += $$PWD/../../../VS2015_Tesseract-master/leptonica/src
    DEPENDPATH += $$PWD/../../../VS2015_Tesseract-master/leptonica/src

    INCLUDEPATH += $$PWD/../../../VS2015_Tesseract-master\tesseract_3.04\api
    INCLUDEPATH += $$PWD/../../../VS2015_Tesseract-master\tesseract_3.04\ccmain
    INCLUDEPATH += $$PWD/../../../VS2015_Tesseract-master\tesseract_3.04\ccstruct
    INCLUDEPATH += $$PWD/../../../VS2015_Tesseract-master\tesseract_3.04\ccutil
    INCLUDEPATH += $$PWD/../../../VS2015_Tesseract-master\tesseract_3.04\classify
    INCLUDEPATH += $$PWD/../../../VS2015_Tesseract-master\tesseract_3.04\cube
    INCLUDEPATH += $$PWD/../../../VS2015_Tesseract-master\tesseract_3.04\cutil
    INCLUDEPATH += $$PWD/../../../VS2015_Tesseract-master\tesseract_3.04\dict
    INCLUDEPATH += $$PWD/../../../VS2015_Tesseract-master\tesseract_3.04\neural_networks\runtime
    INCLUDEPATH += $$PWD/../../../VS2015_Tesseract-master\tesseract_3.04\opencl
    INCLUDEPATH += $$PWD/../../../VS2015_Tesseract-master\tesseract_3.04\textord
    INCLUDEPATH += $$PWD/../../../VS2015_Tesseract-master\tesseract_3.04\training
    INCLUDEPATH += $$PWD/../../../VS2015_Tesseract-master\tesseract_3.04\viewer
    INCLUDEPATH += $$PWD/../../../VS2015_Tesseract-master\tesseract_3.04\vs2010\port
    INCLUDEPATH += $$PWD/../../../VS2015_Tesseract-master\tesseract_3.04\wordrec

    DEPENDPATH += $$PWD/../../../VS2015_Tesseract-master\liblept
    DEPENDPATH += $$PWD/../../../VS2015_Tesseract-master\leptonica\src
    DEPENDPATH += $$PWD/../../../VS2015_Tesseract-master\tesseract_3.04\api
    DEPENDPATH += $$PWD/../../../VS2015_Tesseract-master\tesseract_3.04\ccmain
    DEPENDPATH += $$PWD/../../../VS2015_Tesseract-master\tesseract_3.04\ccmain
    DEPENDPATH += $$PWD/../../../VS2015_Tesseract-master\tesseract_3.04\ccutil
    DEPENDPATH += $$PWD/../../../VS2015_Tesseract-master\tesseract_3.04\classify
    DEPENDPATH += $$PWD/../../../VS2015_Tesseract-master\tesseract_3.04\cube
    DEPENDPATH += $$PWD/../../../VS2015_Tesseract-master\tesseract_3.04\cutil
    DEPENDPATH += $$PWD/../../../VS2015_Tesseract-master\tesseract_3.04\dict
    DEPENDPATH += $$PWD/../../../VS2015_Tesseract-master\tesseract_3.04\neural_networks\runtime
    DEPENDPATH += $$PWD/../../../VS2015_Tesseract-master\tesseract_3.04\opencl
    DEPENDPATH += $$PWD/../../../VS2015_Tesseract-master\tesseract_3.04\textord
    DEPENDPATH += $$PWD/../../../VS2015_Tesseract-master\tesseract_3.04\training
    DEPENDPATH += $$PWD/../../../VS2015_Tesseract-master\tesseract_3.04\viewer
    DEPENDPATH += $$PWD/../../../VS2015_Tesseract-master\tesseract_3.04\vs2010\port
    DEPENDPATH += $$PWD/../../../VS2015_Tesseract-master\tesseract_3.04\wordrec

    INCLUDEPATH += $$PWD/../MyLibrary
    DEPENDPATH += $$PWD/../MyLibrary
    INCLUDEPATH += $$PWD/../Producto
    DEPENDPATH += $$PWD/../Producto
    INCLUDEPATH += $$PWD/../Persona
    DEPENDPATH += $$PWD/../Persona
    INCLUDEPATH += $$PWD/../Persona
    DEPENDPATH += $$PWD/../Persona
    INCLUDEPATH += $$PWD/../Compra
    DEPENDPATH += $$PWD/../Compra

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-QtXlsxWriter-Desktop_Qt_5_8_0_MSVC2015_64bit-Debug/release/ -lQtXlsxWriter
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-QtXlsxWriter-Desktop_Qt_5_8_0_MSVC2015_64bit-Debug/debug/ -lQtXlsxWriter
else:unix: LIBS += -L$$PWD/../build-QtXlsxWriter-Desktop_Qt_5_8_0_MSVC2015_64bit-Debug/ -lQtXlsxWriter

INCLUDEPATH += $$PWD/../QtXlsxWriter
DEPENDPATH += $$PWD/../QtXlsxWriter
