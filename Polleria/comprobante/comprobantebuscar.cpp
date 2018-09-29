#include "comprobantebuscar.h"
#include "ui_comprobantebuscar.h"
#include "xlsxdocument.h"
#include "xlsxrichstring.h"
#include "xlsxworkbook.h"
#include "xlsxformat.h"

#include <QtXml>
#include "SoapSunatCall.h"


ComprobanteBuscar::ComprobanteBuscar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ComprobanteBuscar)
{
    ui->setupUi(this);

    ui->dateEdit_exportar->setDate(QDate::currentDate());

    ui->tableWidget->setAlternatingRowColors(true);

    QDateTime dt_ini = QDateTime::currentDateTime();
    dt_ini.setTime(QTime(0, 0, 0));
    disconnect(ui->dateTimeEdit_ini, SIGNAL(dateTimeChanged(QDateTime)), this, SLOT(on_dateTimeEdit_ini_dateTimeChanged(QDateTime)));
    ui->dateTimeEdit_ini->setDateTime(dt_ini);
    connect(ui->dateTimeEdit_ini, SIGNAL(dateTimeChanged(QDateTime)), this, SLOT(on_dateTimeEdit_ini_dateTimeChanged(QDateTime)));
    QDateTime dt_fin = QDateTime::currentDateTime();
    dt_fin.setTime(QTime(23, 59, 59));
    disconnect(ui->dateTimeEdit_fin, SIGNAL(dateTimeChanged(QDateTime)), this, SLOT(on_dateTimeEdit_fin_dateTimeChanged(QDateTime)));
    ui->dateTimeEdit_fin->setDateTime(dt_fin);
    connect(ui->dateTimeEdit_fin, SIGNAL(dateTimeChanged(QDateTime)), this, SLOT(on_dateTimeEdit_fin_dateTimeChanged(QDateTime)));
    firstShow = false;
    afterShow = false;

    pos = 0;
    size_query = 10;

    cod = "";
    nombre = "";
    direccion = "";

    ui->tableWidget->setColumnWidth(0, 0);
    ui->tableWidget->setColumnWidth(1, 100);
    ui->tableWidget->setColumnWidth(2, 100);
    ui->tableWidget->setColumnWidth(3, 300);
    ui->tableWidget->setColumnWidth(4, 100);
    ui->tableWidget->setColumnWidth(5, 500);
    ui->tableWidget->setColumnWidth(6, 100);
    ui->tableWidget->setColumnWidth(7, 180);

/*
    disconnect(ui->lineEdit_codigo, SIGNAL(returnPressed())
                , this, SLOT(on_lineEdit_codigo_returnPressed()));
    disconnect(ui->lineEdit_descripcion_buscar, SIGNAL(returnPressed())
                , this, SLOT(on_lineEdit_descripcion_buscar_returnPressed()));
    disconnect(ui->lineEdit_marca_buscar, SIGNAL(returnPressed())
                , this, SLOT(on_lineEdit_marca_buscar_returnPressed()));
    disconnect(ui->lineEdit_tipo_buscar, SIGNAL(returnPressed())
                , this, SLOT(on_lineEdit_tipo_buscar_returnPressed()));
    disconnect(ui->lineEdit_unidad_buscar, SIGNAL(returnPressed())
                , this, SLOT(on_lineEdit_unidad_buscar_returnPressed()));
*/
    QScrollBar* bar = ui->tableWidget->verticalScrollBar();
    connect(bar, SIGNAL(valueChanged(int)), this, SLOT(on_verticalScrollBar_valueChanged(int)));
    connect(bar, SIGNAL(actionTriggered(int)), this, SLOT(on_verticalScrollBar_actionTriggered(int)));

    ui->tableWidget->hideColumn(0);
    ui->tableWidget->hideColumn(8);
    ui->tableWidget->hideColumn(9);

    this->installEventFilter(this);

    ui->tableWidget->installEventFilter(this);
    ui->lineEdit_buscar->installEventFilter(this);
    ui->pushButton_salir->installEventFilter(this);
}

ComprobanteBuscar::~ComprobanteBuscar()
{
    qDebug()<<"delete cliente"<<endl;
    delete ui;
}

void ComprobanteBuscar::on_verticalScrollBar_actionTriggered(int value)
{
    QScrollBar* bar = ui->tableWidget->verticalScrollBar();

    /*
    qDebug()<<"activation value: "<<value<<endl;
    qDebug()<<"activation bar maximum: "<<bar->maximum()<<endl;
    qDebug()<<"activation bar value: "<<bar->value()<<endl;
    */
    if(bar->value() == bar->maximum()) {
        set_buscar();
    }
}
void ComprobanteBuscar::on_verticalScrollBar_valueChanged(int value)
{
    /*
    QScrollBar* bar = ui->tableWidget->verticalScrollBar();

    if(value == bar->maximum())
        set_buscar();
*/
}

void ComprobanteBuscar::on_tableWidget_itemDoubleClicked(QTableWidgetItem *item)
{
    //if(widget_previous){
        //on_pushButton_ingresar_clicked();
    //}else{
        //editarItem(item);
    //}

}

void ComprobanteBuscar::on_pushButton_salir_clicked()
{
    cod = "";
    this->close();
}
void ComprobanteBuscar::set_buscar()
{
    QString nombre = ui->lineEdit_buscar->text();
    nombre = nombre.trimmed();

    QString producto = ui->lineEdit_buscarProducto->text();
    producto = producto.trimmed();

    QString fecha_ini = ui->dateTimeEdit_ini->dateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString fecha_fin = ui->dateTimeEdit_fin->dateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString check_efectivo, check_visa, check_masterCard;
    QString check_compra, check_venta, check_egresos;
    QString check_boleta, check_factura, check_pre_venta, check_ncredito;
    if(ui->checkBox_compra->isChecked()){
        check_compra = COMPRA;
    }
    if(ui->checkBox_venta->isChecked()){
        check_venta = VENTA;
    }
    if(ui->checkBox_egresos->isChecked()){
        check_egresos = EGRESOS;
    }
    if(ui->checkBox_efectivo->isChecked()){
        check_efectivo = EFECTIVO;
    }
    if(ui->checkBox_visa->isChecked()){
        check_visa = VISA;
    }
    if(ui->checkBox_masterCard->isChecked()){
        check_masterCard = MASTER_CARD;
    }
    if(ui->checkBox_boleta->isChecked()){
        check_boleta = BOLETA;
    }
    if(ui->checkBox_factura->isChecked()){
        check_factura = FACTURA;
    }
    if(ui->checkBox_ticket->isChecked()){
        check_pre_venta = TICKET;
    }
    if(ui->checkBox_ncredito->isChecked()){
        check_ncredito = NCREDITO;
    }
    QString str_query = "SELECT c.id, c.estado_item_nombre, CONCAT(c.serie,'-',c.numero), GROUP_CONCAT(concat(c_h_prod.cantidad, ' | ', prod.nombre) SEPARATOR '\\n'), SUM(c_h_prod.precio)";
    str_query += ", c.nombre, c.pago_item_nombre, c.fecha_emision, c.operacion_item_nombre, c.tipo_item_nombre";
    str_query += " FROM comprobante c";
    str_query += " LEFT JOIN comprobante_has_persona c_h_per ON c.id = c_h_per.comprobante_id";
    str_query += " LEFT JOIN persona per ON c_h_per.persona_cod = per.cod";
    str_query += " JOIN comprobante_has_producto c_h_prod ON c.id = c_h_prod.comprobante_id";
    str_query += " JOIN producto prod ON prod.id = c_h_prod.producto_id";
    str_query += " WHERE c.nombre LIKE '%" + nombre + "%'";
    str_query += " AND prod.nombre LIKE '%" + producto + "%'";
    str_query += " AND c.fecha_emision BETWEEN '" + fecha_ini + "' AND '" + fecha_fin + "'";
    //str_query += " AND c.estado_item_nombre <> 'Suspendido'";
    QString operacion_item = "";
    if(check_compra.compare("") != 0){
        operacion_item += "c.operacion_item_nombre = '" + check_compra + "'";
    }

    if(check_venta.compare("") != 0){
        if(operacion_item.compare("") != 0){
            operacion_item += " OR c.operacion_item_nombre = '" + check_venta + "'";
        }else{
            operacion_item += "c.operacion_item_nombre = '" + check_venta + "'";
        }
    }
    if(check_egresos.compare("") != 0){
        if(operacion_item.compare("") != 0){
            operacion_item += " OR c.operacion_item_nombre = '" + check_egresos + "'";
        }else{
            operacion_item += "c.operacion_item_nombre = '" + check_egresos + "'";
        }
    }
    if(operacion_item.compare("") != 0){
        str_query += " AND (" + operacion_item + ")";
    }

    QString pago_item = "";
    if(check_efectivo.compare("") != 0){
        pago_item += "c.pago_item_nombre = '" + check_efectivo + "'";
    }

    if(check_visa.compare("") != 0){
        if(pago_item.compare("") != 0){
            pago_item += " OR c.pago_item_nombre = '" + check_visa + "'";
        }else{
            pago_item += "c.pago_item_nombre = '" + check_visa + "'";
        }
    }
    if(check_masterCard.compare("") != 0){
        if(pago_item.compare("") != 0){
            pago_item += " OR c.pago_item_nombre = '" + check_masterCard + "'";
        }else{
            pago_item += "c.pago_item_nombre = '" + check_masterCard + "'";
        }
    }
    if(pago_item.compare("") != 0){
        str_query += " AND (" + pago_item + ")";
    }

    QString tipo_item = "";
    if(check_boleta.compare("") != 0){
        tipo_item += "c.tipo_item_nombre = '" + check_boleta + "'";
    }

    if(check_factura.compare("") != 0){
        if(tipo_item.compare("") != 0){
            tipo_item += " OR c.tipo_item_nombre = '" + check_factura + "'";
        }else{
            tipo_item += "c.tipo_item_nombre = '" + check_factura + "'";
        }
    }
    if(check_pre_venta.compare("") != 0){
        if(tipo_item.compare("") != 0){
            tipo_item += " OR c.tipo_item_nombre = '" + check_pre_venta + "'";
        }else{
            tipo_item += "c.tipo_item_nombre = '" + check_pre_venta + "'";
        }
    }
    if(check_ncredito.compare("") != 0){
        if(tipo_item.compare("") != 0){
            tipo_item += " OR c.tipo_item_nombre = '" + check_ncredito + "'";
        }else{
            tipo_item += "c.tipo_item_nombre = '" + check_ncredito + "'";
        }
    }
    if(tipo_item.compare("") != 0){
        str_query += " AND (" + tipo_item + ")";
    }
    str_query += " GROUP BY c.id";
    str_query += " ORDER BY c.numero DESC, c.fecha_emision DESC, c.nombre ASC";
    str_query += " LIMIT " + QString().setNum(pos) + ", " + QString().setNum(size_query) + "";

    QSqlQuery query;
    qDebug() << str_query << endl;
    if (query.exec(str_query)) {
        int rowCount = ui->tableWidget->rowCount();
        ui->tableWidget->setRowCount(rowCount + query.size());
        //ui->tableWidget->verticalHeader();
        //int columnCount = query.record().count();
        //ui->tableWidget->setColumnCount(columnCount);

        //ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "ID" << "Estado" << "Numero" << "Descripción"
        //                                           << "Total" << "Nombre" << "Pago" << "Fecha");


        //ui->tableWidget->setColumnWidth(8, 150);
        while (query.next()) {
            ui->tableWidget->setRowHeight(pos, 50);

            QString id = query.value(0).toString();
            QString estado = query.value(1).toString();
            QString s_nro = query.value(2).toString();
            //int n = 7 - nro_ticket.length();
            //nro_ticket = SYSTEM->zeros(n) + nro_ticket;
            //QString numero = nro_ticket;
            //QString cantidad = query.value(3).toString();
            QString descripcion = query.value(3).toString();
            QString total = QString().setNum(query.value(4).toString().toDouble(), ' ', 2);
            QString nombre = query.value(5).toString();
            QString pago = query.value(6).toString();
            QString fecha = query.value(7).toDateTime().toString("dd-MM-yyyy hh:mm:ss");
            QString operacion = query.value(8).toString();
            QString documento = query.value(9).toString();

            QTableWidgetItem* item_id = new QTableWidgetItem(id);
            item_id->setToolTip(id);
            QTableWidgetItem* item_estado = new QTableWidgetItem(estado);
            item_estado->setToolTip(estado);
            QTableWidgetItem* item_s_nro = new QTableWidgetItem(s_nro);
            item_s_nro->setToolTip(s_nro);
            item_s_nro->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            //descripcion = descripcion.mid(0, 50) + "\n" + descripcion.mid(50, descripcion.length()-50);
            QTableWidgetItem* item_descripcion = new QTableWidgetItem(descripcion);
            item_descripcion->setToolTip(descripcion);
            QTableWidgetItem* item_total = new QTableWidgetItem(total);
            item_total->setToolTip(total);
            item_total->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            QTableWidgetItem* item_nombre = new QTableWidgetItem(nombre);
            item_nombre->setToolTip(nombre);
            QTableWidgetItem* item_pago = new QTableWidgetItem(pago);
            item_pago->setToolTip(pago);
            QTableWidgetItem* item_fecha = new QTableWidgetItem(fecha);
            item_fecha->setToolTip(fecha);
            QTableWidgetItem* item_operacion = new QTableWidgetItem(operacion);
            item_operacion->setToolTip(operacion);
            QTableWidgetItem* item_documento = new QTableWidgetItem(documento);
            item_documento->setToolTip(documento);


            //cantidad = QString().setNum(cantidad.toDouble(), ' ', 3);
            total = QString().setNum(total.toDouble(), ' ', 2);
            ui->tableWidget->setItem(pos, 0, item_id);
            ui->tableWidget->setItem(pos, 1, item_estado);
            ui->tableWidget->setItem(pos, 2, item_s_nro);
            //ui->tableWidget->setItem(pos, 3, new QTableWidgetItem(cantidad));
            ui->tableWidget->setItem(pos, 3, item_descripcion);
            ui->tableWidget->setItem(pos, 4, item_total);
            ui->tableWidget->setItem(pos, 5, item_nombre);
            ui->tableWidget->setItem(pos, 6, item_pago);
            ui->tableWidget->setItem(pos, 7, item_fecha);
            ui->tableWidget->setItem(pos, 8, item_operacion);
            ui->tableWidget->setItem(pos, 9, item_documento);

            if(ui->tableWidget->item(pos, 1)->text().compare("Anulado") == 0){
                for(int j = 0; j < ui->tableWidget->columnCount(); j++) {
                    ui->tableWidget->item(pos, j)->setTextColor(QColor(255, 0, 0));
                    ui->tableWidget->item(pos, j)->setBackgroundColor(QColor(200, 255, 255));
                }
            }
            for(int j=0; j < ui->tableWidget->columnCount(); j++)
                ui->tableWidget->item(pos, j)->setFlags(Qt::ItemIsEnabled
                                                             | Qt::ItemIsSelectable);
            ++pos;
        }

        //SYSTEM->table_resize_to_contents(0, ui->tableWidget, size_query);
        set_total();
    }else{

    }
}
void ComprobanteBuscar::set_total()
{
    QSqlQuery query;
    QString str_query;
    str_query  = "SELECT c.operacion_item_nombre, SUM(c_h_prod.precio), c.pago_item_nombre";
    str_query += " FROM comprobante c";
    str_query += " JOIN comprobante_has_producto c_h_prod ON c.id = c_h_prod.comprobante_id";
    str_query += " JOIN producto prod ON prod.id = c_h_prod.producto_id";
    str_query += " WHERE c.estado_item_nombre = '" + QString(ACTIVO) + "'";
    str_query += " AND DATE(c.fecha_emision) = '"+ui->dateEdit_exportar->date().toString("yyyy-MM-dd")+"'";
    str_query += " GROUP BY c.operacion_item_nombre, prod.id, c_h_prod.precio, c.pago_item_nombre";
    str_query += " ORDER BY c.operacion_item_nombre DESC, c.pago_item_nombre, prod.nombre";

    qDebug()<<str_query<<endl;
    if(query.exec(str_query)){
        double val_ventas_efectivo = 0.0, val_compras_efectivo = 0.0, val_egresos_efectivo = 0.0
                , val_ventas_masterCard = 0.0, val_compras_masterCard = 0.0, val_egresos_masterCard = 0.0
                , val_ventas_visa = 0.0, val_compras_visa = 0.0, val_egresos_visa = 0.0;

        while(query.next()){
            QString operacion, precio, pago;

            operacion = query.value(0).toString();
            precio = QString().setNum(query.value(1).toDouble(), ' ', 1);
            pago = query.value(2).toString();

            if(operacion.compare(VENTA) == 0){
                if(pago.compare(EFECTIVO) == 0){
                    val_ventas_efectivo += precio.toDouble();
                }
                if(pago.compare(MASTER_CARD) == 0){
                    val_ventas_masterCard += precio.toDouble();
                }
                if(pago.compare(VISA) == 0){
                    val_ventas_visa += precio.toDouble();
                }
            }
            if(operacion.compare(COMPRA) == 0){
                if(pago.compare(EFECTIVO) == 0){
                    val_compras_efectivo += precio.toDouble();
                }
                if(pago.compare(MASTER_CARD) == 0){
                    val_compras_masterCard += precio.toDouble();
                }
                if(pago.compare(VISA) == 0){
                    val_compras_visa += precio.toDouble();
                }
            }
            if(operacion.compare(EGRESOS) == 0){
                if(pago.compare(EFECTIVO) == 0){
                    val_egresos_efectivo += precio.toDouble();
                }
                if(pago.compare(MASTER_CARD) == 0){
                    val_egresos_masterCard += precio.toDouble();
                }
                if(pago.compare(VISA) == 0){
                    val_egresos_visa += precio.toDouble();
                }
            }
        }
        QString total = QString().setNum(((val_ventas_efectivo)
                                                - (val_compras_efectivo)
                                                - (val_egresos_efectivo)), ' ', 2);
        ui->lineEdit_totalDown->setText(total);
        ui->lineEdit_totalUp->setText(total);
    }
}

void ComprobanteBuscar::on_lineEdit_buscar_textChanged(const QString& arg)
{
    pos = 0;

    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    //ui->tableWidget->setColumnCount(0);


    set_buscar();
}
void ComprobanteBuscar::on_lineEdit_buscar_returnPressed()
{
    qDebug()<<"return pressed"<<endl;
    pos = 0;

    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    //ui->tableWidget->setColumnCount(0);


    set_buscar();
}
void ComprobanteBuscar::showEvent(QShowEvent *se)
{
    se->accept();

    afterShow = true;

    if(!firstShow){
        on_lineEdit_buscar_textChanged(ui->lineEdit_buscar->text());
        //on_lineEdit_descripcion_buscar_returnPressed();
        firstShow = true;
    }else{
        //on_lineEdit_buscar_textChanged(ui->lineEdit_buscar->text());
    }
}
void ComprobanteBuscar::closeEvent(QCloseEvent *ce)
{
    ce->accept();
    emit closing();
}
bool ComprobanteBuscar::eventFilter(QObject *obj, QEvent *e)
{
    QWidget* w_temp;
    w_temp = this;
    if(obj == w_temp){
        if(e->type() == QEvent::MouseButtonPress){
            if(focusWidget()){
                focusWidget()->setFocus();
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
                    ui->lineEdit_buscar->setFocus();
                    ui->lineEdit_buscar->setCursorPosition(ui->lineEdit_buscar->text().length());
                }
                afterShow = false;
            }
            return true;
        }
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Escape:
                ui->pushButton_salir->click();
                return true;
            case Qt::Key_Up: {
                if (this->focusWidget() != ui->tableWidget) {
                    ui->tableWidget->setFocus(Qt::TabFocusReason);
                    if (ui->tableWidget->currentItem())
                        ui->tableWidget->currentItem()->setSelected(true);
                    return true;
                }
            }break;
            case Qt::Key_Down: {
                if (this->focusWidget() != ui->tableWidget) {
                    ui->tableWidget->setFocus(Qt::TabFocusReason);
                    if (ui->tableWidget->currentItem())
                        ui->tableWidget->currentItem()->setSelected(true);
                    return true;
                }
            }break;
            }
        }else{

        }
        return false;
    }
    w_temp = ui->tableWidget;
    if (obj == w_temp) {
        if (e->type() == QEvent::KeyPress) {
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch (KeyEvent->key())
            {
            case Qt::Key_Return:
                //ui->pushButton_ingresar->click();
                return true;
            case Qt::Key_Enter:{
                QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QApplication::sendEvent(w_temp, key);
                return true;
            }break;
            case Qt::Key_Down: {
                int index = ui->tableWidget->currentRow();
                if (index == ui->tableWidget->rowCount() - 1) {
                    set_buscar();
                    return true;
                }
            }break;
            }

        } else {

        }
        return false;
    }
    w_temp = ui->lineEdit_buscar;
    if (obj == w_temp) {
        if (e->type() == QEvent::KeyPress) {
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch (KeyEvent->key())
            {
            case Qt::Key_Return: {
                on_lineEdit_buscar_returnPressed();
                return true;
            }break;
            case Qt::Key_Enter:{
                QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QApplication::sendEvent(w_temp, key);
                return true;
            }break;
            }

        } else {

        }
        return false;
    }

    w_temp = ui->pushButton_salir;
    if (obj == w_temp) {
        if (e->type() == QEvent::KeyPress) {
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch (KeyEvent->key())
            {
            case Qt::Key_Tab:{
                this->setFocus();
            }break;
            case Qt::Key_Return: {
                ui->pushButton_salir->click();
                return true;
            }break;
            case Qt::Key_Enter:{
                QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QApplication::sendEvent(w_temp, key);
                return true;
            }break;
            }

        } else {

        }
        return false;
    }

    return eventFilter(obj, e);
}

void ComprobanteBuscar::on_pushButton_ingresar_clicked()
{
    QTableWidgetItem* item = ui->tableWidget->currentItem();
    if(!item){
        QMessageBox::warning(this, "Advertencia", "Selecciona un item de la tabla.", "Ok");
        return;
    }

    int row = item->row();
    QTableWidget* tb = ui->tableWidget;

    cod = tb->item(row, 0)->text();
    nombre = tb->item(row, 1)->text();
    direccion = tb->item(row, 2)->text();

    this->close();
}

void ComprobanteBuscar::on_dateTimeEdit_ini_dateTimeChanged(const QDateTime &dateTime)
{
    pos = 0;

    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);        

    set_buscar();
}

void ComprobanteBuscar::on_dateTimeEdit_fin_dateTimeChanged(const QDateTime &dateTime)
{
    pos = 0;

    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);    

    set_buscar();
}

void ComprobanteBuscar::on_checkBox_efectivo_toggled(bool checked)
{
    pos = 0;

    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);        

    set_buscar();
}

void ComprobanteBuscar::on_checkBox_visa_toggled(bool checked)
{
    pos = 0;

    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);    

    set_buscar();
}

void ComprobanteBuscar::on_checkBox_masterCard_toggled(bool checked)
{
    pos = 0;

    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);    

    set_buscar();
}

void ComprobanteBuscar::on_pushButton_anular_clicked()
{
    QTableWidgetItem* item = ui->tableWidget->currentItem();
    if(!item){
        QMessageBox::warning(this, "Advertencia", "Seleccione item", "Aceptar");
        return;
    }
    /*
    int ret = QMessageBox::warning(this, "Advertencia"
                                   , "Va a anular un documento. ¿Esta seguro(a)?", "Aceptar", "Cancelar");
    switch(ret){
    case 0:{

    }break;
    case 1:{
        return;
    }break;
    }*/
    QDate item_date = QDateTime::fromString(ui->tableWidget->item(item->row(), 7)->text(), "dd-MM-yyyy hh:mm:ss").date();
    qDebug()<<item_date<<endl;
    if(item_date != QDate::currentDate()){
        if(SYSTEM->tipo_persona.compare(Persona::master) != 0){
            QMessageBox::warning(this, "Advertencia", "Solo puede con fecha de hoy.\nSe requiere al usuario \"Master\".", "Aceptar");
            return;
        }
    }
    /*
    QString operacion = ui->tableWidget->item(item->row(), 8)->text();
    QString documento = ui->tableWidget->item(item->row(), 9)->text();
    if(operacion.compare(VENTA) == 0 && (documento.compare(BOLETA) == 0 || documento.compare(FACTURA) == 0)){
        AnularCPE* w = new AnularCPE(this);
        w->setWindowFlag(Qt::Dialog);
        w->setWindowModality(Qt::WindowModal);
        w->setAttribute(Qt::WA_AcceptDrops);
        w->show();
        //QMessageBox::warning(this, "Advertencia", "No se puede anular boletas y facturas aun.", "Aceptar");
        return;
    }*/

    AdminPass* ap = new AdminPass(this);
    ap->setModalidad(AdminPass::CONFIRM_MY_PASS);
    //ap->setAttribute(Qt::WA_DeleteOnClose);
    ap->exec();
    bool confirmado = ap->get_confirmado();

    if(!confirmado){
        return;
    }

    QString str_query = "";
    QSqlQuery query;

    QString id = ui->tableWidget->item(item->row(), 0)->text();
    str_query += "UPDATE comprobante SET estado_item_nombre = 'Anulado'";
    str_query += " WHERE id = " + id;

    if(query.exec(str_query)){
        for(int j = 0; j < ui->tableWidget->columnCount(); j++) {
            ui->tableWidget->item(item->row(), j)->setTextColor(QColor(255, 0, 0));
            ui->tableWidget->item(item->row(), j)->setBackgroundColor(QColor(200, 255, 255));
        }
        ui->tableWidget->item(item->row(), 1)->setText("Anulado");

        QString id = ui->tableWidget->item(item->row(), 0)->text();
        for(int i = item->row()-1; i >= 0; i--){
            if(id.compare(ui->tableWidget->item(i, 0)->text()) == 0){
                for(int j = 0; j < ui->tableWidget->columnCount(); j++) {
                    ui->tableWidget->item(i, j)->setTextColor(QColor(255, 0, 0));
                    ui->tableWidget->item(i, j)->setBackgroundColor(QColor(200, 255, 255));
                }
                ui->tableWidget->item(i, 1)->setText("Anulado");
            }else{
                break;
            }
        }
        for(int i = item->row()+1; i < ui->tableWidget->rowCount(); i++){
            if(id.compare(ui->tableWidget->item(i, 0)->text()) == 0){
                for(int j = 0; j < ui->tableWidget->columnCount(); j++) {
                    ui->tableWidget->item(i, j)->setTextColor(QColor(255, 0, 0));
                    ui->tableWidget->item(i, j)->setBackgroundColor(QColor(200, 255, 255));
                }
                ui->tableWidget->item(i, 1)->setText("Anulado");
            }else{
                break;
            }
        }

        //SYSTEM->table_resize_to_contents(0, ui->tableWidget, size_query);

        set_total();
        //on_lineEdit_buscar_textChanged(ui->lineEdit_buscar->text());
    }else{
        QMessageBox::critical(this, "Error", "Ocurrio un error inesperado.", "Ok");
    }
}

void ComprobanteBuscar::on_pushButton_ver_clicked()
{
    QTableWidgetItem* item = ui->tableWidget->currentItem();
    if(!item){
        QMessageBox::warning(this, "Advertencia", "Ver item", "Aceptar");
        return;
    }
    PreviewTicket* w = new PreviewTicket;
    w->setAttribute(Qt::WA_DeleteOnClose);
    w->setWindowModality(Qt::ApplicationModal);

    QString id = ui->tableWidget->item(item->row(), 0)->text();
    QString estado = ui->tableWidget->item(item->row(), 1)->text();
    QString numero = ui->tableWidget->item(item->row(), 2)->text();
    QString nombre = ui->tableWidget->item(item->row(), 5)->text();
    QString pago = ui->tableWidget->item(item->row(), 6)->text();
    QString fecha = ui->tableWidget->item(item->row(), 7)->text();

    w->set_cabecera(numero, fecha, nombre, pago);

    w->set_detalle(id);

    //w->ver_documento();
    //w->ver_documento_asa();
    w->ver_documento_machelo();

    w->show();

    w->resize(500, 600);
}

void ComprobanteBuscar::on_pushButton_habilitar_clicked()
{
    QTableWidgetItem* item = ui->tableWidget->currentItem();
    if(!item){
        QMessageBox::warning(this, "Advertencia", "Seleccione item", "Aceptar");
        return;
    }
    /*
    int ret = QMessageBox::warning(this, "Advertencia"
                                   , "Este documento estará habilitado.", "Aceptar", "Cancelar");
    switch(ret){
    case 0:{

    }break;
    case 1:{
        return;
    }break;
    }*/
    QDate item_date = QDateTime::fromString(ui->tableWidget->item(item->row(), 7)->text(), "dd-MM-yyyy hh:mm:ss").date();
    qDebug()<<item_date<<endl;
    if(item_date != QDate::currentDate()){
        if(SYSTEM->tipo_persona.compare(Persona::master) != 0){
            QMessageBox::warning(this, "Advertencia", "Solo puede con fecha de hoy.\nSe requiere al usuario \"Master\".", "Aceptar");
            return;
        }
    }

    AdminPass* ap = new AdminPass(this);
    ap->setModalidad(AdminPass::CONFIRM_MY_PASS);
    //ap->setAttribute(Qt::WA_DeleteOnClose);
    ap->exec();
    bool confirmado = ap->get_confirmado();

    if(!confirmado){
        return;
    }

    QString str_query = "";
    QSqlQuery query;

    QString id = ui->tableWidget->item(item->row(), 0)->text();
    str_query += "UPDATE comprobante SET estado_item_nombre = 'Activo'";
    str_query += " WHERE id = " + id;

    if(query.exec(str_query)){
        for(int j = 0; j < ui->tableWidget->columnCount(); j++) {
            ui->tableWidget->item(item->row(), j)->setTextColor(QColor(0, 0, 0));
            ui->tableWidget->item(item->row(), j)->setBackgroundColor(QColor(255, 255, 255));
        }
        ui->tableWidget->item(item->row(), 1)->setText("Activo");

        QString id = ui->tableWidget->item(item->row(), 0)->text();
        for(int i = item->row()-1; i >= 0; i--){
            if(id.compare(ui->tableWidget->item(i, 0)->text()) == 0){
                for(int j = 0; j < ui->tableWidget->columnCount(); j++) {
                    ui->tableWidget->item(i, j)->setTextColor(QColor(0, 0, 0));
                    ui->tableWidget->item(i, j)->setBackgroundColor(QColor(255, 255, 255));
                }
                ui->tableWidget->item(i, 1)->setText("Activo");
            }else{
                break;
            }
        }
        for(int i = item->row()+1; i < ui->tableWidget->rowCount(); i++){
            if(id.compare(ui->tableWidget->item(i, 0)->text()) == 0){
                for(int j = 0; j < ui->tableWidget->columnCount(); j++) {
                    ui->tableWidget->item(i, j)->setTextColor(QColor(0, 0, 0));
                    ui->tableWidget->item(i, j)->setBackgroundColor(QColor(255, 255, 255));
                }
                ui->tableWidget->item(i, 1)->setText("Activo");
            }else{
                break;
            }
        }

        //SYSTEM->table_resize_to_contents(0, ui->tableWidget, size_query);

        set_total();
    }

}

void ComprobanteBuscar::on_pushButton_exportar_clicked()
{
    DatosModifcables* w = new DatosModifcables(this);
    w->setAttribute(Qt::WA_DeleteOnClose);
    w->setWindowFlag(Qt::Dialog);
    w->setWindowModality(Qt::WindowModal);

    connect(w, SIGNAL(closing()), this, SLOT(on_datosModificables_closing()));
    w->show();
}

void ComprobanteBuscar::on_lineEdit_buscarProducto_textChanged(const QString &arg1)
{
    pos = 0;

    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);    

    set_buscar();
}
void ComprobanteBuscar::on_myProccess_finished(int exitCode, QProcess::ExitStatus exitStatus)
{
    //qDebug() << "Exit Code: " << exitCode;
    //qDebug() << "Exit Status: " << exitStatus;

    //if (exitCode == 0) {
    QSqlQuery query;
    QString str_query;
    QXlsx::Document xlsx;
    int i=2;
    QXlsx::Format format3;

    // GENERAL
    {
    str_query  = "SELECT c.operacion_item_nombre, prod.nombre, SUM(c_h_prod.cantidad)";
    str_query += ", SUM(c_h_prod.precio), c.pago_item_nombre";
    str_query += ", prod.conteo, prod.medida";
    str_query += " FROM comprobante c";
    str_query += " JOIN comprobante_has_producto c_h_prod ON c.id = c_h_prod.comprobante_id";
    str_query += " JOIN producto prod ON prod.id = c_h_prod.producto_id";
    str_query += " WHERE c.estado_item_nombre = '" + QString(ACTIVO) + "'";
    str_query += " AND DATE(c.fecha_emision) = '"+ui->dateEdit_exportar->date().toString("yyyy-MM-dd")+"'";
    str_query += " GROUP BY c.operacion_item_nombre, prod.id, c_h_prod.precio, c.pago_item_nombre";
    str_query += " ORDER BY c.operacion_item_nombre DESC, c.pago_item_nombre, prod.nombre";



    qDebug()<<str_query<<endl;
    if(query.exec(str_query)){
        xlsx.setColumnWidth(1, 0);
        xlsx.setColumnWidth(2, 15);
        xlsx.setColumnWidth(3, 40);
        xlsx.setColumnWidth(4, 15);
        xlsx.setColumnWidth(5, 15);
        xlsx.setColumnWidth(6, 15);
        xlsx.setColumnWidth(7, 20);
        xlsx.setColumnWidth(8, 7);
        // /////////////////////////////
        xlsx.setColumnWidth(9, 20);
        xlsx.setColumnWidth(10, 20);
        xlsx.setColumnWidth(11, 15);

        QXlsx::Format format_0;
        format_0.setPatternBackgroundColor(QColor(255, 255, 0));
        format_0.setFontColor(QColor(Qt::black));
        format_0.setFontSize(12);
        format_0.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
        format_0.setBorderStyle(QXlsx::Format::BorderDashDotDot);

        format_0.setHorizontalAlignment(QXlsx::Format::AlignHCenter);

        xlsx.write("B" + QString().setNum(i), "OPERACION", format_0);
        xlsx.write("C" + QString().setNum(i), "Producto", format_0);
        xlsx.write("D" + QString().setNum(i), "Cantidad", format_0);
        xlsx.write("E" + QString().setNum(i), "P. Unitario", format_0);
        xlsx.write("F" + QString().setNum(i), "P. Total", format_0);
        xlsx.write("G" + QString().setNum(i), "Tipo de Pago", format_0);

        format_0.setPatternBackgroundColor(QColor(255, 255, 255));

        i++;

        while(query.next()){
            QString operacion, nombre, cantidad, precio, pago, conteo, medida;
            operacion = query.value(0).toString();
            nombre = query.value(1).toString();
            cantidad = QString().setNum(query.value(2).toDouble(), ' ', 3);
            precio = QString().setNum(query.value(3).toDouble(), ' ', 1);
            pago = query.value(4).toString();
            conteo = query.value(5).toString();
            medida = query.value(6).toString();

            QXlsx::Format format1;
            if(operacion.compare(VENTA) == 0){
                format1.setPatternBackgroundColor(QColor(200, 255, 150));
            }
            if(operacion.compare(COMPRA) == 0){
                format1.setPatternBackgroundColor(QColor(100, 255, 255));
            }
            if(operacion.compare(EGRESOS) == 0){
                format1.setPatternBackgroundColor(QColor(150, 150, 255));
            }

            format1.setFontColor(QColor(Qt::black));
            format1.setFontSize(12);
            format1.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
            format1.setBorderStyle(QXlsx::Format::BorderDashDotDot);

            xlsx.write("B" + QString().setNum(i), operacion, format1);

            xlsx.write("C" + QString().setNum(i), nombre, format1);

            format1.setHorizontalAlignment(QXlsx::Format::AlignRight);

            xlsx.write("D" + QString().setNum(i), cantidad, format1);

            xlsx.write("F" + QString().setNum(i), precio, format1);

            xlsx.write("E" + QString().setNum(i)
                       , QString().setNum(precio.toDouble()/cantidad.toDouble(), ' ', 3), format1);


            if(pago.compare(EFECTIVO) == 0){
                format3.setPatternBackgroundColor(QColor(200, 255, 150));
                format3.setFontColor(QColor(Qt::black));
            }
            if(pago.compare(VISA) == 0){
                format3.setPatternBackgroundColor(QColor(200, 255, 255));
            }
            if(pago.compare(MASTER_CARD) == 0){
                format3.setPatternBackgroundColor(QColor(Qt::yellow));
                format3.setFontColor(QColor(Qt::red));
            }


            format3.setFontSize(12);
            format3.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
            format3.setBorderStyle(QXlsx::Format::BorderThin);

            xlsx.write("G" + QString().setNum(i), pago, format3);
            i++;
        }
    }else{
        return;
    }
    }

    double val_ventas_efectivo = 0.0, val_compras_efectivo = 0.0, val_egresos_efectivo = 0.0
            , val_ventas_masterCard = 0.0, val_compras_masterCard = 0.0, val_egresos_masterCard = 0.0
            , val_ventas_visa = 0.0, val_compras_visa = 0.0, val_egresos_visa = 0.0;

    // TOTALES
    {

    str_query  = "SELECT c.operacion_item_nombre, SUM(c_h_prod.precio), c.pago_item_nombre";
    str_query += " FROM comprobante c";
    str_query += " JOIN comprobante_has_producto c_h_prod ON c.id = c_h_prod.comprobante_id";
    str_query += " JOIN producto prod ON prod.id = c_h_prod.producto_id";
    str_query += " WHERE c.estado_item_nombre = '" + QString(ACTIVO) + "'";
    str_query += " AND DATE(c.fecha_emision) = '"+ui->dateEdit_exportar->date().toString("yyyy-MM-dd")+"'";
    str_query += " GROUP BY c.operacion_item_nombre, prod.id, c_h_prod.precio, c.pago_item_nombre";
    str_query += " ORDER BY c.operacion_item_nombre DESC, c.pago_item_nombre, prod.nombre";

    qDebug()<<str_query<<endl;
    if(query.exec(str_query)){
        while(query.next()){
            QString operacion, precio, pago;

            operacion = query.value(0).toString();
            precio = QString().setNum(query.value(1).toDouble(), ' ', 1);
            pago = query.value(2).toString();

            if(operacion.compare(VENTA) == 0){
                if(pago.compare(EFECTIVO) == 0){
                    val_ventas_efectivo += precio.toDouble();
                }
                if(pago.compare(MASTER_CARD) == 0){
                    val_ventas_masterCard += precio.toDouble();
                }
                if(pago.compare(VISA) == 0){
                    val_ventas_visa += precio.toDouble();
                }
            }
            if(operacion.compare(COMPRA) == 0){
                if(pago.compare(EFECTIVO) == 0){
                    val_compras_efectivo += precio.toDouble();
                }
                if(pago.compare(MASTER_CARD) == 0){
                    val_compras_masterCard += precio.toDouble();
                }
                if(pago.compare(VISA) == 0){
                    val_compras_visa += precio.toDouble();
                }
            }
            if(operacion.compare(EGRESOS) == 0){
                if(pago.compare(EFECTIVO) == 0){
                    val_egresos_efectivo += precio.toDouble();
                }
                if(pago.compare(MASTER_CARD) == 0){
                    val_egresos_masterCard += precio.toDouble();
                }
                if(pago.compare(VISA) == 0){
                    val_egresos_visa += precio.toDouble();
                }
            }
        }
    }

    QXlsx::Format format1;
    QXlsx::Format format3;
    format1.setFontColor(QColor(Qt::black));
    format1.setFontSize(12);
    format1.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    format1.setBorderStyle(QXlsx::Format::BorderDashDotDot);

    format3.setFontSize(12);
    format3.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    format3.setBorderStyle(QXlsx::Format::BorderThin);

    format1.setPatternBackgroundColor(QColor(200, 255, 150));
    xlsx.write("I" + QString().setNum(2), VENTA, format1);
    xlsx.write("I" + QString().setNum(3), VENTA, format1);
    xlsx.write("I" + QString().setNum(4), VENTA, format1);

    format1.setHorizontalAlignment(QXlsx::Format::AlignRight);
    xlsx.write("J" + QString().setNum(2), val_ventas_efectivo, format1);
    xlsx.write("J" + QString().setNum(3), val_ventas_masterCard, format1);
    xlsx.write("J" + QString().setNum(4), val_ventas_visa, format1);
    format1.setHorizontalAlignment(QXlsx::Format::AlignHCenter);

    format3.setPatternBackgroundColor(QColor(200, 255, 150));
    format3.setFontColor(QColor(Qt::black));
    xlsx.write("K" + QString().setNum(2), EFECTIVO, format3);
    format3.setPatternBackgroundColor(QColor(Qt::yellow));
    format3.setFontColor(QColor(Qt::red));
    xlsx.write("K" + QString().setNum(3), MASTER_CARD, format3);
    format3.setPatternBackgroundColor(QColor(200, 255, 255));
    format3.setFontColor(QColor(Qt::black));
    xlsx.write("K" + QString().setNum(4), VISA, format3);


    format1.setPatternBackgroundColor(QColor(150, 150, 255));
    xlsx.write("I" + QString().setNum(5), EGRESOS, format1);
    xlsx.write("I" + QString().setNum(6), EGRESOS, format1);
    xlsx.write("I" + QString().setNum(7), EGRESOS, format1);

    format1.setHorizontalAlignment(QXlsx::Format::AlignRight);
    xlsx.write("J" + QString().setNum(5), val_egresos_efectivo, format1);
    xlsx.write("J" + QString().setNum(6), val_egresos_masterCard, format1);
    xlsx.write("J" + QString().setNum(7), val_egresos_visa, format1);
    format1.setHorizontalAlignment(QXlsx::Format::AlignHCenter);

    format3.setPatternBackgroundColor(QColor(200, 255, 150));
    format3.setFontColor(QColor(Qt::black));
    xlsx.write("K" + QString().setNum(5), EFECTIVO, format3);
    format3.setPatternBackgroundColor(QColor(Qt::yellow));
    format3.setFontColor(QColor(Qt::red));
    xlsx.write("K" + QString().setNum(6), MASTER_CARD, format3);
    format3.setPatternBackgroundColor(QColor(200, 255, 255));
    format3.setFontColor(QColor(Qt::black));
    xlsx.write("K" + QString().setNum(7), VISA, format3);

    format1.setPatternBackgroundColor(QColor(100, 255, 255));
    xlsx.write("I" + QString().setNum(8), COMPRA, format1);
    xlsx.write("I" + QString().setNum(9), COMPRA, format1);
    xlsx.write("I" + QString().setNum(10), COMPRA, format1);

    format1.setHorizontalAlignment(QXlsx::Format::AlignRight);
    xlsx.write("J" + QString().setNum(8), val_compras_efectivo, format1);
    xlsx.write("J" + QString().setNum(9), val_compras_masterCard, format1);
    xlsx.write("J" + QString().setNum(10), val_compras_visa, format1);
    format1.setHorizontalAlignment(QXlsx::Format::AlignHCenter);

    format3.setPatternBackgroundColor(QColor(200, 255, 150));
    format3.setFontColor(QColor(Qt::black));
    xlsx.write("K" + QString().setNum(8), EFECTIVO, format3);
    format3.setPatternBackgroundColor(QColor(Qt::yellow));
    format3.setFontColor(QColor(Qt::red));
    xlsx.write("K" + QString().setNum(9), MASTER_CARD, format3);
    format3.setPatternBackgroundColor(QColor(200, 255, 255));
    format3.setFontColor(QColor(Qt::black));
    xlsx.write("K" + QString().setNum(10), VISA, format3);

    format3.setPatternBackgroundColor(QColor(200, 255, 150));
    format3.setFontColor(QColor(Qt::black));
    format3.setHorizontalAlignment(QXlsx::Format::AlignRight);
    xlsx.write("J" + QString().setNum(11), "Total Efectivo:", format3);
    xlsx.write("K" + QString().setNum(11), (val_ventas_efectivo - val_compras_efectivo - val_egresos_efectivo), format3);
    //format1.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    format3.setPatternBackgroundColor(QColor(Qt::yellow));
    format3.setFontColor(QColor(Qt::red));
    xlsx.write("J" + QString().setNum(12), "Total Master Card:", format3);
    //format1.setHorizontalAlignment(QXlsx::Format::AlignRight);
    xlsx.write("K" + QString().setNum(12), (val_ventas_masterCard - val_compras_masterCard - val_egresos_masterCard), format3);
    //format1.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    format3.setPatternBackgroundColor(QColor(200, 255, 255));
    format3.setFontColor(QColor(Qt::black));
    xlsx.write("J" + QString().setNum(13), "Total VISA:", format3);
    //format1.setHorizontalAlignment(QXlsx::Format::AlignRight);
    xlsx.write("K" + QString().setNum(13), (val_ventas_visa - val_compras_visa - val_egresos_visa), format3);
    //format1.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    format3.setPatternBackgroundColor(QColor(200, 255, 150));
    format3.setFontColor(QColor(Qt::black));
    xlsx.write("J" + QString().setNum(14), "Total:", format3);
    //format1.setHorizontalAlignment(QXlsx::Format::AlignRight);
    xlsx.write("K" + QString().setNum(14), ((val_ventas_efectivo + val_ventas_masterCard + val_ventas_visa)
                                            - (val_compras_efectivo + val_compras_masterCard + val_compras_visa)
                                            - (val_egresos_efectivo + val_egresos_masterCard + val_egresos_visa)), format3);
    //format1.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    }

    double cantidad_saldo_ayer = 0.0, cantidad_entrada = 0.0, cantidad_salida = 0.0;
    // CANTIDAD de POLLOS
    {

    str_query  = "SELECT t.cant_ent_ah - t.cant_sal_ah, t.cant_ent_h, t.cant_sal_h";
    str_query += " FROM (SELECT ";
    str_query += " IFNULL(t_ent_ah.cantidad, 0) AS cant_ent_ah, IFNULL(t_sal_ah.cantidad, 0) AS cant_sal_ah";
    str_query += ", IFNULL(t_ent_h.cantidad, 0) AS cant_ent_h, IFNULL(t_sal_h.cantidad, 0) AS cant_sal_h";

    str_query += " FROM (SELECT 1) AS t";

    // ANTES DE HOY

    str_query += " LEFT JOIN (SELECT SUM(c_h_prod_ent.cantidad*prod_ent.medida) AS cantidad FROM comprobante c_ent";
    str_query += " JOIN comprobante_has_producto c_h_prod_ent ON ";
    str_query += "((c_ent.operacion_item_nombre = '"+QString(COMPRA)+"'";
    str_query += " OR c_ent.operacion_item_nombre = '"+QString(EGRESOS)+"')";
    str_query += " AND c_ent.id = c_h_prod_ent.comprobante_id)";
    str_query += " JOIN producto prod_ent ON prod_ent.id = c_h_prod_ent.producto_id";
    str_query += " WHERE DATE(c_ent.fecha_emision) < '"+ui->dateEdit_exportar->date().toString("yyyy-MM-dd")+"'";
    str_query += " AND c_ent.estado_item_nombre = '" + QString(ACTIVO) + "'";
    str_query += " AND prod_ent.conteo = '1'";
    str_query += " ) AS t_ent_ah ON 1";

    str_query += " LEFT JOIN (SELECT SUM(c_h_prod_sal.cantidad*prod_sal.medida) AS cantidad FROM comprobante c_sal";
    str_query += " JOIN comprobante_has_producto c_h_prod_sal ON ";
    str_query += "(c_sal.operacion_item_nombre = '"+QString(VENTA)+"'";
    str_query += " AND c_sal.id = c_h_prod_sal.comprobante_id)";
    str_query += " JOIN producto prod_sal ON prod_sal.id = c_h_prod_sal.producto_id";
    str_query += " WHERE DATE(c_sal.fecha_emision) < '"+ui->dateEdit_exportar->date().toString("yyyy-MM-dd")+"'";
    str_query += " AND c_sal.estado_item_nombre = '" + QString(ACTIVO) + "'";
    str_query += " AND prod_sal.conteo = '1'";
    str_query += " ) AS t_sal_ah ON 1";

    // HOY

    str_query += " LEFT JOIN (SELECT SUM(c_h_prod_ent.cantidad*prod_ent.medida) AS cantidad FROM comprobante c_ent";
    str_query += " JOIN comprobante_has_producto c_h_prod_ent ON ";
    str_query += "((c_ent.operacion_item_nombre = '"+QString(COMPRA)+"'";
    str_query += " OR c_ent.operacion_item_nombre = '"+QString(EGRESOS)+"')";
    str_query += " AND c_ent.id = c_h_prod_ent.comprobante_id)";
    str_query += " JOIN producto prod_ent ON prod_ent.id = c_h_prod_ent.producto_id";
    str_query += " WHERE DATE(c_ent.fecha_emision) = '"+ui->dateEdit_exportar->date().toString("yyyy-MM-dd")+"'";
    str_query += " AND c_ent.estado_item_nombre = '" + QString(ACTIVO) + "'";
    str_query += " AND prod_ent.conteo = '1'";
    str_query += " ) AS t_ent_h ON 1";

    str_query += " LEFT JOIN (SELECT SUM(c_h_prod_sal.cantidad*prod_sal.medida) AS cantidad FROM comprobante c_sal";
    str_query += " JOIN comprobante_has_producto c_h_prod_sal ON ";
    str_query += "(c_sal.operacion_item_nombre = '"+QString(VENTA)+"'";
    str_query += " AND c_sal.id = c_h_prod_sal.comprobante_id)";
    str_query += " JOIN producto prod_sal ON prod_sal.id = c_h_prod_sal.producto_id";
    str_query += " WHERE DATE(c_sal.fecha_emision) = '"+ui->dateEdit_exportar->date().toString("yyyy-MM-dd")+"'";
    str_query += " AND c_sal.estado_item_nombre = '" + QString(ACTIVO) + "'";
    str_query += " AND prod_sal.conteo = '1'";
    str_query += " ) AS t_sal_h ON 1";
/*
    str_query += " WHERE";
    //str_query += " WHERE t_ent_h.nombre = t_sal_h.nombre";
    //str_query += " AND t_ent_ah.nombre = t_sal_ah.nombre";
    str_query += " (IFNULL(t_ent_ah.cantidad, 0) <> 0";
    str_query += " AND IFNULL(t_sal_ah.cantidad, 0) <> 0)";
    str_query += " OR (IFNULL(t_ent_h.cantidad, 0) <> 0";
    str_query += " AND IFNULL(t_sal_h.cantidad, 0) <> 0)";*/

    //str_query += " GROUP BY prod.nombre";

    str_query += ") AS t";

    qDebug()<<str_query<<endl;
    if(query.exec(str_query)){
        query.next();
        cantidad_saldo_ayer = query.value(0).toDouble();
        //query.next();
        //cantidad_saldo_ayer -= query.value(1).toDouble();
        //query.next();
        cantidad_entrada = query.value(1).toDouble();
        //query.next();
        cantidad_salida = query.value(2).toDouble();


        format3.setPatternBackgroundColor(QColor(255, 255, 150));
        //format1.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
        xlsx.write("I" + QString().setNum(16), "SALDO pollos:", format3);
        //format1.setHorizontalAlignment(QXlsx::Format::AlignRight);
        xlsx.write("J" + QString().setNum(16), QString().setNum((cantidad_saldo_ayer), ' ', 3), format3);

        xlsx.write("I" + QString().setNum(17), "HOY Entrada:", format3);
        //format1.setHorizontalAlignment(QXlsx::Format::AlignRight);
        xlsx.write("J" + QString().setNum(17), QString().setNum((cantidad_entrada), ' ', 3), format3);

        xlsx.write("I" + QString().setNum(18), "HOY Salida:", format3);
        //format1.setHorizontalAlignment(QXlsx::Format::AlignRight);
        xlsx.write("J" + QString().setNum(18), QString().setNum((cantidad_salida), ' ', 3), format3);

        xlsx.write("I" + QString().setNum(19), "Cantidad de Pollos:", format3);
        //format1.setHorizontalAlignment(QXlsx::Format::AlignRight);
        xlsx.write("J" + QString().setNum(19), QString().setNum((cantidad_saldo_ayer
                                                                 +cantidad_entrada-cantidad_salida), ' ', 3), format3);
    }
    }

    // BEBIDAS
    {
    str_query  = "SELECT t.nombre, t.cant_ent_ah - t.cant_sal_ah, t.cant_ent_h, t.cant_sal_h";
    str_query += " FROM (SELECT prod.nombre AS nombre";
    str_query += ", IFNULL(t_ent_ah.cantidad, 0) AS cant_ent_ah, IFNULL(t_sal_ah.cantidad, 0) AS cant_sal_ah";
    str_query += ", IFNULL(t_ent_h.cantidad, 0) AS cant_ent_h, IFNULL(t_sal_h.cantidad, 0) AS cant_sal_h";

    str_query += " FROM producto prod";

    // ANTES DE HOY

    str_query += " LEFT JOIN (SELECT prod_ent.nombre AS nombre, SUM(c_h_prod_ent.cantidad) AS cantidad FROM comprobante c_ent";
    str_query += " JOIN comprobante_has_producto c_h_prod_ent ON ";
    str_query += "((c_ent.operacion_item_nombre = '"+QString(COMPRA)+"'";
    str_query += " OR c_ent.operacion_item_nombre = '"+QString(EGRESOS)+"')";
    str_query += " AND c_ent.id = c_h_prod_ent.comprobante_id)";
    str_query += " JOIN producto prod_ent ON prod_ent.id = c_h_prod_ent.producto_id";
    str_query += " JOIN familia fam ON prod_ent.familia_id = fam.id";
    str_query += " WHERE DATE(c_ent.fecha_emision) < '"+ui->dateEdit_exportar->date().toString("yyyy-MM-dd")+"'";
    str_query += " AND c_ent.estado_item_nombre = '" + QString(ACTIVO) + "'";
    str_query += " AND fam.nombre = 'Bebidas'";
    str_query += " GROUP BY prod_ent.nombre) AS t_ent_ah ON prod.nombre = t_ent_ah.nombre";

    str_query += " LEFT JOIN (SELECT prod_sal.nombre AS nombre, SUM(c_h_prod_sal.cantidad) AS cantidad FROM comprobante c_sal";
    str_query += " JOIN comprobante_has_producto c_h_prod_sal ON ";
    str_query += "(c_sal.operacion_item_nombre = '"+QString(VENTA)+"'";
    str_query += " AND c_sal.id = c_h_prod_sal.comprobante_id)";
    str_query += " JOIN producto prod_sal ON prod_sal.id = c_h_prod_sal.producto_id";
    str_query += " JOIN familia fam ON prod_sal.familia_id = fam.id";
    str_query += " WHERE DATE(c_sal.fecha_emision) < '"+ui->dateEdit_exportar->date().toString("yyyy-MM-dd")+"'";
    str_query += " AND c_sal.estado_item_nombre = '" + QString(ACTIVO) + "'";
    str_query += " AND fam.nombre = 'Bebidas'";
    str_query += " GROUP BY prod_sal.nombre) AS t_sal_ah ON prod.nombre = t_sal_ah.nombre";

    // HOY

    str_query += " LEFT JOIN (SELECT prod_ent.nombre AS nombre, SUM(c_h_prod_ent.cantidad) AS cantidad FROM comprobante c_ent";
    str_query += " JOIN comprobante_has_producto c_h_prod_ent ON ";
    str_query += "((c_ent.operacion_item_nombre = '"+QString(COMPRA)+"'";
    str_query += " OR c_ent.operacion_item_nombre = '"+QString(EGRESOS)+"')";
    str_query += " AND c_ent.id = c_h_prod_ent.comprobante_id)";
    str_query += " JOIN producto prod_ent ON prod_ent.id = c_h_prod_ent.producto_id";
    str_query += " JOIN familia fam ON prod_ent.familia_id = fam.id";
    str_query += " WHERE DATE(c_ent.fecha_emision) = '"+ui->dateEdit_exportar->date().toString("yyyy-MM-dd")+"'";
    str_query += " AND c_ent.estado_item_nombre = '" + QString(ACTIVO) + "'";
    str_query += " AND fam.nombre = 'Bebidas'";
    str_query += " GROUP BY prod_ent.nombre) AS t_ent_h ON prod.nombre = t_ent_h.nombre";

    str_query += " LEFT JOIN (SELECT prod_sal.nombre AS nombre, SUM(c_h_prod_sal.cantidad) AS cantidad FROM comprobante c_sal";
    str_query += " JOIN comprobante_has_producto c_h_prod_sal ON ";
    str_query += "(c_sal.operacion_item_nombre = '"+QString(VENTA)+"'";
    str_query += " AND c_sal.id = c_h_prod_sal.comprobante_id)";
    str_query += " JOIN producto prod_sal ON prod_sal.id = c_h_prod_sal.producto_id";
    str_query += " JOIN familia fam ON prod_sal.familia_id = fam.id";
    str_query += " WHERE DATE(c_sal.fecha_emision) = '"+ui->dateEdit_exportar->date().toString("yyyy-MM-dd")+"'";
    str_query += " AND c_sal.estado_item_nombre = '" + QString(ACTIVO) + "'";
    str_query += " AND fam.nombre = 'Bebidas'";
    str_query += " GROUP BY prod_sal.nombre) AS t_sal_h ON prod.nombre = t_sal_h.nombre";

    str_query += " JOIN familia fam ON prod.familia_id = fam.id";

    str_query += " WHERE";
    //str_query += " WHERE t_ent_h.nombre = t_sal_h.nombre";
    //str_query += " AND t_ent_ah.nombre = t_sal_ah.nombre";
    str_query += " (IFNULL(t_ent_ah.cantidad, 0) <> 0";
    str_query += " OR IFNULL(t_sal_ah.cantidad, 0) <> 0)";
    str_query += " OR (IFNULL(t_ent_h.cantidad, 0) <> 0";
    str_query += " OR IFNULL(t_sal_h.cantidad, 0) <> 0)";
    str_query += " AND fam.nombre = 'Bebidas'";

    str_query += " GROUP BY prod.nombre";

    str_query += ") AS t";

    i++;
    qDebug()<<str_query<<endl;
    if(query.exec(str_query)){
        QXlsx::Format format1;
        format1.setPatternBackgroundColor(QColor(255, 255, 0));
        format1.setFontColor(QColor(Qt::black));
        format1.setFontSize(12);
        format1.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
        format1.setBorderStyle(QXlsx::Format::BorderDashDotDot);

        format1.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
        xlsx.write("C" + QString().setNum(i), "Nombre", format1);
        xlsx.write("D" + QString().setNum(i), "Saldo", format1);
        xlsx.write("E" + QString().setNum(i), "HOY Entrada", format1);
        xlsx.write("F" + QString().setNum(i), "HOY Salida", format1);
        xlsx.write("G" + QString().setNum(i), "Cant. Total", format1);

        format1.setPatternBackgroundColor(QColor(255, 255, 255));

        i++;
        while(query.next()){
            QString nombre, saldo_ayer, hoy_ent, hoy_sal;
            nombre = query.value(0).toString();
            saldo_ayer = query.value(1).toString();
            hoy_ent = query.value(2).toString();
            hoy_sal = query.value(3).toString();

            format1.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
            format1.setBorderStyle(QXlsx::Format::BorderDashDotDot);
            xlsx.write("C" + QString().setNum(i), nombre, format1);
            format1.setHorizontalAlignment(QXlsx::Format::AlignRight);
            format3.setBorderStyle(QXlsx::Format::BorderThin);
            xlsx.write("D" + QString().setNum(i), QString().setNum(saldo_ayer.toDouble(), ' ', 3), format1);
            xlsx.write("E" + QString().setNum(i), QString().setNum(hoy_ent.toDouble(), ' ', 3), format1);
            xlsx.write("F" + QString().setNum(i), QString().setNum(hoy_sal.toDouble(), ' ', 3), format1);
            xlsx.write("G" + QString().setNum(i)
                       , QString().setNum(saldo_ayer.toDouble()+(hoy_ent.toDouble()-hoy_sal.toDouble()), ' ', 3), format1);
            i++;
        }
    }else{
        return;
    }
    }

    // CORTESIAS
    {

    str_query  = "SELECT c.nombre, prod.nombre, SUM(c_h_prod.cantidad)";
    str_query += "";
    str_query += ", prod.conteo, prod.medida";
    str_query += " FROM comprobante c";
    str_query += " JOIN comprobante_has_producto c_h_prod ON c.id = c_h_prod.comprobante_id";
    str_query += " JOIN producto prod ON prod.id = c_h_prod.producto_id";
    str_query += " WHERE c.estado_item_nombre = '" + QString(ACTIVO) + "'";
    str_query += " AND c.operacion_item_nombre = '" + QString(VENTA) + "'";
    str_query += " AND DATE(c.fecha_emision) = '"+ui->dateEdit_exportar->date().toString("yyyy-MM-dd")+"'";
    str_query += " AND c_h_prod.precio = '0'";
    str_query += " GROUP BY c.nombre, prod.nombre";
    str_query += " ORDER BY c.nombre, prod.nombre";

    i++;
    qDebug()<<str_query<<endl;
    if(query.exec(str_query)){
        QXlsx::Format format1;
        format1.setPatternBackgroundColor(QColor(255, 255, 0));
        format1.setFontColor(QColor(Qt::black));
        format1.setFontSize(12);
        format1.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
        format1.setBorderStyle(QXlsx::Format::BorderDashDotDot);

        format1.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
        format3.setBorderStyle(QXlsx::Format::BorderThin);
        xlsx.write("C" + QString().setNum(i), "Cliente", format1);
        xlsx.mergeCells("D"+QString().setNum(i)+":F"+QString().setNum(i));
        xlsx.write("D" + QString().setNum(i), "Producto", format1);
        xlsx.write("E" + QString().setNum(i), "", format1);
        xlsx.write("F" + QString().setNum(i), "", format1);
        xlsx.write("G" + QString().setNum(i), "Cantidad", format1);

        format1.setPatternBackgroundColor(QColor(255, 255, 255));

        i++;
        while(query.next()){
            QString cliente, producto, cantidad;
            cliente = query.value(0).toString();
            producto = query.value(1).toString();
            cantidad = query.value(2).toString();

            format1.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
            format1.setBorderStyle(QXlsx::Format::BorderDashDotDot);
            xlsx.write("C" + QString().setNum(i), cliente, format1);
            format1.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
            format1.setBorderStyle(QXlsx::Format::BorderDashDotDot);
            xlsx.mergeCells("D"+QString().setNum(i)+":F"+QString().setNum(i));
            xlsx.write("D" + QString().setNum(i), producto, format1);
            xlsx.write("E" + QString().setNum(i), "", format1);
            xlsx.write("F" + QString().setNum(i), "", format1);
            format1.setHorizontalAlignment(QXlsx::Format::AlignRight);
            format1.setBorderStyle(QXlsx::Format::BorderThin);
            xlsx.write("G" + QString().setNum(i)
                       , QString().setNum(cantidad.toDouble(), ' ', 3), format1);
            i++;
        }
    }
    }


    xlsx.renameSheet("Sheet1", "R. Interno");

    xlsx.addSheet("Pollo a la Brasa");

    i = 1;
    double total_pollo_brasa = 0.0, real_brasa = 0.0;
    // POLLO A LA BRASA
    {
        str_query  = "SELECT prod.nombre, SUM(prod.medida*c_h_prod.cantidad), SUM(c_h_prod.cantidad)";
        str_query += ", SUM(c_h_prod.precio)/SUM(c_h_prod.cantidad), SUM(c_h_prod.precio)";
        str_query += " FROM comprobante c";
        str_query += " JOIN comprobante_has_producto c_h_prod ON c.id = c_h_prod.comprobante_id";
        str_query += " JOIN producto prod ON prod.id = c_h_prod.producto_id";
        str_query += " JOIN familia fam ON fam.id = prod.familia_id";
        str_query += " WHERE c.estado_item_nombre = '" + QString(ACTIVO) + "'";
        str_query += " AND fam.nombre = 'Pollo a la Brasa'";
        str_query += " AND c.operacion_item_nombre = '"+QString(VENTA)+"'";
        str_query += " AND DATE(c.fecha_emision) = '"+ui->dateEdit_exportar->date().toString("yyyy-MM-dd")+"'";
        str_query += " GROUP BY prod.id";
        //str_query += " ORDER BY c.operacion_item_nombre DESC, c.pago_item_nombre, prod.nombre";

        qDebug()<<str_query<<endl;
        if(query.exec(str_query)){
            xlsx.setColumnWidth(1, 40);
            xlsx.setColumnWidth(2, 20);
            xlsx.setColumnWidth(3, 20);
            xlsx.setColumnWidth(4, 20);
            xlsx.setColumnWidth(5, 20);

            // /////////////////////////////

            QXlsx::Format format1;
            format1.setPatternBackgroundColor(QColor(255, 255, 0));
            format1.setFontColor(QColor(Qt::black));
            format1.setFontSize(12);
            format1.setFontBold(true);
            format1.setFontUnderline(QXlsx::Format::FontUnderlineSingle);
            format1.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
            format1.setBorderStyle(QXlsx::Format::BorderDashDotDot);

            xlsx.mergeCells("A"+QString().setNum(i)+":E"+QString().setNum(i));

            xlsx.write("A" + QString().setNum(i), "Pollo a la Brasa", format1);
            xlsx.write("B" + QString().setNum(i), "", format1);
            xlsx.write("C" + QString().setNum(i), "", format1);
            xlsx.write("D" + QString().setNum(i), "", format1);
            xlsx.write("E" + QString().setNum(i), "", format1);

            //  ///////////////////////////////////////////////////////////
            format1.setFontUnderline(QXlsx::Format::FontUnderlineNone);
            format1.setFontBold(false);
            i++;

            xlsx.write("A" + QString().setNum(i), "Nombre", format1);
            xlsx.write("B" + QString().setNum(i), "Real", format1);
            xlsx.write("C" + QString().setNum(i), "Cantidad", format1);
            xlsx.write("D" + QString().setNum(i), "P. Unit", format1);
            xlsx.write("E" + QString().setNum(i), "P. Total", format1);

            i++;

            format1.setPatternBackgroundColor(QColor(200, 255, 150));

            double real_total = 0.0, total = 0.0;
            while(query.next()){
                QString nombre, real, cantidad, p_unit, p_total;
                nombre = query.value(0).toString();
                real = QString().setNum(query.value(1).toDouble(), ' ', 3);
                cantidad = QString().setNum(query.value(2).toDouble(), ' ', 3);
                p_unit = QString().setNum(query.value(3).toDouble(), ' ', 1);
                p_total = QString().setNum(query.value(4).toDouble(), ' ', 1);

                real_total += real.toDouble();
                total += p_total.toDouble();

                format1.setFontColor(QColor(Qt::black));
                format1.setFontSize(12);
                format1.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
                format1.setBorderStyle(QXlsx::Format::BorderDashDotDot);

                xlsx.write("A" + QString().setNum(i), nombre, format1);

                xlsx.write("B" + QString().setNum(i), real, format1);

                format1.setHorizontalAlignment(QXlsx::Format::AlignRight);

                xlsx.write("C" + QString().setNum(i), cantidad, format1);

                xlsx.write("D" + QString().setNum(i), p_unit, format1);

                xlsx.write("E" + QString().setNum(i)
                           , QString().setNum(p_total.toDouble(), ' ', 1), format1);

                i++;
            }
            real_brasa = real_total;
            total_pollo_brasa = total;
            format1.setPatternBackgroundColor(QColor(255, 0, 0));
            format1.setFontColor(QColor(Qt::white));

            xlsx.write("B" + QString().setNum(i), real_total, format1);

            format1.setPatternBackgroundColor(QColor(200, 255, 150));
            format1.setFontColor(QColor(Qt::black));

            xlsx.write("E" + QString().setNum(i), total, format1);
        }else{
            return;
        }
    }

    xlsx.addSheet("Pollo Broaster");

    i = 1;
    double total_pollo_broaster = 0.0, real_broaster = 0.0, queda_manana_broaster = 0.0;
    // POLLO BROASTER
    {
        str_query = "(SELECT ingreso_broaster, COUNT(*), '', '', '' FROM modificables";
        str_query += " WHERE DATE(fecha) = '"+ui->dateEdit_exportar->date().toString("yyyy-MM-dd")+"'";
        str_query += ") UNION ALL (";
        str_query += "SELECT SUM(ingreso_broaster), COUNT(*), '', '', '' FROM modificables";
        str_query += " WHERE DATE(fecha) < '"+ui->dateEdit_exportar->date().toString("yyyy-MM-dd")+"'";
        str_query += ") UNION ALL (";
        str_query += "SELECT SUM(prod.medida*c_h_prod.cantidad), COUNT(*), '', '', ''";
        str_query += " FROM comprobante c";
        str_query += " JOIN comprobante_has_producto c_h_prod ON c.id = c_h_prod.comprobante_id";
        str_query += " JOIN producto prod ON prod.id = c_h_prod.producto_id";
        str_query += " JOIN familia fam ON fam.id = prod.familia_id";
        str_query += " WHERE c.estado_item_nombre = '" + QString(ACTIVO) + "'";
        str_query += " AND fam.nombre = 'Pollo Broaster'";
        str_query += " AND c.operacion_item_nombre = '"+QString(VENTA)+"'";
        str_query += " AND DATE(c.fecha_emision) < '"+ui->dateEdit_exportar->date().toString("yyyy-MM-dd")+"'";
        str_query += ") UNION ALL (";
        str_query += "SELECT prod.nombre, SUM(prod.medida*c_h_prod.cantidad), SUM(c_h_prod.cantidad)";
        str_query += ", SUM(c_h_prod.precio)/SUM(c_h_prod.cantidad), SUM(c_h_prod.precio)";
        str_query += " FROM comprobante c";
        str_query += " JOIN comprobante_has_producto c_h_prod ON c.id = c_h_prod.comprobante_id";
        str_query += " JOIN producto prod ON prod.id = c_h_prod.producto_id";
        str_query += " JOIN familia fam ON fam.id = prod.familia_id";
        str_query += " WHERE c.estado_item_nombre = '" + QString(ACTIVO) + "'";
        str_query += " AND fam.nombre = 'Pollo Broaster'";
        str_query += " AND c.operacion_item_nombre = '"+QString(VENTA)+"'";
        str_query += " AND DATE(c.fecha_emision) = '"+ui->dateEdit_exportar->date().toString("yyyy-MM-dd")+"'";
        str_query += " GROUP BY prod.id)";
        //str_query += " ORDER BY c.operacion_item_nombre DESC, c.pago_item_nombre, prod.nombre";

        double ingreso_broaster = 0.0, ingreso_broaster_ayer = 0.0, venta_ayer = 0.0;
        qDebug()<<str_query<<endl;
        if(query.exec(str_query)){
            query.next();
            ingreso_broaster = query.value(0).toDouble();
            query.next();
            ingreso_broaster_ayer = query.value(0).toDouble();
            query.next();
            venta_ayer = query.value(0).toDouble();

            xlsx.setColumnWidth(1, 40);
            xlsx.setColumnWidth(2, 20);
            xlsx.setColumnWidth(3, 20);
            xlsx.setColumnWidth(4, 20);
            xlsx.setColumnWidth(5, 20);

            // /////////////////////////////

            QXlsx::Format format1;
            format1.setPatternBackgroundColor(QColor(255, 255, 0));
            format1.setFontColor(QColor(Qt::black));
            format1.setFontSize(12);
            format1.setFontBold(true);
            format1.setFontUnderline(QXlsx::Format::FontUnderlineSingle);
            format1.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
            format1.setBorderStyle(QXlsx::Format::BorderDashDotDot);

            xlsx.mergeCells("A"+QString().setNum(i)+":E"+QString().setNum(i));

            xlsx.write("A" + QString().setNum(i), "Pollo Broaster", format1);
            xlsx.write("B" + QString().setNum(i), "", format1);
            xlsx.write("C" + QString().setNum(i), "", format1);
            xlsx.write("D" + QString().setNum(i), "", format1);
            xlsx.write("E" + QString().setNum(i), "", format1);

            //  ///////////////////////////////////////////////////////////
            format1.setFontUnderline(QXlsx::Format::FontUnderlineNone);
            format1.setFontBold(false);
            i++;

            xlsx.write("A" + QString().setNum(i), "Nombre", format1);
            xlsx.write("B" + QString().setNum(i), "Real", format1);
            xlsx.write("C" + QString().setNum(i), "Cantidad", format1);
            xlsx.write("D" + QString().setNum(i), "P. Unit", format1);
            xlsx.write("E" + QString().setNum(i), "P. Total", format1);

            i++;

            format1.setPatternBackgroundColor(QColor(200, 255, 150));

            double real_total = 0.0, total = 0.0;
            while(query.next()){
                QString nombre, real, cantidad, p_unit, p_total;
                nombre = query.value(0).toString();
                real = QString().setNum(query.value(1).toDouble(), ' ', 3);
                cantidad = QString().setNum(query.value(2).toDouble(), ' ', 3);
                p_unit = QString().setNum(query.value(3).toDouble(), ' ', 1);
                p_total = QString().setNum(query.value(4).toDouble(), ' ', 1);

                real_total += real.toDouble();
                total += p_total.toDouble();

                format1.setFontColor(QColor(Qt::black));
                format1.setFontSize(12);
                format1.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
                format1.setBorderStyle(QXlsx::Format::BorderDashDotDot);

                xlsx.write("A" + QString().setNum(i), nombre, format1);

                xlsx.write("B" + QString().setNum(i), real, format1);

                format1.setHorizontalAlignment(QXlsx::Format::AlignRight);

                xlsx.write("C" + QString().setNum(i), cantidad, format1);

                xlsx.write("D" + QString().setNum(i), p_unit, format1);

                xlsx.write("E" + QString().setNum(i)
                           , QString().setNum(p_total.toDouble(), ' ', 1), format1);

                i++;
            }
            real_broaster = real_total;
            total_pollo_broaster = total;
            format1.setPatternBackgroundColor(QColor(255, 0, 0));
            format1.setFontColor(QColor(Qt::white));

            xlsx.write("B" + QString().setNum(i), real_total, format1);

            format1.setPatternBackgroundColor(QColor(200, 255, 150));
            format1.setFontColor(QColor(Qt::black));

            xlsx.write("E" + QString().setNum(i), total, format1);

            format1.setPatternBackgroundColor(QColor(255, 0, 0));
            format1.setFontColor(QColor(Qt::white));

            i+=2;
            xlsx.write("A" + QString().setNum(i), "Queda (Ayer)", format1);
            xlsx.write("A" + QString().setNum(i+1), "Ingreso", format1);
            xlsx.write("A" + QString().setNum(i+2), "Venta", format1);
            xlsx.write("A" + QString().setNum(i+3), "Queda (Mañana)", format1);

            xlsx.write("B" + QString().setNum(i), QString().setNum(ingreso_broaster_ayer-venta_ayer, ' ', 3), format1);
            xlsx.write("B" + QString().setNum(i+1), QString().setNum(ingreso_broaster, ' ', 3), format1);
            xlsx.write("B" + QString().setNum(i+2), QString().setNum(real_total, ' ', 3), format1);
            queda_manana_broaster = ingreso_broaster_ayer
                    -venta_ayer+ingreso_broaster-real_total;
            xlsx.write("B" + QString().setNum(i+3), QString().setNum(queda_manana_broaster, ' ', 3), format1);
        }else{
            return;
        }
    }

    xlsx.addSheet("Gaseosas");

    i = 1;
    double total_gaseosas = 0.0;
    // BEBIDAS
    {
        str_query  = "SELECT t.nombre, t.cant_ent_ah - t.cant_sal_ah, t.cant_ent_h, t.cant_sal_h, t.costo, t.p_total";
        str_query += " FROM (SELECT prod.nombre AS nombre";
        str_query += ", IFNULL(t_ent_ah.cantidad, 0) AS cant_ent_ah, IFNULL(t_sal_ah.cantidad, 0) AS cant_sal_ah";
        str_query += ", IFNULL(t_ent_h.cantidad, 0) AS cant_ent_h, IFNULL(t_sal_h.cantidad, 0) AS cant_sal_h";
        str_query += ", IFNULL(t_ent_costo.costo, 0) AS costo, IFNULL(t_sal_p_total.p_total, 0) AS p_total";

        str_query += " FROM producto prod";

        // ANTES DE HOY

        str_query += " LEFT JOIN (SELECT prod_ent.nombre AS nombre, SUM(c_h_prod_ent.cantidad) AS cantidad FROM comprobante c_ent";
        str_query += " JOIN comprobante_has_producto c_h_prod_ent ON ";
        str_query += "((c_ent.operacion_item_nombre = '"+QString(COMPRA)+"'";
        str_query += " OR c_ent.operacion_item_nombre = '"+QString(EGRESOS)+"')";
        str_query += " AND c_ent.id = c_h_prod_ent.comprobante_id)";
        str_query += " JOIN producto prod_ent ON prod_ent.id = c_h_prod_ent.producto_id";
        str_query += " JOIN familia fam ON prod_ent.familia_id = fam.id";
        str_query += " WHERE DATE(c_ent.fecha_emision) < '"+ui->dateEdit_exportar->date().toString("yyyy-MM-dd")+"'";
        str_query += " AND c_ent.estado_item_nombre = '" + QString(ACTIVO) + "'";
        str_query += " AND fam.nombre = 'Bebidas'";
        str_query += " GROUP BY prod_ent.nombre) AS t_ent_ah ON prod.nombre = t_ent_ah.nombre";

        str_query += " LEFT JOIN (SELECT prod_sal.nombre AS nombre, SUM(c_h_prod_sal.cantidad) AS cantidad FROM comprobante c_sal";
        str_query += " JOIN comprobante_has_producto c_h_prod_sal ON ";
        str_query += "(c_sal.operacion_item_nombre = '"+QString(VENTA)+"'";
        str_query += " AND c_sal.id = c_h_prod_sal.comprobante_id)";
        str_query += " JOIN producto prod_sal ON prod_sal.id = c_h_prod_sal.producto_id";
        str_query += " JOIN familia fam ON prod_sal.familia_id = fam.id";
        str_query += " WHERE DATE(c_sal.fecha_emision) < '"+ui->dateEdit_exportar->date().toString("yyyy-MM-dd")+"'";
        str_query += " AND c_sal.estado_item_nombre = '" + QString(ACTIVO) + "'";
        str_query += " AND fam.nombre = 'Bebidas'";
        str_query += " GROUP BY prod_sal.nombre) AS t_sal_ah ON prod.nombre = t_sal_ah.nombre";

        // HOY

        str_query += " LEFT JOIN (SELECT prod_ent.nombre AS nombre, SUM(c_h_prod_ent.cantidad) AS cantidad FROM comprobante c_ent";
        str_query += " JOIN comprobante_has_producto c_h_prod_ent ON ";
        str_query += "((c_ent.operacion_item_nombre = '"+QString(COMPRA)+"'";
        str_query += " OR c_ent.operacion_item_nombre = '"+QString(EGRESOS)+"')";
        str_query += " AND c_ent.id = c_h_prod_ent.comprobante_id)";
        str_query += " JOIN producto prod_ent ON prod_ent.id = c_h_prod_ent.producto_id";
        str_query += " JOIN familia fam ON prod_ent.familia_id = fam.id";
        str_query += " WHERE DATE(c_ent.fecha_emision) = '"+ui->dateEdit_exportar->date().toString("yyyy-MM-dd")+"'";
        str_query += " AND c_ent.estado_item_nombre = '" + QString(ACTIVO) + "'";
        str_query += " AND fam.nombre = 'Bebidas'";
        str_query += " GROUP BY prod_ent.nombre) AS t_ent_h ON prod.nombre = t_ent_h.nombre";

        str_query += " LEFT JOIN (SELECT prod_sal.nombre AS nombre, SUM(c_h_prod_sal.cantidad) AS cantidad FROM comprobante c_sal";
        str_query += " JOIN comprobante_has_producto c_h_prod_sal ON ";
        str_query += "(c_sal.operacion_item_nombre = '"+QString(VENTA)+"'";
        str_query += " AND c_sal.id = c_h_prod_sal.comprobante_id)";
        str_query += " JOIN producto prod_sal ON prod_sal.id = c_h_prod_sal.producto_id";
        str_query += " JOIN familia fam ON prod_sal.familia_id = fam.id";
        str_query += " WHERE DATE(c_sal.fecha_emision) = '"+ui->dateEdit_exportar->date().toString("yyyy-MM-dd")+"'";
        str_query += " AND c_sal.estado_item_nombre = '" + QString(ACTIVO) + "'";
        str_query += " AND fam.nombre = 'Bebidas'";
        str_query += " GROUP BY prod_sal.nombre) AS t_sal_h ON prod.nombre = t_sal_h.nombre";

        // Costo
        str_query += " LEFT JOIN (SELECT prod_ent.nombre AS nombre, SUM(c_h_prod_ent.precio) AS costo FROM comprobante c_ent";
        str_query += " JOIN comprobante_has_producto c_h_prod_ent ON ";
        str_query += "((c_ent.operacion_item_nombre = '"+QString(COMPRA)+"'";
        str_query += " OR c_ent.operacion_item_nombre = '"+QString(EGRESOS)+"')";
        str_query += " AND c_ent.id = c_h_prod_ent.comprobante_id)";
        str_query += " JOIN producto prod_ent ON prod_ent.id = c_h_prod_ent.producto_id";
        str_query += " JOIN familia fam ON prod_ent.familia_id = fam.id";
        str_query += " WHERE DATE(c_ent.fecha_emision) = '"+ui->dateEdit_exportar->date().toString("yyyy-MM-dd")+"'";
        str_query += " AND c_ent.estado_item_nombre = '" + QString(ACTIVO) + "'";
        str_query += " AND fam.nombre = 'Bebidas'";
        str_query += " GROUP BY prod_ent.nombre) AS t_ent_costo ON prod.nombre = t_ent_costo.nombre";

        // P. Total
        str_query += " LEFT JOIN (SELECT prod_sal.nombre AS nombre, SUM(c_h_prod_sal.precio) AS p_total FROM comprobante c_sal";
        str_query += " JOIN comprobante_has_producto c_h_prod_sal ON ";
        str_query += "(c_sal.operacion_item_nombre = '"+QString(VENTA)+"'";
        str_query += " AND c_sal.id = c_h_prod_sal.comprobante_id)";
        str_query += " JOIN producto prod_sal ON prod_sal.id = c_h_prod_sal.producto_id";
        str_query += " JOIN familia fam ON prod_sal.familia_id = fam.id";
        str_query += " WHERE DATE(c_sal.fecha_emision) = '"+ui->dateEdit_exportar->date().toString("yyyy-MM-dd")+"'";
        str_query += " AND c_sal.estado_item_nombre = '" + QString(ACTIVO) + "'";
        str_query += " AND fam.nombre = 'Bebidas'";
        str_query += " GROUP BY prod_sal.nombre) AS t_sal_p_total ON prod.nombre = t_sal_p_total.nombre";

        str_query += " JOIN familia fam ON prod.familia_id = fam.id";

        str_query += " WHERE";
        //str_query += " WHERE t_ent_h.nombre = t_sal_h.nombre";
        //str_query += " AND t_ent_ah.nombre = t_sal_ah.nombre";
        //str_query += " (IFNULL(t_ent_ah.cantidad, 0) <> 0";
        //str_query += " OR IFNULL(t_sal_ah.cantidad, 0) <> 0)";
        //str_query += " OR (IFNULL(t_ent_h.cantidad, 0) <> 0";
        //str_query += " OR IFNULL(t_sal_h.cantidad, 0) <> 0)";
        //str_query += " AND fam.nombre = 'Bebidas'";
        str_query += " fam.nombre = 'Bebidas'";

        str_query += " GROUP BY prod.nombre";

        str_query += ") AS t";

        i++;
        qDebug()<<str_query<<endl;
        if(query.exec(str_query)){
            xlsx.setColumnWidth(1, 40);
            xlsx.setColumnWidth(2, 20);
            xlsx.setColumnWidth(3, 20);
            xlsx.setColumnWidth(4, 20);
            xlsx.setColumnWidth(5, 20);
            xlsx.setColumnWidth(6, 20);
            xlsx.setColumnWidth(7, 20);
            xlsx.setColumnWidth(8, 20);
            xlsx.setColumnWidth(9, 20);

            QXlsx::Format format1;
            format1.setPatternBackgroundColor(QColor(255, 255, 0));
            format1.setFontColor(QColor(Qt::black));
            format1.setFontSize(12);
            format1.setFontBold(true);
            format1.setFontUnderline(QXlsx::Format::FontUnderlineSingle);
            format1.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
            format1.setBorderStyle(QXlsx::Format::BorderDashDotDot);

            xlsx.mergeCells("A"+QString().setNum(i)+":I"+QString().setNum(i));

            xlsx.write("A" + QString().setNum(i), "Bebidas", format1);
            xlsx.write("B" + QString().setNum(i), "", format1);
            xlsx.write("C" + QString().setNum(i), "", format1);
            xlsx.write("D" + QString().setNum(i), "", format1);
            xlsx.write("E" + QString().setNum(i), "", format1);
            xlsx.write("F" + QString().setNum(i), "", format1);
            xlsx.write("G" + QString().setNum(i), "", format1);
            xlsx.write("H" + QString().setNum(i), "", format1);
            xlsx.write("I" + QString().setNum(i), "", format1);

            //  ///////////////////////////////////////////////////////////

            i++;

            format1.setPatternBackgroundColor(QColor(255, 255, 0));
            format1.setFontColor(QColor(Qt::black));
            format1.setFontSize(12);
            format1.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
            format1.setBorderStyle(QXlsx::Format::BorderDashDotDot);

            format1.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
            xlsx.write("A" + QString().setNum(i), "Nombre", format1);

            format1.setPatternBackgroundColor(QColor(255, 0, 0));
            format1.setFontColor(QColor(Qt::white));

            xlsx.write("B" + QString().setNum(i), "Queda(Ayer)", format1);
            xlsx.write("C" + QString().setNum(i), "Ingreso", format1);
            xlsx.write("D" + QString().setNum(i), "Venta", format1);
            xlsx.write("E" + QString().setNum(i), "Queda(Mañana)", format1);

            format1.setPatternBackgroundColor(QColor(255, 255, 0));
            format1.setFontColor(QColor(Qt::black));

            xlsx.write("F" + QString().setNum(i), "Costo Unit.", format1);
            xlsx.write("G" + QString().setNum(i), "Costo", format1);
            xlsx.write("H" + QString().setNum(i), "P. Unit.", format1);
            xlsx.write("I" + QString().setNum(i), "P. Total", format1);

            format1.setPatternBackgroundColor(QColor(255, 255, 255));

            i++;

            format1.setFontUnderline(QXlsx::Format::FontUnderlineNone);
            format1.setFontBold(false);

            format1.setPatternBackgroundColor(QColor(200, 255, 150));
            format1.setFontColor(QColor(Qt::black));

            double total_costo = 0.0, total = 0.0;
            while(query.next()){
                QString nombre, saldo_ayer, hoy_ent, hoy_sal, costo_unit, costo, p_unit, p_total;
                nombre = query.value(0).toString();
                saldo_ayer = query.value(1).toString();
                hoy_ent = query.value(2).toString();
                hoy_sal = query.value(3).toString();
                costo = query.value(4).toString();
                if(hoy_ent.toDouble() == 0.0){
                    costo_unit = QString().setNum(0.0, ' ', 3);
                }else{
                    costo_unit = QString().setNum(costo.toDouble()/hoy_ent.toDouble(), ' ', 3);
                }
                p_total = query.value(5).toString();
                if(hoy_sal.toDouble() == 0.0){
                    p_unit = QString().setNum(0.0, ' ', 1);
                }else{
                    p_unit = QString().setNum(p_total.toDouble()/hoy_sal.toDouble(), ' ', 1);
                }
                total_costo += costo.toDouble();
                total+= p_total.toDouble();

                format1.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
                format1.setBorderStyle(QXlsx::Format::BorderDashDotDot);
                xlsx.write("A" + QString().setNum(i), nombre, format1);
                format1.setHorizontalAlignment(QXlsx::Format::AlignRight);
                format3.setBorderStyle(QXlsx::Format::BorderThin);

                format1.setPatternBackgroundColor(QColor(255, 0, 0));
                format1.setFontColor(QColor(Qt::white));

                xlsx.write("B" + QString().setNum(i), QString().setNum(saldo_ayer.toDouble(), ' ', 3), format1);
                xlsx.write("C" + QString().setNum(i), QString().setNum(hoy_ent.toDouble(), ' ', 3), format1);
                xlsx.write("D" + QString().setNum(i), QString().setNum(hoy_sal.toDouble(), ' ', 3), format1);
                xlsx.write("E" + QString().setNum(i)
                           , QString().setNum(saldo_ayer.toDouble()+(hoy_ent.toDouble()-hoy_sal.toDouble()), ' ', 3), format1);

                format1.setPatternBackgroundColor(QColor(200, 255, 150));
                format1.setFontColor(QColor(Qt::black));

                xlsx.write("F" + QString().setNum(i), QString().setNum(costo_unit.toDouble(), ' ', 1), format1);
                xlsx.write("G" + QString().setNum(i), QString().setNum(costo.toDouble(), ' ', 1), format1);
                xlsx.write("H" + QString().setNum(i), QString().setNum(p_unit.toDouble(), ' ', 1), format1);
                xlsx.write("I" + QString().setNum(i), QString().setNum(p_total.toDouble(), ' ', 1), format1);
                i++;
            }
            total_gaseosas = total;
            format1.setPatternBackgroundColor(QColor(200, 255, 150));
            format1.setFontColor(QColor(Qt::black));

            xlsx.write("G" + QString().setNum(i), total_costo, format1);

            format1.setPatternBackgroundColor(QColor(200, 255, 150));
            format1.setFontColor(QColor(Qt::black));

            xlsx.write("I" + QString().setNum(i), total, format1);
        }else{
            return;
        }
    }

    xlsx.addSheet("R. Ingresos y Egresos");
    i = 1;

    // Ingresos y Egresos
    {
        str_query = "(SELECT dinero_inicial, venta_otros_conceptos";
        str_query += ", ingreso_pollo, personal_pollo, baja_pollo";
        str_query += ", chicharron_pollo, cortesia_pollo, ingreso_broaster, COUNT(*) FROM modificables";
        str_query += " WHERE DATE(fecha) = '"+ui->dateEdit_exportar->date().toString("yyyy-MM-dd")+"'";

        str_query += ")UNION ALL(";

        str_query += "SELECT dinero_inicial, venta_otros_conceptos";
        str_query += ", SUM(ingreso_pollo), SUM(personal_pollo), SUM(baja_pollo)";
        str_query += ", SUM(chicharron_pollo), SUM(cortesia_pollo), SUM(ingreso_broaster), COUNT(*) FROM modificables";
        str_query += " WHERE DATE(fecha) < '"+ui->dateEdit_exportar->date().toString("yyyy-MM-dd")+"'";

        str_query += ")UNION ALL(";

        str_query += "SELECT SUM(prod.medida*c_h_prod.cantidad), COUNT(*), '', '', '', '', '', '', ''";
        str_query += " FROM comprobante c";
        str_query += " JOIN comprobante_has_producto c_h_prod ON c.id = c_h_prod.comprobante_id";
        str_query += " JOIN producto prod ON prod.id = c_h_prod.producto_id";
        str_query += " JOIN familia fam ON fam.id = prod.familia_id";
        str_query += " WHERE c.estado_item_nombre = '" + QString(ACTIVO) + "'";
        str_query += " AND fam.nombre = 'Pollo a la Brasa'";
        str_query += " AND c.operacion_item_nombre = '"+QString(VENTA)+"'";
        str_query += " AND DATE(c.fecha_emision) < '"+ui->dateEdit_exportar->date().toString("yyyy-MM-dd")+"'";

        str_query += ")UNION ALL(";

        str_query += "SELECT SUM(prod.medida*c_h_prod.cantidad), COUNT(*), '', '', '', '', '', '', ''";
        str_query += " FROM comprobante c";
        str_query += " JOIN comprobante_has_producto c_h_prod ON c.id = c_h_prod.comprobante_id";
        str_query += " JOIN producto prod ON prod.id = c_h_prod.producto_id";
        str_query += " JOIN familia fam ON fam.id = prod.familia_id";
        str_query += " WHERE c.estado_item_nombre = '" + QString(ACTIVO) + "'";
        str_query += " AND fam.nombre = 'Pollo Broaster'";
        str_query += " AND c.operacion_item_nombre = '"+QString(VENTA)+"'";
        str_query += " AND DATE(c.fecha_emision) < '"+ui->dateEdit_exportar->date().toString("yyyy-MM-dd")+"'";

        str_query += ")UNION ALL(";

        str_query += "SELECT prod.nombre, SUM(c_h_prod.precio)";
        str_query += ", '', '', '', '', '', '', ''";
        str_query += " FROM comprobante c";
        str_query += " JOIN comprobante_has_producto c_h_prod ON c.id = c_h_prod.comprobante_id";
        str_query += " JOIN producto prod ON prod.id = c_h_prod.producto_id";
        str_query += " JOIN familia fam ON prod.familia_id = fam.id";
        str_query += " WHERE c.estado_item_nombre = '" + QString(ACTIVO) + "'";
        str_query += " AND c.operacion_item_nombre = '" + QString(VENTA) + "'";
        str_query += " AND familia_id <> 1";str_query += " AND familia_id <> 5";str_query += " AND familia_id <> 6";
        str_query += " AND DATE(c.fecha_emision) = '"+ui->dateEdit_exportar->date().toString("yyyy-MM-dd")+"'";
        str_query += " GROUP BY prod.nombre)";
        //str_query += " ORDER BY prod.nombre";

        double total_ingresos = 0.0, total_egresos = 0.0;
        double real_brasa_ayer = 0.0, real_broaster_ayer = 0.0;
        double dinero_inicial_ayer = 0.0, venta_otros_conceptos_ayer = 0.0
                , ingreso_pollo_ayer = 0.0, personal_pollo_ayer = 0.0, baja_pollo_ayer = 0.0
                , chicharron_pollo_ayer = 0.0, cortesia_pollo_ayer = 0.0, ingreso_broaster_ayer = 0.0;
        double dinero_inicial = 0.0, venta_otros_conceptos = 0.0
                , ingreso_pollo = 0.0, personal_pollo = 0.0, baja_pollo = 0.0
                , chicharron_pollo = 0.0, cortesia_pollo = 0.0, ingreso_broaster = 0.0;
        qDebug()<<str_query<<endl;
        if(query.exec(str_query)){
            query.next();
            dinero_inicial = query.value(0).toDouble();
            venta_otros_conceptos = query.value(1).toDouble();
            ingreso_pollo = query.value(2).toDouble();
            personal_pollo = query.value(3).toDouble();
            baja_pollo = query.value(4).toDouble();
            chicharron_pollo = query.value(5).toDouble();
            cortesia_pollo = query.value(6).toDouble();
            ingreso_broaster = query.value(7).toDouble();
            query.next();
            dinero_inicial_ayer = query.value(0).toDouble();
            venta_otros_conceptos_ayer = query.value(1).toDouble();
            ingreso_pollo_ayer = query.value(2).toDouble();
            personal_pollo_ayer = query.value(3).toDouble();
            baja_pollo_ayer = query.value(4).toDouble();
            chicharron_pollo_ayer = query.value(5).toDouble();
            cortesia_pollo_ayer = query.value(6).toDouble();
            ingreso_broaster_ayer = query.value(7).toDouble();
            query.next();
            real_brasa_ayer = query.value(0).toDouble();
            query.next();
            real_broaster_ayer = query.value(0).toDouble();

            xlsx.setColumnWidth(1, 40);
            xlsx.setColumnWidth(2, 20);
            xlsx.setColumnWidth(3, 50);

            xlsx.setColumnWidth(4, 20);
            // /////////////////////////////

            QXlsx::Format format1;
            format1.setPatternBackgroundColor(QColor(255, 255, 0));
            format1.setFontColor(QColor(Qt::black));
            format1.setFontSize(12);
            format1.setFontBold(true);
            format1.setFontUnderline(QXlsx::Format::FontUnderlineSingle);
            format1.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
            format1.setBorderStyle(QXlsx::Format::BorderDashDotDot);

            xlsx.mergeCells("A"+QString().setNum(i)+":B"+QString().setNum(i));

            xlsx.write("A" + QString().setNum(i), "Ingresos", format1);
            xlsx.write("B" + QString().setNum(i), "", format1);

            xlsx.mergeCells("C"+QString().setNum(i)+":D"+QString().setNum(i));

            xlsx.write("C" + QString().setNum(i), "Egresos", format1);
            xlsx.write("D" + QString().setNum(i), "", format1);
            //  ///////////////////////////////////////////////////////////
            format1.setFontUnderline(QXlsx::Format::FontUnderlineNone);
            format1.setFontBold(false);
            i++;
            xlsx.write("A" + QString().setNum(i), "Nombre", format1);
            xlsx.write("B" + QString().setNum(i), "Monto", format1);

            xlsx.write("C" + QString().setNum(i), "Nombre", format1);
            xlsx.write("D" + QString().setNum(i), "Monto", format1);

            i++;

            format1.setPatternBackgroundColor(QColor(200, 255, 150));
            format1.setFontColor(QColor(Qt::black));
            format1.setFontSize(12);
            format1.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
            format1.setBorderStyle(QXlsx::Format::BorderDashDotDot);

            xlsx.write("A" + QString().setNum(i), "Pollo a la Brasa", format1);

            format1.setHorizontalAlignment(QXlsx::Format::AlignRight);

            xlsx.write("B"+ QString().setNum(i), QString().setNum(total_pollo_brasa, ' ', 1), format1);

            i++;

            format1.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
            xlsx.write("A" + QString().setNum(i), "Pollo Broaster", format1);

            format1.setHorizontalAlignment(QXlsx::Format::AlignRight);
            xlsx.write("B"+ QString().setNum(i), QString().setNum(total_pollo_broaster, ' ', 1), format1);

            i++;

            double total = 0.0;
            while(query.next()){
                QString nombre, precio;
                nombre = query.value(0).toString();
                precio = QString().setNum(query.value(1).toDouble(), ' ', 1);

                total += precio.toDouble();

                format1.setPatternBackgroundColor(QColor(200, 255, 150));
                format1.setFontColor(QColor(Qt::black));
                format1.setFontSize(12);
                format1.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
                format1.setBorderStyle(QXlsx::Format::BorderDashDotDot);

                xlsx.write("A" + QString().setNum(i), nombre, format1);

                format1.setHorizontalAlignment(QXlsx::Format::AlignRight);

                xlsx.write("B"+ QString().setNum(i), precio, format1);

                i++;
            }
            format1.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
            xlsx.write("A" + QString().setNum(i), "Bebidas", format1);

            format1.setHorizontalAlignment(QXlsx::Format::AlignRight);
            xlsx.write("B"+ QString().setNum(i), QString().setNum(total_gaseosas, ' ', 1), format1);

            i++;
            format1.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
            xlsx.write("A" + QString().setNum(i), "Otros Conceptos", format1);

            format1.setHorizontalAlignment(QXlsx::Format::AlignRight);
            xlsx.write("B"+ QString().setNum(i), QString().setNum(venta_otros_conceptos, ' ', 1), format1);

            i++;
            format1.setHorizontalAlignment(QXlsx::Format::AlignRight);
            total = total+total_pollo_brasa+total_pollo_broaster
                    +total_gaseosas+venta_otros_conceptos;
            xlsx.write("B"+ QString().setNum(i), QString().setNum(total, ' ', 1), format1);

            total_ingresos = total;

            int ii = 2;
            str_query = "SELECT prod.nombre, SUM(c_h_prod.precio), c.nombre";
            str_query += " FROM comprobante c";
            str_query += " JOIN comprobante_has_producto c_h_prod ON c.id = c_h_prod.comprobante_id";
            str_query += " JOIN producto prod ON prod.id = c_h_prod.producto_id";
            str_query += " JOIN familia fam ON fam.id = prod.familia_id";
            str_query += " WHERE c.estado_item_nombre = '" + QString(ACTIVO) + "'";
            str_query += " AND c.operacion_item_nombre = '" + QString(EGRESOS) + "'";
            str_query += " AND DATE(c.fecha_emision) = '"+ui->dateEdit_exportar->date().toString("yyyy-MM-dd")+"'";
            str_query += " AND prod.nombre IS NOT NULL";
            str_query += " GROUP BY c.id";
            //str_query += " ORDER BY c.operacion_item_nombre, prod.nombre)";

            qDebug()<<"hola"<<endl;
            qDebug()<<str_query<<endl;
            if(query.exec(str_query)){
                QXlsx::Format format1;

                ii++;

                format1.setPatternBackgroundColor(QColor(150, 150, 255));
                format1.setFontColor(QColor(Qt::black));
                format1.setFontSize(12);
                format1.setBorderStyle(QXlsx::Format::BorderDashDotDot);
                total = 0.0;
                while(query.next()){
                    QString nombre, precio, nombre_comprobante;
                    nombre = query.value(0).toString();
                    precio = QString().setNum(query.value(1).toDouble(), ' ', 1);
                    nombre_comprobante = query.value(2).toString();

                    total += precio.toDouble();

                    format1.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
                    xlsx.write("C" + QString().setNum(ii), nombre+" - "+nombre_comprobante, format1);

                    format1.setHorizontalAlignment(QXlsx::Format::AlignRight);
                    xlsx.write("D"+ QString().setNum(ii), precio, format1);

                    ii++;
                }
                total_egresos = total;
                format1.setHorizontalAlignment(QXlsx::Format::AlignRight);
                xlsx.write("D"+ QString().setNum(ii), QString().setNum(total, ' ', 1), format1);
            }
            double queda_ayer = ingreso_pollo_ayer
                    -real_brasa_ayer-ingreso_broaster_ayer//-real_broaster_ayer
                    -baja_pollo_ayer-personal_pollo_ayer
                    -chicharron_pollo_ayer-cortesia_pollo_ayer;

            int i_mayor = 0;
            if(i > ii)
                i_mayor = i;
            else
                i_mayor = ii;

            i_mayor++;

            format1.setPatternBackgroundColor(QColor(255, 255, 0));
            format1.setFontColor(QColor(Qt::black));
            format1.setFontSize(12);
            format1.setFontBold(true);
            format1.setFontUnderline(QXlsx::Format::FontUnderlineSingle);
            format1.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
            format1.setBorderStyle(QXlsx::Format::BorderDashDotDot);

            xlsx.mergeCells("C"+QString().setNum(i_mayor)+":D"+QString().setNum(i_mayor));

            xlsx.write("C" + QString().setNum(i_mayor), "KARDEX DE POLLO", format1);
            xlsx.write("D" + QString().setNum(i_mayor), "", format1);

            i_mayor++;
            format1.setPatternBackgroundColor(QColor(255, 0, 0));
            format1.setFontColor(QColor(Qt::white));
            format1.setFontSize(12);
            format1.setBorderStyle(QXlsx::Format::BorderDashDotDot);
            format1.setFontBold(false);
            format1.setFontUnderline(QXlsx::Format::FontUnderlineNone);

            format1.setHorizontalAlignment(QXlsx::Format::AlignRight);
            xlsx.write("C" + QString().setNum(i_mayor), "Queda(Ayer)", format1);
            format1.setHorizontalAlignment(QXlsx::Format::AlignLeft);
            xlsx.write("D" + QString().setNum(i_mayor), QString().setNum(queda_ayer, ' ', 3), format1);

            i_mayor++;
            format1.setHorizontalAlignment(QXlsx::Format::AlignRight);
            xlsx.write("C" + QString().setNum(i_mayor), "Ingreso", format1);
            format1.setHorizontalAlignment(QXlsx::Format::AlignLeft);
            xlsx.write("D" + QString().setNum(i_mayor), QString().setNum(ingreso_pollo, ' ', 3), format1);
            i_mayor++;
            format1.setHorizontalAlignment(QXlsx::Format::AlignRight);
            xlsx.write("C" + QString().setNum(i_mayor), "Pollo a la Brasa", format1);
            format1.setHorizontalAlignment(QXlsx::Format::AlignLeft);
            xlsx.write("D" + QString().setNum(i_mayor), QString().setNum(real_brasa, ' ', 3), format1);
            i_mayor++;
            format1.setHorizontalAlignment(QXlsx::Format::AlignRight);
            xlsx.write("C" + QString().setNum(i_mayor), "Ingreso Broaster", format1);
            format1.setHorizontalAlignment(QXlsx::Format::AlignLeft);
            xlsx.write("D" + QString().setNum(i_mayor), QString().setNum(ingreso_broaster, ' ', 3), format1);
            i_mayor++;
            format1.setHorizontalAlignment(QXlsx::Format::AlignRight);
            xlsx.write("C" + QString().setNum(i_mayor), "Baja", format1);
            format1.setHorizontalAlignment(QXlsx::Format::AlignLeft);
            xlsx.write("D" + QString().setNum(i_mayor), QString().setNum(baja_pollo, ' ', 3), format1);
            i_mayor++;
            format1.setHorizontalAlignment(QXlsx::Format::AlignRight);
            xlsx.write("C" + QString().setNum(i_mayor), "Personal", format1);
            format1.setHorizontalAlignment(QXlsx::Format::AlignLeft);
            xlsx.write("D" + QString().setNum(i_mayor), QString().setNum(personal_pollo, ' ', 3), format1);
            /*
            i_mayor++;
            format1.setHorizontalAlignment(QXlsx::Format::AlignRight);
            xlsx.write("C" + QString().setNum(i_mayor), "Chicharron", format1);
            format1.setHorizontalAlignment(QXlsx::Format::AlignLeft);
            xlsx.write("D" + QString().setNum(i_mayor), QString().setNum(chicharron_pollo, ' ', 3), format1);*/
            i_mayor++;
            format1.setHorizontalAlignment(QXlsx::Format::AlignRight);
            xlsx.write("C" + QString().setNum(i_mayor), "Cortesia", format1);
            format1.setHorizontalAlignment(QXlsx::Format::AlignLeft);
            xlsx.write("D" + QString().setNum(i_mayor), QString().setNum(cortesia_pollo, ' ', 3), format1);
            double queda_manana = queda_ayer+ingreso_pollo
                    -real_brasa-ingreso_broaster
                    -baja_pollo-personal_pollo
                    -chicharron_pollo-cortesia_pollo;
            //double queda_manana_brasa = queda_manana-queda_manana_broaster;
            i_mayor++;
            format1.setHorizontalAlignment(QXlsx::Format::AlignRight);
            xlsx.write("C" + QString().setNum(i_mayor), "Queda(Mañana) Brasa", format1);
            format1.setHorizontalAlignment(QXlsx::Format::AlignLeft);
            xlsx.write("D" + QString().setNum(i_mayor), QString().setNum(queda_manana, ' ', 3), format1);
            /*
            i_mayor++;
            format1.setHorizontalAlignment(QXlsx::Format::AlignRight);
            xlsx.write("C" + QString().setNum(i_mayor), "Queda(Mañana) Broaster", format1);
            format1.setHorizontalAlignment(QXlsx::Format::AlignLeft);
            xlsx.write("D" + QString().setNum(i_mayor), QString().setNum(queda_manana_broaster, ' ', 3), format1);
            i_mayor++;
            format1.setHorizontalAlignment(QXlsx::Format::AlignRight);
            xlsx.write("C" + QString().setNum(i_mayor), "Queda(Mañana)", format1);
            format1.setHorizontalAlignment(QXlsx::Format::AlignLeft);
            xlsx.write("D" + QString().setNum(i_mayor), QString().setNum(queda_manana, ' ', 3), format1);*/

            if(i > ii)
                i_mayor = i;
            else
                i_mayor = ii;

            i_mayor++;

            format1.setPatternBackgroundColor(QColor(255, 255, 0));
            format1.setFontColor(QColor(Qt::black));
            format1.setFontSize(12);
            format1.setFontBold(true);
            format1.setFontUnderline(QXlsx::Format::FontUnderlineSingle);
            format1.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
            format1.setBorderStyle(QXlsx::Format::BorderDashDotDot);

            xlsx.mergeCells("A"+QString().setNum(i_mayor)+":B"+QString().setNum(i_mayor));

            xlsx.write("A" + QString().setNum(i_mayor), "TOTALES", format1);
            xlsx.write("B" + QString().setNum(i_mayor), "", format1);

            format1.setPatternBackgroundColor(QColor(200, 255, 150));
            format1.setFontColor(QColor(Qt::black));
            format1.setFontSize(12);
            format1.setBorderStyle(QXlsx::Format::BorderDashDotDot);
            format1.setFontBold(false);
            format1.setFontUnderline(QXlsx::Format::FontUnderlineNone);

            i_mayor++;
            format1.setHorizontalAlignment(QXlsx::Format::AlignRight);
            xlsx.write("A" + QString().setNum(i_mayor), "T. Ingresos", format1);
            format1.setHorizontalAlignment(QXlsx::Format::AlignLeft);
            xlsx.write("B" + QString().setNum(i_mayor), QString().setNum(total_ingresos, ' ', 1), format1);
            i_mayor++;
            format1.setHorizontalAlignment(QXlsx::Format::AlignRight);
            xlsx.write("A" + QString().setNum(i_mayor), "T. Egresos", format1);
            format1.setHorizontalAlignment(QXlsx::Format::AlignLeft);
            xlsx.write("B" + QString().setNum(i_mayor), QString().setNum(total_egresos, ' ', 1), format1);
            i_mayor++;
            //format1.setHorizontalAlignment(QXlsx::Format::AlignRight);
            //xlsx.write("A" + QString().setNum(i_mayor), "Total", format1);
            format1.setHorizontalAlignment(QXlsx::Format::AlignLeft);
            xlsx.write("B" + QString().setNum(i_mayor), QString().setNum(total_ingresos-total_egresos, ' ', 1), format1);
            i_mayor++;
            format1.setHorizontalAlignment(QXlsx::Format::AlignRight);
            xlsx.write("A" + QString().setNum(i_mayor), "Dinero Inicial", format1);
            format1.setHorizontalAlignment(QXlsx::Format::AlignLeft);
            xlsx.write("B" + QString().setNum(i_mayor), QString().setNum(dinero_inicial, ' ', 1), format1);
            i_mayor++;
            format1.setHorizontalAlignment(QXlsx::Format::AlignRight);
            xlsx.write("A" + QString().setNum(i_mayor), "Total", format1);
            format1.setHorizontalAlignment(QXlsx::Format::AlignLeft);
            xlsx.write("B" + QString().setNum(i_mayor), QString().setNum(dinero_inicial+total_ingresos-total_egresos, ' ', 1), format1);
        }else{
            return;
        }
    }

    xlsx.selectSheet("R. Ingresos y Egresos");
    QString name_reporte = "reporte_" + ui->dateEdit_exportar->date().toString("dd-MM-yyyy")+ ".xlsx";

    if(xlsx.saveAs(directory + "/" + name_reporte))
    {
        QProcess process;
        // machelo
        process.startDetached("\"C:\\Program Files\\Microsoft Office\\Office15\\EXCEL.exe\" \""
                              + directory + "/" + name_reporte + "\"");
        // bill mc quack caja 2
        //process.startDetached("\"C:\\Program Files\\Microsoft Office\\Office16\\EXCEL.exe\" \""
        //                      + directory + "/" + name_reporte + "\"");

        //QDesktopServices::openUrl(QUrl(directory + "/reporte.xlsx"));
    }else{
        on_myProccess_finished(0, QProcess::NormalExit);
    }

        //QMessageBox::information(this, "Información", "Felicidades. Finalizo la operacion sin errores.", "Ok");

    /*else {
        QMessageBox::critical(this, "Error", "No finalizo corretamente.");
    }
*/
    //delete sender();
}

void ComprobanteBuscar::on_checkBox_compra_toggled(bool checked)
{
    pos = 0;

    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    //ui->tableWidget->setColumnCount(0);


    set_buscar();
}

void ComprobanteBuscar::on_checkBox_venta_toggled(bool checked)
{
    pos = 0;

    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    //ui->tableWidget->setColumnCount(0);

    set_buscar();
}

void ComprobanteBuscar::on_checkBox_egresos_toggled(bool checked)
{
    pos = 0;

    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    //ui->tableWidget->setColumnCount(0);

    set_buscar();
}

void ComprobanteBuscar::on_checkBox_boleta_toggled(bool checked)
{
    pos = 0;

    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    //ui->tableWidget->setColumnCount(0);

    set_buscar();
}

void ComprobanteBuscar::on_checkBox_factura_toggled(bool checked)
{
    pos = 0;

    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    //ui->tableWidget->setColumnCount(0);

    set_buscar();
}

void ComprobanteBuscar::on_pushButton_deliverys_clicked()
{
    Delivery* w = new Delivery(this);
    w->setWindowFlag(Qt::Dialog);
    w->setAttribute(Qt::WA_DeleteOnClose);
    w->setWindowModality(Qt::WindowModal);

    w->show();
}
void ComprobanteBuscar::on_datosModificables_closing()
{
    DatosModifcables* w = (DatosModifcables*)sender();

    int opc = w->getOpc();

    if(opc == DatosModifcables::CONTINUAR){
        QString str_query;
        str_query = "SELECT SUM(IF(delivery_estado_item_nombre = 'Activo', 1, 0)) FROM comprobante";
        str_query += " WHERE DATE(fecha_emision) = '"+QDate::currentDate().toString("yyyy-MM-dd")+"'";
        QSqlQuery query;
        qDebug()<<str_query<<endl;
        if(query.exec(str_query)){
            query.next();
            int isDelivery = query.value(0).toInt();
            if(isDelivery > 0){
                QMessageBox::warning(this, "Advertencia", "Corrija los gastos en delivery", "Aceptar");
                return;
            }
        }else{
            QMessageBox::critical(this, "Error", ".", "Aceptar");
            return;
        }

        QString curPath = QDir::currentPath();
        QFileDialog dialog(this, "Abrir Archivo", curPath);
        dialog.setAcceptMode(QFileDialog::AcceptOpen);
        dialog.setFileMode(QFileDialog::Directory);
        //dialog.setNameFilter(tr("Archivo Excel (*.xlsx)"));

        if (dialog.exec()) {
            directory = dialog.directory().absolutePath();
        }
        else {
            return;
        }
        qDebug() << directory << endl;
        QProcess* process = new QProcess(this);
        process->start("taskkill /F /IM EXCEL.exe");
        connect(process, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(on_myProccess_finished(int,QProcess::ExitStatus)));
    }
}

void ComprobanteBuscar::on_checkBox_ticket_toggled(bool checked)
{
    pos = 0;

    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    //ui->tableWidget->setColumnCount(0);

    set_buscar();
}

void ComprobanteBuscar::on_checkBox_ncredito_toggled(bool checked)
{
    pos = 0;

    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    //ui->tableWidget->setColumnCount(0);

    set_buscar();
}

void ComprobanteBuscar::on_pushButton_enviarSUNAT_clicked()
{
    QTableWidgetItem* item = ui->tableWidget->currentItem();
    if(!item){
        QMessageBox::warning(this, "Advertencia", "Seleccione item", "Aceptar");
        return;
    }
/*
    QString operacion = ui->tableWidget->item(item->row(), 8)->text();
    QString documento = ui->tableWidget->item(item->row(), 9)->text();
    if(operacion.compare(VENTA) == 0 && (documento.compare(BOLETA) == 0 || documento.compare(FACTURA) == 0)){
        SoapSunatCall obj;
        obj.set_str_certificatekeyfile((char *)"mycertificate.pem");
        obj.set_str_keyfile((char *)"mykey.pem");
        obj.set_str_password((char *)"4t48gWXqdd358Msx");
        obj.set_str_cafile((char *)"myca.pem");
        obj.set_str_certfile((char *)"mycert.pem");
        obj.set_str_sec_username((char *)"20498590587ELBIBAM6");
        obj.set_str_sec_password((char *)"nArputo-");

        QString str_ruc = "20498590587";
        //QString str_razon = "20498590587";
        QString str_serie = ui->lineEdit_serie->text();
        QString str_numero = ui->lineEdit_numero->text();

        string fileName_zip = QString(str_ruc+"-"+"03"+"-"+str_serie+"-"+str_numero+".ZIP").toStdString();
        string fileName_xml = QString(str_ruc+"-"+"03"+"-"+str_serie+"-"+str_numero+".XML").toStdString();


        obj.set_str_zip_xml_filename((char *)fileName_zip.c_str());
        obj.set_str_zip_xml_out_filename((char *)"res.zip");
        obj.set_str_xml_file((char *)fileName_xml.c_str());
        obj.set_str_id_signature((char *)"SignatureKG");


        qDebug()<<fileName_xml.c_str()<<endl;
        if(obj.xmlDSig() < 0){
            QMessageBox::information(this, "Informacion", "No se puede firmar el documento","Aceptar");
            SYSTEM->rollback();
            return;
        }else{
            qDebug() << "Se firmo el doc" << endl;
        }



        QString app_dir = QCoreApplication::applicationDirPath();

        QString cmp_release = app_dir.mid(app_dir.length()-8, 9);
        QString str_rel = "";
        //qDebug() << cmp_release << endl;
        if(cmp_release.compare("/release") == 0){
            str_rel = "/release";
            //qDebug() << str_rel << endl;
        }

        app_dir = app_dir.mid(0, app_dir.length() - str_rel.length());
        app_dir.replace("/","\\");
        QString str_zip_dir = "\"" + app_dir + "\\"+QString(fileName_zip.c_str())+"\"";

        QString command = "\"c:\\Program Files\\7-Zip\\7z.exe\" a -tzip"
                          " "+str_zip_dir+" "+QString(fileName_xml.c_str());

        qDebug()<<command<<endl;
        QProcess process;
        process.start(command);
        //QProcess::execute(command);
        process.waitForFinished();
        //QMessageBox::warning(this, "Advertencia", "No se puede anular boletas y facturas aun.", "Aceptar");
        return;
    }*/
}

void ComprobanteBuscar::on_pushButton_imprimir_clicked()
{
    QTableWidgetItem* item = ui->tableWidget->currentItem();
    if(!item){
        QMessageBox::warning(this, "Advertencia", "Seleccione item", "Aceptar");
        return;
    }
    QString operacion = ui->tableWidget->item(item->row(), 8)->text();
    QString documento = ui->tableWidget->item(item->row(), 9)->text();
    if(operacion.compare(VENTA) == 0 && (documento.compare(BOLETA) == 0 || documento.compare(FACTURA) == 0)){
        QString id = ui->tableWidget->item(item->row(), 0)->text();

        QString str_query;
        str_query = "(SELECT serie, numero, fecha_emision, delivery, vuelto, pago_item_nombre, direccion, nombre FROM comprobante WHERE id='"+id+"')";
        str_query += "UNION ALL";
        str_query += "(SELECT per.cod, per.nombre, per.direccion, per.tipo_item_nombre, '', '', '', '' FROM comprobante_has_persona com_h_per";
        str_query += " JOIN persona per ON per.cod = com_h_per.persona_cod";
        str_query += " WHERE com_h_per.comprobante_id='"+id+"')";
        str_query += "UNION ALL";
        str_query += "(SELECT com_h_prod.cantidad, 'UND', prod.nombre, com_h_prod.precio, '', '', '', '' FROM comprobante_has_producto com_h_prod";
        str_query += " JOIN producto prod ON prod.id = com_h_prod.producto_id";
        str_query += " WHERE com_h_prod.comprobante_id='"+id+"')";
        QSqlQuery query;

        qDebug()<<str_query<<endl;
        if(query.exec(str_query)){
            query.next();
            QString serie = query.value(0).toString();
            QString numero = query.value(1).toString();
            QDateTime dt = query.value(2).toDateTime();
            double delivery_query = query.value(3).toDouble();
            double vuelto_query = query.value(4).toDouble();
            QString pago_query = query.value(5).toString();
            QString direccion_query = query.value(6).toString();
            QString nombre_query = query.value(7).toString();

            query.next();
            QString cod = query.value(0).toString();
            //QString nombre_query = query.value(1).toString();
            //QString direccion_query = query.value(2).toString();
            QString tipo_persona = query.value(3).toString();

            if(tipo_persona.compare(Persona::cliente_eventual) == 0 || tipo_persona.compare(Persona::cliente_delivery) == 0){
                cod = "";
            }
            qDebug()<<serie+"-"+numero<<endl;
            QFile file("print_epson.dat");
            if (!file.open(QFile::WriteOnly)) {
                qDebug() << "Could not open file for writing";
                return;
            }
            QDataStream out(&file);
            out.setVersion(QDataStream::Qt_4_8);

            SYSTEM->insertImage(out, "npedido_leal_gris_epson.png");
            /*
            for(int j = 0; j < 4; j++){
                out<<(qint64)0x0A;
            }*/
            //qDebug()<<getchar()<<endl;
            SYSTEM->epson_lineFeed(out);

            QVector<QString> centerTexts;

            qDebug()<<"LUNA"<<endl;
            if(documento.compare("Boleta") == 0){
                centerTexts.push_back("BOLETA ELECTRONICA");
                centerTexts.push_back("                                          ");

                centerTexts.push_back("Serie-Nro: " + serie + "-" + numero);
                if(cod.length() == 8){
                    centerTexts.push_back("DNI: "+cod);
                }
                QString nombre = "Nombre: " + nombre_query;
                nombre.replace("\t", " ");
                nombre.replace("\n", " ");
                nombre.replace("\r", " ");

                if(nombre.length() > 42){
                    int recorrido = 0;
                    while(recorrido < nombre.length()) {
                        if(nombre[recorrido+42-1] == ' '){
                            centerTexts.push_back(nombre.mid(recorrido, 42));
                            recorrido += 42;
                        }else{
                            if(nombre[recorrido+42] == ' '){
                                centerTexts.push_back(nombre.mid(recorrido, 42));
                                recorrido += 42;
                            }else{
                                int i = recorrido+42-2, count=0;
                                while(nombre[i] != ' '){
                                    i--;
                                    count++;
                                }
                                if(count == 41){
                                    centerTexts.push_back(nombre.mid(recorrido, 42));
                                    recorrido += 42;
                                }else{
                                    centerTexts.push_back(nombre.mid(recorrido, 42-count-1));
                                    recorrido += 42-count-1;
                                }
                            }
                        }

                    }

                }else{
                    centerTexts.push_back(nombre);
                }
            }
            if(documento.compare("Factura") == 0){
                centerTexts.push_back("FACTURA ELECTRONICA");
                centerTexts.push_back("                                          ");
                centerTexts.push_back("Serie-Nro: " + serie + "-" + numero);

                centerTexts.push_back("RUC: " + cod);

                QString nombre = "Razon S.: " + nombre_query;
                nombre.replace("\t", " ");
                nombre.replace("\n", " ");
                nombre.replace("\r", " ");

                if(nombre.length() > 42){
                    int recorrido = 0;
                    while(recorrido < nombre.length()) {
                        if(nombre[recorrido+42-1] == ' '){
                            centerTexts.push_back(nombre.mid(recorrido, 42));
                            recorrido += 42;
                        }else{
                            if(nombre[recorrido+42] == ' '){
                                centerTexts.push_back(nombre.mid(recorrido, 42));
                                recorrido += 42;
                            }else{
                                int i = recorrido+42-2, count=0;
                                while(nombre[i] != ' '){
                                    i--;
                                    count++;
                                }
                                if(count == 41){
                                    centerTexts.push_back(nombre.mid(recorrido, 42));
                                    recorrido += 42;
                                }else{
                                    centerTexts.push_back(nombre.mid(recorrido, 42-count-1));
                                    recorrido += 42-count-1;
                                }
                            }
                        }

                    }

                }else{
                    centerTexts.push_back(nombre);
                }
            }

            QString direccion = "Direccion: " + direccion_query;
            direccion.replace("\t", " ");
            direccion.replace("\n", " ");
            direccion.replace("\r", " ");

            if(direccion.length() > 42){
                int recorrido = 0;
                while(recorrido < direccion.length()) {
                    if(direccion[recorrido+42-1] == ' '){
                        centerTexts.push_back(direccion.mid(recorrido, 42));
                        recorrido += 42;
                    }else{
                        if(direccion[recorrido+42] == ' '){
                            centerTexts.push_back(direccion.mid(recorrido, 42));
                            recorrido += 42;
                        }else{
                            int i = recorrido+42-2, count=0;
                            while(direccion[i] != ' '){
                                i--;
                                count++;
                            }
                            if(count == 41){
                                centerTexts.push_back(direccion.mid(recorrido, 42));
                                recorrido += 42;
                            }else{
                                centerTexts.push_back(direccion.mid(recorrido, 42-count-1));
                                recorrido += 42-count-1;
                            }
                        }
                    }

                }

            }else{
                centerTexts.push_back(direccion);
            }

            centerTexts.push_back("                                          ");
            centerTexts.push_back("Fecha: " + dt.date().toString("dd/MM/yyyy"));
            centerTexts.push_back("Hora: " + dt.time().toString("hh:mm:ss"));
            centerTexts.push_back("==========================================");
            centerTexts.push_back("Cant      Unidad  Descripcion             ");
            centerTexts.push_back("                             P.Total      ");
            // Peso 13 P.Unit. 12 P.Total 13
            centerTexts.push_back("------------------------------------------");

            SYSTEM->centerTexts(centerTexts, 42);

            //out << (qint64)0x1D4C0002;

            for(int i = 0; i < centerTexts.size(); i++){
                //SYSTEM->insert_left_spaces(centerTexts[i], 3);
                //SYSTEM->insert_right_spaces(centerTexts[i], 3);

                SYSTEM->epson_printText(out, centerTexts[i]);
                //SYSTEM->epson_lineFeed(out);
            }
            //SYSTEM->epson_lineFeed(out);
            double total = 0.0, igv = 0.18;
            while(query.next()){
                QString cantidad = QString().setNum(query.value(0).toDouble(), ' ', 0);
                QString unidad = query.value(1).toString();
                QString descripcion = query.value(2).toString();
                //QString peso = ui->tableWidget->item(i, 3)->text();
                //QString p_unit = ui->tableWidget->item(i, 4)->text();
                QString p_total = QString().setNum(query.value(3).toDouble(), ' ', 1);
                total += query.value(3).toDouble();
                //double p_total = ui->tableWidget->item(i, 5)->text().toDouble();
                //QString str_p_total = QString().setNum(p_total, ' ', 1);

                SYSTEM->align_left(cantidad, 10);
                SYSTEM->align_left(unidad, 8);
                SYSTEM->align_left(descripcion, 49);
                //SYSTEM->align_left(peso, 13);
                //SYSTEM->align_left(p_unit, 12);
                SYSTEM->justified(p_total, 13);

                QString text = cantidad + unidad
                        + descripcion + "    "
                        + p_total;
                SYSTEM->epson_printText(out, text);

                centerTexts.push_back("                                    ");
            }

            QString str_st = QString().setNum(total / (1.0 + igv), ' ', 2);
            SYSTEM->justified(str_st, 6);
            QString str_i = QString().setNum(total / (1.0 + igv) * igv, ' ', 2);
            SYSTEM->justified(str_i, 6);
            QString str_t = QString().setNum(total, ' ', 2);
            SYSTEM->justified(str_t, 6);
            QString str_d = QString().setNum(delivery_query, ' ', 1);
            SYSTEM->justified(str_d, 6);
            QString str_v = QString().setNum(vuelto_query, ' ', 1);
            SYSTEM->justified(str_v, 6);
            SYSTEM->justified(str_t, 13);
            QString str_sub_total = "Sub-Total: " + str_st;
            QString str_igv = "IGV: " + str_i;
            QString str_total = "Total: " + str_t;
            QString str_delivery = "Delivery: " + str_d;
            QString str_vuelto = "Vuelto: " + str_v;
            QVector<QString> totales;
            totales.push_back("------------------------");
            totales.push_back(str_sub_total);
            totales.push_back(str_igv);
            totales.push_back(str_total);
            totales.push_back("------------------------");
            totales.push_back(str_delivery);
            totales.push_back(str_vuelto);
            QString forma_de_pago = QString("Forma de Pago:" + pago_query.toUpper());
            totales.push_back(SYSTEM->centerText(forma_de_pago, 42));
            totales.push_back(" ");
            QString clave_wifi = "*** Clave Wifi: SIN INTERNET ***";
            totales.push_back(SYSTEM->centerText(clave_wifi, 42));

            SYSTEM->rightTexts(totales, 42);

            SYSTEM->epson_lineFeed(out);

            for(int i = 0; i < totales.size(); i++){
                SYSTEM->epson_printText(out, totales[i]);
                //SYSTEM->epson_lineFeed(out);
            }
            qDebug()<<"ANTES DE DOM"<<endl;
            //SYSTEM->epson_lineFeed(out);

            //SYSTEM->insertImage(out, "footer.png");

            SYSTEM->epson_lineFeed(out);
            //SYSTEM->epson_printText(out, SYSTEM->centerText(QString("PAGOS LLAMAR A: CRISS: 967252119")));
            //SYSTEM->epson_lineFeed(out);
            SYSTEM->epson_lineFeed(out);
            //SYSTEM->epson_printText(out, SYSTEM->centerText(QString("")));
            QDomDocument dom;
            QString str_ruc = "20498590587";
            string fileName_xml;
            if(documento.compare(BOLETA) == 0){
                fileName_xml = QString(str_ruc+"-"+"03"+"-"+serie+"-"+numero+".XML").toStdString();
            }
            if(documento.compare(FACTURA) == 0){
                fileName_xml = QString(str_ruc+"-"+"01"+"-"+serie+"-"+numero+".XML").toStdString();
            }
            QString str_digestValue;
            {
                QFile file(QString(fileName_xml.c_str()));
                if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
                    return;
                if (!dom.setContent(&file)) {
                    file.close();
                    return;
                }

                // print out the element names of all elements that are direct children
                // of the outermost element.
                QDomElement root = dom.firstChildElement("Invoice");
                QDomNodeList items = root.elementsByTagName("ext:UBLExtensions");
                QDomNode itemnode = items.at(1);

                items = root.elementsByTagName("DigestValue");
                itemnode = items.at(0);

                if(itemnode.isElement()){
                    QDomElement iteme = itemnode.toElement();
                    str_digestValue = iteme.text();
                }
            }
            qDebug()<<"DigestValue: "<<str_digestValue<<endl;

            SYSTEM->epson_printText(out, SYSTEM->centerText(str_digestValue, 42));
            //SYSTEM->epson_printText(out, SYSTEM->centerText(QString("\"'Cambiar a Boleta o Factura'\""), 48));

            SYSTEM->epson_lineFeed(out);
            SYSTEM->epson_lineFeed(out);
            SYSTEM->epson_lineFeed(out);
            SYSTEM->epson_lineFeed(out);
            SYSTEM->epson_lineFeed(out);
            SYSTEM->epson_lineFeed(out);
            SYSTEM->epson_lineFeed(out);


            SYSTEM->epson_paperCut(out);


            file.close();

            QString app_dir = QCoreApplication::applicationDirPath();

            QString cmp_release = app_dir.mid(app_dir.length()-8, 9);
            QString str_rel = "";
            //qDebug() << cmp_release << endl;
            if(cmp_release.compare("/release") == 0){
                str_rel = "/release";
                //qDebug() << str_rel << endl;
            }

            app_dir = app_dir.mid(0, app_dir.length() - str_rel.length());
            app_dir.replace("/","\\");
            app_dir = "\"" + app_dir + "\\" + file.fileName() + "\"";

            //app_dir = QDir::fromNativeSeparators(app_dir);

            //QString lpt_print = QDir::fromNativeSeparators("\\\\localhost\\caja001");
            QString lpt_print = "\"\\\\localhost\\EPSON TM-T88V Receipt\"";
            //QString lpt_print = "\\\\localhost\\caja001";

            //FILE * pFile;
            //char buffer[] = { (char)0x0a };
            //pFile = fopen ("c:\\test.txt", "wb");
            //fwrite (buffer , sizeof(char), sizeof(buffer), pFile);
            //fclose (pFile);
            //system(command.toStdString().c_str());

            //QProcess *myProcess = new QProcess();
            QString command = "copy /b " + app_dir
                                + " " + lpt_print + " < nul";

            /*
            QStringList arguments = QStringList();
            arguments << argument_1;
            connect(myProcess, SIGNAL(started())
                , this, SLOT(on_myProccess_started()));
            connect(myProcess, SIGNAL(finished(int, QProcess::ExitStatus))
                , this, SLOT(on_myProccess_finished(int, QProcess::ExitStatus)));
        */
            //WinExec(command.toStdString().c_str(), SW_HIDE);
            //ShellExecuteA(0, "open", "cmd.exe", command.toStdString().c_str(), 0, SW_HIDE);

            QMessageBox::information(this, "informacion", command,"aceptar");

            {
                QString app_dir = QCoreApplication::applicationDirPath();

                QString cmp_release = app_dir.mid(app_dir.length()-8, 9);
                QString str_rel = "";
                //qDebug() << cmp_release << endl;
                if(cmp_release.compare("/release") == 0){
                    str_rel = "/release";
                    //qDebug() << str_rel << endl;
                }

                app_dir = app_dir.mid(0, app_dir.length() - str_rel.length());
                app_dir = "\""+ app_dir + "\"/chp.exe cmd /c ";
                command = app_dir + command;
            }

            QProcess::execute(command);

        }

    }
}
