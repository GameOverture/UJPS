#ifndef UJPSCOREGLOBAL_H
#define UJPSCOREGLOBAL_H

#include <QtCore/qglobal.h>

#if defined(UJPSCORE_LIBRARY)
# define UJPSCORE_EXPORT Q_DECL_EXPORT
#else
# define UJPSCORE_EXPORT Q_DECL_IMPORT
#endif

#endif // UJPSCOREGLOBAL_H
