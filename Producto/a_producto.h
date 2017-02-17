#ifndef A_PRODUCTO_H
#define A_PRODUCTO_H

#include "producto_global.h"

namespace Ui {
class A_Producto;
}

class PRODUCTOSHARED_EXPORT A_Producto : public QWidget
{    
	Q_OBJECT
public:
    explicit A_Producto(QWidget *parent = 0);
    ~A_Producto();

    void set_widget_previous(QWidget* widget_previous);

private slots:
    void on_pushButton_guardar_clicked();

    void on_pushButton_buscar_clicked();

    void on_pushButton_salir_clicked();

protected:
    void showEvent(QShowEvent *se);

    bool eventFilter(QObject *obj, QEvent *e);

private:
    Ui::A_Producto *ui;

    QWidget* widget_previous;

};

#endif // A_PRODUCTO_H
