#include "ventaboleta.h"
#include "ui_ventaboleta.h"

VentaBoleta::VentaBoleta(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VentaBoleta)
{    
    ui->setupUi(this);

    afterShow = false;
    op = -1;

    anulado = 0;

    QRegExp regExp_ruc("[0-9]{11,11}");
    ui->lineEdit_codigo->setValidator(new QRegExpValidator(regExp_ruc));

    // DISABLES

    //ui->lineEdit_cod->setReadOnly(true);
    //ui->lineEdit_nombre->setReadOnly(true);

    // SET DATA
    disconnect(ui->dateEdit_declaracion, SIGNAL(dateChanged(QDate)), this, SLOT(on_dateEdit_declaracion_dateChanged(QDate)));
    disconnect(ui->dateEdit_emision, SIGNAL(dateChanged(QDate)), this, SLOT(on_dateEdit_emision_dateChanged(QDate)));
    ui->dateEdit_declaracion->setDate(QDate::currentDate());
    mes_declaracion = QDate::currentDate().month();
    ui->dateEdit_emision->setDate(QDate::currentDate());
    mes_emision = QDate::currentDate().month();
    connect(ui->dateEdit_declaracion, SIGNAL(dateChanged(QDate)), this, SLOT(on_dateEdit_declaracion_dateChanged(QDate)));
    connect(ui->dateEdit_emision, SIGNAL(dateChanged(QDate)), this, SLOT(on_dateEdit_emision_dateChanged(QDate)));
    ui->dateTimeEdit_sistema->setDateTime(QDateTime::currentDateTime());

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(set_time()));
    timer->start(1000);

    // SET SERIE Y NUMERO
    A_Venta::set_serie_numero(ui->lineEdit_serie, ui->lineEdit_numero);

    // SET DELEGATE
    LineEditNumberDelegate* delegate = new LineEditNumberDelegate;
    delegate->setDecimals(DECIMALS_CANTIDAD);
    delegate->setMinimumWidth(100);
    delegate->setMinimumHeight(22);
    delegate->setMaximumWidth(100);
    delegate->setMaximumHeight(22);
    ui->tableWidget->setItemDelegateForColumn(INDEX_CANTIDAD, delegate);
    delegate = new LineEditNumberDelegate;
    delegate->setMinimumWidth(100);
    delegate->setMinimumHeight(22);
    delegate->setMaximumWidth(100);
    delegate->setMaximumHeight(22);
    delegate->setDecimals(DECIMALS_PRECIO_UNITARIO);
    ui->tableWidget->setItemDelegateForColumn(INDEX_P_UNIT, delegate);
    delegate = new LineEditNumberDelegate;
    delegate->setMinimumWidth(100);
    delegate->setMinimumHeight(22);
    delegate->setMaximumWidth(100);
    delegate->setMaximumHeight(22);
    delegate->setDecimals(DECIMALS_PRECIO_TOTAL);
    ui->tableWidget->setItemDelegateForColumn(INDEX_P_TOTAL, delegate);

    ui->tableWidget->hideColumn(INDEX_ID);

    ui->label_title_registroSinDoc->hide();
    ui->label_serie_numero_registroSinDoc->hide();
    ui->comboBox_id_registroSinDoc->hide();
    ui->comboBox_serie_numero_registroSinDoc->hide();
    ui->pushButton_buscar_registroSinDoc->hide();
    ui->pushButton_quitar_registroSinDoc->hide();
    ui->pushButton_jalar_registroSinDoc->hide();

    ui->comboBox_id_cotizacion->hide();
    ui->comboBox_id_notaPedido->hide();
    ui->comboBox_id_guiarr->hide();

    this->installEventFilter(this);
    ui->dateEdit_declaracion->installEventFilter(this);
    ui->dateEdit_emision->installEventFilter(this);
    ui->dateTimeEdit_sistema->installEventFilter(this);
    ui->lineEdit_serie->installEventFilter(this);
    ui->lineEdit_numero->installEventFilter(this);
    ui->lineEdit_codigo->installEventFilter(this);
    ui->pushButton_cliente->installEventFilter(this);
    ui->lineEdit_nombre->installEventFilter(this);
    ui->lineEdit_direccion->installEventFilter(this);

    ui->pushButton_ing_prod->installEventFilter(this);
    ui->pushButton_up->installEventFilter(this);
    ui->pushButton_down->installEventFilter(this);
    ui->pushButton_borrar->installEventFilter(this);
    ui->tableWidget->installEventFilter(this);
    ui->lineEdit_total->installEventFilter(this);
    ui->pushButton_amarres->installEventFilter(this);
    ui->pushButton_eliminar->installEventFilter(this);
    ui->pushButton_anular->installEventFilter(this);
    ui->pushButton_imprimir->installEventFilter(this);
    ui->pushButton_guardar->installEventFilter(this);
    ui->pushButton_salir->installEventFilter(this);

    ui->comboBox_id_registroSinDoc->installEventFilter(this);
    ui->comboBox_serie_numero_registroSinDoc->installEventFilter(this);
    ui->pushButton_buscar_registroSinDoc->installEventFilter(this);
    ui->pushButton_jalar_registroSinDoc->installEventFilter(this);
    ui->pushButton_quitar_registroSinDoc->installEventFilter(this);
    ui->comboBox_id_guiarr->installEventFilter(this);
    ui->comboBox_serie_numero_guiarr->installEventFilter(this);
    ui->pushButton_buscar_guiarr->installEventFilter(this);
    ui->pushButton_jalar_guiarr->installEventFilter(this);
    ui->pushButton_quitar_guiarr->installEventFilter(this);
    ui->comboBox_id_notaPedido->installEventFilter(this);
    ui->comboBox_serie_numero_notaPedido->installEventFilter(this);
    ui->pushButton_buscar_notaPedido->installEventFilter(this);
    ui->pushButton_jalar_notaPedido->installEventFilter(this);
    ui->pushButton_quitar_notaPedido->installEventFilter(this);
    ui->comboBox_id_cotizacion->installEventFilter(this);
    ui->comboBox_serie_numero_cotizacion->installEventFilter(this);
    ui->pushButton_buscar_cotizacion->installEventFilter(this);
    ui->pushButton_jalar_cotizacion->installEventFilter(this);
    ui->pushButton_quitar_cotizacion->installEventFilter(this);
}

VentaBoleta::~VentaBoleta()
{
    qDebug()<<"delete venta boleta";
    delete ui;
}
void VentaBoleta::setVer()
{
    ui->pushButton_amarres->hide();
    ui->pushButton_anular->hide();
    ui->pushButton_imprimir->hide();
    ui->pushButton_eliminar->hide();
    ui->pushButton_guardar->hide();
}
void VentaBoleta::set_widget_previous(QWidget *widget_previous)
{
    this->widget_previous = widget_previous;
}

void VentaBoleta::on_ventaSNSelector_closing()
{
    VentaSNSelector* w = (VentaSNSelector*)sender();

    if(w->getSerie().compare("") == 0)return;

    ui->lineEdit_serie->setText(w->getSerie());
    ui->lineEdit_numero->setText(w->getNumero());
}

void VentaBoleta::next_serie_numero()
{
    VentaSNSelector* w = new VentaSNSelector;
    w->set_widget_previous(this);
    w->set_tipo(venta_items::BOLETA);

    connect(w, SIGNAL(closing()), this, SLOT(on_ventaSNSelector_closing()));

    SYSTEM->change_center_w(this, w);
}

bool VentaBoleta::select(QString id
                          , QString persona_id, QString tipo_persona_id
                          , QString fecha_emision
                          , QString serie, QString numero
                          , QString codigo, QString nombre, QString direccion)
{
    this->id = id;
    this->persona_id = persona_id;
    this->tipo_persona_id = tipo_persona_id;
    this->fecha_emision = fecha_emision;
    this->serie = serie;
    this->numero = numero;
    this->codigo = codigo;
    this->nombre = nombre;
    this->direccion = direccion;

    QString str_query;

    str_query = "(SELECT COUNT(*), '', '', '', '' FROM documento_h_documento";
    str_query += " JOIN registro_sin_documento ON documento_h_documento.documento_id1 = registro_sin_documento.comprobante_documento_id";
    str_query += " WHERE documento_h_documento.documento_id = "+id+")";
    str_query += " UNION ALL";
    str_query += "(SELECT documento_h_documento.documento_id1, anexo.serie, anexo.numero, '', '' FROM documento_h_documento";
    str_query += " JOIN registro_sin_documento ON documento_h_documento.documento_id1 = registro_sin_documento.comprobante_documento_id";
    str_query += " JOIN anexo ON documento_h_documento.documento_id1 = anexo.documento_id";
    str_query += " WHERE documento_h_documento.documento_id = "+id+")";
    str_query += " UNION ALL";
    str_query += "(SELECT COUNT(*), '', '', '', '' FROM documento_h_documento";
    str_query += " JOIN nota_pedido ON documento_h_documento.documento_id1 = nota_pedido.comprobante_documento_id";
    str_query += " WHERE documento_h_documento.documento_id = "+id+")";
    str_query += " UNION ALL";
    str_query += "(SELECT documento_h_documento.documento_id1, anexo.serie, anexo.numero, '', '' FROM documento_h_documento";
    str_query += " JOIN nota_pedido ON documento_h_documento.documento_id1 = nota_pedido.comprobante_documento_id";
    str_query += " JOIN anexo ON documento_h_documento.documento_id1 = anexo.documento_id";
    str_query += " WHERE documento_h_documento.documento_id = "+id+")";
    str_query += " UNION ALL";
    str_query += "(SELECT COUNT(*), '', '', '', '' FROM documento_h_documento";
    str_query += " JOIN cotizacion ON documento_h_documento.documento_id1 = cotizacion.comprobante_documento_id";
    str_query += " WHERE documento_h_documento.documento_id = "+id+")";
    str_query += " UNION ALL";
    str_query += "(SELECT documento_h_documento.documento_id1, anexo.serie, anexo.numero, '', '' FROM documento_h_documento";
    str_query += " JOIN cotizacion ON documento_h_documento.documento_id1 = cotizacion.comprobante_documento_id";
    str_query += " JOIN anexo ON documento_h_documento.documento_id1 = anexo.documento_id";
    str_query += " WHERE documento_h_documento.documento_id = "+id+")";
    str_query += " UNION ALL";
    str_query += "(SELECT COUNT(*), '', '', '', '' FROM documento_h_documento";
    str_query += " JOIN guia_remision_remitente ON documento_h_documento.documento_id1 = guia_remision_remitente.comprobante_documento_id";
    str_query += " WHERE documento_h_documento.documento_id = "+id+")";
    str_query += " UNION ALL";
    str_query += "(SELECT documento_h_documento.documento_id1, anexo.serie, anexo.numero, '', '' FROM documento_h_documento";
    str_query += " JOIN guia_remision_remitente ON documento_h_documento.documento_id1 = guia_remision_remitente.comprobante_documento_id";
    str_query += " JOIN anexo ON documento_h_documento.documento_id1 = anexo.documento_id";
    str_query += " WHERE documento_h_documento.documento_id = "+id+")";
    str_query += " UNION ALL";
    str_query += "(SELECT boleta.declaracion, boleta.anulado, '', '', '' FROM boleta";
    str_query += " WHERE boleta.comprobante_documento_id = "+id+")";
    str_query += " UNION ALL";
    str_query += "(SELECT producto.id, d_h_prod.cantidad, unidad.unidad";
    //str_query += ", concat(producto.descripcion,' ',IF(marca.marca IS NULL, '', concat(' ', marca.marca))), d_h_prod.precio FROM documento";
    str_query += ", producto.descripcion, d_h_prod.precio FROM documento";
    str_query += " JOIN documento_h_producto d_h_prod ON documento.id = d_h_prod.documento_id";
    str_query += " JOIN producto ON producto.id = d_h_prod.producto_id";
    str_query += " LEFT JOIN marca ON marca.id = producto.marca_id";
    str_query += " LEFT JOIN unidad ON unidad.id = producto.unidad_id";
    str_query += " WHERE documento.id = "+id+" GROUP BY d_h_prod.id)";

    qDebug()<<str_query<<endl;
    if(query.exec(str_query)){
        query.next();
        int countRSN = query.value(0).toInt();
        for(int i=0; i<countRSN; i++){
            query.next();
            QString id = query.value(0).toString();
            QString serie = query.value(1).toString();
            QString numero = query.value(2).toString();
            QString sn = serie + " - " + numero;
            ui->comboBox_id_registroSinDoc->addItem(id);
            ui->comboBox_serie_numero_registroSinDoc->addItem(sn);
        }

        query.next();
        int countNP = query.value(0).toInt();
        for(int i=0; i<countNP; i++){
            query.next();
            QString id = query.value(0).toString();
            QString serie = query.value(1).toString();
            QString numero = query.value(2).toString();
            QString sn = serie + " - " + numero;
            ui->comboBox_id_notaPedido->addItem(id);
            ui->comboBox_serie_numero_notaPedido->addItem(sn);
        }
        query.next();
        int countCotizacion = query.value(0).toInt();
        for(int i=0; i<countCotizacion; i++){
            query.next();
            QString id = query.value(0).toString();
            QString serie = query.value(1).toString();
            QString numero = query.value(2).toString();
            QString sn = serie + " - " + numero;
            ui->comboBox_id_cotizacion->addItem(id);
            ui->comboBox_serie_numero_cotizacion->addItem(sn);
        }
        query.next();
        int countGuiarr = query.value(0).toInt();
        for(int i=0; i<countGuiarr; i++){
            query.next();
            QString id = query.value(0).toString();
            QString serie = query.value(1).toString();
            QString numero = query.value(2).toString();
            QString sn = serie + " - " + numero;
            ui->comboBox_id_guiarr->addItem(id);
            ui->comboBox_serie_numero_guiarr->addItem(sn);
        }
        QDate date = QDate::fromString(fecha_emision, "dd-MM-yyyy");

        ui->dateEdit_emision->setDate(date);

        ui->lineEdit_serie->setText(serie);
        ui->lineEdit_numero->setText(numero);


        // PROVEEDOR
        ui->lineEdit_codigo->setText(codigo);
        ui->lineEdit_nombre->setText(nombre);
        ui->lineEdit_direccion->setText(direccion);

        query.next();
        ui->dateEdit_declaracion->setDate(query.value(0).toDate());
        anulado = query.value(1).toInt();
        if(!anulado){
            ui->pushButton_anular->setIcon(QIcon(":/new/Iconos/anular.png"));
        }else{
            ui->pushButton_anular->setIcon(QIcon(":/new/Iconos/anulado.png"));
        }

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
void VentaBoleta::set_cliente(QString persona_id, QString codigo, QString nombre, QString direccion)
{
    /*
    this->persona_id = persona_id;
    this->codigo = codigo;
    this->nombre = nombre;
    */

    ui->lineEdit_codigo->setText(codigo);
    ui->lineEdit_nombre->setText(nombre);
    ui->lineEdit_direccion->setText(direccion);
}

void VentaBoleta::set_producto(QString producto_id
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
    ui->tableWidget->setItem(rowCount, INDEX_P_UNIT, new QTableWidgetItem(p_unitario));
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
                                                 | Qt::ItemIsSelectable);

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

    QString str_total = QString().setNum(total, ' ', DECIMALS_PRECIO_TOTAL);
    SYSTEM->normalDecimal(str_total);
    ui->lineEdit_total->setText(str_total);

    SYSTEM->table_resize_to_contents(0, ui->tableWidget);

    connect(ui->tableWidget, SIGNAL(itemChanged(QTableWidgetItem*))
               , this, SLOT(on_tableWidget_itemChanged(QTableWidgetItem*)));
}

void VentaBoleta::set_data(QString persona_id, QString tipo_persona_id
                          , QDateTime dt_emision, QDateTime dt_sistema, QString serie, QString numero
                          , QString codigo, QString nombre, QString direccion
                          , QVector<QVector<QString>> productos)
{
    if(tipo_persona_id.toInt() == tipo_persona::CLIENTE_DNI){
        this->tipo_persona_id = tipo_persona_id;
        this->persona_id = persona_id;
        ui->lineEdit_codigo->setText(codigo);
        ui->lineEdit_nombre->setText(nombre);
        ui->lineEdit_direccion->setText(direccion);
    }

    for(int i=0; i<productos.size(); i++){
        QString id = productos[i][0];
        QString cantidad = productos[i][1];
        QString unidad = productos[i][2];
        QString descripcion = productos[i][3];
        QString p_total = productos[i][4];

        set_producto(id, cantidad, unidad, descripcion, p_total);
    }
}
void VentaBoleta::set_data(int tipo, QString id, QString persona_id, QString tipo_persona_id
                           , QString fecha_emision, QString fecha_sistema, QString serie, QString numero
                           , QString codigo, QString nombre, QString direccion
                           , QVector<QVector<QString>> productos)
{
    switch(tipo){
    case venta_items::REGISTRO_SIN_DOCUMENTO:{
        ui->comboBox_id_registroSinDoc->addItem(id);
        ui->comboBox_serie_numero_registroSinDoc->addItem(serie+" - "+numero);
    }break;
    case venta_items::NOTA_PEDIDO:{
        ui->comboBox_id_notaPedido->addItem(id);
        ui->comboBox_serie_numero_notaPedido->addItem(serie+" - "+numero);
    }break;
    case venta_items::GUIA_REMISION_REMITENTE:{
        ui->comboBox_id_guiarr->addItem(id);
        ui->comboBox_serie_numero_guiarr->addItem(serie+" - "+numero);
    }break;
    case venta_items::COTIZACION:{
        ui->comboBox_id_cotizacion->addItem(id);
        ui->comboBox_serie_numero_cotizacion->addItem(serie+" - "+numero);
    }break;
    }

    if(tipo_persona_id.toInt() == tipo_persona::CLIENTE_DNI){
        this->tipo_persona_id = tipo_persona_id;
        this->persona_id = persona_id;
        ui->lineEdit_codigo->setText(codigo);
        ui->lineEdit_nombre->setText(nombre);
        ui->lineEdit_direccion->setText(direccion);
    }

    for(int i=0; i<productos.size(); i++){
        QString id = productos[i][0];
        QString cantidad = productos[i][1];
        QString unidad = productos[i][2];
        QString descripcion = productos[i][3];
        QString p_total = productos[i][4];

        set_producto(id, cantidad, unidad, descripcion, p_total);
    }
}

bool VentaBoleta::guardar()
{
    if(ui->dateEdit_declaracion->date().month() > ui->dateEdit_emision->date().month()){
        return false;
    }
    if(ui->tableWidget->rowCount() <= 0)
    {
        return false;
    }

    QString str_query;

    ui->dateEdit_emision->setDisplayFormat("yyyy-MM-dd hh:mm:ss");
    ui->dateTimeEdit_sistema->setDisplayFormat("yyyy-MM-dd hh:mm:ss");

    if (id.compare("") == 0) {
        // DOCUMENTO
        str_query += "INSERT INTO documento(tipo_documento_id, habilitado)VALUES(";
        str_query += QString().setNum(tipo_documento::BOLETA);
        str_query += ", 1)";
        str_query += "&&END_QUERY&&";

        // COMPROBANTE
        str_query += "INSERT INTO comprobante(documento_id";
        str_query += ", operacion_id)VALUES(";
        str_query += "(SELECT MAX(documento.id) FROM documento)";
        str_query += ", "+QString().setNum(operacion_items::VENTA)+")";
        str_query += "&&END_QUERY&&";

        // ANEXO
        str_query += "INSERT INTO anexo(documento_id";
        str_query += ", fecha_emision, fecha_sistema, serie, numero)VALUES(";
        str_query += "(SELECT MAX(documento.id) FROM documento)";
        str_query += ", '"+ui->dateEdit_emision->date().toString("yyyy-MM-dd")+"'";
        str_query += ", '"+ui->dateTimeEdit_sistema->dateTime().toString("yyyy-MM-dd hh:mm:ss")+"'";
        str_query += ", '"+ui->lineEdit_serie->text()+"'";
        str_query += ", '"+ui->lineEdit_numero->text()+"')";
        str_query += "&&END_QUERY&&";

        // DOCUMENTO_H_DOCUMENTO

        // DOCUMENTO_H_PERSONA
        if(persona_id.compare("") == 0){
            str_query += "INSERT INTO documento_h_persona(documento_id, persona_id)VALUES(";
            str_query += "(SELECT MAX(documento.id) FROM documento), "+persona_id+")";
            str_query += "&&END_QUERY&&";
        }

        // DOCUMENTO_H_PRODUCTO
        str_query += "INSERT INTO documento_h_producto(documento_id, producto_id, cantidad, precio)VALUES";
        QString str_query_2;
        for(int i= 0; i< ui->tableWidget->rowCount(); i++){
            QString producto_id = ui->tableWidget->item(i, INDEX_ID)->text();
            QString cantidad = ui->tableWidget->item(i, INDEX_CANTIDAD)->text();
            QString total = ui->tableWidget->item(i, INDEX_P_TOTAL)->text();

            str_query_2 += QString()+",((SELECT MAX(documento.id) FROM documento), "+producto_id+", "+cantidad+", "+total+")";
        }

        str_query_2.remove(0,1);
        str_query += str_query_2+"&&END_QUERY&&";

        // BOLETA
        str_query += "INSERT INTO boleta(comprobante_documento_id";
        str_query += ", nombre";
        str_query += ", declaracion";
        str_query += ", anulado";
        str_query += ")VALUES(";
        str_query += "(SELECT MAX(documento.id) FROM documento)";
        str_query += ", '"+ui->lineEdit_nombre->text()+"'";
        str_query += ", '"+ui->dateEdit_declaracion->date().toString("yyyy-MM-dd")+"'";
        str_query += ", '"+QString().setNum(anulado)+"')";
        str_query += "&&END_QUERY&&";

        // DOCUMENTO_H_DOCUMENTO REGISTRO SIN DOCUMENTO

        str_query_2 = "";
        for(int i = 0; i < ui->comboBox_id_registroSinDoc->count(); i++){
            QString id = ui->comboBox_id_registroSinDoc->itemText(i);

            str_query_2 += ",((SELECT MAX(documento.id) FROM documento)";
            str_query_2 += ", " + id + ")";
        }
        if(ui->comboBox_id_registroSinDoc->count() > 0){
            str_query += "INSERT INTO documento_h_documento(documento_id, documento_id1)VALUES";
            str_query_2.remove(0,1);
            str_query += str_query_2+"&&END_QUERY&&";
        }

        // DOCUMENTO_H_DOCUMENTO NOTA PEDIDO
        str_query_2 = "";
        for(int i = 0; i < ui->comboBox_id_notaPedido->count(); i++){
            QString id = ui->comboBox_id_notaPedido->itemText(i);

            str_query_2 += ",((SELECT MAX(documento.id) FROM documento)";
            str_query_2 += ", " + id + ")";
        }
        if(ui->comboBox_id_notaPedido->count() > 0){
            str_query += "INSERT INTO documento_h_documento(documento_id, documento_id1)VALUES";
            str_query_2.remove(0,1);
            str_query += str_query_2+"&&END_QUERY&&";
        }

        // DOCUMENTO_H_DOCUMENTO COTIZACIONES
        str_query_2 = "";
        for(int i = 0; i < ui->comboBox_id_cotizacion->count(); i++){
            QString id = ui->comboBox_id_cotizacion->itemText(i);

            str_query_2 += ",((SELECT MAX(documento.id) FROM documento)";
            str_query_2 += ", " + id + ")";
        }
        if(ui->comboBox_id_cotizacion->count() > 0){
            str_query += "INSERT INTO documento_h_documento(documento_id, documento_id1)VALUES";
            str_query_2.remove(0,1);
            str_query += str_query_2+"&&END_QUERY&&";
        }
        // DOCUMENTO_H_DOCUMENTO GUIA DE REMISION REMITENTE
        str_query_2 = "";
        for(int i = 0; i < ui->comboBox_id_guiarr->count(); i++){
            QString id = ui->comboBox_id_guiarr->itemText(i);

            str_query_2 += ",((SELECT MAX(documento.id) FROM documento)";
            str_query_2 += ", " + id + ")";
        }
        if(ui->comboBox_id_guiarr->count() > 0){
            str_query += "INSERT INTO documento_h_documento(documento_id, documento_id1)VALUES";
            str_query_2.remove(0,1);
            str_query += str_query_2+"&&END_QUERY&&";
        }
        str_query += "SELECT MAX(id) FROM documento";
        str_query += "&&END_QUERY&&";
    }else{
        // COMPROBANTE
        /*
        str_query =  "UPDATE comprobante SET";
        str_query += " WHERE documento_id = "+id;
        str_query += "&&END_QUERY&&";
        */

        // ANEXO
        str_query += "UPDATE anexo SET";
        str_query += " fecha_emision = '"+ui->dateEdit_emision->date().toString("yyyy-MM-dd")+"'";
        str_query += ", fecha_sistema = '"+ui->dateTimeEdit_sistema->dateTime().toString("yyyy-MM-dd hh:mm:ss")+"'";
        str_query += ", serie = '"+ui->lineEdit_serie->text()+"'";
        str_query += ", numero = '"+ui->lineEdit_numero->text()+"'";
        str_query += " WHERE documento_id = "+id;
        str_query += "&&END_QUERY&&";

        // DOCUMENTO_H_DOCUMENTO

        // DOCUMENTO_H_PERSONA
        if(persona_id.compare("") == 0){
            str_query += "DELETE FROM documento_h_persona WHERE documento_id = "+id;
            str_query += "&&END_QUERY&&";
            str_query += "INSERT INTO documento_h_persona(documento_id, persona_id)VALUES(";
            str_query += "(SELECT MAX(documento.id) FROM documento), "+persona_id+")";
            str_query += "&&END_QUERY&&";
        }

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

        str_query += "UPDATE boleta SET";
        str_query += " nombre = '"+ui->lineEdit_nombre->text()+"'";
        str_query += ", declaracion = '"+ui->dateEdit_declaracion->date().toString("yyyy-MM-dd")+"'";
        str_query += ", anulado = '"+QString().setNum(anulado)+"'";
        str_query += " WHERE comprobante_documento_id = "+id;
        str_query += "&&END_QUERY&&";

        // DOCUMENTO_H_DOCUMENTO REGISTRO SIN DOCUMENTO
        str_query += "DELETE FROM documento_h_documento WHERE documento_id = "+id;
        str_query += "&&END_QUERY&&";

        str_query_2 = "";
        for(int i = 0; i < ui->comboBox_id_registroSinDoc->count(); i++){
            QString id = ui->comboBox_id_registroSinDoc->itemText(i);

            str_query_2 += ",("+this->id;
            str_query_2 += ", " + id + ")";
        }
        if(ui->comboBox_id_registroSinDoc->count() > 0){
            str_query += "INSERT INTO documento_h_documento(documento_id, documento_id1)VALUES";
            str_query_2.remove(0,1);
            str_query += str_query_2+"&&END_QUERY&&";
        }

        // DOCUMENTO_H_DOCUMENTO NOTA PEDIDO
        str_query_2 = "";
        for(int i = 0; i < ui->comboBox_id_notaPedido->count(); i++){
            QString id = ui->comboBox_id_notaPedido->itemText(i);

            str_query_2 += ",("+this->id;
            str_query_2 += ", " + id + ")";
        }
        if(ui->comboBox_id_notaPedido->count() > 0){
            str_query += "INSERT INTO documento_h_documento(documento_id, documento_id1)VALUES";
            str_query_2.remove(0,1);
            str_query += str_query_2+"&&END_QUERY&&";
        }

        // DOCUMENTO_H_DOCUMENTO COTIZACIONES
        str_query_2 = "";
        for(int i = 0; i < ui->comboBox_id_cotizacion->count(); i++){
            QString id = ui->comboBox_id_cotizacion->itemText(i);

            str_query_2 += ",("+this->id;
            str_query_2 += ", " + id + ")";
        }
        if(ui->comboBox_id_cotizacion->count() > 0){
            str_query += "INSERT INTO documento_h_documento(documento_id, documento_id1)VALUES";
            str_query_2.remove(0,1);
            str_query += str_query_2+"&&END_QUERY&&";
        }
        // DOCUMENTO_H_DOCUMENTO GUIA DE REMISION REMITENTE
        str_query_2 = "";
        for(int i = 0; i < ui->comboBox_id_guiarr->count(); i++){
            QString id = ui->comboBox_id_guiarr->itemText(i);

            str_query_2 += ",("+this->id;
            str_query_2 += ", " + id + ")";
        }
        if(ui->comboBox_id_guiarr->count() > 0){
            str_query += "INSERT INTO documento_h_documento(documento_id, documento_id1)VALUES";
            str_query_2.remove(0,1);
            str_query += str_query_2+"&&END_QUERY&&";
        }
    }
    str_query += "COMMIT";
    str_query += "&&END_QUERY&&";

    ui->dateEdit_emision->setDisplayFormat("dd-MM-yyyy");
    ui->dateTimeEdit_sistema->setDisplayFormat("dd-MM-yyyy hh:mm:ss");

    QSqlQuery query;
    SYSTEM->multiple_query(str_query);
    qDebug()<<str_query<<endl;
    if(query.exec(str_query)){
        if(this->id.compare("") == 0){
            op = INGRESAR;
            query.next();
            this->id = query.value(0).toString();
        }else
            op = MODIFICAR;
        return true;
    }else{
        if(query.exec("ROLLBACK")){

        }else{

        }
        return false;
    }
}
bool VentaBoleta::remove()
{
    QString str_query;

    str_query += "DELETE FROM documento WHERE id = "+id;
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
        if(query.exec("ROLLBACK")){

        }else{

        }
        return false;
    }
}

void VentaBoleta::set_time()
{
    ui->dateTimeEdit_sistema->setDateTime(QDateTime::currentDateTime());
}

void VentaBoleta::on_cliente_closing()
{
    ColaboradorBuscar* w = (ColaboradorBuscar*)sender();
    QString persona_id = w->get_ID();
    if(persona_id.compare("") == 0) {
        return;
    }
    tipo_persona_id = QString().setNum(tipo_persona::CLIENTE_DNI);
    this->persona_id = persona_id;
    codigo = w->get_codigo();
    nombre = w->get_nombre();
    direccion = w->get_direccion();

    set_cliente(persona_id, codigo, nombre, direccion);

    ui->pushButton_ing_prod->setFocus(Qt::TabFocusReason);
}
void VentaBoleta::on_producto_closing()
{
    ProductoBuscar* w = (ProductoBuscar*)sender();
    QString producto_id = w->getID();
    qDebug()<<"producto_id: "<<producto_id<<endl;
    if(producto_id.compare("") == 0){
        return;
    }
    QString cantidad = "1.0";
    QString unidad = w->getUnidad();
    QString marca = w->getMarca();
    QString descripcion = w->getDescripcion()+" "+marca;
    QString p_total = "0.000";

    set_producto(producto_id, cantidad, unidad, descripcion, p_total);
}
void VentaBoleta::on_pushButton_cliente_clicked()
{
    VentaCliente* w = new VentaCliente;
    w->set_widget_previous(this);

    w->setTipoClienteDNI();

    connect(w, SIGNAL(closing()), this, SLOT(on_cliente_closing()));

    SYSTEM->change_center_w(this, w);
}

void VentaBoleta::on_pushButton_ing_prod_clicked()
{
    VentaProducto* w = new VentaProducto;
    w->set_widget_previous(this);
    connect(w, SIGNAL(closing()), this, SLOT(on_producto_closing()));

    SYSTEM->change_center_w(this, w);
}

void VentaBoleta::on_pushButton_guardar_clicked()
{
    if(ui->lineEdit_serie->text().compare("") == 0 || ui->lineEdit_numero->text().compare("") == 0){
        int nro_digits = A_Venta::SERIE_N_DIGITS;
        QString str = ui->lineEdit_serie->text();
        int count = str.length();

        for(int i = 0; i < nro_digits-count; i++){
            str.push_front('0');
        }
        ui->lineEdit_serie->setText(str);

        nro_digits = A_Venta::NUMERO_N_DIGITS;
        str = ui->lineEdit_numero->text();
        count = str.length();

        for(int i = 0; i < nro_digits-count; i++){
            str.push_front('0');
        }
        ui->lineEdit_numero->setText(str);

        int ret = QMessageBox::warning(this, "Advertencia", "Serie o número no ha sido modificado."
                                       "\n¿Desea continuar?", "Si", "No");
        switch(ret){
        case 0:{

        }break;
        case 1:{
            return;
        }break;
        }
    }

    if(!(ui->lineEdit_serie->text().compare(serie) == 0 && ui->lineEdit_numero->text().compare(numero) == 0)){
        QString str_query = "(SELECT IF(EXISTS(SELECT 1 FROM boleta";
        str_query += " JOIN anexo ON anexo.documento_id = boleta.comprobante_documento_id";
        str_query += " JOIN comprobante ON (comprobante.operacion_id = "+QString().setNum(operacion_items::VENTA);
        str_query += " AND comprobante.documento_id = boleta.comprobante_documento_id)";
        str_query += " WHERE anexo.serie = '"+ui->lineEdit_serie->text()+"'";
        str_query += " AND anexo.numero = '"+ui->lineEdit_numero->text()+"'";
        str_query += " ORDER BY anexo.serie, anexo.numero), 1 , 0), '')";
        str_query += " UNION ALL";
        str_query += "(SELECT anexo.serie, anexo.numero+1 FROM boleta";
        str_query += " JOIN anexo ON anexo.documento_id = boleta.comprobante_documento_id";
        str_query += " ORDER BY boleta.comprobante_documento_id DESC, anexo.numero DESC LIMIT 1)";

        QSqlQuery query;
        qDebug()<<str_query<<endl;
        if(query.exec(str_query)){
            if(query.next()){
                int exists = query.value(0).toInt();
                if(exists == 1){
                    int ret = QMessageBox::warning(this, "Advertencia", "¡Ya se encuentra registrado!\n"
                                         "¿Desea obtener el ultimo número?", "Si", "No");
                    switch(ret){
                    case 0:{
                        VentaSNSelector* w = new VentaSNSelector;
                        w->set_widget_previous(this);
                        w->set_tipo(venta_items::BOLETA);

                        connect(w, SIGNAL(closing()), this, SLOT(on_ventaSNSelector_closing()));

                        SYSTEM->change_center_w(this, w);
                        return;
                    }break;
                    case 1:{
                        return;
                    }break;
                    }
                }else{

                }
            }else{
                return;
            }
        }else{
            return;
        }
    }

    int ret = QMessageBox::warning(this, "Advertencia", "¿Desea guardar los datos?", "Si", "No");
    switch(ret){
    case 0:{
        if(guardar()){
            this->setAttribute(Qt::WA_DeleteOnClose);
            SYSTEM->change_center_w(this, widget_previous);

            QMainWindow* mw = SYSTEM->get_mainw(this);
            SnackBarInfo* w = new SnackBarInfo;
            w->set_data("Se guardo exitosamente.", ":/new/Iconos/successfull.png");
            mw->statusBar()->addWidget(w);
            int width = mw->width();
            w->setMinimumWidth(width);
            w->setMaximumWidth(width);
        }else{
            if(ui->dateEdit_declaracion->date().month() > ui->dateEdit_emision->date().month()){
                QMessageBox::critical(this, "Error", "El mes de declaración es mayor que el mes emisión.");
            }else{
                if(ui->tableWidget->rowCount() <= 0){
                    QMessageBox::critical(this, "Error", "No tiene productos.");
                }else{
                    QMessageBox::critical(this, "Error", "No se pudieron guardar los datos.");
                    /*
                    QMainWindow* mw = SYSTEM->get_mainw(this);
                    SnackBarInfo* w = new SnackBarInfo;
                    w->set_data("Error inesperado. Consulte al programador.", ":/new/Iconos/exclamation.png");
                    mw->statusBar()->addWidget(w);
                    int width = mw->width();
                    w->setMinimumWidth(width);
                    w->setMaximumWidth(width);
                    */
                }
            }
        }
        return;
    }break;
    case 1:{

    }
    }
}

void VentaBoleta::on_pushButton_eliminar_clicked()
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
            this->setAttribute(Qt::WA_DeleteOnClose);
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

void VentaBoleta::on_pushButton_imprimir_clicked()
{
    if(id.compare("") == 0){
        qDebug()<<"is id"<<endl;
        return;
    }
    int width = 595, height = 842, margen_fila = 0, margen_columna = 0;
    if(ui->lineEdit_serie->text().length() == 4){
        qDebug()<<"is series"<<endl;
        QString str_query = "SELECT series.id, series.width, series.height, series.margen_fila, series.margen_columna FROM series";
        str_query += " WHERE series.serie = '"+ui->lineEdit_serie->text()+"'";
        str_query += " AND series.operacion_id = "+QString().setNum(operacion_items::VENTA);
        str_query += " AND series.tipo_documento_id = "+QString().setNum(tipo_documento::BOLETA);

        QSqlQuery query;
        qDebug()<<str_query<<endl;
        if(query.exec(str_query)){
            if(query.next()){
                id_series = query.value(0).toString();
                width = query.value(1).toInt();
                height = query.value(2).toInt();
                margen_fila = query.value(3).toInt();
                margen_columna = query.value(4).toInt();
            }else{
                str_query = "INSERT INTO series(operacion_id, tipo_documento_id, serie, width, height, margen_fila, margen_columna)VALUES(";
                str_query += QString().setNum(operacion_items::VENTA);
                str_query += ", "+QString().setNum(tipo_documento::BOLETA);
                str_query += ", '"+ui->lineEdit_serie->text()+"'";
                str_query += ", '"+QString().setNum(width)+"'";
                str_query += ", '"+QString().setNum(height)+"'";
                str_query += ", '"+QString().setNum(margen_fila)+"'";
                str_query += ", '"+QString().setNum(margen_columna)+"')";
                str_query += "&&END_QUERY&&";
                str_query += "SELECT MAX(series.id) FROM series";
                str_query += "&&END_QUERY&&";

                SYSTEM->multiple_query(str_query);
                qDebug()<<str_query<<endl;
                if(query.exec(str_query)){
                    qDebug()<< "Guardó serie"<<endl;
                    query.next();
                    id_series = query.value(0).toString();
                    qDebug()<< "id_series:"<<id_series<<endl;
                }else{
                    return;
                }
            }
        }else{
            return;
        }
    }
    VentaConfigHoja* w = new VentaConfigHoja;
    w->set_widget_previous(this);

    QVector<QLabel*> labels;

    QLabel* label = new QLabel(ui->dateEdit_emision->date().toString("dd-MM-yyyy"), 0);
    label->setObjectName(ui->dateEdit_emision->objectName());
    labels.push_back(label);
    label = new QLabel(ui->dateTimeEdit_sistema->dateTime().toString("dd-MM-yyyy hh:mm:ss"), 0);
    label->setObjectName(ui->dateTimeEdit_sistema->objectName());
    labels.push_back(label);    
    label = new QLabel(ui->lineEdit_serie->text(), 0);
    label->setObjectName(ui->lineEdit_serie->objectName());
    labels.push_back(label);    
    label = new QLabel(ui->lineEdit_numero->text(), 0);
    label->setObjectName(ui->lineEdit_numero->objectName());
    labels.push_back(label);
    label = new QLabel(ui->lineEdit_codigo->text(), 0);
    label->setObjectName(ui->lineEdit_codigo->objectName());
    labels.push_back(label);
    label = new QLabel(ui->lineEdit_nombre->text(), 0);
    label->setObjectName(ui->lineEdit_nombre->objectName());
    labels.push_back(label);
    label = new QLabel(ui->lineEdit_direccion->text(), 0);
    label->setObjectName(ui->lineEdit_direccion->objectName());
    labels.push_back(label);

    QTextCursor cursor(new QTextDocument(this));
    cursor.movePosition(QTextCursor::Start);

    QTextTable *table = cursor.insertTable(ui->tableWidget->rowCount(), ui->tableWidget->columnCount()-1);    
    QTextTableFormat format;
    format.setBorder(0);
    table->setFormat(format);
    for(int i=0; i<ui->tableWidget->rowCount(); i++) {
        for(int j=1; j<ui->tableWidget->columnCount(); j++) {
            QString str = ui->tableWidget->item(i, j)->text();
            table->cellAt(i, j-1).firstCursorPosition().insertText(str);
        }
    }
    //qDebug()<<table->document()->toHtml()<<endl;
    label = new QLabel(table->document()->toHtml(), 0);
    label->setObjectName(ui->tableWidget->objectName());    
    labels.push_back(label);

    label = new QLabel(ui->lineEdit_total->text(), 0);
    label->setObjectName(ui->lineEdit_total->objectName());
    labels.push_back(label);

    w->set_tipo_documento(tipo_documento::BOLETA, ui->lineEdit_serie->text(), id_series
                          , labels, width, height, margen_fila, margen_columna);

    SYSTEM->change_center_w(this, w);
}

void VentaBoleta::on_pushButton_amarres_clicked()
{
    if(id.compare("") == 0) {
        QMessageBox::warning(this, "Advertencia", "No existe documento. Debe guardarlo primero.", "Ok");
        return;
    }

    VentaAmarres* w_compra_amarres = new VentaAmarres;
    w_compra_amarres->set_widget_previous(this);
    w_compra_amarres->set_documento(this->id, tipo_documento::BOLETA);

    SYSTEM->change_center_w(this, w_compra_amarres);
}

void VentaBoleta::on_pushButton_salir_clicked()
{
    int ret = QMessageBox::warning(this, "Advertencia", "¿Esta seguro de salir del formulario?", "Si", "No");
    switch(ret){
    case 0:{
        if(widget_previous){
            id = "";
            this->setAttribute(Qt::WA_DeleteOnClose);
            SYSTEM->change_center_w(this, widget_previous);
        }else{
            SYSTEM->clear_center_w(this);
        }
    }break;
    case 1:{

    }break;
    }
}
void VentaBoleta::showEvent(QShowEvent *se)
{
    emit showing();

    se->accept();

    afterShow = true;
}
void VentaBoleta::hideEvent(QHideEvent *he)
{
    emit hiding();
    he->accept();
}
void VentaBoleta::closeEvent(QCloseEvent *ce)
{
    emit closing();
    ce->accept();
}
bool VentaBoleta::eventFilter(QObject *obj, QEvent *e)
{
    QWidget* w_temp;
    w_temp = this;
    if(obj == w_temp){
        if(e->type() == QEvent::MouseButtonPress){
            if(focusWidget()){
                focusWidget()->setFocus();
            }else{
                ui->dateEdit_declaracion->setFocus();
                ui->dateEdit_declaracion->setCurrentSectionIndex(ui->dateEdit_declaracion->currentSectionIndex());
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
                    focusWidget()->setFocus();
                }else{
                    ui->dateEdit_declaracion->setFocus();
                    ui->dateEdit_declaracion->setCurrentSectionIndex(ui->dateEdit_declaracion->currentSectionIndex());
                }
                afterShow = false;
            }
            return true;
        }
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
                ui->dateEdit_emision->setFocus(Qt::TabFocusReason);
                return true;
            }break;
            case Qt::Key_Enter:{
                QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QApplication::sendEvent(w_temp, key);
                return true;
            }break;
            }
        }
        return false;
    }
    w_temp = ui->dateEdit_emision;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;
            switch(KeyEvent->key())
            {
            case Qt::Key_Return:{
                ui->dateTimeEdit_sistema->setFocus(Qt::TabFocusReason);
                return true;
            }break;
            case Qt::Key_Enter:{
                QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QApplication::sendEvent(w_temp, key);
                return true;
            }break;
            }
        }
        return false;
    }

    w_temp = ui->dateTimeEdit_sistema;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;
            switch(KeyEvent->key())
            {
            case Qt::Key_Return:{
                ui->lineEdit_serie->setFocus(Qt::TabFocusReason);
                return true;
            }break;
            case Qt::Key_Enter:{
                QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QApplication::sendEvent(w_temp, key);
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
            case Qt::Key_Enter:{
                QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QApplication::sendEvent(w_temp, key);
                return true;
            }break;
            }


        }
        if(e->type() == QEvent::FocusOut){
            int nro_digits = A_Venta::SERIE_N_DIGITS;
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
                ui->pushButton_cliente->setFocus(Qt::TabFocusReason);
                return true;
            }break;
            case Qt::Key_Enter:{
                QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QApplication::sendEvent(w_temp, key);
                return true;
            }break;
            }
        }
        if(e->type() == QEvent::FocusOut){
            int nro_digits = A_Venta::NUMERO_N_DIGITS;
            QString str = ui->lineEdit_numero->text();
            int count = str.length();

            for(int i = 0; i < nro_digits-count; i++){
                str.push_front('0');
            }
            ui->lineEdit_numero->setText(str);
        }
        return false;
    }    

    w_temp = ui->lineEdit_codigo;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;
            switch(KeyEvent->key())
            {
            case Qt::Key_Return:{
                ui->pushButton_cliente->setFocus(Qt::TabFocusReason);
                return true;
            }break;
            case Qt::Key_Enter:{
                QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QApplication::sendEvent(w_temp, key);
                return true;
            }break;
            }
        }
        return false;
    }

    w_temp = ui->pushButton_cliente;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;
            switch(KeyEvent->key())
            {
            case Qt::Key_Return:{
                ui->pushButton_cliente->click();
                return true;
            }break;
            case Qt::Key_Enter:{
                QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QApplication::sendEvent(w_temp, key);
                return true;
            }break;
            }
        }
        return false;
    }

    w_temp = ui->lineEdit_nombre;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;
            switch(KeyEvent->key())
            {
            case Qt::Key_Return:{
                ui->pushButton_ing_prod->setFocus(Qt::TabFocusReason);
                return true;
            }break;
            case Qt::Key_Enter:{
                QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QApplication::sendEvent(w_temp, key);
                return true;
            }break;
            }
        }
        return false;
    }

    w_temp = ui->lineEdit_direccion;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;
            switch(KeyEvent->key())
            {
            case Qt::Key_Return:{
                ui->pushButton_ing_prod->setFocus(Qt::TabFocusReason);
                return true;
            }break;
            case Qt::Key_Enter:{
                QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QApplication::sendEvent(w_temp, key);
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
            case Qt::Key_Enter:{
                QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QApplication::sendEvent(w_temp, key);
                return true;
            }break;
            }
        }
        return false;
    }

    w_temp = ui->pushButton_down;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:{
                ui->pushButton_down->click();
                return true;
            }break;
            case Qt::Key_Enter:{
                QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QApplication::sendEvent(w_temp, key);
                return true;
            }break;
            }
        }
        return false;
    }

    w_temp = ui->pushButton_up;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:{
                ui->pushButton_up->click();
                return true;
            }break;
            case Qt::Key_Enter:{
                QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QApplication::sendEvent(w_temp, key);
                return true;
            }break;
            }
        }
        return false;
    }

    w_temp = ui->pushButton_borrar;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:{
                ui->pushButton_borrar->click();
                return true;
            }break;
            case Qt::Key_Enter:{
                QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QApplication::sendEvent(w_temp, key);
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
            case Qt::Key_Enter:{
                QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QApplication::sendEvent(w_temp, key);
                return true;
            }break;
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
            case Qt::Key_F4:{
                QTableWidgetItem* item = ui->tableWidget->currentItem();
                if(item) {
                    VentaChartPrecio* w = new VentaChartPrecio();
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

    w_temp = ui->lineEdit_total;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:{
                ui->pushButton_guardar->setFocus(Qt::TabFocusReason);
                return true;
            }break;
            case Qt::Key_Enter:{
                QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QApplication::sendEvent(w_temp, key);
                return true;
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
            case Qt::Key_Enter:{
                QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QApplication::sendEvent(w_temp, key);
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
            case Qt::Key_Enter:{
                QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QApplication::sendEvent(w_temp, key);
                return true;
            }break;
            }
        }
        return false;
    }

    w_temp = ui->pushButton_anular;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:{
                ui->pushButton_anular->click();
                return true;
            }break;
            case Qt::Key_Enter:{
                QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QApplication::sendEvent(w_temp, key);
                return true;
            }break;
            }
        }
        return false;
    }

    w_temp = ui->pushButton_imprimir;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:{
                ui->pushButton_imprimir->click();
                return true;
            }break;
            case Qt::Key_Enter:{
                QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QApplication::sendEvent(w_temp, key);
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
            case Qt::Key_Enter:{
                QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QApplication::sendEvent(w_temp, key);
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
            case Qt::Key_Enter:{
                QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QApplication::sendEvent(w_temp, key);
                return true;
            }break;
            }
        }
        return false;
    }
    w_temp = ui->comboBox_id_registroSinDoc;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            //case Qt::Key_Tab:{
                //this->setFocus();
            //}break;
            case Qt::Key_Return:{
                ui->comboBox_serie_numero_registroSinDoc->setFocus(Qt::TabFocusReason);
                return true;
            }break;
            case Qt::Key_Enter:{
                QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QApplication::sendEvent(w_temp, key);
                return true;
            }break;
            }
        }
        return false;
    }
    w_temp = ui->comboBox_serie_numero_registroSinDoc;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            //case Qt::Key_Tab:{
                //this->setFocus();
            //}break;
            case Qt::Key_Return:{
                ui->pushButton_buscar_registroSinDoc->setFocus(Qt::TabFocusReason);
                return true;
            }break;
            case Qt::Key_Enter:{
                QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QApplication::sendEvent(w_temp, key);
                return true;
            }break;
            }
        }
        return false;
    }
    w_temp = ui->pushButton_buscar_registroSinDoc;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            //case Qt::Key_Tab:{
                //this->setFocus();
            //}break;
            case Qt::Key_Return:{
                ui->pushButton_buscar_registroSinDoc->click();
                return true;
            }break;
            case Qt::Key_Enter:{
                QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QApplication::sendEvent(w_temp, key);
                return true;
            }break;
            }
        }
        return false;
    }
    w_temp = ui->pushButton_jalar_registroSinDoc;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            //case Qt::Key_Tab:{
                //this->setFocus();
            //}break;
            case Qt::Key_Return:{
                ui->pushButton_jalar_registroSinDoc->click();
                return true;
            }break;
            case Qt::Key_Enter:{
                QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QApplication::sendEvent(w_temp, key);
                return true;
            }break;
            }
        }
        return false;
    }
    w_temp = ui->pushButton_quitar_registroSinDoc;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            //case Qt::Key_Tab:{
                //this->setFocus();
            //}break;
            case Qt::Key_Return:{
                ui->pushButton_quitar_registroSinDoc->click();
                return true;
            }break;
            case Qt::Key_Enter:{
                QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QApplication::sendEvent(w_temp, key);
                return true;
            }break;
            }
        }
        return false;
    }
    w_temp = ui->comboBox_id_guiarr;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            //case Qt::Key_Tab:{
                //this->setFocus();
            //}break;
            case Qt::Key_Return:{
                ui->comboBox_serie_numero_guiarr->setFocus(Qt::TabFocusReason);
                return true;
            }break;
            case Qt::Key_Enter:{
                QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QApplication::sendEvent(w_temp, key);
                return true;
            }break;
            }
        }
        return false;
    }
    w_temp = ui->comboBox_serie_numero_guiarr;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            //case Qt::Key_Tab:{
                //this->setFocus();
            //}break;
            case Qt::Key_Return:{
                ui->pushButton_buscar_guiarr->setFocus(Qt::TabFocusReason);
                return true;
            }break;
            case Qt::Key_Enter:{
                QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QApplication::sendEvent(w_temp, key);
                return true;
            }break;
            }
        }
        return false;
    }
    w_temp = ui->pushButton_buscar_guiarr;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            //case Qt::Key_Tab:{
                //this->setFocus();
            //}break;
            case Qt::Key_Return:{
                ui->pushButton_buscar_guiarr->click();
                return true;
            }break;
            case Qt::Key_Enter:{
                QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QApplication::sendEvent(w_temp, key);
                return true;
            }break;
            }
        }
        return false;
    }
    w_temp = ui->pushButton_jalar_guiarr;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            //case Qt::Key_Tab:{
                //this->setFocus();
            //}break;
            case Qt::Key_Return:{
                ui->pushButton_jalar_guiarr->click();
                return true;
            }break;
            case Qt::Key_Enter:{
                QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QApplication::sendEvent(w_temp, key);
                return true;
            }break;
            }
        }
        return false;
    }
    w_temp = ui->pushButton_quitar_guiarr;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            //case Qt::Key_Tab:{
                //this->setFocus();
            //}break;
            case Qt::Key_Return:{
                ui->pushButton_quitar_guiarr->click();
                return true;
            }break;
            case Qt::Key_Enter:{
                QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QApplication::sendEvent(w_temp, key);
                return true;
            }break;
            }
        }
        return false;
    }
    w_temp = ui->comboBox_id_notaPedido;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            //case Qt::Key_Tab:{
                //this->setFocus();
            //}break;
            case Qt::Key_Return:{
                ui->comboBox_serie_numero_notaPedido->setFocus(Qt::TabFocusReason);
                return true;
            }break;
            case Qt::Key_Enter:{
                QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QApplication::sendEvent(w_temp, key);
                return true;
            }break;
            }
        }
        return false;
    }
    w_temp = ui->comboBox_serie_numero_notaPedido;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            //case Qt::Key_Tab:{
                //this->setFocus();
            //}break;
            case Qt::Key_Return:{
                ui->pushButton_buscar_notaPedido->setFocus(Qt::TabFocusReason);
                return true;
            }break;
            case Qt::Key_Enter:{
                QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QApplication::sendEvent(w_temp, key);
                return true;
            }break;
            }
        }
        return false;
    }
    w_temp = ui->pushButton_buscar_notaPedido;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            //case Qt::Key_Tab:{
                //this->setFocus();
            //}break;
            case Qt::Key_Return:{
                ui->pushButton_buscar_notaPedido->click();
                return true;
            }break;
            case Qt::Key_Enter:{
                QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QApplication::sendEvent(w_temp, key);
                return true;
            }break;
            }
        }
        return false;
    }
    w_temp = ui->pushButton_jalar_notaPedido;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            //case Qt::Key_Tab:{
                //this->setFocus();
            //}break;
            case Qt::Key_Return:{
                ui->pushButton_jalar_notaPedido->click();
                return true;
            }break;
            case Qt::Key_Enter:{
                QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QApplication::sendEvent(w_temp, key);
                return true;
            }break;
            }
        }
        return false;
    }
    w_temp = ui->pushButton_quitar_notaPedido;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            //case Qt::Key_Tab:{
                //this->setFocus();
            //}break;
            case Qt::Key_Return:{
                ui->pushButton_quitar_notaPedido->click();
                return true;
            }break;
            case Qt::Key_Enter:{
                QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QApplication::sendEvent(w_temp, key);
                return true;
            }break;
            }
        }
        return false;
    }
    w_temp = ui->comboBox_id_cotizacion;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            //case Qt::Key_Tab:{
                //this->setFocus();
            //}break;
            case Qt::Key_Return:{
                ui->comboBox_serie_numero_cotizacion->setFocus(Qt::TabFocusReason);
                return true;
            }break;
            case Qt::Key_Enter:{
                QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QApplication::sendEvent(w_temp, key);
                return true;
            }break;
            }
        }
        return false;
    }
    w_temp = ui->comboBox_serie_numero_cotizacion;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            //case Qt::Key_Tab:{
                //this->setFocus();
            //}break;
            case Qt::Key_Return:{
                ui->pushButton_buscar_cotizacion->setFocus(Qt::TabFocusReason);
                return true;
            }break;
            case Qt::Key_Enter:{
                QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QApplication::sendEvent(w_temp, key);
                return true;
            }break;
            }
        }
        return false;
    }
    w_temp = ui->pushButton_buscar_cotizacion;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            //case Qt::Key_Tab:{
                //this->setFocus();
            //}break;
            case Qt::Key_Return:{
                ui->pushButton_buscar_cotizacion->click();
                return true;
            }break;
            case Qt::Key_Enter:{
                QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QApplication::sendEvent(w_temp, key);
                return true;
            }break;
            }
        }
        return false;
    }
    w_temp = ui->pushButton_jalar_cotizacion;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            //case Qt::Key_Tab:{
                //this->setFocus();
            //}break;
            case Qt::Key_Return:{
                ui->pushButton_jalar_cotizacion->click();
                return true;
            }break;
            case Qt::Key_Enter:{
                QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QApplication::sendEvent(w_temp, key);
                return true;
            }break;
            }
        }
        return false;
    }
    w_temp = ui->pushButton_quitar_cotizacion;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            //case Qt::Key_Tab:{
                //this->setFocus();
            //}break;
            case Qt::Key_Return:{
                ui->pushButton_quitar_cotizacion->click();
                return true;
            }break;
            case Qt::Key_Enter:{
                QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QApplication::sendEvent(w_temp, key);
                return true;
            }break;
            }
        }
        return false;
    }

    return eventFilter(obj, e);
}
void VentaBoleta::on_pushButton_up_clicked()
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

void VentaBoleta::on_pushButton_down_clicked()
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

void VentaBoleta::on_pushButton_borrar_clicked()
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
        }
    }

    double total = 0.0;

    rowCount = ui->tableWidget->rowCount();

    for(int i = 0; i < rowCount; i++){
        QString s_p_total = ui->tableWidget->item(i, INDEX_P_TOTAL)->text();

        double p_total = s_p_total.toDouble();

        total += p_total;
    }

    ui->lineEdit_total->setText(QString().setNum(total, ' ', DECIMALS_PRECIO_TOTAL));
}

void VentaBoleta::on_lineEdit_serie_returnPressed()
{

}

void VentaBoleta::on_tableWidget_itemChanged(QTableWidgetItem *item)
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

        QString value = QString().setNum(p_total, ' ', DECIMALS_PRECIO_TOTAL);
        SYSTEM->normalDecimal(value);
        ui->tableWidget->item(item->row(), INDEX_P_TOTAL)->setText(value);
    }
    // P_UNITARIO
    if(item->column() == INDEX_P_UNIT){
        QString s_cantidad = ui->tableWidget->item(item->row(), INDEX_CANTIDAD)->text();
        QString s_p_unitario = ui->tableWidget->item(item->row(), INDEX_P_UNIT)->text();

        double cantidad = s_cantidad.toDouble();
        double p_unitario = s_p_unitario.toDouble();
        double p_total;

        p_total = cantidad*p_unitario;

        QString value = QString().setNum(p_total, ' ', DECIMALS_PRECIO_TOTAL);
        SYSTEM->normalDecimal(value);
        ui->tableWidget->item(item->row(), INDEX_P_TOTAL)->setText(value);
    }
    // P_TOTAL
    if(item->column() == INDEX_P_TOTAL){
        QString s_cantidad = ui->tableWidget->item(item->row(), INDEX_CANTIDAD)->text();
        QString s_p_total = ui->tableWidget->item(item->row(), INDEX_P_TOTAL)->text();

        double cantidad = s_cantidad.toDouble();
        double p_total = s_p_total.toDouble();

        double p_unitario = p_total/cantidad;

        QString value = QString().setNum(p_unitario, ' ', DECIMALS_PRECIO_UNITARIO);
        SYSTEM->normalDecimal(value);
        ui->tableWidget->item(item->row(), INDEX_P_UNIT)->setText(value);
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
    QString str_total = QString().setNum(total, ' ', DECIMALS_PRECIO_TOTAL);
    SYSTEM->normalDecimal(str_total);
    ui->lineEdit_total->setText(str_total);

    connect(ui->tableWidget, SIGNAL(itemChanged(QTableWidgetItem*))
               , this, SLOT(on_tableWidget_itemChanged(QTableWidgetItem*)));
}

void VentaBoleta::on_pushButton_anular_clicked()
{
    if(id.compare("") == 0){
        QMessageBox::warning(this, "Advertencia", "Aún no puede anular el documento.", "Ok");
    }

    int anulado = this->anulado;
    if(anulado == 0){
        anulado = 1;
    }else{
        anulado = 0;
    }
    QString str_query = "UPDATE boleta SET";
    str_query += " anulado = '"+QString().setNum(anulado)+"'";
    str_query += " WHERE comprobante_documento_id = "+id;

    QSqlQuery query;
    if(query.exec(str_query)){
        if(anulado == 0){
            ui->pushButton_anular->setIcon(QIcon(":/new/Iconos/anular.png"));
            this->anulado = anulado;
        }else{
            ui->pushButton_anular->setIcon(QIcon(":/new/Iconos/anulado.png"));
            this->anulado = anulado;
        }

    }else{

    }
}

void VentaBoleta::on_dateEdit_declaracion_dateChanged(const QDate &date)
{
    if(date.month() > ui->dateEdit_emision->date().month()){
        /*
        QMessageBox::warning(this, "Advertencia", "El mes de declaración no puede ser mayor\nque la fecha de emisión.", "Ok");

        disconnect(ui->dateEdit_declaracion, SIGNAL(dateChanged(QDate)), this, SLOT(on_dateEdit_declaracion_dateChanged(QDate)));
        QDate date_temp;
        date_temp.setDate(date.year(), mes_declaracion, date.day());
        ui->dateEdit_declaracion->setDate(date_temp);
        connect(ui->dateEdit_declaracion, SIGNAL(dateChanged(QDate)), this, SLOT(on_dateEdit_declaracion_dateChanged(QDate)));
        */
    }else{
        mes_declaracion = date.month();
    }
}

void VentaBoleta::on_dateEdit_emision_dateChanged(const QDate &date)
{
    if(date.month() < ui->dateEdit_declaracion->date().month()){
        /*
        QMessageBox::warning(this, "Advertencia", "El mes de declaración no puede ser mayor\nque la fecha de emisión.", "Ok");

        disconnect(ui->dateEdit_emision, SIGNAL(dateChanged(QDate)), this, SLOT(on_dateEdit_emision_dateChanged(QDate)));
        QDate date_temp;
        date_temp.setDate(date.year(), mes_emision, date.day());
        ui->dateEdit_emision->setDate(date_temp);
        connect(ui->dateEdit_emision, SIGNAL(dateChanged(QDate)), this, SLOT(on_dateEdit_emision_dateChanged(QDate)));
        */
    }else{
        mes_emision = date.month();
    }
}
void VentaBoleta::on_registroSinDoc_buscar_closing()
{
    VentaBuscar* w = (VentaBuscar*)sender();

    if(w->getID().compare("")==0)return;

    QString id = w->getID();
    QString sn = w->getSerie() + " - " + w->getNumero();

    ui->comboBox_id_registroSinDoc->addItem(id);
    ui->comboBox_serie_numero_registroSinDoc->addItem(sn);
}

void VentaBoleta::on_notaPedido_buscar_closing()
{
    VentaBuscar* w = (VentaBuscar*)sender();

    if(w->getID().compare("")==0)return;

    QString id = w->getID();
    QString sn = w->getSerie() + " - " + w->getNumero();

    ui->comboBox_id_notaPedido->addItem(id);
    ui->comboBox_serie_numero_notaPedido->addItem(sn);
}
void VentaBoleta::on_cotizacion_buscar_closing()
{
    VentaBuscar* w = (VentaBuscar*)sender();

    if(w->getID().compare("")==0)return;

    QString id = w->getID();
    QString sn = w->getSerie() + " - " + w->getNumero();

    ui->comboBox_id_cotizacion->addItem(id);
    ui->comboBox_serie_numero_cotizacion->addItem(sn);
}
void VentaBoleta::on_guiarr_buscar_closing()
{
    VentaBuscar* w = (VentaBuscar*)sender();

    if(w->getID().compare("")==0)return;

    QString id = w->getID();
    QString sn = w->getSerie() + " - " + w->getNumero();

    ui->comboBox_id_guiarr->addItem(id);
    ui->comboBox_serie_numero_guiarr->addItem(sn);
}
void VentaBoleta::on_pushButton_buscar_registroSinDoc_clicked()
{
    if(persona_id.compare("") == 0){
        QMessageBox::warning(this, "Advertencia", "No ha ingresado el cliente.", "Ok");
        return;
    }

    VentaBuscar* w = new VentaBuscar;
    w->set_widget_previous(this);
    w->set_tipo(venta_items::REGISTRO_SIN_DOCUMENTO);
    connect(w, SIGNAL(closing()), this, SLOT(on_registroSinDoc_buscar_closing()));

    SYSTEM->change_center_w(this, w);
}

void VentaBoleta::on_pushButton_jalar_registroSinDoc_clicked()
{
    if(ui->comboBox_serie_numero_registroSinDoc->currentText().compare("") == 0){
        return;
    }else{
    }
    QString sn = ui->comboBox_serie_numero_registroSinDoc->currentText();
    QStringList list = sn.split(QRegExp("\\s|-"), QString::SkipEmptyParts);
    QString serie = list[0];
    QString numero = list[1];
    if (serie.length() == A_Venta::SERIE_N_DIGITS && numero.length() == A_Venta::NUMERO_N_DIGITS) {
        ui->comboBox_id_registroSinDoc->setCurrentIndex(ui->comboBox_serie_numero_registroSinDoc->currentIndex());
        QString id = ui->comboBox_id_registroSinDoc->currentText();
        QString str_query = "";
        str_query += "SELECT producto.id, documento_h_producto.cantidad, unidad.unidad";
        str_query += ", concat(producto.descripcion, ' ', marca.marca), documento_h_producto.precio";
        str_query += " FROM registro_sin_documento";
        str_query += " JOIN documento ON documento.id = registro_sin_documento.comprobante_documento_id";
        str_query += " JOIN documento_h_producto ON documento.id = documento_h_producto.documento_id";
        str_query += " JOIN producto ON producto.id = documento_h_producto.producto_id";
        str_query += " LEFT JOIN tipo ON tipo.id = producto.tipo_id";
        str_query += " LEFT JOIN marca ON marca.id = producto.marca_id";
        str_query += " LEFT JOIN unidad ON unidad.id = producto.unidad_id";
        str_query += " WHERE documento.id = "+id;
        str_query += " GROUP BY documento_h_producto.id";

        QSqlQuery query;
        qDebug()<<str_query<<endl;
        if(query.exec(str_query)){
            while(query.next()){
                QString producto_id = query.value(0).toString();
                QString cantidad = query.value(1).toString();
                QString unidad = query.value(2).toString();
                QString descripcion = query.value(3).toString();
                QString p_total = query.value(4).toString();

                set_producto(producto_id, cantidad, unidad, descripcion, p_total);
            }
        }else{

        }
    }
}

void VentaBoleta::on_pushButton_quitar_registroSinDoc_clicked()
{
    if(ui->comboBox_serie_numero_registroSinDoc->currentIndex() == -1)
        return;
    int ret = QMessageBox::warning(this, "Advertencia", "¿Esta seguro de quitar el documento?", "Si", "No");
    switch(ret){
    case 0:{
        ui->comboBox_id_registroSinDoc->removeItem(ui->comboBox_serie_numero_registroSinDoc->currentIndex());
        ui->comboBox_serie_numero_registroSinDoc->removeItem(ui->comboBox_serie_numero_registroSinDoc->currentIndex());
    }break;
    case 1:{

    }break;
    }
}

void VentaBoleta::on_pushButton_buscar_notaPedido_clicked()
{
    if(persona_id.compare("") == 0){
        QMessageBox::warning(this, "Advertencia", "No ha ingresado el cliente.", "Ok");
        return;
    }

    VentaBuscar* w = new VentaBuscar;
    w->set_widget_previous(this);
    w->set_tipo(venta_items::NOTA_PEDIDO);
    connect(w, SIGNAL(closing()), this, SLOT(on_notaPedido_buscar_closing()));

    SYSTEM->change_center_w(this, w);
}

void VentaBoleta::on_pushButton_jalar_notaPedido_clicked()
{
    if(ui->comboBox_serie_numero_notaPedido->currentText().compare("") == 0){
        return;
    }else{
    }
    QString sn = ui->comboBox_serie_numero_notaPedido->currentText();
    QStringList list = sn.split(QRegExp("\\s|-"), QString::SkipEmptyParts);
    QString serie = list[0];
    QString numero = list[1];
    if (serie.length() == A_Venta::SERIE_N_DIGITS && numero.length() == A_Venta::NUMERO_N_DIGITS) {
        ui->comboBox_id_notaPedido->setCurrentIndex(ui->comboBox_serie_numero_notaPedido->currentIndex());
        QString id = ui->comboBox_id_notaPedido->currentText();
        QString str_query = "";
        str_query += "SELECT producto.id, documento_h_producto.cantidad, unidad.unidad";
        str_query += ", concat(producto.descripcion, ' ', marca.marca), documento_h_producto.precio";
        str_query += " FROM nota_pedido";
        str_query += " JOIN documento ON documento.id = nota_pedido.comprobante_documento_id";
        str_query += " JOIN documento_h_producto ON documento.id = documento_h_producto.documento_id";
        str_query += " JOIN producto ON producto.id = documento_h_producto.producto_id";
        str_query += " LEFT JOIN tipo ON tipo.id = producto.tipo_id";
        str_query += " LEFT JOIN marca ON marca.id = producto.marca_id";
        str_query += " LEFT JOIN unidad ON unidad.id = producto.unidad_id";
        str_query += " WHERE documento.id = "+id;
        str_query += " GROUP BY documento_h_producto.id";

        QSqlQuery query;
        qDebug()<<str_query<<endl;
        if(query.exec(str_query)){
            while(query.next()){
                QString producto_id = query.value(0).toString();
                QString cantidad = query.value(1).toString();
                QString unidad = query.value(2).toString();
                QString descripcion = query.value(3).toString();
                QString p_total = query.value(4).toString();

                set_producto(producto_id, cantidad, unidad, descripcion, p_total);
            }
        }else{

        }
    }
}

void VentaBoleta::on_pushButton_quitar_notaPedido_clicked()
{
    if(ui->comboBox_serie_numero_notaPedido->currentIndex() == -1)
        return;
    int ret = QMessageBox::warning(this, "Advertencia", "¿Esta seguro de quitar el documento?", "Si", "No");
    switch(ret){
    case 0:{
        ui->comboBox_id_notaPedido->removeItem(ui->comboBox_serie_numero_notaPedido->currentIndex());
        ui->comboBox_serie_numero_notaPedido->removeItem(ui->comboBox_serie_numero_notaPedido->currentIndex());
    }break;
    case 1:{

    }break;
    }
}

void VentaBoleta::on_pushButton_buscar_cotizacion_clicked()
{
    if(persona_id.compare("") == 0){
        QMessageBox::warning(this, "Advertencia", "No ha ingresado el cliente.", "Ok");
        return;
    }

    VentaBuscar* w = new VentaBuscar;
    w->set_widget_previous(this);
    w->set_tipo(venta_items::COTIZACION);
    connect(w, SIGNAL(closing()), this, SLOT(on_cotizacion_buscar_closing()));

    SYSTEM->change_center_w(this, w);
}

void VentaBoleta::on_pushButton_jalar_cotizacion_clicked()
{
    if(ui->comboBox_serie_numero_cotizacion->currentText().compare("") == 0){
        return;
    }else{
    }
    QString sn = ui->comboBox_serie_numero_cotizacion->currentText();
    QStringList list = sn.split(QRegExp("\\s|-"), QString::SkipEmptyParts);
    QString serie = list[0];
    QString numero = list[1];
    if (serie.length() == A_Venta::SERIE_N_DIGITS && numero.length() == A_Venta::NUMERO_N_DIGITS) {
        ui->comboBox_id_cotizacion->setCurrentIndex(ui->comboBox_serie_numero_cotizacion->currentIndex());
        QString id = ui->comboBox_id_cotizacion->currentText();
        QString str_query = "";
        str_query += "SELECT producto.id, documento_h_producto.cantidad, unidad.unidad";
        str_query += ", concat(producto.descripcion, ' ', marca.marca), documento_h_producto.precio";
        str_query += " FROM cotizacion";
        str_query += " JOIN documento ON documento.id = cotizacion.comprobante_documento_id";
        str_query += " JOIN documento_h_producto ON documento.id = documento_h_producto.documento_id";
        str_query += " JOIN producto ON producto.id = documento_h_producto.producto_id";
        str_query += " LEFT JOIN tipo ON tipo.id = producto.tipo_id";
        str_query += " LEFT JOIN marca ON marca.id = producto.marca_id";
        str_query += " LEFT JOIN unidad ON unidad.id = producto.unidad_id";
        str_query += " WHERE documento.id = "+id;
        str_query += " GROUP BY documento_h_producto.id";

        QSqlQuery query;
        qDebug()<<str_query<<endl;
        if(query.exec(str_query)){
            while(query.next()){
                QString producto_id = query.value(0).toString();
                QString cantidad = query.value(1).toString();
                QString unidad = query.value(2).toString();
                QString descripcion = query.value(3).toString();
                QString p_total = query.value(4).toString();

                set_producto(producto_id, cantidad, unidad, descripcion, p_total);
            }
        }else{

        }
    }
}

void VentaBoleta::on_pushButton_quitar_cotizacion_clicked()
{
    if(ui->comboBox_serie_numero_cotizacion->currentIndex() == -1)
        return;
    int ret = QMessageBox::warning(this, "Advertencia", "¿Esta seguro de quitar el documento?", "Si", "No");
    switch(ret){
    case 0:{
        ui->comboBox_id_cotizacion->removeItem(ui->comboBox_serie_numero_cotizacion->currentIndex());
        ui->comboBox_serie_numero_cotizacion->removeItem(ui->comboBox_serie_numero_cotizacion->currentIndex());
    }break;
    case 1:{

    }break;
    }
}

void VentaBoleta::on_pushButton_buscar_guiarr_clicked()
{
    if(persona_id.compare("") == 0){
        QMessageBox::warning(this, "Advertencia", "No ha ingresado el cliente.", "Ok");
        return;
    }

    VentaBuscar* w = new VentaBuscar;
    w->set_widget_previous(this);
    w->set_tipo(venta_items::GUIA_REMISION_REMITENTE);
    connect(w, SIGNAL(closing()), this, SLOT(on_guiarr_buscar_closing()));

    SYSTEM->change_center_w(this, w);
}

void VentaBoleta::on_pushButton_jalar_guiarr_clicked()
{
    if(ui->comboBox_serie_numero_guiarr->currentText().compare("") == 0){
        return;
    }else{
    }
    QString sn = ui->comboBox_serie_numero_guiarr->currentText();
    QStringList list = sn.split(QRegExp("\\s|-"), QString::SkipEmptyParts);
    QString serie = list[0];
    QString numero = list[1];
    if (serie.length() == A_Venta::SERIE_N_DIGITS && numero.length() == A_Venta::NUMERO_N_DIGITS) {
        ui->comboBox_id_guiarr->setCurrentIndex(ui->comboBox_serie_numero_guiarr->currentIndex());
        QString id = ui->comboBox_id_guiarr->currentText();
        QString str_query = "";
        str_query += "SELECT producto.id, documento_h_producto.cantidad, unidad.unidad";
        str_query += ", concat(producto.descripcion, ' ', marca.marca), documento_h_producto.precio";
        str_query += " FROM guia_remision_remitente";
        str_query += " JOIN documento ON documento.id = guia_remision_remitente.comprobante_documento_id";
        str_query += " JOIN documento_h_producto ON documento.id = documento_h_producto.documento_id";
        str_query += " JOIN producto ON producto.id = documento_h_producto.producto_id";
        str_query += " LEFT JOIN tipo ON tipo.id = producto.tipo_id";
        str_query += " LEFT JOIN marca ON marca.id = producto.marca_id";
        str_query += " LEFT JOIN unidad ON unidad.id = producto.unidad_id";
        str_query += " WHERE documento.id = "+id;
        str_query += " GROUP BY documento_h_producto.id";

        QSqlQuery query;
        qDebug()<<str_query<<endl;
        if(query.exec(str_query)){
            while(query.next()){
                QString producto_id = query.value(0).toString();
                QString cantidad = query.value(1).toString();
                QString unidad = query.value(2).toString();
                QString descripcion = query.value(3).toString();
                QString p_total = "0.000";

                set_producto(producto_id, cantidad, unidad, descripcion, p_total);
            }
        }else{

        }
    }
}

void VentaBoleta::on_pushButton_quitar_guiarr_clicked()
{
    if(ui->comboBox_serie_numero_guiarr->currentIndex() == -1)
        return;
    int ret = QMessageBox::warning(this, "Advertencia", "¿Esta seguro de quitar el documento?", "Si", "No");
    switch(ret){
    case 0:{
        ui->comboBox_id_guiarr->removeItem(ui->comboBox_serie_numero_guiarr->currentIndex());
        ui->comboBox_serie_numero_guiarr->removeItem(ui->comboBox_serie_numero_guiarr->currentIndex());
    }break;
    case 1:{

    }break;
    }
}
