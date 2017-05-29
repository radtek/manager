#ifndef VENTASNSELECTOR_H
#define VENTASNSELECTOR_H

#include "facturacion_global.h"
#include "venta_headers.h"

namespace Ui {
class VentaSNSelector;
}

class FACTURACIONSHARED_EXPORT VentaSNSelector : public QWidget
{
    Q_OBJECT

public:
    explicit VentaSNSelector(QWidget *parent = 0);
    ~VentaSNSelector();

    void set_widget_previous(QWidget* widget_previous);

    void set_tipo(int tipo);

    QString getSerie();
    QString getNumero();

private slots:
    void on_pushButton_ok_clicked();

    void on_pushButton_ultimo_clicked();

    void on_pushButton_salir_clicked();

protected:
    void showEvent(QShowEvent *event);

    void closeEvent(QCloseEvent *event);

    bool eventFilter(QObject *watched, QEvent *event);

signals:
    void closing();

private:
    Ui::VentaSNSelector *ui;

    QWidget* widget_previous;

    QString serie;
    QString numero;

    int tipo;

    int pos;
    int size_query;
};

#endif // VENTASNSELECTOR_H
