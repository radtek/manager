#ifndef FACTURACION_GLOBAL_H
#define FACTURACION_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(FACTURACION_LIBRARY)
#  define FACTURACIONSHARED_EXPORT Q_DECL_EXPORT
#else
#  define FACTURACIONSHARED_EXPORT Q_DECL_IMPORT
#endif

#include "mylibrary/mylibrary_global.h"

#define DECIMALS_CANTIDAD 4
#define DECIMALS_PRECIO_UNITARIO 4
#define DECIMALS_PRECIO_TOTAL 3

#endif // FACTURACION_GLOBAL_H
