#ifndef PRODUCTOSERVICIO_H
#define PRODUCTOSERVICIO_H

#include "producto_headers.h"

namespace Ui {
class ProductoServicio;
}

class MYLIBRARYSHARED_EXPORT ProductoServicio : public QWidget
{
    Q_OBJECT

public:
    explicit ProductoServicio(QWidget *parent = 0);
    ~ProductoServicio();

private:
    Ui::ProductoServicio *ui;
};

#endif // PRODUCTOSERVICIO_H
