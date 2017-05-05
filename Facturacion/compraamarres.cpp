#include "compraamarres.h"
#include "ui_compraamarres.h"

CompraAmarres::CompraAmarres(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CompraAmarres)
{
    ui->setupUi(this);

    afterShow = false;

    widget_previous = NULL;

    ui->tableWidget->hideColumn(0);
    ui->tableWidget->hideColumn(1);
    ui->tableWidget->hideColumn(2);

    this->installEventFilter(this);
    ui->pushButton_editar->installEventFilter(this);
    ui->tableWidget->installEventFilter(this);    
    ui->pushButton_salir->installEventFilter(this);
}
CompraAmarres::~CompraAmarres()
{
    qDebug()<<"delete compra amarres"<<endl;
    delete ui;
}
void CompraAmarres::set_widget_previous(QWidget *widget_previous)
{
    this->widget_previous = widget_previous;
}
void CompraAmarres::set_documento(QString id, int tipo)
{
    this->id = id;
    this->tipo = tipo;

    QString str_query = "CALL compra_amarres("+id+")";

    QSqlQuery query;
    qDebug()<<str_query<<endl;
    if(query.exec(str_query)){
        while(query.next()){
            QString id = query.value(0).toString();
            QString persona_id = query.value(1).toString();
            QString tipo_documento_id = query.value(2).toString();
            QString fecha_emision = query.value(3).toDate().toString("dd-MM-yyyy");
            QString serie = query.value(4).toString();
            QString numero = query.value(5).toString();
            QString codigo = query.value(6).toString();
            QString nombre = query.value(7).toString();

            int rowCount = ui->tableWidget->rowCount();
            ui->tableWidget->setRowCount(rowCount+1);

            ui->tableWidget->setItem(rowCount, 0, new QTableWidgetItem(id));
            ui->tableWidget->setItem(rowCount, 1, new QTableWidgetItem(persona_id));
            ui->tableWidget->setItem(rowCount, 2, new QTableWidgetItem(tipo_documento_id));
            QString str_tipo;
            if(tipo_documento_id.toInt() == tipo_documento::ORDEN)str_tipo = "Orden";
            if(tipo_documento_id.toInt() == tipo_documento::GUIA_REMISION_REMITENTE)str_tipo = "Guía";
            if(tipo_documento_id.toInt() == tipo_documento::BOLETA)str_tipo = "Boleta";
            if(tipo_documento_id.toInt() == tipo_documento::FACTURA)str_tipo = "Factura";
            if(tipo_documento_id.toInt() == tipo_documento::FLETE)str_tipo = "Flete";
            if(tipo_documento_id.toInt() == tipo_documento::NOTA_CREDITO)str_tipo = "N. Crédito";
            if(tipo_documento_id.toInt() == tipo_documento::NOTA_DEBITO)str_tipo = "N. Débito";
            ui->tableWidget->setItem(rowCount, 3, new QTableWidgetItem(str_tipo));
            ui->tableWidget->setItem(rowCount, 4, new QTableWidgetItem(fecha_emision));
            /*
            QFont font;
            font.setBold(true);            
            ui->tableWidget->item(rowCount, 1)->setFont(font);
            */
            ui->tableWidget->setItem(rowCount, 5, new QTableWidgetItem(serie));
            ui->tableWidget->setItem(rowCount, 6, new QTableWidgetItem(numero));
            ui->tableWidget->setItem(rowCount, 7, new QTableWidgetItem(codigo));
            ui->tableWidget->setItem(rowCount, 8, new QTableWidgetItem(nombre));

            for(int j=0; j<ui->tableWidget->columnCount(); j++)
                ui->tableWidget->item(rowCount, j)->setFlags(Qt::ItemIsEnabled
                                                             | Qt::ItemIsSelectable);
        }
        SYSTEM->table_resize_to_contents(0, ui->tableWidget, 10);

        ui->tableWidget->setFocus(Qt::TabFocusReason);
        ui->tableWidget->selectRow(0);
    }else{

    }
}
void CompraAmarres::on_compra_closing()
{
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);

    set_documento(id, tipo);
}
void CompraAmarres::on_pushButton_editar_clicked()
{
    QTableWidgetItem* item = ui->tableWidget->currentItem();
    editarItem(item);
}
void CompraAmarres::editarItem(QTableWidgetItem *item)
{
    //QTableWidget* tb = ui->tableWidget;

    int ret = QMessageBox::warning(this, "Advertencia", "¿Desea MODIFICAR los datos de este DOCUMENTO?", "Si", "No");
    switch(ret){
    case 0:{
        int row = item->row();
        QString id = ui->tableWidget->item(row, 0)->text();
        QString persona_id = ui->tableWidget->item(row, 1)->text();
        int tipo_documento_id = ui->tableWidget->item(row, 2)->text().toInt();
        QString fecha_emision = ui->tableWidget->item(row, 4)->text();
        QString serie = ui->tableWidget->item(row, 5)->text();
        QString numero = ui->tableWidget->item(row, 6)->text();
        QString codigo = ui->tableWidget->item(row, 7)->text();
        QString nombre = ui->tableWidget->item(row, 8)->text();

        switch(tipo_documento_id){
        case tipo_documento::ORDEN:{
            CompraOrden* w = new CompraOrden;
            w->set_widget_previous(this);
            w->select(id, persona_id, fecha_emision, serie, numero, codigo, nombre);
            SYSTEM->change_center_w(this, w);
        }break;
        case tipo_documento::BOLETA:{
            CompraBoleta* w = new CompraBoleta;
            w->set_widget_previous(this);
            w->select(id, persona_id, fecha_emision, serie, numero, codigo, nombre);
            SYSTEM->change_center_w(this, w);
        }break;
        case tipo_documento::FACTURA:{
            CompraFactura* w = new CompraFactura;
            w->set_widget_previous(this);
            w->select(id, persona_id, fecha_emision, serie, numero, codigo, nombre);
            SYSTEM->change_center_w(this, w);
        }break;
        case tipo_documento::GUIA_REMISION_REMITENTE:{
            CompraGuiaRR* w = new CompraGuiaRR;
            w->set_widget_previous(this);
            w->select(id, persona_id, fecha_emision, serie, numero, codigo, nombre);
            SYSTEM->change_center_w(this, w);
        }break;
        case tipo_documento::FLETE:{
            CompraFlete* w = new CompraFlete;
            w->set_widget_previous(this);
            w->select(id, persona_id, fecha_emision, serie, numero, codigo, nombre);
            SYSTEM->change_center_w(this, w);
        }break;
        case tipo_documento::NOTA_CREDITO:{
            CompraNotaCredito* w = new CompraNotaCredito;
            w->set_widget_previous(this);
            w->select(id, persona_id, fecha_emision, serie, numero, codigo, nombre);
            SYSTEM->change_center_w(this, w);
        }break;
        case tipo_documento::NOTA_DEBITO:{
            CompraNotaDebito* w = new CompraNotaDebito;
            w->set_widget_previous(this);
            w->select(id, persona_id, fecha_emision, serie, numero, codigo, nombre);
            SYSTEM->change_center_w(this, w);
        }break;
        }
    }break;
    case 1:{

    }break;
    }
}
void CompraAmarres::on_tableWidget_itemDoubleClicked(QTableWidgetItem *item)
{
    editarItem(item);
}
void CompraAmarres::on_pushButton_salir_clicked()
{
    if(widget_previous){
        setAttribute(Qt::WA_DeleteOnClose);
        SYSTEM->change_center_w(this, widget_previous);
    }else{
        SYSTEM->clear_center_w(this);
    }
}
void CompraAmarres::showEvent(QShowEvent *se)
{
    se->accept();

    afterShow = true;
}
void CompraAmarres::closeEvent(QCloseEvent *ce)
{
    ce->accept();
    emit closing();
}
bool CompraAmarres::eventFilter(QObject *obj, QEvent *e)
{
    QWidget* w_temp;
    w_temp = this;
    if(obj == w_temp){
        if(e->type() == QEvent::MouseButtonPress){
            if(focusWidget()){
                focusWidget()->setFocus();
            }else{
                ui->tableWidget->setFocus();
                ui->tableWidget->selectRow(0);
                for(int i=0; i<ui->tableWidget->columnCount(); i++)
                    ui->tableWidget->item(0, i)->setSelected(true);
            }
            return true;
        }
        if(e->type() == QEvent::MouseButtonDblClick){
            if(focusWidget()){
                focusWidget()->setFocus();
            }
            return true;
        }
        if(e->type() == QEvent::Paint){
            if(afterShow) {
                if(focusWidget()){
                    focusWidget()->setFocus();
                }else{
                    ui->tableWidget->setFocus();
                    ui->tableWidget->selectRow(0);
                    for(int i=0; i<ui->tableWidget->columnCount(); i++)
                        ui->tableWidget->item(0, i)->setSelected(true);
                }
                afterShow = false;
            }
            return true;
        }
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;
            switch(KeyEvent->key())
            {
            case Qt::Key_Escape:{
                ui->pushButton_salir->click();
                return true;
            }break;
            }
        }
        return false;
    }

    w_temp = ui->pushButton_editar;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:{
                ui->pushButton_editar->click();
                return true;
            }break;
            case Qt::Key_Enter:{
                QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QApplication::sendEvent(w_temp, key);
                return true;
            }break;
            }
        }
        return false;
    }

    w_temp = ui->tableWidget;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:{
                if(QString(typeid(*QApplication::focusWidget()).name()) == "18QExpandingLineEdit"){
                    QApplication::focusWidget()->parentWidget()->setFocus();
                }else{
                    //ui->pushButton_modificar->setFocus(Qt::TabFocusReason);
                    //ui->pushButton_modificar->click();
                    return true;
                }
            }break;
            case Qt::Key_Enter:{
                QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QApplication::sendEvent(w_temp, key);
                return true;
            }break;
            }
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
            case Qt::Key_Enter:{
                QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QApplication::sendEvent(w_temp, key);
                return true;
            }break;
            }
        }
        return false;
    }
    return eventFilter(obj, e);
}
