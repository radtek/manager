#ifndef QTXLSXWRITER_GLOBAL_H
#define QTXLSXWRITER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QTXLSXWRITER_LIBRARY)
#  define QTXLSXWRITERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define QTXLSXWRITERSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // QTXLSXWRITER_GLOBAL_H
