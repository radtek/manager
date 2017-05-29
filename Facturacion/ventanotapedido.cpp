#include "ventanotapedido.h"
#include "ui_ventanotapedido.h"

VentaNotaPedido::VentaNotaPedido(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VentaNotaPedido)
{
    ui->setupUi(this);

    afterShow = false;
    op = -1;

    anulado = 0;
    QRegExp regExp_ruc("[0-9]{11,11}");
    ui->lineEdit_codigo->setValidator(new QRegExpValidator(regExp_ruc));

    // DISABLES

    //ui->lineEdit_cod->setReadOnly(true);
    ui->lineEdit_nombre->setReadOnly(true);

    // SET DATA
    disconnect(ui->dateEdit_emision, SIGNAL(dateChanged(QDate)), this, SLOT(on_dateEdit_emision_dateChanged(QDate)));
    ui->dateEdit_emision->setDate(QDate::currentDate());
    connect(ui->dateEdit_emision, SIGNAL(dateChanged(QDate)), this, SLOT(on_dateEdit_emision_dateChanged(QDate)));
    ui->dateTimeEdit_sistema->setDateTime(QDateTime::currentDateTime());

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(set_time()));
    timer->start(1000);

    // SET SERIE Y NUMERO
    A_Venta::set_serie_numero(ui->lineEdit_serie, ui->lineEdit_numero);

    LineEditDelegate* lineEditDelegate = new LineEditDelegate;
    ui->tableWidget->setItemDelegateForColumn(INDEX_DESCRIPCION, lineEditDelegate);
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

    ui->tableWidget->hideColumn(INDEX_ID);

    ui->comboBox_id_cotizacion->hide();

    this->installEventFilter(this);
    ui->dateEdit_emision->installEventFilter(this);
    ui->dateTimeEdit_sistema->installEventFilter(this);
    ui->lineEdit_serie->installEventFilter(this);
    ui->lineEdit_numero->installEventFilter(this);
    ui->radioButton_tipo->installEventFilter(this);
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
    ui->pushButton_anular->installEventFilter(this);
    ui->pushButton_imprimir->installEventFilter(this);
    ui->pushButton_eliminar->installEventFilter(this);
    ui->pushButton_guardar->installEventFilter(this);
    ui->pushButton_salir->installEventFilter(this);
}

VentaNotaPedido::~VentaNotaPedido()
{
    qDebug()<<"delete venta nota pedido"<<endl;
    delete ui;
}

void VentaNotaPedido::set_widget_previous(QWidget *widget_previous)
{
    this->widget_previous = widget_previous;
}
void VentaNotaPedido::on_ventaSNSelector_closing()
{
    VentaSNSelector* w = (VentaSNSelector*)sender();

    if(w->getSerie().compare("") == 0)return;

    ui->lineEdit_serie->setText(w->getSerie());
    ui->lineEdit_numero->setText(w->getNumero());
}

void VentaNotaPedido::next_serie_numero()
{
    VentaSNSelector* w = new VentaSNSelector;
    w->set_widget_previous(this);
    w->set_tipo(venta_items::NOTA_PEDIDO);

    connect(w, SIGNAL(closing()), this, SLOT(on_ventaSNSelector_closing()));

    SYSTEM->change_center_w(this, w);
}
bool VentaNotaPedido::select(QString id
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

    str_query += "(SELECT COUNT(*), '', '', '', '' FROM documento_h_documento";
    str_query += " JOIN cotizacion ON documento_h_documento.documento_id1 = cotizacion.comprobante_documento_id";
    str_query += " WHERE documento_h_documento.documento_id = "+id+")";
    str_query += " UNION ALL";
    str_query += "(SELECT documento_h_documento.documento_id1, anexo.serie, anexo.numero, '', '' FROM documento_h_documento";
    str_query += " JOIN cotizacion ON documento_h_documento.documento_id1 = cotizacion.comprobante_documento_id";
    str_query += " JOIN anexo ON documento_h_documento.documento_id1 = anexo.documento_id";
    str_query += " WHERE documento_h_documento.documento_id = "+id+")";
    str_query += " UNION ALL";
    str_query += "(SELECT nota_pedido.anulado, '', '', '', '' FROM nota_pedido";
    str_query += " WHERE nota_pedido.comprobante_documento_id = "+id+")";
    str_query += " UNION ALL";
    str_query += "(SELECT producto.id, d_h_prod.cantidad, unidad.unidad";
    //str_query += ", concat(producto.descripcion,' ',IF(marca.marca IS NULL, '', concat(' ', marca.marca))), d_h_prod.precio FROM documento";
    str_query += ", producto.descripcion, d_h_prod.precio FROM documento";
    str_query += " JOIN documento_h_producto d_h_prod ON documento.id = d_h_prod.documento_id";
    str_query += " JOIN producto ON producto.id = d_h_prod.producto_id";
    str_query += " LEFT JOIN marca ON marca.id = producto.marca_id";
    str_query += " LEFT JOIN unidad ON unidad.id = producto.unidad_id";
    str_query += " WHERE documento.id = "+id+" GROUP BY d_h_prod.id)";

    QSqlQuery query;

    qDebug()<<str_query<<endl;
    if(query.exec(str_query)){
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
        QDate date = QDate::fromString(fecha_emision, "dd-MM-yyyy");

        ui->dateEdit_emision->setDate(date);

        ui->lineEdit_serie->setText(serie);
        ui->lineEdit_numero->setText(numero);


        // PROVEEDOR
        ui->lineEdit_codigo->setText(codigo);
        ui->lineEdit_nombre->setText(nombre);
        ui->lineEdit_direccion->setText(direccion);

        query.next();
        anulado = query.value(0).toInt();
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
void VentaNotaPedido::set_cliente(QString persona_id, QString codigo, QString nombre, QString direccion)
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

void VentaNotaPedido::set_producto(QString producto_id
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

    ui->lineEdit_total->setText(QString().setNum(total, ' ', DECIMALS_PRECIO_TOTAL));

    SYSTEM->table_resize_to_contents(0, ui->tableWidget);

    connect(ui->tableWidget, SIGNAL(itemChanged(QTableWidgetItem*))
               , this, SLOT(on_tableWidget_itemChanged(QTableWidgetItem*)));
}

void VentaNotaPedido::set_data(QString persona_id, QString tipo_persona_id
              , QDateTime dt_emision, QDateTime dt_sistema, QString serie, QString numero
              , QString codigo, QString nombre, QString direccion
              , QVector<QVector<QString>> productos)
{
    this->tipo_persona_id = tipo_persona_id;
    this->persona_id = persona_id;
    ui->lineEdit_codigo->setText(codigo);
    ui->lineEdit_nombre->setText(nombre);
    ui->lineEdit_direccion->setText(direccion);

    for(int i=0; i<productos.size(); i++){
        QString id = productos[i][0];
        QString cantidad = productos[i][1];
        QString unidad = productos[i][2];
        QString descripcion = productos[i][3];
        QString p_total = productos[i][4];

        set_producto(id, cantidad, unidad, descripcion, p_total);
    }
}
void VentaNotaPedido::set_data(int tipo, QString id, QString persona_id, QString tipo_persona_id
                           , QString fecha_emision, QString fecha_sistema, QString serie, QString numero
                           , QString codigo, QString nombre, QString direccion
                           , QVector<QVector<QString>> productos)
{
    switch(tipo){
    case venta_items::COTIZACION:{
        ui->comboBox_id_cotizacion->addItem(id);
        ui->comboBox_serie_numero_cotizacion->addItem(serie+" - "+numero);
    }break;
    }
    this->tipo_persona_id = tipo_persona_id;
    this->persona_id = persona_id;
    ui->lineEdit_codigo->setText(codigo);
    ui->lineEdit_nombre->setText(nombre);
    ui->lineEdit_direccion->setText(direccion);

    for(int i=0; i<productos.size(); i++){
        QString id = productos[i][0];
        QString cantidad = productos[i][1];
        QString unidad = productos[i][2];
        QString descripcion = productos[i][3];
        QString p_total = productos[i][4];

        set_producto(id, cantidad, unidad, descripcion, p_total);
    }
}

bool VentaNotaPedido::guardar()
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

    ui->dateEdit_emision->setDisplayFormat("yyyy-MM-dd hh:mm:ss");
    ui->dateTimeEdit_sistema->setDisplayFormat("yyyy-MM-dd hh:mm:ss");

    if (id.compare("") == 0) {
        // DOCUMENTO
        str_query += "INSERT INTO documento(tipo_documento_id, habilitado)VALUES(";
        str_query += QString().setNum(tipo_documento::NOTA_PEDIDO);
        str_query += ", 1)";
        str_query += "&&END_QUERY&&";

        // COMPROBANTE
        str_query +=  "INSERT INTO comprobante(documento_id";
        str_query += ", operacion_id)VALUES(";
        str_query += "(SELECT MAX(documento.id) FROM documento)";
        str_query += ", "+QString().setNum(operacion_items::VENTA)+")";
        str_query += "&&END_QUERY&&";

        // ANEXO
        str_query +=  "INSERT INTO anexo(documento_id";
        str_query += ", fecha_emision, fecha_sistema, serie, numero)VALUES(";
        str_query += "(SELECT MAX(documento.id) FROM documento)";
        str_query += ", '"+ui->dateEdit_emision->date().toString("yyyy-MM-dd")+"'";
        str_query += ", '"+ui->dateTimeEdit_sistema->dateTime().toString("yyyy-MM-dd hh:mm:ss")+"'";
        str_query += ", '"+ui->lineEdit_serie->text()+"'";
        str_query += ", '"+ui->lineEdit_numero->text()+"')";
        str_query += "&&END_QUERY&&";

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

        // NOTA PEDIDO
        str_query += "INSERT INTO nota_pedido(comprobante_documento_id";
        str_query += ", anulado";
        str_query += ")VALUES(";
        str_query += "(SELECT MAX(documento.id) FROM documento)";
        str_query += ", '"+QString().setNum(anulado)+"')";
        str_query += "&&END_QUERY&&";

        // DOCUMENTO_H_DOCUMENTO COTIZACION
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

        str_query += "UPDATE nota_pedido SET";
        str_query += " anulado = '"+QString().setNum(anulado)+"'";
        str_query += " WHERE comprobante_documento_id = "+id;
        str_query += "&&END_QUERY&&";

        // DOCUMENTO_H_DOCUMENTO COTIZACION
        str_query += "DELETE FROM documento_h_documento WHERE documento_id = "+id;
        str_query += "&&END_QUERY&&";

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
bool VentaNotaPedido::remove()
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

void VentaNotaPedido::set_time()
{
    ui->dateTimeEdit_sistema->setDateTime(QDateTime::currentDateTime());
}

void VentaNotaPedido::on_dateTimeEdit_emision_dateChanged(const QDate &date)
{

}
void VentaNotaPedido::on_cliente_closing()
{
    ColaboradorBuscar* w = (ColaboradorBuscar*)sender();
    QString persona_id = w->get_ID();
    if(persona_id.compare("") == 0) {
        return;
    }
    if(ui->radioButton_tipo->isChecked()){
        tipo_persona_id = QString().setNum(tipo_persona::CLIENTE_RUC);
    }else{
        tipo_persona_id = QString().setNum(tipo_persona::CLIENTE_DNI);
    }
    this->persona_id = persona_id;
    codigo = w->get_codigo();
    nombre = w->get_nombre();
    direccion = w->get_direccion();

    set_cliente(persona_id, codigo, nombre, direccion);

    ui->pushButton_ing_prod->setFocus(Qt::TabFocusReason);
}
void VentaNotaPedido::on_producto_closing()
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
void VentaNotaPedido::on_pushButton_cliente_clicked()
{
    VentaCliente* w = new VentaCliente;
    w->set_widget_previous(this);

    w->hideOptProveedor();
    w->hideOptTransportista();

    w->setTipoClienteDNI();
    w->setTipoClienteRUC();

    w->hideOptUsuario();

    if(ui->radioButton_tipo->isChecked()){
        w->set_tipo(persona_items::CLIENTE_RUC);
    }else{
        w->set_tipo(persona_items::CLIENTE_DNI);
    }
    connect(w, SIGNAL(closing()), this, SLOT(on_cliente_closing()));

    SYSTEM->change_center_w(this, w);
}

void VentaNotaPedido::on_pushButton_ing_prod_clicked()
{
    VentaProducto* w = new VentaProducto;
    w->set_widget_previous(this);
    connect(w, SIGNAL(closing()), this, SLOT(on_producto_closing()));

    SYSTEM->change_center_w(this, w);
}

void VentaNotaPedido::on_pushButton_guardar_clicked()
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
        QString str_query = "(SELECT IF(EXISTS(SELECT 1 FROM nota_pedido";
        str_query += " JOIN anexo ON anexo.documento_id = nota_pedido.comprobante_documento_id";
        str_query += " JOIN comprobante ON (comprobante.operacion_id = "+QString().setNum(operacion_items::VENTA);
        str_query += " AND comprobante.documento_id = nota_pedido.comprobante_documento_id)";
        str_query += " WHERE anexo.serie = '"+ui->lineEdit_serie->text()+"'";
        str_query += " AND anexo.numero = '"+ui->lineEdit_numero->text()+"'";
        str_query += " ORDER BY anexo.serie, anexo.numero), 1 , 0), '')";
        str_query += " UNION ALL";
        str_query += "(SELECT anexo.serie, anexo.numero+1 FROM nota_pedido";
        str_query += " JOIN anexo ON anexo.documento_id = nota_pedido.comprobante_documento_id";
        str_query += " ORDER BY nota_pedido.comprobante_documento_id DESC, anexo.numero DESC LIMIT 1)";

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
                        w->set_tipo(venta_items::NOTA_PEDIDO);

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
            QMessageBox::information(this, "Información", "Se guardo con éxito.");
            this->setAttribute(Qt::WA_DeleteOnClose);
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

void VentaNotaPedido::on_pushButton_eliminar_clicked()
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

void VentaNotaPedido::on_pushButton_imprimir_clicked()
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
        str_query += " AND series.tipo_documento_id = "+QString().setNum(tipo_documento::NOTA_PEDIDO);

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
                str_query += ", "+QString().setNum(tipo_documento::NOTA_PEDIDO);
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

    w->set_tipo_documento(tipo_documento::NOTA_PEDIDO, ui->lineEdit_serie->text(), id_series
                          , labels, width, height, margen_fila, margen_columna);

    SYSTEM->change_center_w(this, w);
}

void VentaNotaPedido::on_pushButton_amarres_clicked()
{

}

void VentaNotaPedido::on_pushButton_salir_clicked()
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
void VentaNotaPedido::showEvent(QShowEvent *se)
{
    emit showing();

    se->accept();

    afterShow = true;
}
void VentaNotaPedido::hideEvent(QHideEvent *he)
{
    emit hiding();
    he->accept();
}
void VentaNotaPedido::closeEvent(QCloseEvent *ce)
{
    emit closing();
    ce->accept();
}
bool VentaNotaPedido::eventFilter(QObject *obj, QEvent *e)
{
    QWidget* w_temp;
    w_temp = this;
    if(obj == w_temp){
        if(e->type() == QEvent::MouseButtonPress){
            if(focusWidget()){
                focusWidget()->setFocus();
            }else{
                ui->dateEdit_emision->setFocus();
                ui->dateEdit_emision->setCurrentSectionIndex(ui->dateEdit_emision->currentSectionIndex());
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
                    ui->dateEdit_emision->setFocus();
                    ui->dateEdit_emision->setCurrentSectionIndex(ui->dateEdit_emision->currentSectionIndex());
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
                ui->radioButton_tipo->setFocus(Qt::TabFocusReason);
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

    w_temp = ui->radioButton_tipo;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;
            switch(KeyEvent->key())
            {
            case Qt::Key_Return:{
                ui->pushButton_cliente->setFocus(Qt::TabFocusReason);
                return true;
            }break;
            }
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
void VentaNotaPedido::on_pushButton_up_clicked()
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

void VentaNotaPedido::on_pushButton_down_clicked()
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

void VentaNotaPedido::on_pushButton_borrar_clicked()
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

void VentaNotaPedido::on_lineEdit_serie_returnPressed()
{



}

void VentaNotaPedido::on_tableWidget_itemChanged(QTableWidgetItem *item)
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
    ui->lineEdit_total->setText(QString().setNum(total, ' ', DECIMALS_PRECIO_TOTAL));

    connect(ui->tableWidget, SIGNAL(itemChanged(QTableWidgetItem*))
               , this, SLOT(on_tableWidget_itemChanged(QTableWidgetItem*)));
}
void VentaNotaPedido::on_cotizacion_buscar_closing()
{
    VentaBuscar* w = (VentaBuscar*)sender();

    if(w->getID().compare("")==0)return;

    QString id = w->getID();
    QString sn = w->getSerie() + " - " + w->getNumero();

    ui->comboBox_id_cotizacion->addItem(id);
    ui->comboBox_serie_numero_cotizacion->addItem(sn);
}

void VentaNotaPedido::on_pushButton_buscar_cotizacion_clicked()
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

void VentaNotaPedido::on_pushButton_jalar_cotizacion_clicked()
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

void VentaNotaPedido::on_pushButton_quitar_cotizacion_clicked()
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

void VentaNotaPedido::on_pushButton_anular_clicked()
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
    QString str_query = "UPDATE nota_pedido SET";
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
