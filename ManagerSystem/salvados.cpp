#include "salvados.h"
#include "ui_salvados.h"

Salvados::Salvados(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Salvados)
{
    ui->setupUi(this);
}

Salvados::~Salvados()
{
    delete ui;
}
