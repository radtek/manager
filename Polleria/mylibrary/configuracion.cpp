#include "configuracion.h"
#include "ui_configuracion.h"

Configuracion::Configuracion(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Configuracion)
{
    ui->setupUi(this);

    afterShow = false;
    firstShow = false;

    widget_previous = NULL;

    this->installEventFilter(this);

    ui->pushButton_salir->installEventFilter(this);
}

Configuracion::~Configuracion()
{
    qDebug()<<"delete configuracion"<<endl;
    delete ui;
}
void Configuracion::set_widget_previous(QWidget *widget_previous)
{
    this->widget_previous = widget_previous;
}

void Configuracion::showEvent(QShowEvent* se)
{
    se->accept();

    afterShow = true;

    if(!firstShow){
        firstShow = true;
    }
}
bool Configuracion::eventFilter(QObject *obj, QEvent *e)
{
    QWidget* w_temp;
    w_temp = this;
    if(obj == w_temp){
        if(e->type() == QEvent::MouseButtonPress){
            if(focusWidget()){
                focusWidget()->setFocus();
            }else{

            }
            return true;
        }
        if(e->type() == QEvent::MouseButtonDblClick){
            if(focusWidget()){
                focusWidget()->setFocus();
            }
            return true;
        }
        if(e->type() == QEvent::Paint){
            if(afterShow) {
                if(focusWidget()){
                    focusWidget()->setFocus();
                }else{

                }
                afterShow = false;
            }
            return true;
        }
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Escape:{
                ui->pushButton_salir->click();
                return true;
            }break;
            }
        }else{

        }
        return false;
    }

    w_temp = ui->pushButton_salir;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Tab:{
                this->setFocus();
            }break;
            case Qt::Key_Return:{
                ui->pushButton_salir->click();
                return true;
            }break;
            case Qt::Key_Enter:{
                QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QApplication::sendEvent(w_temp, key);
                return true;
            }break;
            }

        }else{

        }
        return false;
    }
    return eventFilter(obj, e);
}

void Configuracion::on_pushButton_salir_clicked()
{
    if(widget_previous){
        setAttribute(Qt::WA_DeleteOnClose);
        SYSTEM->change_center_w(this, widget_previous);
    }else{
        SYSTEM->clear_center_w(this);
    }
}

void Configuracion::on_pushButton_aplicarLetras_clicked()
{
    //qDebug()<<ui->textEdit_ticket->toPlainText()<<endl;


}

void Configuracion::on_pushButton_backup_clicked()
{
    QString curPath = QDir::currentPath();
    QFileDialog dialog(this, "Guardar Archivo", curPath);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setNameFilter(tr("Archivo sql (*.sql)"));

    QString fileName = "";

    if (dialog.exec()) {
        //QDateTime dt = QDateTime::currentDateTime();
        //QString str_dt = dt.toString("dd-MM-yyyy");
        fileName = dialog.selectedFiles().front();
        /*
        if (fileName.compare("") == 0) {
            fileName += str_dt + ".sql";
        }
        */
    } else {
        return;
    }
    qDebug() << fileName << endl;

    QString db_name = QSqlDatabase::database().databaseName();

    QProcess *myProcess = new QProcess(this);
    QString command = "\"C:/Program Files/MySQL/MySQL Server 5.7/bin/mysqldump\"";
    QStringList arguments = QStringList() << "--host=127.0.0.1" << "--user=root" << "--password=1234"
        //<< "--add-drop-database" << "--routines"
        << "--no-create-db"
        << "--no-create-info"
        << "--no-set-names"
        << "--no-tablespaces"
        << "--skip-add-locks"
        << "--skip-disable-keys"
        << "--databases" << db_name
        << QString()+"--result-file="+""+fileName+"";

    connect(myProcess, SIGNAL(finished(int, QProcess::ExitStatus))
        , this, SLOT(on_myProccess_finished(int, QProcess::ExitStatus)));

    qDebug() << command << " " << arguments << endl;
    myProcess->start(command, arguments);
}

void Configuracion::on_pushButton_restore_clicked()
{
    int ret = QMessageBox::warning(this, "ATENCION", "Va restaurar una copia anterior.\n"
                                   "¿Esta seguro de realizar esta operación?", "Si", "No");
    switch (ret) {
    case 0: {


    }break;
    case 1: {
        return;
    }
    }

    QString curPath = QDir::currentPath();
    QFileDialog dialog(this, "Abrir Archivo", curPath);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setNameFilter(tr("Archivo sql (*.sql)"));

    QString fileName = "";
    if (dialog.exec()) {
        fileName = dialog.selectedFiles().front();
    }
    else {
        return;
    }
    qDebug() << fileName << endl;

    if (fileName.compare("") == 0)
        return;
    QString str_query;
    str_query += "DELETE FROM comprobante";
    str_query += "&&END_QUERY&&";
    str_query += "ALTER TABLE comprobante AUTO_INCREMENT = 1";
    str_query += "&&END_QUERY&&";
    str_query += "ALTER TABLE comprobante_has_producto AUTO_INCREMENT = 1";
    str_query += "&&END_QUERY&&";
    str_query += "ALTER TABLE comprobante_has_persona AUTO_INCREMENT = 1";
    str_query += "&&END_QUERY&&";
    str_query += "DELETE FROM producto";
    str_query += "&&END_QUERY&&";
    str_query += "ALTER TABLE producto AUTO_INCREMENT = 1";
    str_query += "&&END_QUERY&&";
    str_query += "DELETE FROM familia";
    str_query += "&&END_QUERY&&";
    str_query += "ALTER TABLE familia AUTO_INCREMENT = 1";
    str_query += "&&END_QUERY&&";
    str_query += "DELETE FROM usuario";
    str_query += "&&END_QUERY&&";
    str_query += "ALTER TABLE usuario AUTO_INCREMENT = 1";
    str_query += "&&END_QUERY&&";
    str_query += "DELETE FROM persona";
    str_query += "&&END_QUERY&&";
    str_query += "DELETE FROM grupo";
    str_query += "&&END_QUERY&&";
    str_query += "DELETE FROM item";
    str_query += "&&END_QUERY&&";
    str_query += "DELETE FROM direccion";
    str_query += "&&END_QUERY&&";
    str_query += "DELETE FROM ingrediente";
    str_query += "&&END_QUERY&&";
    str_query += "ALTER TABLE ingrediente AUTO_INCREMENT = 1";
    str_query += "&&END_QUERY&&";
    str_query += "DELETE FROM datos";
    str_query += "&&END_QUERY&&";

    QSqlQuery query;

    SYSTEM->multiple_query(str_query);

    qDebug() << str_query << endl;
    if (query.exec(str_query)) {

    } else {
        QMessageBox::critical(this, "Error", "No finalizo corretamente.");
        return;
    }
    QString db_name = QSqlDatabase::database().databaseName();

    QProcess *myProcess = new QProcess(this);
    QString command = "\"C:/Program Files/MySQL/MySQL Server 5.7/bin/mysql.exe\"";
    QStringList arguments = QStringList()
            << "--host=127.0.0.1" << "--user=root" << "--password=1234"
            << "--database = " + db_name;

        //<< "--execute=\"source " + fileName + "\"";
    myProcess->waitForFinished(100000);
    myProcess->setStandardInputFile(fileName);
    connect(myProcess, SIGNAL(started())
        , this, SLOT(on_myProccess_started()));
    connect(myProcess, SIGNAL(finished(int, QProcess::ExitStatus))
        , this, SLOT(on_myProccess_finished(int, QProcess::ExitStatus)));

    qDebug() << command << endl;//<< arguments << endl;
    myProcess->start(command, arguments);
}
void Configuracion::on_myProccess_started()
{
    qDebug() << "START" << endl;

}
void Configuracion::on_myProccess_finished(int exitCode, QProcess::ExitStatus exitStatus)
{
    qDebug() << "Exit Code: " << exitCode;
    qDebug() << "Exit Status: " << exitStatus;

    if (exitCode == 0) {
        QMessageBox::information(this, "Información", "Felicidades. Finalizo la operacion sin errores.", "Ok");
    } else {
        QMessageBox::critical(this, "Error", "No finalizo corretamente.");
    }

    delete sender();
    //QProcess* p = (QProcess*)QObject::sender();

    //qDebug() << p->errorString() << endl;
}
