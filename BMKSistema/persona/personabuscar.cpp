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

    newTipoSelected = -1;
    newStringChanged = "&%&%&$&((/&)-1";
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

        ui->checkBox_1->setText("Con RUC");
        ui->checkBox_2->setText("Con DNI");
        ui->checkBox_3->setText("Eventual");
    }
    if(opcPersona.compare(Persona::opc_proveedores) == 0){
        setWindowTitle(Persona::opc_proveedores);
        ui->label_titulo->setText(Persona::opc_proveedores);

        ui->checkBox_1->setIcon(QIcon(":/new/Iconos/proveedor_gorro.jpg"));
        ui->checkBox_2->setIcon(QIcon(":/new/Iconos/transportista.png"));
        ui->checkBox_3->hide();

        ui->checkBox_1->setText("Proveedores");
        ui->checkBox_2->setText("Transportistas");
    }
    if(opcPersona.compare(Persona::opc_usuarios) == 0){
        setWindowTitle(Persona::opc_usuarios);
        ui->label_titulo->setText(Persona::opc_usuarios);

        ui->checkBox_1->setIcon(QIcon(":/new/Iconos/admin.png"));
        ui->checkBox_2->setIcon(QIcon(":/new/Iconos/cajero.png"));
        ui->checkBox_3->hide();

        ui->checkBox_1->setText("Administradores");
        ui->checkBox_2->setText("Cajeros");
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
    w->show();
}

void PersonaBuscar::on_lineEdit_codigo_textChanged(const QString &arg1)
{
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
    QString str_query;
    str_query  = "SELECT persona.tipo_item_nombre, persona.cod, persona.nombre, persona.direccion";
    str_query += " FROM persona";
    str_query += " WHERE persona.cod = '"+arg1+"'";
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
    //str_query += " LIMIT 0, "+QString().setNum(ui->tableWidget->rowCount()+15);
    QSqlQuery query;
    if(query.exec(str_query)){
        ui->tableWidget->clearContents();
        ui->tableWidget->setRowCount(query.size());
        int rowCount = 0;
        while(query.next()){
            QTableWidgetItem* item_tipo_item_nombre = new QTableWidgetItem(query.value(0).toString());
            QTableWidgetItem* item_cod = new QTableWidgetItem(query.value(1).toString());
            QTableWidgetItem* item_nombre = new QTableWidgetItem(query.value(2).toString());
            QTableWidgetItem* item_direccion = new QTableWidgetItem(query.value(3).toString());

            ui->tableWidget->setItem(rowCount, 0, item_tipo_item_nombre);
            ui->tableWidget->setItem(rowCount, 1, item_cod);
            ui->tableWidget->setItem(rowCount, 2, item_nombre);
            ui->tableWidget->setItem(rowCount, 3, item_direccion);
            rowCount++;
        }
    }
}

void PersonaBuscar::on_lineEdit_nombre_textChanged(const QString &arg1)
{
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
    QString str_query;
    str_query  = "SELECT persona.tipo_item_nombre, persona.cod, persona.nombre, persona.direccion";
    str_query += " FROM persona";
    str_query += " WHERE persona.nombre LIKE '%"+arg1+"%'";
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
    str_query += " ORDER BY persona.nombre, persona.cod, persona.direccion";
    str_query += " LIMIT "+QString().setNum(ui->tableWidget->rowCount())+", 15";

    qDebug()<<str_query<<endl;
    QSqlQuery query;
    if(query.exec(str_query)){
        ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+query.size());
        int rowCount = 0;
        while(query.next()){
            QTableWidgetItem* item_tipo_item_nombre = new QTableWidgetItem(query.value(0).toString());
            QTableWidgetItem* item_cod = new QTableWidgetItem(query.value(1).toString());
            QTableWidgetItem* item_nombre = new QTableWidgetItem(query.value(2).toString());
            QTableWidgetItem* item_direccion = new QTableWidgetItem(query.value(3).toString());

            ui->tableWidget->setItem(rowCount, 0, item_tipo_item_nombre);
            ui->tableWidget->setItem(rowCount, 1, item_cod);
            ui->tableWidget->setItem(rowCount, 2, item_nombre);
            ui->tableWidget->setItem(rowCount, 3, item_direccion);
            rowCount++;
        }
    }
}

void PersonaBuscar::on_lineEdit_direccion_textChanged(const QString &arg1)
{
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
    QString str_query;
    str_query  = "SELECT persona.tipo_item_nombre, persona.cod, persona.nombre, persona.direccion";
    str_query += " FROM persona";
    str_query += " WHERE persona.direccion LIKE '%"+arg1+"%'";
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
    str_query += " ORDER BY persona.nombre, persona.cod, persona.direccion";
    str_query += " LIMIT "+QString().setNum(ui->tableWidget->rowCount())+", 15";
    QSqlQuery query;
    if(query.exec(str_query)){
        ui->tableWidget->setRowCount(query.size());
        int rowCount = 0;
        while(query.next()){
            QTableWidgetItem* item_tipo_item_nombre = new QTableWidgetItem(query.value(0).toString());
            QTableWidgetItem* item_cod = new QTableWidgetItem(query.value(1).toString());
            QTableWidgetItem* item_nombre = new QTableWidgetItem(query.value(2).toString());
            QTableWidgetItem* item_direccion = new QTableWidgetItem(query.value(3).toString());

            ui->tableWidget->setItem(rowCount, 0, item_tipo_item_nombre);
            ui->tableWidget->setItem(rowCount, 1, item_cod);
            ui->tableWidget->setItem(rowCount, 2, item_nombre);
            ui->tableWidget->setItem(rowCount, 3, item_direccion);
            rowCount++;
        }
    }
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
