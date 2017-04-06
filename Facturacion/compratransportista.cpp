#include "compratransportista.h"

CompraTransportista::CompraTransportista(QWidget *parent) :
    ColaboradorBuscar(parent)
{

}

CompraTransportista::~CompraTransportista()
{

}

void CompraTransportista::showEvent(QShowEvent *se)
{
    se->accept();
    ColaboradorBuscar::showEvent(se);
}
void CompraTransportista::closeEvent(QCloseEvent *event)
{
    event->accept();
    emit closing();
    ColaboradorBuscar::closeEvent(event);
}
