#include "ventaamarres.h"
#include "ui_ventaamarres.h"

VentaAmarres::VentaAmarres(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VentaAmarres)
{
    ui->setupUi(this);

    widget_previous = 0;
}

VentaAmarres::~VentaAmarres()
{
    qDebug()<<"delete venta amarres"<<endl;
    delete ui;
}

void VentaAmarres::set_widget_previous(QWidget *widget_previous)
{
    this->widget_previous = widget_previous;
}

void VentaAmarres::set_documento_id(QString id)
{
    documento_id = id;
}
