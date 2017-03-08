#include "compraguiarr.h"
#include "ui_compraguiarr.h"

CompraGuiaRR::CompraGuiaRR(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CompraGuiaRR)
{
    ui->setupUi(this);

    widget_previous = NULL;

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
    dSpinBoxDelegate->setDecimals(DECIMALS_CANTIDAD);
    dSpinBoxDelegate->setSingleStep(QString("0."+SYSTEM->zeros(DECIMALS_CANTIDAD)+"1").toDouble());

    ui->tableWidget->hideColumn(INDEX_ID);

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

    str_query = "SELECT producto.id, d_h_prod.cantidad, unidad.unidad";
    str_query += ", producto.descripcion, d_h_prod.precio FROM documento";
    //str_query += " JOIN documento_h_persona d_h_per ON d.id = d_h_per.documento_id";
    //str_query += " JOIN persona ON persona.id = d_h_per.persona_id";
    //str_query += " JOIN juridica ON persona.id = d_h_per.persona_id";
    //str_query += " JOIN proveedor ON persona.id = proveedor.juridica_persona_id";
    str_query += " JOIN documento_h_producto d_h_prod ON documento.id = d_h_prod.documento_id";
    str_query += " JOIN producto ON producto.id = d_h_prod.producto_id";
    str_query += " JOIN marca ON marca.id = producto.marca_id";
    str_query += " JOIN unidad ON unidad.id = producto.unidad_id";
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
    //unidad = v[1];
    //descripcion = v[2];
    p_total = QString().setNum(p_total.toDouble(), ' ', DECIMALS_PRECIO_TOTAL);

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

        // FACTURA
        str_query += "INSERT INTO guia_remision_remitente(comprobante_documento_id)VALUES(";
        str_query += "(SELECT MAX(documento.id) FROM documento)";
        str_query += ")";
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
bool CompraGuiaRR::remove()
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
void CompraGuiaRR::set_time()
{
    ui->dateTimeEdit_sistema->setDateTime(QDateTime::currentDateTime());
}
void CompraGuiaRR::on_proveedor_closing()
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
    QString p_total = w->getPrecio();

    set_producto(producto_id, cantidad, unidad, descripcion, p_total);
}

void CompraGuiaRR::on_pushButton_proveedor_clicked()
{
    CompraProveedor* w = new CompraProveedor;
    w->set_tipo(persona_items::PROVEEDOR);
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
void CompraGuiaRR::on_pushButton_salir_clicked()
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
void CompraGuiaRR::showEvent(QShowEvent *se)
{
    se->accept();

    if(focusWidget()){
        focusWidget()->setFocus();
    }else{
        ui->dateTimeEdit_emision->setFocus(Qt::TabFocusReason);
    }
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

void CompraGuiaRR::on_pushButton_dame_productos_izq_clicked()
{
    QString str_query;

    str_query = "SELECT producto.id, d_h_prod.cantidad, unidad.unidad";
    str_query += ", concat(producto.descripcion,' ',marca.marca), d_h_prod.precio FROM orden";
    str_query += " JOIN documento ON orden.comprobante_documento_id = documento.id";
    str_query += " JOIN anexo ON anexo.documento_id = documento.id";    
    str_query += " JOIN documento_h_producto d_h_prod ON documento.id = d_h_prod.documento_id";
    str_query += " JOIN producto ON producto.id = d_h_prod.producto_id";
    str_query += " JOIN marca ON marca.id = producto.marca_id";
    str_query += " JOIN unidad ON unidad.id = producto.unidad_id";
    str_query += " WHERE documento.id = (";
    str_query += "SELECT anexo.documento_id FROM anexo";
    str_query += " JOIN orden ON orden.comprobante_documento_id = anexo.documento_id";
    str_query += " JOIN documento_h_persona ON orden.comprobante_documento_id = documento_h_persona.documento_id";
    str_query += " JOIN persona ON persona.id = documento_h_persona.persona_id";
    str_query += " JOIN juridica ON persona.id = documento_h_persona.persona_id";
    str_query += " JOIN proveedor ON persona.id = proveedor.juridica_persona_id";
    str_query += " WHERE juridica.ruc = '"+ui->lineEdit_cod->text()+"'";
    str_query += " ORDER BY anexo.id DESC, anexo.fecha_emision DESC";
    str_query += " LIMIT "+QString().setNum(++pos_dame_productos)+",1)";
    str_query += " GROUP BY d_h_prod.id";
    str_query += " ORDER BY orden.comprobante_documento_id DESC, anexo.fecha_emision DESC";

    qDebug()<<str_query<<endl;
    if(query.exec(str_query)){
        int rowCount = ui->tableWidget->rowCount();
        for(int i=0; i < rowCount; i++)
            ui->tableWidget->removeRow(0);
        while(query.next()){
            QString producto_id = query.value(0).toString();
            QString cantidad = query.value(1).toString();
            QString unidad = query.value(2).toString();
            QString descripcion = query.value(3).toString();
            QString p_total = query.value(4).toString();

            set_producto(producto_id, cantidad, unidad, descripcion, p_total);
        }
        //return true;
    }else{
        //return false;
    }
}

void CompraGuiaRR::on_pushButton_dame_productos_der_clicked()
{
    QString str_query;

    str_query = "SELECT producto.id, d_h_prod.cantidad, unidad.unidad";
    str_query += ", producto.descripcion, d_h_prod.precio FROM orden";
    str_query += " JOIN documento ON orden.comprobante_documento_id = documento.id";
    str_query += " JOIN anexo ON anexo.documento_id = documento.id";    
    str_query += " JOIN documento_h_producto d_h_prod ON documento.id = d_h_prod.documento_id";
    str_query += " JOIN producto ON producto.id = d_h_prod.producto_id";
    str_query += " JOIN marca ON marca.id = producto.marca_id";
    str_query += " JOIN unidad ON unidad.id = producto.unidad_id";
    str_query += " WHERE documento.id = (";
    str_query += "SELECT anexo.documento_id FROM anexo";
    str_query += " JOIN orden ON orden.comprobante_documento_id = anexo.documento_id";
    str_query += " JOIN documento_h_persona ON orden.comprobante_documento_id = documento_h_persona.documento_id";
    str_query += " JOIN persona ON persona.id = documento_h_persona.persona_id";
    str_query += " JOIN juridica ON persona.id = documento_h_persona.persona_id";
    str_query += " JOIN proveedor ON persona.id = proveedor.juridica_persona_id";
    str_query += " WHERE juridica.ruc = '"+ui->lineEdit_cod->text()+"'";
    str_query += " ORDER BY anexo.id DESC, anexo.fecha_emision DESC";
    str_query += " LIMIT "+QString().setNum(--pos_dame_productos)+",1)";
    str_query += " GROUP BY d_h_prod.id";
    str_query += " ORDER BY orden.comprobante_documento_id DESC, anexo.fecha_emision DESC";

    qDebug()<<str_query<<endl;
    if(query.exec(str_query)){
        int rowCount = ui->tableWidget->rowCount();
        for(int i=0; i < rowCount; i++)
            ui->tableWidget->removeRow(0);
        while(query.next()){
            QString producto_id = query.value(0).toString();
            QString cantidad = query.value(1).toString();
            QString unidad = query.value(2).toString();
            QString descripcion = query.value(3).toString();
            QString p_total = query.value(4).toString();

            set_producto(producto_id, cantidad, unidad, descripcion, p_total);
        }
        //return true;
    }else{        
        int rowCount = ui->tableWidget->rowCount();
        for(int i=0; i < rowCount; i++)
            ui->tableWidget->removeRow(0);
        //return false;
    }
}

void CompraGuiaRR::on_lineEdit_cod_textEdited(const QString &arg1)
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
    CompraAmarres* w_compra_amarres = new CompraAmarres;
    w_compra_amarres->set_widget_previous(this);
    w_compra_amarres->set_documento(this->id, tipo_documento::GUIA_REMISION_REMITENTE);

    SYSTEM->change_center_w(this, w_compra_amarres);
}
