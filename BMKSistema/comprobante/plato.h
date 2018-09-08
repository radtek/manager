#ifndef PLATO_H
#define PLATO_H

#include "mylibrary/mylibrary_global.h"
#include "mylibrary/adminpass.h"

namespace Ui {
class Plato;
}

class MYLIBRARYSHARED_EXPORT Plato : public QWidget
{
    Q_OBJECT

public:
    explicit Plato(QWidget *parent = 0);
    ~Plato();

    bool isIngresar() { return is_ingresar; }
    bool isModificar() { return is_modificar; }
    bool isEliminar() { return is_eliminar; }

    QString getPlato() { return current_plato; }

    void modo_ingresar(QString familia, int x, int y);

    void modo_modificar(QString familia, QString plato, int x, int y);

    void set_familia(QString familia);

    void ingresar();

    void modificar();

    void eliminar();

signals:
    void closing();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_pushButton_eliminar_clicked();

    void on_pushButton_modificar_clicked();

    void on_pushButton_guardar_clicked();

    void on_pushButton_salir_clicked();    

    void on_lineEdit_medida_textChanged(const QString &arg1);

private:
    Ui::Plato *ui;

    QString familia;

    QString current_plato;

    bool is_ingresar;
    bool is_modificar;
    bool is_eliminar;

    int x;
    int y;
};

#endif // PLATO_H
