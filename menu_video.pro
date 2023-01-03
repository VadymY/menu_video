#QT       += core gui
QT += multimedia multimediawidgets multimedia-private

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Video
TEMPLATE = app

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    VideoFrameGrabber.cpp \
    main.cpp \
    mainwindow.cpp \
    myvideosurface.cpp \
    process_write_frame.cpp

HEADERS += \
    VideoFrameGrabber.h \
    mainwindow.h \
    myvideosurface.h \
    process_write_frame.h

TRANSLATIONS += main_en.ts
TRANSLATIONS += main_ukr.ts
TRANSLATIONS += main_ru.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RC_FILE = dron.rc

RESOURCES += \
    main.qrc \
    main.qrc
