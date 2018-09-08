#include "personausuario.h"
#include "ui_personausuario.h"

PersonaUsuario::PersonaUsuario(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PersonaUsuario)
{
    ui->setupUi(this);

    afterShow = false;

    widget_previous = NULL;

    op = -1;

    // OTHER SETS

    // INSTALLING EVENT FILTERS

    this->installEventFilter(this);
    ui->lineEdit_dni->installEventFilter(this);
    ui->lineEdit_nombre->installEventFilter(this);
    ui->comboBox_tipo->installEventFilter(this);
    ui->lineEdit_usuario->installEventFilter(this);
    ui->lineEdit_pass->installEventFilter(this);
    ui->lineEdit_direccion->installEventFilter(this);

    ui->pushButton_guardar->installEventFilter(this);
    ui->pushButton_eliminar->installEventFilter(this);
    ui->pushButton_salir->installEventFilter(this);
}

PersonaUsuario::~PersonaUsuario()
{
    qDebug()<<"delete usuario op"<<endl;
    delete ui;
}
QString PersonaUsuario::get_ID()
{
    return id;
}
QString PersonaUsuario::get_DNI()
{
    return ui->lineEdit_dni->text();
}
QString PersonaUsuario::get_nombre()
{
    return ui->lineEdit_nombre->text();
}
QString PersonaUsuario::get_ID_rol()
{
    return ui->comboBox_tipo->currentIndex();
}
QString PersonaUsuario::get_rol()
{
    return ui->comboBox_tipo->currentText();
}
QString PersonaUsuario::get_usuario()
{
    return ui->lineEdit_usuario->text();
}
void PersonaUsuario::set_widget_previous(QWidget *widget_previous)
{
    this->widget_previous = widget_previous;
}
bool PersonaUsuario::select(const QString& id)
{
    this->id = id;
    QString str_query;
    QSqlQuery query;

    str_query = "SELECT rol.rol, usuario.usuario, naturales.dni, naturales.nombre FROM usuario";
    str_query += " JOIN rol ON rol.id = usuario.rol_id";
    str_query += " JOIN naturales ON naturales.persona_id = usuario.naturales_persona_id";
    str_query += " JOIN persona ON persona.id = usuario.naturales_persona_id";
    str_query += " WHERE usuario.naturales_persona_id = "+id;

    qDebug()<<str_query<<endl;
    if(query.exec(str_query)) {
        query.next();
        ui->comboBox_tipo->setCurrentText(query.value(0).toString());
        ui->lineEdit_usuario->setText(query.value(1).toString());
        ui->lineEdit_dni->setText(query.value(2).toString());
        ui->lineEdit_nombre->setText(query.value(3).toString());
        return true;
    }else{
        return false;
    }
}
bool PersonaUsuario::guardar()
{
    QString str_query;

    SYSTEM->fixString(ui->lineEdit_dni);
    QString dni = ui->lineEdit_dni->text();
    dni = SYSTEM->replace_quotes(dni);

    SYSTEM->fixString(ui->lineEdit_nombre);
    QString nombre = ui->lineEdit_nombre->text();
    nombre = SYSTEM->replace_quotes(nombre);

    SYSTEM->fixString(ui->lineEdit_usuario);
    QString usuario = ui->lineEdit_usuario->text();
    usuario = SYSTEM->replace_quotes(usuario);

    SYSTEM->fixString(ui->lineEdit_pass);
    QString pass = ui->lineEdit_pass->text();
    pass = SYSTEM->replace_quotes(pass);

    SYSTEM->fixString(ui->lineEdit_direccion);
    QString direccion = ui->lineEdit_direccion->text();
    direccion = SYSTEM->replace_quotes(direccion);

    if (id.compare("") == 0) {
        QString id = "NULL";
        str_query += "INSERT INTO persona(id, tipo_persona_id, habilitado)VALUES";
        str_query += "("+id;
        str_query += ", "+QString().setNum(tipo_persona::USUARIO);
        str_query += ", 1)";
        str_query += "&&END_QUERY&&";

        str_query += "INSERT INTO naturales(persona_id, dni, nombre)VALUES(";
        str_query += "(SELECT MAX(persona.id) FROM persona)";
        str_query += ", '"+dni+"'";
        str_query += ", '"+nombre+"')";
        str_query += "&&END_QUERY&&";

        str_query += "INSERT INTO usuario(naturales_persona_id";
        str_query += ", rol_id";
        str_query += ", usuario";
        str_query += ", pass";
        str_query += ", direccion";
        str_query += ")VALUES(";
        str_query += "(SELECT MAX(persona.id) FROM persona)";
        str_query += ", "+QString().setNum(ui->comboBox_tipo->currentIndex()+1);
        str_query += ", '"+usuario+"'";
        str_query += ", '"+pass+"'";
        str_query += ", '"+pass+"')";
        str_query += "&&END_QUERY&&";

        str_query += "SELECT MAX(id) FROM persona";
        str_query += "&&END_QUERY&&";
    }else{
        str_query += "UPDATE naturales";
        str_query += " SET dni = '"+dni+"'";
        str_query += ", nombre = '"+nombre+"'";
        str_query += ", direccion = '"+direccion+"'";
        str_query += " WHERE persona_id = "+id;
        str_query += "&&END_QUERY&&";

        str_query += "UPDATE usuario";
        str_query += " SET rol_id = "+QString().setNum(ui->comboBox_tipo->currentIndex()+1)+"";
        str_query += ", usuario = '"+nombre+"'";
        str_query += ", pass = '"+pass+"'";
        str_query += " WHERE naturales_persona_id = "+id;
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
        if(query.exec("ROLLBACK")){

        }else{

        }
        return false;
    }
}
bool PersonaUsuario::remove()
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
void PersonaUsuario::on_pushButton_guardar_clicked()
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

void PersonaUsuario::on_pushButton_eliminar_clicked()
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

void PersonaUsuario::on_pushButton_salir_clicked()
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
void PersonaUsuario::showEvent(QShowEvent *event)
{
    event->accept();

    afterShow = true;
}
void PersonaUsuario::closeEvent(QCloseEvent *event)
{
    event->accept();
    emit closing();
}

bool PersonaUsuario::eventFilter(QObject *obj, QEvent *e)
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
                ui->comboBox_tipo->setFocus(Qt::TabFocusReason);
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
    w_temp = ui->comboBox_tipo;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:
                ui->lineEdit_usuario->setFocus(Qt::TabFocusReason);
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
    w_temp = ui->lineEdit_usuario;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:
                ui->lineEdit_pass->setFocus(Qt::TabFocusReason);
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
    w_temp = ui->lineEdit_pass;
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

