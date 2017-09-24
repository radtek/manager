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

    QString getPlato() { return current_plato; }
    bool isEliminar() { return is_eliminar; }

    void modo_ingresar(QString familia, int x, int y);

    void modo_modificacion(QString familia, QString plato, int x, int y);

    void modo_eliminacion(QString familia, QString plato);

    void set_familia_(QString familia);

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

    QString current_plato;

    bool is_eliminar;
};

#endif // PLATO_H
