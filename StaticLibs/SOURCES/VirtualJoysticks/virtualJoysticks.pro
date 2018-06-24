CONFIG(debug, debug|release) {
	DESTDIR = debug
}
CONFIG(release, debug|release) {
	DESTDIR = release
}


TEMPLATE = lib
TARGET = virtualJoysticks
OBJECTS_DIR = $$DESTDIR/objects
MOC_DIR = $$DESTDIR/moc
CONFIG += c++11

DEFINES += VIRTUALJOYSTICKS_LIBRARY

LIBS += ../../../vJoy218SDK-291116/SDK/lib/amd64/vJoyInterface.lib

INCLUDEPATH +=  ../../INCLUDES/Other/ \
				../../INCLUDES/VirtualJoysticks/

HEADERS +=  ../../INCLUDES/VirtualJoysticks/VirtualJoystick.h \
    ../../INCLUDES/VirtualJoysticks/virtualjoysticks-global.h
SOURCES +=  CODE/VirtualJoystick.cpp

