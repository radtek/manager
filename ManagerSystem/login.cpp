#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);

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

	QString str_query = "SELECT rol_id, usuario, pass FROM usuario"
		" WHERE rol_id = 1 AND usuario = '"+user+"' AND pass = '" + pass+"'"
		"&&END_QUERY&&";

    SYSTEM->multiple_query(str_query);

    QSqlQuery query;

    if(query.exec(str_query)){
        qDebug()<<str_query<<" ok :)"<<endl;
        query.next();
        if(query.isValid()){                        
            online = true;
        }
    }else{
        qDebug()<<str_query<<" bad :("<<endl;
    }

}

void Login::on_pushButton_cancelar_clicked()
{
    this->close();
}

void Login::showEvent(QShowEvent *se)
{
    this->setFocus();
    QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Tab, Qt::NoModifier);
    QApplication::sendEvent(this, key);

    se->accept();
}

bool Login::eventFilter(QObject *obj, QEvent *e)
{
    QWidget* w_temp;
    w_temp = this;
    if(obj == w_temp){
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
                ui->lineEdit_contrasenia->setFocus(Qt::TabFocusReason);
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
                ui->pushButton_aceptar->click();
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
            }

        }else{

        }
        return false;
    }
    return eventFilter(obj, e);
}
