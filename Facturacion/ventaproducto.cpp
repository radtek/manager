#include "ventaproducto.h"

VentaProducto::VentaProducto(QWidget *parent) :
    ProductoBuscar(parent)
{

}

VentaProducto::~VentaProducto()
{

}

void VentaProducto::showEvent(QShowEvent *se)
{
    se->accept();
    ProductoBuscar::showEvent(se);
}
void VentaProducto::closeEvent(QCloseEvent *ce)
{
    ce->accept();
    emit closing();
    ProductoBuscar::closeEvent(ce);
}
