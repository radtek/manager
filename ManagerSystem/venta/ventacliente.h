#ifndef VENTACLIENTE_H
#define VENTACLIENTE_H

#include "facturacion_global.h"
#include "venta/venta_headers.h"

#include "persona/personabuscar.h"

class FACTURACIONSHARED_EXPORT VentaCliente : public PersonaBuscar
{
    Q_OBJECT

public:
    explicit VentaCliente(QWidget *parent = 0);
    ~VentaCliente();

protected:
    void showEvent(QShowEvent *se);

    void closeEvent(QCloseEvent *ce);

signals:
    void closing();

private:

};

#endif // VENTACLIENTE_H
