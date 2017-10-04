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
LIBUSBPATH = "C:/Users/santini/repo/libusb/"
LIBUVCPATH = "C:/Users/santini/repo/libuvc/"
LIBPTHREADSPATH = "C:/Users/santini/repo/libuvc/pthreads-2.9.1/"

# Libraries required to access pupil lab's cameras
# LIBUSB-1.0
INCLUDEPATH += "$${LIBUSBPATH}/libusb/"
win32:contains(QMAKE_HOST.arch, x86_64) {
    Release:LIBS += "-L$${LIBUSBPATH}/x64/Release/dll"
    Debug:LIBS += "-L$${LIBUSBPATH}/x64/Debug/dll"
} else {
    Release:LIBS += "-L$${LIBUSBPATH}/x86/Release/dll"
    Debug:LIBS += "-L$${LIBUSBPATH}/x86/Debug/dll"
}
LIBS += -llibusb-1.0

# LIBUVC
INCLUDEPATH += "$${LIBUVCPATH}/install/include/"
win32:contains(QMAKE_HOST.arch, x86_64) {
    Release:LIBS += "-L$${LIBUVCPATH}/build/Release/"
    Debug:LIBS += "-L$${LIBUVCPATH}/build/Debug"
} else {
    Release:LIBS += "-L$${LIBUVCPATH}/build/Release/"
    Debug:LIBS += "-L$${LIBUVCPATH}/build/Debug"
}
LIBS += -luvc

# PTHREADS
INCLUDEPATH += "$${LIBPTHREADSPATH}/include/"
win32:contains(QMAKE_HOST.arch, x86_64) {
    Release:LIBS += "-L$${LIBPTHREADSPATH}/lib/x64/"
    Debug:LIBS += "-L$${LIBPTHREADSPATH}/lib/x64/"
} else {
    Release:LIBS += "-L$${LIBPTHREADSPATH}/lib/x86/"
    Debug:LIBS += "-L$${LIBPTHREADSPATH}/lib/x86/"
}
LIBS += -lpthreadVC2

# Work around for bad visual studio update (msvc14)
win32{
    INCLUDEPATH += "C:/Program Files (x86)/Windows Kits/10/Include/10.0.10240.0/ucrt"
    contains(QMAKE_HOST.arch, x86_64) {
        LIBS += -L"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.10240.0/ucrt/x64"
    } else {
        LIBS += -L"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.10240.0/ucrt/x86"
    }
}
