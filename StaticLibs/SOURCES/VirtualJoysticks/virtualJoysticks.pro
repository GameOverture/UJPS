win32-g++:CONFIG(debug, debug|release) {
	DESTDIR = debug
	LIBS += ../../../vJoy218SDK-291116/SDK/lib/vJoyInterface.lib
}
win32-g++:CONFIG(release, debug|release) {
	DESTDIR = release
	LIBS += ../../../vJoy218SDK-291116/SDK/lib/vJoyInterface.lib
}

win32-msvc*:CONFIG(debug, debug|release) {
    DESTDIR = debug
	LIBS += ../../../vJoy218SDK-291116/SDK/lib/amd64/vJoyInterface.lib
}
win32-msvc*:CONFIG(release, debug|release) {
    DESTDIR = release
	LIBS += ../../../vJoy218SDK-291116/SDK/lib/amd64/vJoyInterface.lib
}

TEMPLATE = lib
TARGET = virtualJoysticks
OBJECTS_DIR = $$DESTDIR/objects
MOC_DIR = $$DESTDIR/moc
CONFIG += c++11

DEFINES += VIRTUALJOYSTICKS_LIBRARY

INCLUDEPATH +=  ../../INCLUDES/Other/ \
				../../INCLUDES/VirtualJoysticks/

HEADERS +=  ../../INCLUDES/VirtualJoysticks/VirtualJoystick.h \
    ../../INCLUDES/VirtualJoysticks/virtualjoysticks-global.h
SOURCES +=  CODE/VirtualJoystick.cpp

