#include "adminpass.h"
#include "ui_adminpass.h"

AdminPass::AdminPass(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AdminPass)
{
    ui->setupUi(this);

    modalidad = ADMIN_PASS;
    confirmado = false;
}

AdminPass::~AdminPass()
{
    delete ui;
}
void AdminPass::setModalidad(int modalidad)
{
    this->modalidad = modalidad;
}
void AdminPass::on_pushButton_aceptar_clicked()
{
    if(modalidad == ADMIN_PASS)
        confirmado = SYSTEM->confirmar_admin(Sistema::name_persona, ui->lineEdit_contrasenia->text());
    if(modalidad == CONFIRM_MY_PASS)
        confirmado = SYSTEM->confirmar_my_pass(Sistema::name_persona, ui->lineEdit_contrasenia->text());

    if(confirmado)
        this->close();
    else
        QMessageBox::warning(this, "Advertencia", "Ingrese una contraseña correcta.", "Aceptar");
}

void AdminPass::on_pushButton_cancelar_clicked()
{
    this->close();
}

void AdminPass::closeEvent(QCloseEvent *event)
{
    emit closing();
    event->accept();
}
