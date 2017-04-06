#include "compraproveedor.h"

CompraProveedor::CompraProveedor(QWidget *parent) :
    ColaboradorBuscar(parent)
{

}

CompraProveedor::~CompraProveedor()
{

}

void CompraProveedor::showEvent(QShowEvent *se)
{
    se->accept();
    ColaboradorBuscar::showEvent(se);
}
void CompraProveedor::closeEvent(QCloseEvent *event)
{
    event->accept();
    emit closing();
    ColaboradorBuscar::closeEvent(event);
}
