#ifndef COMPRAPROVEEDOR_H
#define COMPRAPROVEEDOR_H

#include "facturacion_global.h"
#include "compra_headers.h"
#include "colaboradorbuscar.h"

class ColaboradorBuscar;

class FACTURACIONSHARED_EXPORT CompraProveedor : public ColaboradorBuscar
{
    Q_OBJECT

public:
    explicit CompraProveedor(QWidget *parent = 0);
    ~CompraProveedor();

private slots:


protected:
    void showEvent(QShowEvent *se);

    void closeEvent(QCloseEvent *event);

signals:
    void closing();

};

#endif // COMPRAPROVEEDOR_H
