#include "cambiarcorreo.h"
#include "ui_cambiarcorreo.h"

CambiarCorreo::CambiarCorreo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CambiarCorreo)
{
    ui->setupUi(this);
}

CambiarCorreo::~CambiarCorreo()
{
    delete ui;
}

void CambiarCorreo::on_pushButton_guardar_clicked()
{
    QString str_query;

    str_query  = "UPDATE persona";
    str_query += " JOIN persona_has_usuario per_h_u ON per_h_u.persona_cod = persona.cod";
    str_query += " JOIN usuario ON per_h_u.usuario_id = usuario.id";
    str_query += " SET persona.email = '" + ui->lineEdit_correo->text() + "'";
    str_query += " WHERE usuario.nombre = '" + ui->lineEdit_usuario->text() + "'";
    str_query += " AND usuario.pass = '" + ui->lineEdit_contrasenia->text() + "'";
    QSqlQuery query;

    qDebug()<<str_query<<endl;
    if(query.exec(str_query)){
        QMessageBox::information(this, "Información", "Éxito.", "Aceptar");
        this->close();
    }else{
        QMessageBox::critical(this, "Error", "El usuario o contraseña no coinciden.", "Aceptar");
    }
}

void CambiarCorreo::on_pushButton_salir_clicked()
{
    this->close();
}
