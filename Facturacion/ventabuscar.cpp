#include "ventabuscar.h"
#include "ui_ventabuscar.h"

VentaBuscar::VentaBuscar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VentaBuscar)
{
    ui->setupUi(this);

    widget_previous = NULL;

    modo_only_date = false;

    pos = 0;
    size_query = 10;

    QDate date;
    date.setDate(QDate::currentDate().year(), 1, 1);
    ui->dateEdit_inicio->setDate(date);
    ui->dateEdit_fin->setDate(QDate::currentDate());

    ui->radioButton_boleta->setChecked(true);
    tipo = venta_items::BOLETA;

    // INSTALL EVENT FILTERS
    this->installEventFilter(this);
    ui->dateEdit_inicio->installEventFilter(this);
    ui->dateEdit_fin->installEventFilter(this);
    ui->lineEdit_buscar->installEventFilter(this);
    ui->tableWidget->installEventFilter(this);
    ui->pushButton_ok->installEventFilter(this);
    ui->pushButton_salir->installEventFilter(this);
    ui->pushButton_agregar->installEventFilter(this);
}

VentaBuscar::~VentaBuscar()
{
    qDebug()<<"delete venta buscar"<<endl;
    delete ui;
}
void VentaBuscar::set_tipo(int tipo)
{
    this->tipo = tipo;
}

void VentaBuscar::set_widget_previous(QWidget *w)
{
    this->widget_previous = w;
}
void VentaBuscar::set_ruc(QString ruc)
{
    ui->lineEdit_buscar->setText(ruc);
    ui->lineEdit_buscar->setReadOnly(true);

    modo_only_date = true;

    ui->pushButton_agregar->hide();

    on_lineEdit_buscar_returnPressed();
}

void VentaBuscar::on_compra_closing()
{
    switch(tipo)
    {
    case venta_items::REGISTRO_SIN_DOCUMENTO:{
        VentaRegistroSinDoc* w = (VentaRegistroSinDoc*)QObject::sender();

        id = w->getID();
        persona_id = w->getPersonaID();
        tipo_persona_id = w->getTipoPersonaID();
        fecha_emision = w->getFechaEmision();
        serie = w->getSerie();
        numero = w->getNumero();
        codigo = w->getCodigo();
        nombre = w->getNombre();
        direccion = w->getDireccion();

    }break;
    case venta_items::BOLETA:{
        VentaBoleta* w = (VentaBoleta*)QObject::sender();

        id = w->getID();
        persona_id = w->getPersonaID();
        tipo_persona_id = w->getTipoPersonaID();
        fecha_emision = w->getFechaEmision();
        serie = w->getSerie();
        numero = w->getNumero();
        codigo = w->getCodigo();
        nombre = w->getNombre();
        direccion = w->getDireccion();

    }break;
    case venta_items::FACTURA:{
        VentaFactura* w = (VentaFactura*)QObject::sender();

        id = w->getID();
        persona_id = w->getPersonaID();
        tipo_persona_id = w->getTipoPersonaID();
        fecha_emision = w->getFechaEmision();
        serie = w->getSerie();
        numero = w->getNumero();
        codigo = w->getCodigo();
        nombre = w->getNombre();
        direccion = w->getDireccion();

    }break;
    case venta_items::NOTA_PEDIDO:{
        VentaNotaPedido* w= (VentaNotaPedido*)QObject::sender();

        id = w->getID();
        persona_id = w->getPersonaID();
        tipo_persona_id = w->getTipoPersonaID();
        fecha_emision = w->getFechaEmision();
        serie = w->getSerie();
        numero = w->getNumero();
        codigo = w->getCodigo();
        nombre = w->getNombre();
        direccion = w->getDireccion();

    }break;
    case venta_items::GUIA_REMISION_REMITENTE:{
        VentaGuiaRR* w= (VentaGuiaRR*)QObject::sender();

        id = w->getID();
        persona_id = w->getPersonaID();
        tipo_persona_id = w->getTipoPersonaID();
        fecha_emision = w->getFechaEmision();
        serie = w->getSerie();
        numero = w->getNumero();
        codigo = w->getCodigo();
        nombre = w->getNombre();
        direccion = w->getDireccion();
    }break;
    case venta_items::COTIZACION:{
        VentaCotizacion* w= (VentaCotizacion*)QObject::sender();

        id = w->getID();
        persona_id = w->getPersonaID();
        tipo_persona_id = w->getTipoPersonaID();
        fecha_emision = w->getFechaEmision();
        serie = w->getSerie();
        numero = w->getNumero();
        codigo = w->getCodigo();
        nombre = w->getNombre();
        direccion = w->getDireccion();
    }break;
    case venta_items::NOTA_CREDITO:{
        VentaNotaCredito* w= (VentaNotaCredito*)QObject::sender();

        id = w->getID();
        persona_id = w->getPersonaID();
        tipo_persona_id = w->getTipoPersonaID();
        fecha_emision = w->getFechaEmision();
        serie = w->getSerie();
        numero = w->getNumero();
        codigo = w->getCodigo();
        nombre = w->getNombre();
        direccion = w->getDireccion();        
    }break;
    case venta_items::NOTA_DEBITO:{
        VentaNotaDebito* w= (VentaNotaDebito*)QObject::sender();

        id = w->getID();
        persona_id = w->getPersonaID();
        tipo_persona_id = w->getTipoPersonaID();
        fecha_emision = w->getFechaEmision();
        serie = w->getSerie();
        numero = w->getNumero();
        codigo = w->getCodigo();
        nombre = w->getNombre();
        direccion = w->getDireccion();        
    }break;
    }
}
void VentaBuscar::on_lineEdit_buscar_textEdited(const QString &arg1)
{
    //connect(ui->lineEdit_buscar, SIGNAL(returnPressed()), this, SLOT(on_lineEdit_buscar_returnPressed()));
    pos = 0;

    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(0);
    ui->tableWidget->clear();
}

void VentaBuscar::on_lineEdit_buscar_returnPressed()
{
    //disconnect(ui->lineEdit_buscar, SIGNAL(returnPressed()), this, SLOT(on_lineEdit_buscar_returnPressed()));
    QString str_query;

    QString arg = ui->lineEdit_buscar->text();
    QString match = "";
    bool ok_ruc, ok_dni;

    arg.left(11).toFloat(&ok_ruc);
    arg.left(8).toFloat(&ok_dni);


    switch(tipo)
    {
    case venta_items::REGISTRO_SIN_DOCUMENTO:{
        str_query = "SELECT documento.id, cliente.id, persona.tipo_persona_id";
        str_query += ", anexo.fecha_emision, anexo.serie, anexo.numero";
        str_query += ", cliente.codigo, cliente.nombre, cliente.direccion";
        str_query += " FROM registro_sin_documento";
        str_query += " JOIN comprobante ON (comprobante.operacion_id = "+QString().setNum(operacion_items::VENTA)+" AND registro_sin_documento.comprobante_documento_id = comprobante.documento_id)";
        str_query += " JOIN documento ON registro_sin_documento.comprobante_documento_id = documento.id";
        str_query += " JOIN anexo ON documento.id = anexo.documento_id";
        str_query += " JOIN documento_h_persona ON documento_h_persona.documento_id = documento.id";
        str_query += " JOIN persona ON persona.id = documento_h_persona.persona_id";
        str_query += " JOIN ((SELECT juridica.persona_id AS id, juridica.ruc AS codigo, juridica.razon_social AS nombre, juridica.domicilio_fiscal AS direccion FROM juridica)";
        str_query += " UNION ALL";
        str_query += " (SELECT naturales.persona_id AS id, naturales.dni AS codigo, naturales.nombre AS nombre, naturales.direccion FROM naturales)) AS cliente ON cliente.id = persona.id";
        str_query += " WHERE";
        QString order_cliente;
        if(ok_ruc && arg.length() == 11){
            str_query += " cliente.codigo = '" + arg + "'";
            order_cliente = "cliente.codigo";
        }
        if(ok_dni && arg.length() == 8){
            str_query += " cliente.codigo = '" + arg + "'";
            order_cliente = "cliente.codigo";
        }
        if(!ok_ruc && !ok_dni){
            str_query += " cliente.nombre LIKE '"+arg+"%'";
            order_cliente = "cliente.nombre";
        }
        str_query += " AND (anexo.fecha_emision BETWEEN '"+ui->dateEdit_inicio->date().toString("yyyy-MM-dd")+"' AND '"+ui->dateEdit_fin->date().toString("yyyy-MM-dd")+"')";
        str_query += " ORDER BY anexo.fecha_emision DESC, anexo.serie DESC, anexo.numero DESC, "+order_cliente;
        str_query += " LIMIT " + QString().setNum(pos) + ", " + QString().setNum(size_query) + "";
    }break;
    case venta_items::BOLETA:{
        str_query = "SELECT documento.id, naturales.persona_id, persona.tipo_persona_id";
        str_query += ", anexo.fecha_emision, anexo.serie, anexo.numero";
        str_query += ", naturales.dni, naturales.nombre, naturales.direccion";
        str_query += " FROM boleta";
        str_query += " JOIN comprobante ON (comprobante.operacion_id = "+QString().setNum(operacion_items::VENTA)+" AND boleta.comprobante_documento_id = comprobante.documento_id)";
        str_query += " JOIN documento ON boleta.comprobante_documento_id = documento.id";
        str_query += " JOIN anexo ON documento.id = anexo.documento_id";
        str_query += " JOIN documento_h_persona ON documento_h_persona.documento_id = documento.id";
        str_query += " JOIN persona ON persona.id = documento_h_persona.persona_id";
        str_query += " JOIN naturales ON naturales.persona_id = persona.id";
        str_query += " WHERE";
        QString order_cliente;
        if(ok_dni && arg.length() == 8){
            str_query += " naturales.dni = '" + arg + "'";
            order_cliente = "naturales.dni";
        }
        if(!ok_dni){
            str_query += " naturales.nombre LIKE '"+arg+"%'";
            order_cliente = "naturales.nombre";
        }
        str_query += " AND (anexo.fecha_emision BETWEEN '"+ui->dateEdit_inicio->date().toString("yyyy-MM-dd")+"' AND '"+ui->dateEdit_fin->date().toString("yyyy-MM-dd")+"')";
        str_query += " ORDER BY anexo.fecha_emision DESC, anexo.serie DESC, anexo.numero DESC, "+order_cliente;
        str_query += " LIMIT " + QString().setNum(pos) + ", " + QString().setNum(size_query) + "";
    }break;
    case venta_items::FACTURA:{
        str_query = "SELECT documento.id, juridica.persona_id, persona.tipo_persona_id";
        str_query += ", anexo.fecha_emision, anexo.serie, anexo.numero";
        str_query += ", juridica.ruc, juridica.razon_social, juridica.domicilio_fiscal";
        str_query += " FROM factura";
        str_query += " JOIN comprobante ON (comprobante.operacion_id = "+QString().setNum(operacion_items::VENTA)+" AND factura.comprobante_documento_id = comprobante.documento_id)";
        str_query += " JOIN documento ON factura.comprobante_documento_id = documento.id";
        str_query += " JOIN anexo ON documento.id = anexo.documento_id";
        str_query += " JOIN documento_h_persona ON documento_h_persona.documento_id = documento.id";
        str_query += " JOIN persona ON persona.id = documento_h_persona.persona_id";
        str_query += " JOIN juridica ON juridica.persona_id = persona.id";
        str_query += " WHERE";
        QString order_cliente;
        if(ok_ruc && arg.length() == 11){
            str_query += " juridica.ruc = '" + arg + "'";
            order_cliente = "juridica.ruc";
        }
        if(!ok_ruc){
            str_query += " juridica.razon_social LIKE '"+arg+"%'";
            order_cliente = "juridica.razon_social";
        }
        str_query += " AND (anexo.fecha_emision BETWEEN '"+ui->dateEdit_inicio->date().toString("yyyy-MM-dd")+"' AND '"+ui->dateEdit_fin->date().toString("yyyy-MM-dd")+"')";
        str_query += " ORDER BY anexo.fecha_emision DESC, anexo.serie DESC, anexo.numero DESC, "+order_cliente;
        str_query += " LIMIT " + QString().setNum(pos) + ", " + QString().setNum(size_query) + "";
    }break;
    case venta_items::NOTA_PEDIDO:{
        str_query = "SELECT documento.id, cliente.id, persona.tipo_persona_id";
        str_query += ", anexo.fecha_emision, anexo.serie, anexo.numero";
        str_query += ", cliente.codigo, cliente.nombre, cliente.direccion";
        str_query += " FROM nota_pedido";
        str_query += " JOIN comprobante ON (comprobante.operacion_id = "+QString().setNum(operacion_items::VENTA)+" AND nota_pedido.comprobante_documento_id = comprobante.documento_id)";
        str_query += " JOIN documento ON nota_pedido.comprobante_documento_id = documento.id";
        str_query += " JOIN anexo ON documento.id = anexo.documento_id";
        str_query += " JOIN documento_h_persona ON documento_h_persona.documento_id = documento.id";
        str_query += " JOIN persona ON persona.id = documento_h_persona.persona_id";
        str_query += " JOIN ((SELECT juridica.persona_id AS id, juridica.ruc AS codigo, juridica.razon_social AS nombre, juridica.domicilio_fiscal AS direccion FROM juridica)";
        str_query += " UNION ALL";
        str_query += " (SELECT naturales.persona_id AS id, naturales.dni AS codigo, naturales.nombre AS nombre, naturales.direccion FROM naturales)) AS cliente ON cliente.id = persona.id";
        str_query += " WHERE";
        QString order_cliente;
        if(ok_ruc && arg.length() == 11){
            str_query += " cliente.codigo = '" + arg + "'";
            order_cliente = "cliente.codigo";
        }
        if(ok_dni && arg.length() == 8){
            str_query += " cliente.codigo = '" + arg + "'";
            order_cliente = "cliente.codigo";
        }
        if(!ok_ruc && !ok_dni){
            str_query += " cliente.nombre LIKE '"+arg+"%'";
            order_cliente = "cliente.nombre";
        }
        str_query += " AND (anexo.fecha_emision BETWEEN '"+ui->dateEdit_inicio->date().toString("yyyy-MM-dd")+"' AND '"+ui->dateEdit_fin->date().toString("yyyy-MM-dd")+"')";
        str_query += " ORDER BY anexo.fecha_emision DESC, anexo.serie DESC, anexo.numero DESC, "+order_cliente;
        str_query += " LIMIT " + QString().setNum(pos) + ", " + QString().setNum(size_query) + "";
    }break;
    case venta_items::GUIA_REMISION_REMITENTE:{
        str_query = "SELECT documento.id, cliente.id, persona.tipo_persona_id";
        str_query += ", anexo.fecha_emision, anexo.serie, anexo.numero";
        str_query += ", cliente.codigo, cliente.nombre, cliente.direccion";
        str_query += " FROM guia_remision_remitente";
        str_query += " JOIN comprobante ON (comprobante.operacion_id = "+QString().setNum(operacion_items::VENTA)+" AND guia_remision_remitente.comprobante_documento_id = comprobante.documento_id)";
        str_query += " JOIN documento ON guia_remision_remitente.comprobante_documento_id = documento.id";
        str_query += " JOIN anexo ON documento.id = anexo.documento_id";
        str_query += " JOIN documento_h_persona ON documento_h_persona.documento_id = documento.id";
        str_query += " JOIN persona ON persona.id = documento_h_persona.persona_id";
        str_query += " JOIN ((SELECT juridica.persona_id AS id, juridica.ruc AS codigo, juridica.razon_social AS nombre, juridica.domicilio_fiscal AS direccion FROM juridica)";
        str_query += " UNION ALL";
        str_query += " (SELECT naturales.persona_id AS id, naturales.dni AS codigo, naturales.nombre AS nombre, naturales.direccion FROM naturales)) AS cliente ON cliente.id = persona.id";
        str_query += " WHERE";
        QString order_cliente;
        if(ok_ruc && arg.length() == 11){
            str_query += " cliente.codigo = '" + arg + "'";
            order_cliente = "cliente.codigo";
        }
        if(ok_dni && arg.length() == 8){
            str_query += " cliente.codigo = '" + arg + "'";
            order_cliente = "cliente.codigo";
        }
        if(!ok_ruc && !ok_dni){
            str_query += " cliente.nombre LIKE '"+arg+"%'";
            order_cliente = "cliente.nombre";
        }
        str_query += " AND (anexo.fecha_emision BETWEEN '"+ui->dateEdit_inicio->date().toString("yyyy-MM-dd")+"' AND '"+ui->dateEdit_fin->date().toString("yyyy-MM-dd")+"')";
        str_query += " ORDER BY anexo.fecha_emision DESC, anexo.serie DESC, anexo.numero DESC, "+order_cliente;
        str_query += " LIMIT " + QString().setNum(pos) + ", " + QString().setNum(size_query) + "";
    }break;
    case venta_items::COTIZACION:{
        str_query = "SELECT documento.id, cliente.id, persona.tipo_persona_id";
        str_query += ", anexo.fecha_emision, anexo.serie, anexo.numero";
        str_query += ", cliente.codigo, cliente.nombre, cliente.direccion";
        str_query += " FROM cotizacion";
        str_query += " JOIN comprobante ON (comprobante.operacion_id = "+QString().setNum(operacion_items::VENTA)+" AND cotizacion.comprobante_documento_id = comprobante.documento_id)";
        str_query += " JOIN documento ON cotizacion.comprobante_documento_id = documento.id";
        str_query += " JOIN anexo ON documento.id = anexo.documento_id";
        str_query += " JOIN documento_h_persona ON documento_h_persona.documento_id = documento.id";
        str_query += " JOIN persona ON persona.id = documento_h_persona.persona_id";
        str_query += " JOIN ((SELECT juridica.persona_id AS id, juridica.ruc AS codigo, juridica.razon_social AS nombre, juridica.domicilio_fiscal AS direccion FROM juridica)";
        str_query += " UNION ALL";
        str_query += " (SELECT naturales.persona_id AS id, naturales.dni AS codigo, naturales.nombre AS nombre, naturales.direccion FROM naturales)) AS cliente ON cliente.id = persona.id";
        str_query += " WHERE";
        QString order_cliente;
        if(ok_ruc && arg.length() == 11){
            str_query += " cliente.codigo = '" + arg + "'";
            order_cliente = "cliente.codigo";
        }
        if(ok_dni && arg.length() == 8){
            str_query += " cliente.codigo = '" + arg + "'";
            order_cliente = "cliente.codigo";
        }
        if(!ok_ruc && !ok_dni){
            str_query += " cliente.nombre LIKE '"+arg+"%'";
            order_cliente = "cliente.nombre";
        }
        str_query += " AND (anexo.fecha_emision BETWEEN '"+ui->dateEdit_inicio->date().toString("yyyy-MM-dd")+"' AND '"+ui->dateEdit_fin->date().toString("yyyy-MM-dd")+"')";
        str_query += " ORDER BY anexo.fecha_emision DESC, anexo.serie DESC, anexo.numero DESC, "+order_cliente;
        str_query += " LIMIT " + QString().setNum(pos) + ", " + QString().setNum(size_query) + "";
    }break;
    case venta_items::NOTA_CREDITO:{
        str_query = "SELECT documento.id, cliente.id, persona.tipo_persona_id";
        str_query += ", anexo.fecha_emision, anexo.serie, anexo.numero";
        str_query += ", cliente.codigo, cliente.nombre, cliente.direccion";
        str_query += " FROM nota_credito";
        str_query += " JOIN comprobante ON (comprobante.operacion_id = "+QString().setNum(operacion_items::VENTA)+" AND nota_credito.comprobante_documento_id = comprobante.documento_id)";
        str_query += " JOIN documento ON nota_credito.comprobante_documento_id = documento.id";
        str_query += " JOIN anexo ON documento.id = anexo.documento_id";
        str_query += " JOIN documento_h_persona ON documento_h_persona.documento_id = documento.id";
        str_query += " JOIN persona ON persona.id = documento_h_persona.persona_id";
        str_query += " JOIN ((SELECT juridica.persona_id AS id, juridica.ruc AS codigo, juridica.razon_social AS nombre, juridica.domicilio_fiscal AS direccion FROM juridica)";
        str_query += " UNION ALL";
        str_query += " (SELECT naturales.persona_id AS id, naturales.dni AS codigo, naturales.nombre AS nombre, naturales.direccion FROM naturales)) AS cliente ON cliente.id = persona.id";
        str_query += " WHERE";
        QString order_cliente;
        if(ok_ruc && arg.length() == 11){
            str_query += " cliente.codigo = '" + arg + "'";
            order_cliente = "cliente.codigo";
        }
        if(ok_dni && arg.length() == 8){
            str_query += " cliente.codigo = '" + arg + "'";
            order_cliente = "cliente.codigo";
        }
        if(!ok_ruc && !ok_dni){
            str_query += " cliente.nombre LIKE '"+arg+"%'";
            order_cliente = "cliente.nombre";
        }
        str_query += " AND (anexo.fecha_emision BETWEEN '"+ui->dateEdit_inicio->date().toString("yyyy-MM-dd")+"' AND '"+ui->dateEdit_fin->date().toString("yyyy-MM-dd")+"')";
        str_query += " ORDER BY anexo.fecha_emision DESC, anexo.serie DESC, anexo.numero DESC, "+order_cliente;
        str_query += " LIMIT " + QString().setNum(pos) + ", " + QString().setNum(size_query) + "";
    }break;
    case venta_items::NOTA_DEBITO:{
        str_query = "SELECT documento.id, cliente.id, persona.tipo_persona_id";
        str_query += ", anexo.fecha_emision, anexo.serie, anexo.numero";
        str_query += ", cliente.codigo, cliente.nombre, cliente.direccion";
        str_query += " FROM nota_debito";
        str_query += " JOIN comprobante ON (comprobante.operacion_id = "+QString().setNum(operacion_items::VENTA)+" AND nota_debito.comprobante_documento_id = comprobante.documento_id)";
        str_query += " JOIN documento ON nota_debito.comprobante_documento_id = documento.id";
        str_query += " JOIN anexo ON documento.id = anexo.documento_id";
        str_query += " JOIN documento_h_persona ON documento_h_persona.documento_id = documento.id";
        str_query += " JOIN persona ON persona.id = documento_h_persona.persona_id";
        str_query += " JOIN ((SELECT juridica.persona_id AS id, juridica.ruc AS codigo, juridica.razon_social AS nombre, juridica.domicilio_fiscal AS direccion FROM juridica)";
        str_query += " UNION ALL";
        str_query += " (SELECT naturales.persona_id AS id, naturales.dni AS codigo, naturales.nombre AS nombre, naturales.direccion FROM naturales)) AS cliente ON cliente.id = persona.id";
        str_query += " WHERE";
        QString order_cliente;
        if(ok_ruc && arg.length() == 11){
            str_query += " cliente.codigo = '" + arg + "'";
            order_cliente = "cliente.codigo";
        }
        if(ok_dni && arg.length() == 8){
            str_query += " cliente.codigo = '" + arg + "'";
            order_cliente = "cliente.codigo";
        }
        if(!ok_ruc && !ok_dni){
            str_query += " cliente.nombre LIKE '"+arg+"%'";
            order_cliente = "cliente.nombre";
        }
        str_query += " AND (anexo.fecha_emision BETWEEN '"+ui->dateEdit_inicio->date().toString("yyyy-MM-dd")+"' AND '"+ui->dateEdit_fin->date().toString("yyyy-MM-dd")+"')";
        str_query += " ORDER BY anexo.fecha_emision DESC, anexo.serie DESC, anexo.numero DESC, "+order_cliente;
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
        case venta_items::REGISTRO_SIN_DOCUMENTO:{
            ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "ID"
                                                       << "PERSONA_ID" << "TIPO_PERSONA_ID"
                                                       << "FECHA EMISION"
                                                       << "SERIE" << "NUMERO"
                                                       << "CODIGO" << "NOMBRE"<<"DIRECCION");
            ui->tableWidget->setColumnHidden(0, true);
            ui->tableWidget->setColumnHidden(1, true);
            ui->tableWidget->setColumnHidden(2, true);
            ui->tableWidget->setColumnHidden(8, true);
        }break;
        case venta_items::BOLETA:{
            ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "ID"
                                                       << "PERSONA_ID" << "TIPO_PERSONA_ID"
                                                       << "FECHA_EMISION"
                                                       << "SERIE" << "NUMERO"
                                                       << "CODIGO" << "NOMBRE" << "DIRECCION");
            ui->tableWidget->setColumnHidden(0, true);
            ui->tableWidget->setColumnHidden(1, true);
            ui->tableWidget->setColumnHidden(2, true);
            ui->tableWidget->setColumnHidden(8, true);
        }break;
        case venta_items::FACTURA:{
            ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "ID"
                                                       << "PERSONA_ID" << "TIPO_PERSONA_ID"
                                                       << "FECHA_EMISION"
                                                       << "SERIE" << "NUMERO"
                                                       << "RUC" << "RAZON SOCIAL" << "DIRECCION");
            ui->tableWidget->setColumnHidden(0, true);
            ui->tableWidget->setColumnHidden(1, true);
            ui->tableWidget->setColumnHidden(2, true);
            ui->tableWidget->setColumnHidden(8, true);
        }break;
        case venta_items::NOTA_PEDIDO:{
            ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "ID"
                                                       << "PERSONA_ID" << "TIPO_PERSONA_ID"
                                                       << "FECHA_EMISION"
                                                       << "SERIE" << "NUMERO"
                                                       << "CODIGO" << "NOMBRE" << "DIRECCION");
            ui->tableWidget->setColumnHidden(0, true);
            ui->tableWidget->setColumnHidden(1, true);
            ui->tableWidget->setColumnHidden(2, true);
            ui->tableWidget->setColumnHidden(8, true);
        }break;
        case venta_items::GUIA_REMISION_REMITENTE:{
            ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "ID"
                                                       << "PERSONA_ID" << "TIPO_PERSONA_ID"
                                                       << "FECHA_EMISION"
                                                       << "SERIE" << "NUMERO"
                                                       << "CODIGO" << "NOMBRE" << "DIRECCION");
            ui->tableWidget->setColumnHidden(0, true);
            ui->tableWidget->setColumnHidden(1, true);
            ui->tableWidget->setColumnHidden(2, true);
            ui->tableWidget->setColumnHidden(8, true);
        }break;
        case venta_items::COTIZACION:{
            ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "ID"
                                                       << "PERSONA_ID" << "TIPO_PERSONA_ID"
                                                       << "FECHA_EMISION"
                                                       << "SERIE" << "NUMERO"
                                                       << "CODIGO" << "NOMBRE" << "DIRECCION");
            ui->tableWidget->setColumnHidden(0, true);
            ui->tableWidget->setColumnHidden(1, true);
            ui->tableWidget->setColumnHidden(2, true);
            ui->tableWidget->setColumnHidden(8, true);
        }break;
        case venta_items::NOTA_CREDITO:{
            ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "ID"
                                                       << "PERSONA_ID" << "TIPO_PERSONA_ID"
                                                       << "FECHA_EMISION"
                                                       << "SERIE" << "NUMERO"
                                                       << "CODIGO" << "NOMBRE" << "DIRECCION");
            ui->tableWidget->setColumnHidden(0, true);
            ui->tableWidget->setColumnHidden(1, true);
            ui->tableWidget->setColumnHidden(2, true);
            ui->tableWidget->setColumnHidden(8, true);
        }break;
        case venta_items::NOTA_DEBITO:{
            ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "ID"
                                                       << "PERSONA_ID" << "TIPO_PERSONA_ID"
                                                       << "FECHA_EMISION"
                                                       << "SERIE" << "NUMERO"
                                                       << "CODIGO" << "NOMBRE" << "DIRECCION");
            ui->tableWidget->setColumnHidden(0, true);
            ui->tableWidget->setColumnHidden(1, true);
            ui->tableWidget->setColumnHidden(2, true);
            ui->tableWidget->setColumnHidden(8, true);
        }break;
        }

        while (query.next()) {
            switch(tipo)
            {
            case venta_items::REGISTRO_SIN_DOCUMENTO:{
                QString id = query.value(0).toString();
                QString persona_id = query.value(1).toString();
                QString tipo_persona_id = query.value(2).toString();
                QString fecha_emision = query.value(3).toString();
                QString serie = query.value(4).toString();
                QString numero = query.value(5).toString();
                QString codigo = query.value(6).toString();
                QString nombre = query.value(7).toString();
                QString direccion = query.value(8).toString();

                ui->tableWidget->setItem(pos, 0, new QTableWidgetItem(id));
                ui->tableWidget->setItem(pos, 1, new QTableWidgetItem(persona_id));
                ui->tableWidget->setItem(pos, 2, new QTableWidgetItem(tipo_persona_id));
                ui->tableWidget->setItem(pos, 3, new QTableWidgetItem(fecha_emision));
                ui->tableWidget->setItem(pos, 4, new QTableWidgetItem(serie));
                ui->tableWidget->setItem(pos, 5, new QTableWidgetItem(numero));
                ui->tableWidget->setItem(pos, 6, new QTableWidgetItem(codigo));
                ui->tableWidget->setItem(pos, 7, new QTableWidgetItem(nombre));
                ui->tableWidget->setItem(pos, 8, new QTableWidgetItem(direccion));
            }break;
            case venta_items::BOLETA:{
                QString id = query.value(0).toString();
                QString persona_id = query.value(1).toString();
                QString tipo_persona_id = query.value(2).toString();
                QString fecha_emision = query.value(3).toString();
                QString serie = query.value(4).toString();
                QString numero = query.value(5).toString();
                QString codigo = query.value(6).toString();
                QString nombre = query.value(7).toString();
                QString direccion = query.value(8).toString();

                ui->tableWidget->setItem(pos, 0, new QTableWidgetItem(id));
                ui->tableWidget->setItem(pos, 1, new QTableWidgetItem(persona_id));
                ui->tableWidget->setItem(pos, 2, new QTableWidgetItem(tipo_persona_id));
                ui->tableWidget->setItem(pos, 3, new QTableWidgetItem(fecha_emision));
                ui->tableWidget->setItem(pos, 4, new QTableWidgetItem(serie));
                ui->tableWidget->setItem(pos, 5, new QTableWidgetItem(numero));
                ui->tableWidget->setItem(pos, 6, new QTableWidgetItem(codigo));
                ui->tableWidget->setItem(pos, 7, new QTableWidgetItem(nombre));
                ui->tableWidget->setItem(pos, 8, new QTableWidgetItem(direccion));
            }break;
            case venta_items::FACTURA:{
                QString id = query.value(0).toString();
                QString persona_id = query.value(1).toString();
                QString tipo_persona_id = query.value(2).toString();
                QString fecha_emision = query.value(3).toString();
                QString serie = query.value(4).toString();
                QString numero = query.value(5).toString();
                QString codigo = query.value(6).toString();
                QString nombre = query.value(7).toString();
                QString direccion = query.value(8).toString();

                ui->tableWidget->setItem(pos, 0, new QTableWidgetItem(id));
                ui->tableWidget->setItem(pos, 1, new QTableWidgetItem(persona_id));
                ui->tableWidget->setItem(pos, 2, new QTableWidgetItem(tipo_persona_id));
                ui->tableWidget->setItem(pos, 3, new QTableWidgetItem(fecha_emision));
                ui->tableWidget->setItem(pos, 4, new QTableWidgetItem(serie));
                ui->tableWidget->setItem(pos, 5, new QTableWidgetItem(numero));
                ui->tableWidget->setItem(pos, 6, new QTableWidgetItem(codigo));
                ui->tableWidget->setItem(pos, 7, new QTableWidgetItem(nombre));
                ui->tableWidget->setItem(pos, 8, new QTableWidgetItem(direccion));
            }break;
            case venta_items::NOTA_PEDIDO:{
                QString id = query.value(0).toString();
                QString persona_id = query.value(1).toString();
                QString tipo_persona_id = query.value(2).toString();
                QString fecha_emision = query.value(3).toString();
                QString serie = query.value(4).toString();
                QString numero = query.value(5).toString();
                QString codigo = query.value(6).toString();
                QString nombre = query.value(7).toString();
                QString direccion = query.value(8).toString();

                ui->tableWidget->setItem(pos, 0, new QTableWidgetItem(id));
                ui->tableWidget->setItem(pos, 1, new QTableWidgetItem(persona_id));
                ui->tableWidget->setItem(pos, 2, new QTableWidgetItem(tipo_persona_id));
                ui->tableWidget->setItem(pos, 3, new QTableWidgetItem(fecha_emision));
                ui->tableWidget->setItem(pos, 4, new QTableWidgetItem(serie));
                ui->tableWidget->setItem(pos, 5, new QTableWidgetItem(numero));
                ui->tableWidget->setItem(pos, 6, new QTableWidgetItem(codigo));
                ui->tableWidget->setItem(pos, 7, new QTableWidgetItem(nombre));
                ui->tableWidget->setItem(pos, 8, new QTableWidgetItem(direccion));
            }break;
            case venta_items::GUIA_REMISION_REMITENTE:{
                QString id = query.value(0).toString();
                QString persona_id = query.value(1).toString();
                QString tipo_persona_id = query.value(2).toString();
                QString fecha_emision = query.value(3).toString();
                QString serie = query.value(4).toString();
                QString numero = query.value(5).toString();
                QString codigo = query.value(6).toString();
                QString nombre = query.value(7).toString();
                QString direccion = query.value(8).toString();

                ui->tableWidget->setItem(pos, 0, new QTableWidgetItem(id));
                ui->tableWidget->setItem(pos, 1, new QTableWidgetItem(persona_id));
                ui->tableWidget->setItem(pos, 2, new QTableWidgetItem(tipo_persona_id));
                ui->tableWidget->setItem(pos, 3, new QTableWidgetItem(fecha_emision));
                ui->tableWidget->setItem(pos, 4, new QTableWidgetItem(serie));
                ui->tableWidget->setItem(pos, 5, new QTableWidgetItem(numero));
                ui->tableWidget->setItem(pos, 6, new QTableWidgetItem(codigo));
                ui->tableWidget->setItem(pos, 7, new QTableWidgetItem(nombre));
                ui->tableWidget->setItem(pos, 8, new QTableWidgetItem(direccion));
            }break;
            case venta_items::COTIZACION:{
                QString id = query.value(0).toString();
                QString persona_id = query.value(1).toString();
                QString tipo_persona_id = query.value(2).toString();
                QString fecha_emision = query.value(3).toString();
                QString serie = query.value(4).toString();
                QString numero = query.value(5).toString();
                QString codigo = query.value(6).toString();
                QString nombre = query.value(7).toString();
                QString direccion = query.value(8).toString();

                ui->tableWidget->setItem(pos, 0, new QTableWidgetItem(id));
                ui->tableWidget->setItem(pos, 1, new QTableWidgetItem(persona_id));
                ui->tableWidget->setItem(pos, 2, new QTableWidgetItem(tipo_persona_id));
                ui->tableWidget->setItem(pos, 3, new QTableWidgetItem(fecha_emision));
                ui->tableWidget->setItem(pos, 4, new QTableWidgetItem(serie));
                ui->tableWidget->setItem(pos, 5, new QTableWidgetItem(numero));
                ui->tableWidget->setItem(pos, 6, new QTableWidgetItem(codigo));
                ui->tableWidget->setItem(pos, 7, new QTableWidgetItem(nombre));
                ui->tableWidget->setItem(pos, 8, new QTableWidgetItem(direccion));
            }break;
            case venta_items::NOTA_CREDITO:{
                QString id = query.value(0).toString();
                QString persona_id = query.value(1).toString();
                QString tipo_persona_id = query.value(2).toString();
                QString fecha_emision = query.value(3).toString();
                QString serie = query.value(4).toString();
                QString numero = query.value(5).toString();
                QString codigo = query.value(6).toString();
                QString nombre = query.value(7).toString();
                QString direccion = query.value(8).toString();

                ui->tableWidget->setItem(pos, 0, new QTableWidgetItem(id));
                ui->tableWidget->setItem(pos, 1, new QTableWidgetItem(persona_id));
                ui->tableWidget->setItem(pos, 2, new QTableWidgetItem(tipo_persona_id));
                ui->tableWidget->setItem(pos, 3, new QTableWidgetItem(fecha_emision));
                ui->tableWidget->setItem(pos, 4, new QTableWidgetItem(serie));
                ui->tableWidget->setItem(pos, 5, new QTableWidgetItem(numero));
                ui->tableWidget->setItem(pos, 6, new QTableWidgetItem(codigo));
                ui->tableWidget->setItem(pos, 7, new QTableWidgetItem(nombre));
                ui->tableWidget->setItem(pos, 8, new QTableWidgetItem(direccion));
            }break;
            case venta_items::NOTA_DEBITO:{
                QString id = query.value(0).toString();
                QString persona_id = query.value(1).toString();
                QString tipo_persona_id = query.value(2).toString();
                QString fecha_emision = query.value(3).toString();
                QString serie = query.value(4).toString();
                QString numero = query.value(5).toString();
                QString codigo = query.value(6).toString();
                QString nombre = query.value(7).toString();
                QString direccion = query.value(8).toString();

                ui->tableWidget->setItem(pos, 0, new QTableWidgetItem(id));
                ui->tableWidget->setItem(pos, 1, new QTableWidgetItem(persona_id));
                ui->tableWidget->setItem(pos, 2, new QTableWidgetItem(tipo_persona_id));
                ui->tableWidget->setItem(pos, 3, new QTableWidgetItem(fecha_emision));
                ui->tableWidget->setItem(pos, 4, new QTableWidgetItem(serie));
                ui->tableWidget->setItem(pos, 5, new QTableWidgetItem(numero));
                ui->tableWidget->setItem(pos, 6, new QTableWidgetItem(codigo));
                ui->tableWidget->setItem(pos, 7, new QTableWidgetItem(nombre));
                ui->tableWidget->setItem(pos, 8, new QTableWidgetItem(direccion));
            }break;
            }
            ++pos;
        }
        SYSTEM->table_resize_to_contents(0, ui->tableWidget, size_query);

    } else {
    }
}
void VentaBuscar::on_pushButton_ok_clicked()
{
    QTableWidget* tb = ui->tableWidget;
    QTableWidgetItem* item = tb->currentItem();

    if(!item){
        return;
    }
    int row = item->row();

    id = tb->item(row, 0)->text();
    persona_id = tb->item(row, 1)->text();
    tipo_persona_id = tb->item(row, 2)->text();
    fecha_emision = tb->item(row, 3)->text();
    serie = tb->item(row, 4)->text();
    numero = tb->item(row, 5)->text();
    codigo = tb->item(row, 6)->text();
    nombre = tb->item(row, 7)->text();
    direccion = tb->item(row, 8)->text();

    if(widget_previous){
        this->setAttribute(Qt::WA_DeleteOnClose);
        SYSTEM->change_center_w(this, widget_previous);
    }else{
        SYSTEM->clear_center_w(this);
    }
}

void VentaBuscar::on_pushButton_salir_clicked()
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
void VentaBuscar::on_pushButton_agregar_clicked()
{
    int ret = QMessageBox::warning(this, "Advertencia", "¿Desea AGREGAR una COMPRA?", "Si", "No");
    switch(ret){
    case 0:{        
        switch(tipo)
        {
        case venta_items::REGISTRO_SIN_DOCUMENTO:{
            VentaRegistroSinDoc* w = new VentaRegistroSinDoc;
            w->set_widget_previous(this);            
            connect(w, SIGNAL(closing()), this, SLOT(on_compra_closing()));
            SYSTEM->change_center_w(this, w);
            w->next_serie_numero();
        }break;
        case venta_items::BOLETA:{
            VentaBoleta* w = new VentaBoleta;
            w->set_widget_previous(this);
            connect(w, SIGNAL(closing()), this, SLOT(on_compra_closing()));
            SYSTEM->change_center_w(this, w);
            w->next_serie_numero();
        }break;
        case venta_items::FACTURA:{
            VentaFactura* w = new VentaFactura;
            w->set_widget_previous(this);
            connect(w, SIGNAL(closing()), this, SLOT(on_compra_closing()));
            SYSTEM->change_center_w(this, w);
            w->next_serie_numero();
        }break;

        case venta_items::NOTA_PEDIDO:{
            VentaNotaPedido* w = new VentaNotaPedido;
            w->set_widget_previous(this);
            connect(w, SIGNAL(closing()), this, SLOT(on_compra_closing()));
            SYSTEM->change_center_w(this, w);
            w->next_serie_numero();
        }break;
        case venta_items::GUIA_REMISION_REMITENTE:{
            VentaGuiaRR* w = new VentaGuiaRR;
            w->set_widget_previous(this);
            connect(w, SIGNAL(closing()), this, SLOT(on_compra_closing()));
            SYSTEM->change_center_w(this, w);
            w->next_serie_numero();
        }break;
        case venta_items::COTIZACION:{
            VentaCotizacion* w = new VentaCotizacion;
            w->set_widget_previous(this);
            connect(w, SIGNAL(closing()), this, SLOT(on_compra_closing()));
            SYSTEM->change_center_w(this, w);
            w->next_serie_numero();
        }break;
        case venta_items::NOTA_CREDITO:{
            VentaNotaCredito* w = new VentaNotaCredito;
            w->set_widget_previous(this);
            connect(w, SIGNAL(closing()), this, SLOT(on_compra_closing()));
            SYSTEM->change_center_w(this, w);
            w->next_serie_numero();
        }break;
        case venta_items::NOTA_DEBITO:{
            VentaNotaDebito* w = new VentaNotaDebito;
            w->set_widget_previous(this);
            connect(w, SIGNAL(closing()), this, SLOT(on_compra_closing()));
            SYSTEM->change_center_w(this, w);
            w->next_serie_numero();
        }break;

        }
    }break;
    case 1:{

    }break;
    }
}

void VentaBuscar::on_radioButton_reg_sin_doc_clicked()
{
    tipo = venta_items::REGISTRO_SIN_DOCUMENTO;

    on_lineEdit_buscar_textEdited("");
    on_lineEdit_buscar_returnPressed();
}

void VentaBuscar::on_radioButton_boleta_clicked()
{
    tipo = venta_items::BOLETA;

    on_lineEdit_buscar_textEdited("");
    on_lineEdit_buscar_returnPressed();
}

void VentaBuscar::on_radioButton_factura_clicked()
{
    tipo = venta_items::FACTURA;

    on_lineEdit_buscar_textEdited("");
    on_lineEdit_buscar_returnPressed();
}

void VentaBuscar::on_radioButton_nota_pedido_clicked()
{
    tipo = venta_items::NOTA_PEDIDO;

    on_lineEdit_buscar_textEdited("");
    on_lineEdit_buscar_returnPressed();
}

void VentaBuscar::on_radioButton_guia_clicked()
{
    tipo = venta_items::GUIA_REMISION_REMITENTE;

    on_lineEdit_buscar_textEdited("");
    on_lineEdit_buscar_returnPressed();
}

void VentaBuscar::on_radioButton_cotizacion_clicked()
{
    tipo = venta_items::COTIZACION;

    on_lineEdit_buscar_textEdited("");
    on_lineEdit_buscar_returnPressed();
}

void VentaBuscar::on_radioButton_nota_credito_clicked()
{
    tipo = venta_items::NOTA_CREDITO;

    on_lineEdit_buscar_textEdited("");
    on_lineEdit_buscar_returnPressed();
}

void VentaBuscar::on_radioButton_nota_debito_clicked()
{
    tipo = venta_items::NOTA_DEBITO;

    on_lineEdit_buscar_textEdited("");
    on_lineEdit_buscar_returnPressed();
}
void VentaBuscar::on_tableWidget_itemDoubleClicked(QTableWidgetItem *item)
{
    QTableWidget* tb = ui->tableWidget;

    if(!item){
        return;
    }

    switch(tipo)
    {
    case venta_items::REGISTRO_SIN_DOCUMENTO:{
        VentaRegistroSinDoc* w = new VentaRegistroSinDoc;
        w->set_widget_previous(this);
        QString id = tb->item(item->row(), 0)->text();
        QString persona_id = tb->item(item->row(), 1)->text();
        QString tipo_persona_id = tb->item(item->row(), 2)->text();
        QString fecha_emision = tb->item(item->row(), 3)->text();
        QString serie = tb->item(item->row(), 4)->text();
        QString numero = tb->item(item->row(), 5)->text();
        QString codigo = tb->item(item->row(), 6)->text();
        QString nombre = tb->item(item->row(), 7)->text();
        QString direccion = tb->item(item->row(), 8)->text();

        connect(w, SIGNAL(closing()), this, SLOT(on_compra_closing()));
        w->select(id, persona_id, tipo_persona_id, fecha_emision, serie, numero, codigo, nombre, direccion);

        SYSTEM->change_center_w(this, w);

    }break;
    case venta_items::BOLETA:{
        VentaBoleta* w = new VentaBoleta;
        w->set_widget_previous(this);
        QString id = tb->item(item->row(), 0)->text();
        QString persona_id = tb->item(item->row(), 1)->text();
        QString tipo_persona_id = tb->item(item->row(), 2)->text();
        QString fecha_emision = tb->item(item->row(), 3)->text();
        QString serie = tb->item(item->row(), 4)->text();
        QString numero = tb->item(item->row(), 5)->text();
        QString codigo = tb->item(item->row(), 6)->text();
        QString nombre = tb->item(item->row(), 7)->text();
        QString direccion = tb->item(item->row(), 8)->text();

        connect(w, SIGNAL(closing()), this, SLOT(on_compra_closing()));
        w->select(id, persona_id, tipo_persona_id, fecha_emision, serie, numero, codigo, nombre, direccion);

        SYSTEM->change_center_w(this, w);
    }break;
    case venta_items::FACTURA:{
        VentaFactura* w = new VentaFactura;
        w->set_widget_previous(this);
        QString id = tb->item(item->row(), 0)->text();
        QString persona_id = tb->item(item->row(), 1)->text();
        QString tipo_persona_id = tb->item(item->row(), 2)->text();
        QString fecha_emision = tb->item(item->row(), 3)->text();
        QString serie = tb->item(item->row(), 4)->text();
        QString numero = tb->item(item->row(), 5)->text();
        QString codigo = tb->item(item->row(), 6)->text();
        QString nombre = tb->item(item->row(), 7)->text();
        QString direccion = tb->item(item->row(), 8)->text();

        connect(w, SIGNAL(closing()), this, SLOT(on_compra_closing()));
        w->select(id, persona_id, tipo_persona_id, fecha_emision, serie, numero, codigo, nombre, direccion);

        SYSTEM->change_center_w(this, w);
    }break;
    case venta_items::NOTA_PEDIDO:{
        VentaNotaPedido* w = new VentaNotaPedido;
        w->set_widget_previous(this);
        QString id = tb->item(item->row(), 0)->text();
        QString persona_id = tb->item(item->row(), 1)->text();
        QString tipo_persona_id = tb->item(item->row(), 2)->text();
        QString fecha_emision = tb->item(item->row(), 3)->text();
        QString serie = tb->item(item->row(), 4)->text();
        QString numero = tb->item(item->row(), 5)->text();
        QString codigo = tb->item(item->row(), 6)->text();
        QString nombre = tb->item(item->row(), 7)->text();
        QString direccion = tb->item(item->row(), 8)->text();

        connect(w, SIGNAL(closing()), this, SLOT(on_compra_closing()));
        w->select(id, persona_id, tipo_persona_id, fecha_emision, serie, numero, codigo, nombre, direccion);

        SYSTEM->change_center_w(this, w);
    }break;
    case venta_items::GUIA_REMISION_REMITENTE:{
        VentaGuiaRR* w = new VentaGuiaRR;
        w->set_widget_previous(this);
        QString id = tb->item(item->row(), 0)->text();
        QString persona_id = tb->item(item->row(), 1)->text();
        QString tipo_persona_id = tb->item(item->row(), 2)->text();
        QString fecha_emision = tb->item(item->row(), 3)->text();
        QString serie = tb->item(item->row(), 4)->text();
        QString numero = tb->item(item->row(), 5)->text();
        QString codigo = tb->item(item->row(), 6)->text();
        QString nombre = tb->item(item->row(), 7)->text();
        QString direccion = tb->item(item->row(), 8)->text();

        connect(w, SIGNAL(closing()), this, SLOT(on_compra_closing()));
        w->select(id, persona_id, tipo_persona_id, fecha_emision, serie, numero, codigo, nombre, direccion);

        SYSTEM->change_center_w(this, w);
    }break;
    case venta_items::COTIZACION:{
        VentaCotizacion* w = new VentaCotizacion;
        w->set_widget_previous(this);
        QString id = tb->item(item->row(), 0)->text();
        QString persona_id = tb->item(item->row(), 1)->text();
        QString tipo_persona_id = tb->item(item->row(), 2)->text();
        QString fecha_emision = tb->item(item->row(), 3)->text();
        QString serie = tb->item(item->row(), 4)->text();
        QString numero = tb->item(item->row(), 5)->text();
        QString codigo = tb->item(item->row(), 6)->text();
        QString nombre = tb->item(item->row(), 7)->text();
        QString direccion = tb->item(item->row(), 8)->text();

        connect(w, SIGNAL(closing()), this, SLOT(on_compra_closing()));
        w->select(id, persona_id, tipo_persona_id, fecha_emision, serie, numero, codigo, nombre, direccion);

        SYSTEM->change_center_w(this, w);
    }break;
    case venta_items::NOTA_CREDITO:{
        VentaNotaCredito* w = new VentaNotaCredito;
        w->set_widget_previous(this);
        QString id = tb->item(item->row(), 0)->text();
        QString persona_id = tb->item(item->row(), 1)->text();
        QString tipo_persona_id = tb->item(item->row(), 2)->text();
        QString fecha_emision = tb->item(item->row(), 3)->text();
        QString serie = tb->item(item->row(), 4)->text();
        QString numero = tb->item(item->row(), 5)->text();
        QString codigo = tb->item(item->row(), 6)->text();
        QString nombre = tb->item(item->row(), 7)->text();
        QString direccion = tb->item(item->row(), 8)->text();

        connect(w, SIGNAL(closing()), this, SLOT(on_compra_closing()));
        w->select(id, persona_id, tipo_persona_id, fecha_emision, serie, numero, codigo, nombre, direccion);

        SYSTEM->change_center_w(this, w);
    }break;
    case venta_items::NOTA_DEBITO:{
        VentaNotaDebito* w = new VentaNotaDebito;
        w->set_widget_previous(this);
        QString id = tb->item(item->row(), 0)->text();
        QString persona_id = tb->item(item->row(), 1)->text();
        QString tipo_persona_id = tb->item(item->row(), 2)->text();
        QString fecha_emision = tb->item(item->row(), 3)->text();
        QString serie = tb->item(item->row(), 4)->text();
        QString numero = tb->item(item->row(), 5)->text();
        QString codigo = tb->item(item->row(), 6)->text();
        QString nombre = tb->item(item->row(), 7)->text();
        QString direccion = tb->item(item->row(), 8)->text();

        connect(w, SIGNAL(closing()), this, SLOT(on_compra_closing()));
        w->select(id, persona_id, tipo_persona_id, fecha_emision, serie, numero, codigo, nombre, direccion);

        SYSTEM->change_center_w(this, w);
    }break;
    }
}
void VentaBuscar::showEvent(QShowEvent *event)
{
    event->accept();
    //if(focusWidget()){
        //focusWidget()->setFocus();
    //}else{
    if(modo_only_date){
        ui->tableWidget->setFocus(Qt::TabFocusReason);
    }else{
        ui->dateEdit_inicio->setFocus(Qt::TabFocusReason);
    }

    on_lineEdit_buscar_textEdited(ui->lineEdit_buscar->text());
    on_lineEdit_buscar_returnPressed();
}
void VentaBuscar::closeEvent(QCloseEvent *event)
{
    event->accept();
    emit closing();
}

bool VentaBuscar::eventFilter(QObject *obj, QEvent *e)
{
    QWidget* w_temp;
    w_temp = this;
    if(obj == w_temp){
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
            case Qt::Key_Return:{
                on_lineEdit_buscar_returnPressed();
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
            }
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
            case Qt::Key_Return:
                ui->pushButton_salir->click();
                return true;
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
            }

        }else{

        }
        return false;
    }
    return eventFilter(obj, e);
}
