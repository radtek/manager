#include "comprobantebuscar.h"
#include "ui_comprobantebuscar.h"

ComprobanteBuscar::ComprobanteBuscar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ComprobanteBuscar)
{
    ui->setupUi(this);

    ui->tableWidget->setAlternatingRowColors(true);

    QDateTime dt_ini = QDateTime::currentDateTime();
    dt_ini.setTime(QTime(0, 0, 0));
    disconnect(ui->dateTimeEdit_ini, SIGNAL(dateTimeChanged(QDateTime)), this, SLOT(on_dateTimeEdit_ini_dateTimeChanged(QDateTime)));
    ui->dateTimeEdit_ini->setDateTime(dt_ini);
    connect(ui->dateTimeEdit_ini, SIGNAL(dateTimeChanged(QDateTime)), this, SLOT(on_dateTimeEdit_ini_dateTimeChanged(QDateTime)));
    QDateTime dt_fin = QDateTime::currentDateTime();
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

    QString fecha_ini = ui->dateTimeEdit_ini->dateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString fecha_fin = ui->dateTimeEdit_fin->dateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString check_efectivo, check_visa, check_masterCard;
    if(ui->checkBox_efectivo->isChecked()){
        check_efectivo = "Efectivo";
    }
    if(ui->checkBox_visa->isChecked()){
        check_visa = "Visa";
    }
    if(ui->checkBox_masterCard->isChecked()){
        check_masterCard = "Master Card";
    }
    QString str_query = "SELECT c.id, c.estado_item_nombre, c.numero, c_h_pla.cantidad, pla.item_nombre, c_h_pla.cantidad * pla.precio";
    str_query += ", c.nombre, c.fecha_emision";
    str_query += " FROM comprobante c";
    str_query += " LEFT JOIN comprobante_has_persona c_h_per ON c.id = c_h_per.comprobante_id";
    str_query += " LEFT JOIN persona per ON c_h_per.persona_cod = per.cod";
    str_query += " JOIN comprobante_has_plato c_h_pla ON c.id = c_h_pla.comprobante_id";
    str_query += " JOIN plato pla ON c_h_pla.plato_item_nombre = pla.item_nombre";
    str_query += " WHERE c.nombre LIKE '%" + nombre + "%'";
    str_query += " AND c.fecha_emision BETWEEN '" + fecha_ini + "' AND '" + fecha_fin + "'";
    //str_query += " AND c.estado_item_nombre <> 'Suspendido'";
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
    //str_query += " GROUP BY c.id";
    str_query += " ORDER BY c.fecha_emision DESC, c.nombre ASC";
    str_query += " LIMIT " + QString().setNum(pos) + ", " + QString().setNum(size_query) + "";

    QSqlQuery query;
    qDebug() << str_query << endl;
    if (query.exec(str_query)) {
        int rowCount = ui->tableWidget->rowCount();
        ui->tableWidget->setRowCount(rowCount + query.size());

        int columnCount = query.record().count();
        ui->tableWidget->setColumnCount(columnCount);

        ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "ID" << "Estado" << "Numero" << "Cantidad" << "Descripción"
                                                   << "Total" << "Nombre" << "Fecha");


        while (query.next()) {
            QString id = query.value(0).toString();
            QString estado = query.value(1).toString();
            QString nro_ticket = QString().setNum(query.value(2).toInt());
            int n = 7 - nro_ticket.length();
            nro_ticket = SYSTEM->zeros(n) + nro_ticket;
            QString numero = nro_ticket;
            QString cantidad = query.value(3).toString();
            QString descripcion = query.value(4).toString();
            QString total = query.value(5).toString();
            QString nombre = query.value(6).toString();
            QString fecha = query.value(7).toDateTime().toString("dd-MM-yyyy hh:mm:ss");

            cantidad = QString().setNum(cantidad.toDouble(), ' ', 3);
            total = QString().setNum(total.toDouble(), ' ', 2);
            ui->tableWidget->setItem(pos, 0, new QTableWidgetItem(id));
            ui->tableWidget->setItem(pos, 1, new QTableWidgetItem(estado));
            ui->tableWidget->setItem(pos, 2, new QTableWidgetItem(numero));
            ui->tableWidget->setItem(pos, 3, new QTableWidgetItem(cantidad));
            ui->tableWidget->setItem(pos, 4, new QTableWidgetItem(descripcion));
            ui->tableWidget->setItem(pos, 5, new QTableWidgetItem(total));
            ui->tableWidget->setItem(pos, 6, new QTableWidgetItem(nombre));
            ui->tableWidget->setItem(pos, 7, new QTableWidgetItem(fecha));

            if(ui->tableWidget->item(pos, 1)->text().compare("Suspendido") == 0){
                ui->tableWidget->item(pos, 1)->setTextColor(QColor(255, 0, 0));
                ui->tableWidget->item(pos, 1)->setBackgroundColor(QColor(30, 255, 50));
                for(int j=0; j<ui->tableWidget->columnCount(); j++)
                    ui->tableWidget->item(pos, j)->setFlags(Qt::ItemIsEnabled
                                                                 | Qt::ItemIsSelectable);
            }
            ++pos;
        }
        SYSTEM->table_resize_to_contents(0, ui->tableWidget, size_query);

        set_total();
    }else{

    }
}
void ComprobanteBuscar::set_total()
{
    QString nombre = ui->lineEdit_buscar->text();
    nombre = nombre.trimmed();

    QString fecha_ini = ui->dateTimeEdit_ini->dateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString fecha_fin = ui->dateTimeEdit_fin->dateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString check_efectivo, check_visa, check_masterCard;
    if(ui->checkBox_efectivo->isChecked()){
        check_efectivo = "Efectivo";
    }
    if(ui->checkBox_visa->isChecked()){
        check_visa = "Visa";
    }
    if(ui->checkBox_masterCard->isChecked()){
        check_masterCard = "Master Card";
    }
    QSqlQuery query;
    QString str_query = "SELECT SUM(c_h_pla.cantidad * pla.precio)";
    str_query += " FROM comprobante c";
    str_query += " LEFT JOIN comprobante_has_persona c_h_per ON c.id = c_h_per.comprobante_id";
    str_query += " LEFT JOIN persona per ON c_h_per.persona_cod = per.cod";
    str_query += " JOIN comprobante_has_plato c_h_pla ON c.id = c_h_pla.comprobante_id";
    str_query += " JOIN plato pla ON c_h_pla.plato_item_nombre = pla.item_nombre";
    str_query += " WHERE c.nombre LIKE '%" + nombre + "%'";
    str_query += " AND c.fecha_emision BETWEEN '" + fecha_ini + "' AND '" + fecha_fin + "'";
    str_query += " AND c.estado_item_nombre = 'Activo'";
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
    qDebug() << str_query << endl;
    if (query.exec(str_query)) {
        query.next();
        double total = query.value(0).toDouble();
        ui->doubleSpinBox_total->setValue(total);
        ui->doubleSpinBox_totalTop->setValue(total);
    }
}

void ComprobanteBuscar::on_lineEdit_buscar_textChanged(const QString& arg)
{
    pos = 0;

    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(0);
    ui->tableWidget->clear();

    set_buscar();
}
void ComprobanteBuscar::on_lineEdit_buscar_returnPressed()
{
    qDebug()<<"return pressed"<<endl;
    pos = 0;

    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(0);
    ui->tableWidget->clear();

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

    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(0);
    ui->tableWidget->clear();

    set_buscar();
}

void ComprobanteBuscar::on_dateTimeEdit_fin_dateTimeChanged(const QDateTime &dateTime)
{
    pos = 0;

    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(0);
    ui->tableWidget->clear();

    set_buscar();
}

void ComprobanteBuscar::on_checkBox_efectivo_toggled(bool checked)
{
    pos = 0;

    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(0);
    ui->tableWidget->clear();

    set_buscar();
}

void ComprobanteBuscar::on_checkBox_visa_toggled(bool checked)
{
    pos = 0;

    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(0);
    ui->tableWidget->clear();

    set_buscar();
}

void ComprobanteBuscar::on_checkBox_masterCard_toggled(bool checked)
{
    pos = 0;

    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(0);
    ui->tableWidget->clear();

    set_buscar();
}

void ComprobanteBuscar::on_pushButton_anular_clicked()
{
    QTableWidgetItem* item = ui->tableWidget->currentItem();
    if(!item){
        QMessageBox::warning(this, "Advertencia", "Seleccione item", "Aceptar");
        return;
    }
    int ret = QMessageBox::warning(this, "Advertencia"
                                   , "Va a anular un documento. ¿Esta seguro(a)?", "Aceptar", "Cancelar");
    switch(ret){
    case 0:{

    }break;
    case 1:{
        return;
    }break;
    }

    QString str_query = "";
    QSqlQuery query;

    QString id = ui->tableWidget->item(item->row(), 0)->text();
    str_query += "UPDATE comprobante SET estado_item_nombre = 'Suspendido'";
    str_query += " WHERE id = " + id;

    if(query.exec(str_query)){
        ui->tableWidget->item(item->row(), 1)->setTextColor(QColor(255, 0, 0));
        ui->tableWidget->item(item->row(), 1)->setBackgroundColor(QColor(30, 255, 50));
        ui->tableWidget->item(item->row(), 1)->setText("Suspendido");

        QString id = ui->tableWidget->item(item->row(), 0)->text();
        for(int i = item->row()-1; i >= 0; i--){
            if(id.compare(ui->tableWidget->item(i, 0)->text()) == 0){
                ui->tableWidget->item(i, 1)->setTextColor(QColor(255, 0, 0));
                ui->tableWidget->item(i, 1)->setBackgroundColor(QColor(30, 255, 50));
                ui->tableWidget->item(i, 1)->setText("Suspendido");
            }else{
                break;
            }
        }
        for(int i = item->row()+1; i < ui->tableWidget->rowCount(); i++){
            if(id.compare(ui->tableWidget->item(i, 0)->text()) == 0){
                ui->tableWidget->item(i, 1)->setTextColor(QColor(255, 0, 0));
                ui->tableWidget->item(i, 1)->setBackgroundColor(QColor(30, 255, 50));
                ui->tableWidget->item(i, 1)->setText("Suspendido");
            }else{
                break;
            }
        }

        SYSTEM->table_resize_to_contents(0, ui->tableWidget, size_query);

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
    QString nombre = ui->tableWidget->item(item->row(), 6)->text();
    QString fecha = ui->tableWidget->item(item->row(), 7)->text();

    w->set_cabecera(numero, fecha, nombre);
    w->set_detalle(id);

    w->ver_documento();

    w->show();
}

void ComprobanteBuscar::on_pushButton_habilitar_clicked()
{
    QTableWidgetItem* item = ui->tableWidget->currentItem();
    if(!item){
        QMessageBox::warning(this, "Advertencia", "Seleccione item", "Aceptar");
        return;
    }
    int ret = QMessageBox::warning(this, "Advertencia"
                                   , "Este documento estará habilitado.", "Aceptar", "Cancelar");
    switch(ret){
    case 0:{

    }break;
    case 1:{
        return;
    }break;
    }

    QString str_query = "";
    QSqlQuery query;

    QString id = ui->tableWidget->item(item->row(), 0)->text();
    str_query += "UPDATE comprobante SET estado_item_nombre = 'Activo'";
    str_query += " WHERE id = " + id;

    if(query.exec(str_query)){
        ui->tableWidget->item(item->row(), 1)->setTextColor(QColor(0, 0, 0));
        ui->tableWidget->item(item->row(), 1)->setBackgroundColor(QColor(255, 255, 255));
        ui->tableWidget->item(item->row(), 1)->setText("Activo");

        QString id = ui->tableWidget->item(item->row(), 0)->text();
        for(int i = item->row()-1; i >= 0; i--){
            if(id.compare(ui->tableWidget->item(i, 0)->text()) == 0){
                ui->tableWidget->item(i, 1)->setTextColor(QColor(0, 0, 0));
                ui->tableWidget->item(i, 1)->setBackgroundColor(QColor(255, 255, 255));
                ui->tableWidget->item(i, 1)->setText("Activo");
            }else{
                break;
            }
        }
        for(int i = item->row()+1; i < ui->tableWidget->rowCount(); i++){
            if(id.compare(ui->tableWidget->item(i, 0)->text()) == 0){
                ui->tableWidget->item(i, 1)->setTextColor(QColor(0, 0, 0));
                ui->tableWidget->item(i, 1)->setBackgroundColor(QColor(255, 255, 255));
                ui->tableWidget->item(i, 1)->setText("Activo");
            }else{
                break;
            }
        }

        SYSTEM->table_resize_to_contents(0, ui->tableWidget, size_query);

        set_total();
    }

}
