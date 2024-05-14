QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

TARGET = qTame
TEMPLATE = app

RC_ICONS = logo.ico
VERSION = 1.0.0.0
QMAKE_TARGET_COMPANY = Tamedhon
QMAKE_TARGET_PRODUCT = Tamedhon qTame
QMAKE_TARGET_DESCRIPTION = Tamedhon qTame MUD Client
QMAKE_TARGET_COPYRIGHT = (c) 2024 - Aloy


SOURCES += main.cpp\
    QTelnet.cpp \
    QCmdWidget.cpp \
    crypto.cpp \
    qTame.cpp \
    qaesencryption.cpp

HEADERS  += \
    EnumHelper.h \
    QTelnet.h \
    QCmdWidget.h \
    aesni/aesni-enc-cbc.h \
    aesni/aesni-enc-ecb.h \
    aesni/aesni-key-exp.h \
    aesni/aesni-key-init.h \
    crypto.h \
    qTame.h \
    qaesencryption.h

FORMS    += \
    qTame.ui

RESOURCES += \
    resources.qrc
