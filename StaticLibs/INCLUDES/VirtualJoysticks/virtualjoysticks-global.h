#ifndef VIRTUALJOYSTICKSGLOBAL_H
#define VIRTUALJOYSTICKSGLOBAL_H

#include <QtCore/qglobal.h>

#if defined(VIRTUALJOYSTICKS_LIBRARY)
# define VIRTUALJOYSTICKS_EXPORT Q_DECL_EXPORT
#else
# define VIRTUALJOYSTICKS_EXPORT Q_DECL_IMPORT
#endif

#endif // VIRTUALJOYSTICKSGLOBAL_H