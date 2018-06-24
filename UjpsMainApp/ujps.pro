#CONFIG(debug, debug|release) {
#	DESTDIR = debug
#	LIBS += ../StaticLibs/SOURCES/QtGameControllerModif/debug/libQtGameControllerModif.a \
#			../StaticLibs/SOURCES/VirtualJoysticks/debug/libvirtualJoysticks.a \
#			../StaticLibs/SOURCES/UjpsCore/debug/libujpsCore.a
#}
#CONFIG(release, debug|release) {
#	DESTDIR = release
#	LIBS += ../StaticLibs/SOURCES/QtGameControllerModif/release/libQtGameControllerModif.a \
#			../StaticLibs/SOURCES/VirtualJoysticks/release/libvirtualJoysticks.a \
#			../StaticLibs/SOURCES/UjpsCore/release/libujpsCore.a
#}

CONFIG(debug, debug|release) {
        DESTDIR = debug
        LIBS += ../StaticLibs/SOURCES/QtGameControllerModif/vs2017/QtGameControllerModif.lib \
                        ../StaticLibs/SOURCES/VirtualJoysticks/vs2017/virtualJoysticks.lib \
                        ../StaticLibs/SOURCES/UjpsCore/vs2017/ujpsCore.lib
}
CONFIG(release, debug|release) {
        DESTDIR = release
        LIBS += ../StaticLibs/SOURCES/QtGameControllerModif/vs2017/QtGameControllerModif.lib \
                        ../StaticLibs/SOURCES/VirtualJoysticks/vs2017/virtualJoysticks.lib \
                        ../StaticLibs/SOURCES/UjpsCore/vs2017/ujpsCore.lib
}


TEMPLATE = app
TARGET = UJPS
CONFIG += c++11
OBJECTS_DIR = $$DESTDIR/objects
MOC_DIR = $$DESTDIR/moc
QT = core xmlpatterns gui widgets
DEFINES -= QT_NO_DEBUG


LIBS += ../vJoy218SDK-291116/SDK/lib/vJoyInterface.lib
LIBS += ../vJoy218SDK-291116/SDK/lib/amd64/vJoyInterface.lib

INCLUDEPATH +=  ../StaticLibs/INCLUDES/QtGameControllerModif/ \
				../StaticLibs/INCLUDES/Other/ \
				../StaticLibs/INCLUDES/VirtualJoysticks/ \
				../StaticLibs/INCLUDES/RealJoysticks/ \
				../StaticLibs/INCLUDES/UjpsCore/


HEADERS +=  CODE/ProfileEngine.h \
			CODE/HMI/MainWindow.h \
			CODE/HMI/TextEdit.h \
			CODE/COMPILER/AbstractCompiler.h \
			CODE/COMPILER/QtCompiler.h \
			CODE/COMPILER/QtCompilationProcess.h \
			CODE/COMPILER/CompilationWidget.h \
			CODE/SETTINGS/ApplicationSettings.h \
			CODE/SETTINGS/AbstractSettingsWidget.h \
			CODE/SETTINGS/SettingsDialog.h \
			CODE/SETTINGS/MyFileDialog.h \
			CODE/SETTINGS/GeneralSettingsWidget.h \
			CODE/SETTINGS/VJoySettingsWidget.h \
			CODE/XML/AbstractXmlInfo.h \
			CODE/XML/XmlValidationMessageHandler.h \
			CODE/XML/GenericPropertiesInfo.h
			
			
SOURCES +=  CODE/main.cpp \
			CODE/ProfileEngine.cpp \
			CODE/otherFunctions.cpp \
			CODE/HMI/MainWindow.cpp \
			CODE/HMI/TextEdit.cpp \
			CODE/COMPILER/QtCompiler.cpp \
			CODE/COMPILER/QtCompilationProcess.cpp \
			CODE/COMPILER/CompilationWidget.cpp \
			CODE/SETTINGS/ApplicationSettings.cpp \
			CODE/SETTINGS/SettingsDialog.cpp \
			CODE/SETTINGS/MyFileDialog.cpp \
			CODE/SETTINGS/GeneralSettingsWidget.cpp \
			CODE/SETTINGS/VJoySettingsWidget.cpp \
			CODE/XML/AbstractXmlInfo.cpp \
			CODE/XML/XmlValidationMessageHandler.cpp \
			CODE/XML/GenericPropertiesInfo.cpp


RESOURCES += resources.qrc
RC_FILE += icone.rc

