#ifndef VENTACLIENTE_H
#define VENTACLIENTE_H

#include "facturacion_global.h"
#include "venta_headers.h"

#include "colaboradorbuscar.h"

class FACTURACIONSHARED_EXPORT VentaCliente : public ColaboradorBuscar
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
