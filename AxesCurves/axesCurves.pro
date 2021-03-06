win32-g++:CONFIG(debug, debug|release) {
	DESTDIR = debug
	LIBS += ../StaticLibs/SOURCES/QtGameControllerModif/debug/libQtGameControllerModif.a
	LIBS += ../StaticLibs/SOURCES/RealJoysticks/debug/libRealJoysticks.a
}
win32-g++:CONFIG(release, debug|release) {
	DESTDIR = release
	LIBS += ../StaticLibs/SOURCES/QtGameControllerModif/release/libQtGameControllerModif.a
	LIBS += ../StaticLibs/SOURCES/RealJoysticks/release/libRealJoysticks.a
}

win32-msvc*:CONFIG(debug, debug|release) {
	DESTDIR = debug
	LIBS += ../StaticLibs/SOURCES/QtGameControllerModif/debug/QtGameControllerModif.lib
	LIBS += ../StaticLibs/SOURCES/RealJoysticks/debug/RealJoysticks.lib
}
win32-msvc*:CONFIG(release, debug|release) {
	DESTDIR = release
	LIBS += ../StaticLibs/SOURCES/QtGameControllerModif/release/QtGameControllerModif.lib
	LIBS += ../StaticLibs/SOURCES/RealJoysticks/release/RealJoysticks.lib
}


TEMPLATE = app
TARGET = AxesCurves
CONFIG += c++11
OBJECTS_DIR = $$DESTDIR/objects
MOC_DIR = $$DESTDIR/moc
QT += core gui widgets charts
DEFINES -= QT_NO_DEBUG


INCLUDEPATH +=  ../StaticLibs/INCLUDES/QtGameControllerModif/ \
				../StaticLibs/INCLUDES/Other/ \
				../StaticLibs/INCLUDES/RealJoysticks/


HEADERS +=  CODE/MainWindow.h \
			CODE/CurveChartView.h \
			CODE/CURVES/AbstractAxisCurve.h \
			CODE/CURVES/JCurveGUI.h \
			CODE/CURVES/SCurveGUI.h


SOURCES +=  CODE/main.cpp \
			CODE/MainWindow.cpp \
			CODE/CurveChartView.cpp \
			CODE/CURVES/JCurveGUI.cpp \
			CODE/CURVES/SCurveGUI.cpp


RESOURCES += resources.qrc
RC_FILE += icone.rc

