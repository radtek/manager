#ifndef VENTADEFINES_H
#define VENTADEFINES_H

#include <QtCore/qglobal.h>

#if defined(VENTA_LIBRARY)
#  define VENTASHARED_EXPORT Q_DECL_EXPORT
#else
#  define VENTASHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // VENTADEFINES_H
