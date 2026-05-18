QT += core gui widgets sql network openglwidgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    databasemanager.cpp \
    main.cpp \
    myglwidget.cpp \
    mypainter.cpp \
    osnova.cpp \
    wearcalculator.cpp

HEADERS += \
    databasemanager.h \
    myglwidget.h \
    mypainter.h \
    osnova.h \
    wearcalculator.h

FORMS += \
    osnova.ui

TRANSLATIONS += \
    automechanic_ru_RU.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
LIBS += -lopengl32

RESOURCES += \
    logo.qrc