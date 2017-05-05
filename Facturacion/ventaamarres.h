#ifndef VENTAAMARRES_H
#define VENTAAMARRES_H

#include "facturacion_global.h"
#include "venta_headers.h"

namespace Ui {
class VentaAmarres;
}

class FACTURACIONSHARED_EXPORT VentaAmarres : public QWidget
{
    Q_OBJECT

public:
    explicit VentaAmarres(QWidget *parent = 0);
    ~VentaAmarres();

    void set_widget_previous(QWidget* widget_previous);

    void set_documento(QString id, int tipo);

private slots:
    void on_compra_closing();

    void on_pushButton_editar_clicked();

private:
    void editarItem(QTableWidgetItem* item);

private slots:
    void on_tableWidget_itemDoubleClicked(QTableWidgetItem *item);

    void on_pushButton_salir_clicked();

protected:
    void showEvent(QShowEvent *se);

    void closeEvent(QCloseEvent *ce);

    bool eventFilter(QObject *obj, QEvent *e);

signals:
    void closing();

private:
    Ui::VentaAmarres *ui;

    bool afterShow;

    QWidget* widget_previous;

    int tipo;

    QString id;
};

#endif // VENTAAMARRES_H
