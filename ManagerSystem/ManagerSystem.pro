#-------------------------------------------------
#
# Project created by QtCreator 2017-01-04T17:51:40
#
#-------------------------------------------------
CONFIG += c++14

QT       += core gui opengl sql webengine webenginewidgets charts

greaterThan(QT_MAJOR_VERSION, 4): QT += printsupport

TARGET = ManagerSystem
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += FACTURACION_LIBRARY
DEFINES += MYLIBRARY_LIBRARY

DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        managersystem.cpp \
    _startprogram.cpp \
    configuracion.cpp \
    home.cpp \
    login.cpp \
    opcionesdb.cpp \
    toolbar.cpp \
    vercambio.cpp \
    verigv.cpp \
    compra/compraadjacentes.cpp \
    compra/compraamarres.cpp \
    compra/compraboleta.cpp \
    compra/comprabuscar.cpp \
    compra/compracanjear.cpp \
    compra/comprachartcosto.cpp \
    compra/comprafactura.cpp \
    compra/compraflete.cpp \
    compra/compraguiarr.cpp \
    compra/compranotacredito.cpp \
    compra/compranotadebito.cpp \
    compra/compraorden.cpp \
    compra/compraproducto.cpp \
    compra/compraproductocombobox.cpp \
    compra/compraproductodelegate.cpp \
    compra/compraproveedor.cpp \
    compra/compraregsindoc.cpp \
    compra/comprasaldo.cpp \
    compra/comprathreadsetproducto.cpp \
    compra/compratransportista.cpp \
    mylibrary/comboboxdelegate.cpp \
    mylibrary/database_connection.cpp \
    mylibrary/doublespinboxdelegate.cpp \
    mylibrary/lineedit.cpp \
    mylibrary/lineeditdelegate.cpp \
    mylibrary/lineeditnumberdelegate.cpp \
    mylibrary/mylibrary.cpp \
    mylibrary/reniecconsultadni.cpp \
    mylibrary/session.cpp \
    mylibrary/sistema.cpp \
    mylibrary/snackbarinfo.cpp \
    mylibrary/sunatconsultaruc.cpp \
    mylibrary/sunattipocambio.cpp \
    mylibrary/widgetnumber.cpp \
    persona/personabuscar.cpp \
    persona/personaclientedni.cpp \
    persona/personaclienteruc.cpp \
    persona/personaproveedor.cpp \
    persona/personatransportista.cpp \
    persona/personausuario.cpp \
    producto/producto.cpp \
    producto/productobuscar.cpp \
    producto/productomarca.cpp \
    producto/productomarcabuscar.cpp \
    producto/productotipo.cpp \
    producto/productotipobuscar.cpp \
    producto/productounidad.cpp \
    producto/productounidadbuscar.cpp \
    venta/ventaamarres.cpp \
    venta/ventaboleta.cpp \
    venta/ventabuscar.cpp \
    venta/ventachartprecio.cpp \
    venta/ventacliente.cpp \
    venta/ventaconfighoja.cpp \
    venta/ventaconfiguracion.cpp \
    venta/ventacotizacion.cpp \
    venta/ventafactura.cpp \
    venta/ventaguiarr.cpp \
    venta/ventamenu.cpp \
    venta/ventanotacredito.cpp \
    venta/ventanotadebito.cpp \
    venta/ventanotapedido.cpp \
    venta/ventaproducto.cpp \
    venta/ventaregistrosindoc.cpp \
    venta/ventasnselector.cpp \
    venta/ventathreadsetproducto.cpp \
    salvados.cpp

HEADERS  += managersystem.h \
    _startprogram.h \
    configuracion.h \
    home.h \
    login.h \
    opcionesdb.h \
    toolbar.h \
    locallibrary.h \
    modules_library.h \
    shares.h \
    vercambio.h \
    verigv.h \
    compra/compra_headers.h \
    compra/compraadjacentes.h \
    compra/compraamarres.h \
    compra/compraboleta.h \
    compra/comprabuscar.h \
    compra/compracanjear.h \
    compra/comprachartcosto.h \
    compra/comprafactura.h \
    compra/compraflete.h \
    compra/compraguiarr.h \
    compra/compranotacredito.h \
    compra/compranotadebito.h \
    compra/compraorden.h \
    compra/compraproducto.h \
    compra/compraproductocombobox.h \
    compra/compraproductodelegate.h \
    compra/compraproveedor.h \
    compra/compraregsindoc.h \
    compra/comprasaldo.h \
    compra/comprathreadsetproducto.h \
    compra/compratransportista.h \
    mylibrary/comboboxdelegate.h \
    mylibrary/database_connection.h \
    mylibrary/doublespinboxdelegate.h \
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
    mylibrary/sunatconsultaruc.h \
    mylibrary/sunattipocambio.h \
    mylibrary/widgetnumber.h \
    persona/persona_headers.h \
    persona/personabuscar.h \
    persona/personaclientedni.h \
    persona/personaclienteruc.h \
    persona/personaproveedor.h \
    persona/personatransportista.h \
    persona/personausuario.h \
    producto/producto.h \
    producto/producto_headers.h \
    producto/productobuscar.h \
    producto/productomarca.h \
    producto/productomarcabuscar.h \
    producto/productotipo.h \
    producto/productotipobuscar.h \
    producto/productounidad.h \
    producto/productounidadbuscar.h \
    venta/venta_headers.h \
    venta/ventaamarres.h \
    venta/ventaboleta.h \
    venta/ventabuscar.h \
    venta/ventachartprecio.h \
    venta/ventacliente.h \
    venta/ventaconfighoja.h \
    venta/ventaconfiguracion.h \
    venta/ventacotizacion.h \
    venta/ventafactura.h \
    venta/ventaguiarr.h \
    venta/ventamenu.h \
    venta/ventanotacredito.h \
    venta/ventanotadebito.h \
    venta/ventanotapedido.h \
    venta/ventaproducto.h \
    venta/ventaregistrosindoc.h \
    venta/ventasnselector.h \
    venta/ventathreadsetproducto.h \
    salvados.h \
    shares.h

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

    win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-QtXlsxWriter-Desktop_Qt_5_8_0_MSVC2015_64bit-Release/release/ -lQtXlsxWriter
    else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-QtXlsxWriter-Desktop_Qt_5_8_0_MSVC2015_64bit-Debug/debug/ -lQtXlsxWriter
    else:unix: LIBS += -L$$PWD/../build-QtXlsxWriter-Desktop_Qt_5_8_0_MSVC2015_64bit-Debug/ -lQtXlsxWriter
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

    win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-QtXlsxWriter-Desktop_Qt_5_8_0_MSVC2015_32bit-Release/release/ -lQtXlsxWriter
    else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-QtXlsxWriter-Desktop_Qt_5_8_0_MSVC2015_32bit-Debug/debug/ -lQtXlsxWriter
    else:unix: LIBS += -L$$PWD/../build-QtXlsxWriter-Desktop_Qt_5_8_0_MSVC2015_32bit-Debug/ -lQtXlsxWriter
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

RESOURCES += \
    iconos.qrc

FORMS += \
    configuracion.ui \
    home.ui \
    login.ui \
    managersystem.ui \
    opcionesdb.ui \
    toolbar.ui \
    vercambio.ui \
    verigv.ui \
    compra/compraadjacentes.ui \
    compra/compraamarres.ui \
    compra/compraboleta.ui \
    compra/comprabuscar.ui \
    compra/compracanjear.ui \
    compra/comprachartcosto.ui \
    compra/comprafactura.ui \
    compra/compraflete.ui \
    compra/compraguiarr.ui \
    compra/compranotacredito.ui \
    compra/compranotadebito.ui \
    compra/compraorden.ui \
    compra/compraregsindoc.ui \
    compra/comprasaldo.ui \
    mylibrary/snackbarinfo.ui \
    persona/personabuscar.ui \
    persona/personaclientedni.ui \
    persona/personaclienteruc.ui \
    persona/personaproveedor.ui \
    persona/personatransportista.ui \
    persona/personausuario.ui \
    producto/producto.ui \
    producto/productobuscar.ui \
    producto/productomarca.ui \
    producto/productomarcabuscar.ui \
    producto/productotipo.ui \
    producto/productotipobuscar.ui \
    producto/productounidad.ui \
    producto/productounidadbuscar.ui \
    venta/ventaamarres.ui \
    venta/ventaboleta.ui \
    venta/ventabuscar.ui \
    venta/ventachartprecio.ui \
    venta/ventaconfighoja.ui \
    venta/ventaconfiguracion.ui \
    venta/ventacotizacion.ui \
    venta/ventafactura.ui \
    venta/ventaguiarr.ui \
    venta/ventamenu.ui \
    venta/ventanotacredito.ui \
    venta/ventanotadebito.ui \
    venta/ventanotapedido.ui \
    venta/ventaregistrosindoc.ui \
    venta/ventasnselector.ui \
    salvados.ui
