#ifndef COMPRATHREADSETPRODUCTO_H
#define COMPRATHREADSETPRODUCTO_H

#include "facturacion_global.h"
#include "compra_headers.h"

class FACTURACIONSHARED_EXPORT CompraThreadSetProducto : public QThread
{
public:
    CompraThreadSetProducto();
    ~CompraThreadSetProducto();

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

#endif // COMPRATHREADSETPRODUCTO_H
