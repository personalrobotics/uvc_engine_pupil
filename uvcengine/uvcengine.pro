#-------------------------------------------------
#
# Project created by QtCreator 2017-02-16T18:53:42
#
#-------------------------------------------------

TOP = $$PWD

QT       += core gui multimedia

TARGET = uvcengine
TEMPLATE = lib
CONFIG += plugin
CONFIG += console

DESTDIR = $$[QT_INSTALL_PLUGINS]/mediaservice

SOURCES += uvcserviceplugin.cpp \
    uvccameraservice.cpp \
    uvcvideodevicecontrol.cpp \
    uvccameracontrol.cpp \
    uvccamerasession.cpp \
    uvcvideorenderercontrol.cpp \
    uvccameraviewfindersettingscontrol.cpp

HEADERS += uvcserviceplugin.h \
    uvccameraservice.h \
    uvcvideodevicecontrol.h \
    uvccameracontrol.h \
    uvccamerasession.h \
    uvcvideorenderercontrol.h \
    uvccameraviewfindersettingscontrol.h
DISTFILES += uvcengine.json

unix {
    target.path = /usr/lib
    INSTALLS += target
}

# Dependencies
# (sniyaz): You may need to point these at your own install locations for these.
LIBUSBPATH = "/usr/"
LIBUVCPATH = "/usr/local/"

# Libraries required to access pupil lab's cameras
# LIBUSB-1.0
INCLUDEPATH += "$${LIBUSBPATH}/include/libusb-1.0/"
LIBS += "-L$${LIBUSBPATH}/lib/x86_64-linux-gnu/"
LIBS += -lusb-1.0

# LIBUVC
INCLUDEPATH += "$${LIBUVCPATH}/include/libuvc/"
LIBS += "-L$${LIBUVCPATH}/lib/"
LIBS += -luvc

# PTHREADS
LIBS += -lpthread

# Work around for bad visual studio update (msvc14)
win32{
    INCLUDEPATH += "C:/Program Files (x86)/Windows Kits/10/Include/10.0.10240.0/ucrt"
    contains(QMAKE_HOST.arch, x86_64) {
        LIBS += -L"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.10240.0/ucrt/x64"
    } else {
        LIBS += -L"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.10240.0/ucrt/x86"
    }
}
