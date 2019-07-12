#ifndef QTEP_H
#define QTEP_H

#include <QtCore>

#if defined(QTEP_COMPILE_LIBRARY)
#  define QTEP_LIB_DECL Q_DECL_EXPORT
#elif defined(QTEP_USE_LIBRARY)
#  define QTEP_LIB_DECL Q_DECL_IMPORT
#else
#  define QTEP_LIB_DECL
#endif
#endif // QTEP_H
