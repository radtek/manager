#include "login.h"
#include "ui_login.h"

#include <QHostInfo>
#include <QNetworkInterface>

Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);

    DATABASE_CONFIG(C_DB_FILE_CONFIG_NAME);
    if(!DATABASE_CONNECT)// Se conecto a la base de datos.
    {
        if(!opciones){
            opciones = new OpcionesDB;
            opciones->setAttribute(Qt::WA_DeleteOnClose);
            opciones->show();
        }else{
            opciones->raise();
        }
        //return;
    }

    afterShow = false;

    opciones = NULL;


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
    str_query += "SELECT persona.nombre, persona.tipo_item_nombre FROM persona";
    str_query += " JOIN persona_has_usuario per_h_u ON per_h_u.persona_cod = persona.cod";
    str_query += " JOIN usuario ON per_h_u.usuario_id = usuario.id";
    str_query += " WHERE";
    str_query += " usuario.nombre = '" + user + "'";
    str_query += " AND usuario.pass = '" + pass + "'";
    str_query += "&&END_QUERY&&";

    SYSTEM->multiple_query(str_query);

    QSqlQuery query;

    if(query.exec(str_query)){
        qDebug()<<str_query<<" ok :)"<<endl;
        query.next();
        if(query.isValid()){
            qDebug()<<"ONLINE"<<endl;

            SYSTEM->name_persona = ui->lineEdit_usuario->text();
            SYSTEM->pass_persona = ui->lineEdit_contrasenia->text();
            SYSTEM->tipo_persona = query.value(1).toString();

            if(SYSTEM->tipo_persona.compare(Persona::master) != 0
                    && SYSTEM->tipo_persona.compare(Persona::administrador) != 0
                    && SYSTEM->tipo_persona.compare(Persona::cajero) != 0) {

                QMessageBox::warning(this, "Advertencia", "El usuario no es válido.", "Ok");
                return;
            }


            APP_LOGIN->close();

            APP_MAINWINDOW->setAttribute(Qt::WA_DeleteOnClose);

            APP_MAINWINDOW->set_toolBar(APP_TOOLBAR);

            APP_TOOLBAR->setContextMenuPolicy(Qt::PreventContextMenu);

            APP_MAINWINDOW->showMaximized();


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
                QApplication::sendEvent(ui->pushButton_cancelar, key);
                return true;
            }break;
            }

        }else{

        }
        return false;
    }
    return eventFilter(obj, e);
}

void Login::on_pushButton_cambiarCorreo_clicked()
{
    CambiarCorreo* w = new CambiarCorreo(this);

    w->setAttribute(Qt::WA_DeleteOnClose);
    w->setWindowModality(Qt::WindowModal);
    w->setWindowFlags(Qt::Dialog);

    w->show();
}

void Login::on_pushButton_cambiarPass_clicked()
{
    CambiarPass* w = new CambiarPass(this);

    w->setAttribute(Qt::WA_DeleteOnClose);
    w->setWindowModality(Qt::WindowModal);
    w->setWindowFlags(Qt::Dialog);

    w->show();
}

void Login::on_pushButton_enviarmeCorreo_clicked()
{
    EnviarmeCorreo* w = new EnviarmeCorreo(this);

    w->setAttribute(Qt::WA_DeleteOnClose);
    w->setWindowModality(Qt::WindowModal);
    w->setWindowFlags(Qt::Dialog);

    w->show();
}
