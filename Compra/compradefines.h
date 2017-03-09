#ifndef COMPRADEFINES_H
#define COMPRADEFINES_H

#include <QtCore/qglobal.h>

#if defined(COMPRA_LIBRARY)
#  define COMPRASHARED_EXPORT Q_DECL_EXPORT
#else
#  define COMPRASHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // COMPRADEFINES_H
