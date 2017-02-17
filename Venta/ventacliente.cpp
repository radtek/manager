#include "ventacliente.h"

VentaCliente::VentaCliente(QWidget *parent) :
    ColaboradorBuscar(parent)
{

}

VentaCliente::~VentaCliente()
{

}
void VentaCliente::showEvent(QShowEvent *se)
{
    se->accept();
    ColaboradorBuscar::showEvent(se);
}
void VentaCliente::closeEvent(QCloseEvent *ce)
{
    ce->accept();
    emit closing();

    ColaboradorBuscar::closeEvent(ce);
}

