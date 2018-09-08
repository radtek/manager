#ifndef PRODUCTOBUSCAR_H
#define PRODUCTOBUSCAR_H

#include <QWidget>

namespace Ui {
class ProductoBuscar;
}

class ProductoBuscar : public QWidget
{
    Q_OBJECT

public:
    explicit ProductoBuscar(QWidget *parent = 0);
    ~ProductoBuscar();

private:
    Ui::ProductoBuscar *ui;
};

#endif // PRODUCTOBUSCAR_H
