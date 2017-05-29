#ifndef VENTAMENU_H
#define VENTAMENU_H

#include "facturacion_global.h"
#include "venta_headers.h"

namespace Ui {
class VentaMenu;
}

class FACTURACIONSHARED_EXPORT VentaMenu : public QWidget
{
    Q_OBJECT

public:
    explicit VentaMenu(QWidget *parent = 0);
    ~VentaMenu();

    void set_widget_previous(QWidget *widget_previous);

    void set_modo(int modo);

private slots:
    void on_listWidget_currentRowChanged(int currentRow);

    void on_pushButton_ok_clicked();

    void on_pushButton_salir_clicked();

protected:
    void showEvent(QShowEvent *event);

    void closeEvent(QCloseEvent *event);

    bool eventFilter(QObject *watched, QEvent *event);

signals:
    void closing();

private:
    Ui::VentaMenu *ui;

    QWidget* widget_previous;    

    int modo;

    int tipo;
};

#endif // VENTAMENU_H
