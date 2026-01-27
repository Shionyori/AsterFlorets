#pragma once

#include <QtCore/qglobal.h>

#if defined(AsterFlorets_LIBRARY)
#  define AsterFlorets_EXPORT Q_DECL_EXPORT
#else
#  define AsterFlorets_EXPORT Q_DECL_IMPORT
#endif
