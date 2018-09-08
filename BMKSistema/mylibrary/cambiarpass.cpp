#include "cambiarpass.h"
#include "ui_cambiarpass.h"

CambiarPass::CambiarPass(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CambiarPass)
{
    ui->setupUi(this);
}

CambiarPass::~CambiarPass()
{
    delete ui;
}

void CambiarPass::on_pushButton_guardar_clicked()
{
    QString str_query;

    str_query  = "UPDATE usuario";
    str_query += " SET usuario.pass = '" + ui->lineEdit_contraseniaNueva->text() + "'";
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

void CambiarPass::on_pushButton_salir_clicked()
{
    this->close();
}
