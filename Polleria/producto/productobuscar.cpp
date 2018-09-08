#include "productobuscar.h"
#include "ui_productobuscar.h"

ProductoBuscar::ProductoBuscar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProductoBuscar)
{
    ui->setupUi(this);
}

ProductoBuscar::~ProductoBuscar()
{
    delete ui;
}
