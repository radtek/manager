#ifndef PRODUCTOITEM_H
#define PRODUCTOITEM_H

#include "producto_global.h"

namespace Ui {
class ProductoItem;
}

class PRODUCTOSHARED_EXPORT ProductoItem : public QWidget
{
	Q_OBJECT
public:
    explicit ProductoItem(QWidget *parent = 0);
    ~ProductoItem();

    void set_widget_previous(QWidget* widget_previous);

private slots:
    void on_pushButton_ing_tipo_clicked();

    void on_pushButton_ing_marca_clicked();

    void on_pushButton_ing_unidad_clicked();

    void on_pushButton_bus_tipo_clicked();

    void on_pushButton_bus_marca_clicked();

    void on_pushButton_bus_unidad_clicked();

    void on_pushButton_salir_clicked();

protected:
    void showEvent(QShowEvent *se);

    bool eventFilter(QObject *obj, QEvent *e);

private:
    Ui::ProductoItem *ui;

    QWidget* widget_previous;

    int tipo;
    int transaction;
};

#endif // PRODUCTOITEM_H
