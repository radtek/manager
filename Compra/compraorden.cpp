#include "compraorden.h"
#include "ui_compraorden.h"

CompraOrden::CompraOrden(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CompraOrden)
{    
    ui->setupUi(this);

    igv = SYSTEM->get_igv();
    dolar = SYSTEM->get_dolar();

    widget_previous = NULL;

    QRegExp regExp_ruc("[0-9]{11,11}");
    ui->lineEdit_cod->setValidator(new QRegExpValidator(regExp_ruc));

    // DISABLES

    //ui->lineEdit_cod->setReadOnly(true);
    ui->lineEdit_nombre->setReadOnly(true);

    // SET DATA
    disconnect(ui->dateTimeEdit_emision, SIGNAL(dateChanged(QDate)), this, SLOT(on_dateTimeEdit_emision_dateChanged(QDate)));
    ui->dateTimeEdit_emision->setDateTime(QDateTime::currentDateTime());
    mes_emision = QDate::currentDate().month();
    connect(ui->dateTimeEdit_emision, SIGNAL(dateChanged(QDate)), this, SLOT(on_dateTimeEdit_emision_dateChanged(QDate)));
    ui->dateTimeEdit_sistema->setDateTime(QDateTime::currentDateTime());

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(set_time()));
    timer->start(1000);

    // SET SERIE Y NUMERO
    A_Compra::set_serie_numero(ui->lineEdit_serie, ui->lineEdit_numero);

    // SET DELEGATE
    DoubleSpinBoxDelegate* dSpinBoxDelegate = new DoubleSpinBoxDelegate;
    dSpinBoxDelegate->setMinimum(0);
    dSpinBoxDelegate->setMaximum(999999999);
    dSpinBoxDelegate->setDecimals(DECIMALS_CANTIDAD);
    dSpinBoxDelegate->setSingleStep(1.0);
    ui->tableWidget->setItemDelegateForColumn(INDEX_CANTIDAD, dSpinBoxDelegate);
    dSpinBoxDelegate = new DoubleSpinBoxDelegate;
    dSpinBoxDelegate->setMinimum(0);
    dSpinBoxDelegate->setMaximum(999999999);
    dSpinBoxDelegate->setDecimals(DECIMALS_PRECIO_UNITARIO);
    dSpinBoxDelegate->setSingleStep(QString("0."+SYSTEM->zeros(DECIMALS_PRECIO_UNITARIO-1)+"1").toDouble());
    ui->tableWidget->setItemDelegateForColumn(INDEX_P_UNIT, dSpinBoxDelegate);
    dSpinBoxDelegate = new DoubleSpinBoxDelegate;
    dSpinBoxDelegate->setMinimum(0);
    dSpinBoxDelegate->setMaximum(999999999);
    dSpinBoxDelegate->setDecimals(DECIMALS_PRECIO_TOTAL);
    dSpinBoxDelegate->setSingleStep(QString("0."+SYSTEM->zeros(DECIMALS_PRECIO_TOTAL-1)+"1").toDouble());
    ui->tableWidget->setItemDelegateForColumn(INDEX_P_TOTAL, dSpinBoxDelegate);

    // HIDES
    ui->tableWidget->hideColumn(INDEX_ID);

    ui->label_cambio_dolar->hide();
    ui->label_cambio_dolar_value->hide();

    // SET CUSTOMS
    ui->label_igv->setText("IGV "+QString().setNum(igv, ' ', 3));
    ui->label_cambio_dolar_value->setText(QString().setNum(dolar, ' ', 3));

    // INSTALL EVENTS
    this->installEventFilter(this);
    ui->dateTimeEdit_emision->installEventFilter(this);
    ui->lineEdit_serie->installEventFilter(this);
    ui->lineEdit_numero->installEventFilter(this);
    ui->pushButton_proveedor->installEventFilter(this);
    ui->comboBox_modalidad->installEventFilter(this);
    ui->comboBox_moneda->installEventFilter(this);
    ui->pushButton_ing_prod->installEventFilter(this);
    ui->tableWidget->installEventFilter(this);
    ui->pushButton_guardar->installEventFilter(this);
    ui->pushButton_eliminar->installEventFilter(this);
    ui->pushButton_salir->installEventFilter(this);
}

CompraOrden::~CompraOrden()
{
    qDebug()<<"delete compra orden"<<endl;
    delete ui;
}

void CompraOrden::set_widget_previous(QWidget *w)
{
    this->widget_previous = w;
}

bool CompraOrden::select(QString id
                         , QString proveedor_id, QString fecha_emision
                         , QString serie, QString numero
                         , QString codigo, QString nombre)
{
    this->id = id;
    this->persona_id = proveedor_id;
    this->fecha_emision = fecha_emision;
    this->serie = serie;
    this->numero = numero;
    this->codigo = codigo;
    this->nombre = nombre;

    QString str_query;

    str_query = "SELECT producto.id, d_h_prod.cantidad, unidad.unidad";
    str_query += ", concat(producto.descripcion,' ',marca.marca), d_h_prod.precio FROM documento";
    //str_query += " JOIN documento_h_persona d_h_per ON d.id = d_h_per.documento_id";
    //str_query += " JOIN persona ON persona.id = d_h_per.persona_id";
    //str_query += " JOIN juridica ON persona.id = d_h_per.persona_id";
    //str_query += " JOIN proveedor ON persona.id = proveedor.juridica_persona_id";
    str_query += " JOIN documento_h_producto d_h_prod ON documento.id = d_h_prod.documento_id";
    str_query += " JOIN producto ON producto.id = d_h_prod.producto_id";
    str_query += " LEFT JOIN marca ON marca.id = producto.marca_id";
    str_query += " LEFT JOIN unidad ON unidad.id = producto.unidad_id";
    str_query += " WHERE documento.id = "+id+" GROUP BY d_h_prod.id";

    qDebug()<<str_query<<endl;
    if(query.exec(str_query)){
        QDateTime dt;
        QDate date = QDate::fromString(fecha_emision, "dd-MMM-yyyy");
        dt.setDate(date);

        ui->dateTimeEdit_emision->setDateTime(dt);

        ui->lineEdit_serie->setText(serie);
        ui->lineEdit_numero->setText(numero);


        // PROVEEDOR
        ui->lineEdit_cod->setText(codigo);
        ui->lineEdit_nombre->setText(nombre);

        // PRODUCTOS
        while(query.next()){
            QString producto_id = query.value(0).toString();
            QString cantidad = query.value(1).toString();
            QString unidad = query.value(2).toString();
            QString descripcion = query.value(3).toString();
            QString p_total = query.value(4).toString();
            set_producto(producto_id, cantidad, unidad, descripcion, p_total);
        }
        return true;
    }else{
        return false;
    }
}

void CompraOrden::set_proveedor(QString persona_id, QString codigo, QString nombre)
{
    /*
    this->persona_id = persona_id;
    this->codigo = codigo;
    this->nombre = nombre;
    */

    ui->lineEdit_cod->setText(codigo);
    ui->lineEdit_nombre->setText(nombre);
}
void CompraOrden::set_producto(QString producto_id
                               , QString cantidad, QString unidad
                               , QString descripcion, QString p_total)
{
    disconnect(ui->tableWidget, SIGNAL(itemChanged(QTableWidgetItem*))
               , this, SLOT(on_tableWidget_itemChanged(QTableWidgetItem*)));

    //producto_id = producto_id;
    cantidad = QString().setNum(cantidad.toDouble(), ' ', DECIMALS_CANTIDAD);
    //unidad = v[1];
    //descripcion = v[2];
    p_total = QString().setNum(p_total.toDouble(), ' ', DECIMALS_PRECIO_TOTAL);

    int rowCount = ui->tableWidget->rowCount();
    ui->tableWidget->setRowCount(rowCount+1);

    ui->tableWidget->setItem(rowCount, INDEX_ID, new QTableWidgetItem(producto_id));
    ui->tableWidget->setItem(rowCount, INDEX_CANTIDAD, new QTableWidgetItem(cantidad));
    ui->tableWidget->setItem(rowCount, INDEX_UNIDAD, new QTableWidgetItem(unidad));
    ui->tableWidget->setItem(rowCount, INDEX_DESCRIPCION, new QTableWidgetItem(descripcion));
    QString p_unitario = QString().setNum(p_total.toDouble()/cantidad.toDouble(), ' ', DECIMALS_CANTIDAD);
    QTableWidgetItem* item_p_unitario = new QTableWidgetItem(p_unitario);
    ui->tableWidget->setItem(rowCount, INDEX_P_UNIT, item_p_unitario);
    ui->tableWidget->setItem(rowCount, INDEX_P_TOTAL, new QTableWidgetItem(p_total));

    //ui->tableWidget->openPersistentEditor(ui->tableWidget->item(rowCount, INDEX_CANTIDAD));
    //ui->tableWidget->openPersistentEditor(ui->tableWidget->item(rowCount, INDEX_MEDIDA));
    //ui->tableWidget->openPersistentEditor(ui->tableWidget->item(rowCount, INDEX_DESCRIPCION));
    //ui->tableWidget->openPersistentEditor(ui->tableWidget->item(rowCount, INDEX_P_TOTAL));
    //ui->tableWidget->openPersistentEditor(ui->tableWidget->item(rowCount, INDEX_P_UNIT));
    //ui->tableWidget->openPersistentEditor(ui->tableWidget->item(rowCount, INDEX_REGALO));
    //ui->tableWidget->openPersistentEditor(ui->tableWidget->item(rowCount, INDEX_ID));

    ui->tableWidget->item(rowCount, INDEX_ID)->setFlags(Qt::ItemIsEnabled
                                                 | Qt::ItemIsSelectable);

    ui->tableWidget->item(rowCount, INDEX_CANTIDAD)->setFlags(Qt::ItemIsEnabled
                                                 | Qt::ItemIsSelectable
                                                 | Qt::ItemIsEditable);

    ui->tableWidget->item(rowCount, INDEX_UNIDAD)->setFlags(Qt::ItemIsEnabled
                                                 | Qt::ItemIsSelectable);

    ui->tableWidget->item(rowCount, INDEX_DESCRIPCION)->setFlags(Qt::ItemIsEnabled
                                                 | Qt::ItemIsSelectable
                                                 | Qt::ItemIsEditable);

    ui->tableWidget->item(rowCount, INDEX_P_UNIT)->setFlags(Qt::ItemIsEnabled
                                                 | Qt::ItemIsSelectable
                                                 | Qt::ItemIsEditable);

    ui->tableWidget->item(rowCount, INDEX_P_TOTAL)->setFlags(Qt::ItemIsEnabled
                                                 | Qt::ItemIsSelectable
                                                 | Qt::ItemIsEditable);


    double total = 0.0;
    rowCount = ui->tableWidget->rowCount();

    for(int i = 0; i < rowCount; i++){
        QString s_p_total = ui->tableWidget->item(i, INDEX_P_TOTAL)->text();

        double p_total = s_p_total.toDouble();

        total += p_total;
    }

    SYSTEM->table_resize_to_contents(0, ui->tableWidget);

    // CON IGV
    if(ui->comboBox_modalidad->currentIndex() == 0){
        ui->lineEdit_subtotal->setText(QString().setNum(total/(1.0+igv), ' ', DECIMALS_PRECIO_TOTAL));
        ui->lineEdit_igv->setText(QString().setNum(total-total/(1.0+igv), ' ', DECIMALS_PRECIO_TOTAL));
        ui->lineEdit_total->setText(QString().setNum(total, ' ', DECIMALS_PRECIO_TOTAL));
    }else{
        ui->lineEdit_subtotal->setText(QString().setNum(total, ' ', DECIMALS_PRECIO_TOTAL));
        ui->lineEdit_igv->setText(QString().setNum(total*(1.0+igv)-total, ' ', DECIMALS_PRECIO_TOTAL));
        ui->lineEdit_total->setText(QString().setNum(total*(1.0+igv), ' ', DECIMALS_PRECIO_TOTAL));
    }

    SYSTEM->table_resize_to_contents(0, ui->tableWidget);

    connect(ui->tableWidget, SIGNAL(itemChanged(QTableWidgetItem*))
               , this, SLOT(on_tableWidget_itemChanged(QTableWidgetItem*)));
}

bool CompraOrden::guardar()
{
    if(persona_id.compare("") == 0)
    {
        qDebug()<<"1"<<endl;
        return false;
    }
    if(ui->tableWidget->rowCount() <= 0)
    {
        qDebug()<<"2"<<endl;
        return false;
    }

    QString str_query;

    ui->dateTimeEdit_emision->setDisplayFormat("yyyy-MM-dd hh:mm:ss");
    ui->dateTimeEdit_sistema->setDisplayFormat("yyyy-MM-dd hh:mm:ss");

    if (id.compare("") == 0) {
        // DOCUMENTO
        str_query =  "INSERT INTO documento(tipo_documento_id, habilitado)VALUES(";
        str_query += QString().setNum(tipo_documento::ORDEN);
        str_query += ", 1)";
        str_query += "&&END_QUERY&&";

        // COMPROBANTE
        str_query +=  "INSERT INTO comprobante(documento_id";
        str_query += ", operacion_id)VALUES(";
        str_query += "(SELECT MAX(documento.id) FROM documento)";
        str_query += ", "+QString().setNum(operacion_items::COMPRA)+")";
        str_query += "&&END_QUERY&&";

        // ANEXO
        str_query +=  "INSERT INTO anexo(documento_id";
        str_query += ", fecha_emision, fecha_sistema, serie, numero)VALUES(";
        str_query += "(SELECT MAX(documento.id) FROM documento)";
        str_query += ", '"+ui->dateTimeEdit_emision->date().toString("yyyy-MM-dd")+"'";
        str_query += ", '"+ui->dateTimeEdit_sistema->dateTime().toString("yyyy-MM-dd hh:mm:ss")+"'";
        str_query += ", '"+ui->lineEdit_serie->text()+"'";
        str_query += ", '"+ui->lineEdit_numero->text()+"')";
        str_query += "&&END_QUERY&&";

        // DOCUMENTO_H_DOCUMENTO

        // DOCUMENTO_H_PERSONA
        str_query +=  "INSERT INTO documento_h_persona(documento_id, persona_id)VALUES(";
        str_query += "(SELECT MAX(documento.id) FROM documento), "+persona_id+")";
        str_query += "&&END_QUERY&&";

        // DOCUMENTO_H_PRODUCTO
        str_query +=  "INSERT INTO documento_h_producto(documento_id, producto_id, cantidad, precio)VALUES";
        QString str_query_2;
        for(int i= 0; i< ui->tableWidget->rowCount(); i++){
            QString producto_id = ui->tableWidget->item(i, INDEX_ID)->text();
            QString cantidad = ui->tableWidget->item(i, INDEX_CANTIDAD)->text();
            QString total = ui->tableWidget->item(i, INDEX_P_TOTAL)->text();

            str_query_2 += QString()+",((SELECT MAX(documento.id) FROM documento), "+producto_id+", "+cantidad+", "+total+")";
        }

        str_query_2.remove(0,1);
        str_query += str_query_2+"&&END_QUERY&&";

        // ORDEN
        str_query += "INSERT INTO orden(comprobante_documento_id, modalidad, moneda";
        str_query += ")VALUES(";
        str_query += "(SELECT MAX(documento.id) FROM documento)";
        str_query += ", "+QString().setNum(ui->comboBox_modalidad->currentIndex());
        str_query += ", "+QString().setNum(ui->comboBox_moneda->currentIndex())+")";
        str_query += "&&END_QUERY&&";
    }else{
        // COMPROBANTE
        /*
        str_query =  "UPDATE comprobante SET";
        //str_query += " WHERE documento_id = "+id;
        str_query += "&&END_QUERY&&";
        */
        // ANEXO
        str_query +=  "UPDATE anexo SET";
        str_query += " fecha_emision = '"+ui->dateTimeEdit_emision->date().toString("yyyy-MM-dd")+"'";
        str_query += ", fecha_sistema = '"+ui->dateTimeEdit_sistema->dateTime().toString("yyyy-MM-dd hh:mm:ss")+"'";
        str_query += ", serie = '"+ui->lineEdit_serie->text()+"'";
        str_query += ", numero = '"+ui->lineEdit_numero->text()+"'";
        str_query += " WHERE documento_id = "+id;
        str_query += "&&END_QUERY&&";

        // DOCUMENTO_H_DOCUMENTO

        // DOCUMENTO_H_PERSONA
        str_query +=  "UPDATE documento_h_persona SET";
        str_query += " persona_id = "+persona_id;
        str_query += " WHERE documento_id = "+id;
        str_query += "&&END_QUERY&&";

        // DOCUMENTO_H_PRODUCTO
        str_query += "DELETE FROM documento_h_producto WHERE documento_id = "+id;
        str_query += "&&END_QUERY&&";
        str_query += "INSERT INTO documento_h_producto(documento_id, producto_id, cantidad, precio)VALUES";
        QString str_query_2;
        for(int i= 0; i< ui->tableWidget->rowCount(); i++){
            QString producto_id = ui->tableWidget->item(i, INDEX_ID)->text();
            QString cantidad = ui->tableWidget->item(i, INDEX_CANTIDAD)->text();
            QString total = ui->tableWidget->item(i, INDEX_P_TOTAL)->text();

            str_query_2 += ",("+id+", "+producto_id+", "+cantidad+", "+total+")";
        }

        str_query_2.remove(0,1);
        str_query += str_query_2;
        /*
        str_query += " ON DUPLICATE KEY UPDATE documento_id=VALUES(documento_id)";
        str_query += ", producto_id=VALUES(producto_id)";
        str_query += ", cantidad=VALUES(cantidad_id)";
        str_query += ", precio=VALUES(precio_id)";*/
        str_query += "&&END_QUERY&&";

        str_query += "UPDATE orden SET";
        str_query += " modalidad = "+QString().setNum(ui->comboBox_modalidad->currentIndex());
        str_query += ", moneda = "+QString().setNum(ui->comboBox_moneda->currentIndex());
        str_query += " WHERE comprobante_documento_id = "+id;
        str_query += "&&END_QUERY&&";
    }
    str_query += "COMMIT";
    str_query += "&&END_QUERY&&";

    ui->dateTimeEdit_emision->setDisplayFormat("dd-MM-yyyy");
    ui->dateTimeEdit_sistema->setDisplayFormat("dd-MM-yyyy hh:mm:ss");

    QSqlQuery query;
    SYSTEM->multiple_query(str_query);
    qDebug()<<str_query<<endl;
    if(query.exec(str_query)){
        return true;
    }else{
        return false;
    }
}
bool CompraOrden::remove()
{
    QString str_query;

    str_query = "DELETE FROM documento WHERE id = "+id;
    str_query += "&&END_QUERY&&";
    str_query += "COMMIT";
    str_query += "&&END_QUERY&&";

    QSqlQuery query;

    SYSTEM->multiple_query(str_query);
    qDebug()<<str_query<<endl;
    if(query.exec(str_query)){
        id = "";
        return true;
    }else{
        return false;
    }
}

void CompraOrden::set_time()
{
    ui->dateTimeEdit_sistema->setDateTime(QDateTime::currentDateTime());
}

void CompraOrden::on_proveedor_closing()
{
    ColaboradorBuscar* w = (ColaboradorBuscar*)sender();
    QString persona_id = w->get_ID();
    if(persona_id.compare("") == 0) {
        return;
    }
    this->persona_id = persona_id;
    codigo = w->get_codigo();
    nombre = w->get_nombre();

    set_proveedor(persona_id, codigo, nombre);

    ui->pushButton_ing_prod->setFocus(Qt::TabFocusReason);
}
void CompraOrden::on_producto_closing()
{
    ProductoBuscar* w = (ProductoBuscar*)sender();
    QString producto_id = w->getID();
    if(producto_id.compare("") == 0){
        return;
    }
    QString cantidad = "1.0";
    QString unidad = w->getUnidad();
    QString marca = w->getMarca();
    QString descripcion = w->getDescripcion()+" "+marca;
    QString p_total = w->getPrecio();

    QString str_query;
    str_query += "SELECT t.id, t.tipo_documento_id, t.fecha_emision";
    str_query += ", t.cantidad_bruto";
    str_query += ", t.cantidad_bruto - t.cantidad_nc";
    str_query += ", t.precio / t.cantidad_bruto AS precio_bruto";
    str_query += ", t.precio_flete / t.cantidad_bruto AS flete_unit";
    str_query += ", IF((t.cantidad_bruto - t.cantidad_nc) <> 0, t.desc_nc / (t.cantidad_bruto - t.cantidad_nc), 0) AS desc_nc_unit";
    str_query += ", IF((t.cantidad_bruto - t.cantidad_nc) <> 0, IF(t.total <> 0, (t.precio / t.total), 0) * t.nc_monto / (t.cantidad_bruto - t.cantidad_nc), 0) AS desc_nc_monto_unit";
    str_query += ", t.serie AS serie, t.numero AS numero";
    str_query += ", t.persona_id AS persona_id, t.ruc AS ruc, t.razon_social AS razon_social FROM";
    str_query += " (SELECT";
    str_query += " documento.id AS id";
    str_query += ", documento.tipo_documento_id AS tipo_documento_id";
    str_query += ", anexo.fecha_emision AS fecha_emision";
    str_query += ", SUM(documento_h_producto.cantidad) AS cantidad_bruto";
    str_query += ", IFNULL((SELECT SUM(d_h_prod_nc_cantidad.cantidad_aux) FROM documento d";
    str_query += " JOIN documento_h_documento d_h_d_nc ON d.id = d_h_d_nc.documento_id1";
    str_query += " JOIN documento d_nc ON (d_h_d_nc.documento_id = d_nc.id AND d_nc.tipo_documento_id = "+QString().setNum(tipo_documento::NOTA_CREDITO)+")";
    str_query += " JOIN documento_h_producto d_h_prod_nc_cantidad ON (d_h_prod_nc_cantidad.producto_id = "+producto_id+" AND d_h_prod_nc_cantidad.documento_id = d_nc.id)";
    str_query += " WHERE d.id = documento.id), 0) AS cantidad_nc";
    str_query += ", SUM(documento_h_producto.precio) AS precio";
    str_query += ", (SELECT SUM(documento_h_producto.precio) FROM documento_h_producto";
    str_query += " WHERE documento.id = documento_h_producto.documento_id) AS total";
    str_query += ", IFNULL((SELECT SUM(d_h_prod_flete.precio_aux) FROM documento d";
    str_query += " JOIN documento_h_documento d_h_d_flete ON d.id = d_h_d_flete.documento_id1";
    str_query += " JOIN documento d_flete ON (d_h_d_flete.documento_id = d_flete.id AND d_flete.tipo_documento_id = "+QString().setNum(tipo_documento::FLETE)+")";
    str_query += " JOIN documento_h_producto d_h_prod_flete ON (d_h_prod_flete.producto_id = "+producto_id+" AND d_h_prod_flete.documento_id = d_flete.id)";
    str_query += " WHERE d.id = documento.id), 0) AS precio_flete";
    str_query += ", IFNULL((SELECT SUM(d_h_prod_nc_desc.precio_aux) FROM documento d";
    str_query += " JOIN documento_h_documento d_h_d_nc ON d.id = d_h_d_nc.documento_id1";
    str_query += " JOIN documento d_nc ON (d_h_d_nc.documento_id = d_nc.id AND d_nc.tipo_documento_id = "+QString().setNum(tipo_documento::NOTA_CREDITO)+")";
    str_query += " JOIN documento_h_producto d_h_prod_nc_desc ON (d_h_prod_nc_desc.producto_id = "+producto_id+" AND d_h_prod_nc_desc.documento_id = d_nc.id)";
    str_query += " WHERE d.id = documento.id), 0) AS desc_nc";
    str_query += ", IFNULL((SELECT SUM(nc.monto) FROM documento d";
    str_query += " JOIN documento_h_documento d_h_d_nc ON d.id = d_h_d_nc.documento_id1";
    str_query += " JOIN documento d_nc ON (d_h_d_nc.documento_id = d_nc.id AND d_nc.tipo_documento_id = "+QString().setNum(tipo_documento::NOTA_CREDITO)+")";
    str_query += " JOIN nota_credito nc ON d_nc.id = nc.comprobante_documento_id";
    str_query += " WHERE d.id = documento.id), 0) AS nc_monto";
    str_query += ", anexo.serie AS serie";
    str_query += ", anexo.numero AS numero";
    str_query += ", persona.id AS persona_id";
    str_query += ", juridica.ruc AS ruc";
    str_query += ", juridica.razon_social AS razon_social";
    str_query += " FROM producto";
    str_query += " JOIN documento_h_producto ON producto.id = documento_h_producto.producto_id";

    str_query += " JOIN documento ON ((documento.tipo_documento_id = "+QString().setNum(tipo_documento::SALDO);
    str_query += " OR documento.tipo_documento_id = "+QString().setNum(tipo_documento::BOLETA);
    str_query += " OR documento.tipo_documento_id = "+QString().setNum(tipo_documento::FACTURA)+")";
    str_query += " AND documento.id = documento_h_producto.documento_id)";
    str_query += " JOIN comprobante ON (comprobante.operacion_id = "+QString().setNum(operacion_items::COMPRA)+" AND documento.id = comprobante.documento_id)";

    str_query += " LEFT JOIN saldo ON saldo.comprobante_documento_id = documento.id";
    str_query += " LEFT JOIN boleta ON boleta.comprobante_documento_id = documento.id";
    str_query += " LEFT JOIN factura ON factura.comprobante_documento_id = documento.id";

    str_query += " JOIN anexo ON documento.id = anexo.documento_id";
    str_query += " LEFT JOIN documento_h_persona ON documento.id = documento_h_persona.documento_id";
    str_query += " LEFT JOIN persona ON persona.id = documento_h_persona.persona_id";
    str_query += " LEFT JOIN juridica ON persona.id = juridica.persona_id";
    str_query += " LEFT JOIN proveedor ON persona.id = proveedor.juridica_persona_id";
    str_query += " WHERE producto.id = "+producto_id+" AND YEAR(anexo.fecha_emision) = "+QString().setNum(QDate::currentDate().year());
    str_query += " GROUP BY documento.id";
    str_query += " ORDER BY anexo.fecha_emision DESC, documento.id DESC";
    str_query += " LIMIT 1) AS t";

    qDebug()<<str_query<<endl;
    QSqlQuery query;
    if(query.exec(str_query)){
        if(query.next()){
            double cant = query.value(3).toDouble();
            QString cantidad = QString().setNum(cant, ' ', 4);

            double cant_final = query.value(4).toDouble();
            QString cantidad_final = QString().setNum(cant_final, ' ', 4);

            double precio = query.value(5).toDouble();
            QString precio_bruto = QString().setNum(precio, ' ', 3);

            double flete = query.value(6).toDouble();
            QString precio_flete = QString().setNum(flete, ' ', 3);

            double desc_nc = query.value(7).toDouble();
            QString descuento_nc = QString().setNum(desc_nc, ' ', 3);

            double desc_nc_monto = query.value(8).toDouble();
            QString descuento_nc_monto = QString().setNum(desc_nc_monto, ' ', 3);

            double precio_neto_val = precio + flete - desc_nc - desc_nc_monto;
            QString precio_neto = QString().setNum(precio_neto_val, ' ', 3);

            p_total = precio_neto;
        }
    }else{

    }

    set_producto(producto_id, cantidad, unidad, descripcion, p_total);
}
void CompraOrden::on_pushButton_proveedor_clicked()
{
    CompraProveedor* w_buscar_proveedor = new CompraProveedor;
    w_buscar_proveedor->set_tipo(persona_items::PROVEEDOR);
    w_buscar_proveedor->set_widget_previous(this);
    connect(w_buscar_proveedor, SIGNAL(closing()), this, SLOT(on_proveedor_closing()));

    SYSTEM->change_center_w(this, w_buscar_proveedor);
}

void CompraOrden::on_pushButton_ing_prod_clicked()
{
    /*
    if(persona_id == ""){
        QMessageBox::warning(this, "Advertencia", "Ingrese proveedor.", "Salir");
        return;
    }
    */

    CompraProducto* w_buscar_producto = new CompraProducto;
    w_buscar_producto->set_widget_previous(this);
    connect(w_buscar_producto, SIGNAL(closing()), this, SLOT(on_producto_closing()));

    SYSTEM->change_center_w(this, w_buscar_producto);
}

void CompraOrden::on_comboBox_modalidad_currentIndexChanged(int index)
{
    disconnect(ui->tableWidget, SIGNAL(itemChanged(QTableWidgetItem*))
               , this, SLOT(on_tableWidget_itemChanged(QTableWidgetItem*)));

    double total = 0.0;
    int rowCount = ui->tableWidget->rowCount();

    for(int i = 0; i < rowCount; i++){
        QString s_p_total = ui->tableWidget->item(i, INDEX_P_TOTAL)->text();

        double p_total = s_p_total.toDouble();

        total += p_total;
    }

    // CON IGV
    if(index == 0){
        ui->lineEdit_subtotal->setText(QString().setNum(total/(1.0+igv), ' ', DECIMALS_PRECIO_TOTAL));
        ui->lineEdit_igv->setText(QString().setNum(total-total/(1.0+igv), ' ', DECIMALS_PRECIO_TOTAL));
        ui->lineEdit_total->setText(QString().setNum(total, ' ', DECIMALS_PRECIO_TOTAL));
    }else{
        ui->lineEdit_subtotal->setText(QString().setNum(total, ' ', DECIMALS_PRECIO_TOTAL));
        ui->lineEdit_igv->setText(QString().setNum(total*(1.0+igv)-total, ' ', DECIMALS_PRECIO_TOTAL));
        ui->lineEdit_total->setText(QString().setNum(total*(1.0+igv), ' ', DECIMALS_PRECIO_TOTAL));
    }

    connect(ui->tableWidget, SIGNAL(itemChanged(QTableWidgetItem*))
               , this, SLOT(on_tableWidget_itemChanged(QTableWidgetItem*)));
}
void CompraOrden::on_comboBox_moneda_currentIndexChanged(int index)
{
    disconnect(ui->tableWidget, SIGNAL(itemChanged(QTableWidgetItem*))
               , this, SLOT(on_tableWidget_itemChanged(QTableWidgetItem*)));

    // SOLES
    if(index == 0){
        ui->label_cambio_dolar->hide();
        ui->label_cambio_dolar_value->hide();

        ui->tableWidget->horizontalHeaderItem(INDEX_P_UNIT)->setText("P. Unit. S/.");
        ui->tableWidget->horizontalHeaderItem(INDEX_P_TOTAL)->setText("P. Total S/.");
        ui->label_subtotal->setText("Sub Total S/.");
        ui->label_total->setText("Total S/.");
    }

    // DOLARES
    if(index == 1){
        ui->label_cambio_dolar->show();
        ui->label_cambio_dolar_value->show();

        ui->label_cambio_dolar_value->setText(QString().setNum(SYSTEM->get_dolar(ui->dateTimeEdit_emision->date()), ' ', 3));

        ui->tableWidget->horizontalHeaderItem(INDEX_P_UNIT)->setText("P. Unit. $");
        ui->tableWidget->horizontalHeaderItem(INDEX_P_TOTAL)->setText("P. Total $");
        ui->label_subtotal->setText("Sub Total $");
        ui->label_total->setText("Total $");
    }

    SYSTEM->table_resize_to_contents(this, ui->tableWidget);

    connect(ui->tableWidget, SIGNAL(itemChanged(QTableWidgetItem*))
               , this, SLOT(on_tableWidget_itemChanged(QTableWidgetItem*)));
}

void CompraOrden::on_tableWidget_itemChanged(QTableWidgetItem *item)
{
    disconnect(ui->tableWidget, SIGNAL(itemChanged(QTableWidgetItem*))
               , this, SLOT(on_tableWidget_itemChanged(QTableWidgetItem*)));

    // CANTIDAD
    if(item->column() == INDEX_CANTIDAD){
        QString s_cantidad = ui->tableWidget->item(item->row(), INDEX_CANTIDAD)->text();
        QString s_p_unitario = ui->tableWidget->item(item->row(), INDEX_P_UNIT)->text();

        double cantidad = s_cantidad.toDouble();
        double p_unitario = s_p_unitario.toDouble();
        double p_total;

        if(cantidad == 0.0){
            QMessageBox::warning(this, "Advertencia", "La cantidad no puede ser cero.", "Salir");
            cantidad = 1.0;
        }
        p_total = cantidad*p_unitario;

        ui->tableWidget->item(item->row(), INDEX_P_TOTAL)->setText(QString().setNum(p_total, ' ', DECIMALS_PRECIO_TOTAL));
    }
    // P_UNITARIO
    if(item->column() == INDEX_P_UNIT){
        QString s_cantidad = ui->tableWidget->item(item->row(), INDEX_CANTIDAD)->text();
        QString s_p_unitario = ui->tableWidget->item(item->row(), INDEX_P_UNIT)->text();

        double cantidad = s_cantidad.toDouble();
        double p_unitario = s_p_unitario.toDouble();
        double p_total;

        p_total = cantidad*p_unitario;

        ui->tableWidget->item(item->row(), INDEX_P_TOTAL)->setText(QString().setNum(p_total, ' ', DECIMALS_PRECIO_TOTAL));
    }
    // P_TOTAL
    if(item->column() == INDEX_P_TOTAL){
        QString s_cantidad = ui->tableWidget->item(item->row(), INDEX_CANTIDAD)->text();
        QString s_p_total = ui->tableWidget->item(item->row(), INDEX_P_TOTAL)->text();

        double cantidad = s_cantidad.toDouble();
        double p_total = s_p_total.toDouble();

        double p_unitario = p_total/cantidad;

        ui->tableWidget->item(item->row(), INDEX_P_UNIT)->setText(QString().setNum(p_unitario, ' ', DECIMALS_PRECIO_UNITARIO));
    }
    double total = 0.0;
    int rowCount = ui->tableWidget->rowCount();

    for(int i = 0; i < rowCount; i++){
        QString s_p_total = ui->tableWidget->item(i, INDEX_P_TOTAL)->text();

        double p_total = s_p_total.toDouble();

        total += p_total;
    }
    SYSTEM->table_resize_to_contents(0, ui->tableWidget);

    // CON IGV
    if(ui->comboBox_modalidad->currentIndex() == 0){
        ui->lineEdit_subtotal->setText(QString().setNum(total/(1.0+igv), ' ', DECIMALS_PRECIO_TOTAL));
        ui->lineEdit_igv->setText(QString().setNum(total-total/(1.0+igv), ' ', DECIMALS_PRECIO_TOTAL));
        ui->lineEdit_total->setText(QString().setNum(total, ' ', DECIMALS_PRECIO_TOTAL));
    }else{
        ui->lineEdit_subtotal->setText(QString().setNum(total, ' ', DECIMALS_PRECIO_TOTAL));
        ui->lineEdit_igv->setText(QString().setNum(total*(1.0+igv)-total, ' ', DECIMALS_PRECIO_TOTAL));
        ui->lineEdit_total->setText(QString().setNum(total*(1.0+igv), ' ', DECIMALS_PRECIO_TOTAL));
    }

    connect(ui->tableWidget, SIGNAL(itemChanged(QTableWidgetItem*))
               , this, SLOT(on_tableWidget_itemChanged(QTableWidgetItem*)));
}
void CompraOrden::on_pushButton_amarres_clicked()
{
    CompraAmarres* w_compra_amarres = new CompraAmarres;
    w_compra_amarres->set_widget_previous(this);
    w_compra_amarres->set_documento(this->id, tipo_documento::ORDEN);

    SYSTEM->change_center_w(this, w_compra_amarres);
}

void CompraOrden::on_pushButton_guardar_clicked()
{    
    int ret = QMessageBox::warning(this, "Advertencia", "¿Desea guardar los datos?", "Si", "No");
    switch(ret){
    case 0:{
        if(guardar()){
            QMessageBox::information(this, "Información", "Se guardo con éxito.");
            setAttribute(Qt::WA_DeleteOnClose);
            SYSTEM->change_center_w(this, widget_previous);
        }else{
            QMessageBox::critical(this, "Error", "No se pudieron guardar los datos.");
        }
        return;
    }break;
    case 1:{

    }
    }
}

void CompraOrden::on_pushButton_eliminar_clicked()
{
    if(id.compare("") == 0) {
        QMessageBox::warning(this, "Advertencia", "No se puede eliminar aún.", "Ok");
        return;
    }

    int ret = QMessageBox::warning(this, "Advertencia", "Esta apunto de eliminar los datos. ¿Desea de todas formas eliminar los datos?", "Si", "No");
    switch(ret){
    case 0:{
        if(remove()){
            QMessageBox::information(this, "Información", "Se eliminaron los datos con éxito.");
            id = "";
            setAttribute(Qt::WA_DeleteOnClose);
            SYSTEM->change_center_w(this, widget_previous);
        }else{
            QMessageBox::critical(this, "Error", "No se pudieron eliminar los datos.");
        }
        return;
    }break;
    case 1:{

    }
    }
}

void CompraOrden::on_pushButton_salir_clicked()
{
    int ret = QMessageBox::warning(this, "Advertencia", "¿Esta seguro de salir del formulario?", "Si", "No");
    switch(ret){
    case 0:{
        if(widget_previous){
            id = "";
            setAttribute(Qt::WA_DeleteOnClose);
            SYSTEM->change_center_w(this, widget_previous);
        }else{
            SYSTEM->clear_center_w(this);
        }
    }break;
    case 1:{

    }break;
    }
}

void CompraOrden::on_dateTimeEdit_emision_dateChanged(const QDate &date)
{
    int index = ui->comboBox_moneda->currentIndex();
    // DOLARES
    if(index == 1){
        ui->label_cambio_dolar_value->hide();
        double cambio_dolar = SYSTEM->get_dolar(date);
        ui->label_cambio_dolar_value->setText(QString().setNum(cambio_dolar, ' ', 3));
        if(cambio_dolar == 0.0){
            QMessageBox::warning(0, "Advertencia", "No hay tipo de cambio en ese día.", "OK");
        }
        QTimer *timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(set_time_dolar_value()));
        timer->start(1000);
    }
}
void CompraOrden::showEvent(QShowEvent *se)
{
    emit showing();
    se->accept();

    if(focusWidget()){
        focusWidget()->setFocus();
    }else{
        ui->dateTimeEdit_emision->setFocus(Qt::TabFocusReason);
    }
}
void CompraOrden::closeEvent(QCloseEvent *ce)
{
    emit closing();
    ce->accept();
}

bool CompraOrden::eventFilter(QObject *obj, QEvent *e)
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
    w_temp = ui->dateTimeEdit_emision;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;
            switch(KeyEvent->key())
            {
            case Qt::Key_Return:{
                ui->lineEdit_serie->setFocus(Qt::TabFocusReason);
                return true;
            }break;
            }

        }
        return false;
    }
    w_temp = ui->lineEdit_serie;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;
            switch(KeyEvent->key())
            {
            case Qt::Key_Return:{
                ui->lineEdit_numero->setFocus(Qt::TabFocusReason);
                return true;
            }break;
            }
        }
        if(e->type() == QEvent::FocusOut){
            int nro_digits = A_Compra::SERIE_N_DIGITS;
            QString str = ui->lineEdit_serie->text();
            int count = str.length();

            for(int i = 0; i < nro_digits-count; i++){
                str.push_front('0');
            }
            ui->lineEdit_serie->setText(str);
        }
        return false;
    }

    w_temp = ui->lineEdit_numero;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;
            switch(KeyEvent->key())
            {
            case Qt::Key_Return:{
                ui->pushButton_proveedor->setFocus(Qt::TabFocusReason);
                return true;
            }break;
            }
        }
        if(e->type() == QEvent::FocusOut){
            int nro_digits = A_Compra::NUMERO_N_DIGITS;
            QString str = ui->lineEdit_numero->text();
            int count = str.length();
            for(int i = 0; i < nro_digits-count; i++){
                str.push_front('0');
            }
            ui->lineEdit_numero->setText(str);
        }
        return false;
    }

    w_temp = ui->pushButton_proveedor;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;
            switch(KeyEvent->key())
            {
            case Qt::Key_Return:{
                ui->pushButton_proveedor->click();
                return true;
            }break;
            }
        }
        return false;
    }

    w_temp = ui->comboBox_modalidad;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;
            switch(KeyEvent->key())
            {
            case Qt::Key_Return:{
                ui->comboBox_moneda->setFocus(Qt::TabFocusReason);
                return true;
            }break;
            }
        }
        return false;
    }

    w_temp = ui->comboBox_moneda;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;
            switch(KeyEvent->key())
            {
            case Qt::Key_Return:{
                ui->pushButton_ing_prod->setFocus(Qt::TabFocusReason);
                return true;
            }break;
            }
        }
        return false;
    }
    w_temp = ui->pushButton_ing_prod;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;
            switch(KeyEvent->key())
            {
            case Qt::Key_Return:{
                ui->pushButton_ing_prod->click();
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
            case Qt::Key_Return:
                if(QString(typeid(*QApplication::focusWidget()).name()) == "18QExpandingLineEdit"){
                    QApplication::focusWidget()->parentWidget()->setFocus();
                }else{
                    ui->pushButton_guardar->setFocus(Qt::TabFocusReason);
                    return true;
                }
            break;
            case Qt::Key_F3:{
                QTableWidgetItem* item = ui->tableWidget->currentItem();
                if(item) {
                    CompraChartCosto* w = new CompraChartCosto();                    
                    QString producto_id = ui->tableWidget->item(item->row(), INDEX_ID)->text();
                    QString unidad = ui->tableWidget->item(item->row(), INDEX_UNIDAD)->text();
                    QString descripcion = ui->tableWidget->item(item->row(), INDEX_DESCRIPCION)->text();
                    w->set_producto(producto_id, unidad, descripcion);
                    w->set_widget_previous(this);
                    SYSTEM->change_center_w(this, w);
                    return true;
                }
            }break;
            }
        }
        return false;
    }
    w_temp = ui->pushButton_amarres;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;
            switch(KeyEvent->key())
            {
            case Qt::Key_Return:{
                ui->pushButton_amarres->click();
                return true;
            }break;
            }
        }
        return false;
    }
    w_temp = ui->pushButton_guardar;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;
            switch(KeyEvent->key())
            {
            case Qt::Key_Return:{
                ui->pushButton_guardar->click();
                return true;
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
            }
        }
        return false;
    }
    return eventFilter(obj, e);
}

void CompraOrden::on_lineEdit_cod_textEdited(const QString &arg1)
{
    if(arg1.length() == 11){
        QString str_query;
        QSqlQuery query;

        str_query = "SELECT juridica.persona_id, juridica.ruc";
        str_query += ", juridica.razon_social";
        str_query += " FROM proveedor";
        str_query += " JOIN juridica ON juridica.persona_id = proveedor.juridica_persona_id";
        //str_query += " JOIN persona ON persona.id = proveedor.juridica_persona_id";
        str_query += " WHERE juridica.ruc = '"+arg1+"'";

        qDebug()<<str_query<<endl;
        if(query.exec(str_query)) {
            query.next();
            persona_id = query.value(0).toString();
            codigo = query.value(1).toString();
            nombre = query.value(2).toString();
            ui->lineEdit_cod->setText(codigo);
            ui->lineEdit_nombre->setText(nombre);

        }else{
        }
    }
}

void CompraOrden::on_pushButton_down_clicked()
{

}

void CompraOrden::on_pushButton_up_clicked()
{

}

void CompraOrden::on_pushButton_borrar_clicked()
{
    QTableWidgetItem* item= ui->tableWidget->currentItem();
    if(!item)return;
    int row = item->row();
    int column = item->column();
    ui->tableWidget->removeRow(row);
    int rowCount = ui->tableWidget->rowCount();

    if(row >= 0){
        if(row > 0){
            ui->tableWidget->setCurrentItem(ui->tableWidget->item(row-1, column));
            ui->tableWidget->item(row-1, column)->setSelected(true);
            ui->tableWidget->setFocus(Qt::TabFocusReason);
            //ui->tableWidget->selectRow(row-1);
        }
    }else{

    }

    double total = 0.0;
    rowCount = ui->tableWidget->rowCount();

    for(int i = 0; i < rowCount; i++){
        QString s_p_total = ui->tableWidget->item(i, INDEX_P_TOTAL)->text();

        double p_total = s_p_total.toDouble();

        total += p_total;
    }

    SYSTEM->table_resize_to_contents(0, ui->tableWidget);

    // CON IGV
    if(ui->comboBox_modalidad->currentIndex() == 0){
        ui->lineEdit_subtotal->setText(QString().setNum(total/(1.0+igv), ' ', DECIMALS_PRECIO_TOTAL));
        ui->lineEdit_igv->setText(QString().setNum(total-total/(1.0+igv), ' ', DECIMALS_PRECIO_TOTAL));
        ui->lineEdit_total->setText(QString().setNum(total, ' ', DECIMALS_PRECIO_TOTAL));
    }else{
        ui->lineEdit_subtotal->setText(QString().setNum(total, ' ', DECIMALS_PRECIO_TOTAL));
        ui->lineEdit_igv->setText(QString().setNum(total*(1.0+igv)-total, ' ', DECIMALS_PRECIO_TOTAL));
        ui->lineEdit_total->setText(QString().setNum(total*(1.0+igv), ' ', DECIMALS_PRECIO_TOTAL));
    }
}
