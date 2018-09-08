#ifndef PERSONA_H
#define PERSONA_H

#include "mylibrary/mylibrary_global.h"

namespace Ui {
class Persona;
}

class Persona : public QWidget
{
    Q_OBJECT

public:
    static QString opc_clientes;
    static QString opc_proveedores;
    static QString opc_usuarios;

    static QString master;
    static QString administrador;
    static QString empleado;
    static QString cajero;
    static QString proveedor;
    static QString transportista;
    static QString cliente_ruc;
    static QString cliente_dni;
    static QString cliente_eventual;
    static QString cliente_delivery;

public:
    explicit Persona(QWidget *parent = 0);
    ~Persona();

    void setOperacion(int operacion);

    void setTipoPersona(QString tipoPersona);

    void setCodigo(QString codigo);

    void select(QString cod);

private slots:
    void on_pushButton_eliminar_clicked();

    void on_pushButton_guardar_clicked();

    void on_pushButton_mostrarContrasenia_clicked();

    void on_pushButton_salir_clicked();

private:
    Ui::Persona *ui;

    int operacion;
    QString tipoPersona;

};

#endif // PERSONA_H
