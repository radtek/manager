#include "ventamenu.h"
#include "ui_ventamenu.h"

VentaMenu::VentaMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VentaMenu)
{
    ui->setupUi(this);

    this->installEventFilter(this);
    ui->listWidget->installEventFilter(this);
    ui->pushButton_ok->installEventFilter(this);
    ui->pushButton_salir->installEventFilter(this);
}

VentaMenu::~VentaMenu()
{
    qDebug()<<"delete venta menu"<<endl;
    delete ui;
}
void VentaMenu::set_widget_previous(QWidget *widget_previous)
{
    this->widget_previous = widget_previous;
}
void VentaMenu::set_modo(int modo)
{
    this->modo = modo;
}

void VentaMenu::on_listWidget_currentRowChanged(int currentRow)
{
    tipo = currentRow;
}

void VentaMenu::on_pushButton_ok_clicked()
{
    QListWidgetItem* item = ui->listWidget->currentItem();
    if(!item) return;

    if(modo == BUSCAR){
        VentaBuscar* w = new VentaBuscar;
        w->set_widget_previous(this);
        w->set_tipo(tipo);
        SYSTEM->change_center_w(this, w);
    }
    if(modo == INGRESAR){
        switch(tipo){
        case venta_items::REGISTRO_SIN_DOCUMENTO:{
            VentaRegistroSinDoc* w = new VentaRegistroSinDoc;            
            w->set_widget_previous(this);
            SYSTEM->change_center_w(this, w);
            w->next_serie_numero();
        }break;
        case venta_items::BOLETA:{
            VentaBoleta* w = new VentaBoleta;
            w->set_widget_previous(this);
            SYSTEM->change_center_w(this, w);
            w->next_serie_numero();
        }break;
        case venta_items::FACTURA:{
            VentaFactura* w = new VentaFactura;
            w->set_widget_previous(this);
            SYSTEM->change_center_w(this, w);
            w->next_serie_numero();
        }break;
        case venta_items::NOTA_PEDIDO:{
            VentaNotaPedido* w = new VentaNotaPedido;
            w->set_widget_previous(this);
            SYSTEM->change_center_w(this, w);
            w->next_serie_numero();
        }break;
        case venta_items::GUIA_REMISION_REMITENTE:{
            VentaGuiaRR* w = new VentaGuiaRR;
            w->set_widget_previous(this);
            SYSTEM->change_center_w(this, w);
            w->next_serie_numero();
        }break;
        case venta_items::COTIZACION:{
            VentaCotizacion* w = new VentaCotizacion;
            w->set_widget_previous(this);
            SYSTEM->change_center_w(this, w);
            w->next_serie_numero();
        }break;
        case venta_items::NOTA_CREDITO:{
            VentaNotaCredito* w = new VentaNotaCredito;
            w->set_widget_previous(this);
            SYSTEM->change_center_w(this, w);
            w->next_serie_numero();
        }break;
        case venta_items::NOTA_DEBITO:{
            VentaNotaDebito* w = new VentaNotaDebito;
            w->set_widget_previous(this);
            SYSTEM->change_center_w(this, w);
            w->next_serie_numero();
        }break;
        }
    }
    if(modo == CONFIGURAR){

    }
}

void VentaMenu::on_pushButton_salir_clicked()
{
    if(widget_previous){
        SYSTEM->change_center_w(this, widget_previous);
    }else{
        SYSTEM->clear_center_w(this);
    }
}

void VentaMenu::showEvent(QShowEvent *event)
{
    event->accept();
    if(focusWidget()){
        focusWidget()->setFocus();
    }else{
        ui->listWidget->setFocus(Qt::TabFocusReason);
    }
}
void VentaMenu::closeEvent(QCloseEvent *event)
{
    event->accept();
    emit closing();
}

bool VentaMenu::eventFilter(QObject *watched, QEvent *event)
{
    QWidget* w_temp;
    w_temp = this;
    if(watched == w_temp){
        if(event->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)event;

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
    w_temp = ui->listWidget;
    if(watched == w_temp){
        if(event->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)event;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:{
                ui->pushButton_ok->click();
                return true;
            }break;
            }

        }else{

        }
        return false;
    }
    w_temp = ui->pushButton_ok;
    if(watched == w_temp){
        if(event->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)event;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:{
                ui->pushButton_ok->click();
                return true;
            }break;
            }

        }else{

        }
        return false;
    }
    w_temp = ui->pushButton_salir;
    if(watched == w_temp){
        if(event->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)event;

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
    return eventFilter(watched, event);
}
