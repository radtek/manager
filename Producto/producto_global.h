#ifndef PRODUCTO_GLOBAL_H
#define PRODUCTO_GLOBAL_H

#include <QtCore/QtGlobal>

#if defined(PRODUCTO_LIBRARY)
#  define PRODUCTOSHARED_EXPORT Q_DECL_EXPORT
#else
#  define PRODUCTOSHARED_EXPORT Q_DECL_IMPORT
#endif

class ProductoBuscar;

#include "mylibrary_global.h"

#include "marca.h"
#include "marcabuscar.h"
#include "productobuscar.h"
#include "productoformtransaction.h"
#include "tipo.h"
#include "tipobuscar.h"
#include "unidad.h"
#include "unidadbuscar.h"

#endif // PRODUCTO_GLOBAL_H
