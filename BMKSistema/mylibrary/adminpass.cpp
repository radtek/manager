#include "adminpass.h"
#include "ui_adminpass.h"

AdminPass::AdminPass(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AdminPass)
{
    ui->setupUi(this);

    confirmado = false;
}

AdminPass::~AdminPass()
{
    delete ui;
}

void AdminPass::on_pushButton_aceptar_clicked()
{
    confirmado = SYSTEM->confirmar_admin(ui->lineEdit_contrasenia->text());

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
