QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    audiovisivo.cpp \
    cartaceo.cpp \
    data.cpp \
    film.cpp \
    gestorejson.cpp \
    gestoremedia.cpp \
    giornale.cpp \
    libro.cpp \
    main.cpp \
    mainwindow.cpp \
    media.cpp \
    rivista.cpp \
    vinile.cpp

HEADERS += \
    audiovisivo.h \
    cartaceo.h \
    data.h \
    film.h \
    gestorejson.h \
    gestoremedia.h \
    giornale.h \
    libro.h \
    mainwindow.h \
    media.h \
    rivista.h \
    vinile.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
