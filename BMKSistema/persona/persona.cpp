#include "persona.h"
#include "ui_persona.h"

Persona::Persona(QWidget *parent) :
    WidgetEvents(parent),
    ui(new Ui::Persona)
{
    ui->setupUi(this);
}

Persona::~Persona()
{
    qDebug()<<"delete persona"<<endl;
    delete ui;
}
