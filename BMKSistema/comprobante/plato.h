#ifndef PLATO_H
#define PLATO_H

#include "mylibrary/mylibrary_global.h"

namespace Ui {
class Plato;
}

class MYLIBRARYSHARED_EXPORT Plato : public QWidget
{
    Q_OBJECT

public:
    explicit Plato(QWidget *parent = 0);
    ~Plato();

    void set_familia(QString familia);

    void ingresar();

    void modificar();

    void eliminar();

private:
    void select_all();

signals:
    void closing();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_comboBox_buscar_activated(const QString &arg1);

    void on_spinBox_x_valueChanged(int arg1);

    void on_spinBox_y_valueChanged(int arg1);

    void on_pushButton_eliminar_clicked();

    void on_pushButton_nuevo_clicked();

    void on_pushButton_modificar_clicked();

    void on_pushButton_guardar_clicked();

    void on_pushButton_salir_clicked();    

private:
    Ui::Plato *ui;

    QString familia;
};

#endif // PLATO_H
