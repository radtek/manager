#-------------------------------------------------
#
# Project created by QtCreator 2011-08-11T20:59:25
#
#-------------------------------------------------

QT       += core network
CONFIG += c++11
TARGET = SMTPEmail

# Build as an application
#TEMPLATE = app
# Build as a library
TEMPLATE = lib
DEFINES += SMTP_BUILD
win32:CONFIG += dll

#QMAKE_CXXFLAGS += -fPIC
#5.5.1 -------------------------------------
#DLLDESTDIR = "C:\Users\alexander\Desktop\programa\build-Compra-Desktop_Qt_5_5_1_MinGW_32bit-Debug\debug"
#DLLDESTDIR = "C:\Users\alexander\Desktop\programa\build-Venta-Desktop_Qt_5_5_1_MinGW_32bit-Debug\debug"
#DLLDESTDIR = "C:\Users\alexander\Desktop\programa\build-Producto-Desktop_Qt_5_5_1_MinGW_32bit-Debug\debug"
#DLLDESTDIR = "C:\Users\alexander\Desktop\programa\build-Persona-Desktop_Qt_5_5_1_MinGW_32bit-Debug\debug"
#DLLDESTDIR = "C:\Users\alexander\Desktop\programa\build-ManagerSystem-Desktop_Qt_5_5_1_MinGW_32bit-Debug\debug"
#DLLDESTDIR = "C:\Users\alexander\Desktop\programa\build-MyLibrary-Desktop_Qt_5_5_1_MinGW_32bit-Debug\debug"
#5.6.0 -------------------------------------
#DLLDESTDIR = "C:\Users\alexander\Desktop\programa\build-Compra-Desktop_Qt_5_6_0_MinGW_32bit-Debug\debug"
#DLLDESTDIR = "C:\Users\alexander\Desktop\programa\build-Venta-Desktop_Qt_5_6_0_MinGW_32bit-Debug\debug"
#DLLDESTDIR = "C:\Users\alexander\Desktop\programa\build-Producto-Desktop_Qt_5_6_0_MinGW_32bit-Debug\debug"
#DLLDESTDIR = "C:\Users\alexander\Desktop\programa\build-Persona-Desktop_Qt_5_6_0_MinGW_32bit-Debug\debug"
#DLLDESTDIR = "C:\Users\alexander\Desktop\programa\build-ManagerSystem-Desktop_Qt_5_6_0_MinGW_32bit-Debug\debug"
#DLLDESTDIR = "C:\Users\alexander\Desktop\programa\build-MyLibrary-Desktop_Qt_5_6_0_MinGW_32bit-Debug\debug"
#5.7.0 -------------------------------------
#DLLDESTDIR = "C:\Users\lorda\Desktop\programa\build-Compra-Desktop_Qt_5_7_0_MSVC2015_64bit-Debug\debug"
#DLLDESTDIR = "C:\Users\lorda\Desktop\programa\build-Venta-Desktop_Qt_5_7_0_MSVC2015_64bit-Debug\debug"
#DLLDESTDIR = "C:\Users\lorda\Desktop\programa\build-Producto-Desktop_Qt_5_7_0_MSVC2015_64bit-Debug\debug"
#DLLDESTDIR = "C:\Users\lorda\Desktop\programa\build-Persona-Desktop_Qt_5_7_0_MSVC2015_64bit-Debug\debug"

DLLDESTDIR += "C:\Users\lorda\Desktop\programa\build-MyLibrary-Desktop_Qt_5_7_0_MSVC2015_64bit-Debug\debug"
DLLDESTDIR += "C:\Users\lorda\Desktop\programa\build-ManagerSystem-Desktop_Qt_5_7_0_MSVC2015_64bit-Debug\debug"

SOURCES += \
    src/emailaddress.cpp \
    src/mimeattachment.cpp \
    src/mimefile.cpp \
    src/mimehtml.cpp \
    src/mimeinlinefile.cpp \
    src/mimemessage.cpp \
    src/mimepart.cpp \
    src/mimetext.cpp \
    src/smtpclient.cpp \
    src/quotedprintable.cpp \
    src/mimemultipart.cpp \
    src/mimecontentformatter.cpp \

HEADERS  += \
    src/emailaddress.h \
    src/mimeattachment.h \
    src/mimefile.h \
    src/mimehtml.h \
    src/mimeinlinefile.h \
    src/mimemessage.h \
    src/mimepart.h \
    src/mimetext.h \
    src/smtpclient.h \
    src/SmtpMime \
    src/quotedprintable.h \
    src/mimemultipart.h \
    src/mimecontentformatter.h \
    src/smtpexports.h

OTHER_FILES += \
    LICENSE \
    README.md

FORMS +=
