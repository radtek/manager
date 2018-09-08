#ifndef CAMBIARPASS_H
#define CAMBIARPASS_H

#include "mylibrary/share_static_include.h"
namespace Ui {
class CambiarPass;
}

class CambiarPass : public QWidget
{
    Q_OBJECT

public:
    explicit CambiarPass(QWidget *parent = 0);
    ~CambiarPass();

private slots:
    void on_pushButton_guardar_clicked();

    void on_pushButton_salir_clicked();

private:
    Ui::CambiarPass *ui;
};

#endif // CAMBIARPASS_H
