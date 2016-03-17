QT += core
QT -= gui

#CONFIG += c++11

TARGET = OpenSSL102g
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

INCLUDEPATH += $$PWD/include/openssl

win32: LIBS += -L$$PWD/lib/openssl/win32 \
    -lcrypto
else:unix: LIBS += -L$$PWD/lib/openssl/unix \
    -lcrypto

SOURCES += main.cpp
