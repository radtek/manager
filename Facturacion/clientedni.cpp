#include "clientedni.h"
#include "ui_clientedni.h"

ClienteDNI::ClienteDNI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClienteDNI)
{
    ui->setupUi(this);

    afterShow = false;

    widget_previous = NULL;

    op = -1;

    QRegExp regExp_in("[0-9a-zA-Z -áéíóú]{255,255}");

    QRegExp regExp_dni("[0-9]{8,8}");

    ui->lineEdit_dni->setValidator(new QRegExpValidator(regExp_dni));
    ui->lineEdit_nombre->setValidator(new QRegExpValidator(regExp_in));

    ui->lineEdit_codigo_captcha->hide();
    ui->label_captcha->hide();

    // OTHER SETS
    //ui->pushButton_modificar->setDisabled(true);
    //ui->pushButton_eliminar->setDisabled(true);

    // INSTALL EVENT FILTERS

    this->installEventFilter(this);
    ui->lineEdit_dni->installEventFilter(this);
    ui->lineEdit_nombre->installEventFilter(this);
    ui->lineEdit_direccion->installEventFilter(this);

    ui->pushButton_guardar->installEventFilter(this);
    ui->pushButton_eliminar->installEventFilter(this);
    ui->pushButton_salir->installEventFilter(this);
}

ClienteDNI::~ClienteDNI()
{
    qDebug() << "delete cliente dni" << endl;
    delete ui;
}
QString ClienteDNI::get_ID()
{
    return id;
}
QString ClienteDNI::get_DNI()
{
    return ui->lineEdit_dni->text();
}
QString ClienteDNI::get_nombre()
{
    return ui->lineEdit_nombre->text();
}
QString ClienteDNI::get_direccion()
{
    return ui->lineEdit_direccion->text();
}
void ClienteDNI::set_widget_previous(QWidget *widget_previous)
{
    this->widget_previous = widget_previous;
}
void ClienteDNI::set_captcha()
{	
	consulta_dni.set_data(ui->label_captcha, ui->lineEdit_dni
		, ui->lineEdit_codigo_captcha, ui->lineEdit_nombre);
}
bool ClienteDNI::select(const QString& id)
{
    this->id = id;
    QString str_query;
    QSqlQuery query;

    str_query = "SELECT naturales.dni, naturales.nombre, naturales.direccion FROM cliente_dni";
    str_query += " JOIN naturales ON naturales.persona_id = cliente_dni.naturales_persona_id";
    str_query += " JOIN persona ON persona.id = cliente_dni.naturales_persona_id";
    str_query += " WHERE cliente_dni.naturales_persona_id = "+id;

    qDebug()<<str_query<<endl;
    if(query.exec(str_query)) {
        query.next();
        ui->lineEdit_dni->setText(query.value(0).toString());
        ui->lineEdit_nombre->setText(query.value(1).toString());
        ui->lineEdit_direccion->setText(query.value(2).toString());
        return true;
    }else{
        return false;
    }
}
bool ClienteDNI::guardar()
{
    QString str_query;

    SYSTEM->fixString(ui->lineEdit_dni);
    QString dni = ui->lineEdit_dni->text();
    dni = SYSTEM->replace_quotes(dni);

    SYSTEM->fixString(ui->lineEdit_nombre);
    QString nombre = ui->lineEdit_nombre->text();
    nombre = SYSTEM->replace_quotes(nombre);

    QString direccion = ui->lineEdit_direccion->text().simplified();

    if (id.compare("") == 0) {
        QString id = "NULL";
        str_query = "INSERT INTO persona(id, tipo_persona_id, habilitado)VALUES";
        str_query += "("+id;
        str_query += ", "+QString().setNum(tipo_persona::CLIENTE_DNI);
        str_query += ", 1)";
        str_query += "&&END_QUERY&&";

        str_query += "INSERT INTO naturales(persona_id, dni, nombre, direccion)VALUES(";
        str_query += "(SELECT MAX(persona.id) FROM persona)";
        str_query += ", '"+dni+"'";
        str_query += ", '"+nombre+"'";
        str_query += ", '"+direccion+"')";
        str_query += "&&END_QUERY&&";

        str_query += "INSERT INTO cliente_dni(naturales_persona_id)VALUES";
        str_query += "((SELECT MAX(persona.id) FROM persona))";
        str_query += "&&END_QUERY&&";

        str_query += "SELECT MAX(id) FROM persona";
        str_query += "&&END_QUERY&&";
    }else{

        str_query = "UPDATE naturales";
        str_query += " SET dni = '"+dni+"'";
        str_query += ", nombre = '"+nombre+"'";
        str_query += ", direccion = '"+direccion+"'";
        str_query += " WHERE persona_id = "+id;
        str_query += "&&END_QUERY&&";
    }
    str_query += "COMMIT";
    str_query += "&&END_QUERY&&";

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
        return false;
    }
}
bool ClienteDNI::remove()
{
    QString str_query;

    str_query = "DELETE FROM persona WHERE id = "+id;
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
void ClienteDNI::on_pushButton_guardar_clicked()
{
    int ret = QMessageBox::warning(this, "Advertencia", "¿Desea guardar los datos?", "Si", "No");
    switch(ret){
    case 0:{
        if(guardar()){
            QMessageBox::information(this, "Información", "Se guardo con éxito.");
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
void ClienteDNI::on_pushButton_eliminar_clicked()
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
void ClienteDNI::on_pushButton_salir_clicked()
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
void ClienteDNI::showEvent(QShowEvent *event)
{
    event->accept();

    afterShow = true;
}
void ClienteDNI::closeEvent(QCloseEvent *event)
{
    event->accept();
    emit closing();
}

bool ClienteDNI::eventFilter(QObject *obj, QEvent *e)
{
    QWidget* w_temp;
    w_temp = this;
    if(obj == w_temp){
        if(e->type() == QEvent::MouseButtonPress){
            if(focusWidget()){
                focusWidget()->setFocus();
            }else{
                ui->lineEdit_dni->setFocus();
                ui->lineEdit_dni->setCursorPosition(ui->lineEdit_dni->text().length());
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
                    ui->lineEdit_dni->setFocus();
                    ui->lineEdit_dni->setCursorPosition(ui->lineEdit_dni->text().length());
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
    w_temp = ui->lineEdit_dni;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:
                ui->lineEdit_nombre->setFocus(Qt::TabFocusReason);
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
                ui->lineEdit_direccion->setFocus(Qt::TabFocusReason);
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
    if (obj == w_temp) {
        if (e->type() == QEvent::KeyPress) {
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch (KeyEvent->key())
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

        }
        else {

        }
        return false;
    }
    w_temp = ui->pushButton_salir;
    if (obj == w_temp) {
        if (e->type() == QEvent::KeyPress) {
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch (KeyEvent->key())
            {
            case Qt::Key_Tab: {
                this->setFocus();
            }break;
            case Qt::Key_Return: {
                ui->pushButton_salir->click();
                return true;
            }break;
            case Qt::Key_Enter:{
                QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QApplication::sendEvent(w_temp, key);
                return true;
            }break;
            }

        } else {

        }
        return false;
    }
    return eventFilter(obj, e);
}
