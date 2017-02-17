#ifndef COMPRA_GLOBAL_H
#define COMPRA_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(COMPRA_LIBRARY)
#  define COMPRASHARED_EXPORT Q_DECL_EXPORT
#else
#  define COMPRASHARED_EXPORT Q_DECL_IMPORT
#endif

#define DECIMALS_CANTIDAD 4
#define DECIMALS_PRECIO_UNITARIO 4
#define DECIMALS_PRECIO_TOTAL 3

namespace compra_items{
    enum items {
        ORDEN=0, FACTURA=1, BOLETA=2, GUIA_REMISION_REMITENTE=3, FLETE=4, NOTA_CREDITO=5
        , NOTA_DEBITO=6, SALDO=7
    };
}

#include "mylibrary_global.h"
#include "producto_global.h"
#include "persona_global.h"

#include "a_compra.h"
#include "compraadjacentes.h"
#include "compraamarres.h"
#include "compraboleta.h"
#include "comprabuscar.h"
#include "comprachartcosto.h"
#include "comprafactura.h"
#include "compraflete.h"
#include "compraguiarr.h"
#include "compramenu.h"
#include "compranotacredito.h"
#include "compranotadebito.h"
#include "compraorden.h"
#include "compraproducto.h"
#include "compraproductocombobox.h"
#include "compraproductodelegate.h"
#include "compraproveedor.h"
#include "comprasaldo.h"
#include "comprathreadsetproducto.h"
#include "compratransportista.h"

#endif // COMPRA_GLOBAL_H
