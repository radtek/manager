#include "ventathreadsetproducto.h"

VentaThreadSetProducto::VentaThreadSetProducto()
{
    enable = false;
}
VentaThreadSetProducto::~VentaThreadSetProducto()
{
    qDebug()<<"delete thread"<<endl;
}

void VentaThreadSetProducto::set_query(QString str_query)
{
    this->str_query = str_query;
}
void VentaThreadSetProducto::set_pos(int pos)
{
    this->pos = pos;
}
void VentaThreadSetProducto::set_enable(bool enable)
{
    this->enable = enable;
}
void VentaThreadSetProducto::run()
{
    qDebug()<<str_query<<endl;
    if(query.exec(str_query)) {
        qDebug()<<"query exec success"<<endl;
    }else{
        qDebug()<<"query no exec"<<endl;
    }
}

QSqlQuery VentaThreadSetProducto::get_query()
{
    return query;
}
int VentaThreadSetProducto::get_pos()
{
    return pos;
}
bool VentaThreadSetProducto::get_enable()
{
    return enable;
}

