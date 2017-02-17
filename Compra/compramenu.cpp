#include "compramenu.h"
#include "ui_compramenu.h"

CompraMenu::CompraMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CompraMenu)
{
    ui->setupUi(this);

    tipo = 1;
    ui->listWidget->setCurrentRow(tipo);

    ui->listWidget->installEventFilter(this);
    ui->pushButton_ok->installEventFilter(this);
    ui->pushButton_salir->installEventFilter(this);

}
CompraMenu::~CompraMenu()
{
    qDebug()<<"delete compra menu"<<endl;
    delete ui;
}
void CompraMenu::set_widget_previous(QWidget *widget_previous)
{
    this->widget_previous = widget_previous;
}
void CompraMenu::set_modo(int modo)
{
    this->modo = modo;
}
void CompraMenu::on_listWidget_currentRowChanged(int currentRow)
{
    tipo = currentRow;
}
void CompraMenu::on_pushButton_ok_clicked()
{
    QListWidgetItem* item = ui->listWidget->currentItem();
    if(!item) return;

    if(modo == BUSCAR){
        CompraBuscar* w = new CompraBuscar;
        w->set_widget_previous(this);
        w->set_tipo(tipo);
        SYSTEM->change_center_w(this, w);
    }
    if(modo == INGRESAR){
        switch(tipo){
        case compra_items::ORDEN:{
            CompraOrden* w = new CompraOrden;
            w->set_widget_previous(this);
            SYSTEM->change_center_w(this, w);
        }break;
        case compra_items::FACTURA:{
            CompraFactura* w = new CompraFactura;
            w->set_widget_previous(this);
            SYSTEM->change_center_w(this, w);
        }break;
        case compra_items::BOLETA:{
            CompraBoleta* w = new CompraBoleta;
            w->set_widget_previous(this);
            SYSTEM->change_center_w(this, w);
        }break;
        case compra_items::GUIA_REMISION_REMITENTE:{
            CompraGuiaRR* w = new CompraGuiaRR;
            w->set_widget_previous(this);
            SYSTEM->change_center_w(this, w);
        }break;
        case compra_items::FLETE:{
            CompraFlete* w = new CompraFlete;
            w->set_widget_previous(this);
            SYSTEM->change_center_w(this, w);
        }break;
        case compra_items::NOTA_CREDITO:{
            CompraNotaCredito* w = new CompraNotaCredito;
            w->set_widget_previous(this);
            SYSTEM->change_center_w(this, w);
        }break;
        case compra_items::NOTA_DEBITO:{
            CompraNotaDebito* w = new CompraNotaDebito;
            w->set_widget_previous(this);
            SYSTEM->change_center_w(this, w);
        }break;
        case compra_items::SALDO:{
            CompraSaldo* w = new CompraSaldo;
            w->set_widget_previous(this);
            SYSTEM->change_center_w(this, w);
        }break;
        }
    }
}
void CompraMenu::on_pushButton_salir_clicked()
{
    SYSTEM->clear_center_w(this);
}

void CompraMenu::showEvent(QShowEvent *event)
{
    event->accept();
    if(focusWidget()){
        focusWidget()->setFocus();
    }else{
        ui->listWidget->setFocus(Qt::TabFocusReason);
    }    
}
void CompraMenu::closeEvent(QCloseEvent *event)
{
    event->accept();
    emit closing();
}

bool CompraMenu::eventFilter(QObject *obj, QEvent *e)
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
    w_temp = ui->listWidget;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

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
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

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
