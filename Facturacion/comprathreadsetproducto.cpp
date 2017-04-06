#include "comprathreadsetproducto.h"

CompraThreadSetProducto::CompraThreadSetProducto()
{
    enable = false;
}
CompraThreadSetProducto::~CompraThreadSetProducto()
{
    qDebug()<<"delete thread"<<endl;
}

void CompraThreadSetProducto::set_query(QString str_query)
{
    this->str_query = str_query;
}
void CompraThreadSetProducto::set_pos(int pos)
{
    this->pos = pos;
}
void CompraThreadSetProducto::set_enable(bool enable)
{
    this->enable = enable;
}
void CompraThreadSetProducto::run()
{
    qDebug()<<str_query<<endl;
    if(query.exec(str_query)) {
        qDebug()<<"query exec success"<<endl;
    }else{
        qDebug()<<"query no exec"<<endl;
    }
}

QSqlQuery CompraThreadSetProducto::get_query()
{
    return query;
}
int CompraThreadSetProducto::get_pos()
{
    return pos;
}
bool CompraThreadSetProducto::get_enable()
{
    return enable;
}
