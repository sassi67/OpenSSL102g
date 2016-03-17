QT += core
QT -= gui

TARGET = OpenSSL102g
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

INCLUDEPATH += $$PWD/include/openssl

win32: LIBS += -L$$PWD/lib/openssl/win32/lib \
    -llibeay32
else:unix: LIBS += -L$$PWD/lib/openssl/unix \
    -lcrypto


#install_.path = $$OUT_PWD
#win32: install_.files = $$PWD/lib/openssl/win32/bin/libeay32.dll
#win32: install_.extra = del /F /Q "$$OUT_PWD/libeay32.dll"
#INSTALLS = install_

SOURCES += main.cpp
