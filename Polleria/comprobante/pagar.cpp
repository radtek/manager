#include "pagar.h"
#include "ui_pagar.h"

Pagar::Pagar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Pagar)
{
    ui->setupUi(this);
}

Pagar::~Pagar()
{
    delete ui;
}
double Pagar::getTotal()
{
    return ui->doubleSpinBox_total->value();
}
void Pagar::on_pushButton_010_clicked()
{
    double sum = ui->doubleSpinBox_total->value()+0.1;
    ui->doubleSpinBox_total->setValue(sum);
}

void Pagar::on_pushButton_020_clicked()
{
    double sum = ui->doubleSpinBox_total->value()+0.2;
    ui->doubleSpinBox_total->setValue(sum);
}

void Pagar::on_pushButton_050_clicked()
{
    double sum = ui->doubleSpinBox_total->value()+0.5;
    ui->doubleSpinBox_total->setValue(sum);
}

void Pagar::on_pushButton_01_clicked()
{
    double sum = ui->doubleSpinBox_total->value()+1;
    ui->doubleSpinBox_total->setValue(sum);
}

void Pagar::on_pushButton_02_clicked()
{
    double sum = ui->doubleSpinBox_total->value()+2;
    ui->doubleSpinBox_total->setValue(sum);
}

void Pagar::on_pushButton_05_clicked()
{
    double sum = ui->doubleSpinBox_total->value()+5;
    ui->doubleSpinBox_total->setValue(sum);
}

void Pagar::on_pushButton_10_clicked()
{
    double sum = ui->doubleSpinBox_total->value()+10;
    ui->doubleSpinBox_total->setValue(sum);
}

void Pagar::on_pushButton_20_clicked()
{
    double sum = ui->doubleSpinBox_total->value()+20;
    ui->doubleSpinBox_total->setValue(sum);
}

void Pagar::on_pushButton_50_clicked()
{
    double sum = ui->doubleSpinBox_total->value()+50;
    ui->doubleSpinBox_total->setValue(sum);
}

void Pagar::on_pushButton_100_clicked()
{
    double sum = ui->doubleSpinBox_total->value()+100;
    ui->doubleSpinBox_total->setValue(sum);
}

void Pagar::on_pushButton_200_clicked()
{
    double sum = ui->doubleSpinBox_total->value()+200;
    ui->doubleSpinBox_total->setValue(sum);
}

void Pagar::on_pushButton_aceptar_clicked()
{
    this->close();
}

void Pagar::on_pushButton_salir_clicked()
{
    ui->doubleSpinBox_total->setValue(0);
    this->close();
}
void Pagar::closeEvent(QCloseEvent *event)
{
    emit closing();
    event->accept();
}
