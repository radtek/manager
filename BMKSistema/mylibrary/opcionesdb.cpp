#include "opcionesdb.h"
#include "ui_opcionesdb.h"

OpcionesDB::OpcionesDB(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OpcionesDB)
{
    ui->setupUi(this);

    ui->lineEdit_host->setText(DATABASE->mf_get_host());
    ui->lineEdit_puerto->setText(DATABASE->mf_get_port());
    ui->lineEdit_nombre_db->setText(DATABASE->mf_get_name());
    ui->lineEdit_usuario->setText(DATABASE->mf_get_user());
    ui->lineEdit_clave->setText(DATABASE->mf_get_pass());
}

OpcionesDB::~OpcionesDB()
{
    delete ui;
}

void OpcionesDB::on_pushButton_guardar_clicked()
{
    DATABASE->mf_set_host(ui->lineEdit_host->text());
    DATABASE->mf_set_port(ui->lineEdit_puerto->text());
    DATABASE->mf_set_name(ui->lineEdit_nombre_db->text());
    DATABASE->mf_set_user(ui->lineEdit_usuario->text());
    DATABASE->mf_set_pass(ui->lineEdit_clave->text());

    DATABASE->mf_setConfig();

    this->close();

}
