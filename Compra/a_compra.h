#ifndef A_COMPRA_H
#define A_COMPRA_H

#include "compra_global.h"

namespace Ui {
class A_Compra;
}

class COMPRASHARED_EXPORT A_Compra : public QWidget
{
    Q_OBJECT

public:
    explicit A_Compra(QWidget *parent = 0);
    ~A_Compra();

    void set_widget_previous(QWidget* w);

private slots:
    void on_pushButton_ingresar_clicked();

    void on_pushButton_buscar_clicked();

    void on_pushButton_configurar_hoja_clicked();

    void on_pushButton_salir_clicked();

protected:
    void showEvent(QShowEvent *se);

    bool eventFilter(QObject *obj, QEvent *e);

private:
    Ui::A_Compra *ui;

    QWidget* widget_previous;

public:
    enum{SERIE_N_DIGITS = 4, NUMERO_N_DIGITS = 8, SIZE_TRANSACTION = 12};

    static void set_serie_numero(QLineEdit* serie, QLineEdit* numero);

    static QVector<QString> cb_estado(){
        return QVector<QString>()<<"Comprado"<<"Regalo";
    }
};

#endif // A_COMPRA_H
