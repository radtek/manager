#include "a_venta.h"
#include "ui_a_venta.h"

A_Venta::A_Venta(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::A_Venta)
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

A_Venta::~A_Venta()
{
    qDebug()<<"delete a venta"<<endl;
    delete ui;
}

void A_Venta::set_widget_previous(QWidget *w)
{
    this->widget_previous = w;
}

void A_Venta::on_pushButton_ingresar_clicked()
{
    VentaMenu* w = new VentaMenu;
    w->set_widget_previous(this);
    w->set_modo(INGRESAR);
    SYSTEM->change_center_w(this, w);
}

void A_Venta::on_pushButton_buscar_clicked()
{
    VentaMenu* w = new VentaMenu;
    w->set_widget_previous(this);
    w->set_modo(BUSCAR);
    SYSTEM->change_center_w(this, w);
}

void A_Venta::on_pushButton_configurar_clicked()
{
    VentaMenu* w = new VentaMenu;
    w->set_widget_previous(this);
    w->set_modo(CONFIGURAR);
    SYSTEM->change_center_w(this, w);
}

void A_Venta::on_pushButton_salir_clicked()
{
    if(widget_previous){
        SYSTEM->change_center_w(this, widget_previous);
    }else{
        SYSTEM->clear_center_w(this);
    }
}

void A_Venta::showEvent(QShowEvent *se)
{
    if(focusWidget()){
        focusWidget()->setFocus();
    }else{
        ui->pushButton_ingresar->setFocus(Qt::TabFocusReason);
    }
    se->accept();
}
bool A_Venta::eventFilter(QObject *obj, QEvent *e)
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
            }break;
            }

        }else{

        }
        return false;
    }
    return eventFilter(obj, e);
}
void A_Venta::set_serie_numero(QLineEdit *serie, QLineEdit *numero)
{
    QRegExp regExp_serie(QString()+"[0-9]{"+QString().setNum(SERIE_N_DIGITS)+","+QString().setNum(SERIE_N_DIGITS)+"}");
    QRegExp regExp_numero(QString()+"[0-9]{"+QString().setNum(NUMERO_N_DIGITS)+","+QString().setNum(NUMERO_N_DIGITS)+"}");

    serie->setValidator(new QRegExpValidator(regExp_serie));
    numero->setValidator(new QRegExpValidator(regExp_numero));
}
