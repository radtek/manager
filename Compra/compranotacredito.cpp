#include "compranotacredito.h"
#include "ui_compranotacredito.h"

CompraNotaCredito::CompraNotaCredito(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CompraNotaCredito)
{
    ui->setupUi(this);

    igv = SYSTEM->get_igv();
    dolar = SYSTEM->get_dolar();

    widget_previous = NULL;

    QRegExp regExp_ruc("[0-9]{11,11}");
    ui->lineEdit_codigo->setValidator(new QRegExpValidator(regExp_ruc));

    // DISABLES

    //ui->lineEdit_cod->setReadOnly(true);
    ui->lineEdit_nombre->setReadOnly(true);

    // SET DATA
    disconnect(ui->dateEdit_declaracion, SIGNAL(dateChanged(QDate)), this, SLOT(on_dateEdit_declaracion_dateChanged(QDate)));
    disconnect(ui->dateTimeEdit_emision, SIGNAL(dateChanged(QDate)), this, SLOT(on_dateTimeEdit_emision_dateChanged(QDate)));
    ui->dateEdit_declaracion->setDate(QDate::currentDate());
    mes_declaracion = QDate::currentDate().month();
    ui->dateTimeEdit_emision->setDateTime(QDateTime::currentDateTime());
    mes_emision = QDate::currentDate().month();
    connect(ui->dateEdit_declaracion, SIGNAL(dateChanged(QDate)), this, SLOT(on_dateEdit_declaracion_dateChanged(QDate)));
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
    ui->tableWidget->setItemDelegateForColumn(INDEX_DESC_CANT, dSpinBoxDelegate);
    dSpinBoxDelegate = new DoubleSpinBoxDelegate;
    dSpinBoxDelegate->setMinimum(0);
    dSpinBoxDelegate->setMaximum(999999999);
    dSpinBoxDelegate->setDecimals(DECIMALS_PRECIO_TOTAL);
    dSpinBoxDelegate->setSingleStep(QString("0."+SYSTEM->zeros(DECIMALS_PRECIO_TOTAL-1)+"1").toDouble());
    ui->tableWidget->setItemDelegateForColumn(INDEX_DESC_P_TOTAL, dSpinBoxDelegate);

    // HIDES
    ui->tableWidget->hideColumn(INDEX_ID);

    ui->label_cambio_dolar->hide();
    ui->label_cambio_dolar_value->hide();

    // SET CUSTOMS
    ui->label_igv->setText("IGV "+QString().setNum(igv, ' ', 3));
    ui->label_cambio_dolar_value->setText(QString().setNum(dolar, ' ', 3));

    // INSTALL EVENTS
    this->installEventFilter(this);
    ui->dateEdit_declaracion->installEventFilter(this);
    ui->dateTimeEdit_emision->installEventFilter(this);
    ui->lineEdit_serie->installEventFilter(this);
    ui->lineEdit_numero->installEventFilter(this);
    ui->pushButton_proveedor->installEventFilter(this);
    ui->comboBox_modalidad->installEventFilter(this);
    ui->comboBox_moneda->installEventFilter(this);
    ui->doubleSpinBox_monto->installEventFilter(this);
    ui->pushButton_ing_prod->installEventFilter(this);
    ui->tableWidget->installEventFilter(this);
    ui->pushButton_amarres->installEventFilter(this);
    ui->pushButton_guardar->installEventFilter(this);
    ui->pushButton_eliminar->installEventFilter(this);
    ui->pushButton_salir->installEventFilter(this);

    ui->comboBox_serie_numero_boleta->installEventFilter(this);
    ui->pushButton_buscar_boleta->installEventFilter(this);
    ui->pushButton_jalar_boleta->installEventFilter(this);
    ui->pushButton_quitar_boleta->installEventFilter(this);
    ui->comboBox_serie_numero_factura->installEventFilter(this);
    ui->pushButton_buscar_factura->installEventFilter(this);
    ui->pushButton_jalar_factura->installEventFilter(this);
    ui->pushButton_quitar_factura->installEventFilter(this);
}

CompraNotaCredito::~CompraNotaCredito()
{
    qDebug()<<"delete compra nota credito"<<endl;
    delete ui;
}
void CompraNotaCredito::set_widget_previous(QWidget *w)
{
    this->widget_previous = w;
}

bool CompraNotaCredito::select(QString id
                               , QString persona_id, QString fecha_emision
                               , QString serie, QString numero
                               , QString codigo, QString nombre)
{
    this->id = id;
    this->persona_id = persona_id;
    this->fecha_emision = fecha_emision;
    this->serie = serie;
    this->numero = numero;
    this->codigo = codigo;
    this->nombre = nombre;

    QString str_query;

    str_query = "(SELECT COUNT(*), '', '', '', '', '', '' FROM documento_h_documento";
    str_query += " JOIN factura ON documento_h_documento.documento_id1 = factura.comprobante_documento_id";
    str_query += " WHERE documento_h_documento.documento_id = "+id+")";
    str_query += " UNION ALL";
    str_query += "(SELECT documento_h_documento.documento_id1, anexo.serie, anexo.numero, juridica.persona_id, juridica.ruc, juridica.razon_social, '' FROM documento_h_documento";
    str_query += " JOIN factura ON documento_h_documento.documento_id1 = factura.comprobante_documento_id";
    str_query += " JOIN anexo ON documento_h_documento.documento_id1 = anexo.documento_id";
    str_query += " JOIN documento_h_persona ON documento_h_persona.documento_id = factura.comprobante_documento_id";
    str_query += " JOIN juridica ON juridica.persona_id = documento_h_persona.persona_id";
    str_query += " WHERE documento_h_documento.documento_id = "+id+")";
    str_query += " UNION ALL";
    str_query += "(SELECT COUNT(*), '', '', '', '', '', '' FROM documento_h_documento";
    str_query += " JOIN boleta ON documento_h_documento.documento_id1 = boleta.comprobante_documento_id";
    str_query += " WHERE documento_h_documento.documento_id = "+id+")";
    str_query += " UNION ALL";
    str_query += "(SELECT documento_h_documento.documento_id1, anexo.serie, anexo.numero, juridica.persona_id, juridica.ruc, juridica.razon_social, '' FROM documento_h_documento";
    str_query += " JOIN boleta ON documento_h_documento.documento_id1 = boleta.comprobante_documento_id";
    str_query += " JOIN anexo ON documento_h_documento.documento_id1 = anexo.documento_id";
    str_query += " JOIN documento_h_persona ON documento_h_persona.documento_id = boleta.comprobante_documento_id";
    str_query += " JOIN juridica ON juridica.persona_id = documento_h_persona.persona_id";
    str_query += " WHERE documento_h_documento.documento_id = "+id+")";
    str_query += " UNION ALL";
    str_query += "(SELECT nota_credito.declaracion, nota_credito.modalidad, nota_credito.moneda, nota_credito.monto, '', '', '' FROM nota_credito";
    str_query += " WHERE nota_credito.comprobante_documento_id = "+id+")";
    str_query += " UNION ALL";
    str_query += "(SELECT producto.id, d_h_prod.cantidad, unidad.unidad";
    str_query += ", concat(producto.descripcion,' ',marca.marca), d_h_prod.precio, d_h_prod.cantidad_aux, d_h_prod.precio_aux FROM documento";
    //str_query += " JOIN documento_h_persona d_h_per ON d.id = d_h_per.documento_id";
    //str_query += " JOIN persona ON persona.id = d_h_per.persona_id";
    //str_query += " JOIN juridica ON persona.id = d_h_per.persona_id";
    //str_query += " JOIN proveedor ON persona.id = proveedor.juridica_persona_id";
    str_query += " JOIN documento_h_producto d_h_prod ON documento.id = d_h_prod.documento_id";
    str_query += " JOIN producto ON producto.id = d_h_prod.producto_id";
    str_query += " JOIN marca ON marca.id = producto.marca_id";
    str_query += " JOIN unidad ON unidad.id = producto.unidad_id";
    str_query += " WHERE documento.id = "+id+" GROUP BY d_h_prod.id)";

    qDebug()<<str_query<<endl;
    if(query.exec(str_query)){
        query.next();
        int countFactura = query.value(0).toInt();
        for(int i=0; i<countFactura; i++){
            query.next();
            QString id = query.value(0).toString();
            QString serie = query.value(1).toString();
            QString numero = query.value(2).toString();
            //QString persona_id = query.value(3).toString();
            QString ruc = query.value(4).toString();
            QString razon_social = query.value(5).toString();
            QString sn = ruc + " - " + razon_social+ " - " + serie + " - " + numero;
            ui->comboBox_id_factura->addItem(id);
            ui->comboBox_serie_numero_factura->addItem(sn);
        }

        query.next();
        int countBoleta = query.value(0).toInt();
        for(int i=0; i<countBoleta; i++){
            query.next();
            QString id = query.value(0).toString();
            QString serie = query.value(1).toString();
            QString numero = query.value(2).toString();
            //QString persona_id = query.value(3).toString();
            QString ruc = query.value(4).toString();
            QString razon_social = query.value(5).toString();
            QString sn = ruc + " - " + razon_social+ " - " + serie + " - " + numero;
            ui->comboBox_id_boleta->addItem(id);
            ui->comboBox_serie_numero_boleta->addItem(sn);
        }

        QDateTime dt;
        QDate date = QDate::fromString(fecha_emision, "dd-MMM-yyyy");
        dt.setDate(date);

        ui->dateTimeEdit_emision->setDateTime(dt);

        query.next();
        ui->dateEdit_declaracion->setDate(query.value(0).toDate());
        ui->comboBox_modalidad->setCurrentIndex(query.value(1).toInt());
        ui->comboBox_moneda->setCurrentIndex(query.value(2).toInt());
        ui->doubleSpinBox_monto->setValue(query.value(3).toDouble());

        ui->lineEdit_serie->setText(serie);
        ui->lineEdit_numero->setText(numero);


        // PROVEEDOR
        ui->lineEdit_codigo->setText(codigo);
        ui->lineEdit_nombre->setText(nombre);

        // PRODUCTOS
        while(query.next()){
            QString producto_id = query.value(0).toString();
            QString cantidad = query.value(1).toString();
            QString unidad = query.value(2).toString();
            QString descripcion = query.value(3).toString();
            QString p_total = query.value(4).toString();
            QString desc_cantidad = query.value(5).toString();
            QString desc_p_total = query.value(6).toString();
            set_producto(producto_id, cantidad, unidad, descripcion, p_total, desc_cantidad, desc_p_total);
        }

        return true;
    }else{
        return false;
    }
}

void CompraNotaCredito::set_proveedor(QString persona_id, QString codigo, QString nombre)
{
    /*
    this->persona_id = persona_id;
    this->codigo = codigo;
    this->nombre = nombre;
    */

    ui->lineEdit_codigo->setText(codigo);
    ui->lineEdit_nombre->setText(nombre);
}

void CompraNotaCredito::set_producto(QString producto_id
                                     , QString cantidad, QString unidad
                                     , QString descripcion, QString p_total
                                     , QString desc_cantidad, QString desc_p_total)
{
    disconnect(ui->tableWidget, SIGNAL(itemChanged(QTableWidgetItem*))
               , this, SLOT(on_tableWidget_itemChanged(QTableWidgetItem*)));

    cantidad = QString().setNum(cantidad.toDouble(), ' ', DECIMALS_CANTIDAD);
    p_total = QString().setNum(p_total.toDouble(), ' ', DECIMALS_PRECIO_TOTAL);
    desc_cantidad = QString().setNum(desc_cantidad.toDouble(), ' ', DECIMALS_PRECIO_TOTAL);
    desc_p_total = QString().setNum(desc_p_total.toDouble(), ' ', DECIMALS_PRECIO_TOTAL);

    int row = ui->tableWidget->rowCount();
    ui->tableWidget->setRowCount(row+1);

    ui->tableWidget->setItem(row, INDEX_ID, new QTableWidgetItem(producto_id));
    ui->tableWidget->setItem(row, INDEX_CANTIDAD, new QTableWidgetItem(cantidad));
    ui->tableWidget->setItem(row, INDEX_UNIDAD, new QTableWidgetItem(unidad));
    ui->tableWidget->setItem(row, INDEX_DESCRIPCION, new QTableWidgetItem(descripcion));

    ui->tableWidget->setItem(row, INDEX_P_TOTAL, new QTableWidgetItem(p_total));

    QString p_unitario = QString().setNum(p_total.toDouble() / cantidad.toDouble(), ' ', DECIMALS_PRECIO_UNITARIO);
    ui->tableWidget->setItem(row, INDEX_P_UNIT, new QTableWidgetItem(p_unitario));

    ui->tableWidget->setItem(row, INDEX_DESC_CANT, new QTableWidgetItem(desc_cantidad));
    ui->tableWidget->setItem(row, INDEX_DESC_P_TOTAL, new QTableWidgetItem(desc_p_total));


    ui->tableWidget->item(row, INDEX_ID)->setFlags(Qt::ItemIsEnabled
                                                 | Qt::ItemIsSelectable);

    ui->tableWidget->item(row, INDEX_CANTIDAD)->setFlags(Qt::ItemIsEnabled
                                                 | Qt::ItemIsSelectable);

    ui->tableWidget->item(row, INDEX_UNIDAD)->setFlags(Qt::ItemIsEnabled
                                                 | Qt::ItemIsSelectable);

    ui->tableWidget->item(row, INDEX_DESCRIPCION)->setFlags(Qt::ItemIsEnabled
                                                 | Qt::ItemIsSelectable);

    ui->tableWidget->item(row, INDEX_P_UNIT)->setFlags(Qt::ItemIsEnabled
                                                 | Qt::ItemIsSelectable);

    ui->tableWidget->item(row, INDEX_P_TOTAL)->setFlags(Qt::ItemIsEnabled
                                                 | Qt::ItemIsSelectable);

    ui->tableWidget->item(row, INDEX_DESC_CANT)->setFlags(Qt::ItemIsEnabled
                                                 | Qt::ItemIsSelectable
                                                 | Qt::ItemIsEditable);

    ui->tableWidget->item(row, INDEX_DESC_P_TOTAL)->setFlags(Qt::ItemIsEnabled
                                                 | Qt::ItemIsSelectable
                                                 | Qt::ItemIsEditable);

    double total = 0.0;
    int rowCount = ui->tableWidget->rowCount();

    for(int i = 0; i < rowCount; i++) {
        QString desc_p_total = ui->tableWidget->item(i, INDEX_DESC_P_TOTAL)->text();

        double p_total = desc_p_total.toDouble();

        total += p_total;
    }

    total += ui->doubleSpinBox_monto->value();

    for(int i = 0; i < rowCount; i++) {
        QString desc_cantidad = ui->tableWidget->item(i, INDEX_DESC_CANT)->text();
        QString p_unitario = ui->tableWidget->item(i, INDEX_P_UNIT)->text();

        double p_total = p_unitario.toDouble() * desc_cantidad.toDouble();

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

bool CompraNotaCredito::guardar()
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
        str_query += QString().setNum(tipo_documento::NOTA_CREDITO);
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
        str_query +=  "INSERT INTO documento_h_producto(documento_id, producto_id, cantidad, precio, cantidad_aux, precio_aux)VALUES";
        QString str_query_2;
        for(int i= 0; i< ui->tableWidget->rowCount(); i++){
            QString producto_id = ui->tableWidget->item(i, INDEX_ID)->text();
            QString cantidad = ui->tableWidget->item(i, INDEX_CANTIDAD)->text();
            QString total = ui->tableWidget->item(i, INDEX_P_TOTAL)->text();
            QString desc_cantidad = ui->tableWidget->item(i, INDEX_DESC_CANT)->text();
            QString desc_p_total = ui->tableWidget->item(i, INDEX_DESC_P_TOTAL)->text();

            str_query_2 += QString()+",((SELECT MAX(documento.id) FROM documento), "+producto_id+", "+cantidad+", "+total+", "+desc_cantidad+", "+desc_p_total+")";
        }

        str_query_2.remove(0,1);
        str_query += str_query_2+"&&END_QUERY&&";

        // NOTA DE CREDITO
        str_query += "INSERT INTO nota_credito(comprobante_documento_id";
        str_query += ", declaracion";
        str_query += ", modalidad";
        str_query += ", moneda";
        str_query += ", monto";
        str_query += ")VALUES(";
        str_query += "(SELECT MAX(documento.id) FROM documento)";
        str_query += ", '"+ui->dateEdit_declaracion->date().toString("yyyy-MM-dd")+"'";
        str_query += ", "+QString().setNum(ui->comboBox_modalidad->currentIndex())+"";
        str_query += ", "+QString().setNum(ui->comboBox_moneda->currentIndex())+"";
        str_query += ", "+QString().setNum(ui->doubleSpinBox_monto->value())+")";
        str_query += "&&END_QUERY&&";

        // DOCUMENTO_H_DOCUMENTO BOLETA
        str_query_2 = "";
        for(int i = 0; i < ui->comboBox_serie_numero_boleta->count(); i++){
            QString id = ui->comboBox_id_boleta->itemText(i);

            str_query_2 += ",((SELECT MAX(id) FROM documento)";
            str_query_2 += ", "+id+")";
        }
        if(ui->comboBox_id_boleta->count() > 0){
            str_query += "INSERT INTO documento_h_documento(documento_id, documento_id1)VALUES";
            str_query_2.remove(0,1);
            str_query += str_query_2+"&&END_QUERY&&";
        }

        // DOCUMENTO_H_DOCUMENTO FACTURA
        str_query_2 = "";
        for(int i = 0; i < ui->comboBox_serie_numero_factura->count(); i++){
            QString id = ui->comboBox_id_factura->itemText(i);

            str_query_2 += ",((SELECT MAX(id) FROM documento)";
            str_query_2 += ", "+id+")";
        }
        if(ui->comboBox_id_factura->count() > 0){
            str_query += "INSERT INTO documento_h_documento(documento_id, documento_id1)VALUES";
            str_query_2.remove(0,1);
            str_query += str_query_2+"&&END_QUERY&&";
        }
    }else{
        // COMPROBANTE
        /*
        str_query =  "UPDATE comprobante SET";
        str_query += " WHERE documento_id = "+id;
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
        str_query += "INSERT INTO documento_h_producto(documento_id, producto_id, cantidad, precio, cantidad_aux, precio_aux)VALUES";
        QString str_query_2;
        for(int i= 0; i< ui->tableWidget->rowCount(); i++){
            QString producto_id = ui->tableWidget->item(i, INDEX_ID)->text();
            QString cantidad = ui->tableWidget->item(i, INDEX_CANTIDAD)->text();
            QString total = ui->tableWidget->item(i, INDEX_P_TOTAL)->text();
            QString desc_cantidad = ui->tableWidget->item(i, INDEX_DESC_CANT)->text();
            QString desc_p_total = ui->tableWidget->item(i, INDEX_DESC_P_TOTAL)->text();

            str_query_2 += QString()+",((SELECT MAX(documento.id) FROM documento), "+producto_id+", "+cantidad+", "+total+", "+desc_cantidad+", "+desc_p_total+")";
        }

        str_query_2.remove(0,1);
        str_query += str_query_2;
        /*
        str_query += " ON DUPLICATE KEY UPDATE documento_id=VALUES(documento_id)";
        str_query += ", producto_id=VALUES(producto_id)";
        str_query += ", cantidad=VALUES(cantidad_id)";
        str_query += ", precio=VALUES(precio_id)";*/
        str_query += "&&END_QUERY&&";

        // UPDATE NOTA DE CREDITO
        str_query += "UPDATE nota_credito SET";
        str_query += " declaracion = '"+ui->dateEdit_declaracion->date().toString("yyyy-MM-dd")+"'";
        str_query += ", modalidad = "+QString().setNum(ui->comboBox_modalidad->currentIndex());
        str_query += ", moneda = "+QString().setNum(ui->comboBox_moneda->currentIndex());
        str_query += ", monto = "+QString().setNum(ui->doubleSpinBox_monto->value());
        str_query += " WHERE comprobante_documento_id = "+id;
        str_query += "&&END_QUERY&&";

        // DOCUMENTO_H_DOCUMENTO BOLETA
        str_query += "DELETE FROM documento_h_documento WHERE documento_id = "+id;
        str_query += "&&END_QUERY&&";

        str_query_2 = "";
        for(int i = 0; i < ui->comboBox_serie_numero_boleta->count(); i++){
            QString id = ui->comboBox_id_boleta->itemText(i);

            str_query_2 += ",("+this->id;
            str_query_2 += ", "+id+")";
        }
        if(ui->comboBox_id_boleta->count() > 0){
            str_query += "INSERT INTO documento_h_documento(documento_id, documento_id1)VALUES";
            str_query_2.remove(0,1);
            str_query += str_query_2+"&&END_QUERY&&";
        }

        // DOCUMENTO_H_DOCUMENTO FACTURA
        str_query_2 = "";
        for(int i = 0; i < ui->comboBox_serie_numero_factura->count(); i++){
            QString id = ui->comboBox_id_factura->itemText(i);

            str_query_2 += ",("+this->id;
            str_query_2 += ", "+id+")";
        }
        if(ui->comboBox_id_factura->count() > 0){
            str_query += "INSERT INTO documento_h_documento(documento_id, documento_id1)VALUES";
            str_query_2.remove(0,1);
            str_query += str_query_2+"&&END_QUERY&&";
        }
    }

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
bool CompraNotaCredito::remove()
{
    QString str_query;

    str_query = "DELETE FROM documento WHERE id = "+id;
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

void CompraNotaCredito::set_time()
{
    ui->dateTimeEdit_sistema->setDateTime(QDateTime::currentDateTime());
}
void CompraNotaCredito::set_time_dolar_value()
{
    ui->label_cambio_dolar_value->show();
    delete sender();
}
void CompraNotaCredito::on_proveedor_closing()
{
    ColaboradorBuscar* w = (ColaboradorBuscar*)sender();
    QString persona_id = w->get_ID();
    qDebug() << "persona_id: " << persona_id << endl;
    if(persona_id.compare("") == 0) {
        return;
    }

    this->persona_id = persona_id;
    codigo = w->get_codigo();
    nombre = w->get_nombre();

    set_proveedor(persona_id, codigo, nombre);

    ui->pushButton_ing_prod->setFocus(Qt::TabFocusReason);
}

void CompraNotaCredito::on_pushButton_proveedor_clicked()
{
    CompraProveedor* w_buscar_proveedor = new CompraProveedor;
    w_buscar_proveedor->set_widget_previous(this);
    w_buscar_proveedor->set_tipo(persona_items::PROVEEDOR);
    connect(w_buscar_proveedor, SIGNAL(closing()), this, SLOT(on_proveedor_closing()));

    SYSTEM->change_center_w(this, w_buscar_proveedor);
}

void CompraNotaCredito::on_producto_closing()
{
    ProductoBuscar* w = (ProductoBuscar*)sender();
    QString producto_id = w->getID();
    if(producto_id.compare("") == 0){
        return;
    }
    qDebug()<<"producto_id: "<<producto_id<<endl;
    QString cantidad = "1.0";
    QString unidad = w->getUnidad();
    QString marca = w->getMarca();
    QString descripcion = w->getDescripcion()+" "+marca;
    QString p_total = w->getPrecio();
    QString desc_cantidad = "0.000";
    QString desc_p_total = "0.000";

    set_producto(producto_id, cantidad, unidad, descripcion, p_total, desc_cantidad, desc_p_total);
}

void CompraNotaCredito::on_pushButton_ing_prod_clicked()
{
    CompraProducto* w_buscar_producto = new CompraProducto;
    w_buscar_producto->set_widget_previous(this);
    connect(w_buscar_producto, SIGNAL(closing()), this, SLOT(on_producto_closing()));;

    SYSTEM->change_center_w(this, w_buscar_producto);
}

void CompraNotaCredito::on_pushButton_amarres_clicked()
{
    /*
    if(!w_compra_amarres){
        w_compra_amarres = new CompraAmarres;
        ((CompraAmarres*)w_compra_amarres)->set_widget_previous(this);
        ((CompraAmarres*)w_compra_amarres)->set_documento_id(this->id);
    }
    SYSTEM->change_center_w(this, w_compra_amarres);
    */
}

void CompraNotaCredito::on_pushButton_guardar_clicked()
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

void CompraNotaCredito::on_pushButton_eliminar_clicked()
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
void CompraNotaCredito::on_pushButton_salir_clicked()
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

void CompraNotaCredito::on_tableWidget_itemChanged(QTableWidgetItem *item)
{
    disconnect(ui->tableWidget, SIGNAL(itemChanged(QTableWidgetItem*))
               , this, SLOT(on_tableWidget_itemChanged(QTableWidgetItem*)));

    // P_UNITARIO
    if(item->column() == INDEX_DESC_CANT){
        /*
        QString s_cantidad = ui->tableWidget->item(item->row(), INDEX_CANTIDAD)->text();
        //QString s_regalo = ui->tableWidget->item(item->row(), INDEX_REGALO)->text();
        QString s_p_unitario = ui->tableWidget->item(item->row(), INDEX_P_U_FLETE)->text();

        double cantidad = s_cantidad.toDouble();
        double p_unitario = s_p_unitario.toDouble();
        double p_total;

        p_total = cantidad*p_unitario;

        ui->tableWidget->item(item->row(), INDEX_P_T_FLETE)->setText(QString().setNum(p_total, ' ', DECIMALS_PRECIO_TOTAL));
        */
    }
    // P_TOTAL
    if(item->column() == INDEX_DESC_P_TOTAL){
        /*
        QString s_cantidad = ui->tableWidget->item(item->row(), INDEX_CANTIDAD)->text();
        //QString s_regalo = ui->tableWidget->item(item->row(), INDEX_REGALO)->text();
        QString s_p_total = ui->tableWidget->item(item->row(), INDEX_P_TOTAL)->text();

        double cantidad = s_cantidad.toDouble();
        double p_total = s_p_total.toDouble();

        double p_unitario = p_total/cantidad;

        ui->tableWidget->item(item->row(), INDEX_P_U_FLETE)->setText(QString().setNum(p_unitario, ' ', DECIMALS_PRECIO_UNITARIO));
        */
    }

    double total = 0.0;
    int rowCount = ui->tableWidget->rowCount();

    for(int i = 0; i < rowCount; i++) {
        QString desc_p_total = ui->tableWidget->item(i, INDEX_DESC_P_TOTAL)->text();

        double p_total = desc_p_total.toDouble();

        total += p_total;
    }

    total += ui->doubleSpinBox_monto->value();

    for(int i = 0; i < rowCount; i++) {
        QString desc_cantidad = ui->tableWidget->item(i, INDEX_DESC_CANT)->text();
        QString p_unitario = ui->tableWidget->item(i, INDEX_P_UNIT)->text();

        double p_total = p_unitario.toDouble() * desc_cantidad.toDouble();

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

void CompraNotaCredito::on_dateEdit_declaracion_dateChanged(const QDate &date)
{
    if(date.month() > ui->dateTimeEdit_emision->date().month()){
        QMessageBox::warning(this, "Advertencia", "El mes de declaración no puede ser mayor\nque la fecha de emisión.", "Ok");

        disconnect(ui->dateEdit_declaracion, SIGNAL(dateChanged(QDate)), this, SLOT(on_dateEdit_declaracion_dateChanged(QDate)));
        QDate date_temp;
        date_temp.setDate(date.year(), mes_declaracion, date.day());
        ui->dateEdit_declaracion->setDate(date_temp);
        connect(ui->dateEdit_declaracion, SIGNAL(dateChanged(QDate)), this, SLOT(on_dateEdit_declaracion_dateChanged(QDate)));
    }else{
        mes_declaracion = date.month();
    }
}

void CompraNotaCredito::on_dateTimeEdit_emision_dateChanged(const QDate &date)
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


    if(date.month() < ui->dateEdit_declaracion->date().month()){
        QMessageBox::warning(this, "Advertencia", "El mes de declaración no puede ser mayor\nque la fecha de emisión.", "Ok");

        disconnect(ui->dateTimeEdit_emision, SIGNAL(dateChanged(QDate)), this, SLOT(on_dateTimeEdit_emision_dateChanged(QDate)));
        QDate date_temp;
        date_temp.setDate(date.year(), mes_emision, date.day());
        ui->dateTimeEdit_emision->setDate(date_temp);
        connect(ui->dateTimeEdit_emision, SIGNAL(dateChanged(QDate)), this, SLOT(on_dateTimeEdit_emision_dateChanged(QDate)));
    }else{
        mes_emision = date.month();
    }
}

void CompraNotaCredito::showEvent(QShowEvent *se)
{
    emit showing();

    se->accept();

    if(focusWidget()){
        focusWidget()->setFocus();
    }else{
        ui->dateEdit_declaracion->setFocus(Qt::TabFocusReason);
    }
}

void CompraNotaCredito::closeEvent(QCloseEvent *ce)
{
    emit closing();
    ce->accept();
}

bool CompraNotaCredito::eventFilter(QObject *obj, QEvent *e)
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

    w_temp = ui->dateEdit_declaracion;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;
            switch(KeyEvent->key())
            {
            case Qt::Key_Return:{
                ui->dateTimeEdit_emision->setFocus(Qt::TabFocusReason);
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
                ui->pushButton_ing_prod->setFocus(Qt::TabFocusReason);
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

    w_temp = ui->doubleSpinBox_monto;
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
            case Qt::Key_Return:{
                if(QString(typeid(*QApplication::focusWidget()).name()) == "18QExpandingLineEdit"){
                    QApplication::focusWidget()->parentWidget()->setFocus();
                }else{
                    ui->pushButton_guardar->setFocus(Qt::TabFocusReason);
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
                ui->pushButton_guardar->click();
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
    w_temp = ui->comboBox_serie_numero_boleta;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;
            switch(KeyEvent->key())
            {
            case Qt::Key_Return:{
                ui->pushButton_buscar_boleta->setFocus(Qt::TabFocusReason);
                return true;
            }break;
            }
        }
        return false;
    }
    w_temp = ui->pushButton_buscar_boleta;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:{
                ui->pushButton_buscar_boleta->click();
                return true;
            }break;
            }
        }
        return false;
    }
    w_temp = ui->pushButton_jalar_boleta;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:{
                ui->pushButton_jalar_boleta->click();
                return true;
            }break;
            }
        }
        return false;
    }
    w_temp = ui->pushButton_quitar_boleta;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:{
                ui->pushButton_quitar_boleta->click();
                return true;
            }break;
            }
        }
        return false;
    }
    w_temp = ui->comboBox_serie_numero_factura;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;
            switch(KeyEvent->key())
            {
            case Qt::Key_Return:{
                ui->pushButton_buscar_factura->setFocus(Qt::TabFocusReason);
                return true;
            }break;
            }
        }
        return false;
    }
    w_temp = ui->pushButton_buscar_factura;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:{
                ui->pushButton_buscar_factura->click();
                return true;
            }break;
            }
        }
        return false;
    }
    w_temp = ui->pushButton_jalar_factura;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:{
                ui->pushButton_jalar_factura->click();
                return true;
            }break;
            }
        }
        return false;
    }
    w_temp = ui->pushButton_quitar_factura;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:{
                ui->pushButton_quitar_factura->click();
                return true;
            }break;
            }
        }
        return false;
    }

    return eventFilter(obj, e);
}
void CompraNotaCredito::on_factura_buscar_closing()
{
    CompraBuscar* w = (CompraBuscar*)sender();

    if(w->getID().compare("")==0)return;

    QString id = w->getID();
    QString sn = w->getCodigo() + " - " + w->getNombre() + " - " + w->getSerie() + " - " + w->getNumero();

    ui->comboBox_id_factura->addItem(id);
    ui->comboBox_serie_numero_factura->addItem(sn);
}
void CompraNotaCredito::on_boleta_buscar_closing()
{
    CompraBuscar* w = (CompraBuscar*)sender();

    if(w->getID().compare("")==0)return;

    QString id = w->getID();
    QString sn = w->getCodigo() + " - " + w->getNombre() + " - " + w->getSerie() + " - " + w->getNumero();

    ui->comboBox_id_boleta->addItem(id);
    ui->comboBox_serie_numero_boleta->addItem(sn);
}

void CompraNotaCredito::on_pushButton_buscar_factura_clicked()
{
    if(persona_id.compare("") == 0){
        QMessageBox::warning(this, "Advertencia", "No ha ingresado el proveedor.", "Ok");
        return;
    }
    CompraBuscar* w = new CompraBuscar;
    w->set_widget_previous(this);
    w->set_tipo(compra_items::FACTURA);
    connect(w, SIGNAL(closing()), this, SLOT(on_factura_buscar_closing()));
    w->set_ruc(ui->lineEdit_codigo->text());

    SYSTEM->change_center_w(this, w);
}

void CompraNotaCredito::on_pushButton_buscar_boleta_clicked()
{
    if(persona_id.compare("") == 0){
        QMessageBox::warning(this, "Advertencia", "No ha ingresado el proveedor.", "Ok");
        return;
    }
    CompraBuscar* w = new CompraBuscar;
    w->set_widget_previous(this);
    w->set_tipo(compra_items::BOLETA);
    connect(w, SIGNAL(closing()), this, SLOT(on_boleta_buscar_closing()));
    w->set_ruc(ui->lineEdit_codigo->text());

    SYSTEM->change_center_w(this, w);
}

void CompraNotaCredito::on_pushButton_jalar_factura_clicked()
{
    if(ui->comboBox_serie_numero_factura->currentText().compare("") == 0){
        return;
    }else{
    }
    ui->comboBox_id_factura->setCurrentIndex(ui->comboBox_serie_numero_factura->currentIndex());
    QString sn = ui->comboBox_serie_numero_factura->currentText();
    QStringList list = sn.split(QRegExp("-"), QString::SkipEmptyParts);
    QString id = ui->comboBox_id_factura->currentText();
    QString codigo = ((QString)list[0]).trimmed();
    QString nombre = ((QString)list[1]).trimmed();
    QString serie = ((QString)list[2]).trimmed();
    QString numero = ((QString)list[3]).trimmed();

    qDebug() << list << endl;
    if (serie.length() == A_Compra::SERIE_N_DIGITS && numero.length() == A_Compra::NUMERO_N_DIGITS) {
        QString str_query = "";
        str_query += "SELECT producto.id, documento_h_producto.cantidad, unidad.unidad";
        str_query += ", concat(producto.descripcion, ' ', marca.marca), documento_h_producto.precio";
        str_query += " FROM factura";
        str_query += " JOIN documento ON documento.id = factura.comprobante_documento_id";
        str_query += " JOIN comprobante ON (comprobante.operacion_id = "+QString().setNum(operacion_items::COMPRA)+" AND documento.id = comprobante.documento_id)";
        str_query += " JOIN anexo ON documento.id = anexo.documento_id";
        str_query += " JOIN documento_h_producto ON documento.id = documento_h_producto.documento_id";
        str_query += " JOIN producto ON producto.id = documento_h_producto.producto_id";
        str_query += " LEFT JOIN tipo ON tipo.id = producto.tipo_id";
        str_query += " LEFT JOIN marca ON marca.id = producto.marca_id";
        str_query += " LEFT JOIN unidad ON unidad.id = producto.unidad_id";
        str_query += " WHERE documento.id = "+id;
        str_query += " GROUP BY documento_h_producto.id";
        str_query += " ORDER BY factura.comprobante_documento_id DESC, anexo.fecha_emision DESC";

        QSqlQuery query;
        qDebug()<<str_query<<endl;
        if(query.exec(str_query)){
            while(query.next()){
                QString producto_id = query.value(0).toString();
                QString cantidad = query.value(1).toString();
                QString unidad = query.value(2).toString();
                QString descripcion = query.value(3).toString();
                QString p_total = query.value(4).toString();
                QString desc_cant = "0.000";
                QString desc_p_total = "0.000";

                set_producto(producto_id, cantidad, unidad, descripcion, p_total, desc_cant, desc_p_total);
            }
        }else{

        }
    }
}

void CompraNotaCredito::on_pushButton_jalar_boleta_clicked()
{
    if(ui->comboBox_serie_numero_boleta->currentText().compare("") == 0){
        return;
    }else{
    }
    ui->comboBox_id_boleta->setCurrentIndex(ui->comboBox_serie_numero_boleta->currentIndex());
    QString sn = ui->comboBox_serie_numero_boleta->currentText();
    QStringList list = sn.split(QRegExp("-"), QString::SkipEmptyParts);
    QString id = ui->comboBox_id_boleta->currentText();
    QString codigo = ((QString)list[0]).trimmed();
    QString nombre = ((QString)list[1]).trimmed();
    QString serie = ((QString)list[2]).trimmed();
    QString numero = ((QString)list[3]).trimmed();

    if (serie.length() == A_Compra::SERIE_N_DIGITS && numero.length() == A_Compra::NUMERO_N_DIGITS) {
        QString str_query = "";
        str_query += "SELECT producto.id, documento_h_producto.cantidad, unidad.unidad";
        str_query += ", concat(producto.descripcion, ' ', marca.marca), documento_h_producto.precio";
        str_query += " FROM boleta";
        str_query += " JOIN documento ON documento.id = boleta.comprobante_documento_id";
        str_query += " JOIN comprobante ON (comprobante.operacion_id = "+QString().setNum(operacion_items::COMPRA)+" AND documento.id = comprobante.documento_id)";
        str_query += " JOIN anexo ON documento.id = anexo.documento_id";
        str_query += " JOIN documento_h_producto ON documento.id = documento_h_producto.documento_id";
        str_query += " JOIN producto ON producto.id = documento_h_producto.producto_id";
        str_query += " LEFT JOIN tipo ON tipo.id = producto.tipo_id";
        str_query += " LEFT JOIN marca ON marca.id = producto.marca_id";
        str_query += " LEFT JOIN unidad ON unidad.id = producto.unidad_id";
        str_query += " WHERE documento.id = "+id;
        str_query += " GROUP BY documento_h_producto.id";
        str_query += " ORDER BY boleta.comprobante_documento_id DESC, anexo.fecha_emision DESC";

        QSqlQuery query;
        qDebug()<<str_query<<endl;
        if(query.exec(str_query)){
            while(query.next()){
                QString producto_id = query.value(0).toString();
                QString cantidad = query.value(1).toString();
                QString unidad = query.value(2).toString();
                QString descripcion = query.value(3).toString();
                QString p_total = query.value(4).toString();
                QString desc_cant = "0.000";
                QString desc_p_total = "0.000";

                set_producto(producto_id, cantidad, unidad, descripcion, p_total, desc_cant, desc_p_total);
            }
        }else{

        }
    }
}

void CompraNotaCredito::on_pushButton_quitar_factura_clicked()
{
    if(ui->comboBox_serie_numero_factura->currentIndex() == -1)
        return;
    int ret = QMessageBox::warning(this, "Advertencia", "¿Esta seguro de quitar el documento?", "Si", "No");
    switch(ret){
    case 0:{
        ui->comboBox_id_factura->removeItem(ui->comboBox_serie_numero_factura->currentIndex());
        ui->comboBox_serie_numero_factura->removeItem(ui->comboBox_serie_numero_factura->currentIndex());
    }break;
    case 1:{

    }break;
    }
}

void CompraNotaCredito::on_pushButton_quitar_boleta_clicked()
{
    if(ui->comboBox_serie_numero_boleta->currentIndex() == -1)
        return;
    int ret = QMessageBox::warning(this, "Advertencia", "¿Esta seguro de quitar el documento?", "Si", "No");
    switch(ret){
    case 0:{
        ui->comboBox_id_boleta->removeItem(ui->comboBox_serie_numero_boleta->currentIndex());
        ui->comboBox_serie_numero_boleta->removeItem(ui->comboBox_serie_numero_boleta->currentIndex());
    }break;
    case 1:{

    }break;
    }
}

void CompraNotaCredito::on_pushButton_up_clicked()
{
    QTableWidgetItem* item= ui->tableWidget->currentItem();
    if(item){
        int column = item->column();
        int row_source= ui->tableWidget->currentRow();
        int row_destiny= ui->tableWidget->currentRow()-1;

        if(row_source == 0){
            ui->tableWidget->setCurrentItem(ui->tableWidget->item(row_source, column));
            ui->tableWidget->item(row_source, column)->setSelected(true);
            ui->tableWidget->setFocus(Qt::TabFocusReason);
            return;
        }

        disconnect(ui->tableWidget, SIGNAL(itemChanged(QTableWidgetItem*))
                   , this, SLOT(on_tableWidget_itemChanged(QTableWidgetItem*)));

        for(int i=0; i<ui->tableWidget->columnCount(); i++){
            QTableWidgetItem* item_source= ui->tableWidget->takeItem(row_source, i);
            QTableWidgetItem* item_destiny= ui->tableWidget->takeItem(row_destiny, i);
            ui->tableWidget->setItem(row_source, i, item_destiny);
            ui->tableWidget->setItem(row_destiny, i, item_source);
        }
        ui->tableWidget->setCurrentItem(ui->tableWidget->item(row_destiny, column));
        ui->tableWidget->item(row_destiny, column)->setSelected(true);
        ui->tableWidget->setFocus(Qt::TabFocusReason);
        //ui->tableWidget->selectRow(row_destiny);
        connect(ui->tableWidget, SIGNAL(itemChanged(QTableWidgetItem*))
                , this, SLOT(on_tableWidget_itemChanged(QTableWidgetItem*)));
    }
}

void CompraNotaCredito::on_pushButton_down_clicked()
{
    QTableWidgetItem* item= ui->tableWidget->currentItem();
    if(item){
        int column = item->column();
        int row_source= ui->tableWidget->currentRow();
        int row_destiny= ui->tableWidget->currentRow()+1;

        if(row_source == ui->tableWidget->rowCount()-1){
            ui->tableWidget->setCurrentItem(ui->tableWidget->item(row_source, column));
            ui->tableWidget->item(row_source, column)->setSelected(true);
            ui->tableWidget->setFocus(Qt::TabFocusReason);
            return;
        }

        disconnect(ui->tableWidget, SIGNAL(itemChanged(QTableWidgetItem*))
                   , this, SLOT(on_tableWidget_itemChanged(QTableWidgetItem*)));

        for(int i=0; i<ui->tableWidget->columnCount(); i++){
            QTableWidgetItem* item_source= ui->tableWidget->takeItem(row_source, i);
            QTableWidgetItem* item_destiny= ui->tableWidget->takeItem(row_destiny, i);
            ui->tableWidget->setItem(row_source, i, item_destiny);
            ui->tableWidget->setItem(row_destiny, i, item_source);
        }
        ui->tableWidget->setCurrentItem(ui->tableWidget->item(row_destiny, column));
        ui->tableWidget->item(row_destiny, column)->setSelected(true);
        ui->tableWidget->setFocus(Qt::TabFocusReason);
        //ui->tableWidget->selectRow(row_destiny);
        connect(ui->tableWidget, SIGNAL(itemChanged(QTableWidgetItem*))
                , this, SLOT(on_tableWidget_itemChanged(QTableWidgetItem*)));
    }
}

void CompraNotaCredito::on_pushButton_borrar_clicked()
{
    QTableWidgetItem* item= ui->tableWidget->currentItem();
    if(!item)return;
    int row = item->row();
    int column = item->column();
    ui->tableWidget->removeRow(row);

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
    int rowCount = ui->tableWidget->rowCount();

    for(int i = 0; i < rowCount; i++) {
        QString desc_p_total = ui->tableWidget->item(i, INDEX_DESC_P_TOTAL)->text();

        double p_total = desc_p_total.toDouble();

        total += p_total;
    }

    total += ui->doubleSpinBox_monto->value();

    for(int i = 0; i < rowCount; i++) {
        QString desc_cantidad = ui->tableWidget->item(i, INDEX_DESC_CANT)->text();
        QString p_unitario = ui->tableWidget->item(i, INDEX_P_UNIT)->text();

        double p_total = p_unitario.toDouble() * desc_cantidad.toDouble();

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

void CompraNotaCredito::on_doubleSpinBox_monto_valueChanged(double arg1)
{
    double total = 0.0;
    int rowCount = ui->tableWidget->rowCount();

    for(int i = 0; i < rowCount; i++) {
        QString desc_p_total = ui->tableWidget->item(i, INDEX_DESC_P_TOTAL)->text();

        double p_total = desc_p_total.toDouble();

        total += p_total;
    }

    total += arg1;

    for(int i = 0; i < rowCount; i++) {
        QString desc_cantidad = ui->tableWidget->item(i, INDEX_DESC_CANT)->text();
        QString p_unitario = ui->tableWidget->item(i, INDEX_P_UNIT)->text();

        double p_total = p_unitario.toDouble() * desc_cantidad.toDouble();

        total += p_total;
    }

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

void CompraNotaCredito::on_comboBox_modalidad_currentIndexChanged(int index)
{
    disconnect(ui->tableWidget, SIGNAL(itemChanged(QTableWidgetItem*))
               , this, SLOT(on_tableWidget_itemChanged(QTableWidgetItem*)));

    double total = 0.0;
    int rowCount = ui->tableWidget->rowCount();

    for(int i = 0; i < rowCount; i++) {
        QString desc_p_total = ui->tableWidget->item(i, INDEX_DESC_P_TOTAL)->text();

        double p_total = desc_p_total.toDouble();

        total += p_total;
    }

    total += ui->doubleSpinBox_monto->value();

    for(int i = 0; i < rowCount; i++) {
        QString desc_cantidad = ui->tableWidget->item(i, INDEX_DESC_CANT)->text();
        QString p_unitario = ui->tableWidget->item(i, INDEX_P_UNIT)->text();

        double p_total = p_unitario.toDouble() * desc_cantidad.toDouble();

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
void CompraNotaCredito::on_comboBox_moneda_currentIndexChanged(int index)
{
    disconnect(ui->tableWidget, SIGNAL(itemChanged(QTableWidgetItem*))
               , this, SLOT(on_tableWidget_itemChanged(QTableWidgetItem*)));

    // SOLES
    if(index == 0){
        ui->label_cambio_dolar->hide();
        ui->label_cambio_dolar_value->hide();

        ui->tableWidget->horizontalHeaderItem(INDEX_P_UNIT)->setText("P. Unit. S/.");
        ui->tableWidget->horizontalHeaderItem(INDEX_P_TOTAL)->setText("P. Total S/.");
        ui->tableWidget->horizontalHeaderItem(INDEX_DESC_CANT)->setText("Desc. Cantidad S/.");
        ui->tableWidget->horizontalHeaderItem(INDEX_DESC_P_TOTAL)->setText("Desc. P. Total S/.");
        ui->label_total->setText("Total S/.");
    }

    // DOLARES
    if(index == 1){
        ui->label_cambio_dolar->show();
        ui->label_cambio_dolar_value->show();

        ui->label_cambio_dolar_value->setText(QString().setNum(SYSTEM->get_dolar(ui->dateTimeEdit_emision->date()), ' ', 3));

        ui->tableWidget->horizontalHeaderItem(INDEX_P_UNIT)->setText("P. Unit. $");
        ui->tableWidget->horizontalHeaderItem(INDEX_P_TOTAL)->setText("P. Total $");
        ui->tableWidget->horizontalHeaderItem(INDEX_DESC_CANT)->setText("Desc. Cantidad $");
        ui->tableWidget->horizontalHeaderItem(INDEX_DESC_P_TOTAL)->setText("Desc. P. Total $");
        ui->label_total->setText("Total $");
    }

    SYSTEM->table_resize_to_contents(this, ui->tableWidget);

    connect(ui->tableWidget, SIGNAL(itemChanged(QTableWidgetItem*))
               , this, SLOT(on_tableWidget_itemChanged(QTableWidgetItem*)));
}