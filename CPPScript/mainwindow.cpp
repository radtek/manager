#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "selecttablestogenscript.h"
#include <QFile>
#include <QDebug>
const char * C_CONNECT_ERROR_MSG = "No se pudo conectar con la base de datos";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    foreach(QString e, QSqlDatabase::drivers())
    {
        ui->cb_manager->addItem(e);
    }

    QFile out("config.cfg");

    if(out.open(QIODevice::ReadOnly))
    {
        out.close();
    }
    QFile file("config.cfg");

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);

        QString driver =in.readLine();
        //in>>driver;

        QString host = in.readLine();
        QString port = in.readLine();
        QString user = in.readLine();
        QString pass = in.readLine();

        QString database;
        in>>database;

        file.close();
        int index = ui->cb_manager->findText(driver);
        //Si existe
        if(index>=0)
        {
            ui->cb_manager->setCurrentIndex(index);
        }

        ui->le_host->setText(host);
        ui->le_port->setText(port);
        ui->le_username->setText(user);
        ui->le_pass->setText(pass);
        ui->le_database->setText(database);


        ui->btn_connect->setFocus(Qt::OtherFocusReason);

    }
    else
    {
       qDebug()<<"No se pudo abrir el archivo";

    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_btn_connect_clicked()
{
    QString driver = ui->cb_manager->currentText();
    QString host = ui->le_host->text();
    QString user = ui->le_username->text();
    QString pass = ui->le_pass->text();
    QString port = ui->le_port->text();
    QString database = ui->le_database->text();

    QSqlDatabase db;
    db = QSqlDatabase::addDatabase(driver);
    db.setHostName(host);
    db.setUserName(user);
    db.setPassword(pass);
    db.setPort(port.toInt());
    db.setDatabaseName(database);

    QFile file("config.cfg");
    if(file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QTextStream stream(&file);
        stream<<driver<<endl;
        stream<<host<<endl;
        stream<<port<<endl;
        stream<<user<<endl;
        stream<<pass<<endl;
        stream<<database<<endl;
        file.close();
    }
    if(db.open())
    {
        //Llamar a la siguiente ventana
        SelectTablesToGenScript* form = new SelectTablesToGenScript();
        form->setAttribute(Qt::WA_DeleteOnClose);
        form->show();
        close();
    }
    else
    {
        QMessageBox::information(this,"Error",C_CONNECT_ERROR_MSG);
    }
}
