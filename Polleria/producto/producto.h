#ifndef PRODUCTO_H
#define PRODUCTO_H

#include "mylibrary/mylibrary_global.h"

namespace Ui {
class Producto;
}

class Producto : public QWidget
{
    Q_OBJECT

public:
    explicit Producto(QWidget *parent = 0);
    ~Producto();

private:
    Ui::Producto *ui;
};

#endif // PRODUCTO_H
