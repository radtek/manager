#include "personabuscar.h"
#include "ui_personabuscar.h"

PersonaBuscar::PersonaBuscar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PersonaBuscar)
{
    ui->setupUi(this);

    ui->tableWidget->setColumnWidth(0, 150);
    ui->tableWidget->setColumnWidth(1, 150);
    ui->tableWidget->setColumnWidth(2, 400);
    ui->tableWidget->setColumnWidth(3, 500);

    size_query = 10;

    newTipoSelected = -1;
    newStringChanged = "&%&%&$&((/&)-1";

    pos = 0;

    firstShow = false;
    afterShow = false;

    QScrollBar* bar = ui->tableWidget->verticalScrollBar();
    connect(bar, SIGNAL(valueChanged(int)), this, SLOT(on_verticalScrollBar_valueChanged(int)));
    connect(bar, SIGNAL(actionTriggered(int)), this, SLOT(on_verticalScrollBar_actionTriggered(int)));

    this->installEventFilter(this);

    ui->tableWidget->installEventFilter(this);
    ui->lineEdit_nombre->installEventFilter(this);
    ui->pushButton_salir->installEventFilter(this);
}

PersonaBuscar::~PersonaBuscar()
{
    delete ui;
}

void PersonaBuscar::setOpcPersona(QString opcPersona)
{
    this->opcPersona = opcPersona;

    if(opcPersona.compare(Persona::opc_clientes) == 0){
        setWindowTitle(Persona::opc_clientes);
        ui->label_titulo->setText(Persona::opc_clientes);

        ui->checkBox_1->setIcon(QIcon(":/new/Iconos/sunat.png"));
        ui->checkBox_2->setIcon(QIcon(":/new/Iconos/dni.png"));
        ui->checkBox_3->setIcon(QIcon(":/new/Iconos/eventual.png"));
        ui->checkBox_4->setIcon(QIcon(":/new/Iconos/eventual.png"));

        ui->checkBox_1->setText("Con RUC");
        ui->checkBox_2->setText("Con DNI");
        ui->checkBox_3->setText("Cliente Eventual");
        ui->checkBox_4->setText("Cliente Delivery");
    }
    if(opcPersona.compare(Persona::opc_proveedores) == 0){
        setWindowTitle(Persona::opc_proveedores);
        ui->label_titulo->setText(Persona::opc_proveedores);

        ui->checkBox_1->setIcon(QIcon(":/new/Iconos/proveedor_gorro.jpg"));
        ui->checkBox_2->setIcon(QIcon(":/new/Iconos/transportista.png"));
        ui->checkBox_3->hide();
        ui->checkBox_4->hide();

        ui->checkBox_1->setText("Proveedores");
        ui->checkBox_2->setText("Transportistas");
    }
    if(opcPersona.compare(Persona::opc_usuarios) == 0){
        setWindowTitle(Persona::opc_usuarios);
        ui->label_titulo->setText(Persona::opc_usuarios);

        ui->checkBox_1->setIcon(QIcon(":/new/Iconos/admin.png"));
        ui->checkBox_2->setIcon(QIcon(":/new/Iconos/cajero.png"));
        ui->checkBox_3->hide();
        ui->checkBox_3->hide();

        ui->checkBox_1->setText("Administradores");
        ui->checkBox_2->setText("Cajeros");
    }
}
QString PersonaBuscar::get_cod()
{
    return codigo;
}
QString PersonaBuscar::get_tipoPersona()
{
    return tipoPersona;
}
QString PersonaBuscar::get_nombre()
{
    return nombre;
}
QString PersonaBuscar::get_direccion()
{
    return direccion;
}
void PersonaBuscar::set_buscar()
{
    QString str_query;
    str_query  = "SELECT persona.tipo_item_nombre, persona.cod, persona.nombre, persona.direccion";
    str_query += " FROM persona";
    if(newTipoSelected == 0)
        str_query += " WHERE persona.cod = '"+ui->lineEdit_codigo->text()+"'";
    if(newTipoSelected == 1)
        str_query += " WHERE persona.nombre LIKE '%"+ui->lineEdit_nombre->text()+"%'";
    if(newTipoSelected == 2)
        str_query += " WHERE persona.direccion LIKE '%"+ui->lineEdit_direccion->text()+"%'";

    if(opcPersona.compare(Persona::opc_clientes) == 0){
        str_query +=  " AND (";
        str_query += "persona.tipo_item_nombre = '-1'";
        if(ui->checkBox_1->checkState() == Qt::Checked){
            str_query += " OR persona.tipo_item_nombre = '"+Persona::cliente_ruc+"'";
        }
        if(ui->checkBox_2->checkState() == Qt::Checked){
            str_query += " OR persona.tipo_item_nombre = '"+Persona::cliente_dni+"'";
        }
        if(ui->checkBox_3->checkState() == Qt::Checked){
            str_query += " OR persona.tipo_item_nombre = '"+Persona::cliente_eventual+"'";
        }
        if(ui->checkBox_4->checkState() == Qt::Checked){
            str_query += " OR persona.tipo_item_nombre = '"+Persona::cliente_delivery+"'";
        }
        str_query += ")";
    }
    if(opcPersona.compare(Persona::opc_proveedores) == 0){
        str_query +=  " AND (";
        str_query += "persona.tipo_item_nombre = '-1'";
        if(ui->checkBox_1->checkState() == Qt::Checked){
            str_query += " OR persona.tipo_item_nombre = '"+Persona::proveedor+"'";
        }
        if(ui->checkBox_2->checkState() == Qt::Checked){
            str_query += " OR persona.tipo_item_nombre = '"+Persona::transportista+"'";
        }
        str_query += ")";
    }
    if(opcPersona.compare(Persona::opc_usuarios) == 0){
        str_query +=  " AND (";
        str_query += "persona.tipo_item_nombre = '-1'";
        if(ui->checkBox_1->checkState() == Qt::Checked){
            str_query += " OR persona.tipo_item_nombre = '"+Persona::administrador+"'";
        }
        if(ui->checkBox_2->checkState() == Qt::Checked){
            str_query += " OR persona.tipo_item_nombre = '"+Persona::cajero+"'";
        }
        str_query += ")";
    }
    str_query += " LIMIT " + QString().setNum(pos) + ", " + QString().setNum(size_query) + "";
    QSqlQuery query;
    qDebug()<<str_query<<endl;
    if(query.exec(str_query)){
        //ui->tableWidget->clearContents();
        int rowCount = ui->tableWidget->rowCount();
        ui->tableWidget->setRowCount(rowCount + query.size());

        qDebug()<<"11"<<endl;
        while(query.next()){
            QTableWidgetItem* item_tipo_item_nombre = new QTableWidgetItem(query.value(0).toString());
            QTableWidgetItem* item_cod = new QTableWidgetItem(query.value(1).toString());
            QTableWidgetItem* item_nombre = new QTableWidgetItem(query.value(2).toString());
            QTableWidgetItem* item_direccion = new QTableWidgetItem(query.value(3).toString());

            ui->tableWidget->setItem(pos, 0, item_tipo_item_nombre);
            ui->tableWidget->setItem(pos, 1, item_cod);
            ui->tableWidget->setItem(pos, 2, item_nombre);
            ui->tableWidget->setItem(pos, 3, item_direccion);

            for(int j=0; j < ui->tableWidget->columnCount(); j++)
                ui->tableWidget->item(pos, j)->setFlags(Qt::ItemIsEnabled
                                                             | Qt::ItemIsSelectable);

            ++pos;
        }
        qDebug()<<"22"<<endl;
        /*
        for(int i=0; i<ui->tableWidget->rowCount(); i++){
            for(int j=0; j<ui->tableWidget->columnCount(); j++){
                ui->tableWidget->item(i, j)->setFlags(Qt::ItemIsSelectable
                                                             | Qt::ItemIsEnabled);
            }
        }*/
        qDebug()<<"33"<<endl;
    }
}
void PersonaBuscar::on_pushButton_ingresar_clicked()
{
    if(opcPersona.compare(Persona::opc_clientes) == 0){
        QListWidget* lw_tipo = new QListWidget(this);
        ((QWidget*)lw_tipo)->setWindowFlag(Qt::Dialog);
        ((QWidget*)lw_tipo)->setWindowModality(Qt::WindowModal);

        ((QAbstractItemView*)lw_tipo)->setIconSize(QSize(48, 48));
        lw_tipo->addItem(Persona::cliente_ruc);
        lw_tipo->item(0)->setIcon(QIcon(":/new/Iconos/sunat.png"));
        lw_tipo->addItem(Persona::cliente_dni);
        lw_tipo->item(1)->setIcon(QIcon(":/new/Iconos/dni.png"));
        lw_tipo->addItem(Persona::cliente_eventual);
        lw_tipo->item(2)->setIcon(QIcon(":/new/Iconos/eventual.png"));
        lw_tipo->addItem(Persona::cliente_delivery);
        lw_tipo->item(3)->setIcon(QIcon(":/new/Iconos/eventual.png"));
        QFont font;
        font.setPointSize(10);
        ((QWidget*)lw_tipo)->setFont(font);

        connect(lw_tipo, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(on_lw_tipo_itemClicked(QListWidgetItem*)));
        lw_tipo->show();
    }
    if(opcPersona.compare(Persona::opc_proveedores) == 0){
        QListWidget* lw_tipo = new QListWidget(this);
        ((QWidget*)lw_tipo)->setWindowFlag(Qt::Dialog);
        ((QWidget*)lw_tipo)->setWindowModality(Qt::WindowModal);

        ((QAbstractItemView*)lw_tipo)->setIconSize(QSize(48, 48));
        lw_tipo->addItem(Persona::proveedor);
        lw_tipo->item(0)->setIcon(QIcon(":/new/Iconos/proveedor_gorro.jpg"));
        lw_tipo->addItem(Persona::transportista);
        lw_tipo->item(1)->setIcon(QIcon(":/new/Iconos/transportista.png"));
        QFont font;
        font.setPointSize(10);
        ((QWidget*)lw_tipo)->setFont(font);

        connect(lw_tipo, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(on_lw_tipo_itemClicked(QListWidgetItem*)));
        lw_tipo->show();
    }
    if(opcPersona.compare(Persona::opc_usuarios) == 0){
        QListWidget* lw_tipo = new QListWidget(this);
        ((QWidget*)lw_tipo)->setWindowFlag(Qt::Dialog);
        ((QWidget*)lw_tipo)->setWindowModality(Qt::WindowModal);

        ((QAbstractItemView*)lw_tipo)->setIconSize(QSize(48, 48));
        lw_tipo->addItem(Persona::administrador);
        lw_tipo->item(0)->setIcon(QIcon(":/new/Iconos/admin.png"));
        lw_tipo->addItem(Persona::cajero);
        lw_tipo->item(1)->setIcon(QIcon(":/new/Iconos/cajero.png"));
        QFont font;
        font.setPointSize(10);
        ((QWidget*)lw_tipo)->setFont(font);

        connect(lw_tipo, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(on_lw_tipo_itemClicked(QListWidgetItem*)));
        lw_tipo->show();
    }
}

void PersonaBuscar::on_pushButton_modificar_clicked()
{
    if(!ui->tableWidget->currentItem()){
        QMessageBox::warning(this, "Advertencia", "Seleccione Un item.", "Aceptar");
        return;
    }
    QString tipoPersona = ui->tableWidget->item(ui->tableWidget->currentItem()->row(), 0)->text();
    if(SYSTEM->tipo_persona.compare(Persona::administrador) == 0){
        if(tipoPersona.compare(Persona::administrador) == 0){
            QMessageBox::warning(this, "Advertencia", "No es posible esta acción.", "Aceptar");
            return;
        }
    }
    if(SYSTEM->tipo_persona.compare(Persona::cajero) == 0){
        if(tipoPersona.compare(Persona::administrador) == 0 || tipoPersona.compare(Persona::cajero) == 0){
            QMessageBox::warning(this, "Advertencia", "No es posible esta acción.", "Aceptar");
            return;
        }
    }
    Persona* w = new Persona(this);
    w->setWindowFlag(Qt::Dialog);
    w->setWindowModality(Qt::WindowModal);

    w->setOperacion(MODIFICAR);
    w->setTipoPersona(tipoPersona);

    QTableWidgetItem* item_codigo = ui->tableWidget->item(ui->tableWidget->currentItem()->row(), 1);
    w->setCodigo(item_codigo->text());
    w->select(item_codigo->text());
    w->adjustSize();
    w->show();
}

void PersonaBuscar::on_lineEdit_codigo_textChanged(const QString &arg1)
{
    qDebug()<<"lineEdit codigo"<<endl;
    //oldTipoSelected = newTipoSelected;
    //newTipoSelected = 0;
    if(newTipoSelected != 0){
        newTipoSelected = 0;

        ui->tableWidget->clearContents();
        ui->tableWidget->setRowCount(0);
    }else{
        if(newStringChanged.compare(arg1) != 0){
            newStringChanged = arg1;

            ui->tableWidget->clearContents();
            ui->tableWidget->setRowCount(0);
        }
    }
    pos = 0;
    set_buscar();
}

void PersonaBuscar::on_lineEdit_nombre_textChanged(const QString &arg1)
{
    qDebug()<<"lineEdit nombre"<<endl;
    if(newTipoSelected != 1){
        newTipoSelected = 1;

        ui->tableWidget->clearContents();
        ui->tableWidget->setRowCount(0);
    }else{
        if(newStringChanged.compare(arg1) != 0){
            newStringChanged = arg1;

            ui->tableWidget->clearContents();
            ui->tableWidget->setRowCount(0);
        }
    }
    pos = 0;
    set_buscar();
}

void PersonaBuscar::on_lineEdit_direccion_textChanged(const QString &arg1)
{
    qDebug()<<"lineEdit direccion"<<endl;
    if(newTipoSelected != 2){
        newTipoSelected = 2;

        ui->tableWidget->clearContents();
        ui->tableWidget->setRowCount(0);
    }else{
        if(newStringChanged.compare(arg1) != 0){
            newStringChanged = arg1;

            ui->tableWidget->clearContents();
            ui->tableWidget->setRowCount(0);
        }
    }
    pos = 0;
    set_buscar();
}
void PersonaBuscar::on_lw_tipo_itemClicked(QListWidgetItem *item)
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
    Persona* w = new Persona(this);
    w->setWindowFlag(Qt::Dialog);
    w->setWindowModality(Qt::WindowModal);

    w->setOperacion(INGRESAR);
    w->setTipoPersona(item->text());
    w->adjustSize();
    w->show();
}

void PersonaBuscar::on_checkBox_1_stateChanged(int arg1)
{
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);

    if(newTipoSelected == 0){
        on_lineEdit_codigo_textChanged(ui->lineEdit_codigo->text());
    }
    if(newTipoSelected == 1){
        on_lineEdit_nombre_textChanged(ui->lineEdit_nombre->text());
    }
    if(newTipoSelected == 2){
        on_lineEdit_direccion_textChanged(ui->lineEdit_direccion->text());
    }
}

void PersonaBuscar::on_checkBox_2_stateChanged(int arg1)
{
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);

    if(newTipoSelected == 0){
        on_lineEdit_codigo_textChanged(ui->lineEdit_codigo->text());
    }
    if(newTipoSelected == 1){
        on_lineEdit_nombre_textChanged(ui->lineEdit_nombre->text());
    }
    if(newTipoSelected == 2){
        on_lineEdit_direccion_textChanged(ui->lineEdit_direccion->text());
    }
}

void PersonaBuscar::on_checkBox_3_stateChanged(int arg1)
{
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);

    if(newTipoSelected == 0){
        on_lineEdit_codigo_textChanged(ui->lineEdit_codigo->text());
    }
    if(newTipoSelected == 1){
        on_lineEdit_nombre_textChanged(ui->lineEdit_nombre->text());
    }
    if(newTipoSelected == 2){
        on_lineEdit_direccion_textChanged(ui->lineEdit_direccion->text());
    }
}

void PersonaBuscar::on_checkBox_4_stateChanged(int arg1)
{
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);

    if(newTipoSelected == 0){
        on_lineEdit_codigo_textChanged(ui->lineEdit_codigo->text());
    }
    if(newTipoSelected == 1){
        on_lineEdit_nombre_textChanged(ui->lineEdit_nombre->text());
    }
    if(newTipoSelected == 2){
        on_lineEdit_direccion_textChanged(ui->lineEdit_direccion->text());
    }
}

void PersonaBuscar::on_pushButton_aceptar_clicked()
{
    if(!ui->tableWidget->currentItem()){
        QMessageBox::warning(this, "Advertencia", "Seleccione Un item.", "Aceptar");
        return;
    }
    QTableWidgetItem* item_tipoPersona = ui->tableWidget->item(ui->tableWidget->currentItem()->row(), 0);
    QTableWidgetItem* item_codigo = ui->tableWidget->item(ui->tableWidget->currentItem()->row(), 1);
    QTableWidgetItem* item_nombre = ui->tableWidget->item(ui->tableWidget->currentItem()->row(), 2);
    QTableWidgetItem* item_direccion = ui->tableWidget->item(ui->tableWidget->currentItem()->row(), 3);
    tipoPersona = item_tipoPersona->text();
    codigo = item_codigo->text();
    nombre = item_nombre->text();
    direccion = item_direccion->text();

    this->close();
}

void PersonaBuscar::on_pushButton_salir_clicked()
{
    this->close();
}
void PersonaBuscar::on_verticalScrollBar_actionTriggered(int value)
{
    QScrollBar* bar = ui->tableWidget->verticalScrollBar();

    /*
    qDebug()<<"activation value: "<<value<<endl;
    qDebug()<<"activation bar maximum: "<<bar->maximum()<<endl;
    qDebug()<<"activation bar value: "<<bar->value()<<endl;
    */
    if(bar->value() == bar->maximum()) {
        qDebug()<<"1"<<endl;
        set_buscar();
        qDebug()<<"2"<<endl;
    }
}
void PersonaBuscar::on_verticalScrollBar_valueChanged(int value)
{
    /*
    QScrollBar* bar = ui->tableWidget->verticalScrollBar();

    if(value == bar->maximum())
        set_buscar();
*/
}
void PersonaBuscar::showEvent(QShowEvent *se)
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
void PersonaBuscar::closeEvent(QCloseEvent *ce)
{
    ce->accept();
    emit closing();
}
bool PersonaBuscar::eventFilter(QObject *obj, QEvent *e)
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
                    set_buscar();
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
