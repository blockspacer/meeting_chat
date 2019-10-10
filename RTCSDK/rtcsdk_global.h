#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(RTCSDK_LIB)
#  define RTCSDK_EXPORT Q_DECL_EXPORT
# else
#  define RTCSDK_EXPORT Q_DECL_IMPORT
# endif
#else
# define RTCSDK_EXPORT
#endif
