#include "toolbar.h"
#include "ui_toolbar.h"

ToolBar::ToolBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ToolBar)
{
    ui->setupUi(this);

    afterShow = false;

    cur_label = NULL;    

    this->installEventFilter(this);
    ui->toolButton_colaboradores->installEventFilter(this);
    ui->toolButton_compras->installEventFilter(this);
    ui->toolButton_home->installEventFilter(this);    
    ui->toolButton_productos->installEventFilter(this);
    ui->toolButton_reportes->installEventFilter(this);
    ui->toolButton_ventas->installEventFilter(this);
    ui->toolButton_salvados->installEventFilter(this);
    ui->toolButton_configuracion->installEventFilter(this);
}

ToolBar::~ToolBar()
{
    qDebug()<<"TOOLBAR DELETE"<<endl;
    delete ui;
}

QToolButton* ToolBar::tb_home(){ return ui->toolButton_home; }
QToolButton* ToolBar::tb_ventas(){ return ui->toolButton_ventas; }
QToolButton* ToolBar::tb_compras(){return ui->toolButton_compras; }
QToolButton* ToolBar::tb_productos(){ return ui->toolButton_productos; }
QToolButton* ToolBar::tb_colaboradores(){ return ui->toolButton_colaboradores; }
QToolButton* ToolBar::tb_reportes(){ return ui->toolButton_reportes; }
QToolButton* ToolBar::tb_configuracion(){ return ui->toolButton_configuracion; }

void ToolBar::set_labels_ssheet(int pos)
{
    /*
    SYSTEM->set_stylesheet(QVector<QWidget*>()<<ui->label_ventas<<ui->label_compras<<ui->label_reportes
                           <<ui->label_productos<<ui->label_colaboradores
                           <<ui->label_opciones, pos);
                           */
}

void ToolBar::on_toolButton_home_clicked()
{
    ui->toolButton_home->setFocus();

    QWidget* w = new Home;

    APP_MAINWINDOW->hideToolBar();
    APP_MAINWINDOW->setCentralWidget(w);
}

void ToolBar::on_toolButton_ventas_clicked()
{	    
    //bool b = false;
    //double igv = SYSTEM->get_igv();
    //if(igv == 0.0){
        //QMessageBox::warning(0, "Ingrese IGV", "Usted puede ingresar el IGV en el módulo de Configuración", "Ok");
        //b = true;
    //}
    //double dolar = SYSTEM->get_dolar();
    //if(dolar == 0.0){
        //QMessageBox::warning(0, "Actualize Tipo de Cambio", "Usted puede actualizar el Tipo de Cambio en el módulo de Configuración", "Ok");
        //b = true;
    //}

    ui->toolButton_ventas->setFocus();

    // memory background-color: rgb(151, 192, 36);
    QWidget* w = SYSTEM->set_center_w(new VentaBuscar, "background-color: rgb(211, 235, 148);");
    APP_MAINWINDOW->setCentralWidget(w);

    if(cur_label){
        cur_label->setStyleSheet("");
    }
    cur_label = ui->label_ventas;

    ui->label_ventas->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: rgb(0, 0, 0);");
}

void ToolBar::on_toolButton_compras_clicked()
{    
	//bool b = false;
    //double igv = SYSTEM->get_igv();
    //if(igv == 0.0){
        //QMessageBox::warning(0, "Ingrese IGV", "Usted puede ingresar el IGV en el módulo de Configuración", "Ok");
        //b = true;
    //}
    //double dolar = SYSTEM->get_dolar();
    //if(dolar == 0.0){
        //QMessageBox::warning(0, "Actualize Tipo de Cambio", "Usted puede actualizar el Tipo de Cambio en el módulo de Configuración", "Ok");
        //b = true;
    //}
	
    ui->toolButton_compras->setFocus();

    // memory background-color: rgb(28, 112, 239);
    QWidget* w = SYSTEM->set_center_w(new CompraBuscar, "background-color: rgb(160, 195, 248);");
    APP_MAINWINDOW->setCentralWidget(w);

    if(cur_label){
        cur_label->setStyleSheet("");
    }
    cur_label = ui->label_compras;

    ui->label_compras->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: rgb(0, 0, 0);");
}
void ToolBar::on_toolButton_productos_clicked()
{
    ui->toolButton_productos->setFocus();

    QWidget* place = new ProductoBuscar;
    // memory background-color: rgb(228, 193, 0);
    QWidget* w = SYSTEM->set_center_w(place, "background-color: rgb(255, 236, 128);");

    APP_MAINWINDOW->setCentralWidget(w);

    if(cur_label){
        cur_label->setStyleSheet("");
    }
    cur_label = ui->label_productos;

    ui->label_productos->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: rgb(0, 0, 0);");
}

void ToolBar::on_toolButton_colaboradores_clicked()
{
    ui->toolButton_colaboradores->setFocus();

    // memory background-color: rgb(0, 51, 153);
    QWidget* w = SYSTEM->set_center_w(new ProductoBuscar, "background-color: rgb(51, 119, 255);");
    APP_MAINWINDOW->setCentralWidget(w);

    if(cur_label){
        cur_label->setStyleSheet("");
    }
    cur_label = ui->label_colaboradores;

    ui->label_colaboradores->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: rgb(0, 0, 0);");

}
void ToolBar::on_toolButton_reportes_clicked()
{
    ui->toolButton_reportes->setFocus();

    // memory background-color: rgb(0, 151, 68);
    QWidget* w = SYSTEM->set_center_w(new A_Reporte, "background-color: rgb(51, 255, 143)");
    APP_MAINWINDOW->setCentralWidget(w);

    if(cur_label){
        cur_label->setStyleSheet("");
    }
    cur_label = ui->label_reportes;

    ui->label_reportes->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: rgb(0, 0, 0);");
}

void ToolBar::on_toolButton_configuracion_clicked()
{
    ui->toolButton_configuracion->setFocus();

    // memory background-color: rgb(110, 110, 110);
    QWidget* w = SYSTEM->set_center_w(new Configuracion, "background-color: rgb(191, 191, 191);");
    APP_MAINWINDOW->setCentralWidget(w);

    if(cur_label){
        cur_label->setStyleSheet("");
    }
    cur_label = ui->label_configuracion;

    ui->label_configuracion->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: rgb(0, 0, 0);");
}

void ToolBar::on_toolButton_salvados_clicked()
{
    ui->toolButton_salvados->setFocus();

    // memory background-color: rgb(110, 110, 110);
    QWidget* w = SYSTEM->set_center_w(new Salvados, "background-color: rgb(191, 191, 191);");
    APP_MAINWINDOW->setCentralWidget(w);

    if(cur_label){
        cur_label->setStyleSheet("");
    }
    cur_label = ui->label_salvados;

    ui->label_salvados->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: rgb(0, 0, 0);");
}


void ToolBar::showEvent(QShowEvent *se)
{
    se->accept();

    afterShow = true;
}
bool ToolBar::eventFilter(QObject *obj, QEvent *e)
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
            }
            return true;
        }
        if(e->type() == QEvent::Paint){
            if(afterShow) {
                if(focusWidget()){
                    focusWidget()->setFocus(Qt::TabFocusReason);
                }else{
                    this->setFocus();
                    QKeyEvent *event = new QKeyEvent(QEvent::KeyPress, Qt::Key_Tab, Qt::NoModifier);
                    QApplication::sendEvent(this, event);
                }
                afterShow = false;
            }
            return true;
        }
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Tab:{
                if(cur_label){
                    cur_label->setStyleSheet("");
                }
                cur_label = ui->label_inicio;

                ui->label_inicio->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: rgb(0, 0, 0);");
                ui->toolButton_home->setFocus(Qt::TabFocusReason);
                return true;
            }break;
            }
        }
        return false;
    }

    w_temp = ui->toolButton_home;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Tab:{
                if(cur_label){
                    cur_label->setStyleSheet("");
                }
                cur_label = ui->label_ventas;

                ui->label_ventas->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: rgb(0, 0, 0);");
                ui->toolButton_ventas->setFocus(Qt::TabFocusReason);
                return true;
            }break;
            case Qt::Key_Return:{
                ui->toolButton_home->click();
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
    w_temp = ui->toolButton_ventas;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Tab:{
                if(cur_label){
                    cur_label->setStyleSheet("");
                }
                cur_label = ui->label_compras;

                ui->label_compras->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: rgb(0, 0, 0);");
                ui->toolButton_compras->setFocus(Qt::TabFocusReason);
                return true;
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
                if(cur_label){
                    cur_label->setStyleSheet("");
                }
                cur_label = ui->label_productos;

                ui->label_productos->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: rgb(0, 0, 0);");
                ui->toolButton_productos->setFocus(Qt::TabFocusReason);
                return true;
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
                if(cur_label){
                    cur_label->setStyleSheet("");
                }
                cur_label = ui->label_colaboradores;

                ui->label_colaboradores->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: rgb(0, 0, 0);");
                ui->toolButton_colaboradores->setFocus(Qt::TabFocusReason);
                return true;
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
                if(cur_label){
                    cur_label->setStyleSheet("");
                }
                cur_label = ui->label_reportes;

                ui->label_reportes->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: rgb(0, 0, 0);");
                ui->toolButton_reportes->setFocus(Qt::TabFocusReason);
                return true;
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
                if(cur_label){
                    cur_label->setStyleSheet("");
                }
                cur_label = ui->label_salvados;

                ui->label_salvados->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: rgb(0, 0, 0);");
                ui->toolButton_salvados->setFocus(Qt::TabFocusReason);
                return true;
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
    w_temp = ui->toolButton_salvados;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Tab:{
                if(cur_label){
                    cur_label->setStyleSheet("");
                }
                cur_label = ui->label_configuracion;

                ui->label_configuracion->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: rgb(0, 0, 0);");
                ui->toolButton_configuracion->setFocus(Qt::TabFocusReason);
                return true;
            }break;
            case Qt::Key_Return:{
                ui->toolButton_salvados->click();
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
                QKeyEvent *event = new QKeyEvent ( QEvent::KeyPress, Qt::Key_Tab, Qt::NoModifier);
                QApplication::sendEvent(this, event);
                return true;
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
