TEMPLATE = app
CONFIG += console c++11 static
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lgcc
QMAKE_LFLAGS += -static

VERSION = 1.0.0

win32
{
    RC_ICONS = logo.ico
    QMAKE_TARGET_COMPANY = Tamedhon
    QMAKE_TARGET_PRODUCT = Tamedhon qTame Launcher
    QMAKE_TARGET_DESCRIPTION = Tamedhon qTame MUD Client Launcher
    QMAKE_TARGET_COPYRIGHT = (c) 2024 - Aloy
}

SOURCES += \
        main.cpp
