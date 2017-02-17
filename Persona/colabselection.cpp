#include "colabselection.h"
#include "ui_colabselection.h"

ColabSelection::ColabSelection(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ColabSelection)
{
    ui->setupUi(this);

    widget_previous = NULL;

    ui->listWidget->installEventFilter(this);
    ui->pushButton_seleccionar->installEventFilter(this);
    ui->pushButton_salir->installEventFilter(this);
}

ColabSelection::~ColabSelection()
{
    qDebug()<<"delete colab selection"<<endl;
    delete ui;
}

void ColabSelection::set_widget_previous(QWidget *widget_previous)
{
    this->widget_previous = widget_previous;
}
void ColabSelection::set_modo(int modo)
{
    this->modo = modo;
}

QListWidget* ColabSelection::get_lw()
{
    return ui->listWidget;
}

void ColabSelection::on_listWidget_itemClicked(QListWidgetItem *item)
{
    on_pushButton_seleccionar_clicked();
}
void ColabSelection::on_pushButton_seleccionar_clicked()
{
    QListWidgetItem* item = ui->listWidget->currentItem();
    if(!item)return;    

    if(this->modo == BUSCAR){
        int opc = ui->listWidget->currentRow(), tipo = -1;

        switch(opc)
        {
        case persona_items::PROVEEDOR:{
            tipo = opc;
        }break;
        case persona_items::TRANSPORTISTA:{
            tipo = opc;
        }break;
        case persona_items::CLIENTE_RUC:{
            tipo = opc;
        }break;
        case persona_items::CLIENTE_DNI:{
            tipo = opc;
        }break;
        case persona_items::USUARIO:{
            tipo = opc;
        }break;
        default:
            break;
        }
        ColaboradorBuscar* colab_bus = new ColaboradorBuscar;
        colab_bus->set_tipo(tipo);
        colab_bus->set_widget_previous(this);

        SYSTEM->change_center_w(this, colab_bus);
    }else{
        int opc = ui->listWidget->currentRow();

        QWidget* w;
        switch(opc)
        {
        case persona_items::PROVEEDOR:{
            w = new ProveedorOp;
            ((ProveedorOp*)w)->set_widget_previous(widget_previous);
            SYSTEM->change_center_w(this, w);
            ((ProveedorOp*)w)->set_captcha();
        }break;
        case persona_items::TRANSPORTISTA:{
            w = new TransportistaOp;
            ((TransportistaOp*)w)->set_widget_previous(widget_previous);
            SYSTEM->change_center_w(this, w);
            ((TransportistaOp*)w)->set_captcha();
        }break;
        case persona_items::CLIENTE_RUC:{
            w = new ClienteRUC;
            ((ClienteRUC*)w)->set_widget_previous(widget_previous);
            SYSTEM->change_center_w(this, w);
            ((ClienteRUC*)w)->set_captcha();
        }break;
        case persona_items::CLIENTE_DNI:{
            w = new ClienteDNI;
            ((ClienteDNI*)w)->set_widget_previous(widget_previous);
            SYSTEM->change_center_w(this, w);
			((ClienteDNI*)w)->set_captcha();
        }break;
        case persona_items::USUARIO:{
            w = new UsuarioOp;
            ((UsuarioOp*)w)->set_widget_previous(widget_previous);
            SYSTEM->change_center_w(this, w);
        }break;
        default:
            break;
        }        
    }
}
void ColabSelection::on_pushButton_salir_clicked()
{
    if(!widget_previous){
        SYSTEM->clear_center_w(this);
    }else{
        setAttribute(Qt::WA_DeleteOnClose);
        SYSTEM->change_center_w(this, widget_previous);
    }
}
void ColabSelection::showEvent(QShowEvent *event)
{
    event->accept();
    if(focusWidget()){
        focusWidget()->setFocus();
    }else{
        ui->listWidget->setFocus(Qt::TabFocusReason);
    }
}
void ColabSelection::closeEvent(QCloseEvent *event)
{
    event->accept();
    emit closing();
}

bool ColabSelection::eventFilter(QObject *obj, QEvent *e)
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
            case Qt::Key_Return:
                on_listWidget_itemClicked(ui->listWidget->currentItem());
                return true;
            }

        }else{

        }
        return false;
    }
    w_temp = ui->pushButton_seleccionar;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:
                ui->pushButton_seleccionar->click();
                return true;
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
