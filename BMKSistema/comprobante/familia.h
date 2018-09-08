#ifndef FAMILIA_H
#define FAMILIA_H

#include "mylibrary/mylibrary_global.h"
#include "mylibrary/adminpass.h"

namespace Ui {
class Familia;
}

class MYLIBRARYSHARED_EXPORT Familia : public QWidget
{
    Q_OBJECT

public:
    explicit Familia(QWidget *parent = 0);
    ~Familia();

    bool isIngresar() { return is_ingresar; }
    bool isModificar() { return is_modificar; }
    bool isEliminar() { return is_eliminar; }

    QString getFamilia() { return current_familia; }

    void modo_ingresar(int x, int y);

    void modo_modificacion(QString familia, int x, int y);

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
    void on_pushButton_guardar_clicked();

    void on_pushButton_salir_clicked();

    void on_pushButton_modificar_clicked();

    void on_pushButton_eliminar_clicked();    

private:
    Ui::Familia *ui;

    QString current_familia;

    bool is_ingresar;
    bool is_modificar;
    bool is_eliminar;

    int x;
    int y;
};

#endif // FAMILIA_H
