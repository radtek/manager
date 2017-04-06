#ifndef PERSONA_HEADERS_H
#define PERSONA_HEADERS_H

namespace persona_items{
    enum items {
        PROVEEDOR=0, TRANSPORTISTA=1, CLIENTE_RUC=2, CLIENTE_DNI=3, USUARIO=4, PROPIETARIO=5
    };
}

#include "clientedni.h"
#include "clienteruc.h"
#include "colaboradorbuscar.h"
#include "proveedorop.h"
#include "transportistaop.h"
#include "usuarioop.h"

#endif // PERSONA_HEADERS_H
