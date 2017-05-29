#ifndef COMPRAADJACENTES_H
#define COMPRAADJACENTES_H

#include "facturacion_global.h"
#include "compra_headers.h"

namespace Ui {
class CompraAdjacentes;
}

class FACTURACIONSHARED_EXPORT CompraAdjacentes : public QWidget
{
    Q_OBJECT

public:
    explicit CompraAdjacentes(QWidget *parent = 0);
    ~CompraAdjacentes();

    void set_widget_previous(QWidget* widget_previous);

    void set_documento_id(const QString& id);

private slots:
    void on_pushButton_orden_clicked();

    void on_pushButton_guia_clicked();

    void on_pushButton_boleta_clicked();

    void on_pushButton_factura_clicked();

    void on_pushButton_flete_clicked();

    void on_pushButton_ncredito_clicked();

    void on_pushButton_ndebito_clicked();

    void on_pushButton_modificar_clicked();

    void on_pushButton_eliminar_clicked();

    void on_pushButton_salir_clicked();    

protected:
    void showEvent(QShowEvent *se);

    bool eventFilter(QObject *obj, QEvent *e);

private:
    Ui::CompraAdjacentes *ui;

    QSqlQuery query;

    QWidget* widget_previous;

    QString documento_id;

};

#endif // COMPRAADJACENTES_H
