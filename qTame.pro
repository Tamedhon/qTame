QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

TARGET = qTame
TEMPLATE = app

VERSION = 1.0.0
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

win32
{
    RC_ICONS = logo.ico
    QMAKE_TARGET_COMPANY = Tamedhon
    QMAKE_TARGET_PRODUCT = Tamedhon qTame
    QMAKE_TARGET_DESCRIPTION = Tamedhon qTame MUD Client
    QMAKE_TARGET_COPYRIGHT = (c) 2024 - Aloy
}


SOURCES += main.cpp\
    QTelnet.cpp \
    QCmdWidget.cpp \
    crypto.cpp \
    info.cpp \
    qTame.cpp \
    qaesencryption.cpp \
    qtextconsole.cpp \
    settings.cpp

HEADERS  += \
    QTelnet.h \
    QCmdWidget.h \
    aesni/aesni-enc-cbc.h \
    aesni/aesni-enc-ecb.h \
    aesni/aesni-key-exp.h \
    aesni/aesni-key-init.h \
    crypto.h \
    enum.h \
    info.h \
    qTame.h \
    qaesencryption.h \
    qtextconsole.h \
    settings.h

FORMS    += \
    info.ui \
    qTame.ui \
    settings.ui

RESOURCES += \
    resources.qrc
