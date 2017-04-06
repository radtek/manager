#include "comprabuscar.h"
#include "ui_comprabuscar.h"

CompraBuscar::CompraBuscar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CompraBuscar)
{
    ui->setupUi(this);

    firstShow = false;
    afterShow = false;

    widget_previous = NULL;

    modo_only_date = false;

    pos = 0;
    size_query = 10;    

    QDate date;
    date.setDate(QDate::currentDate().year(), 1, 1);
    ui->dateEdit_inicio->setDate(date);
    ui->dateEdit_fin->setDate(QDate::currentDate());

    ui->radioButton_factura->setChecked(true);
    tipo = compra_items::FACTURA;

    //disconnect(ui->lineEdit_buscar, SIGNAL(returnPressed()), this, SLOT(on_lineEdit_buscar_returnPressed()));

    // INSTALL EVENT FILTERS
    this->installEventFilter(this);
    ui->dateEdit_inicio->installEventFilter(this);
    ui->dateEdit_fin->installEventFilter(this);
    ui->lineEdit_buscar->installEventFilter(this);
    ui->tableWidget->installEventFilter(this);
    ui->pushButton_ok->installEventFilter(this);
    ui->pushButton_salir->installEventFilter(this);
    ui->pushButton_agregar->installEventFilter(this);
    ui->pushButton_editar->installEventFilter(this);
}

CompraBuscar::~CompraBuscar()
{
    qDebug()<<"delete compra buscar"<<endl;
    delete ui;
}
void CompraBuscar::setTipoOrden()
{
    tipo = compra_items::ORDEN;
    ui->radioButton_orden->setChecked(true);
}
void CompraBuscar::setTipoBoleta()
{
    tipo = compra_items::BOLETA;
    ui->radioButton_boleta->setChecked(true);
}
void CompraBuscar::setTipoFactura()
{
    tipo = compra_items::FACTURA;
    ui->radioButton_factura->setChecked(true);
}
void CompraBuscar::setTipoGuiaRR()
{
    tipo = compra_items::GUIA_REMISION_REMITENTE;
    ui->radioButton_guia->setChecked(true);
}
void CompraBuscar::setTipoFlete()
{
    tipo = compra_items::FLETE;
    ui->radioButton_flete->setChecked(true);
}
void CompraBuscar::setTipoNotaCredito()
{
    tipo = compra_items::NOTA_CREDITO;
    ui->radioButton_nota_credito->setChecked(true);
}
void CompraBuscar::setTipoNotaDebito()
{
    tipo = compra_items::NOTA_DEBITO;
    ui->radioButton_nota_debito->setChecked(true);
}
void CompraBuscar::setTipoSaldo()
{
    tipo = compra_items::SALDO;
    ui->radioButton_saldo->setChecked(true);
}
void CompraBuscar::setTipoRegSinDoc()
{
    tipo = compra_items::REG_SIN_DOC;
    ui->radioButton_reg_sin_doc->setChecked(true);
}
void CompraBuscar::hideOptOrden()
{
    ui->radioButton_orden->hide();
}
void CompraBuscar::hideOptBoleta()
{
    ui->radioButton_boleta->hide();
}
void CompraBuscar::hideOptFactura()
{
    ui->radioButton_factura->hide();
}
void CompraBuscar::hideOptGuiaRR()
{
    ui->radioButton_guia->hide();
}
void CompraBuscar::hideOptFlete()
{
    ui->radioButton_flete->hide();
}
void CompraBuscar::hideOptNotaCredito()
{
    ui->radioButton_nota_credito->hide();
}
void CompraBuscar::hideOptNotaDebito()
{
    ui->radioButton_nota_debito->hide();
}
void CompraBuscar::hideOptSaldo()
{
    ui->radioButton_saldo->hide();
}
void CompraBuscar::hideOptRegSinDoc()
{
    ui->radioButton_reg_sin_doc->hide();
}
void CompraBuscar::set_tipo(int tipo)
{
    this->tipo = tipo;
}

void CompraBuscar::set_widget_previous(QWidget *w)
{
    this->widget_previous = w;
}
void CompraBuscar::set_ruc(QString ruc)
{
    ui->lineEdit_buscar->setText(ruc);
    ui->lineEdit_buscar->setReadOnly(true);

    modo_only_date = true;

    //ui->pushButton_agregar->hide();

    on_lineEdit_buscar_returnPressed();
}

void CompraBuscar::on_compra_closing()
{
    switch(tipo)
    {
    case compra_items::ORDEN:{
        CompraOrden* w = (CompraOrden*)QObject::sender();

        int op = w->getOp();
        switch(op){
        case INGRESAR:{
            pos = 0;

            ui->tableWidget->setRowCount(0);
            ui->tableWidget->setColumnCount(0);
            ui->tableWidget->clear();

            int rowCount = 1;
            ui->tableWidget->setRowCount(rowCount);

            int columnCount = 7;
            ui->tableWidget->setColumnCount(columnCount);

            ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "ID"
                                                       << "PERSONA_ID" << "FECHA_EMISION"
                                                       << "SERIE" << "NUMERO"
                                                       << "RUC" << "RAZON SOCIAL");
            ui->tableWidget->setColumnHidden(0, true);
            ui->tableWidget->setColumnHidden(1, true);

            QString id = w->getID();
            QString persona_id = w->getPersonaID();
            QString fecha_emision = w->getFechaEmision();
            QString serie = w->getSerie();
            QString numero = w->getNumero();
            QString ruc = w->getCodigo();
            QString razon_social = w->getNombre();

            ui->tableWidget->setItem(pos, 0, new QTableWidgetItem(id));
            ui->tableWidget->setItem(pos, 1, new QTableWidgetItem(persona_id));
            ui->tableWidget->setItem(pos, 2, new QTableWidgetItem(fecha_emision));
            ui->tableWidget->setItem(pos, 3, new QTableWidgetItem(serie));
            ui->tableWidget->setItem(pos, 4, new QTableWidgetItem(numero));
            ui->tableWidget->setItem(pos, 5, new QTableWidgetItem(ruc));
            ui->tableWidget->setItem(pos, 6, new QTableWidgetItem(razon_social));

            for(int j=0; j<ui->tableWidget->columnCount(); j++)
                ui->tableWidget->item(pos, j)->setFlags(Qt::ItemIsEnabled
                                                             | Qt::ItemIsSelectable);

            pos++;
            ui->tableWidget->setFocus();
            ui->tableWidget->selectRow(0);
            for(int j=0; j<ui->tableWidget->columnCount(); j++){
                ui->tableWidget->item(0, j)->setSelected(true);
            }
            SYSTEM->table_resize_to_contents(0, ui->tableWidget, size_query);
        }break;
        case MODIFICAR:{
            QString id = w->getID();
            QString persona_id = w->getPersonaID();
            QString fecha_emision = w->getFechaEmision();
            QString serie = w->getSerie();
            QString numero = w->getNumero();
            QString ruc = w->getCodigo();
            QString razon_social = w->getNombre();

            QTableWidgetItem* item = ui->tableWidget->currentItem();
            int row = item->row();
            ui->tableWidget->setItem(row, 0, new QTableWidgetItem(id));
            ui->tableWidget->setItem(row, 1, new QTableWidgetItem(persona_id));
            ui->tableWidget->setItem(row, 2, new QTableWidgetItem(fecha_emision));
            ui->tableWidget->setItem(row, 3, new QTableWidgetItem(serie));
            ui->tableWidget->setItem(row, 4, new QTableWidgetItem(numero));
            ui->tableWidget->setItem(row, 5, new QTableWidgetItem(ruc));
            ui->tableWidget->setItem(row, 6, new QTableWidgetItem(razon_social));

            SYSTEM->table_resize_to_contents(0, ui->tableWidget, size_query);
        }break;
        case ELIMINAR:{
            QTableWidgetItem* item = ui->tableWidget->currentItem();
            ui->tableWidget->removeRow(item->row());
        }break;
        case SALIR:{

        }break;
        }
    }break;
    case compra_items::FACTURA:{
        CompraFactura* w = (CompraFactura*)QObject::sender();

        int op = w->getOp();
        switch(op){
        case INGRESAR:{
            pos = 0;

            ui->tableWidget->setRowCount(0);
            ui->tableWidget->setColumnCount(0);
            ui->tableWidget->clear();

            int rowCount = 1;
            ui->tableWidget->setRowCount(rowCount);

            int columnCount = 7;
            ui->tableWidget->setColumnCount(columnCount);

            ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "ID"
                                                       << "PERSONA_ID" << "FECHA_EMISION"
                                                       << "SERIE" << "NUMERO"
                                                       << "RUC" << "RAZON SOCIAL");
            ui->tableWidget->setColumnHidden(0, true);
            ui->tableWidget->setColumnHidden(1, true);

            QString id = w->getID();
            QString persona_id = w->getPersonaID();
            QString fecha_emision = w->getFechaEmision();
            QString serie = w->getSerie();
            QString numero = w->getNumero();
            QString ruc = w->getCodigo();
            QString razon_social = w->getNombre();

            ui->tableWidget->setItem(pos, 0, new QTableWidgetItem(id));
            ui->tableWidget->setItem(pos, 1, new QTableWidgetItem(persona_id));
            ui->tableWidget->setItem(pos, 2, new QTableWidgetItem(fecha_emision));
            ui->tableWidget->setItem(pos, 3, new QTableWidgetItem(serie));
            ui->tableWidget->setItem(pos, 4, new QTableWidgetItem(numero));
            ui->tableWidget->setItem(pos, 5, new QTableWidgetItem(ruc));
            ui->tableWidget->setItem(pos, 6, new QTableWidgetItem(razon_social));

            for(int j=0; j<ui->tableWidget->columnCount(); j++)
                ui->tableWidget->item(pos, j)->setFlags(Qt::ItemIsEnabled
                                                             | Qt::ItemIsSelectable);

            pos++;
            ui->tableWidget->setFocus();
            ui->tableWidget->selectRow(0);
            for(int j=0; j<ui->tableWidget->columnCount(); j++){
                ui->tableWidget->item(0, j)->setSelected(true);
            }
            SYSTEM->table_resize_to_contents(0, ui->tableWidget, size_query);
        }break;
        case MODIFICAR:{
            QString id = w->getID();
            QString persona_id = w->getPersonaID();
            QString fecha_emision = w->getFechaEmision();
            QString serie = w->getSerie();
            QString numero = w->getNumero();
            QString ruc = w->getCodigo();
            QString razon_social = w->getNombre();

            QTableWidgetItem* item = ui->tableWidget->currentItem();
            int row = item->row();
            ui->tableWidget->setItem(row, 0, new QTableWidgetItem(id));
            ui->tableWidget->setItem(row, 1, new QTableWidgetItem(persona_id));
            ui->tableWidget->setItem(row, 2, new QTableWidgetItem(fecha_emision));
            ui->tableWidget->setItem(row, 3, new QTableWidgetItem(serie));
            ui->tableWidget->setItem(row, 4, new QTableWidgetItem(numero));
            ui->tableWidget->setItem(row, 5, new QTableWidgetItem(ruc));
            ui->tableWidget->setItem(row, 6, new QTableWidgetItem(razon_social));

            SYSTEM->table_resize_to_contents(0, ui->tableWidget, size_query);
        }break;
        case ELIMINAR:{
            QTableWidgetItem* item = ui->tableWidget->currentItem();
            ui->tableWidget->removeRow(item->row());
        }break;
        case SALIR:{

        }break;
        }
    }break;
    case compra_items::BOLETA:{
        CompraBoleta* w = (CompraBoleta*)QObject::sender();

        int op = w->getOp();
        switch(op){
        case INGRESAR:{
            pos = 0;

            ui->tableWidget->setRowCount(0);
            ui->tableWidget->setColumnCount(0);
            ui->tableWidget->clear();

            int rowCount = 1;
            ui->tableWidget->setRowCount(rowCount);

            int columnCount = 7;
            ui->tableWidget->setColumnCount(columnCount);

            ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "ID"
                                                       << "PERSONA_ID" << "FECHA_EMISION"
                                                       << "SERIE" << "NUMERO"
                                                       << "RUC" << "RAZON SOCIAL");
            ui->tableWidget->setColumnHidden(0, true);
            ui->tableWidget->setColumnHidden(1, true);

            QString id = w->getID();
            QString persona_id = w->getPersonaID();
            QString fecha_emision = w->getFechaEmision();
            QString serie = w->getSerie();
            QString numero = w->getNumero();
            QString ruc = w->getCodigo();
            QString razon_social = w->getNombre();

            ui->tableWidget->setItem(pos, 0, new QTableWidgetItem(id));
            ui->tableWidget->setItem(pos, 1, new QTableWidgetItem(persona_id));
            ui->tableWidget->setItem(pos, 2, new QTableWidgetItem(fecha_emision));
            ui->tableWidget->setItem(pos, 3, new QTableWidgetItem(serie));
            ui->tableWidget->setItem(pos, 4, new QTableWidgetItem(numero));
            ui->tableWidget->setItem(pos, 5, new QTableWidgetItem(ruc));
            ui->tableWidget->setItem(pos, 6, new QTableWidgetItem(razon_social));

            for(int j=0; j<ui->tableWidget->columnCount(); j++)
                ui->tableWidget->item(pos, j)->setFlags(Qt::ItemIsEnabled
                                                             | Qt::ItemIsSelectable);

            pos++;
            ui->tableWidget->setFocus();
            ui->tableWidget->selectRow(0);
            for(int j=0; j<ui->tableWidget->columnCount(); j++){
                ui->tableWidget->item(0, j)->setSelected(true);
            }
            SYSTEM->table_resize_to_contents(0, ui->tableWidget, size_query);
        }break;
        case MODIFICAR:{
            QString id = w->getID();
            QString persona_id = w->getPersonaID();
            QString fecha_emision = w->getFechaEmision();
            QString serie = w->getSerie();
            QString numero = w->getNumero();
            QString ruc = w->getCodigo();
            QString razon_social = w->getNombre();

            QTableWidgetItem* item = ui->tableWidget->currentItem();
            int row = item->row();
            ui->tableWidget->setItem(row, 0, new QTableWidgetItem(id));
            ui->tableWidget->setItem(row, 1, new QTableWidgetItem(persona_id));
            ui->tableWidget->setItem(row, 2, new QTableWidgetItem(fecha_emision));
            ui->tableWidget->setItem(row, 3, new QTableWidgetItem(serie));
            ui->tableWidget->setItem(row, 4, new QTableWidgetItem(numero));
            ui->tableWidget->setItem(row, 5, new QTableWidgetItem(ruc));
            ui->tableWidget->setItem(row, 6, new QTableWidgetItem(razon_social));

            SYSTEM->table_resize_to_contents(0, ui->tableWidget, size_query);
        }break;
        case ELIMINAR:{
            QTableWidgetItem* item = ui->tableWidget->currentItem();
            ui->tableWidget->removeRow(item->row());
        }break;
        case SALIR:{

        }break;
        }
    }break;
    case compra_items::GUIA_REMISION_REMITENTE:{
        CompraGuiaRR* w= (CompraGuiaRR*)QObject::sender();

        int op = w->getOp();
        switch(op){
        case INGRESAR:{
            pos = 0;

            ui->tableWidget->setRowCount(0);
            ui->tableWidget->setColumnCount(0);
            ui->tableWidget->clear();

            int rowCount = 1;
            ui->tableWidget->setRowCount(rowCount);

            int columnCount = 7;
            ui->tableWidget->setColumnCount(columnCount);

            ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "ID"
                                                       << "PERSONA_ID" << "FECHA_EMISION"
                                                       << "SERIE" << "NUMERO"
                                                       << "RUC" << "RAZON SOCIAL");
            ui->tableWidget->setColumnHidden(0, true);
            ui->tableWidget->setColumnHidden(1, true);

            QString id = w->getID();
            QString persona_id = w->getPersonaID();
            QString fecha_emision = w->getFechaEmision();
            QString serie = w->getSerie();
            QString numero = w->getNumero();
            QString ruc = w->getCodigo();
            QString razon_social = w->getNombre();

            ui->tableWidget->setItem(pos, 0, new QTableWidgetItem(id));
            ui->tableWidget->setItem(pos, 1, new QTableWidgetItem(persona_id));
            ui->tableWidget->setItem(pos, 2, new QTableWidgetItem(fecha_emision));
            ui->tableWidget->setItem(pos, 3, new QTableWidgetItem(serie));
            ui->tableWidget->setItem(pos, 4, new QTableWidgetItem(numero));
            ui->tableWidget->setItem(pos, 5, new QTableWidgetItem(ruc));
            ui->tableWidget->setItem(pos, 6, new QTableWidgetItem(razon_social));

            for(int j=0; j<ui->tableWidget->columnCount(); j++)
                ui->tableWidget->item(pos, j)->setFlags(Qt::ItemIsEnabled
                                                             | Qt::ItemIsSelectable);

            pos++;
            ui->tableWidget->setFocus();
            ui->tableWidget->selectRow(0);
            for(int j=0; j<ui->tableWidget->columnCount(); j++){
                ui->tableWidget->item(0, j)->setSelected(true);
            }
            SYSTEM->table_resize_to_contents(0, ui->tableWidget, size_query);
        }break;
        case MODIFICAR:{
            QString id = w->getID();
            QString persona_id = w->getPersonaID();
            QString fecha_emision = w->getFechaEmision();
            QString serie = w->getSerie();
            QString numero = w->getNumero();
            QString ruc = w->getCodigo();
            QString razon_social = w->getNombre();

            QTableWidgetItem* item = ui->tableWidget->currentItem();
            int row = item->row();
            ui->tableWidget->setItem(row, 0, new QTableWidgetItem(id));
            ui->tableWidget->setItem(row, 1, new QTableWidgetItem(persona_id));
            ui->tableWidget->setItem(row, 2, new QTableWidgetItem(fecha_emision));
            ui->tableWidget->setItem(row, 3, new QTableWidgetItem(serie));
            ui->tableWidget->setItem(row, 4, new QTableWidgetItem(numero));
            ui->tableWidget->setItem(row, 5, new QTableWidgetItem(ruc));
            ui->tableWidget->setItem(row, 6, new QTableWidgetItem(razon_social));

            SYSTEM->table_resize_to_contents(0, ui->tableWidget, size_query);
        }break;
        case ELIMINAR:{
            QTableWidgetItem* item = ui->tableWidget->currentItem();
            ui->tableWidget->removeRow(item->row());
        }break;
        case SALIR:{

        }break;
        }
    }break;
    case compra_items::FLETE:{
        CompraFlete* w= (CompraFlete*)QObject::sender();

        int op = w->getOp();
        switch(op){
        case INGRESAR:{
            pos = 0;

            ui->tableWidget->setRowCount(0);
            ui->tableWidget->setColumnCount(0);
            ui->tableWidget->clear();

            int rowCount = 1;
            ui->tableWidget->setRowCount(rowCount);

            int columnCount = 7;
            ui->tableWidget->setColumnCount(columnCount);

            ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "ID"
                                                       << "PERSONA_ID" << "FECHA_EMISION"
                                                       << "SERIE" << "NUMERO"
                                                       << "RUC" << "RAZON SOCIAL");
            ui->tableWidget->setColumnHidden(0, true);
            ui->tableWidget->setColumnHidden(1, true);

            QString id = w->getID();
            QString persona_id = w->getPersonaID();
            QString fecha_emision = w->getFechaEmision();
            QString serie = w->getSerie();
            QString numero = w->getNumero();
            QString ruc = w->getCodigo();
            QString razon_social = w->getNombre();

            ui->tableWidget->setItem(pos, 0, new QTableWidgetItem(id));
            ui->tableWidget->setItem(pos, 1, new QTableWidgetItem(persona_id));
            ui->tableWidget->setItem(pos, 2, new QTableWidgetItem(fecha_emision));
            ui->tableWidget->setItem(pos, 3, new QTableWidgetItem(serie));
            ui->tableWidget->setItem(pos, 4, new QTableWidgetItem(numero));
            ui->tableWidget->setItem(pos, 5, new QTableWidgetItem(ruc));
            ui->tableWidget->setItem(pos, 6, new QTableWidgetItem(razon_social));

            for(int j=0; j<ui->tableWidget->columnCount(); j++)
                ui->tableWidget->item(pos, j)->setFlags(Qt::ItemIsEnabled
                                                             | Qt::ItemIsSelectable);

            pos++;
            ui->tableWidget->setFocus();
            ui->tableWidget->selectRow(0);
            for(int j=0; j<ui->tableWidget->columnCount(); j++){
                ui->tableWidget->item(0, j)->setSelected(true);
            }
            SYSTEM->table_resize_to_contents(0, ui->tableWidget, size_query);
        }break;
        case MODIFICAR:{
            QString id = w->getID();
            QString persona_id = w->getPersonaID();
            QString fecha_emision = w->getFechaEmision();
            QString serie = w->getSerie();
            QString numero = w->getNumero();
            QString ruc = w->getCodigo();
            QString razon_social = w->getNombre();

            QTableWidgetItem* item = ui->tableWidget->currentItem();
            int row = item->row();
            ui->tableWidget->setItem(row, 0, new QTableWidgetItem(id));
            ui->tableWidget->setItem(row, 1, new QTableWidgetItem(persona_id));
            ui->tableWidget->setItem(row, 2, new QTableWidgetItem(fecha_emision));
            ui->tableWidget->setItem(row, 3, new QTableWidgetItem(serie));
            ui->tableWidget->setItem(row, 4, new QTableWidgetItem(numero));
            ui->tableWidget->setItem(row, 5, new QTableWidgetItem(ruc));
            ui->tableWidget->setItem(row, 6, new QTableWidgetItem(razon_social));

            SYSTEM->table_resize_to_contents(0, ui->tableWidget, size_query);
        }break;
        case ELIMINAR:{
            QTableWidgetItem* item = ui->tableWidget->currentItem();
            ui->tableWidget->removeRow(item->row());
        }break;
        case SALIR:{

        }break;
        }
    }break;
    case compra_items::NOTA_CREDITO:{
        CompraNotaCredito* w= (CompraNotaCredito*)QObject::sender();

        int op = w->getOp();
        switch(op){
        case INGRESAR:{
            pos = 0;

            ui->tableWidget->setRowCount(0);
            ui->tableWidget->setColumnCount(0);
            ui->tableWidget->clear();

            int rowCount = 1;
            ui->tableWidget->setRowCount(rowCount);

            int columnCount = 7;
            ui->tableWidget->setColumnCount(columnCount);

            ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "ID"
                                                       << "PERSONA_ID" << "FECHA_EMISION"
                                                       << "SERIE" << "NUMERO"
                                                       << "RUC" << "RAZON SOCIAL");
            ui->tableWidget->setColumnHidden(0, true);
            ui->tableWidget->setColumnHidden(1, true);

            QString id = w->getID();
            QString persona_id = w->getPersonaID();
            QString fecha_emision = w->getFechaEmision();
            QString serie = w->getSerie();
            QString numero = w->getNumero();
            QString ruc = w->getCodigo();
            QString razon_social = w->getNombre();

            ui->tableWidget->setItem(pos, 0, new QTableWidgetItem(id));
            ui->tableWidget->setItem(pos, 1, new QTableWidgetItem(persona_id));
            ui->tableWidget->setItem(pos, 2, new QTableWidgetItem(fecha_emision));
            ui->tableWidget->setItem(pos, 3, new QTableWidgetItem(serie));
            ui->tableWidget->setItem(pos, 4, new QTableWidgetItem(numero));
            ui->tableWidget->setItem(pos, 5, new QTableWidgetItem(ruc));
            ui->tableWidget->setItem(pos, 6, new QTableWidgetItem(razon_social));

            for(int j=0; j<ui->tableWidget->columnCount(); j++)
                ui->tableWidget->item(pos, j)->setFlags(Qt::ItemIsEnabled
                                                             | Qt::ItemIsSelectable);

            pos++;
            ui->tableWidget->setFocus();
            ui->tableWidget->selectRow(0);
            for(int j=0; j<ui->tableWidget->columnCount(); j++){
                ui->tableWidget->item(0, j)->setSelected(true);
            }
            SYSTEM->table_resize_to_contents(0, ui->tableWidget, size_query);
        }break;
        case MODIFICAR:{
            QString id = w->getID();
            QString persona_id = w->getPersonaID();
            QString fecha_emision = w->getFechaEmision();
            QString serie = w->getSerie();
            QString numero = w->getNumero();
            QString ruc = w->getCodigo();
            QString razon_social = w->getNombre();

            QTableWidgetItem* item = ui->tableWidget->currentItem();
            int row = item->row();
            ui->tableWidget->setItem(row, 0, new QTableWidgetItem(id));
            ui->tableWidget->setItem(row, 1, new QTableWidgetItem(persona_id));
            ui->tableWidget->setItem(row, 2, new QTableWidgetItem(fecha_emision));
            ui->tableWidget->setItem(row, 3, new QTableWidgetItem(serie));
            ui->tableWidget->setItem(row, 4, new QTableWidgetItem(numero));
            ui->tableWidget->setItem(row, 5, new QTableWidgetItem(ruc));
            ui->tableWidget->setItem(row, 6, new QTableWidgetItem(razon_social));

            SYSTEM->table_resize_to_contents(0, ui->tableWidget, size_query);
        }break;
        case ELIMINAR:{
            QTableWidgetItem* item = ui->tableWidget->currentItem();
            ui->tableWidget->removeRow(item->row());
        }break;
        case SALIR:{

        }break;
        }
    }break;
    case compra_items::NOTA_DEBITO:{
        CompraNotaDebito* w= (CompraNotaDebito*)QObject::sender();

        int op = w->getOp();
        switch(op){
        case INGRESAR:{
            pos = 0;

            ui->tableWidget->setRowCount(0);
            ui->tableWidget->setColumnCount(0);
            ui->tableWidget->clear();

            int rowCount = 1;
            ui->tableWidget->setRowCount(rowCount);

            int columnCount = 7;
            ui->tableWidget->setColumnCount(columnCount);

            ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "ID"
                                                       << "PERSONA_ID" << "FECHA_EMISION"
                                                       << "SERIE" << "NUMERO"
                                                       << "RUC" << "RAZON SOCIAL");
            ui->tableWidget->setColumnHidden(0, true);
            ui->tableWidget->setColumnHidden(1, true);

            QString id = w->getID();
            QString persona_id = w->getPersonaID();
            QString fecha_emision = w->getFechaEmision();
            QString serie = w->getSerie();
            QString numero = w->getNumero();
            QString ruc = w->getCodigo();
            QString razon_social = w->getNombre();

            ui->tableWidget->setItem(pos, 0, new QTableWidgetItem(id));
            ui->tableWidget->setItem(pos, 1, new QTableWidgetItem(persona_id));
            ui->tableWidget->setItem(pos, 2, new QTableWidgetItem(fecha_emision));
            ui->tableWidget->setItem(pos, 3, new QTableWidgetItem(serie));
            ui->tableWidget->setItem(pos, 4, new QTableWidgetItem(numero));
            ui->tableWidget->setItem(pos, 5, new QTableWidgetItem(ruc));
            ui->tableWidget->setItem(pos, 6, new QTableWidgetItem(razon_social));

            for(int j=0; j<ui->tableWidget->columnCount(); j++)
                ui->tableWidget->item(pos, j)->setFlags(Qt::ItemIsEnabled
                                                             | Qt::ItemIsSelectable);

            pos++;
            ui->tableWidget->setFocus();
            ui->tableWidget->selectRow(0);
            for(int j=0; j<ui->tableWidget->columnCount(); j++){
                ui->tableWidget->item(0, j)->setSelected(true);
            }
            SYSTEM->table_resize_to_contents(0, ui->tableWidget, size_query);
        }break;
        case MODIFICAR:{
            QString id = w->getID();
            QString persona_id = w->getPersonaID();
            QString fecha_emision = w->getFechaEmision();
            QString serie = w->getSerie();
            QString numero = w->getNumero();
            QString ruc = w->getCodigo();
            QString razon_social = w->getNombre();

            QTableWidgetItem* item = ui->tableWidget->currentItem();
            int row = item->row();
            ui->tableWidget->setItem(row, 0, new QTableWidgetItem(id));
            ui->tableWidget->setItem(row, 1, new QTableWidgetItem(persona_id));
            ui->tableWidget->setItem(row, 2, new QTableWidgetItem(fecha_emision));
            ui->tableWidget->setItem(row, 3, new QTableWidgetItem(serie));
            ui->tableWidget->setItem(row, 4, new QTableWidgetItem(numero));
            ui->tableWidget->setItem(row, 5, new QTableWidgetItem(ruc));
            ui->tableWidget->setItem(row, 6, new QTableWidgetItem(razon_social));

            SYSTEM->table_resize_to_contents(0, ui->tableWidget, size_query);
        }break;
        case ELIMINAR:{
            QTableWidgetItem* item = ui->tableWidget->currentItem();
            ui->tableWidget->removeRow(item->row());
        }break;
        case SALIR:{

        }break;
        }
    }break;
    case compra_items::SALDO:{
        CompraSaldo* w= (CompraSaldo*)QObject::sender();

        int op = w->getOp();
        switch(op){
        case INGRESAR:{
            pos = 0;

            ui->tableWidget->setRowCount(0);
            ui->tableWidget->setColumnCount(0);
            ui->tableWidget->clear();

            int rowCount = 1;
            ui->tableWidget->setRowCount(rowCount);

            int columnCount = 2;
            ui->tableWidget->setColumnCount(columnCount);

            ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "ID"
                                                       << "FECHA_EMISION");
            ui->tableWidget->setColumnHidden(0, true);

            QString id = w->getID();
            QString fecha_emision = w->getFechaEmision();

            ui->tableWidget->setItem(pos, 0, new QTableWidgetItem(id));
            ui->tableWidget->setItem(pos, 1, new QTableWidgetItem(fecha_emision));

            for(int j=0; j<ui->tableWidget->columnCount(); j++)
                ui->tableWidget->item(pos, j)->setFlags(Qt::ItemIsEnabled
                                                             | Qt::ItemIsSelectable);

            pos++;
            ui->tableWidget->setFocus();
            ui->tableWidget->selectRow(0);
            for(int j=0; j<ui->tableWidget->columnCount(); j++){
                ui->tableWidget->item(0, j)->setSelected(true);
            }
            SYSTEM->table_resize_to_contents(0, ui->tableWidget, size_query);
        }break;
        case MODIFICAR:{
            QString id = w->getID();
            QString fecha_emision = w->getFechaEmision();

            QTableWidgetItem* item = ui->tableWidget->currentItem();
            int row = item->row();
            ui->tableWidget->setItem(row, 0, new QTableWidgetItem(id));
            ui->tableWidget->setItem(row, 1, new QTableWidgetItem(fecha_emision));

            SYSTEM->table_resize_to_contents(0, ui->tableWidget, size_query);
        }break;
        case ELIMINAR:{
            QTableWidgetItem* item = ui->tableWidget->currentItem();
            ui->tableWidget->removeRow(item->row());
        }break;
        case SALIR:{

        }break;
        }
    }break;
    case compra_items::REG_SIN_DOC:{
        CompraRegSinDoc* w= (CompraRegSinDoc*)QObject::sender();

        int op = w->getOp();
        switch(op){
        case INGRESAR:{
            pos = 0;

            ui->tableWidget->setRowCount(0);
            ui->tableWidget->setColumnCount(0);
            ui->tableWidget->clear();

            int rowCount = 1;
            ui->tableWidget->setRowCount(rowCount);

            int columnCount = 2;
            ui->tableWidget->setColumnCount(columnCount);

            ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "ID"
                                                       << "FECHA_EMISION");
            ui->tableWidget->setColumnHidden(0, true);

            QString id = w->getID();
            QString fecha_emision = w->getFechaEmision();

            ui->tableWidget->setItem(pos, 0, new QTableWidgetItem(id));
            ui->tableWidget->setItem(pos, 1, new QTableWidgetItem(fecha_emision));

            for(int j=0; j<ui->tableWidget->columnCount(); j++)
                ui->tableWidget->item(pos, j)->setFlags(Qt::ItemIsEnabled
                                                             | Qt::ItemIsSelectable);

            pos++;
            ui->tableWidget->setFocus();
            ui->tableWidget->selectRow(0);
            for(int j=0; j<ui->tableWidget->columnCount(); j++){
                ui->tableWidget->item(0, j)->setSelected(true);
            }
            SYSTEM->table_resize_to_contents(0, ui->tableWidget, size_query);
        }break;
        case MODIFICAR:{
            QString id = w->getID();
            QString fecha_emision = w->getFechaEmision();

            QTableWidgetItem* item = ui->tableWidget->currentItem();
            int row = item->row();
            ui->tableWidget->setItem(row, 0, new QTableWidgetItem(id));
            ui->tableWidget->setItem(row, 1, new QTableWidgetItem(fecha_emision));

            SYSTEM->table_resize_to_contents(0, ui->tableWidget, size_query);
        }break;
        case ELIMINAR:{
            QTableWidgetItem* item = ui->tableWidget->currentItem();
            ui->tableWidget->removeRow(item->row());
        }break;
        case SALIR:{

        }break;
        }
    }break;
    }
}
void CompraBuscar::set_buscar()
{
    QString str_query;

    QString arg = ui->lineEdit_buscar->text();
    QString match = "";
    bool ok;

    arg.left(11).toFloat(&ok);
    if(ok){
        match = "ruc";
        arg = arg.mid(0, 11)+"%";
    }else{
        match = "razon_social";
        arg = arg+"%";
    }

    switch(tipo)
    {
    case compra_items::ORDEN:{
        str_query = "SELECT documento.id, persona.id, anexo.fecha_emision, anexo.serie, anexo.numero, juridica.ruc, juridica.razon_social";
        str_query += " FROM orden";
        str_query += " JOIN comprobante ON (comprobante.operacion_id = "+QString().setNum(operacion_items::COMPRA)+" AND orden.comprobante_documento_id = comprobante.documento_id)";
        str_query += " JOIN documento ON orden.comprobante_documento_id = documento.id";
        str_query += " JOIN anexo ON documento.id = anexo.documento_id";
        str_query += " JOIN documento_h_persona ON documento_h_persona.documento_id = documento.id";
        str_query += " JOIN persona ON persona.id = documento_h_persona.persona_id";
        str_query += " JOIN juridica ON persona.id = juridica.persona_id";
        str_query += " WHERE juridica."+match+" LIKE '%" + arg + "' AND ";
        str_query += " (anexo.fecha_emision BETWEEN '"+ui->dateEdit_inicio->date().toString("yyyy-MM-dd")+"' AND '"+ui->dateEdit_fin->date().toString("yyyy-MM-dd")+"')";
        str_query += " ORDER BY anexo.fecha_emision DESC, anexo.serie DESC, anexo.numero DESC, juridica."+match;
        str_query += " LIMIT " + QString().setNum(pos) + ", " + QString().setNum(size_query) + "";
    }break;
    case compra_items::FACTURA:{
        str_query = "SELECT documento.id, persona.id, anexo.fecha_emision, anexo.serie, anexo.numero, juridica.ruc, juridica.razon_social";
        str_query += " FROM factura";
        str_query += " JOIN comprobante ON (comprobante.operacion_id = "+QString().setNum(operacion_items::COMPRA)+" AND factura.comprobante_documento_id = comprobante.documento_id)";
        str_query += " JOIN documento ON factura.comprobante_documento_id = documento.id";
        str_query += " JOIN anexo ON documento.id = anexo.documento_id";
        str_query += " JOIN documento_h_persona ON documento_h_persona.documento_id = documento.id";
        str_query += " JOIN persona ON persona.id = documento_h_persona.persona_id";
        str_query += " JOIN juridica ON persona.id = juridica.persona_id";
        str_query += " WHERE juridica."+match+" LIKE '%" + arg + "' AND ";
        str_query += " (anexo.fecha_emision BETWEEN '"+ui->dateEdit_inicio->date().toString("yyyy-MM-dd")+"' AND '"+ui->dateEdit_fin->date().toString("yyyy-MM-dd")+"')";
        str_query += " ORDER BY anexo.fecha_emision DESC, anexo.serie DESC, anexo.numero DESC, juridica."+match;
        str_query += " LIMIT " + QString().setNum(pos) + ", " + QString().setNum(size_query) + "";
    }break;
    case compra_items::BOLETA:{
        str_query = "SELECT documento.id, persona.id, anexo.fecha_emision, anexo.serie, anexo.numero, juridica.ruc, juridica.razon_social";
        str_query += " FROM boleta";
        str_query += " JOIN comprobante ON (comprobante.operacion_id = "+QString().setNum(operacion_items::COMPRA)+" AND boleta.comprobante_documento_id = comprobante.documento_id)";
        str_query += " JOIN documento ON boleta.comprobante_documento_id = documento.id";
        str_query += " JOIN anexo ON documento.id = anexo.documento_id";
        str_query += " JOIN documento_h_persona ON documento_h_persona.documento_id = documento.id";
        str_query += " JOIN persona ON persona.id = documento_h_persona.persona_id";
        str_query += " JOIN juridica ON persona.id = juridica.persona_id";
        str_query += " WHERE juridica."+match+" LIKE '%" + arg + "' AND ";
        str_query += " (anexo.fecha_emision BETWEEN '"+ui->dateEdit_inicio->date().toString("yyyy-MM-dd")+"' AND '"+ui->dateEdit_fin->date().toString("yyyy-MM-dd")+"')";
        str_query += " ORDER BY anexo.fecha_emision DESC, anexo.serie DESC, anexo.numero DESC, juridica."+match;
        str_query += " LIMIT " + QString().setNum(pos) + ", " + QString().setNum(size_query) + "";
    }break;
    case compra_items::GUIA_REMISION_REMITENTE:{
        str_query = "SELECT documento.id, persona.id, anexo.fecha_emision, anexo.serie, anexo.numero, juridica.ruc, juridica.razon_social";
        str_query += " FROM guia_remision_remitente";
        str_query += " JOIN comprobante ON (comprobante.operacion_id = "+QString().setNum(operacion_items::COMPRA)+" AND guia_remision_remitente.comprobante_documento_id = comprobante.documento_id)";
        str_query += " JOIN documento ON guia_remision_remitente.comprobante_documento_id = documento.id";
        str_query += " JOIN anexo ON documento.id = anexo.documento_id";
        str_query += " JOIN documento_h_persona ON documento_h_persona.documento_id = documento.id";
        str_query += " JOIN persona ON persona.id = documento_h_persona.persona_id";
        str_query += " JOIN juridica ON persona.id = juridica.persona_id";
        str_query += " WHERE juridica."+match+" LIKE '%" + arg + "' AND ";
        str_query += " (anexo.fecha_emision BETWEEN '"+ui->dateEdit_inicio->date().toString("yyyy-MM-dd")+"' AND '"+ui->dateEdit_fin->date().toString("yyyy-MM-dd")+"')";
        str_query += " ORDER BY anexo.fecha_emision DESC, anexo.serie DESC, anexo.numero DESC, juridica."+match;
        str_query += " LIMIT " + QString().setNum(pos) + ", " + QString().setNum(size_query) + "";
    }break;
    case compra_items::FLETE:{
        str_query = "SELECT documento.id, persona.id, anexo.fecha_emision, anexo.serie, anexo.numero, juridica.ruc, juridica.razon_social";
        str_query += " FROM flete";
        str_query += " JOIN comprobante ON (comprobante.operacion_id = "+QString().setNum(operacion_items::COMPRA)+" AND flete.comprobante_documento_id = comprobante.documento_id)";
        str_query += " JOIN documento ON flete.comprobante_documento_id = documento.id";
        str_query += " JOIN anexo ON documento.id = anexo.documento_id";
        str_query += " JOIN documento_h_persona ON documento_h_persona.documento_id = documento.id";
        str_query += " JOIN persona ON persona.id = documento_h_persona.persona_id";
        str_query += " JOIN juridica ON persona.id = juridica.persona_id";
        str_query += " WHERE juridica."+match+" LIKE '%" + arg + "' AND ";
        str_query += " (anexo.fecha_emision BETWEEN '"+ui->dateEdit_inicio->date().toString("yyyy-MM-dd")+"' AND '"+ui->dateEdit_fin->date().toString("yyyy-MM-dd")+"')";
        str_query += " ORDER BY anexo.fecha_emision DESC, anexo.serie DESC, anexo.numero DESC, juridica."+match;
        str_query += " LIMIT " + QString().setNum(pos) + ", " + QString().setNum(size_query) + "";
    }break;
    case compra_items::NOTA_CREDITO:{
        str_query = "SELECT documento.id, persona.id, anexo.fecha_emision, anexo.serie, anexo.numero, juridica.ruc, juridica.razon_social";
        str_query += " FROM nota_credito";
        str_query += " JOIN comprobante ON (comprobante.operacion_id = "+QString().setNum(operacion_items::COMPRA)+" AND nota_credito.comprobante_documento_id = comprobante.documento_id)";
        str_query += " JOIN documento ON nota_credito.comprobante_documento_id = documento.id";
        str_query += " JOIN anexo ON documento.id = anexo.documento_id";
        str_query += " JOIN documento_h_persona ON documento_h_persona.documento_id = documento.id";
        str_query += " JOIN persona ON persona.id = documento_h_persona.persona_id";
        str_query += " JOIN juridica ON persona.id = juridica.persona_id";
        str_query += " WHERE juridica."+match+" LIKE '%" + arg + "' AND ";
        str_query += " (anexo.fecha_emision BETWEEN '"+ui->dateEdit_inicio->date().toString("yyyy-MM-dd")+"' AND '"+ui->dateEdit_fin->date().toString("yyyy-MM-dd")+"')";
        str_query += " ORDER BY anexo.fecha_emision DESC, anexo.serie DESC, anexo.numero DESC, juridica."+match;
        str_query += " LIMIT " + QString().setNum(pos) + ", " + QString().setNum(size_query) + "";
    }break;
    case compra_items::NOTA_DEBITO:{
        str_query = "SELECT documento.id, persona.id, anexo.fecha_emision, anexo.serie, anexo.numero, juridica.ruc, juridica.razon_social";
        str_query += " FROM nota_debito";
        str_query += " JOIN comprobante ON (comprobante.operacion_id = "+QString().setNum(operacion_items::COMPRA)+" AND nota_debito.comprobante_documento_id = comprobante.documento_id)";
        str_query += " JOIN documento ON nota_debito.comprobante_documento_id = documento.id";
        str_query += " JOIN anexo ON documento.id = anexo.documento_id";
        str_query += " JOIN documento_h_persona ON documento_h_persona.documento_id = documento.id";
        str_query += " JOIN persona ON persona.id = documento_h_persona.persona_id";
        str_query += " JOIN juridica ON persona.id = juridica.persona_id";
        str_query += " WHERE juridica."+match+" LIKE '%" + arg + "' AND ";
        str_query += " (anexo.fecha_emision BETWEEN '"+ui->dateEdit_inicio->date().toString("yyyy-MM-dd")+"' AND '"+ui->dateEdit_fin->date().toString("yyyy-MM-dd")+"')";
        str_query += " ORDER BY anexo.fecha_emision DESC, anexo.serie DESC, anexo.numero DESC, juridica."+match;
        str_query += " LIMIT " + QString().setNum(pos) + ", " + QString().setNum(size_query) + "";
    }break;
    case compra_items::SALDO:{
        str_query = "SELECT documento.id, anexo.fecha_emision";
        str_query += " FROM saldo";
        str_query += " JOIN comprobante ON (comprobante.operacion_id = "+QString().setNum(operacion_items::COMPRA)+" AND saldo.comprobante_documento_id = comprobante.documento_id)";
        str_query += " JOIN documento ON saldo.comprobante_documento_id = documento.id";
        str_query += " JOIN anexo ON documento.id = anexo.documento_id";
        str_query += " WHERE";
        str_query += " (anexo.fecha_emision BETWEEN '"+ui->dateEdit_inicio->date().toString("yyyy-MM-dd")+"' AND '"+ui->dateEdit_fin->date().toString("yyyy-MM-dd")+"')";
        str_query += " ORDER BY anexo.fecha_emision DESC";
        str_query += " LIMIT " + QString().setNum(pos) + ", " + QString().setNum(size_query) + "";
    }break;
    case compra_items::REG_SIN_DOC:{
        str_query = "SELECT documento.id, anexo.fecha_emision";
        str_query += " FROM registro_sin_documento";
        str_query += " JOIN comprobante ON (comprobante.operacion_id = "+QString().setNum(operacion_items::COMPRA)+" AND registro_sin_documento.comprobante_documento_id = comprobante.documento_id)";
        str_query += " JOIN documento ON registro_sin_documento.comprobante_documento_id = documento.id";
        str_query += " JOIN anexo ON documento.id = anexo.documento_id";
        str_query += " WHERE";
        str_query += " (anexo.fecha_emision BETWEEN '"+ui->dateEdit_inicio->date().toString("yyyy-MM-dd")+"' AND '"+ui->dateEdit_fin->date().toString("yyyy-MM-dd")+"')";
        str_query += " ORDER BY anexo.fecha_emision DESC";
        str_query += " LIMIT " + QString().setNum(pos) + ", " + QString().setNum(size_query) + "";
    }break;
    }

    QSqlQuery query;
    qDebug() << str_query << endl;
    if (query.exec(str_query)) {
        int rowCount = ui->tableWidget->rowCount();
        ui->tableWidget->setRowCount(rowCount + query.size());

        int columnCount = query.record().count();
        ui->tableWidget->setColumnCount(columnCount);

        switch(tipo)
        {
        case compra_items::ORDEN:{
            ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "ID"
                                                       << "PERSONA_ID" << "FECHA_EMISION"
                                                       << "SERIE" << "NUMERO"
                                                       << "RUC" << "RAZON SOCIAL");
            ui->tableWidget->setColumnHidden(0, true);
            ui->tableWidget->setColumnHidden(1, true);
        }break;
        case compra_items::FACTURA:{
            ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "ID"
                                                       << "PERSONA_ID" << "FECHA_EMISION"
                                                       << "SERIE" << "NUMERO"
                                                       << "RUC" << "RAZON SOCIAL");
            ui->tableWidget->setColumnHidden(0, true);
            ui->tableWidget->setColumnHidden(1, true);
        }break;
        case compra_items::BOLETA:{
            ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "ID"
                                                       << "PERSONA_ID" << "FECHA_EMISION"
                                                       << "SERIE" << "NUMERO"
                                                       << "RUC" << "RAZON SOCIAL");
            ui->tableWidget->setColumnHidden(0, true);
            ui->tableWidget->setColumnHidden(1, true);
        }break;
        case compra_items::GUIA_REMISION_REMITENTE:{
            ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "ID"
                                                       << "PERSONA_ID" << "FECHA_EMISION"
                                                       << "SERIE" << "NUMERO"
                                                       << "RUC" << "RAZON SOCIAL");
            ui->tableWidget->setColumnHidden(0, true);
            ui->tableWidget->setColumnHidden(1, true);
        }break;
        case compra_items::FLETE:{
            ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "ID"
                                                       << "PERSONA_ID" << "FECHA_EMISION"
                                                       << "SERIE" << "NUMERO"
                                                       << "RUC" << "RAZON SOCIAL");
            ui->tableWidget->setColumnHidden(0, true);
            ui->tableWidget->setColumnHidden(1, true);
        }break;
        case compra_items::NOTA_CREDITO:{
            ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "ID"
                                                       << "PERSONA_ID" << "FECHA_EMISION"
                                                       << "SERIE" << "NUMERO"
                                                       << "RUC" << "RAZON SOCIAL");
            ui->tableWidget->setColumnHidden(0, true);
            ui->tableWidget->setColumnHidden(1, true);
        }break;
        case compra_items::NOTA_DEBITO:{
            ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "ID"
                                                       << "PERSONA_ID" << "FECHA_EMISION"
                                                       << "SERIE" << "NUMERO"
                                                       << "RUC" << "RAZON SOCIAL");
            ui->tableWidget->setColumnHidden(0, true);
            ui->tableWidget->setColumnHidden(1, true);
        }break;
        case compra_items::SALDO:{
            ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "ID"
                                                       << "FECHA_EMISION");
            ui->tableWidget->setColumnHidden(0, true);
        }break;
        case compra_items::REG_SIN_DOC:{
            ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "ID"
                                                       << "FECHA_EMISION");
            ui->tableWidget->setColumnHidden(0, true);
        }break;
        }

        while (query.next()) {
            switch(tipo)
            {
            case compra_items::ORDEN:{
                QString id = query.value(0).toString();
                QString persona_id = query.value(1).toString();
                QString fecha_emision = query.value(2).toDate().toString("dd-MM-yyyy");
                QString serie = query.value(3).toString();
                QString numero = query.value(4).toString();
                QString ruc = query.value(5).toString();
                QString razon_social = query.value(6).toString();

                ui->tableWidget->setItem(pos, 0, new QTableWidgetItem(id));
                ui->tableWidget->setItem(pos, 1, new QTableWidgetItem(persona_id));
                ui->tableWidget->setItem(pos, 2, new QTableWidgetItem(fecha_emision));
                ui->tableWidget->setItem(pos, 3, new QTableWidgetItem(serie));
                ui->tableWidget->setItem(pos, 4, new QTableWidgetItem(numero));
                ui->tableWidget->setItem(pos, 5, new QTableWidgetItem(ruc));
                ui->tableWidget->setItem(pos, 6, new QTableWidgetItem(razon_social));

            }break;
            case compra_items::FACTURA:{
                QString id = query.value(0).toString();
                QString persona_id = query.value(1).toString();
                QString fecha_emision = query.value(2).toDate().toString("dd-MM-yyyy");
                QString serie = query.value(3).toString();
                QString numero = query.value(4).toString();
                QString ruc = query.value(5).toString();
                QString razon_social = query.value(6).toString();

                ui->tableWidget->setItem(pos, 0, new QTableWidgetItem(id));
                ui->tableWidget->setItem(pos, 1, new QTableWidgetItem(persona_id));
                ui->tableWidget->setItem(pos, 2, new QTableWidgetItem(fecha_emision));
                ui->tableWidget->setItem(pos, 3, new QTableWidgetItem(serie));
                ui->tableWidget->setItem(pos, 4, new QTableWidgetItem(numero));
                ui->tableWidget->setItem(pos, 5, new QTableWidgetItem(ruc));
                ui->tableWidget->setItem(pos, 6, new QTableWidgetItem(razon_social));

            }break;
            case compra_items::BOLETA:{
                QString id = query.value(0).toString();
                QString persona_id = query.value(1).toString();
                QString fecha_emision = query.value(2).toDate().toString("dd-MM-yyyy");
                QString serie = query.value(3).toString();
                QString numero = query.value(4).toString();
                QString ruc = query.value(5).toString();
                QString razon_social = query.value(6).toString();

                ui->tableWidget->setItem(pos, 0, new QTableWidgetItem(id));
                ui->tableWidget->setItem(pos, 1, new QTableWidgetItem(persona_id));
                ui->tableWidget->setItem(pos, 2, new QTableWidgetItem(fecha_emision));
                ui->tableWidget->setItem(pos, 3, new QTableWidgetItem(serie));
                ui->tableWidget->setItem(pos, 4, new QTableWidgetItem(numero));
                ui->tableWidget->setItem(pos, 5, new QTableWidgetItem(ruc));
                ui->tableWidget->setItem(pos, 6, new QTableWidgetItem(razon_social));

            }break;
            case compra_items::GUIA_REMISION_REMITENTE:{
                QString id = query.value(0).toString();
                QString persona_id = query.value(1).toString();
                QString fecha_emision = query.value(2).toDate().toString("dd-MM-yyyy");
                QString serie = query.value(3).toString();
                QString numero = query.value(4).toString();
                QString ruc = query.value(5).toString();
                QString razon_social = query.value(6).toString();

                ui->tableWidget->setItem(pos, 0, new QTableWidgetItem(id));
                ui->tableWidget->setItem(pos, 1, new QTableWidgetItem(persona_id));
                ui->tableWidget->setItem(pos, 2, new QTableWidgetItem(fecha_emision));
                ui->tableWidget->setItem(pos, 3, new QTableWidgetItem(serie));
                ui->tableWidget->setItem(pos, 4, new QTableWidgetItem(numero));
                ui->tableWidget->setItem(pos, 5, new QTableWidgetItem(ruc));
                ui->tableWidget->setItem(pos, 6, new QTableWidgetItem(razon_social));

            }break;
            case compra_items::FLETE:{
                QString id = query.value(0).toString();
                QString persona_id = query.value(1).toString();
                QString fecha_emision = query.value(2).toDate().toString("dd-MM-yyyy");
                QString serie = query.value(3).toString();
                QString numero = query.value(4).toString();
                QString ruc = query.value(5).toString();
                QString razon_social = query.value(6).toString();

                ui->tableWidget->setItem(pos, 0, new QTableWidgetItem(id));
                ui->tableWidget->setItem(pos, 1, new QTableWidgetItem(persona_id));
                ui->tableWidget->setItem(pos, 2, new QTableWidgetItem(fecha_emision));
                ui->tableWidget->setItem(pos, 3, new QTableWidgetItem(serie));
                ui->tableWidget->setItem(pos, 4, new QTableWidgetItem(numero));
                ui->tableWidget->setItem(pos, 5, new QTableWidgetItem(ruc));
                ui->tableWidget->setItem(pos, 6, new QTableWidgetItem(razon_social));

            }break;
            case compra_items::NOTA_CREDITO:{
                QString id = query.value(0).toString();
                QString persona_id = query.value(1).toString();
                QString fecha_emision = query.value(2).toDate().toString("dd-MM-yyyy");
                QString serie = query.value(3).toString();
                QString numero = query.value(4).toString();
                QString ruc = query.value(5).toString();
                QString razon_social = query.value(6).toString();

                ui->tableWidget->setItem(pos, 0, new QTableWidgetItem(id));
                ui->tableWidget->setItem(pos, 1, new QTableWidgetItem(persona_id));
                ui->tableWidget->setItem(pos, 2, new QTableWidgetItem(fecha_emision));
                ui->tableWidget->setItem(pos, 3, new QTableWidgetItem(serie));
                ui->tableWidget->setItem(pos, 4, new QTableWidgetItem(numero));
                ui->tableWidget->setItem(pos, 5, new QTableWidgetItem(ruc));
                ui->tableWidget->setItem(pos, 6, new QTableWidgetItem(razon_social));

            }break;
            case compra_items::NOTA_DEBITO:{
                QString id = query.value(0).toString();
                QString persona_id = query.value(1).toString();
                QString fecha_emision = query.value(2).toDate().toString("dd-MM-yyyy");
                QString serie = query.value(3).toString();
                QString numero = query.value(4).toString();
                QString ruc = query.value(5).toString();
                QString razon_social = query.value(6).toString();

                ui->tableWidget->setItem(pos, 0, new QTableWidgetItem(id));
                ui->tableWidget->setItem(pos, 1, new QTableWidgetItem(persona_id));
                ui->tableWidget->setItem(pos, 2, new QTableWidgetItem(fecha_emision));
                ui->tableWidget->setItem(pos, 3, new QTableWidgetItem(serie));
                ui->tableWidget->setItem(pos, 4, new QTableWidgetItem(numero));
                ui->tableWidget->setItem(pos, 5, new QTableWidgetItem(ruc));
                ui->tableWidget->setItem(pos, 6, new QTableWidgetItem(razon_social));

            }break;
            case compra_items::SALDO:{
                QString id = query.value(0).toString();
                QString fecha_emision = query.value(1).toDate().toString("dd-MM-yyyy");

                ui->tableWidget->setItem(pos, 0, new QTableWidgetItem(id));
                ui->tableWidget->setItem(pos, 1, new QTableWidgetItem(fecha_emision));

            }break;
            case compra_items::REG_SIN_DOC:{
                QString id = query.value(0).toString();
                QString fecha_emision = query.value(1).toDate().toString("dd-MM-yyyy");

                ui->tableWidget->setItem(pos, 0, new QTableWidgetItem(id));
                ui->tableWidget->setItem(pos, 1, new QTableWidgetItem(fecha_emision));

            }break;
            }
            ++pos;
        }
        SYSTEM->table_resize_to_contents(0, ui->tableWidget, size_query);

    } else {
    }
}

void CompraBuscar::on_lineEdit_buscar_textEdited(const QString &arg1)
{
    pos = 0;

    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(0);
    ui->tableWidget->clear();

    set_buscar();
}

void CompraBuscar::on_lineEdit_buscar_returnPressed()
{
    qDebug()<<"return Pressed"<<endl;
    pos = 0;

    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(0);
    ui->tableWidget->clear();

    set_buscar();
}
void CompraBuscar::on_pushButton_ok_clicked()
{    
    QTableWidget* tb = ui->tableWidget;
    QTableWidgetItem* item = tb->currentItem();

    if(!item){
        return;
    }
    int row = item->row();

    switch(tipo)
    {
    case compra_items::SALDO:{
        id = tb->item(row, 0)->text();
        fecha_emision = tb->item(row, 1)->text();
    }break;
    default:{
        id = tb->item(row, 0)->text();
        persona_id = tb->item(row, 1)->text();
        fecha_emision = tb->item(row, 2)->text();
        serie = tb->item(row, 3)->text();
        numero = tb->item(row, 4)->text();
        codigo = tb->item(row, 5)->text();
        nombre = tb->item(row, 6)->text();
    }break;
    }

    if(widget_previous){
        setAttribute(Qt::WA_DeleteOnClose);
        SYSTEM->change_center_w(this, widget_previous);
    }else{
        editarItem(item);
    }    
}

void CompraBuscar::on_pushButton_salir_clicked()
{    
    //int ret = QMessageBox::warning(this, "Advertencia", "¿Esta seguro que desea salir?", "Si", "No");
    //switch(ret){
    //case 0:{
        if(widget_previous){
            setAttribute(Qt::WA_DeleteOnClose);
            SYSTEM->change_center_w(this, widget_previous);
        }else{
            SYSTEM->clear_center_w(this);
        }
    //}break;
    //case 1:{

    //}
    //}
}
void CompraBuscar::on_pushButton_agregar_clicked()
{    
    //int ret = QMessageBox::warning(this, "Advertencia", "¿Desea AGREGAR una COMPRA?", "Si", "No");
    //switch(ret){
    //case 0:{
        switch(tipo)
        {
        case compra_items::ORDEN:{
            CompraOrden* w = new CompraOrden;
            w->set_widget_previous(this);
            connect(w, SIGNAL(closing()), this, SLOT(on_compra_closing()));
            SYSTEM->change_center_w(this, w);
        }break;
        case compra_items::FACTURA:{
            CompraFactura* w = new CompraFactura;
            w->set_widget_previous(this);
            connect(w, SIGNAL(closing()), this, SLOT(on_compra_closing()));
            SYSTEM->change_center_w(this, w);
        }break;
        case compra_items::BOLETA:{
            CompraBoleta* w = new CompraBoleta;
            w->set_widget_previous(this);
            connect(w, SIGNAL(closing()), this, SLOT(on_compra_closing()));
            SYSTEM->change_center_w(this, w);
        }break;

        case compra_items::GUIA_REMISION_REMITENTE:{
            CompraGuiaRR* w = new CompraGuiaRR;
            w->set_widget_previous(this);
            connect(w, SIGNAL(closing()), this, SLOT(on_compra_closing()));
            SYSTEM->change_center_w(this, w);
        }break;
        case compra_items::FLETE:{
            CompraFlete* w = new CompraFlete;
            w->set_widget_previous(this);
            connect(w, SIGNAL(closing()), this, SLOT(on_compra_closing()));
            SYSTEM->change_center_w(this, w);
        }break;
        case compra_items::NOTA_CREDITO:{
            CompraNotaCredito* w = new CompraNotaCredito;
            w->set_widget_previous(this);
            connect(w, SIGNAL(closing()), this, SLOT(on_compra_closing()));
            SYSTEM->change_center_w(this, w);
        }break;
        case compra_items::NOTA_DEBITO:{
            CompraNotaDebito* w = new CompraNotaDebito;
            w->set_widget_previous(this);
            connect(w, SIGNAL(closing()), this, SLOT(on_compra_closing()));
            SYSTEM->change_center_w(this, w);
        }break;
        case compra_items::SALDO:{
            CompraSaldo* w = new CompraSaldo;
            w->set_widget_previous(this);
            connect(w, SIGNAL(closing()), this, SLOT(on_compra_closing()));
            SYSTEM->change_center_w(this, w);
        }break;
        case compra_items::REG_SIN_DOC:{
            CompraRegSinDoc* w = new CompraRegSinDoc;
            w->set_widget_previous(this);
            connect(w, SIGNAL(closing()), this, SLOT(on_compra_closing()));
            SYSTEM->change_center_w(this, w);
        }break;
        }
    //}break;
    //case 1:{

    //}break;
    //}
}

void CompraBuscar::on_radioButton_orden_clicked()
{
    tipo = compra_items::ORDEN;

    on_lineEdit_buscar_textEdited("");
    on_lineEdit_buscar_returnPressed();
}

void CompraBuscar::on_radioButton_factura_clicked()
{
    tipo = compra_items::FACTURA;

    on_lineEdit_buscar_textEdited("");
    on_lineEdit_buscar_returnPressed();
}

void CompraBuscar::on_radioButton_boleta_clicked()
{
    tipo = compra_items::BOLETA;

    on_lineEdit_buscar_textEdited("");
    on_lineEdit_buscar_returnPressed();
}

void CompraBuscar::on_radioButton_guia_clicked()
{
    tipo = compra_items::GUIA_REMISION_REMITENTE;

    on_lineEdit_buscar_textEdited("");
    on_lineEdit_buscar_returnPressed();
}

void CompraBuscar::on_radioButton_flete_clicked()
{
    tipo = compra_items::FLETE;

    on_lineEdit_buscar_textEdited("");
    on_lineEdit_buscar_returnPressed();
}

void CompraBuscar::on_radioButton_nota_credito_clicked()
{
    tipo = compra_items::NOTA_CREDITO;

    on_lineEdit_buscar_textEdited("");
    on_lineEdit_buscar_returnPressed();
}

void CompraBuscar::on_radioButton_nota_debito_clicked()
{
    tipo = compra_items::NOTA_DEBITO;

    on_lineEdit_buscar_textEdited("");
    on_lineEdit_buscar_returnPressed();
}

void CompraBuscar::on_radioButton_saldo_clicked()
{
    tipo = compra_items::SALDO;

    on_lineEdit_buscar_textEdited("");
    on_lineEdit_buscar_returnPressed();
}

void CompraBuscar::on_radioButton_reg_sin_doc_clicked()
{
    tipo = compra_items::REG_SIN_DOC;

    on_lineEdit_buscar_textEdited("");
    on_lineEdit_buscar_returnPressed();
}
void CompraBuscar::on_pushButton_editar_clicked()
{
    QTableWidgetItem* item = ui->tableWidget->currentItem();
    editarItem(item);
}
void CompraBuscar::on_tableWidget_itemDoubleClicked(QTableWidgetItem *item)
{
    if(widget_previous){
        on_pushButton_ok_clicked();
    }else{
        editarItem(item);
    }
}
void CompraBuscar::showEvent(QShowEvent *event)
{
    event->accept();

    afterShow = true;

    if(!firstShow){
        on_lineEdit_buscar_textEdited(ui->lineEdit_buscar->text());
        //on_lineEdit_buscar_returnPressed();
        firstShow = true;
    }
}
void CompraBuscar::closeEvent(QCloseEvent *event)
{
    event->accept();
    emit closing();
}

bool CompraBuscar::eventFilter(QObject *obj, QEvent *e)
{
    QWidget* w_temp;
    w_temp = this;
    if(obj == w_temp){
        if(e->type() == QEvent::MouseButtonPress){
            if(focusWidget()){
                focusWidget()->setFocus();
            }else{
                ui->lineEdit_buscar->setFocus();
                ui->lineEdit_buscar->setCursorPosition(ui->lineEdit_buscar->text().length());
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
                    if(focusWidget() == ui->pushButton_agregar){
                        ui->lineEdit_buscar->setFocus();
                        ui->lineEdit_buscar->setCursorPosition(ui->lineEdit_buscar->text().length());
                    }else{
                        focusWidget()->setFocus();
                    }
                }else{
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
    w_temp = ui->dateEdit_inicio;
    if(w_temp == obj){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:
                ui->dateEdit_fin->setFocus(Qt::TabFocusReason);
                return true;
            case Qt::Key_Enter:{
                QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QApplication::sendEvent(w_temp, key);
                return true;
            }break;
            }

        }else{

        }
        if(e->type() == QEvent::FocusOut){
            //ui->dateEdit_inicio->setFo
        }
        return false;
    }
    w_temp = ui->dateEdit_fin;
    if(w_temp == obj){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:
                if(modo_only_date){
                    on_lineEdit_buscar_textEdited(ui->lineEdit_buscar->text());
                    on_lineEdit_buscar_returnPressed();
                    ui->tableWidget->setFocus(Qt::TabFocusReason);
                }else{
                    ui->lineEdit_buscar->setFocus(Qt::TabFocusReason);
                }
                return true;
            case Qt::Key_Enter:{
                QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QApplication::sendEvent(w_temp, key);
                return true;
            }break;
            }

        }else{

        }
        if(e->type() == QEvent::FocusOut){

        }
        return false;
    }
    w_temp = ui->lineEdit_buscar;
    if(w_temp == obj){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:
                on_lineEdit_buscar_returnPressed();
                return true;
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
    w_temp = ui->tableWidget;
    if(w_temp == obj){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:
                ui->pushButton_ok->click();
                return true;
            case Qt::Key_Down: {
                int index = ui->tableWidget->currentRow();
                if (index == ui->tableWidget->rowCount() - 1) {
                    on_lineEdit_buscar_returnPressed();
                }
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
    w_temp = ui->pushButton_ok;
    if(w_temp == obj){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:
                ui->pushButton_ok->click();
                return true;
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
    w_temp = ui->pushButton_salir;
    if(w_temp == obj){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Tab:
                this->setFocus();
                break;
            case Qt::Key_Return:
                ui->pushButton_salir->click();
                return true;
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
    w_temp = ui->pushButton_agregar;
    if(w_temp == obj){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:
                ui->pushButton_agregar->click();
                return true;
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
    w_temp = ui->pushButton_editar;
    if(w_temp == obj){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:
                ui->pushButton_editar->click();
                return true;
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
void CompraBuscar::editarItem(QTableWidgetItem *item)
{
    if(!item){
        QMessageBox::warning(this, "Advertencia", "Selecciona un item de la tabla.", "Ok");
        return;
    }
    QTableWidget* tb = ui->tableWidget;

    switch(tipo)
    {
    case compra_items::ORDEN:{
        CompraOrden* w = new CompraOrden;
        w->set_widget_previous(this);
        QString id = tb->item(item->row(), 0)->text();
        QString persona_id = tb->item(item->row(), 1)->text();
        QString fecha_emision = tb->item(item->row(), 2)->text();
        QString serie = tb->item(item->row(), 3)->text();
        QString numero = tb->item(item->row(), 4)->text();
        QString ruc = tb->item(item->row(), 5)->text();
        QString razon_social = tb->item(item->row(), 6)->text();

        connect(w, SIGNAL(closing()), this, SLOT(on_compra_closing()));
        w->select(id, persona_id, fecha_emision, serie, numero, ruc, razon_social);
        SYSTEM->change_center_w(this, w);

    }break;
    case compra_items::FACTURA:{
        CompraFactura* w = new CompraFactura;
        w->set_widget_previous(this);
        QString id = tb->item(item->row(), 0)->text();
        QString persona_id = tb->item(item->row(), 1)->text();
        QString fecha_emision = tb->item(item->row(), 2)->text();
        QString serie = tb->item(item->row(), 3)->text();
        QString numero = tb->item(item->row(), 4)->text();
        QString ruc = tb->item(item->row(), 5)->text();
        QString razon_social = tb->item(item->row(), 6)->text();

        connect(w, SIGNAL(closing()), this, SLOT(on_compra_closing()));
        w->select(id, persona_id, fecha_emision, serie, numero, ruc, razon_social);
        SYSTEM->change_center_w(this, w);
    }break;
    case compra_items::BOLETA:{
        CompraBoleta* w = new CompraBoleta;
        w->set_widget_previous(this);
        QString id = tb->item(item->row(), 0)->text();
        QString persona_id = tb->item(item->row(), 1)->text();
        QString fecha_emision = tb->item(item->row(), 2)->text();
        QString serie = tb->item(item->row(), 3)->text();
        QString numero = tb->item(item->row(), 4)->text();
        QString ruc = tb->item(item->row(), 5)->text();
        QString razon_social = tb->item(item->row(), 6)->text();

        connect(w, SIGNAL(closing()), this, SLOT(on_compra_closing()));
        w->select(id, persona_id, fecha_emision, serie, numero, ruc, razon_social);
        SYSTEM->change_center_w(this, w);
    }break;
    case compra_items::GUIA_REMISION_REMITENTE:{
        CompraGuiaRR* w = new CompraGuiaRR;
        w->set_widget_previous(this);
        QString id = tb->item(item->row(), 0)->text();
        QString persona_id = tb->item(item->row(), 1)->text();
        QString fecha_emision = tb->item(item->row(), 2)->text();
        QString serie = tb->item(item->row(), 3)->text();
        QString numero = tb->item(item->row(), 4)->text();
        QString ruc = tb->item(item->row(), 5)->text();
        QString razon_social = tb->item(item->row(), 6)->text();

        connect(w, SIGNAL(closing()), this, SLOT(on_compra_closing()));
        w->select(id, persona_id, fecha_emision, serie, numero, ruc, razon_social);
        SYSTEM->change_center_w(this, w);
    }break;
    case compra_items::FLETE:{
        CompraFlete* w = new CompraFlete;
        w->set_widget_previous(this);
        QString id = tb->item(item->row(), 0)->text();
        QString persona_id = tb->item(item->row(), 1)->text();
        QString fecha_emision = tb->item(item->row(), 2)->text();
        QString serie = tb->item(item->row(), 3)->text();
        QString numero = tb->item(item->row(), 4)->text();
        QString ruc = tb->item(item->row(), 5)->text();
        QString razon_social = tb->item(item->row(), 6)->text();

        connect(w, SIGNAL(closing()), this, SLOT(on_compra_closing()));
        w->select(id, persona_id, fecha_emision, serie, numero, ruc, razon_social);
        SYSTEM->change_center_w(this, w);
    }break;
    case compra_items::NOTA_CREDITO:{
        CompraNotaCredito* w = new CompraNotaCredito;
        w->set_widget_previous(this);
        QString id = tb->item(item->row(), 0)->text();
        QString persona_id = tb->item(item->row(), 1)->text();
        QString fecha_emision = tb->item(item->row(), 2)->text();
        QString serie = tb->item(item->row(), 3)->text();
        QString numero = tb->item(item->row(), 4)->text();
        QString ruc = tb->item(item->row(), 5)->text();
        QString razon_social = tb->item(item->row(), 6)->text();

        connect(w, SIGNAL(closing()), this, SLOT(on_compra_closing()));
        w->select(id, persona_id, fecha_emision, serie, numero, ruc, razon_social);
        SYSTEM->change_center_w(this, w);
    }break;
    case compra_items::NOTA_DEBITO:{
        CompraNotaDebito* w = new CompraNotaDebito;
        w->set_widget_previous(this);
        QString id = tb->item(item->row(), 0)->text();
        QString persona_id = tb->item(item->row(), 1)->text();
        QString fecha_emision = tb->item(item->row(), 2)->text();
        QString serie = tb->item(item->row(), 3)->text();
        QString numero = tb->item(item->row(), 4)->text();
        QString ruc = tb->item(item->row(), 5)->text();
        QString razon_social = tb->item(item->row(), 6)->text();

        connect(w, SIGNAL(closing()), this, SLOT(on_compra_closing()));
        w->select(id, persona_id, fecha_emision, serie, numero, ruc, razon_social);
        SYSTEM->change_center_w(this, w);
    }break;
    case compra_items::SALDO:{
        CompraSaldo* w = new CompraSaldo;
        w->set_widget_previous(this);
        QString id = tb->item(item->row(), 0)->text();
        QString fecha_emision = tb->item(item->row(), 1)->text();

        connect(w, SIGNAL(closing()), this, SLOT(on_compra_closing()));
        w->select(id, fecha_emision);
        SYSTEM->change_center_w(this, w);
    }break;
    case compra_items::REG_SIN_DOC:{
        CompraRegSinDoc* w = new CompraRegSinDoc;
        w->set_widget_previous(this);
        QString id = tb->item(item->row(), 0)->text();
        QString fecha_emision = tb->item(item->row(), 1)->text();

        connect(w, SIGNAL(closing()), this, SLOT(on_compra_closing()));
        w->select(id, fecha_emision);
        SYSTEM->change_center_w(this, w);
    }break;
    }
}
