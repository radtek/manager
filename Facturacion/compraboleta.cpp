#include "compraboleta.h"
#include "ui_compraboleta.h"

CompraBoleta::CompraBoleta(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CompraBoleta)
{
    ui->setupUi(this);

    afterShow = false;

    widget_previous = NULL;

    op = -1;

    QRegExp regExp_ruc("[0-9]{11,11}");
    ui->lineEdit_cod->setValidator(new QRegExpValidator(regExp_ruc));

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

    ui->comboBox_id_guia->hide();
    ui->comboBox_id_orden->hide();

    // INSTALL EVENTS
    this->installEventFilter(this);
    ui->dateEdit_declaracion->installEventFilter(this);
    ui->dateTimeEdit_emision->installEventFilter(this);
    ui->lineEdit_serie->installEventFilter(this);
    ui->lineEdit_numero->installEventFilter(this);
    ui->pushButton_proveedor->installEventFilter(this);
    ui->pushButton_ing_prod->installEventFilter(this);
    ui->tableWidget->installEventFilter(this);
    ui->pushButton_amarres->installEventFilter(this);
    ui->pushButton_guardar->installEventFilter(this);
    ui->pushButton_eliminar->installEventFilter(this);
    ui->pushButton_salir->installEventFilter(this);

    ui->comboBox_serie_numero_orden->installEventFilter(this);
    ui->pushButton_buscar_orden->installEventFilter(this);
    ui->pushButton_jalar_orden->installEventFilter(this);
    ui->pushButton_buscar_orden->installEventFilter(this);
    ui->comboBox_serie_numero_guia->installEventFilter(this);
    ui->pushButton_buscar_guia->installEventFilter(this);
    ui->pushButton_jalar_guia->installEventFilter(this);
    ui->pushButton_buscar_guia->installEventFilter(this);
}

CompraBoleta::~CompraBoleta()
{
    qDebug()<<"delete compra boleta"<<endl;
    delete ui;
}
void CompraBoleta::setVer()
{
    ui->pushButton_amarres->hide();
    ui->pushButton_canjear->hide();
    ui->pushButton_eliminar->hide();
    ui->pushButton_guardar->hide();
}
QString CompraBoleta::getID(){
    return id;
}
QString CompraBoleta::getPersonaID(){
    return persona_id;
}
QString CompraBoleta::getFechaDeclaracion(){
    return ui->dateEdit_declaracion->date().toString("dd-MM-yyyy");
}
QString CompraBoleta::getFechaEmision(){
    return ui->dateTimeEdit_emision->date().toString("dd-MM-yyyy");
}
QString CompraBoleta::getSerie(){
    return ui->lineEdit_serie->text();
}
QString CompraBoleta::getNumero(){
    return ui->lineEdit_numero->text();
}
QString CompraBoleta::getCodigo(){
    return ui->lineEdit_cod->text();
}
QString CompraBoleta::getNombre(){
    return ui->lineEdit_serie->text();
}
QVector<QVector<QString> > CompraBoleta::getOrdenes(){
    for(int i=0; i<ui->comboBox_id_orden->count(); i++){
        ordenes.push_back(QVector<QString>());
        ordenes[i].push_back(ui->comboBox_id_orden->itemText(i));
        ordenes[i].push_back(ui->comboBox_serie_numero_orden->itemText(i));
    }
    return ordenes;
}
QVector<QVector<QString> > CompraBoleta::getGuias(){
    for(int i=0; i<ui->comboBox_id_guia->count(); i++){
        guias.push_back(QVector<QString>());
        guias[i].push_back(ui->comboBox_id_guia->itemText(i));
        guias[i].push_back(ui->comboBox_serie_numero_guia->itemText(i));
    }
    return guias;
}

void CompraBoleta::set_widget_previous(QWidget *w)
{
    this->widget_previous = w;
}

bool CompraBoleta::select(QString id
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

    str_query = "(SELECT COUNT(*), '', '', '', '' FROM documento_h_documento";
    str_query += " JOIN orden ON documento_h_documento.documento_id1 = orden.comprobante_documento_id";
    str_query += " WHERE documento_h_documento.documento_id = "+id+")";
    str_query += " UNION ALL";
    str_query += "(SELECT documento_h_documento.documento_id1, anexo.serie, anexo.numero, '', '' FROM documento_h_documento";
    str_query += " JOIN orden ON documento_h_documento.documento_id1 = orden.comprobante_documento_id";
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
    str_query += "(SELECT boleta.declaracion, '', '', '', '' FROM boleta";
    str_query += " WHERE boleta.comprobante_documento_id = "+id+")";
    str_query += " UNION ALL";
    str_query += "(SELECT producto.id, d_h_prod.cantidad, unidad.unidad";
    //str_query += ", concat(producto.descripcion,' ',IF(marca.marca IS NULL, '', concat(' ', marca.marca))), d_h_prod.precio FROM documento";
    str_query += ", producto.descripcion, d_h_prod.precio FROM documento";
    //str_query += " JOIN documento_h_persona d_h_per ON d.id = d_h_per.documento_id";
    //str_query += " JOIN persona ON persona.id = d_h_per.persona_id";
    //str_query += " JOIN juridica ON persona.id = d_h_per.persona_id";
    //str_query += " JOIN proveedor ON persona.id = proveedor.juridica_persona_id";
    str_query += " JOIN documento_h_producto d_h_prod ON documento.id = d_h_prod.documento_id";
    str_query += " JOIN producto ON producto.id = d_h_prod.producto_id";
    str_query += " LEFT JOIN marca ON marca.id = producto.marca_id";
    str_query += " LEFT JOIN unidad ON unidad.id = producto.unidad_id";
    str_query += " WHERE documento.id = "+id+" GROUP BY d_h_prod.id)";

    qDebug()<<str_query<<endl;
    if(query.exec(str_query)){
        query.next();
        int countOrdenes = query.value(0).toInt();
        for(int i=0; i<countOrdenes; i++){
            query.next();
            QString id = query.value(0).toString();
            QString serie = query.value(1).toString();
            QString numero = query.value(2).toString();
            QString sn = serie + " - " + numero;
            ui->comboBox_id_orden->addItem(id);
            ui->comboBox_serie_numero_orden->addItem(sn);
        }

        query.next();
        int countGuias = query.value(0).toInt();
        for(int i=0; i<countGuias; i++){
            query.next();
            QString id = query.value(0).toString();
            QString serie = query.value(1).toString();
            QString numero = query.value(2).toString();
            QString sn = serie + " - " + numero;
            ui->comboBox_id_guia->addItem(id);
            ui->comboBox_serie_numero_guia->addItem(sn);
        }

        QDateTime dt;
        QDate date = QDate::fromString(fecha_emision, "dd-MM-yyyy");
        dt.setDate(date);

        ui->dateTimeEdit_emision->setDateTime(dt);

        query.next();
        ui->dateEdit_declaracion->setDate(query.value(0).toDate());

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

void CompraBoleta::set_proveedor(QString persona_id, QString codigo, QString nombre)
{
    /*
    this->persona_id = persona_id;
    this->codigo = codigo;
    this->nombre = nombre;
    */

    ui->lineEdit_cod->setText(codigo);
    ui->lineEdit_nombre->setText(nombre);
}

void CompraBoleta::set_producto(QString producto_id
                                , QString cantidad, QString unidad
                                , QString descripcion, QString p_total)
{
    disconnect(ui->tableWidget, SIGNAL(itemChanged(QTableWidgetItem*))
               , this, SLOT(on_tableWidget_itemChanged(QTableWidgetItem*)));

    cantidad = QString().setNum(cantidad.toDouble(), ' ', DECIMALS_CANTIDAD);
    SYSTEM->normalDecimal(cantidad);
    p_total = QString().setNum(p_total.toDouble(), ' ', DECIMALS_PRECIO_TOTAL);
    SYSTEM->normalDecimal(p_total);

    int rowCount = ui->tableWidget->rowCount();
    ui->tableWidget->setRowCount(rowCount+1);

    ui->tableWidget->setItem(rowCount, INDEX_ID, new QTableWidgetItem(producto_id));
    ui->tableWidget->setItem(rowCount, INDEX_CANTIDAD, new QTableWidgetItem(cantidad));
    ui->tableWidget->setItem(rowCount, INDEX_UNIDAD, new QTableWidgetItem(unidad));
    ui->tableWidget->setItem(rowCount, INDEX_DESCRIPCION, new QTableWidgetItem(descripcion));
    QString p_unitario = QString().setNum(p_total.toDouble()/cantidad.toDouble(), ' ', DECIMALS_CANTIDAD);
    SYSTEM->normalDecimal(p_unitario);
    ui->tableWidget->setItem(rowCount, INDEX_P_UNIT, new QTableWidgetItem(p_unitario));
    ui->tableWidget->setItem(rowCount, INDEX_P_TOTAL, new QTableWidgetItem(p_total));    

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

    // CON IGV
    QString str_total = QString().setNum(total, ' ', DECIMALS_PRECIO_TOTAL);
    SYSTEM->normalDecimal(str_total);
    ui->lineEdit_total->setText(str_total);

    SYSTEM->table_resize_to_contents(0, ui->tableWidget);

    connect(ui->tableWidget, SIGNAL(itemChanged(QTableWidgetItem*))
               , this, SLOT(on_tableWidget_itemChanged(QTableWidgetItem*)));
}
bool CompraBoleta::guardar()
{
    if(ui->dateEdit_declaracion->date().month() > ui->dateTimeEdit_emision->date().month()){
        return false;
    }
    if(persona_id.compare("") == 0)
    {
        return false;
    }
    if(ui->tableWidget->rowCount() <= 0)
    {
        return false;
    }

    QString str_query;

    ui->dateTimeEdit_emision->setDisplayFormat("yyyy-MM-dd hh:mm:ss");
    ui->dateTimeEdit_sistema->setDisplayFormat("yyyy-MM-dd hh:mm:ss");

    if (id.compare("") == 0) {
        // DOCUMENTO
        str_query += "INSERT INTO documento(tipo_documento_id, habilitado)VALUES(";
        str_query += QString().setNum(tipo_documento::BOLETA);
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
        //str_query += ", '"+ui->dateEdit_declaracion->date().toString("yyyy-MM-dd")+"'";
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

        // BOLETA
        str_query += "INSERT INTO boleta(comprobante_documento_id";
        str_query += ", declaracion)VALUES(";
        str_query += "(SELECT MAX(documento.id) FROM documento)";
        str_query += ", '"+ui->dateEdit_declaracion->date().toString("yyyy-MM-dd")+"')";
        str_query += "&&END_QUERY&&";

        // DOCUMENTO_H_DOCUMENTO ORDEN
        str_query_2 = "";
        for(int i = 0; i < ui->comboBox_id_orden->count(); i++){
            QString id = ui->comboBox_id_orden->itemText(i);

            str_query_2 += ",((SELECT MAX(id) FROM documento)";
            str_query_2 += ", " + id + ")";
        }
        if(ui->comboBox_id_orden->count() > 0){
            str_query += "INSERT INTO documento_h_documento(documento_id, documento_id1)VALUES";
            str_query_2.remove(0,1);
            str_query += str_query_2+"&&END_QUERY&&";
        }

        // DOCUMENTO_H_DOCUMENTO GUIA
        str_query_2 = "";
        for(int i = 0; i < ui->comboBox_id_guia->count(); i++){
            QString id = ui->comboBox_id_guia->itemText(i);

            str_query_2 += ",((SELECT MAX(id) FROM documento)";
            str_query_2 += ", " + id + ")";
        }
        if(ui->comboBox_id_guia->count() > 0){
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

        str_query += "UPDATE boleta SET";
        str_query += " declaracion = '"+ui->dateEdit_declaracion->date().toString("yyyy-MM-dd")+"'";
        str_query += " WHERE comprobante_documento_id = "+id;
        str_query += "&&END_QUERY&&";

        // DOCUMENTO_H_DOCUMENTO ORDEN
        str_query += "DELETE FROM documento_h_documento WHERE documento_id = "+id;
        str_query += "&&END_QUERY&&";

        str_query_2 = "";
        for(int i = 0; i < ui->comboBox_id_orden->count(); i++){
            QString id = ui->comboBox_id_orden->itemText(i);

            str_query_2 += ",("+this->id;
            str_query_2 += ", " + id + ")";
        }
        if(ui->comboBox_id_orden->count() > 0){
            str_query += "INSERT INTO documento_h_documento(documento_id, documento_id1)VALUES";
            str_query_2.remove(0,1);
            str_query += str_query_2+"&&END_QUERY&&";
        }

        // DOCUMENTO_H_DOCUMENTO GUIA
        str_query_2 = "";
        for(int i = 0; i < ui->comboBox_id_guia->count(); i++){
            QString id = ui->comboBox_id_guia->itemText(i);

            str_query_2 += ",("+this->id;
            str_query_2 += ", " + id + ")";
        }
        if(ui->comboBox_id_guia->count() > 0){
            str_query += "INSERT INTO documento_h_documento(documento_id, documento_id1)VALUES";
            str_query_2.remove(0,1);
            str_query += str_query_2+"&&END_QUERY&&";
        }
    }
    str_query += "COMMIT";
    str_query += "&&END_QUERY&&";

    ui->dateTimeEdit_emision->setDisplayFormat("dd-MM-yyyy");
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
bool CompraBoleta::remove()
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
void CompraBoleta::set_time()
{
    ui->dateTimeEdit_sistema->setDateTime(QDateTime::currentDateTime());
}

void CompraBoleta::on_proveedor_closing()
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
void CompraBoleta::on_producto_closing()
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
    QString p_total = w->getPrecio();

    set_producto(producto_id, cantidad, unidad, descripcion, p_total);
}
void CompraBoleta::on_pushButton_proveedor_clicked()
{
    CompraProveedor* w_buscar_proveedor = new CompraProveedor;
    w_buscar_proveedor->setTipoProveedor();
    w_buscar_proveedor->hideOptTransportista();
    w_buscar_proveedor->hideOptClienteDNI();
    w_buscar_proveedor->hideOptClienteRUC();
    w_buscar_proveedor->hideOptUsuario();
    w_buscar_proveedor->set_widget_previous(this);
    connect(w_buscar_proveedor, SIGNAL(closing()), this, SLOT(on_proveedor_closing()));;

    SYSTEM->change_center_w(this, w_buscar_proveedor);
}

void CompraBoleta::on_pushButton_ing_prod_clicked()
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

void CompraBoleta::on_tableWidget_itemChanged(QTableWidgetItem *item)
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

void CompraBoleta::on_pushButton_guardar_clicked()
{
    int ret = QMessageBox::warning(this, "Advertencia", "¿Desea guardar los datos?", "Si", "No");
    switch(ret){
    case 0:{
        if(guardar()){
            //QMessageBox::information(this, "Información", "Se guardo con éxito.");
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
            if(ui->dateEdit_declaracion->date().month() > ui->dateTimeEdit_emision->date().month()){
                QMessageBox::critical(this, "Error", "El mes de declaración es mayor que el mes emisión.");
            }else{
                if(persona_id.compare("") == 0){
                    QMessageBox::critical(this, "Error", "No ingresó el proveedor.");
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
        }
        return;
    }break;
    case 1:{

    }
    }
}


void CompraBoleta::on_pushButton_eliminar_clicked()
{    
    if(id.compare("") == 0) {
        QMessageBox::warning(this, "Advertencia", "No se puede eliminar aún.", "Ok");
        return;
    }

    int ret = QMessageBox::warning(this, "Advertencia", "Esta apunto de eliminar los datos. ¿Desea de todas formas eliminar los datos?", "Si", "No");
    switch(ret){
    case 0:{
        if(remove()){
            op = ELIMINAR;
            QMessageBox::information(this, "Información", "Se eliminaron los datos con éxito.");
            id = "";
            this->setAttribute(Qt::WA_DeleteOnClose);
            SYSTEM->change_center_w(this, widget_previous);

            QMainWindow* mw = SYSTEM->get_mainw(this);
            SnackBarInfo* w = new SnackBarInfo;
            w->set_data("Item eliminado con éxito.", ":/new/Iconos/trash_full_onyx.png");
            mw->statusBar()->addWidget(w);
            int width = mw->width();
            w->setMinimumWidth(width);
            w->setMaximumWidth(width);
        }else{
            QMessageBox::critical(this, "Error", "No se pudieron eliminar los datos.");
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
        return;
    }break;
    case 1:{

    }
    }
}
void CompraBoleta::on_pushButton_salir_clicked()
{
    int ret = QMessageBox::warning(this, "Advertencia", "¿Esta seguro de salir del formulario?", "Si", "No");
    switch(ret){
    case 0:{        
        if(widget_previous){
            op = SALIR;
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
void CompraBoleta::on_dateEdit_declaracion_dateChanged(const QDate &date)
{
    if(date.month() > ui->dateTimeEdit_emision->date().month()){
        //QMessageBox::warning(this, "Advertencia", "El mes de declaración no puede ser mayor\nque la fecha de emisión.", "Ok");

        //disconnect(ui->dateEdit_declaracion, SIGNAL(dateChanged(QDate)), this, SLOT(on_dateEdit_declaracion_dateChanged(QDate)));
        //QDate date_temp;
        //date_temp.setDate(date.year(), mes_declaracion, date.day());
        //ui->dateEdit_declaracion->setDate(date_temp);
        //connect(ui->dateEdit_declaracion, SIGNAL(dateChanged(QDate)), this, SLOT(on_dateEdit_declaracion_dateChanged(QDate)));
    }else{
        //mes_declaracion = date.month();
    }
}

void CompraBoleta::on_dateTimeEdit_emision_dateChanged(const QDate &date)
{
    if(date.month() < ui->dateEdit_declaracion->date().month()){
        //QMessageBox::warning(this, "Advertencia", "El mes de declaración no puede ser mayor\nque la fecha de emisión.", "Ok");

        //disconnect(ui->dateTimeEdit_emision, SIGNAL(dateChanged(QDate)), this, SLOT(on_dateTimeEdit_emision_dateChanged(QDate)));
        //QDate date_temp;
        //date_temp.setDate(date.year(), mes_emision, date.day());
        //ui->dateTimeEdit_emision->setDate(date_temp);
        //connect(ui->dateTimeEdit_emision, SIGNAL(dateChanged(QDate)), this, SLOT(on_dateTimeEdit_emision_dateChanged(QDate)));
    }else{
        //mes_emision = date.month();
    }
}

void CompraBoleta::showEvent(QShowEvent *se)
{
    se->accept();

    afterShow = true;
}
void CompraBoleta::hideEvent(QHideEvent *he)
{
    emit hiding();
    he->accept();
}
void CompraBoleta::closeEvent(QCloseEvent *ce)
{
    emit closing();
    ce->accept();
}
bool CompraBoleta::eventFilter(QObject *obj, QEvent *e)
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
                ui->dateTimeEdit_emision->setFocus(Qt::TabFocusReason);
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
            case Qt::Key_Enter:{
                QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QApplication::sendEvent(w_temp, key);
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
                    //ui->pushButton_guardar->setFocus(Qt::TabFocusReason);
                }
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
    w_temp = ui->comboBox_serie_numero_orden;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;
            switch(KeyEvent->key())
            {
            case Qt::Key_Return:{
                ui->pushButton_buscar_orden->setFocus(Qt::TabFocusReason);
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
    w_temp = ui->pushButton_buscar_orden;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;
            switch(KeyEvent->key())
            {
            case Qt::Key_Return:{
                ui->pushButton_buscar_orden->click();
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
    w_temp = ui->pushButton_jalar_orden;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;
            switch(KeyEvent->key())
            {
            case Qt::Key_Return:{
                ui->pushButton_jalar_orden->click();
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
    w_temp = ui->pushButton_quitar_orden;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;
            switch(KeyEvent->key())
            {
            case Qt::Key_Return:{
                ui->pushButton_quitar_orden->click();
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
    w_temp = ui->comboBox_serie_numero_guia;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;
            switch(KeyEvent->key())
            {
            case Qt::Key_Return:{
                ui->pushButton_buscar_guia->setFocus(Qt::TabFocusReason);
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
    w_temp = ui->pushButton_buscar_guia;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;
            switch(KeyEvent->key())
            {
            case Qt::Key_Return:{
                ui->pushButton_buscar_guia->click();
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
    w_temp = ui->pushButton_jalar_guia;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;
            switch(KeyEvent->key())
            {
            case Qt::Key_Return:{
                ui->pushButton_jalar_guia->click();
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
    w_temp = ui->pushButton_quitar_guia;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;
            switch(KeyEvent->key())
            {            
            case Qt::Key_Return:{
                ui->pushButton_quitar_orden->click();
                return true;
            }break;
            case Qt::Key_Enter:{
                QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QApplication::sendEvent(w_temp, key);
                return true;
            }break;
            }
        }
    }

    return eventFilter(obj, e);
}

void CompraBoleta::on_lineEdit_cod_textEdited(const QString &arg1)
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

void CompraBoleta::on_pushButton_amarres_clicked()
{    
    CompraAmarres* w_compra_amarres = new CompraAmarres;
    w_compra_amarres->set_widget_previous(this);
    w_compra_amarres->set_documento(this->id, tipo_documento::BOLETA);

    SYSTEM->change_center_w(this, w_compra_amarres);
}
void CompraBoleta::on_orden_buscar_closing()
{
    CompraBuscar* w = (CompraBuscar*)sender();

    if(w->getID().compare("")==0)return;

    QString id = w->getID();
    QString sn = w->getSerie() + " - " + w->getNumero();

    ui->comboBox_id_orden->addItem(id);
    ui->comboBox_serie_numero_orden->addItem(sn);
}

void CompraBoleta::on_guia_buscar_closing()
{
    CompraBuscar* w = (CompraBuscar*)sender();

    if(w->getID().compare("")==0)return;

    QString id = w->getID();
    QString sn = w->getSerie() + " - " + w->getNumero();

    ui->comboBox_id_guia->addItem(id);
    ui->comboBox_serie_numero_guia->addItem(sn);
}

void CompraBoleta::on_pushButton_buscar_orden_clicked()
{
    if(persona_id.compare("") == 0){
        QMessageBox::warning(this, "Advertencia", "No ha ingresado el proveedor.", "Ok");
        return;
    }

    CompraBuscar* w = new CompraBuscar;
    w->set_widget_previous(this);
    w->setTipoOrden();
    w->hideOptBoleta();
    w->hideOptFactura();
    w->hideOptFlete();
    w->hideOptGuiaRR();
    w->hideOptNotaCredito();
    w->hideOptNotaDebito();
    w->hideOptRegSinDoc();
    w->hideOptSaldo();

    connect(w, SIGNAL(closing()), this, SLOT(on_orden_buscar_closing()));
    w->set_ruc(ui->lineEdit_cod->text());

    SYSTEM->change_center_w(this, w);
}

void CompraBoleta::on_pushButton_buscar_guia_clicked()
{
    if(persona_id.compare("") == 0){
        QMessageBox::warning(this, "Advertencia", "No ha ingresado el proveedor.", "Ok");
        return;
    }

    CompraBuscar* w = new CompraBuscar;
    w->set_widget_previous(this);
    w->setTipoGuiaRR();
    w->hideOptBoleta();
    w->hideOptFactura();
    w->hideOptFlete();
    w->hideOptNotaCredito();
    w->hideOptNotaDebito();
    w->hideOptOrden();
    w->hideOptRegSinDoc();
    w->hideOptSaldo();
    connect(w, SIGNAL(closing()), this, SLOT(on_guia_buscar_closing()));
    w->set_ruc(ui->lineEdit_cod->text());

    SYSTEM->change_center_w(this, w);
}

void CompraBoleta::on_pushButton_jalar_orden_clicked()
{
    if(ui->comboBox_serie_numero_orden->currentText().compare("") == 0){
        return;
    }else{
    }
    QString sn = ui->comboBox_serie_numero_orden->currentText();
    QStringList list = sn.split(QRegExp("\\s|-"), QString::SkipEmptyParts);
    QString serie = list[0];
    QString numero = list[1];
    if (serie.length() == A_Compra::SERIE_N_DIGITS && numero.length() == A_Compra::NUMERO_N_DIGITS) {
        ui->comboBox_id_orden->setCurrentIndex(ui->comboBox_serie_numero_orden->currentIndex());
        QString id = ui->comboBox_id_orden->currentText();
        QString str_query = "";
        str_query += "SELECT producto.id, documento_h_producto.cantidad, unidad.unidad";
        str_query += ", concat(producto.descripcion, ' ', marca.marca), documento_h_producto.precio";
        str_query += " FROM orden";
        str_query += " JOIN documento ON documento.id = orden.comprobante_documento_id";
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

void CompraBoleta::on_pushButton_jalar_guia_clicked()
{
    if(ui->comboBox_serie_numero_guia->currentText().compare("") == 0){
        return;
    }else{
    }
    QString sn = ui->comboBox_serie_numero_guia->currentText();
    QStringList list = sn.split(QRegExp("\\s|-"), QString::SkipEmptyParts);
    QString serie = list[0];
    QString numero = list[1];
    if (serie.length() == A_Compra::SERIE_N_DIGITS && numero.length() == A_Compra::NUMERO_N_DIGITS) {
        ui->comboBox_id_guia->setCurrentIndex(ui->comboBox_serie_numero_guia->currentIndex());
        QString id = ui->comboBox_id_guia->currentText();
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
                QString p_total = query.value(4).toString();

                set_producto(producto_id, cantidad, unidad, descripcion, p_total);
            }
        }else{

        }
    }
}

void CompraBoleta::on_pushButton_borrar_clicked()
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
    }

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
}

void CompraBoleta::on_pushButton_down_clicked()
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

void CompraBoleta::on_pushButton_up_clicked()
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

void CompraBoleta::on_pushButton_quitar_orden_clicked()
{
    if(ui->comboBox_serie_numero_orden->currentIndex() == -1)
        return;
    int ret = QMessageBox::warning(this, "Advertencia", "¿Esta seguro de quitar el documento?", "Si", "No");
    switch(ret){
    case 0:{
        ui->comboBox_id_orden->removeItem(ui->comboBox_serie_numero_orden->currentIndex());
        ui->comboBox_serie_numero_orden->removeItem(ui->comboBox_serie_numero_orden->currentIndex());
    }break;
    case 1:{

    }break;
    }
}

void CompraBoleta::on_pushButton_quitar_guia_clicked()
{
    if(ui->comboBox_serie_numero_guia->currentIndex() == -1)
        return;
    int ret = QMessageBox::warning(this, "Advertencia", "¿Esta seguro de quitar el documento?", "Si", "No");
    switch(ret){
    case 0:{
        ui->comboBox_id_guia->removeItem(ui->comboBox_serie_numero_guia->currentIndex());
        ui->comboBox_serie_numero_guia->removeItem(ui->comboBox_serie_numero_guia->currentIndex());
    }break;
    case 1:{

    }break;
    }
}

void CompraBoleta::on_pushButton_canjear_clicked()
{

}
