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
    qDebug()<<ui->textEdit_ticket->toPlainText()<<endl;


}
