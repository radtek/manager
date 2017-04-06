#include "home.h"
#include "ui_home.h"

Home::Home(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Home)
{
    ui->setupUi(this);

    afterShow = false;

    ui->pushButton->hide();

    this->installEventFilter(this);

    ui->toolButton_colaboradores->installEventFilter(this);
    ui->toolButton_compras->installEventFilter(this);
    ui->toolButton_configuracion->installEventFilter(this);
    ui->toolButton_productos->installEventFilter(this);
    ui->toolButton_reportes->installEventFilter(this);
    ui->toolButton_ventas->installEventFilter(this);

}

Home::~Home()
{
    qDebug()<<"HOME DELETE"<<endl;
    delete ui;
}

QToolButton* Home::tb_ventas(){ return ui->toolButton_ventas; }
QToolButton* Home::tb_compras(){ return ui->toolButton_compras; }
QToolButton* Home::tb_reportes(){ return ui->toolButton_reportes; }
QToolButton* Home::tb_configuracion(){ return ui->toolButton_configuracion; }
QToolButton* Home::tb_personas(){ return ui->toolButton_colaboradores; }
QToolButton* Home::tb_productos(){ return ui->toolButton_productos; }

void Home::set_toolbar(QWidget *toolbar)
{
    this->toolbar = toolbar;
}

void Home::on_toolButton_ventas_clicked()
{
    APP_MAINWINDOW->showToolBar();

    APP_TOOLBAR->tb_ventas()->click();
}

void Home::on_toolButton_compras_clicked()
{	
    APP_MAINWINDOW->showToolBar();

    APP_TOOLBAR->tb_compras()->click();
}

void Home::on_toolButton_productos_clicked()
{
    APP_MAINWINDOW->showToolBar();

    APP_TOOLBAR->tb_productos()->click();
}

void Home::on_toolButton_colaboradores_clicked()
{
    APP_MAINWINDOW->showToolBar();

    APP_TOOLBAR->tb_colaboradores()->click();
}

void Home::on_toolButton_reportes_clicked()
{
    APP_MAINWINDOW->showToolBar();

    APP_TOOLBAR->tb_reportes()->click();
}

void Home::on_toolButton_configuracion_clicked()
{
    APP_MAINWINDOW->showToolBar();

    APP_TOOLBAR->tb_configuracion()->click();
}
void Home::setFocus_ini()
{
    ui->toolButton_ventas->setFocus(Qt::TabFocusReason);
}

void Home::showEvent(QShowEvent *se)
{
    se->accept();

    afterShow = true;
}
bool Home::eventFilter(QObject *obj, QEvent *e)
{
    QWidget* w_temp;
    w_temp = this;
    if(obj == w_temp){
        if(e->type() == QEvent::MouseButtonPress){
            if(focusWidget()){
                focusWidget()->setFocus(Qt::TabFocusReason);
            }else{
                this->setFocus();
                QKeyEvent *event = new QKeyEvent(QEvent::KeyPress, Qt::Key_Tab, Qt::NoModifier);
                QApplication::sendEvent(this, event);
                ui->toolButton_ventas->setStyleSheet("background-color: rgb(255, 255, 0);");
                ui->toolButton_ventas->setFocus(Qt::TabFocusReason);
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
                //if(focusWidget()){
                    //focusWidget()->setFocus();
                //}else{
                this->setFocus();
                QKeyEvent *event = new QKeyEvent ( QEvent::KeyPress, Qt::Key_Tab, Qt::NoModifier);
                QApplication::sendEvent(this, event);
                ui->toolButton_ventas->setStyleSheet("background-color: rgb(255, 255, 0);");
                ui->toolButton_ventas->setFocus(Qt::TabFocusReason);
                //}
                afterShow = false;
            }
            return true;
        }
        return false;
    }
    w_temp = ui->toolButton_ventas;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Tab:{
                ui->toolButton_ventas->setStyleSheet("");
                ui->toolButton_compras->setStyleSheet("background-color: rgb(255, 255, 0);");
            }break;
            case Qt::Key_Return:{
                ui->toolButton_ventas->click();
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
    w_temp = ui->toolButton_compras;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Tab:{
                ui->toolButton_compras->setStyleSheet("");
                ui->toolButton_productos->setStyleSheet("background-color: rgb(255, 255, 0);");
            }break;
            case Qt::Key_Return:{
                ui->toolButton_compras->click();
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
    w_temp = ui->toolButton_productos;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Tab:{
                ui->toolButton_productos->setStyleSheet("");
                ui->toolButton_colaboradores->setStyleSheet("background-color: rgb(255, 255, 0);");
            }break;
            case Qt::Key_Return:{
                ui->toolButton_productos->click();
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
    w_temp = ui->toolButton_colaboradores;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Tab:{
                ui->toolButton_colaboradores->setStyleSheet("");
                ui->toolButton_reportes->setStyleSheet("background-color: rgb(255, 255, 0);");
            }break;
            case Qt::Key_Return:{
                ui->toolButton_colaboradores->click();
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
    w_temp = ui->toolButton_reportes;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Tab:{
                ui->toolButton_reportes->setStyleSheet("");
                ui->toolButton_configuracion->setStyleSheet("background-color: rgb(255, 255, 0);");
            }break;
            case Qt::Key_Return:{
                ui->toolButton_reportes->click();
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
    w_temp = ui->toolButton_configuracion;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Tab:{
                this->setFocus();
                ui->toolButton_configuracion->setStyleSheet("");
                ui->toolButton_ventas->setStyleSheet("background-color: rgb(255, 255, 0);");
            }break;
            case Qt::Key_Return:{
                ui->toolButton_configuracion->click();
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

void Home::on_pushButton_clicked()
{
/*
    SmtpClient smtp("smtp.gmail.com", 465, SmtpClient::SslConnection);

    // We need to set the username (your email address) and password
    // for smtp authentification.

    smtp.setUser("lordalex27@gmail.com");
    smtp.setPassword("Olhaok12");

    // Now we create a MimeMessage object. This is the email.

    MimeMessage message;

    EmailAddress sender("lordalex27@gmail.com", "Your Name");
    message.setSender(&sender);

    EmailAddress to("lordalex27@gmail.com", "Recipient's Name");
    message.addRecipient(&to);

    message.setSubject("SmtpClient for Qt - Demo");

    // Now add some text to the email.
    // First we create a MimeText object.

    MimeText text;

    text.setText("Hi,\nThis is a simple email message.\n");

    // Now add it to the mail

    message.addPart(&text);

    // Now we can send the mail

    if (!smtp.connectToHost()) {
        qDebug() << "Failed to connect to host!" << endl;
    }

    if (!smtp.login()) {
        qDebug() << "Failed to login!" << endl;
    }

    if (!smtp.sendMail(message)) {
        qDebug() << "Failed to send mail!" << endl;
    }

    smtp.quit();
*/
}
