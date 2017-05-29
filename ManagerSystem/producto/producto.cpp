#include "producto.h"
#include "ui_producto.h"

Producto::Producto(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Producto)
{
    ui->setupUi(this);

    op = -1;

    afterShow = false;

    widget_previous = NULL;

    this->id = "";
    this->id_tipo = "";
    this->id_marca = "";
    this->id_unidad = "";
    //QRegExp regExp_in_255("[0-9a-zA-Z ]{255,255}");
    //QRegExp regExp_in_135("[0-9a-zA-Z ]{135,135}");
    //QRegExp regExp_in_45("[0-9a-zA-Z ]{45,45}");

    //ui->lineEdit_tipo->setValidator(new QRegExpValidator(regExp_in_135));
    //ui->lineEdit_marca->setValidator(new QRegExpValidator(regExp_in_135));
    //ui->lineEdit_unidad->setValidator(new QRegExpValidator(regExp_in_135));
    //ui->lineEdit_nombre->setValidator(new QRegExpValidator(regExp_in_255));
    //ui->lineEdit_codigo->setValidator(new QRegExpValidator(regExp_in_45));

    // WIDGET DISABLEDS
    //ui->lineEdit_codigo->setReadOnly(true);
    ui->lineEdit_tipo->setReadOnly(true);
    ui->lineEdit_marca->setReadOnly(true);
    ui->lineEdit_unidad->setReadOnly(true);

    //ui->lineEdit_nombre->setReadOnly(true);

    //ui->pushButton_buscar_tipo->setDisabled(true);
    //ui->pushButton_buscar_marca->setDisabled(true);
    //ui->pushButton_buscar_unidad->setDisabled(true);

    //ui->pushButton_limpiar_tipo->setDisabled(true);
    //ui->pushButton_limpiar_marca->setDisabled(true);
    //ui->pushButton_limpiar_unidad->setDisabled(true);

    //ui->pushButton_guardar->setDisabled(true);

    // INSTALL EVENT FILTERS
    this->installEventFilter(this);

    ui->lineEdit_codigo->installEventFilter(this);

    ui->pushButton_buscar_tipo->installEventFilter(this);
    ui->pushButton_buscar_marca->installEventFilter(this);
    ui->pushButton_buscar_unidad->installEventFilter(this);

    ui->pushButton_limpiar_tipo->installEventFilter(this);
    ui->pushButton_limpiar_marca->installEventFilter(this);
    ui->pushButton_limpiar_unidad->installEventFilter(this);

    ui->lineEdit_descripcion->installEventFilter(this);

    ui->pushButton_guardar->installEventFilter(this);
    ui->pushButton_salir->installEventFilter(this);
    ui->pushButton_eliminar->installEventFilter(this);
}

Producto::~Producto()
{
    qDebug()<<"delete producto"<<endl;
    delete ui;
}
QString Producto::get_ID()
{
    return id;
}
QString Producto::get_IDTipo()
{
    return id_tipo;
}
QString Producto::get_IDMarca()
{
    return id_marca;
}
QString Producto::get_IDUnidad()
{
    return id_unidad;
}
QString Producto::get_codigo()
{
    return ui->lineEdit_codigo->text();
}
QString Producto::get_tipo()
{
    return ui->lineEdit_tipo->text();
}
QString Producto::get_descripcion()
{
    return ui->lineEdit_descripcion->text();
}
QString Producto::get_marca()
{
    return ui->lineEdit_marca->text();
}
QString Producto::get_unidad()
{
    return ui->lineEdit_unidad->text();
}
void Producto::set_widget_previous(QWidget *w)
{
    this->widget_previous = w;
}
void Producto::set_data(QString id, QString id_tipo, QString id_marca, QString id_unidad
    , QString codigo, QString tipo, QString descripcion, QString marca, QString unidad)
{
    this->id = id;
    this->id_tipo = id_tipo;
    this->id_marca = id_marca;
    this->id_unidad = id_unidad;

    ui->lineEdit_codigo->setText(codigo);
    ui->lineEdit_tipo->setText(tipo);
    ui->lineEdit_marca->setText(marca);
    ui->lineEdit_unidad->setText(unidad);
    ui->lineEdit_descripcion->setText(descripcion);
}
bool Producto::guardar()
{
    QSqlQuery query;

    QString id = this->id;
    QString id_tipo = this->id_tipo;
    QString id_marca = this->id_marca;
    QString id_unidad = this->id_unidad;

    SYSTEM->fixString(ui->lineEdit_codigo);
    SYSTEM->fixString(ui->lineEdit_descripcion);
    QString codigo = ui->lineEdit_codigo->text();
    QString nombre = ui->lineEdit_descripcion->text();
    codigo = SYSTEM->replace_quotes(codigo);
    nombre = SYSTEM->replace_quotes(nombre);

    if(this->id_tipo.compare("") == 0 || this->id_tipo.compare("0") == 0)
        id_tipo = "NULL";
    if(this->id_marca.compare("") == 0 || this->id_marca.compare("0") == 0)
        id_marca = "NULL";
    if(this->id_unidad.compare("") == 0 || this->id_unidad.compare("0") == 0)
        id_unidad = "NULL";

    QString str_query;

    if(id.compare("") == 0) {
        id = "NULL";

        str_query += "INSERT INTO producto(id, codigo, tipo_id, marca_id, unidad_id, descripcion, habilitado)VALUES";
        str_query += QString() + "(" + id + ", '" + codigo + "', " + id_tipo;
        str_query += ", "+id_marca+", "+id_unidad+", '"+nombre+"'";
        str_query += ", 1)";
        str_query += "&&END_QUERY&&";
        str_query += "SELECT MAX(id) FROM producto";
        str_query += "&&END_QUERY&&";
    }else{
        str_query += "UPDATE producto";
        str_query += " SET codigo = '"+codigo+"'";
        str_query += ", tipo_id = "+id_tipo+"";
        str_query += ", marca_id = "+id_marca;
        str_query += ", unidad_id = "+id_unidad;
        str_query += ", descripcion = '"+nombre+"'";
        str_query += ", habilitado = 1";
        str_query += " WHERE id = "+id;
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
bool Producto::remove()
{
    QSqlQuery query;

    QString str_query;

    str_query += "DELETE FROM producto WHERE id = "+id;
    str_query += "&&END_QUERY&&";
    str_query += "COMMIT";
    str_query += "&&END_QUERY&&";

    SYSTEM->multiple_query(str_query);
    qDebug()<<str_query<<endl;
    if(query.exec(str_query)){
        return true;
    }else{
        if(query.exec("ROLLBACK")){

        }else{

        }
        return false;
    }
}

void Producto::on_pushButton_buscar_tipo_clicked()
{
    ProductoTipoBuscar* w = new ProductoTipoBuscar;
    w->set_widget_previous(this);
    connect(w, SIGNAL(closing()), this, SLOT(on_tipoBuscar_closing()));
    SYSTEM->change_center_w(this, w);
}
void Producto::on_tipoBuscar_closing()
{
    ProductoTipoBuscar* tBuscar = (ProductoTipoBuscar*)sender();

    QString id = tBuscar->getID();
    if (id.compare("") == 0) {
        ui->pushButton_buscar_marca->setFocus(Qt::TabFocusReason);
        return;
    }
    QString tipo = tBuscar->getTipo();

    this->id_tipo = id;
    ui->lineEdit_tipo->setText(tipo);

    ui->pushButton_buscar_marca->setFocus(Qt::TabFocusReason);
}
void Producto::on_pushButton_buscar_marca_clicked()
{
    ProductoMarcaBuscar* w = new ProductoMarcaBuscar;
    w->set_widget_previous(this);
    connect(w, SIGNAL(closing()), this, SLOT(on_marcaBuscar_closing()));
    SYSTEM->change_center_w(this, w);
}
void Producto::on_marcaBuscar_closing()
{
    ProductoMarcaBuscar* mBuscar = (ProductoMarcaBuscar*)sender();

    QString id = mBuscar->getID();
    if (id.compare("") == 0) {
        ui->pushButton_buscar_unidad->setFocus(Qt::TabFocusReason);
        return;
    }
    QString marca = mBuscar->getMarca();

    this->id_marca = id;
    ui->lineEdit_marca->setText(marca);

    ui->pushButton_buscar_unidad->setFocus(Qt::TabFocusReason);
}
void Producto::on_pushButton_buscar_unidad_clicked()
{
    ProductoUnidadBuscar* w = new ProductoUnidadBuscar;
    w->set_widget_previous(this);
    connect(w, SIGNAL(closing()), this, SLOT(on_unidadBuscar_closing()));
    SYSTEM->change_center_w(this, w);
}

void Producto::on_unidadBuscar_closing()
{
    ProductoUnidadBuscar* mBuscar = (ProductoUnidadBuscar*)sender();

    QString id = mBuscar->getID();
    if (id.compare("") == 0) {
        ui->lineEdit_descripcion->setFocus(Qt::TabFocusReason);
        return;
    }
    QString unidad = mBuscar->getUnidad();

    this->id_unidad = id;
    ui->lineEdit_unidad->setText(unidad);

    ui->lineEdit_descripcion->setFocus(Qt::TabFocusReason);
}

void Producto::on_pushButton_guardar_clicked()
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
    }break;
    case 1:{

    }break;
    }
}

void Producto::on_pushButton_eliminar_clicked()
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
    }break;
    case 1:{

    }break;
    }
}

void Producto::on_pushButton_salir_clicked()
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

void Producto::showEvent(QShowEvent* se)
{
    se->accept();

    afterShow = true;
}
void Producto::closeEvent(QCloseEvent* ce)
{
    ce->accept();

    emit closing();
}
bool Producto::eventFilter(QObject *obj, QEvent *e)
{
    QWidget* w_temp;
    w_temp = this;
    if(obj == w_temp){
        if(e->type() == QEvent::MouseButtonPress){
            if(focusWidget()){
                focusWidget()->setFocus();
            }else{
                ui->lineEdit_descripcion->setFocus();
                ui->lineEdit_descripcion->setCursorPosition(ui->lineEdit_descripcion->text().length());
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
                    ui->lineEdit_descripcion->setFocus();
                    ui->lineEdit_descripcion->setCursorPosition(ui->lineEdit_descripcion->text().length());
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
    w_temp = ui->lineEdit_codigo;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:
                ui->pushButton_buscar_tipo->setFocus(Qt::TabFocusReason);
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
    w_temp = ui->pushButton_buscar_tipo;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:
                ui->pushButton_buscar_tipo->click();
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
    w_temp = ui->pushButton_buscar_marca;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:
                ui->pushButton_buscar_marca->click();
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
    w_temp = ui->pushButton_buscar_unidad;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:
                ui->pushButton_buscar_unidad->click();
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
    w_temp = ui->pushButton_limpiar_tipo;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:
                ui->pushButton_limpiar_tipo->click();
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
    w_temp = ui->pushButton_limpiar_marca;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:
                ui->pushButton_limpiar_marca->click();
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
    w_temp = ui->pushButton_limpiar_unidad;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:
                ui->pushButton_limpiar_unidad->click();
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
    w_temp = ui->lineEdit_descripcion;
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

    return eventFilter(obj, e);
}

void Producto::on_pushButton_limpiar_tipo_clicked()
{
    id_tipo = "";
    ui->lineEdit_tipo->setText("");
}

void Producto::on_pushButton_limpiar_marca_clicked()
{
    id_marca = "";
    ui->lineEdit_marca->setText("");
}

void Producto::on_pushButton_limpiar_unidad_clicked()
{
    id_unidad = "";
    ui->lineEdit_unidad->setText("");
}



