win32-g++:CONFIG(debug, debug|release) {
    DESTDIR = debug
	LIBS += ../../../StaticLibs/SOURCES/QtGameControllerModif/debug/libQtGameControllerModif.a \
			../../../StaticLibs/SOURCES/RealJoysticks/debug/librealJoysticks.a
}
win32-g++:CONFIG(release, debug|release) {
    DESTDIR = release
	LIBS += ../../../StaticLibs/SOURCES/QtGameControllerModif/release/libQtGameControllerModif.a \
			../../../StaticLibs/SOURCES/RealJoysticks/release/librealJoysticks.a
}

win32-msvc*:CONFIG(debug, debug|release) {
    DESTDIR = debug
	LIBS += ../../../StaticLibs/SOURCES/QtGameControllerModif/debug/QtGameControllerModif.lib \
			../../../StaticLibs/SOURCES/RealJoysticks/debug/realJoysticks.lib
}
win32-msvc*:CONFIG(release, debug|release) {
    DESTDIR = release
	LIBS += ../../../StaticLibs/SOURCES/QtGameControllerModif/release/QtGameControllerModif.lib \
			../../../StaticLibs/SOURCES/RealJoysticks/release/realJoysticks.lib
}

TEMPLATE = lib
TARGET = vJoyDevice
DEPENDPATH += .
INCLUDEPATH += .
CONFIG += plugin c++11
OBJECTS_DIR = $$DESTDIR/objects
MOC_DIR = $$DESTDIR/moc


INCLUDEPATH +=  ../../../StaticLibs/INCLUDES/QtGameControllerModif/ \
				../../../StaticLibs/INCLUDES/Other/ \
				../../../StaticLibs/INCLUDES/RealJoysticks/


HEADERS +=  CODE/RealJoystickFactory.h \
			CODE/vJoyDevice.h

SOURCES +=  CODE/vJoyDevice.cpp

