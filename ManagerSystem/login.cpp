#include "login.h"
#include "ui_login.h"

#include <QHostInfo>
#include <QNetworkInterface>

Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);

    afterShow = false;

    QString text;
    foreach(QNetworkInterface interfac, QNetworkInterface::allInterfaces())
    {
        text = interfac.hardwareAddress();
        qDebug()<<text<<endl;
    }

    foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost))
             qDebug() << address.toString();
    }

    online = false;

    this->installEventFilter(this);
    ui->lineEdit_usuario->installEventFilter(this);
    ui->lineEdit_contrasenia->installEventFilter(this);

    ui->pushButton_aceptar->installEventFilter(this);
    ui->pushButton_cancelar->installEventFilter(this);

}

Login::~Login()
{
    delete ui;
}

QPushButton* Login::pb_aceptar(){ return ui->pushButton_aceptar; }

void Login::on_pushButton_aceptar_clicked()
{
    QString user = ui->lineEdit_usuario->text();
    QString pass = ui->lineEdit_contrasenia->text();

    QString str_query;
    str_query += "SELECT rol_id, usuario, pass FROM usuario"
		" WHERE rol_id = 1 AND usuario = '"+user+"' AND pass = '" + pass+"'"
		"&&END_QUERY&&";

    SYSTEM->multiple_query(str_query);

    QSqlQuery query;

    if(query.exec(str_query)){
        qDebug()<<str_query<<" ok :)"<<endl;
        query.next();
        if(query.isValid()){
            online = true;
        }else{
            QMessageBox::warning(this, "Advertencia", "El usuario no es válido.", "Ok");
        }
    }else{
        QMessageBox::warning(this, "Advertencia", "El usuario no es válido.", "Ok");
        qDebug()<<str_query<<" bad :("<<endl;
    }

}

void Login::on_pushButton_cancelar_clicked()
{
    this->close();
}

void Login::showEvent(QShowEvent *se)
{
    se->accept();

    afterShow = false;
}

bool Login::eventFilter(QObject *obj, QEvent *e)
{
    QWidget* w_temp;
    w_temp = this;
    if(obj == w_temp){
        if(e->type() == QEvent::MouseButtonPress){
            if(focusWidget()){
                QFocusEvent *event = new QFocusEvent(QEvent::FocusIn, Qt::OtherFocusReason);
                QApplication::sendEvent(focusWidget(), event);
            }else{
                this->setFocus();
                QKeyEvent *event = new QKeyEvent(QEvent::KeyPress, Qt::Key_Tab, Qt::NoModifier);
                QApplication::sendEvent(this, event);
            }
            return true;
        }
        if(e->type() == QEvent::Paint){
            if(afterShow) {
                if(focusWidget()){
                    focusWidget()->setFocus(Qt::TabFocusReason);
                }else{
                    this->setFocus();
                    QKeyEvent *event = new QKeyEvent(QEvent::KeyPress, Qt::Key_Tab, Qt::NoModifier);
                    QApplication::sendEvent(this, event);
                }
                afterShow = false;
            }
            return true;
        }
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Escape:
                ui->pushButton_cancelar->click();
                return true;
            }

        }else{

        }
        return false;
    }
    w_temp = ui->lineEdit_usuario;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:{
                if(ui->lineEdit_contrasenia->text().compare("") == 0)
                    ui->lineEdit_contrasenia->setFocus(Qt::TabFocusReason);
                else
                    ui->pushButton_aceptar->click();
                return true;
            }break;
            case Qt::Key_Enter:{
                QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QApplication::sendEvent(ui->lineEdit_usuario, key);
                return true;
            }break;
            }

        }else{

        }
        return false;
    }
    w_temp = ui->lineEdit_contrasenia;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:{
                if(ui->lineEdit_usuario->text().compare("") == 0)
                    ui->lineEdit_usuario->setFocus(Qt::TabFocusReason);
                else
                    ui->pushButton_aceptar->click();
                return true;
            }break;
            case Qt::Key_Enter:{
                QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QApplication::sendEvent(ui->lineEdit_contrasenia, key);
                return true;
            }break;
            }

        }else{

        }
        return false;
    }

    w_temp = ui->pushButton_aceptar;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:{
                ui->pushButton_aceptar->click();
                return true;
            }break;
            case Qt::Key_Enter:{
                QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QApplication::sendEvent(ui->pushButton_aceptar, key);
                return true;
            }break;
            }

        }else{

        }
        return false;
    }
    w_temp = ui->pushButton_cancelar;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Tab:{
                this->setFocus();
            }break;
            case Qt::Key_Return:{
                ui->pushButton_cancelar->click();
                return true;
            }break;
            case Qt::Key_Enter:{
                QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QApplication::sendEvent(ui->pushButton_aceptar, key);
                return true;
            }break;
            }

        }else{

        }
        return false;
    }
    return eventFilter(obj, e);
}
