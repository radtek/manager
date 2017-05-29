#ifndef COMPRAPRODUCTO_H
#define COMPRAPRODUCTO_H

#include "facturacion_global.h"
#include "compra/compra_headers.h"

#include "producto/productobuscar.h"

class FACTURACIONSHARED_EXPORT CompraProducto : public ProductoBuscar
{
    Q_OBJECT

public:
    explicit CompraProducto(QWidget *parent = 0);
    ~CompraProducto();

private slots:
    //void on_le_buscar_clicked();

    //void on_tb_clicked();

protected:
    void showEvent(QShowEvent *se);

    void closeEvent(QCloseEvent *ce);

signals:
    void closing();

};

#endif // COMPRAPRODUCTO_H
