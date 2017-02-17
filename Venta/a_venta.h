#ifndef A_VENTA_H
#define A_VENTA_H

#include "venta_global.h"

namespace Ui {
class A_Venta;
}

class VENTASHARED_EXPORT A_Venta : public QWidget
{
    Q_OBJECT

public:
    explicit A_Venta(QWidget *parent = 0);
    ~A_Venta();

    enum{SERIE_N_DIGITS = 4, NUMERO_N_DIGITS = 8, SIZE_TRANSACTION = 12};

    static void set_serie_numero(QLineEdit* serie, QLineEdit* numero);

    void set_widget_previous(QWidget* w);

private slots:
    void on_pushButton_ingresar_clicked();

    void on_pushButton_buscar_clicked();

    void on_pushButton_configurar_clicked();

    void on_pushButton_salir_clicked();

protected:
    void showEvent(QShowEvent *se);

    bool eventFilter(QObject *obj, QEvent *e);

private:
    Ui::A_Venta *ui;

    QWidget* widget_previous;
};

#endif // A_VENTA_H
