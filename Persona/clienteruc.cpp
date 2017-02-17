#include "clienteruc.h"
#include "ui_clienteruc.h"

ClienteRUC::ClienteRUC(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClienteRUC)
{
    ui->setupUi(this);
    widget_previous = NULL;

    QRegExp regExp_ruc("[0-9]{11,11}");
    QRegExp regExp_in("[0-9a-zA-Z -áéíóú]{255,255}");
    QRegExp regExp_nu("[0-9a-zA-Z -áéíóú]{55,55}");

    ui->lineEdit_ruc->setValidator(new QRegExpValidator(regExp_ruc));
    ui->lineEdit_razon_social->setValidator(new QRegExpValidator(regExp_in));
    ui->lineEdit_tipo_contribuyente->setValidator(new QRegExpValidator(regExp_in));
    ui->lineEdit_nombre_comercial->setValidator(new QRegExpValidator(regExp_in));
    ui->lineEdit_estado->setValidator(new QRegExpValidator(regExp_in));
    ui->lineEdit_condicion->setValidator(new QRegExpValidator(regExp_in));    
    ui->lineEdit_direccion->setValidator(new QRegExpValidator(regExp_in));
    ui->lineEdit_telefono->setValidator(new QRegExpValidator(regExp_nu));

    // OTHER SETS ---------------------------------------
    ui->lineEdit_codigo_captcha->hide();
    ui->label_captcha->hide();

    // INSTALL EVENT FILTERS

    this->installEventFilter(this);
    ui->lineEdit_codigo_captcha->installEventFilter(this);
    ui->lineEdit_ruc->installEventFilter(this);
    ui->lineEdit_razon_social->installEventFilter(this);
    ui->lineEdit_tipo_contribuyente->installEventFilter(this);
    ui->lineEdit_nombre_comercial->installEventFilter(this);
    ui->lineEdit_estado->installEventFilter(this);
    ui->lineEdit_condicion->installEventFilter(this);
    ui->lineEdit_direccion->installEventFilter(this);
    ui->lineEdit_telefono->installEventFilter(this);

    ui->pushButton_guardar->installEventFilter(this);
    ui->pushButton_eliminar->installEventFilter(this);
    ui->pushButton_salir->installEventFilter(this);
}

ClienteRUC::~ClienteRUC()
{
    qDebug()<<"delete cliente ruc"<<endl;
    delete ui;
}
QString ClienteRUC::get_ID()
{
    return id;
}
QString ClienteRUC::get_ruc()
{
    return ui->lineEdit_ruc->text();
}
QString ClienteRUC::get_razon_social()
{
    return ui->lineEdit_razon_social->text();
}
QString ClienteRUC::get_direccion()
{
    return ui->lineEdit_direccion->text();
}
void ClienteRUC::set_widget_previous(QWidget *widget_previous)
{
    this->widget_previous = widget_previous;
}

void ClienteRUC::set_captcha()
{
	consulta_ruc.set_data(ui->label_captcha
        , ui->lineEdit_ruc, ui->lineEdit_codigo_captcha
        , ui->lineEdit_razon_social, ui->lineEdit_tipo_contribuyente
        , ui->lineEdit_nombre_comercial, ui->lineEdit_direccion
        , ui->lineEdit_estado, ui->lineEdit_condicion, ui->tableWidget
	);
                          
}
bool ClienteRUC::select(const QString& id)
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
    str_query += ", cliente_ruc.telefono";
    str_query += " FROM cliente_ruc";
    str_query += " JOIN juridica ON juridica.persona_id = cliente_ruc.juridica_persona_id";
    str_query += " JOIN persona ON persona.id = cliente_ruc.juridica_persona_id";
    str_query += " WHERE cliente_ruc.juridica_persona_id = "+id+")";
    str_query += " UNION ALL";
    str_query += "(SELECT establecimientos.codigo";
    str_query += ", establecimientos.tipo";
    str_query += ", establecimientos.direccion";
    str_query += ", establecimientos.actividad";
    str_query += ", '', '', '', ''";
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
        ui->lineEdit_telefono->setText(query.value(8).toString());

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
void ClienteRUC::set_establecimiento(QString codigo, QString tipo, QString direccion, QString actividad)
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
bool ClienteRUC::guardar()
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
        str_query = "INSERT INTO persona(id, tipo_persona_id, habilitado)VALUES";
        str_query += "("+id;
        str_query += ", "+QString().setNum(tipo_persona::CLIENTE_RUC);
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

        str_query += "INSERT INTO cliente_ruc(juridica_persona_id";
        str_query += ", telefono)VALUES((SELECT MAX(persona.id) FROM persona)";
        str_query += ", '"+ui->lineEdit_telefono->text()+"')";
        str_query += "&&END_QUERY&&";

    }else{
        str_query = "UPDATE juridica SET";
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

        str_query += "UPDATE cliente_ruc SET";
        str_query += " telefono = '"+ui->lineEdit_telefono->text()+"'";
        str_query += " WHERE juridica_persona_id = "+id;
        str_query += "&&END_QUERY&&";
    }

    SYSTEM->multiple_query(str_query);
    qDebug()<<str_query<<endl;
    if(query.exec(str_query)){
        return true;
    }else{
        return false;
    }
}
bool ClienteRUC::remove()
{
    QString str_query = "DELETE FROM persona WHERE id = "+id;
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

void ClienteRUC::on_pushButton_guardar_clicked()
{
    int ret = QMessageBox::warning(this, "Advertencia", "¿Desea guardar los datos?", "Si", "No");
    switch(ret){
    case 0:{
        if(guardar()){
            QMessageBox::information(this, "Información", "Se guardaron los datos con éxito.");
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

void ClienteRUC::on_pushButton_eliminar_clicked()
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

void ClienteRUC::on_pushButton_salir_clicked()
{
    int ret = QMessageBox::warning(this, "Advertencia", "¿Desea salir del formulario?", "Si", "No");
    switch(ret){
    case 0:{
        if(widget_previous){
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
void ClienteRUC::showEvent(QShowEvent *event)
{
    event->accept();

    if(focusWidget()){
        focusWidget()->setFocus();
    }else{
        ui->lineEdit_ruc->setFocus(Qt::TabFocusReason);
    }
}
void ClienteRUC::closeEvent(QCloseEvent *event)
{
    event->accept();
    emit closing();
}

bool ClienteRUC::eventFilter(QObject *obj, QEvent *e)
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
                ui->lineEdit_razon_social->setFocus(Qt::TabFocusReason);
                return true;
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
                ui->pushButton_guardar->click();
                return true;
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
            }

        }else{

        }
        return false;
    }
    return eventFilter(obj, e);
}
