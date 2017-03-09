#ifndef VENTACLIENTE_H
#define VENTACLIENTE_H

#include "venta_global.h"

#include "colaboradorbuscar.h"

class VENTASHARED_EXPORT VentaCliente : public ColaboradorBuscar
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
