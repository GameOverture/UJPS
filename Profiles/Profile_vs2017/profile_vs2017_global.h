#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(PROFILE_VS2017_LIB)
#  define PROFILE_VS2017_EXPORT Q_DECL_EXPORT
# else
#  define PROFILE_VS2017_EXPORT Q_DECL_IMPORT
# endif
#else
# define PROFILE_VS2017_EXPORT
#endif
