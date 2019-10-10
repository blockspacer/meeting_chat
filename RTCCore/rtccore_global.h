#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(RTCCORE_LIB)
#  define RTCCORE_EXPORT Q_DECL_EXPORT
# else
#  define RTCCORE_EXPORT Q_DECL_IMPORT
# endif
#else
# define RTCCORE_EXPORT
#endif
