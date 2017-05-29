#ifndef VENTA_HEADERS_H
#define VENTA_HEADERS_H

namespace venta_items{
    enum items {
        REGISTRO_SIN_DOCUMENTO=0, BOLETA=1, FACTURA=2, NOTA_PEDIDO=3, GUIA_REMISION_REMITENTE=4
        , COTIZACION=5, NOTA_CREDITO=6, NOTA_DEBITO=7
    };
}

namespace A_Venta{
    enum{SERIE_N_DIGITS = 4, NUMERO_N_DIGITS = 8, SIZE_TRANSACTION = 12};
    static void set_serie_numero(QLineEdit* serie, QLineEdit* numero) {
        QRegExp regExp_serie(QString()+"[0-9]{"+QString().setNum(SERIE_N_DIGITS)+","+QString().setNum(SERIE_N_DIGITS)+"}");
        QRegExp regExp_numero(QString()+"[0-9]{"+QString().setNum(NUMERO_N_DIGITS)+","+QString().setNum(NUMERO_N_DIGITS)+"}");

        serie->setValidator(new QRegExpValidator(regExp_serie));
        numero->setValidator(new QRegExpValidator(regExp_numero));

    }
}

#include "ventaamarres.h"
#include "ventaboleta.h"
#include "ventabuscar.h"
#include "ventacliente.h"
#include "ventachartprecio.h"
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

#endif // VENTA_HEADERS_H
