#include "compracanjear.h"
#include "ui_compracanjear.h"

CompraCanjear::CompraCanjear(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CompraCanjear)
{
    ui->setupUi(this);

    widget_previous = NULL;

    afterShow = false;

    this->installEventFilter(this);
    ui->tableWidget->installEventFilter(this);
    ui->pushButton_ok->installEventFilter(this);
    ui->pushButton_salir->installEventFilter(this);
}

CompraCanjear::~CompraCanjear()
{
    delete ui;
}
void CompraCanjear::set_widget_previous(QWidget *widget_previous)
{
    this->widget_previous = widget_previous;
}
void CompraCanjear::set_data(int id_tipo, QString id
                             , QString codigo, QString nombre)
{
    this->id_tipo_sender = id_tipo;
    this->id_sender = id;
    this->codigo_sender = codigo;
    this->nombre_sender = nombre;

    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"id"<<"documento");
    switch(id_tipo){
    case compra_items::ORDEN:{
        int rowCount = ui->tableWidget->rowCount();
        ui->tableWidget->setRowCount(rowCount+1);
        ui->tableWidget->setItem(rowCount, 0, new QTableWidgetItem(QString().setNum(compra_items::BOLETA)));
        ui->tableWidget->setItem(rowCount, 1, new QTableWidgetItem("BOLETA"));

        rowCount = ui->tableWidget->rowCount();
        ui->tableWidget->setRowCount(rowCount+1);
        ui->tableWidget->setItem(rowCount, 0, new QTableWidgetItem(QString().setNum(compra_items::FACTURA)));
        ui->tableWidget->setItem(rowCount, 1, new QTableWidgetItem("FACTURA"));

        rowCount = ui->tableWidget->rowCount();
        ui->tableWidget->setRowCount(rowCount+1);
        ui->tableWidget->setItem(rowCount, 0, new QTableWidgetItem(QString().setNum(compra_items::GUIA_REMISION_REMITENTE)));
        ui->tableWidget->setItem(rowCount, 1, new QTableWidgetItem("GUIA"));
    }break;
    case compra_items::BOLETA:{
        int rowCount = ui->tableWidget->rowCount();
        ui->tableWidget->setRowCount(rowCount+1);
        ui->tableWidget->setItem(rowCount, 0, new QTableWidgetItem(QString().setNum(compra_items::BOLETA)));
        ui->tableWidget->setItem(rowCount, 1, new QTableWidgetItem("BOLETA"));

        rowCount = ui->tableWidget->rowCount();
        ui->tableWidget->setRowCount(rowCount+1);
        ui->tableWidget->setItem(rowCount, 0, new QTableWidgetItem(QString().setNum(compra_items::FACTURA)));
        ui->tableWidget->setItem(rowCount, 1, new QTableWidgetItem("FACTURA"));

        rowCount = ui->tableWidget->rowCount();
        ui->tableWidget->setRowCount(rowCount+1);
        ui->tableWidget->setItem(rowCount, 0, new QTableWidgetItem(QString().setNum(compra_items::GUIA_REMISION_REMITENTE)));
        ui->tableWidget->setItem(rowCount, 1, new QTableWidgetItem("GUIA"));

        rowCount = ui->tableWidget->rowCount();
        ui->tableWidget->setRowCount(rowCount+1);
        ui->tableWidget->setItem(rowCount, 0, new QTableWidgetItem(QString().setNum(compra_items::FLETE)));
        ui->tableWidget->setItem(rowCount, 1, new QTableWidgetItem("FLETE"));

        rowCount = ui->tableWidget->rowCount();
        ui->tableWidget->setRowCount(rowCount+1);
        ui->tableWidget->setItem(rowCount, 0, new QTableWidgetItem(QString().setNum(compra_items::NOTA_CREDITO)));
        ui->tableWidget->setItem(rowCount, 1, new QTableWidgetItem("NOTA DE CREDITO"));

        rowCount = ui->tableWidget->rowCount();
        ui->tableWidget->setRowCount(rowCount+1);
        ui->tableWidget->setItem(rowCount, 0, new QTableWidgetItem(QString().setNum(compra_items::NOTA_DEBITO)));
        ui->tableWidget->setItem(rowCount, 1, new QTableWidgetItem("NOTA DE DEBITO"));
    }break;
    case compra_items::FACTURA:{
        int rowCount = ui->tableWidget->rowCount();
        ui->tableWidget->setRowCount(rowCount+1);
        ui->tableWidget->setItem(rowCount, 0, new QTableWidgetItem(QString().setNum(compra_items::BOLETA)));
        ui->tableWidget->setItem(rowCount, 1, new QTableWidgetItem("BOLETA"));

        rowCount = ui->tableWidget->rowCount();
        ui->tableWidget->setRowCount(rowCount+1);
        ui->tableWidget->setItem(rowCount, 0, new QTableWidgetItem(QString().setNum(compra_items::FACTURA)));
        ui->tableWidget->setItem(rowCount, 1, new QTableWidgetItem("FACTURA"));

        rowCount = ui->tableWidget->rowCount();
        ui->tableWidget->setRowCount(rowCount+1);
        ui->tableWidget->setItem(rowCount, 0, new QTableWidgetItem(QString().setNum(compra_items::GUIA_REMISION_REMITENTE)));
        ui->tableWidget->setItem(rowCount, 1, new QTableWidgetItem("GUIA"));

        rowCount = ui->tableWidget->rowCount();
        ui->tableWidget->setRowCount(rowCount+1);
        ui->tableWidget->setItem(rowCount, 0, new QTableWidgetItem(QString().setNum(compra_items::FLETE)));
        ui->tableWidget->setItem(rowCount, 1, new QTableWidgetItem("FLETE"));

        rowCount = ui->tableWidget->rowCount();
        ui->tableWidget->setRowCount(rowCount+1);
        ui->tableWidget->setItem(rowCount, 0, new QTableWidgetItem(QString().setNum(compra_items::NOTA_CREDITO)));
        ui->tableWidget->setItem(rowCount, 1, new QTableWidgetItem("NOTA DE CREDITO"));

        rowCount = ui->tableWidget->rowCount();
        ui->tableWidget->setRowCount(rowCount+1);
        ui->tableWidget->setItem(rowCount, 0, new QTableWidgetItem(QString().setNum(compra_items::NOTA_DEBITO)));
        ui->tableWidget->setItem(rowCount, 1, new QTableWidgetItem("NOTA DE DEBITO"));
    }break;
    case compra_items::FLETE:{

    }break;
    case compra_items::GUIA_REMISION_REMITENTE:{
        int rowCount = ui->tableWidget->rowCount();
        ui->tableWidget->setRowCount(rowCount+1);
        ui->tableWidget->setItem(rowCount, 0, new QTableWidgetItem(QString().setNum(compra_items::BOLETA)));
        ui->tableWidget->setItem(rowCount, 1, new QTableWidgetItem("BOLETA"));

        rowCount = ui->tableWidget->rowCount();
        ui->tableWidget->setRowCount(rowCount+1);
        ui->tableWidget->setItem(rowCount, 0, new QTableWidgetItem(QString().setNum(compra_items::FACTURA)));
        ui->tableWidget->setItem(rowCount, 1, new QTableWidgetItem("FACTURA"));
    }break;
    case compra_items::NOTA_CREDITO:{

    }break;
    case compra_items::NOTA_DEBITO:{

    }break;
    case compra_items::REG_SIN_DOC:{

    }break;
    case compra_items::SALDO:{

    }break;
    }
    for(int i=0; i<ui->tableWidget->rowCount(); i++){
        for(int j=0; j<ui->tableWidget->columnCount(); j++){
            ui->tableWidget->item(i, j)->setFlags(Qt::ItemIsSelectable
                                                | Qt::ItemIsEnabled);
        }
    }
    SYSTEM->table_resize_to_contents(0, ui->tableWidget);
}

void CompraCanjear::on_formDocumento_closing()
{
    id_tipo_recipient = -1;
    id_recipient = "";
    codigo_recipient = "";
    nombre_recipient = "";
}
void CompraCanjear::on_pushButton_ok_clicked()
{
    QTableWidgetItem* item = ui->tableWidget->currentItem();
    if(!item)return;

    if(widget_previous){
        switch(id_tipo_sender){
        case compra_items::ORDEN:{

        }break;
        case compra_items::BOLETA:{

        }break;
        case compra_items::FACTURA:{

        }break;
        case compra_items::FLETE:{

        }break;
        case compra_items::GUIA_REMISION_REMITENTE:{

        }break;
        case compra_items::NOTA_CREDITO:{

        }break;
        case compra_items::NOTA_DEBITO:{

        }break;
        case compra_items::REG_SIN_DOC:{

        }break;
        case compra_items::SALDO:{

        }break;
        }

        SYSTEM->change_center_w(this, widget_previous);
    }else{
        SYSTEM->clear_center_w(this);
    }
}

void CompraCanjear::on_pushButton_salir_clicked()
{
    if(widget_previous){
        SYSTEM->change_center_w(this, widget_previous);
    }else{
        SYSTEM->clear_center_w(this);
    }
}

void CompraCanjear::on_tableWidget_itemDoubleClicked(QTableWidgetItem *item)
{
    item = 0;
    on_pushButton_ok_clicked();
}
void CompraCanjear::showEvent(QShowEvent *event)
{
    afterShow = true;
}

bool CompraCanjear::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == this){
        if(event->type() == QEvent::MouseButtonPress){
            if(focusWidget()){
                focusWidget()->setFocus();
            }else{
                ui->tableWidget->setFocus();
            }
            return true;
        }
        if(event->type() == QEvent::MouseButtonDblClick){
            if(focusWidget()){
                focusWidget()->setFocus();
            }
            return true;
        }
        if(event->type() == QEvent::Paint){
            if(afterShow) {
                if(focusWidget()){
                    focusWidget()->setFocus();
                }else{
                    ui->tableWidget->setFocus();
                }
                afterShow = false;
            }
            return true;
        }
        if(event->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)event;
            switch(KeyEvent->key())
            {
            case Qt::Key_Escape:{
                ui->pushButton_salir->click();
                return true;
            }break;
            case Qt::Key_Up: {
                if (this->focusWidget() != ui->tableWidget) {
                    ui->tableWidget->setFocus(Qt::TabFocusReason);
                    if (ui->tableWidget->currentItem())
                        ui->tableWidget->currentItem()->setSelected(true);
                }
            }break;
            case Qt::Key_Down: {
                if (this->focusWidget() != ui->tableWidget) {
                    ui->tableWidget->setFocus(Qt::TabFocusReason);
                    if (ui->tableWidget->currentItem())
                        ui->tableWidget->currentItem()->setSelected(true);
                }
            }break;
            }
        }
        return false;
    }
    if(watched == ui->tableWidget){
        if(event->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)event;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:
                ui->pushButton_ok->click();
                return true;
            case Qt::Key_Enter:{
                QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QApplication::sendEvent(watched, key);
                return true;
            }break;
            }

        }else{

        }
        return false;
    }
    if(watched == ui->pushButton_ok){
        if(event->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)event;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:
                ui->pushButton_ok->click();
                return true;
            case Qt::Key_Enter:{
                QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QApplication::sendEvent(watched, key);
                return true;
            }break;
            }

        }else{

        }
        return false;
    }
    if(watched == ui->pushButton_salir){
        if(event->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)event;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:
                ui->pushButton_salir->click();
                return true;
            case Qt::Key_Enter:{
                QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QApplication::sendEvent(watched, key);
                return true;
            }break;
            }

        }else{

        }
        return false;
    }
    return eventFilter(watched, event);
}
