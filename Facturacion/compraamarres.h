#ifndef COMPRAAMARRES_H
#define COMPRAAMARRES_H

#include "facturacion_global.h"
#include "compra_headers.h"

namespace Ui {
class CompraAmarres;
}

class FACTURACIONSHARED_EXPORT CompraAmarres : public QWidget
{
    Q_OBJECT

public:
    explicit CompraAmarres(QWidget *parent = 0);
    ~CompraAmarres();

    void set_widget_previous(QWidget* widget_previous);

    void set_documento(QString id, int tipo);

private slots:
    void on_compra_closing();

    void on_pushButton_editar_clicked();

    void on_tableWidget_itemDoubleClicked(QTableWidgetItem *item);

    void on_pushButton_salir_clicked();        

protected:
    void showEvent(QShowEvent *se);

    void closeEvent(QCloseEvent *ce);

    bool eventFilter(QObject *obj, QEvent *e);

signals:
    void closing();

private:
    Ui::CompraAmarres *ui;

    bool afterShow;

    QWidget* widget_previous;

    int tipo;

    QString id;
};

#endif // COMPRAAMARRES_H
