#include "compratransportista.h"

CompraTransportista::CompraTransportista(QWidget *parent) :
    PersonaBuscar(parent)
{

}

CompraTransportista::~CompraTransportista()
{

}

void CompraTransportista::showEvent(QShowEvent *se)
{
    se->accept();
    PersonaBuscar::showEvent(se);
}
void CompraTransportista::closeEvent(QCloseEvent *event)
{
    event->accept();
    emit closing();
    PersonaBuscar::closeEvent(event);
}
