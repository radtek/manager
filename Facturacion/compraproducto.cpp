#include "compraproducto.h"

CompraProducto::CompraProducto(QWidget *parent) :
    ProductoBuscar(parent)
{

}

CompraProducto::~CompraProducto()
{

}

void CompraProducto::showEvent(QShowEvent *se)
{
    se->accept();
    ProductoBuscar::showEvent(se);
}
void CompraProducto::closeEvent(QCloseEvent *ce)
{
    ce->accept();
    emit closing();
    ProductoBuscar::closeEvent(ce);
}
