#ifndef PRODUCTO_H
#define PRODUCTO_H

#include "facturacion_global.h"
#include "producto_headers.h"

namespace Ui {
class Producto;
}

class FACTURACIONSHARED_EXPORT Producto : public QWidget
{
    Q_OBJECT

public:
    explicit Producto(QWidget *parent = 0);
    ~Producto();

    QString get_ID();
    QString get_IDTipo();
    QString get_IDMarca();
    QString get_IDUnidad();
    QString get_codigo();
    QString get_tipo();
    QString get_descripcion();
    QString get_marca();
    QString get_unidad();

    void set_widget_previous(QWidget* w);

    int getOp(){ return op; }

    void set_data(QString id, QString id_tipo, QString id_marca, QString id_unidad
        , QString codigo, QString tipo, QString descripcion, QString marca, QString unidad);

private:
    bool guardar();

    bool remove();

private slots:
    void on_pushButton_buscar_tipo_clicked();

    void on_tipoBuscar_closing();

    void on_pushButton_buscar_marca_clicked();

    void on_marcaBuscar_closing();

    void on_pushButton_buscar_unidad_clicked();

    void on_unidadBuscar_closing();

    void on_pushButton_limpiar_tipo_clicked();

    void on_pushButton_limpiar_marca_clicked();

    void on_pushButton_limpiar_unidad_clicked();

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
    Ui::Producto *ui;

    bool afterShow;

    QWidget* widget_previous;

    int op;

    QString id_tipo;
    QString id_marca;
    QString id_unidad;
    QString id;
};

#endif // PRODUCTO_H
