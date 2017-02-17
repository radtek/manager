#include "ventasnselector.h"
#include "ui_ventasnselector.h"

VentaSNSelector::VentaSNSelector(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VentaSNSelector)
{
    ui->setupUi(this);

    pos = 0;
    size_query = 10;

    this->installEventFilter(this);
    ui->tableWidget->installEventFilter(this);
    ui->pushButton_ultimo->installEventFilter(this);
    ui->pushButton_ok->installEventFilter(this);
    ui->pushButton_salir->installEventFilter(this);
}

VentaSNSelector::~VentaSNSelector()
{
    delete ui;
}
void VentaSNSelector::set_widget_previous(QWidget* widget_previous)
{
    this->widget_previous = widget_previous;
}

QString VentaSNSelector::getSerie()
{
    return serie;
}
QString VentaSNSelector::getNumero()
{
    return numero;
}
void VentaSNSelector::set_tipo(int tipo)
{
    this->tipo = tipo;

    QString str_query;
    switch(tipo){
    case venta_items::REGISTRO_SIN_DOCUMENTO:{
        str_query = "SELECT anexo.serie, anexo.numero+1 FROM registro_sin_documento";
        str_query += " JOIN anexo ON anexo.documento_id = registro_sin_documento.comprobante_documento_id";
        str_query += " GROUP BY anexo.serie";
        str_query += " ORDER BY registro_sin_documento.comprobante_documento_id DESC, anexo.numero DESC";
        str_query += " LIMIT "+QString().setNum(pos)+", "+QString().setNum(size_query);

    }break;
    case venta_items::BOLETA:{
        str_query = "SELECT anexo.serie, anexo.numero+1 FROM boleta";
        str_query += " JOIN anexo ON anexo.documento_id = boleta.comprobante_documento_id";
        str_query += " GROUP BY anexo.serie";
        str_query += " ORDER BY boleta.comprobante_documento_id DESC, anexo.numero DESC";
        str_query += " LIMIT "+QString().setNum(pos)+", "+QString().setNum(size_query);
    }break;
    case venta_items::FACTURA:{
        str_query = "SELECT anexo.serie, anexo.numero+1 FROM factura";
        str_query += " JOIN anexo ON anexo.documento_id = factura.comprobante_documento_id";
        str_query += " GROUP BY anexo.serie";
        str_query += " ORDER BY factura.comprobante_documento_id DESC, anexo.numero DESC";
        str_query += " LIMIT "+QString().setNum(pos)+", "+QString().setNum(size_query);
    }break;
    case venta_items::NOTA_PEDIDO:{
        str_query = "SELECT anexo.serie, anexo.numero+1 FROM nota_pedido";
        str_query += " JOIN anexo ON anexo.documento_id = nota_pedido.comprobante_documento_id";
        str_query += " GROUP BY anexo.serie";
        str_query += " ORDER BY nota_pedido.comprobante_documento_id DESC, anexo.numero DESC";
        str_query += " LIMIT "+QString().setNum(pos)+", "+QString().setNum(size_query);
    }break;
    case venta_items::GUIA_REMISION_REMITENTE:{
        str_query = "SELECT anexo.serie, anexo.numero+1 FROM guia_remision_remitente";
        str_query += " JOIN anexo ON anexo.documento_id = guia_remision_remitente.comprobante_documento_id";
        str_query += " GROUP BY anexo.serie";
        str_query += " ORDER BY guia_remision_remitente.comprobante_documento_id DESC, anexo.numero DESC";
        str_query += " LIMIT "+QString().setNum(pos)+", "+QString().setNum(size_query);
    }break;
    case venta_items::COTIZACION:{
        str_query = "SELECT anexo.serie, anexo.numero+1 FROM cotizacion";
        str_query += " JOIN anexo ON anexo.documento_id = cotizacion.comprobante_documento_id";
        str_query += " GROUP BY anexo.serie";
        str_query += " ORDER BY cotizacion.comprobante_documento_id DESC, anexo.numero DESC";
        str_query += " LIMIT "+QString().setNum(pos)+", "+QString().setNum(size_query);
    }break;
    case venta_items::NOTA_CREDITO:{
        str_query = "SELECT anexo.serie, anexo.numero+1 FROM nota_credito";
        str_query += " JOIN anexo ON anexo.documento_id = nota_credito.comprobante_documento_id";
        str_query += " GROUP BY anexo.serie";
        str_query += " ORDER BY nota_credito.comprobante_documento_id DESC, anexo.numero DESC";
        str_query += " LIMIT "+QString().setNum(pos)+", "+QString().setNum(size_query);
    }break;
    case venta_items::NOTA_DEBITO:{
        str_query = "SELECT anexo.serie, anexo.numero+1 FROM nota_debito";
        str_query += " JOIN anexo ON anexo.documento_id = nota_debito.comprobante_documento_id";
        str_query += " GROUP BY anexo.serie";
        str_query += " ORDER BY nota_debito.comprobante_documento_id DESC, anexo.numero DESC";
        str_query += " LIMIT "+QString().setNum(pos)+", "+QString().setNum(size_query);
    }break;
    }
    QSqlQuery query;
    qDebug()<<str_query<<endl;
    if(query.exec(str_query)){
        while(query.next()){
            QString serie = query.value(0).toString();
            QString numero = query.value(1).toString();

            int nro_digits = A_Venta::SERIE_N_DIGITS;
            int count = serie.length();

            for(int i = 0; i < nro_digits-count; i++){
                serie.push_front('0');
            }

            nro_digits = A_Venta::NUMERO_N_DIGITS;
            count = numero.length();

            for(int i = 0; i < nro_digits-count; i++){
                numero.push_front('0');
            }
            ui->tableWidget->setRowCount(pos+1);
            ui->tableWidget->setItem(pos, 0, new QTableWidgetItem(serie));
            ui->tableWidget->setItem(pos, 1, new QTableWidgetItem(numero));
            pos++;
        }
    }else{

    }
}

void VentaSNSelector::on_pushButton_ok_clicked()
{
    QTableWidgetItem* item = ui->tableWidget->currentItem();

    if(item){
        serie = ui->tableWidget->item(item->row(), 0)->text();
        numero = ui->tableWidget->item(item->row(), 1)->text();
    }

    if(widget_previous){
        this->setAttribute(Qt::WA_DeleteOnClose);
        SYSTEM->change_center_w(this, widget_previous);
    }else{
        SYSTEM->clear_center_w(this);
    }
}

void VentaSNSelector::on_pushButton_ultimo_clicked()
{
    QTableWidgetItem* item = ui->tableWidget->item(0, 0);

    if(!item)return;

    serie = ui->tableWidget->item(item->row(), 0)->text();
    numero = ui->tableWidget->item(item->row(), 1)->text();

    if(widget_previous){
        this->setAttribute(Qt::WA_DeleteOnClose);
        SYSTEM->change_center_w(this, widget_previous);
    }else{
        SYSTEM->clear_center_w(this);
    }
}

void VentaSNSelector::on_pushButton_salir_clicked()
{
    int ret = QMessageBox::warning(this, "Advertencia", "¿Esta seguro que desea salir?", "Si", "No");
    switch(ret){
    case 0:{
        if(widget_previous){
            this->setAttribute(Qt::WA_DeleteOnClose);
            SYSTEM->change_center_w(this, widget_previous);
        }else{
            SYSTEM->clear_center_w(this);
        }
    }break;
    case 1:{

    }
    }
}

void VentaSNSelector::showEvent(QShowEvent *event)
{
    ui->tableWidget->setFocus(Qt::TabFocusReason);

    event->accept();
}

void VentaSNSelector::closeEvent(QCloseEvent *event)
{
    emit closing();
    event->accept();
}

bool VentaSNSelector::eventFilter(QObject *watched, QEvent *event)
{
    QWidget* w_temp;
    w_temp = this;
    if(watched == w_temp){
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

        }else{

        }
        return false;
    }
    w_temp = ui->tableWidget;
    if(watched == w_temp){
        if(event->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)event;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:{
                ui->pushButton_ok->setFocus(Qt::TabFocusReason);
                ui->pushButton_ok->click();
                return true;
            }break;
            case Qt::Key_Down: {
                int index = ui->tableWidget->currentRow();
                if (index == ui->tableWidget->rowCount() - 1) {
                    set_tipo(tipo);
                }
            }break;
            }
        }
        return false;
    }
    w_temp = ui->pushButton_ultimo;
    if(watched == w_temp){
        if(event->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)event;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:{
                ui->pushButton_ultimo->click();
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
