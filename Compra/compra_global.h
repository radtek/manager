#ifndef COMPRA_GLOBAL_H
#define COMPRA_GLOBAL_H

#define DECIMALS_CANTIDAD 4
#define DECIMALS_PRECIO_UNITARIO 4
#define DECIMALS_PRECIO_TOTAL 3

namespace compra_items{
    enum items {
        ORDEN=0, FACTURA=1, BOLETA=2, GUIA_REMISION_REMITENTE=3, FLETE=4, NOTA_CREDITO=5
        , NOTA_DEBITO=6, SALDO=7, REG_SIN_DOC=8
    };
}

#include "mylibrary_global.h"

#include "compradefines.h"

namespace A_Compra{
    enum{SERIE_N_DIGITS = 4, NUMERO_N_DIGITS = 8, SIZE_TRANSACTION = 12};
    static void set_serie_numero(QLineEdit* serie, QLineEdit* numero) {
        QRegExp regExp_serie(QString()+"[0-9]{"+QString().setNum(SERIE_N_DIGITS)+","+QString().setNum(SERIE_N_DIGITS)+"}");
        QRegExp regExp_numero(QString()+"[0-9]{"+QString().setNum(NUMERO_N_DIGITS)+","+QString().setNum(NUMERO_N_DIGITS)+"}");

        serie->setValidator(new QRegExpValidator(regExp_serie));
        numero->setValidator(new QRegExpValidator(regExp_numero));

    }
}

#include "compraadjacentes.h"
#include "compraamarres.h"
#include "compraboleta.h"
#include "comprabuscar.h"
#include "comprachartcosto.h"
#include "comprafactura.h"
#include "compraflete.h"
#include "compraguiarr.h"
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
