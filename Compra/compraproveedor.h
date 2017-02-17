#ifndef COMPRAPROVEEDOR_H
#define COMPRAPROVEEDOR_H

#include "compra_global.h"

class COMPRASHARED_EXPORT CompraProveedor : public ColaboradorBuscar
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
