TARGET = shiftnet-billing-client
TEMPLATE = app
DESTDIR = $$PWD/../dist
QT = core gui widgets

SOURCES += \
    main.cpp \
    screenlocker.cpp \
    adminloginwidget.cpp \
    settingswidget.cpp \
    memberloginwidget.cpp

HEADERS  += \
    screenlocker.h \
    adminloginwidget.h \
    settingswidget.h \
    global.h \
    memberloginwidget.h

RESOURCES += \
    main.qrc

FORMS += \
    adminloginwidget.ui \
    settingswidget.ui \
    memberloginwidget.ui
