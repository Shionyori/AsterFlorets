#pragma once

#include <QtCore/qglobal.h>

#if defined(ASTERUI_LIBRARY)
#  define ASTERUI_EXPORT Q_DECL_EXPORT
#else
#  define ASTERUI_EXPORT Q_DECL_IMPORT
#endif
