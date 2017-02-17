#include "a_compra.h"
#include "ui_a_compra.h"

A_Compra::A_Compra(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::A_Compra)
{
    ui->setupUi(this);    

    widget_previous = 0;

    // INSTALL EVENTS
    this->installEventFilter(this);
    ui->pushButton_ingresar->installEventFilter(this);
    ui->pushButton_buscar->installEventFilter(this);
    ui->pushButton_configurar_hoja->installEventFilter(this);    
    ui->pushButton_salir->installEventFilter(this);
}

A_Compra::~A_Compra()
{
    qDebug()<<"delete a compra"<<endl;
    delete ui;
}
void A_Compra::set_widget_previous(QWidget *w)
{
    this->widget_previous = w;
}

void A_Compra::on_pushButton_ingresar_clicked()
{	
    CompraMenu* w = new CompraMenu;
    w->set_widget_previous(this);
    w->set_modo(INGRESAR);
    SYSTEM->change_center_w(this, w);	
}

void A_Compra::on_pushButton_buscar_clicked()
{	
    CompraMenu* w= new CompraMenu;
    w->set_widget_previous(this);
    w->set_modo(BUSCAR);
    SYSTEM->change_center_w(this, w);	
}

void A_Compra::on_pushButton_configurar_hoja_clicked()
{    
    CompraMenu* w= new CompraMenu;
    w->set_widget_previous(this);
    w->set_modo(CONFIGURAR);
    SYSTEM->change_center_w(this, w);
}
void A_Compra::on_pushButton_salir_clicked()
{
    if(widget_previous){
        setAttribute(Qt::WA_DeleteOnClose);
        SYSTEM->change_center_w(this, widget_previous);
    }else{
        SYSTEM->clear_center_w(this);
    }
}
void A_Compra::showEvent(QShowEvent *se)
{
    if(focusWidget()){
        focusWidget()->setFocus();
    }else{
        ui->pushButton_ingresar->setFocus(Qt::TabFocusReason);
    }
    se->accept();
}
bool A_Compra::eventFilter(QObject *obj, QEvent *e)
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
                ui->pushButton_ingresar->click();
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
    w_temp = ui->pushButton_configurar_hoja;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:{
                ui->pushButton_configurar_hoja->click();
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

void A_Compra::set_serie_numero(QLineEdit *serie, QLineEdit *numero)
{
    QRegExp regExp_serie(QString()+"[0-9]{"+QString().setNum(SERIE_N_DIGITS)+","+QString().setNum(SERIE_N_DIGITS)+"}");
    QRegExp regExp_numero(QString()+"[0-9]{"+QString().setNum(NUMERO_N_DIGITS)+","+QString().setNum(NUMERO_N_DIGITS)+"}");

    serie->setValidator(new QRegExpValidator(regExp_serie));
    numero->setValidator(new QRegExpValidator(regExp_numero));
}
