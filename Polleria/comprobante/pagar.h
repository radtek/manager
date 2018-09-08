#ifndef PAGAR_H
#define PAGAR_H

#include "mylibrary/mylibrary_global.h"
#include "mylibrary/share_static_include.h"

namespace Ui {
class Pagar;
}

class MYLIBRARYSHARED_EXPORT Pagar : public QWidget
{
    Q_OBJECT

public:
    explicit Pagar(QWidget *parent = 0);
    ~Pagar();

    double getTotal();

private slots:
    void on_pushButton_010_clicked();

    void on_pushButton_020_clicked();

    void on_pushButton_050_clicked();

    void on_pushButton_01_clicked();

    void on_pushButton_02_clicked();

    void on_pushButton_05_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_20_clicked();

    void on_pushButton_50_clicked();

    void on_pushButton_100_clicked();

    void on_pushButton_200_clicked();

    void on_pushButton_aceptar_clicked();

    void on_pushButton_salir_clicked();

protected:
    void closeEvent(QCloseEvent *event);

signals:
    void closing();

private:
    Ui::Pagar *ui;
};

#endif // PAGAR_H
