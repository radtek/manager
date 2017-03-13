#include "productoformtransaction.h"
#include "ui_productoformtransaction.h"

ProductoFormTransaction::ProductoFormTransaction(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProductoFormTransaction)
{
    ui->setupUi(this);

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

    ui->lineEdit_precio->installEventFilter(this);
    ui->lineEdit_cantidad->installEventFilter(this);

    ui->pushButton_guardar->installEventFilter(this);
    ui->pushButton_salir->installEventFilter(this);    
    ui->pushButton_eliminar->installEventFilter(this);
}

ProductoFormTransaction::~ProductoFormTransaction()
{
    qDebug()<<"delete producto form transaction"<<endl;
    delete ui;
}
QString ProductoFormTransaction::get_ID()
{
	return id;
}
QString ProductoFormTransaction::get_IDTipo()
{
	return id_tipo;
}
QString ProductoFormTransaction::get_IDMarca()
{
	return id_marca;
}
QString ProductoFormTransaction::get_IDUnidad()
{
	return id_unidad;
}
QString ProductoFormTransaction::get_codigo()
{
	return ui->lineEdit_codigo->text();
}
QString ProductoFormTransaction::get_tipo()
{
	return ui->lineEdit_tipo->text();
}
QString ProductoFormTransaction::get_descripcion()
{
	return ui->lineEdit_descripcion->text();
}
QString ProductoFormTransaction::get_marca()
{
	return ui->lineEdit_marca->text();
}
QString ProductoFormTransaction::get_unidad()
{
	return ui->lineEdit_unidad->text();
}
QString ProductoFormTransaction::get_precio()
{
    return ui->lineEdit_precio->text();
}
QString ProductoFormTransaction::get_cantidad()
{
    return ui->lineEdit_cantidad->text();
}
void ProductoFormTransaction::set_widget_previous(QWidget *w)
{
    this->widget_previous = w;
}
void ProductoFormTransaction::set_data(QString id, QString id_tipo, QString id_marca, QString id_unidad
	, QString codigo, QString tipo, QString descripcion, QString marca, QString unidad
	, QString precio, QString cantidad)
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
    ui->lineEdit_precio->setText(precio);
    ui->lineEdit_cantidad->setText(cantidad);
}
bool ProductoFormTransaction::guardar()
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
    if(this->id.compare("") == 0) {
        id = "NULL";

        str_query = "INSERT INTO producto(id, codigo, tipo_id, marca_id, unidad_id, descripcion, precio, cantidad, habilitado)VALUES";
        str_query += QString() + "(" + id + ", '" + codigo + "', " + id_tipo;
        str_query += ", "+id_marca+", "+id_unidad+", '"+nombre+"', '"+ ui->lineEdit_precio->text() + "', '" + ui->lineEdit_cantidad->text() +"', 1)";
        str_query += "&&END_QUERY&&";
    }else{
        str_query = "UPDATE producto";
        str_query += " SET codigo = '"+codigo+"'";
        str_query += ", tipo_id = "+id_tipo+"";
        str_query += ", marca_id = "+id_marca;
        str_query += ", unidad_id = "+id_unidad;
        str_query += ", descripcion = '"+nombre+"'";
        str_query += ", precio = '"+ui->lineEdit_precio->text()+"'";
        str_query += ", cantidad = '"+ui->lineEdit_cantidad->text()+"'";
        str_query += ", habilitado = 1";
        str_query += " WHERE id = "+id;
        str_query += "&&END_QUERY&&";
    }
    str_query += "COMMIT";
    str_query += "&&END_QUERY&&";

    SYSTEM->multiple_query(str_query);
    qDebug()<<str_query<<endl;
    if(query.exec(str_query)){		
        return true;
    }else{
        return false;
    }
}
bool ProductoFormTransaction::remove()
{
    QSqlQuery query;

    QString str_query = "DELETE FROM producto WHERE id = "+id;
    str_query += "&&END_QUERY&&";
    str_query += "COMMIT";
    str_query += "&&END_QUERY&&";

    SYSTEM->multiple_query(str_query);
    qDebug()<<str_query<<endl;
    if(query.exec(str_query)){
        return true;
    }else{
        return false;
    }
}

void ProductoFormTransaction::on_pushButton_buscar_tipo_clicked()
{
	TipoBuscar* w = new TipoBuscar;
    w->set_widget_previous(this);
	connect(w, SIGNAL(closing()), this, SLOT(on_tipoBuscar_closing()));	
    SYSTEM->change_center_w(this, w);
}
void ProductoFormTransaction::on_tipoBuscar_closing()
{
	TipoBuscar* tBuscar = (TipoBuscar*)sender();
	
	QString id = tBuscar->getID();
	if (id.compare("") == 0) {
		return;
	}
	QString tipo = tBuscar->getTipo();

	this->id_tipo = id;
	ui->lineEdit_tipo->setText(tipo);

	ui->pushButton_buscar_marca->setFocus(Qt::TabFocusReason);
}
void ProductoFormTransaction::on_pushButton_buscar_marca_clicked()
{
	MarcaBuscar* w = new MarcaBuscar;
    w->set_widget_previous(this);
	connect(w, SIGNAL(closing()), this, SLOT(on_marcaBuscar_closing()));    
    SYSTEM->change_center_w(this, w);
}
void ProductoFormTransaction::on_marcaBuscar_closing()
{
	MarcaBuscar* mBuscar = (MarcaBuscar*)sender();

	QString id = mBuscar->getID();
	if (id.compare("") == 0) {
		return;
	}
	QString marca = mBuscar->getMarca();

	this->id_marca = id;
	ui->lineEdit_marca->setText(marca);

	ui->pushButton_buscar_unidad->setFocus(Qt::TabFocusReason);
}
void ProductoFormTransaction::on_pushButton_buscar_unidad_clicked()
{
	UnidadBuscar* w = new UnidadBuscar;
	w->set_widget_previous(this);
	connect(w, SIGNAL(closing()), this, SLOT(on_unidadBuscar_closing()));
    SYSTEM->change_center_w(this, w);
}

void ProductoFormTransaction::on_unidadBuscar_closing()
{
	UnidadBuscar* mBuscar = (UnidadBuscar*)sender();

	QString id = mBuscar->getID();
	if (id.compare("") == 0) {
		return;
	}
	QString unidad = mBuscar->getUnidad();

	this->id_unidad = id;
	ui->lineEdit_unidad->setText(unidad);

	ui->lineEdit_descripcion->setFocus(Qt::TabFocusReason);
}

void ProductoFormTransaction::on_pushButton_guardar_clicked()
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
    }break;
    case 1:{

    }break;
    }
}

void ProductoFormTransaction::on_pushButton_eliminar_clicked()
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
    }break;
    case 1:{

    }break;
    }
}

void ProductoFormTransaction::on_pushButton_salir_clicked()
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

void ProductoFormTransaction::showEvent(QShowEvent* se)
{    
    se->accept();

    afterShow = true;
}
void ProductoFormTransaction::closeEvent(QCloseEvent* ce)
{
	ce->accept();

	emit closing();
}
bool ProductoFormTransaction::eventFilter(QObject *obj, QEvent *e)
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
                ui->lineEdit_precio->setFocus(Qt::TabFocusReason);
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
    w_temp = ui->lineEdit_precio;
	if (obj == w_temp) {        
		if (e->type() == QEvent::KeyPress) {
            qDebug()<<"KeyPress"<<endl;
			QKeyEvent *KeyEvent = (QKeyEvent*)e;
			switch (KeyEvent->key())
			{
            case Qt::Key_Return:{
                ui->lineEdit_cantidad->setFocus(Qt::TabFocusReason);
                return true;
			}break;
            case Qt::Key_Enter:{
                QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QApplication::sendEvent(w_temp, key);
                return true;
            }break;
            case Qt::Key_Period:{
                QString str = ui->lineEdit_precio->text();
                int periodPosition = str.indexOf('.');
                int cursorPosition;
                if(periodPosition == -1){
                    cursorPosition = ui->lineEdit_precio->cursorPosition();
                    str.insert(cursorPosition, '.');
                    cursorPosition += 1;
                }else{
                    cursorPosition = ui->lineEdit_precio->cursorPosition();
                    str.replace(".", " ");
                    str.insert(ui->lineEdit_precio->cursorPosition(), '.');
                    str.replace(" ", "");
                    if(periodPosition >= cursorPosition)
                        cursorPosition += 1;
                }
                ui->lineEdit_precio->setText(str);
                ui->lineEdit_precio->setCursorPosition(cursorPosition);
                return true;
            }break;
			}

        } else {

		}
        if(e->type() == QEvent::FocusOut){

        }
		return false;
	}
    w_temp = ui->lineEdit_cantidad;
	if (obj == w_temp) {
		if (e->type() == QEvent::KeyPress) {
			QKeyEvent *KeyEvent = (QKeyEvent*)e;

			switch (KeyEvent->key())
			{
			case Qt::Key_Return: {
				ui->pushButton_guardar->click();
                return true;
			}break;
            case Qt::Key_Enter:{
                QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QApplication::sendEvent(w_temp, key);
                return true;
            }break;
            case Qt::Key_Period:{
                QString str = ui->lineEdit_cantidad->text();
                int periodPosition = str.indexOf('.');
                int cursorPosition;
                if(periodPosition == -1){
                    cursorPosition = ui->lineEdit_cantidad->cursorPosition();
                    str.insert(cursorPosition, '.');
                    cursorPosition += 1;
                }else{
                    cursorPosition = ui->lineEdit_cantidad->cursorPosition();
                    str.replace(".", " ");
                    str.insert(ui->lineEdit_cantidad->cursorPosition(), '.');
                    str.replace(" ", "");
                    if(periodPosition >= cursorPosition)
                        cursorPosition += 1;
                }
                ui->lineEdit_cantidad->setText(str);
                ui->lineEdit_cantidad->setCursorPosition(cursorPosition);
                return true;
            }break;
			}

        } else {

		}
        if(e->type() == QEvent::FocusOut){

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

void ProductoFormTransaction::on_pushButton_limpiar_tipo_clicked()
{
    id_tipo = "";
    ui->lineEdit_tipo->setText("");
}

void ProductoFormTransaction::on_pushButton_limpiar_marca_clicked()
{
    id_marca = "";
    ui->lineEdit_marca->setText("");
}

void ProductoFormTransaction::on_pushButton_limpiar_unidad_clicked()
{
    id_unidad = "";
    ui->lineEdit_unidad->setText("");
}



