#include "producto.h"
#include "ui_producto.h"

Producto::Producto(QWidget *parent) :
    WidgetEvents(parent),
    ui(new Ui::Producto)
{
    ui->setupUi(this);
}

Producto::~Producto()
{
    qDebug()<<"delete producto"<<endl;
    delete ui;
}
