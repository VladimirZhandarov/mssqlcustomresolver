#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(UPELIB_LIB)
#  define UPELIB_EXPORT Q_DECL_EXPORT
# else
#  define UPELIB_EXPORT Q_DECL_IMPORT
# endif
#else
# define UPELIB_EXPORT
#endif
