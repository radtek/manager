#ifndef CAMBIARCORREO_H
#define CAMBIARCORREO_H

#include "mylibrary/share_static_include.h"

namespace Ui {
class CambiarCorreo;
}

class CambiarCorreo : public QWidget
{
    Q_OBJECT

public:
    explicit CambiarCorreo(QWidget *parent = 0);
    ~CambiarCorreo();

private slots:
    void on_pushButton_guardar_clicked();

    void on_pushButton_salir_clicked();

private:
    Ui::CambiarCorreo *ui;
};

#endif // CAMBIARCORREO_H
