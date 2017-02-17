#ifndef VENTA_GLOBAL_H
#define VENTA_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(VENTA_LIBRARY)
#  define VENTASHARED_EXPORT Q_DECL_EXPORT
#else
#  define VENTASHARED_EXPORT Q_DECL_IMPORT
#endif

namespace venta_items{
    enum items {
        REGISTRO_SIN_DOCUMENTO=0, BOLETA=1, FACTURA=2, NOTA_PEDIDO=3, GUIA_REMISION_REMITENTE=4
        , COTIZACION=5, NOTA_CREDITO=6, NOTA_DEBITO=7
    };
}

#include "mylibrary_global.h"
#include "producto_global.h"
#include "persona_global.h"

#include "a_venta.h"
#include "ventaamarres.h"
#include "ventaboleta.h"
#include "ventabuscar.h"
#include "ventacliente.h"
#include "ventaconfiguracion.h"
#include "ventaconfighoja.h"
#include "ventacotizacion.h"
#include "ventafactura.h"
#include "ventaguiarr.h"
#include "ventamenu.h"
#include "ventanotacredito.h"
#include "ventanotadebito.h"
#include "ventanotapedido.h"
#include "ventaproducto.h"
#include "ventaregistrosindoc.h"
#include "ventasnselector.h"
#include "ventathreadsetproducto.h"

#endif // VENTA_GLOBAL_H
