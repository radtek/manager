#ifndef PERSONA_GLOBAL_H
#define PERSONA_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PERSONA_LIBRARY)
#  define PERSONASHARED_EXPORT Q_DECL_EXPORT
#else
#  define PERSONASHARED_EXPORT Q_DECL_IMPORT
#endif

namespace persona_items{
    enum items {
        PROVEEDOR=0, TRANSPORTISTA=1, CLIENTE_RUC=2, CLIENTE_DNI=3, USUARIO=4, PROPIETARIO=5
    };
}

#include "mylibrary_global.h"

#include "a_persona.h"
#include "clientedni.h"
#include "clienteruc.h"
#include "colaboradorbuscar.h"
#include "colabselection.h"
#include "proveedorop.h"
#include "transportistaop.h"
#include "usuarioop.h"

#endif // PERSONA_GLOBAL_H
