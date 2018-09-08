#include "personaproveedor.h"
#include "ui_personaproveedor.h"

PersonaProveedor::PersonaProveedor(QWidget *parent) :
   QWidget(parent),
    ui(new Ui::PersonaProveedor)
{
    ui->setupUi(this);

    afterShow = false;

    widget_previous = NULL;

    op = -1;

    // SET REG EXPS -------------------------------------
    QRegExp regExp_ruc("[0-9]{11,11}");
    QRegExp regExp_in("[0-9a-zA-Z -áéíóú]{255,255}");
    QRegExp regExp_nu("[0-9a-zA-Z -áéíóú]{55,55}");
    QRegExp regExp_captcha("[A-Z]{4,4}");

    ui->lineEdit_ruc->setValidator(new QRegExpValidator(regExp_ruc));
    ui->lineEdit_razon_social->setValidator(new QRegExpValidator(regExp_in));
    ui->lineEdit_direccion->setValidator(new QRegExpValidator(regExp_in));
    ui->lineEdit_email->setValidator(new QRegExpValidator(regExp_in));
    ui->lineEdit_estado->setValidator(new QRegExpValidator(regExp_in));
    ui->lineEdit_condicion->setValidator(new QRegExpValidator(regExp_in));
    ui->lineEdit_fax->setValidator(new QRegExpValidator(regExp_nu));
    ui->lineEdit_nombre->setValidator(new QRegExpValidator(regExp_in));
    ui->lineEdit_nombre_comercial->setValidator(new QRegExpValidator(regExp_in));
    ui->lineEdit_representante->setValidator(new QRegExpValidator(regExp_in));
    ui->lineEdit_rpc->setValidator(new QRegExpValidator(regExp_nu));
    ui->lineEdit_rpm->setValidator(new QRegExpValidator(regExp_nu));
    ui->lineEdit_telefono->setValidator(new QRegExpValidator(regExp_nu));
    ui->lineEdit_tipo_contribuyente->setValidator(new QRegExpValidator(regExp_in));

    ui->lineEdit_codigo_captcha->setValidator(new QRegExpValidator(regExp_captcha));
    //ui->lineEdit_comentario->setValidator(new QRegExpValidator(regExp_in));

    // OTHER SETS ---------------------------------------
    ui->lineEdit_codigo_captcha->hide();
    ui->label_captcha->hide();

    // INSTALLING EVENT FILTERS

    this->installEventFilter(this);
    ui->lineEdit_codigo_captcha->installEventFilter(this);
    ui->lineEdit_comentario->installEventFilter(this);
    ui->lineEdit_direccion->installEventFilter(this);
    ui->lineEdit_email->installEventFilter(this);
    ui->lineEdit_estado->installEventFilter(this);
    ui->lineEdit_condicion->installEventFilter(this);
    ui->lineEdit_fax->installEventFilter(this);
    ui->lineEdit_nombre->installEventFilter(this);
    ui->lineEdit_nombre_comercial->installEventFilter(this);
    ui->lineEdit_pagina_web->installEventFilter(this);
    ui->lineEdit_razon_social->installEventFilter(this);
    ui->lineEdit_representante->installEventFilter(this);
    ui->lineEdit_rpc->installEventFilter(this);
    ui->lineEdit_rpm->installEventFilter(this);
    ui->lineEdit_ruc->installEventFilter(this);
    ui->lineEdit_telefono->installEventFilter(this);
    ui->lineEdit_tipo_contribuyente->installEventFilter(this);

    ui->pushButton_guardar->installEventFilter(this);
    ui->pushButton_eliminar->installEventFilter(this);
    ui->pushButton_salir->installEventFilter(this);
}

PersonaProveedor::~PersonaProveedor()
{
    qDebug()<<"delete proveedor op"<<endl;
    delete ui;
}
QString PersonaProveedor::get_ID()
{
    return id;
}
QString PersonaProveedor::get_ruc()
{
    return ui->lineEdit_ruc->text();
}
QString PersonaProveedor::get_razon_social()
{
    return ui->lineEdit_razon_social->text();
}
QString PersonaProveedor::get_direccion()
{
    return ui->lineEdit_direccion->text();
}
void PersonaProveedor::set_widget_previous(QWidget *widget_previous)
{
    this->widget_previous = widget_previous;
}

void PersonaProveedor::set_captcha()
{
    consulta_ruc.set_data(ui->label_captcha
        , ui->lineEdit_ruc, ui->lineEdit_codigo_captcha
        , ui->lineEdit_razon_social, ui->lineEdit_tipo_contribuyente
        , ui->lineEdit_nombre_comercial, ui->lineEdit_direccion
        , ui->lineEdit_estado, ui->lineEdit_condicion, ui->tableWidget
    );

}
bool PersonaProveedor::select(const QString& id)
{
    this->id = id;
    QString str_query;
    QSqlQuery query;

    str_query = "(SELECT juridica.ruc";
    str_query += ", juridica.razon_social";
    str_query += ", juridica.tipo_contribuyente";
    str_query += ", juridica.nombre_comercial";
    str_query += ", juridica.estado";
    str_query += ", juridica.condicion";
    str_query += ", juridica.domicilio_fiscal";
    str_query += ", proveedor.representante";
    str_query += ", proveedor.telefono";
    str_query += ", proveedor.fax";
    str_query += ", proveedor.pagina_web";
    str_query += ", proveedor.email";
    str_query += ", proveedor.comentario";
    str_query += ", proveedor.nombre";
    str_query += ", proveedor.rpm";
    str_query += ", proveedor.rpc";
    str_query += " FROM proveedor";
    str_query += " JOIN juridica ON juridica.persona_id = proveedor.juridica_persona_id";
    str_query += " JOIN persona ON persona.id = proveedor.juridica_persona_id";
    str_query += " WHERE proveedor.juridica_persona_id = "+id+")";
    str_query += " UNION ALL";
    str_query += "(SELECT establecimientos.codigo";
    str_query += ", establecimientos.tipo";
    str_query += ", establecimientos.direccion";
    str_query += ", establecimientos.actividad";
    str_query += ", '', '', '', '', '', '', '', '', '', '', '', ''";
    str_query += " FROM establecimientos";
    str_query += " WHERE establecimientos.juridica_persona_id = "+id+")";

    qDebug()<<str_query<<endl;
    if(query.exec(str_query)) {
        query.next();
        ui->lineEdit_ruc->setText(query.value(0).toString());
        ui->lineEdit_razon_social->setText(query.value(1).toString());
        ui->lineEdit_tipo_contribuyente->setText(query.value(2).toString());
        ui->lineEdit_nombre_comercial->setText(query.value(3).toString());
        ui->lineEdit_estado->setText(query.value(4).toString());
        ui->lineEdit_condicion->setText(query.value(5).toString());
        ui->lineEdit_direccion->setText(query.value(6).toString());
        ui->lineEdit_representante->setText(query.value(7).toString());
        ui->lineEdit_telefono->setText(query.value(8).toString());
        ui->lineEdit_fax->setText(query.value(9).toString());
        ui->lineEdit_pagina_web->setText(query.value(10).toString());
        ui->lineEdit_email->setText(query.value(11).toString());
        ui->lineEdit_comentario->setText(query.value(12).toString());
        ui->lineEdit_nombre->setText(query.value(13).toString());
        ui->lineEdit_rpm->setText(query.value(14).toString());
        ui->lineEdit_rpc->setText(query.value(15).toString());

        while(query.next()) {
            QString codigo = query.value(0).toString();
            QString tipo = query.value(1).toString();
            QString direccion = query.value(2).toString();
            QString actividad = query.value(3).toString();

            set_establecimiento(codigo, tipo, direccion, actividad);
        }
        return true;
    }else{
        return false;
    }
}
void PersonaProveedor::set_establecimiento(QString codigo, QString tipo, QString direccion, QString actividad)
{
    int rowCount = ui->tableWidget->rowCount();
    ui->tableWidget->setRowCount(rowCount+1);

    QTableWidgetItem* item_codigo = new QTableWidgetItem(codigo);
    QTableWidgetItem* item_tipo = new QTableWidgetItem(tipo);
    QTableWidgetItem* item_direccion = new QTableWidgetItem(direccion);
    QTableWidgetItem* item_actividad = new QTableWidgetItem(actividad);

    ui->tableWidget->setItem(rowCount, 0, item_codigo);
    ui->tableWidget->setItem(rowCount, 1, item_tipo);
    ui->tableWidget->setItem(rowCount, 2, item_direccion);
    ui->tableWidget->setItem(rowCount, 3, item_actividad);

    item_codigo->setFlags(Qt::ItemIsEnabled
                          | Qt::ItemIsSelectable);

    item_tipo->setFlags(Qt::ItemIsEnabled
                          | Qt::ItemIsSelectable);

    item_direccion->setFlags(Qt::ItemIsEnabled
                          | Qt::ItemIsSelectable);

    item_actividad->setFlags(Qt::ItemIsEnabled
                          | Qt::ItemIsSelectable);

    SYSTEM->table_resize_to_contents(0, ui->tableWidget);
}

bool PersonaProveedor::guardar()
{
    QString str_query;
    QSqlQuery query;

    SYSTEM->fixString(ui->lineEdit_razon_social);
    QString razon_social = ui->lineEdit_razon_social->text();
    razon_social = SYSTEM->replace_quotes(razon_social);

    SYSTEM->fixString(ui->lineEdit_direccion);
    QString direccion = ui->lineEdit_direccion->text();
    direccion = SYSTEM->replace_quotes(direccion);

    if (id.compare("") == 0) {
        QString id = "NULL";
        str_query += "INSERT INTO persona(id, tipo_persona_id, habilitado)VALUES";
        str_query += "("+id;
        str_query += ", "+QString().setNum(tipo_persona::PROVEEDOR);
        str_query += ", 1)";
        str_query += "&&END_QUERY&&";

        str_query += "INSERT INTO juridica(persona_id";
        str_query += ", ruc";
        str_query += ", razon_social";
        str_query += ", tipo_contribuyente";
        str_query += ", nombre_comercial";
        str_query += ", estado";
        str_query += ", condicion";
        str_query += ", domicilio_fiscal)VALUES((SELECT MAX(persona.id) FROM persona)";
        str_query += ", '"+ui->lineEdit_ruc->text()+"'";
        str_query += ", '"+razon_social+"'";
        str_query += ", '"+ui->lineEdit_tipo_contribuyente->text()+"'";
        str_query += ", '"+ui->lineEdit_nombre_comercial->text()+"'";
        str_query += ", '"+ui->lineEdit_estado->text()+"'";
        str_query += ", '"+ui->lineEdit_condicion->text()+"'";
        str_query += ", '"+direccion+"')";
        str_query += "&&END_QUERY&&";

        if(ui->tableWidget->columnCount() == 4) {
            QString str_query_2;
            for(int i=0; i<ui->tableWidget->rowCount(); i++){
                QString codigo = ui->tableWidget->item(i, 0)->text();
                QString tipo = ui->tableWidget->item(i, 1)->text();
                QString direccion = ui->tableWidget->item(i, 2)->text();
                QString actividad = ui->tableWidget->item(i, 3)->text();
                str_query_2 += ",(";
                str_query_2 += "(SELECT MAX(persona.id) FROM persona)";
                str_query_2 += ", '"+codigo+"'";
                str_query_2 += ", '"+tipo+"'";
                str_query_2 += ", '"+direccion+"'";
                str_query_2 += ", '"+actividad+"')";
            }
            if(ui->tableWidget->rowCount() > 0) {
                str_query += "INSERT INTO establecimientos(juridica_persona_id";
                str_query += ", codigo";
                str_query += ", tipo";
                str_query += ", direccion";
                str_query += ", actividad)VALUES";

                str_query_2.remove(0, 1);
                str_query += str_query_2;
                str_query += "&&END_QUERY&&";
            }
        }

        str_query += "INSERT INTO proveedor(juridica_persona_id";
        str_query += ", representante";
        str_query += ", telefono";
        str_query += ", fax";
        str_query += ", pagina_web";
        str_query += ", email";
        str_query += ", comentario";
        str_query += ", nombre";
        str_query += ", rpm";
        str_query += ", rpc)VALUES((SELECT MAX(persona.id) FROM persona)";
        str_query += ", '"+ui->lineEdit_representante->text()+"'";
        str_query += ", '"+ui->lineEdit_telefono->text()+"'";
        str_query += ", '"+ui->lineEdit_fax->text()+"'";
        str_query += ", '"+ui->lineEdit_pagina_web->text()+"'";
        str_query += ", '"+ui->lineEdit_email->text()+"'";
        str_query += ", '"+ui->lineEdit_comentario->text()+"'";
        str_query += ", '"+ui->lineEdit_nombre->text()+"'";
        str_query += ", '"+ui->lineEdit_rpm->text()+"'";
        str_query += ", '"+ui->lineEdit_rpc->text()+"')";
        str_query += "&&END_QUERY&&";

        str_query += "SELECT MAX(id) FROM persona";
        str_query += "&&END_QUERY&&";
    }else{
        str_query += "UPDATE juridica SET";
        str_query += " ruc = '"+ui->lineEdit_ruc->text()+"'";
        str_query += ", razon_social = '"+razon_social+"'";
        str_query += ", tipo_contribuyente = '"+ui->lineEdit_tipo_contribuyente->text()+"'";
        str_query += ", nombre_comercial = '"+ui->lineEdit_nombre_comercial->text()+"'";
        str_query += ", estado = '"+ui->lineEdit_estado->text()+"'";
        str_query += ", condicion = '"+ui->lineEdit_condicion->text()+"'";
        str_query += ", domicilio_fiscal = '"+direccion+"'";
        str_query += " WHERE persona_id = "+id;
        str_query += "&&END_QUERY&&";

        if(ui->tableWidget->columnCount() == 4) {
            QString str_query_2;
            for(int i=0; i<ui->tableWidget->rowCount(); i++){
                QString codigo = ui->tableWidget->item(i, 0)->text();
                QString tipo = ui->tableWidget->item(i, 1)->text();
                QString direccion = ui->tableWidget->item(i, 2)->text();
                QString actividad = ui->tableWidget->item(i, 3)->text();
                str_query_2 += ",(";
                str_query_2 += id;
                str_query_2 += ", '"+codigo+"'";
                str_query_2 += ", '"+tipo+"'";
                str_query_2 += ", '"+direccion+"'";
                str_query_2 += ", '"+actividad+"')";
            }
            if(ui->tableWidget->rowCount() > 0) {
                str_query += "DELETE FROM establecimientos WHERE juridica_persona_id = "+id;
                str_query += "&&END_QUERY&&";

                str_query += "INSERT INTO establecimientos(juridica_persona_id";
                str_query += ", codigo";
                str_query += ", tipo";
                str_query += ", direccion";
                str_query += ", actividad)VALUES";

                str_query_2.remove(0, 1);
                str_query += str_query_2;
                str_query += "&&END_QUERY&&";
            }
        }

        str_query += "UPDATE proveedor SET";
        str_query += " representante = '"+ui->lineEdit_representante->text()+"'";
        str_query += ", telefono = '"+ui->lineEdit_telefono->text()+"'";
        str_query += ", fax = '"+ui->lineEdit_fax->text()+"'";
        str_query += ", pagina_web = '"+ui->lineEdit_pagina_web->text()+"'";
        str_query += ", email = '"+ui->lineEdit_email->text()+"'";
        str_query += ", comentario = '"+ui->lineEdit_comentario->text()+"'";
        str_query += ", nombre = '"+ui->lineEdit_nombre->text()+"'";
        str_query += ", rpm = '"+ui->lineEdit_rpm->text()+"'";
        str_query += ", rpc = '"+ui->lineEdit_rpc->text()+"'";
        str_query += " WHERE juridica_persona_id = "+id;
        str_query += "&&END_QUERY&&";
    }
    str_query += "COMMIT";
    str_query += "&&END_QUERY&&";

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
bool PersonaProveedor::remove()
{
    QString str_query;

    str_query += "DELETE FROM persona WHERE id = "+id;
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

void PersonaProveedor::on_pushButton_guardar_clicked()
{
    int ret = QMessageBox::warning(this, "Advertencia", "¿Desea guardar los datos?", "Si", "No");
    switch(ret){
    case 0:{
        if(guardar()){
            //QMessageBox::information(this, "Información", "Se guardaron los datos con éxito.");
            setAttribute(Qt::WA_DeleteOnClose);
            SYSTEM->change_center_w(this, widget_previous);

            QMainWindow* mw = SYSTEM->get_mainw(this);
            SnackBarInfo* w = new SnackBarInfo;
            w->set_data("Se guardo exitosamente.", ":/new/Iconos/successfull.png");
            mw->statusBar()->addWidget(w);
            int width = mw->width();
            w->setMinimumWidth(width);
            w->setMaximumWidth(width);
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
        return;
    }break;
    case 1:{

    }
    }
}

void PersonaProveedor::on_pushButton_eliminar_clicked()
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
            //QMessageBox::information(this, "Información", "Se eliminaron los datos con éxito.");
            id = "";
            setAttribute(Qt::WA_DeleteOnClose);
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

void PersonaProveedor::on_pushButton_salir_clicked()
{
    int ret = QMessageBox::warning(this, "Advertencia", "¿Desea salir del formulario?", "Si", "No");
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
        return;
    }break;
    case 1:{

    }
    }
}

void PersonaProveedor::showEvent(QShowEvent *event)
{
    event->accept();

    afterShow = true;
}
void PersonaProveedor::closeEvent(QCloseEvent *event)
{
    event->accept();
    emit closing();
}
bool PersonaProveedor::eventFilter(QObject *obj, QEvent *e)
{
    QWidget* w_temp;
    w_temp = this;
    if(obj == w_temp){
        if(e->type() == QEvent::MouseButtonPress){
            if(focusWidget()){
                focusWidget()->setFocus();
            }else{
                ui->lineEdit_ruc->setFocus();
                ui->lineEdit_ruc->setCursorPosition(ui->lineEdit_ruc->text().length());
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
                    ui->lineEdit_ruc->setFocus();
                    ui->lineEdit_ruc->setCursorPosition(ui->lineEdit_ruc->text().length());
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
            }

        }else{

        }
        return false;
    }
    w_temp = ui->lineEdit_ruc;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:
                ui->lineEdit_razon_social->setFocus();
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
    w_temp = ui->lineEdit_codigo_captcha;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:
                ui->lineEdit_razon_social->setFocus();
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
    w_temp = ui->lineEdit_razon_social;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:
                ui->lineEdit_tipo_contribuyente->setFocus();
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
    w_temp = ui->lineEdit_tipo_contribuyente;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:
                ui->lineEdit_nombre_comercial->setFocus();
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
    w_temp = ui->lineEdit_nombre_comercial;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:
                ui->lineEdit_estado->setFocus();
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
    w_temp = ui->lineEdit_estado;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:
                ui->lineEdit_condicion->setFocus();
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
    w_temp = ui->lineEdit_condicion;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:
                ui->lineEdit_direccion->setFocus();
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
    w_temp = ui->lineEdit_direccion;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:
                ui->lineEdit_telefono->setFocus();
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
    w_temp = ui->lineEdit_telefono;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:
                ui->lineEdit_fax->setFocus();
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
    w_temp = ui->lineEdit_fax;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:
                ui->lineEdit_representante->setFocus();
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
    w_temp = ui->lineEdit_representante;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:
                ui->lineEdit_pagina_web->setFocus();
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
    w_temp = ui->lineEdit_pagina_web;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:
                ui->lineEdit_email->setFocus();
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
    w_temp = ui->lineEdit_email;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:
                ui->lineEdit_comentario->setFocus();
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
    w_temp = ui->lineEdit_comentario;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:
                ui->lineEdit_nombre->setFocus();
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
    w_temp = ui->lineEdit_nombre;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:
                ui->lineEdit_rpm->setFocus();
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
    w_temp = ui->lineEdit_rpm;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:
                ui->lineEdit_rpc->setFocus();
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
    w_temp = ui->lineEdit_rpc;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:
                ui->pushButton_guardar->click();
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
    w_temp = ui->pushButton_guardar;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:
                ui->pushButton_guardar->click();
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

    w_temp = ui->pushButton_eliminar;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:
                ui->pushButton_eliminar->click();
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

        }else{

        }
        return false;
    }
    return eventFilter(obj, e);
}

