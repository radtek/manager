#include "a_persona.h"
#include "ui_a_persona.h"

A_Persona::A_Persona(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::A_Persona)
{        
    ui->setupUi(this);

    widget_previous = NULL;

    this->installEventFilter(this);
    ui->pushButton_buscar->installEventFilter(this);    
    ui->pushButton_ingresar->installEventFilter(this);
    ui->pushButton_salir->installEventFilter(this);
}

A_Persona::~A_Persona()
{
    qDebug()<<"delete a persona"<<endl;
    delete ui;
}
void A_Persona::set_widget_previous(QWidget *widget_previous)
{
    this->widget_previous = widget_previous;
}

void A_Persona::on_pushButton_ingresar_clicked()
{
    QWidget* w = new ColabSelection;
    ((ColabSelection*)w)->set_widget_previous(this);

    SYSTEM->change_center_w(this, w);
}

void A_Persona::on_pushButton_buscar_clicked()
{
    QWidget* w = new ColabSelection;
    ((ColabSelection*)w)->set_widget_previous(this);

    ((ColabSelection*)w)->set_modo(BUSCAR);

    SYSTEM->change_center_w(this, w);
}

void A_Persona::on_pushButton_salir_clicked()
{
    if(widget_previous){
        setAttribute(Qt::WA_DeleteOnClose);
        SYSTEM->change_center_w(this, widget_previous);
    }else{
        SYSTEM->clear_center_w(this);
    }
}

void A_Persona::showEvent(QShowEvent *event)
{
    event->accept();
    if(focusWidget()){
        focusWidget()->setFocus();
    }else{                
        ui->pushButton_ingresar->setFocus(Qt::TabFocusReason);
    }
}
bool A_Persona::eventFilter(QObject *obj, QEvent *e)
{
    QWidget* w_temp;
    w_temp = this;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Escape:
                ui->pushButton_salir->click();
                return true;
            }

        }else{

        }
        return false;
    }
    w_temp = ui->pushButton_ingresar;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:{
                on_pushButton_ingresar_clicked();
                return true;
            }break;
            }

        }else{

        }
        return false;
    }    
    w_temp = ui->pushButton_buscar;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:{
                ui->pushButton_buscar->click();
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
            }

        }else{

        }
        return false;
    }
    return eventFilter(obj, e);
}
