win32-g++:CONFIG(debug, debug|release) {
	DESTDIR = debug
	LIBS += ../RealJoysticks/debug/librealJoysticks.a
	LIBS += ../VirtualJoysticks/debug/libvirtualJoysticks.a
	LIBS += ../../../vJoy218SDK-291116/SDK/lib/vJoyInterface.lib
}
win32-g++:CONFIG(release, debug|release) {
	DESTDIR = release
	LIBS += ../RealJoysticks/release/librealJoysticks.a
	LIBS += ../VirtualJoysticks/release/libvirtualJoysticks.a
	LIBS += ../../../vJoy218SDK-291116/SDK/lib/vJoyInterface.lib
}

win32-msvc*:CONFIG(debug, debug|release) {
    DESTDIR = debug
    LIBS += ../RealJoysticks/debug/realJoysticks.lib
    LIBS += ../VirtualJoysticks/debug/virtualJoysticks.lib
	LIBS += ../../../vJoy218SDK-291116/SDK/lib/amd64/vJoyInterface.lib
}
win32-msvc*:CONFIG(release, debug|release) {
    DESTDIR = release
    LIBS += ../RealJoysticks/release/realJoysticks.lib
    LIBS += ../VirtualJoysticks/release/virtualJoysticks.lib
	LIBS += ../../../vJoy218SDK-291116/SDK/lib/amd64/vJoyInterface.lib
}


TEMPLATE = lib
TARGET = ujpsCore
OBJECTS_DIR = $$DESTDIR/objects
MOC_DIR = $$DESTDIR/moc
CONFIG += c++11

DEFINES += UJPSCORE_LIBRARY

win32-msvc*:CONFIG(release, debug|release): LIBS += -luser32
else:win32-msvc*:CONFIG(debug, debug|release): LIBS += -luser32

INCLUDEPATH +=  ../../INCLUDES/Other/ \
				../../INCLUDES/VirtualJoysticks/ \
				../../INCLUDES/RealJoysticks/ \
				../../INCLUDES/UjpsCore/

HEADERS +=  ../../INCLUDES/UjpsCore/AbstractProfile.h \
			../../INCLUDES/UjpsCore/VirtualEventsQueue.h \
			../../INCLUDES/UjpsCore/LAYERS/LayersCombo.h \
			../../INCLUDES/UjpsCore/LAYERS/LayerCalculator.h \
			../../INCLUDES/UjpsCore/MAPPINGS/AbstractMapping.h \
			../../INCLUDES/UjpsCore/MAPPINGS/MappingStandard.h \
			../../INCLUDES/UjpsCore/MAPPINGS/MappingTempo.h \
			../../INCLUDES/UjpsCore/MAPPINGS/MappingAxisRelative.h \
			../../INCLUDES/UjpsCore/MAPPINGS/MappingAxis1.h \
			../../INCLUDES/UjpsCore/MAPPINGS/MappingAxis2.h \
    ../../INCLUDES/UjpsCore/ujpscore-global.h


SOURCES +=  CODE/AbstractProfile.cpp \
			CODE/VirtualEventsQueue.cpp \
			CODE/LAYERS/LayersCombo.cpp \
			CODE/LAYERS/LayerCalculator.cpp \
			CODE/MAPPINGS/MappingStandard.cpp \
			CODE/MAPPINGS/MappingTempo.cpp \
			CODE/MAPPINGS/MappingAxisRelative.cpp \
			CODE/MAPPINGS/MappingAxis1.cpp \
			CODE/MAPPINGS/MappingAxis2.cpp \
			CODE/MAPPINGS/MappingMergeAxes.cpp

