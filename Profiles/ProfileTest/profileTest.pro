win32-g++:CONFIG(debug, debug|release) {
	DESTDIR = debug
	LIBS += ../../StaticLibs/SOURCES/QtGameControllerModif/debug/libQtGameControllerModif.a \
			../../StaticLibs/SOURCES/RealJoysticks/debug/librealJoysticks.a \
			../../StaticLibs/SOURCES/VirtualJoysticks/debug/libvirtualJoysticks.a \
			../../StaticLibs/SOURCES/UjpsCore/debug/libujpsCore.a \
			../../vJoy218SDK-291116/SDK/lib/vJoyInterface.lib
}
win32-g++:CONFIG(release, debug|release) {
	DESTDIR = release
	LIBS += ../../StaticLibs/SOURCES/QtGameControllerModif/release/libQtGameControllerModif.a \
			../../StaticLibs/SOURCES/RealJoysticks/release/librealJoysticks.a \
			../../StaticLibs/SOURCES/VirtualJoysticks/release/libvirtualJoysticks.a \
			../../StaticLibs/SOURCES/UjpsCore/release/libujpsCore.a \
			../../vJoy218SDK-291116/SDK/lib/vJoyInterface.lib
}

win32-msvc*:CONFIG(debug, debug|release) {
	DESTDIR = debug
	LIBS += ../../StaticLibs/SOURCES/QtGameControllerModif/debug/QtGameControllerModif.lib \
			../../StaticLibs/SOURCES/RealJoysticks/debug/realJoysticks.lib \
			../../StaticLibs/SOURCES/VirtualJoysticks/debug/virtualJoysticks.lib \
			../../StaticLibs/SOURCES/UjpsCore/debug/ujpsCore.lib \
			../../vJoy218SDK-291116/SDK/lib/amd64/vJoyInterface.lib
}
win32-msvc*:CONFIG(release, debug|release) {
	DESTDIR = release
	LIBS += ../../StaticLibs/SOURCES/QtGameControllerModif/release/QtGameControllerModif.lib \
			../../StaticLibs/SOURCES/RealJoysticks/release/realJoysticks.lib \
			../../StaticLibs/SOURCES/VirtualJoysticks/release/virtualJoysticks.lib \
			../../StaticLibs/SOURCES/UjpsCore/release/ujpsCore.lib \
			../../vJoy218SDK-291116/SDK/lib/amd64/vJoyInterface.lib
}

TEMPLATE = lib
TARGET = ProfileTest
DEPENDPATH += .
INCLUDEPATH += .
CONFIG += plugin c++11
OBJECTS_DIR = $$DESTDIR/objects
MOC_DIR = $$DESTDIR/moc


INCLUDEPATH +=  ../../StaticLibs/INCLUDES/QtGameControllerModif/ \
				../../StaticLibs/INCLUDES/Other/ \
				../../StaticLibs/INCLUDES/VirtualJoysticks/ \
				../../StaticLibs/INCLUDES/RealJoysticks/ \
				../../StaticLibs/INCLUDES/UjpsCore/ \
				../../ControllersPlugins/INCLUDES/

HEADERS +=  CODE/Profile.h
SOURCES +=  CODE/Profile.cpp

