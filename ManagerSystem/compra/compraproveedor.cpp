#include "compraproveedor.h"

CompraProveedor::CompraProveedor(QWidget *parent) :
    PersonaBuscar(parent)
{

}

CompraProveedor::~CompraProveedor()
{

}

void CompraProveedor::showEvent(QShowEvent *se)
{
    se->accept();
    PersonaBuscar::showEvent(se);
}
void CompraProveedor::closeEvent(QCloseEvent *event)
{
    event->accept();
    emit closing();
    PersonaBuscar::closeEvent(event);
}
