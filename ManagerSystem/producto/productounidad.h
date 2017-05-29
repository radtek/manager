#ifndef PRODUCTOUNIDAD_H
#define PRODUCTOUNIDAD_H

#include "facturacion_global.h"
#include "producto_headers.h"

namespace Ui {
class ProductoUnidad;
}

class FACTURACIONSHARED_EXPORT ProductoUnidad : public QWidget
{
    Q_OBJECT

public:
    explicit ProductoUnidad(QWidget *parent = 0);
    ~ProductoUnidad();

    QString getID();
    QString getUnidad();

    void set_widget_previous(QWidget* widget_previous);

    int getOp(){ return op; }

    void set_data(QString id, QString unidad);

private:
    bool guardar();

    bool remove();

private slots:
    void on_pushButton_guardar_clicked();

    void on_pushButton_eliminar_clicked();

    void on_pushButton_salir_clicked();

protected:
    void showEvent(QShowEvent *se);

    void closeEvent(QCloseEvent *ce);

    bool eventFilter(QObject *obj, QEvent *e);

signals:
    void closing();

private:
    Ui::ProductoUnidad *ui;

    bool afterShow;

    QWidget* widget_previous;

    int op;

    QString id;
};

#endif // PRODUCTOUNIDAD_H
