#include "selecttablestogenscript.h"
#include "ui_selecttablestogenscript.h"

#include "cppscript.h"
#include <QFileDialog>
#include <QSqlDatabase>
#include <QMessageBox>

SelectTablesToGenScript::SelectTablesToGenScript(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SelectTablesToGenScript)
{
    ui->setupUi(this);

    QSqlDatabase db = QSqlDatabase::database();

    foreach(QString e,db.tables())
    {
        ui->listWidget_tables->addItem(e);
    }

}

SelectTablesToGenScript::~SelectTablesToGenScript()
{
    delete ui;
}

void SelectTablesToGenScript::on_pushButton_selectAll_clicked()
{    
    ui->listWidget_tables->selectAll();
}

void SelectTablesToGenScript::on_pushButton_generate_clicked()
{
    CPPScript obj;

    QFileDialog fileDialog;
    fileDialog.setFileMode(QFileDialog::DirectoryOnly);
    int opt = fileDialog.exec();

    ui->lineEdit_directory->setText(fileDialog.directory().absolutePath());
    obj.setDirFolder(ui->lineEdit_directory->text());

    obj.generar_clases();

}
