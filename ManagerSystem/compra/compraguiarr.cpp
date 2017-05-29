#include "compraguiarr.h"
#include "ui_compraguiarr.h"

CompraGuiaRR::CompraGuiaRR(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CompraGuiaRR)
{
    ui->setupUi(this);

    afterShow = false;

    widget_previous = NULL;

    op = -1;

    pos_dame_productos = -1;

    QRegExp regExp_ruc("[0-9]{11,11}");
    ui->lineEdit_cod->setValidator(new QRegExpValidator(regExp_ruc));

    // DISABLES

    //ui->lineEdit_cod->setReadOnly(true);
    ui->lineEdit_nombre->setReadOnly(true);

    // SET DATA
    ui->dateTimeEdit_emision->setDateTime(QDateTime::currentDateTime());
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

    ui->tableWidget->hideColumn(INDEX_ID);

    ui->comboBox_id_boleta->hide();
    ui->comboBox_id_factura->hide();

    // INSTALL EVENTS
    this->installEventFilter(this);
    ui->dateTimeEdit_emision->installEventFilter(this);
    ui->lineEdit_serie->installEventFilter(this);
    ui->lineEdit_numero->installEventFilter(this);
    ui->pushButton_proveedor->installEventFilter(this);
    ui->pushButton_ing_prod->installEventFilter(this);
    ui->tableWidget->installEventFilter(this);
    ui->pushButton_guardar->installEventFilter(this);
    ui->pushButton_eliminar->installEventFilter(this);
    ui->pushButton_salir->installEventFilter(this);   
}

CompraGuiaRR::~CompraGuiaRR()
{
    qDebug()<<"delete compra guiarr"<<endl;
    delete ui;
}
QString CompraGuiaRR::getID()
{
    return id;
}
QString CompraGuiaRR::getPersonaID()
{
    return persona_id;
}
QString CompraGuiaRR::getFechaEmision()
{
    return ui->dateTimeEdit_emision->date().toString("dd-MM-yyyy");
}
QString CompraGuiaRR::getSerie()
{
    return ui->lineEdit_serie->text();
}
QString CompraGuiaRR::getNumero()
{
    return ui->lineEdit_numero->text();
}
QString CompraGuiaRR::getCodigo()
{
    return ui->lineEdit_cod->text();
}
QString CompraGuiaRR::getNombre()
{
    return ui->lineEdit_nombre->text();
}
QVector<QVector<QString> > CompraGuiaRR::getFacturas()
{
    for(int i=0; i<ui->comboBox_id_factura->count(); i++){
        facturas.push_back(QVector<QString>());
        facturas[i].push_back(ui->comboBox_id_factura->itemText(i));
        facturas[i].push_back(ui->comboBox_serie_numero_factura->itemText(i));
    }
    return facturas;
}
QVector<QVector<QString> > CompraGuiaRR::getBoletas()
{
    for(int i=0; i<ui->comboBox_id_boleta->count(); i++){
        boletas.push_back(QVector<QString>());
        boletas[i].push_back(ui->comboBox_id_boleta->itemText(i));
        boletas[i].push_back(ui->comboBox_serie_numero_boleta->itemText(i));
    }
    return boletas;
}
void CompraGuiaRR::set_widget_previous(QWidget *w)
{
    this->widget_previous = w;
}
bool CompraGuiaRR::select(QString id
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

    str_query = "(SELECT COUNT(*), '', '', '', '', '' FROM documento_h_documento";
    str_query += " JOIN factura ON documento_h_documento.documento_id1 = factura.comprobante_documento_id";
    str_query += " WHERE documento_h_documento.documento_id = "+id+")";
    str_query += " UNION ALL";
    str_query += "(SELECT documento_h_documento.documento_id1, anexo.serie, anexo.numero, juridica.persona_id, juridica.ruc, juridica.razon_social FROM documento_h_documento";
    str_query += " JOIN factura ON documento_h_documento.documento_id1 = factura.comprobante_documento_id";
    str_query += " JOIN anexo ON documento_h_documento.documento_id1 = anexo.documento_id";
    str_query += " JOIN documento_h_persona ON documento_h_persona.documento_id = factura.comprobante_documento_id";
    str_query += " JOIN juridica ON juridica.persona_id = documento_h_persona.persona_id";
    str_query += " WHERE documento_h_documento.documento_id = "+id+")";
    str_query += " UNION ALL";
    str_query += "(SELECT COUNT(*), '', '', '', '', '' FROM documento_h_documento";
    str_query += " JOIN boleta ON documento_h_documento.documento_id1 = boleta.comprobante_documento_id";
    str_query += " WHERE documento_h_documento.documento_id = "+id+")";
    str_query += " UNION ALL";
    str_query += "(SELECT documento_h_documento.documento_id1, anexo.serie, anexo.numero, juridica.persona_id, juridica.ruc, juridica.razon_social FROM documento_h_documento";
    str_query += " JOIN boleta ON documento_h_documento.documento_id1 = boleta.comprobante_documento_id";
    str_query += " JOIN anexo ON documento_h_documento.documento_id1 = anexo.documento_id";
    str_query += " JOIN documento_h_persona ON documento_h_persona.documento_id = boleta.comprobante_documento_id";
    str_query += " JOIN juridica ON juridica.persona_id = documento_h_persona.persona_id";
    str_query += " WHERE documento_h_documento.documento_id = "+id+")";
    str_query += " UNION ALL";
    str_query += "(SELECT producto.id, d_h_prod.cantidad, unidad.unidad";
    //str_query += ", concat(producto.descripcion,' ',IF(marca.marca IS NULL, '', concat(' ', marca.marca))), d_h_prod.precio, '' FROM documento";
    str_query += ", producto.descripcion, d_h_prod.precio, '' FROM documento";
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
        QDate date = QDate::fromString(fecha_emision, "dd-MM-yyyy");
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

void CompraGuiaRR::set_proveedor(QString persona_id, QString codigo, QString nombre)
{
    /*
    this->persona_id = persona_id;
    this->codigo = codigo;
    this->nombre = nombre;
    */

    ui->lineEdit_cod->setText(codigo);
    ui->lineEdit_nombre->setText(nombre);
}

void CompraGuiaRR::set_producto(QString producto_id
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

    ui->tableWidget->item(rowCount, INDEX_ID)->setFlags(Qt::ItemIsEnabled
                                                 | Qt::ItemIsSelectable);

    ui->tableWidget->item(rowCount, INDEX_CANTIDAD)->setFlags(Qt::ItemIsEnabled
                                                 | Qt::ItemIsSelectable
                                                 | Qt::ItemIsEditable);

    ui->tableWidget->item(rowCount, INDEX_UNIDAD)->setFlags(Qt::ItemIsEnabled
                                                 | Qt::ItemIsSelectable);

    ui->tableWidget->item(rowCount, INDEX_DESCRIPCION)->setFlags(Qt::ItemIsEnabled
                                                 | Qt::ItemIsSelectable);

    SYSTEM->table_resize_to_contents(0, ui->tableWidget);

    connect(ui->tableWidget, SIGNAL(itemChanged(QTableWidgetItem*))
               , this, SLOT(on_tableWidget_itemChanged(QTableWidgetItem*)));
}

bool CompraGuiaRR::guardar()
{
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
        str_query += QString().setNum(tipo_documento::GUIA_REMISION_REMITENTE);
        str_query += ", 1)";
        str_query += "&&END_QUERY&&";

        // COMPROBANTE
        str_query +=  "INSERT INTO comprobante(documento_id";
        //str_query += ", igv_id, igv_moneda_id, tipo_cambio_id, tipo_cambio_moneda_id";
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
            QString total = "0.0";

            str_query_2 += QString()+",((SELECT MAX(documento.id) FROM documento), "+producto_id+", "+cantidad+", "+total+")";
        }

        str_query_2.remove(0,1);
        str_query += str_query_2+"&&END_QUERY&&";

        // GUIA REMISION REMITENTE
        str_query += "INSERT INTO guia_remision_remitente(comprobante_documento_id)VALUES(";
        str_query += "(SELECT MAX(documento.id) FROM documento)";
        str_query += ")";
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
            QString total = "0.0";

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
bool CompraGuiaRR::remove()
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
void CompraGuiaRR::set_time()
{
    ui->dateTimeEdit_sistema->setDateTime(QDateTime::currentDateTime());
}
void CompraGuiaRR::on_proveedor_closing()
{
    PersonaBuscar* w = (PersonaBuscar*)sender();
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
void CompraGuiaRR::on_producto_closing()
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
    QString p_total = "0.000";

    set_producto(producto_id, cantidad, unidad, descripcion, p_total);
}

void CompraGuiaRR::on_pushButton_proveedor_clicked()
{
    CompraProveedor* w = new CompraProveedor;
    w->setTipoProveedor();
    
    w->set_widget_previous(this);
    connect(w, SIGNAL(closing()), this, SLOT(on_proveedor_closing()));
    SYSTEM->change_center_w(this, w);
}

void CompraGuiaRR::on_pushButton_ing_prod_clicked()
{
    CompraProducto* w= new CompraProducto;
    w->set_widget_previous(this);
    connect(w, SIGNAL(closing()), this, SLOT(on_producto_closing()));
    SYSTEM->change_center_w(this, w);
}

void CompraGuiaRR::on_tableWidget_itemChanged(QTableWidgetItem *item)
{
    qDebug()<<"item changed"<<endl;
    disconnect(ui->tableWidget, SIGNAL(itemChanged(QTableWidgetItem*))
               , this, SLOT(on_tableWidget_itemChanged(QTableWidgetItem*)));



    connect(ui->tableWidget, SIGNAL(itemChanged(QTableWidgetItem*))
               , this, SLOT(on_tableWidget_itemChanged(QTableWidgetItem*)));

    qDebug()<<"end changed"<<endl;
}

void CompraGuiaRR::on_pushButton_guardar_clicked()
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
        return;
    }break;
    case 1:{

    }
    }
}

void CompraGuiaRR::on_pushButton_eliminar_clicked()
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
void CompraGuiaRR::on_pushButton_salir_clicked()
{
    int ret = QMessageBox::warning(this, "Advertencia", "¿Esta seguro de salir del formulario?", "Si", "No");
    switch(ret){
    case 0:{        
        if(widget_previous){
            op = SALIR;
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
void CompraGuiaRR::showEvent(QShowEvent *se)
{
    se->accept();

    afterShow = true;
}
void CompraGuiaRR::closeEvent(QCloseEvent *ce)
{
    emit closing();
    ce->accept();
}

bool CompraGuiaRR::eventFilter(QObject *obj, QEvent *e)
{    
    QWidget* w_temp;
    w_temp = this;
    if(obj == w_temp){
        if(e->type() == QEvent::MouseButtonPress){
            if(focusWidget()){
                focusWidget()->setFocus();
            }else{
                ui->dateTimeEdit_emision->setFocus();
                ui->dateTimeEdit_emision->setCurrentSectionIndex(ui->dateTimeEdit_emision->currentSectionIndex());
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
                    ui->dateTimeEdit_emision->setFocus();
                    ui->dateTimeEdit_emision->setCurrentSectionIndex(ui->dateTimeEdit_emision->currentSectionIndex());
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
                    return true;
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
    return eventFilter(obj, e);
}

void CompraGuiaRR::on_pushButton_down_clicked()
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

void CompraGuiaRR::on_pushButton_up_clicked()
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

void CompraGuiaRR::on_pushButton_borrar_clicked()
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
}

void CompraGuiaRR::on_pushButton_amarres_clicked()
{
    if(id.compare("") == 0) {
        QMessageBox::warning(this, "Advertencia", "No existe documento. Debe guardarlo primero.", "Ok");
        return;
    }

    CompraAmarres* w_compra_amarres = new CompraAmarres;
    w_compra_amarres->set_widget_previous(this);
    w_compra_amarres->set_documento(this->id, tipo_documento::GUIA_REMISION_REMITENTE);

    SYSTEM->change_center_w(this, w_compra_amarres);
}
void CompraGuiaRR::on_factura_buscar_closing()
{
    CompraBuscar* w = (CompraBuscar*)sender();

    if(w->getID().compare("")==0)return;

    QString id = w->getID();
    QString sn = w->getCodigo() + " - " + w->getNombre() + " - " + w->getSerie() + " - " + w->getNumero();

    ui->comboBox_id_factura->addItem(id);
    ui->comboBox_serie_numero_factura->addItem(sn);
}
void CompraGuiaRR::on_boleta_buscar_closing()
{
    CompraBuscar* w = (CompraBuscar*)sender();

    if(w->getID().compare("")==0)return;

    QString id = w->getID();
    QString sn = w->getCodigo() + " - " + w->getNombre() + " - " + w->getSerie() + " - " + w->getNumero();

    ui->comboBox_id_boleta->addItem(id);
    ui->comboBox_serie_numero_boleta->addItem(sn);
}
void CompraGuiaRR::on_pushButton_buscar_factura_clicked()
{
    CompraBuscar* w = new CompraBuscar;
    w->set_widget_previous(this);
    w->setTipoFactura();
    w->hideOptBoleta();
    w->hideOptFlete();
    w->hideOptGuiaRR();
    w->hideOptNotaCredito();
    w->hideOptNotaDebito();
    w->hideOptOrden();
    w->hideOptRegSinDoc();
    w->hideOptSaldo();
    connect(w, SIGNAL(closing()), this, SLOT(on_factura_buscar_closing()));
    //w->set_ruc(ui->lineEdit_cod->text());

    SYSTEM->change_center_w(this, w);
}

void CompraGuiaRR::on_pushButton_buscar_boleta_clicked()
{
    CompraBuscar* w = new CompraBuscar;
    w->set_widget_previous(this);
    w->setTipoBoleta();
    w->hideOptFactura();
    w->hideOptFlete();
    w->hideOptGuiaRR();
    w->hideOptNotaCredito();
    w->hideOptNotaDebito();
    w->hideOptOrden();
    w->hideOptRegSinDoc();
    w->hideOptSaldo();
    connect(w, SIGNAL(closing()), this, SLOT(on_boleta_buscar_closing()));
    //w->set_ruc(ui->lineEdit_cod->text());

    SYSTEM->change_center_w(this, w);
}

void CompraGuiaRR::on_pushButton_jalar_factura_clicked()
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
                QString p_total_flete = "0.000";

                set_producto(producto_id, cantidad, unidad, descripcion, p_total);
            }
        }else{

        }
    }
}

void CompraGuiaRR::on_pushButton_jalar_boleta_clicked()
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
                QString p_total_flete = "0.000";

                set_producto(producto_id, cantidad, unidad, descripcion, p_total);
            }
        }else{

        }
    }
}

void CompraGuiaRR::on_pushButton_quitar_factura_clicked()
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

void CompraGuiaRR::on_pushButton_quitar_boleta_clicked()
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

void CompraGuiaRR::on_pushButton_canjear_clicked()
{

}

void CompraGuiaRR::on_lineEdit_cod_textChanged(const QString &arg1)
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
