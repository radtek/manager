#ifndef VENTATHREADSETPRODUCTO_H
#define VENTATHREADSETPRODUCTO_H

#include "facturacion_global.h"
#include "venta_headers.h"

class FACTURACIONSHARED_EXPORT VentaThreadSetProducto : public QThread
{
public:
    VentaThreadSetProducto();
    ~VentaThreadSetProducto();

    void set_query(QString str_query);
    void set_pos(int pos);
    void set_enable(bool enable);

    QSqlQuery get_query();
    int get_pos();
    bool get_enable();
public:
    QSqlQuery query;
    QString str_query;
    int pos;
    bool enable;
protected:
    void run();
};

#endif // VENTATHREADSETPRODUCTO_H
