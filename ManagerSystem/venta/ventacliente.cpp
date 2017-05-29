#include "ventacliente.h"

VentaCliente::VentaCliente(QWidget *parent) :
    PersonaBuscar(parent)
{

}

VentaCliente::~VentaCliente()
{

}
void VentaCliente::showEvent(QShowEvent *se)
{
    se->accept();
    PersonaBuscar::showEvent(se);
}
void VentaCliente::closeEvent(QCloseEvent *ce)
{
    ce->accept();
    emit closing();

    PersonaBuscar::closeEvent(ce);
}

