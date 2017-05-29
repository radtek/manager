#ifndef PERSONA_HEADERS_H
#define PERSONA_HEADERS_H

namespace persona_items{
    enum items {
        PROVEEDOR=0, TRANSPORTISTA=1, CLIENTE_RUC=2, CLIENTE_DNI=3, USUARIO=4, PROPIETARIO=5
    };
}

#include "personaclientedni.h"
#include "personaclienteruc.h"
#include "personabuscar.h"
#include "personaproveedor.h"
#include "personatransportista.h"
#include "personausuario.h"

#endif // PERSONA_HEADERS_H
