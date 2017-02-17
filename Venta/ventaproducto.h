#ifndef VENTAPRODUCTO_H
#define VENTAPRODUCTO_H

#include "venta_global.h"

class VENTASHARED_EXPORT VentaProducto : public ProductoBuscar
{
    Q_OBJECT

public:
    explicit VentaProducto(QWidget *parent = 0);
    ~VentaProducto();

protected:
    void showEvent(QShowEvent *se);

    void closeEvent(QCloseEvent *ce);

signals:
    void closing();

private:


};

#endif // VENTAPRODUCTO_H
