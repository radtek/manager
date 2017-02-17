#include "productoitem.h"
#include "ui_productoitem.h"

ProductoItem::ProductoItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProductoItem)
{
    ui->setupUi(this);

    widget_previous = NULL;

    this->installEventFilter(this);

    ui->pushButton_ing_tipo->installEventFilter(this);
    ui->pushButton_bus_tipo->installEventFilter(this);

    ui->pushButton_ing_marca->installEventFilter(this);
    ui->pushButton_bus_marca->installEventFilter(this);

    ui->pushButton_ing_unidad->installEventFilter(this);        
    ui->pushButton_bus_unidad->installEventFilter(this);

    ui->pushButton_salir->installEventFilter(this);
}

ProductoItem::~ProductoItem()
{
    qDebug()<<"delete producto item"<<endl;
    delete ui;
}
void ProductoItem::set_widget_previous(QWidget *widget_previous)
{
    this->widget_previous = widget_previous;
}

void ProductoItem::on_pushButton_ing_tipo_clicked()
{
    Tipo* w = new Tipo;
    w->set_widget_previous(this);

    SYSTEM->change_center_w(this, w);
}

void ProductoItem::on_pushButton_ing_marca_clicked()
{
    Marca* w = new Marca;
    w->set_widget_previous(this);

    SYSTEM->change_center_w(this, w);
}

void ProductoItem::on_pushButton_ing_unidad_clicked()
{
    Unidad* w = new Unidad;
    w->set_widget_previous(this);

    SYSTEM->change_center_w(this, w);
}

void ProductoItem::on_pushButton_bus_tipo_clicked()
{
    TipoBuscar* w = new TipoBuscar;
    w->set_widget_previous(this);

    SYSTEM->change_center_w(this, w);
}

void ProductoItem::on_pushButton_bus_marca_clicked()
{
    MarcaBuscar* w = new MarcaBuscar;
    w->set_widget_previous(this);

    SYSTEM->change_center_w(this, w);
}

void ProductoItem::on_pushButton_bus_unidad_clicked()
{
    UnidadBuscar* w = new UnidadBuscar;
    w->set_widget_previous(this);

    SYSTEM->change_center_w(this, w);
}

void ProductoItem::on_pushButton_salir_clicked()
{
    if(!widget_previous){
        SYSTEM->clear_center_w(this);
    }else{
        SYSTEM->change_center_w(this, widget_previous);
    }
}

void ProductoItem::showEvent(QShowEvent *se)
{
    se->accept();

    if(focusWidget()){
        focusWidget()->setFocus();
    }else{
        ui->pushButton_ing_tipo->setFocus();
    }
}
bool ProductoItem::eventFilter(QObject *obj, QEvent *e)
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
    w_temp = ui->pushButton_ing_tipo;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:
                ui->pushButton_ing_tipo->click();
                return true;
            }

        }else{

        }
        return false;
    }
    w_temp = ui->pushButton_bus_tipo;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:
                ui->pushButton_bus_tipo->click();
                return true;
            }

        }else{

        }
        return false;
    }
    w_temp = ui->pushButton_ing_marca;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:
                ui->pushButton_ing_marca->click();
                return true;
            }

        }else{

        }
        return false;
    }
    w_temp = ui->pushButton_bus_marca;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:
                ui->pushButton_bus_marca->click();
                return true;
            }

        }else{

        }
        return false;
    }
    w_temp = ui->pushButton_ing_unidad;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:
                ui->pushButton_ing_unidad->click();
                return true;
            }

        }else{

        }
        return false;
    }
    w_temp = ui->pushButton_bus_unidad;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:
                ui->pushButton_bus_unidad->click();
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
