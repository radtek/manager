#include "compraadjacentes.h"
#include "ui_compraadjacentes.h"

CompraAdjacentes::CompraAdjacentes(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CompraAdjacentes)
{
    ui->setupUi(this);

    int columnCount = ui->tableWidget->columnCount();
    ui->tableWidget->hideColumn(columnCount-1);
    ui->tableWidget->hideColumn(columnCount-2);

    this->installEventFilter(this);
    ui->tableWidget->installEventFilter(this);
    ui->pushButton_orden->installEventFilter(this);
    ui->pushButton_guia->installEventFilter(this);
    ui->pushButton_factura->installEventFilter(this);
    ui->pushButton_flete->installEventFilter(this);
    ui->pushButton_ncredito->installEventFilter(this);
    ui->pushButton_ndebito->installEventFilter(this);
    ui->pushButton_modificar->installEventFilter(this);
    ui->pushButton_eliminar->installEventFilter(this);
    ui->pushButton_salir->installEventFilter(this);
}
CompraAdjacentes::~CompraAdjacentes()
{
    delete ui;
}
void CompraAdjacentes::set_widget_previous(QWidget *widget_previous)
{
    this->widget_previous = widget_previous;

    if(typeid(*widget_previous) == typeid(CompraOrden)){
        ui->pushButton_orden->setDisabled(true);
        ui->pushButton_flete->setDisabled(true);
        ui->pushButton_ncredito->setDisabled(true);
        ui->pushButton_ndebito->setDisabled(true);
    }
    if(typeid(*widget_previous) == typeid(CompraGuiaRR)){
        ui->pushButton_orden->setDisabled(true);
        ui->pushButton_guia->setDisabled(true);
        ui->pushButton_boleta->setDisabled(true);
        ui->pushButton_factura->setDisabled(true);
        ui->pushButton_flete->setDisabled(true);
        ui->pushButton_ncredito->setDisabled(true);
        ui->pushButton_ndebito->setDisabled(true);
    }
    if(typeid(*widget_previous) == typeid(CompraBoleta)){
        ui->pushButton_orden->setDisabled(true);
        ui->pushButton_guia->setDisabled(true);
        //ui->pushButton_factura->setDisabled(true);
    }
    if(typeid(*widget_previous) == typeid(CompraFactura)){
        ui->pushButton_orden->setDisabled(true);
        ui->pushButton_guia->setDisabled(true);
        //ui->pushButton_boleta->setDisabled(true);
    }
    if(typeid(*widget_previous) == typeid(CompraFlete)){
        ui->pushButton_orden->setDisabled(true);
        ui->pushButton_guia->setDisabled(true);
        ui->pushButton_boleta->setDisabled(true);
        ui->pushButton_factura->setDisabled(true);
        ui->pushButton_flete->setDisabled(true);
        ui->pushButton_ncredito->setDisabled(true);
        ui->pushButton_ndebito->setDisabled(true);
    }
    if(typeid(*widget_previous) == typeid(CompraNotaCredito)){
        ui->pushButton_orden->setDisabled(true);
        ui->pushButton_guia->setDisabled(true);
        ui->pushButton_boleta->setDisabled(true);
        ui->pushButton_factura->setDisabled(true);
        ui->pushButton_flete->setDisabled(true);
        ui->pushButton_ncredito->setDisabled(true);
        ui->pushButton_ndebito->setDisabled(true);
    }
    if(typeid(*widget_previous) == typeid(CompraNotaDebito)){
        ui->pushButton_orden->setDisabled(true);
        ui->pushButton_guia->setDisabled(true);
        ui->pushButton_boleta->setDisabled(true);
        ui->pushButton_factura->setDisabled(true);
        ui->pushButton_flete->setDisabled(true);
        ui->pushButton_ncredito->setDisabled(true);
        ui->pushButton_ndebito->setDisabled(true);
    }
}
void CompraAdjacentes::set_documento_id(const QString &id)
{
    this->documento_id = id;

    QString str_query = "CALL compra_adjacentes("+id+")";

    if(query.exec(str_query)){
        while(query.next()){
            QString fecha_emision = query.value(0).toString();
            QString serie = query.value(1).toString();
            QString numero = query.value(2).toString();
            QString codigo = query.value(3).toString();
            QString nombre = query.value(4).toString();
            QString tipo_item_id = query.value(5).toString();
            QString per_id = query.value(6).toString();
            QString id = query.value(7).toString();

            int rowCount = ui->tableWidget->rowCount();
            ui->tableWidget->setRowCount(rowCount+1);

            ui->tableWidget->setItem(rowCount, 0, new QTableWidgetItem(fecha_emision));

            QString str_tipo;
            if(tipo_item_id.toInt() == items::ORDEN)str_tipo = "Orden";
            if(tipo_item_id.toInt() == items::GUIA_REMISION_REMITENTE)str_tipo = "Guía";
            if(tipo_item_id.toInt() == items::FACTURA)str_tipo = "Factura";
            if(tipo_item_id.toInt() == items::FLETE)str_tipo = "Flete";
            if(tipo_item_id.toInt() == items::NOTA_CREDITO)str_tipo = "N. Crédito";
            if(tipo_item_id.toInt() == items::NOTA_DEBITO)str_tipo = "N. Débito";
            ui->tableWidget->setItem(rowCount, 1, new QTableWidgetItem(str_tipo));

            ui->tableWidget->setItem(rowCount, 2, new QTableWidgetItem(serie));
            ui->tableWidget->setItem(rowCount, 3, new QTableWidgetItem(numero));
            ui->tableWidget->setItem(rowCount, 4, new QTableWidgetItem(codigo));
            ui->tableWidget->setItem(rowCount, 5, new QTableWidgetItem(nombre));
            ui->tableWidget->setItem(rowCount, 6, new QTableWidgetItem(tipo_item_id));
            ui->tableWidget->setItem(rowCount, 7, new QTableWidgetItem(per_id));
            ui->tableWidget->setItem(rowCount, 8, new QTableWidgetItem(id));
        }
        SYSTEM->table_resize_to_contents(0, ui->tableWidget, 10);
        this->adjustSize();
        ui->tableWidget->setFocus(Qt::TabFocusReason);
        ui->tableWidget->selectRow(0);
    }else{

    }
}

void CompraAdjacentes::on_pushButton_orden_clicked()
{
    CompraOrden* w = new CompraOrden;
    w->set_widget_previous(this);

    //w->set_documento_id(documento_id);
    //w->set_tw_amarres(ui->tableWidget);

    SYSTEM->change_center_w(this, w);
}

void CompraAdjacentes::on_pushButton_guia_clicked()
{
    CompraGuiaRR* w = new CompraGuiaRR;
    w->set_widget_previous(this);

    //w->set_documento_id(documento_id);
    //w->set_tw_amarres(ui->tableWidget);

    SYSTEM->change_center_w(this, w);
}

void CompraAdjacentes::on_pushButton_boleta_clicked()
{
    CompraBoleta* w = new CompraBoleta;
    w->set_widget_previous(this);


    SYSTEM->change_center_w(this, w);
}

void CompraAdjacentes::on_pushButton_factura_clicked()
{
    CompraFactura* w = new CompraFactura;
    w->set_widget_previous(this);

    //w->set_documento_id(documento_id);
    //w->set_tw_amarres(ui->tableWidget);

    SYSTEM->change_center_w(this, w);
}
void CompraAdjacentes::on_pushButton_flete_clicked()
{
    QString msj = "¿Desea jalar el detalle de los documentos?";
    int ret = QMessageBox::warning(this, "Advertencia"
                                   , msj, "Ok", "Cancel");

    switch(ret){
    case 0:{
        QVector<QVector<QString> > data_widget_previous;
        QVector<QVector<QString> > vv_ids;

        if(typeid(*widget_previous) == typeid(CompraBoleta)){
            //data_widget_previous = ((CompraBoleta*)widget_previous)->get_data();
        }
        if(typeid(*widget_previous) == typeid(CompraFactura)){
            //data_widget_previous = ((CompraFactura*)widget_previous)->get_data();
        }

        query.seek(-1);
        int i = 0;
        for(int i = 0; i < ui->tableWidget->rowCount(); i++){
            QString codigo = ui->tableWidget->item(i, 4)->text();
            QString nombre = ui->tableWidget->item(i, 5)->text();
            QString tipo_item_id = ui->tableWidget->item(i, 6)->text();
            QString persona_id = ui->tableWidget->item(i, 7)->text();
            QString id = ui->tableWidget->item(i, 8)->text();

            if(tipo_item_id.toInt() == items::BOLETA || tipo_item_id.toInt() == items::FACTURA){
                ui->tableWidget->selectRow(i);
                QVector<QString> v;
                v<<codigo<<nombre<<persona_id<<id;
                if(tipo_item_id.toInt() == items::BOLETA){
                    CompraBoleta* w = new CompraBoleta;
                    //w->select(v);
                    SYSTEM->change_center_w(this, w);
                }
                if(tipo_item_id.toInt() == items::FACTURA){
                    CompraFactura* w = new CompraFactura;
                    //w->select(v);
                    SYSTEM->change_center_w(this, w);
                }

                QString msj = "¿Desea agregar el documento seleccionado al detalle?";
                int ret = QMessageBox::warning(this, "Advertencia"
                                               , msj, "Ok", "Cancel");

                switch(ret){
                case 0:{
                    vv_ids.push_back(QVector<QString>());
                    int size = vv_ids.size()-1;
                    vv_ids[size].push_back(tipo_item_id);
                    vv_ids[size].push_back(id);
                }break;
                case 1:{

                }
                }
            }
            ++i;
        }

        CompraFlete* w = new CompraFlete;
        w->set_widget_previous(this);

        //w->set_documento_id(documento_id);
        //w->set_tw_amarres(ui->tableWidget);

        //w->set_data(data_widget_previous, vv_ids);

        SYSTEM->change_center_w(this, w);
    }break;
    case 1:{
        CompraFlete* w = new CompraFlete;
        w->set_widget_previous(this);

        //w->set_documento_id(documento_id);
        //w->set_tw_amarres(ui->tableWidget);

        SYSTEM->change_center_w(this, w);
    }
    }
}
void CompraAdjacentes::on_pushButton_ncredito_clicked()
{
    CompraNotaCredito* w = new CompraNotaCredito;
    w->set_widget_previous(this);

    //w->set_documento_id(documento_id);
    //w->set_tw_amarres(ui->tableWidget);

    SYSTEM->change_center_w(this, w);
}

void CompraAdjacentes::on_pushButton_ndebito_clicked()
{

}

void CompraAdjacentes::on_pushButton_modificar_clicked()
{
    QTableWidget* tb = ui->tableWidget;
    QTableWidgetItem* item = tb->currentItem();

    if(!item){
        return;
    }
    int ret = QMessageBox::warning(this, "Advertencia", "¿Desea MODIFICAR los datos de este DOCUMENTO?", "Si", "No");
    switch(ret){
    case 0:{
        int row = item->row();

        query.seek(-1);
        int i = 0;
        int tipo;
        while(query.next()){
            if(row == i){
                tipo = query.value(1).toInt();
                break;
            }
            i++;
        }
        QVector<QString> v;
        for(int j= 0; j< tb->columnCount(); j++){
            QTableWidgetItem* item = tb->item(row, j);
            v.push_back(item->text());
        }
        QWidget* w;
        switch(tipo)
        {
        case items::ORDEN:{
            w = new CompraOrden;
            ((CompraOrden*)w)->set_widget_previous(this);
            //((CompraOrden*)w)->select(v);
        }break;
        case items::BOLETA:{
            w = new CompraBoleta;
            ((CompraBoleta*)w)->set_widget_previous(this);
            //((CompraBoleta*)w)->select(v);
        }break;
        case items::FACTURA:{
            w = new CompraFactura;
            ((CompraFactura*)w)->set_widget_previous(this);
            //((CompraFactura*)w)->select(v);
        }break;
        case items::GUIA_REMISION_REMITENTE:{
            w = new CompraGuiaRR;
            ((CompraGuiaRR*)w)->set_widget_previous(this);
            //((CompraGuiaRR*)w)->select(v);
        }break;
        case items::FLETE:{
            w = new CompraFlete;
            ((CompraFlete*)w)->set_widget_previous(this);
            //((CompraFlete*)w)->select(v);
        }break;
        case items::NOTA_CREDITO:{
            w = new CompraNotaCredito;
            ((CompraNotaCredito*)w)->set_widget_previous(this);
            //((CompraNotaCredito*)w)->select(v);
        }break;

        }
        SYSTEM->change_center_w(this, w);

    }break;
    case 1:{

    }break;
    }
}

void CompraAdjacentes::on_pushButton_eliminar_clicked()
{

}
void CompraAdjacentes::on_pushButton_salir_clicked()
{
    if(widget_previous){
        SYSTEM->change_center_w(this, widget_previous);
    }else{
        SYSTEM->clear_center_w(this);
    }
}
void CompraAdjacentes::showEvent(QShowEvent *se)
{
    se->accept();
    if(this->focusWidget()){
        this->focusWidget()->setFocus();
    }else{
        ui->pushButton_orden->setFocus(Qt::TabFocusReason);
    }
}
bool CompraAdjacentes::eventFilter(QObject *obj, QEvent *e)
{
    QWidget* w_temp;
    w_temp = this;
    if(obj == w_temp){
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
                    ui->pushButton_orden->setFocus(Qt::TabFocusReason);
                }
            }break;
            }
        }
        return false;
    }

    w_temp = ui->pushButton_orden;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:{
                ui->pushButton_orden->click();
            }break;
            }
        }
        return false;
    }

    w_temp = ui->pushButton_guia;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:{
                ui->pushButton_guia->click();
            }break;
            }
        }
        return false;
    }

    w_temp = ui->pushButton_factura;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:{
                ui->pushButton_factura->click();
            }break;
            }
        }
        return false;
    }

    w_temp = ui->pushButton_flete;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:{
                ui->pushButton_flete->click();
            }break;
            }
        }
        return false;
    }

    w_temp = ui->pushButton_ncredito;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:{
                ui->pushButton_ncredito->click();
            }break;
            }
        }
        return false;
    }

    w_temp = ui->pushButton_ndebito;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:{
                ui->pushButton_ndebito->click();
            }break;
            }
        }
        return false;
    }

    w_temp = ui->pushButton_modificar;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:{
                ui->pushButton_modificar->click();
            }break;
            }
        }
        return false;
    }

    w_temp = ui->pushButton_eliminar;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:{
                ui->pushButton_eliminar->click();
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
            }break;
            }
        }
        return false;
    }
    return eventFilter(obj, e);
}
