#include "delivery.h"
#include "ui_delivery.h"

Delivery::Delivery(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Delivery)
{
    ui->setupUi(this);

    pos = 0;
    size_query = 10;

    firstShow = false;
    afterShow = false;

    ui->dateEdit->setDate(QDate::currentDate());

    ui->tableWidget->setColumnWidth(0, 0);
    ui->tableWidget->setColumnWidth(1, 80);//estado
    ui->tableWidget->setColumnWidth(2, 200);//nombre
    ui->tableWidget->setColumnWidth(3, 80);//deliv. S/.
    ui->tableWidget->setColumnWidth(4, 250);//direccion
    ui->tableWidget->setColumnWidth(5, 200);//referencia
    ui->tableWidget->setColumnWidth(6, 150);//telefono

    QScrollBar* bar = ui->tableWidget->verticalScrollBar();
    connect(bar, SIGNAL(valueChanged(int)), this, SLOT(on_verticalScrollBar_valueChanged(int)));
    connect(bar, SIGNAL(actionTriggered(int)), this, SLOT(on_verticalScrollBar_actionTriggered(int)));

    this->installEventFilter(this);

    ui->tableWidget->installEventFilter(this);
    ui->lineEdit_nombre->installEventFilter(this);
    ui->pushButton_salir->installEventFilter(this);
}

Delivery::~Delivery()
{
    delete ui;
}

void Delivery::select_clientes()
{
    QString nombre = ui->lineEdit_nombre->text();
    nombre = nombre.trimmed();

    QString str_query;
    str_query  = "SELECT com.id, com.delivery_estado_item_nombre, com.nombre, com.delivery";
    str_query += ", com.direccion, per.referencia, concat(per.telefono, ' / ', per.celular)";
    str_query += " FROM comprobante com";
    str_query += " LEFT JOIN comprobante_has_persona com_h_per ON com.id = com_h_per.comprobante_id";
    str_query += " LEFT JOIN persona per ON per.cod = com_h_per.persona_cod";
    str_query += " WHERE com.nombre LIKE '%"+nombre+"%'";
    str_query += " AND DATE(com.fecha_emision) = '"+ui->dateEdit->date().toString("yyyy-MM-dd")+"'";
    str_query += " AND com.operacion_item_nombre = '"+QString(VENTA)+"'";
    QString delivery_estado_item = "";
    QString check_activo, check_inactivo, check_pagado;
    if(ui->checkBox_activo->isChecked()){
        check_activo = ACTIVO;
    }
    if(ui->checkBox_inactivo->isChecked()){
        check_inactivo = INACTIVO;
    }
    if(ui->checkBox_pagado->isChecked()){
        check_pagado = PAGADO;
    }

    if(check_activo.compare("") != 0){
        delivery_estado_item += "com.delivery_estado_item_nombre = '" + check_activo + "'";
    }

    if(check_inactivo.compare("") != 0){
        if(delivery_estado_item.compare("") != 0){
            delivery_estado_item += " OR com.delivery_estado_item_nombre = '" + check_inactivo + "'";
        }else{
            delivery_estado_item += "com.delivery_estado_item_nombre = '" + check_inactivo + "'";
        }
    }
    if(check_pagado.compare("") != 0){
        if(delivery_estado_item.compare("") != 0){
            delivery_estado_item += " OR com.delivery_estado_item_nombre = '" + check_pagado + "'";
        }else{
            delivery_estado_item += "com.delivery_estado_item_nombre = '" + check_pagado + "'";
        }
    }
    if(delivery_estado_item.compare("") != 0){
        str_query += " AND (" + delivery_estado_item + ")";
    }
    str_query += " GROUP BY com.id";
    str_query += " ORDER BY com.fecha_emision DESC, com.nombre ASC";
    str_query += " LIMIT " + QString().setNum(pos) + ", " + QString().setNum(size_query) + "";

    QSqlQuery query;
    qDebug() << str_query << endl;
    if (query.exec(str_query)) {
        disconnect(ui->tableWidget, SIGNAL(itemChanged(QTableWidgetItem*))
                , this, SLOT(on_tableWidget_itemChanged(QTableWidgetItem*)));

        int rowCount = ui->tableWidget->rowCount();
        ui->tableWidget->setRowCount(rowCount + query.size());

        while (query.next()) {
            //ui->tableWidget->setRowHeight(pos, 50);

            QString id = query.value(0).toString();
            QString delivery_estado = query.value(1).toString();
            QString nombre = query.value(2).toString();
            QString delivery = query.value(3).toString();
            QString direccion = query.value(4).toString();
            QString referencia = query.value(5).toString();
            QString telef_cel = query.value(6).toString();

            QTableWidgetItem* item_id = new QTableWidgetItem(id);
            item_id->setToolTip(id);
            QTableWidgetItem* item_delivery_estado = new QTableWidgetItem(delivery_estado);

            QTableWidgetItem* item_nombre = new QTableWidgetItem(nombre);

            QTableWidgetItem* item_delivery = new QTableWidgetItem(QString().setNum(delivery.toDouble(), ' ', 2));

            QTableWidgetItem* item_direccion = new QTableWidgetItem(direccion);

            QTableWidgetItem* item_referencia = new QTableWidgetItem(referencia);
            //item_total->setToolTip(total);
            //item_total->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            QTableWidgetItem* item_telef_cel = new QTableWidgetItem(telef_cel);

            //cantidad = QString().setNum(cantidad.toDouble(), ' ', 3);
            ui->tableWidget->setItem(pos, 0, item_id);
            ui->tableWidget->setItem(pos, 1, item_delivery_estado);
            ui->tableWidget->setItem(pos, 2, item_nombre);
            ui->tableWidget->setItem(pos, 3, item_delivery);
            //ui->tableWidget->setItem(pos, 3, new QTableWidgetItem(cantidad));
            ui->tableWidget->setItem(pos, 4, item_direccion);
            ui->tableWidget->setItem(pos, 5, item_referencia);
            ui->tableWidget->setItem(pos, 6, item_telef_cel);


            if(ui->tableWidget->item(pos, 1)->text().compare("Activo") == 0){
                for(int j = 0; j < ui->tableWidget->columnCount(); j++) {
                    ui->tableWidget->item(pos, j)->setTextColor(QColor(255, 0, 0));
                    ui->tableWidget->item(pos, j)->setBackgroundColor(QColor(130, 255, 50));
                }
            }else{
                if(ui->tableWidget->item(pos, 1)->text().compare("Inactivo") == 0){
                    for(int j = 0; j < ui->tableWidget->columnCount(); j++) {
                        ui->tableWidget->item(pos, j)->setTextColor(QColor(0, 0, 255));
                        ui->tableWidget->item(pos, j)->setBackgroundColor(QColor(130, 255, 50));
                    }
                }else{
                    for(int j = 0; j < ui->tableWidget->columnCount(); j++) {
                        ui->tableWidget->item(pos, j)->setTextColor(QColor(0, 0, 0));
                        ui->tableWidget->item(pos, j)->setBackgroundColor(QColor(255, 255, 255));
                    }
                }
            }
            for(int j=0; j < ui->tableWidget->columnCount(); j++)
                ui->tableWidget->item(pos, j)->setFlags(Qt::ItemIsEnabled
                                                             | Qt::ItemIsSelectable);

            /*
            ui->tableWidget->item(pos, 3)->setFlags(Qt::ItemIsEnabled
                                                    | Qt::ItemIsSelectable
                                                    | Qt::ItemIsEditable);*/
            pos++;
        }

        connect(ui->tableWidget, SIGNAL(itemChanged(QTableWidgetItem*))
                , this, SLOT(on_tableWidget_itemChanged(QTableWidgetItem*)));
        //SYSTEM->table_resize_to_contents(0, ui->tableWidget, size_query);
    }else{

    }

}
void Delivery::on_pushButton_salir_clicked()
{
    this->close();
}

void Delivery::on_tableWidget_itemDoubleClicked(QTableWidgetItem *item)
{
    if(item->column() == 1){//estado
        QListWidget* lw_tipo = new QListWidget(this);
        ((QWidget*)lw_tipo)->setWindowFlag(Qt::Dialog);
        ((QWidget*)lw_tipo)->setWindowModality(Qt::WindowModal);

        ((QAbstractItemView*)lw_tipo)->setIconSize(QSize(48, 48));
        lw_tipo->addItem("Activo");
        lw_tipo->addItem("Inactivo");
        //lw_tipo->item(1)->setIcon(QIcon(":/new/Iconos/dni.png"));
        lw_tipo->addItem("Pagado");
        //lw_tipo->item(2)->setIcon(QIcon(":/new/Iconos/eventual.png"));
        //lw_tipo->addItem(Persona::cliente_delivery);
        //lw_tipo->item(3)->setIcon(QIcon(":/new/Iconos/eventual.png"));
        QFont font;
        font.setPointSize(10);
        ((QWidget*)lw_tipo)->setFont(font);

        connect(lw_tipo, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(on_lw_tipo_itemClicked(QListWidgetItem*)));
        lw_tipo->show();
    }
}

void Delivery::on_lineEdit_nombre_textChanged(const QString &arg1)
{
    pos = 0;

    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    //ui->tableWidget->setColumnCount(0);

    select_clientes();
}
void Delivery::on_verticalScrollBar_actionTriggered(int value)
{
    QScrollBar* bar = ui->tableWidget->verticalScrollBar();

    /*
    qDebug()<<"activation value: "<<value<<endl;
    qDebug()<<"activation bar maximum: "<<bar->maximum()<<endl;
    qDebug()<<"activation bar value: "<<bar->value()<<endl;
    */
    if(bar->value() == bar->maximum()) {
        select_clientes();
    }
}
void Delivery::on_verticalScrollBar_valueChanged(int value)
{
    /*
    QScrollBar* bar = ui->tableWidget->verticalScrollBar();

    if(value == bar->maximum())
        set_buscar();
*/
}
void Delivery::showEvent(QShowEvent *se)
{
    se->accept();

    afterShow = true;

    if(!firstShow){
        on_lineEdit_nombre_textChanged(ui->lineEdit_nombre->text());
        //on_lineEdit_descripcion_buscar_returnPressed();
        firstShow = true;
    }else{
        //on_lineEdit_buscar_textChanged(ui->lineEdit_buscar->text());
    }
}
void Delivery::closeEvent(QCloseEvent *ce)
{
    ce->accept();
    emit closing();
}
bool Delivery::eventFilter(QObject *obj, QEvent *e)
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
                    ui->lineEdit_nombre->setFocus();
                    ui->lineEdit_nombre->setCursorPosition(ui->lineEdit_nombre->text().length());
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
                    select_clientes();
                    return true;
                }
            }break;
            }

        } else {

        }
        return false;
    }
    w_temp = ui->lineEdit_nombre;
    if (obj == w_temp) {
        if (e->type() == QEvent::KeyPress) {
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch (KeyEvent->key())
            {
            case Qt::Key_Return: {
                on_lineEdit_nombre_textChanged(ui->lineEdit_nombre->text());
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
void Delivery::on_lw_tipo_itemClicked(QListWidgetItem *item)
{
    if(SYSTEM->tipo_persona.compare(Persona::administrador) == 0){
        if(item->text().compare(Persona::administrador) == 0){
            QMessageBox::warning(this, "Advertencia", "No es posible esta acción.", "Aceptar");
            return;
        }
    }
    if(SYSTEM->tipo_persona.compare(Persona::cajero) == 0){
        if(item->text().compare(Persona::administrador) == 0 || item->text().compare(Persona::cajero) == 0){
            QMessageBox::warning(this, "Advertencia", "No es posible esta acción.", "Aceptar");
            return;
        }
    }

    QTableWidgetItem* tableItem = ui->tableWidget->currentItem();
    if(!tableItem){
        return;
    }
    int row = tableItem->row();
    QString id = ui->tableWidget->item(row, 0)->text();

    QString str_query;
    str_query  = "UPDATE comprobante SET delivery_estado_item_nombre = '"+item->text()+"'";
    str_query += " WHERE id = '"+id+"'";

    QSqlQuery query;
    qDebug()<<str_query<<endl;
    if(query.exec(str_query)){
        ui->tableWidget->item(row, 1)->setText(item->text());

        if(ui->tableWidget->item(row, 1)->text().compare("Activo") == 0){
            for(int j = 0; j < ui->tableWidget->columnCount(); j++) {
                ui->tableWidget->item(row, j)->setTextColor(QColor(255, 0, 0));
                ui->tableWidget->item(row, j)->setBackgroundColor(QColor(130, 255, 50));
            }
        }else{
            if(ui->tableWidget->item(row, 1)->text().compare("Inactivo") == 0){
                for(int j = 0; j < ui->tableWidget->columnCount(); j++) {
                    ui->tableWidget->item(row, j)->setTextColor(QColor(0, 0, 255));
                    ui->tableWidget->item(row, j)->setBackgroundColor(QColor(130, 255, 50));
                }
            }else{
                for(int j = 0; j < ui->tableWidget->columnCount(); j++) {
                    ui->tableWidget->item(row, j)->setTextColor(QColor(0, 0, 0));
                    ui->tableWidget->item(row, j)->setBackgroundColor(QColor(255, 255, 255));
                }
            }
        }
    }

    QWidget* w = (QWidget*)sender();
    w->close();
}

void Delivery::on_tableWidget_itemChanged(QTableWidgetItem *item)
{
    disconnect(ui->tableWidget, SIGNAL(itemChanged(QTableWidgetItem*))
            , this, SLOT(on_tableWidget_itemChanged(QTableWidgetItem*)));

    qDebug()<<"item changed"<<endl;
    if(item->column() == 3){//delivery
        qDebug()<<"delivery"<<endl;
        QTableWidgetItem* tableItem = ui->tableWidget->currentItem();

        int row = tableItem->row();
        QString id = ui->tableWidget->item(row, 0)->text();

        QString str_query;
        str_query  = "UPDATE comprobante SET delivery = '"+item->text()+"'";
        str_query += " WHERE id = '"+id+"'";
        QSqlQuery query;
        qDebug()<<str_query<<endl;
        if(query.exec(str_query)){

        }else{

        }
    }
    connect(ui->tableWidget, SIGNAL(itemChanged(QTableWidgetItem*))
            , this, SLOT(on_tableWidget_itemChanged(QTableWidgetItem*)));
}

void Delivery::on_checkBox_activo_toggled(bool checked)
{
    pos = 0;

    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    //ui->tableWidget->setColumnCount(0);

    select_clientes();
}

void Delivery::on_checkBox_inactivo_toggled(bool checked)
{
    pos = 0;

    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    //ui->tableWidget->setColumnCount(0);

    select_clientes();
}

void Delivery::on_checkBox_pagado_toggled(bool checked)
{
    pos = 0;

    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    //ui->tableWidget->setColumnCount(0);

    select_clientes();
}

void Delivery::on_dateEdit_dateChanged(const QDate &date)
{
    pos = 0;

    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);

    select_clientes();
}
