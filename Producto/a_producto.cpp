#include "a_producto.h"
#include "ui_a_producto.h"

A_Producto::A_Producto(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::A_Producto)
{    
    ui->setupUi(this);

    widget_previous = NULL;    

	disconnect(ui->pushButton_guardar, SIGNAL(clicked()), this, SLOT(on_pushButton_guardar_clicked()));
	disconnect(ui->pushButton_buscar, SIGNAL(clicked()), this, SLOT(on_pushButton_buscar_clicked()));
	disconnect(ui->pushButton_salir, SIGNAL(clicked()), this, SLOT(on_pushButton_salir_clicked()));

	connect(ui->pushButton_guardar, SIGNAL(clicked()), this, SLOT(on_pushButton_guardar_clicked()));
	connect(ui->pushButton_buscar, SIGNAL(clicked()), this, SLOT(on_pushButton_buscar_clicked()));
	connect(ui->pushButton_salir, SIGNAL(clicked()), this, SLOT(on_pushButton_salir_clicked()));

    this->installEventFilter(this);
    ui->pushButton_guardar->installEventFilter(this);
    ui->pushButton_buscar->installEventFilter(this);    
    ui->pushButton_salir->installEventFilter(this);
}

A_Producto::~A_Producto()
{
    qDebug()<<"delete a producto"<<endl;
    delete ui;
}
void A_Producto::set_widget_previous(QWidget *widget_previous)
{
    this->widget_previous = widget_previous;
}

void A_Producto::on_pushButton_guardar_clicked()
{
	ProductoFormTransaction* w = new ProductoFormTransaction;
    w->set_widget_previous(this);

    SYSTEM->change_center_w(this, w);
}

void A_Producto::on_pushButton_buscar_clicked()
{
	ProductoBuscar* w = new ProductoBuscar;
    w->set_widget_previous(this);

    SYSTEM->change_center_w(this, w);
}

void A_Producto::on_pushButton_salir_clicked()
{    
    if(widget_previous){
        setAttribute(Qt::WA_DeleteOnClose);
        SYSTEM->change_center_w(this, widget_previous);
    }else{
        SYSTEM->clear_center_w(this);
	}
    
}

void A_Producto::showEvent(QShowEvent *se)
{
    se->accept();
    if(focusWidget()){
        focusWidget()->setFocus(Qt::TabFocusReason);
    }else{
        ui->pushButton_guardar->setFocus(Qt::TabFocusReason);
    }
}
bool A_Producto::eventFilter(QObject *obj, QEvent *e)
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
    w_temp = ui->pushButton_guardar;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:{
                ui->pushButton_guardar->click();
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
