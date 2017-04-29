#include "tipo.h"
#include "ui_tipo.h"

Tipo::Tipo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Tipo)
{
    ui->setupUi(this);

	id = "";

    afterShow = false;

	widget_previous = NULL;

    op = -1;

	//QRegExp regExp_in("[a-zA-Z ]{45,45}");
	//ui->lineEdit_tipo->setValidator(new QRegExpValidator(regExp_in));

	//connect(ui->pushButton_guardar, SIGNAL(clicked()), this, SLOT(on_pushButton_guardar_clicked()));
	//connect(ui->pushButton_eliminar, SIGNAL(clicked()), this, SLOT(on_pushButton_eliminar_clicked()));
	// INSTALL FILTERS
	this->installEventFilter(this);
	ui->lineEdit_tipo->installEventFilter(this);
	ui->pushButton_guardar->installEventFilter(this);
	ui->pushButton_eliminar->installEventFilter(this);
	ui->pushButton_salir->installEventFilter(this);
}

Tipo::~Tipo()
{
    qDebug()<<"delete tipo"<<endl;
    delete ui;
}
QString Tipo::getID() {
	return id; 
}
QString Tipo::getTipo() {
	return ui->lineEdit_tipo->text();
}
void Tipo::set_widget_previous(QWidget* widget_previous)
{
    this->widget_previous = widget_previous;
}

void Tipo::set_data(QString id, QString tipo)
{
	ui->lineEdit_tipo->setText(tipo);
	this->id = id;
}

bool Tipo::guardar()
{
	QSqlQuery query;

	SYSTEM->fixString(ui->lineEdit_tipo);
	QString tipo = ui->lineEdit_tipo->text();
	tipo = SYSTEM->replace_quotes(tipo);

    QString str_query;
    QString id = this->id;

    if (id.compare("") == 0) {
        id = "NULL";

        str_query += "INSERT INTO tipo(id, tipo)VALUES";
        str_query += "(" + id + ", '" + tipo + "')";
        str_query += "&&END_QUERY&&";
        str_query += "SELECT MAX(id) FROM tipo";
        str_query += "&&END_QUERY&&";
    }else{
        str_query += "UPDATE tipo SET tipo = '" + tipo + "'";
        str_query += " WHERE id = "+id;
        str_query += "&&END_QUERY&&";
    }
    str_query += "COMMIT";
    str_query += "&&END_QUERY&&";

	SYSTEM->multiple_query(str_query);
	qDebug() << str_query << endl;
	if (query.exec(str_query)) {
        if(this->id.compare("") == 0){
            op = INGRESAR;
            query.next();
            this->id = query.value(0).toString();
        }else
            op = MODIFICAR;
		return true;
	} else {
        if(query.exec("ROLLBACK")){

        }else{

        }
		return false;
	}
}
bool Tipo::remove()
{
    QSqlQuery query;

    QString str_query;

    str_query += "DELETE FROM tipo WHERE id = "+id;
    str_query += "&&END_QUERY&&";
    str_query += "COMMIT";
    str_query += "&&END_QUERY&&";

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

void Tipo::on_pushButton_guardar_clicked()
{
	int ret = QMessageBox::warning(this, "Advertencia", "¿Desea guardar los datos?", "Si", "No");
	switch (ret) {
	case 0: {
		if (guardar()) {
            //QMessageBox::information(this, "Información", "Se guardaron los datos con éxito.", "Ok");
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
	case 1: {

	}
	}
}

void Tipo::on_pushButton_eliminar_clicked()
{
    if(id.compare("") == 0) {
        QMessageBox::warning(this, "Advertencia", "No se puede eliminar aún.", "Ok");
        return;
    }

	int ret = QMessageBox::warning(this, "Advertencia", "Esta apunto de eliminar los datos. ¿Desea de todas formas eliminar los datos?", "Si", "No");
	switch (ret) {
	case 0: {
		if (remove()) {
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
	case 1: {

	}
	}
}

void Tipo::on_pushButton_salir_clicked()
{
	int ret = QMessageBox::warning(this, "Advertencia", "¿Esta seguro que desea salir?", "Si", "No");

	switch (ret) {
	case 0: {

		if (widget_previous) {
            op = SALIR;
            id = "";
            setAttribute(Qt::WA_DeleteOnClose);
			SYSTEM->change_center_w(this, widget_previous);
		} else {
			SYSTEM->clear_center_w(this);
		}
	}
	case 1: {

	}
	}
}
void Tipo::showEvent(QShowEvent *se)
{
	se->accept();

    afterShow = true;
}
void Tipo::closeEvent(QCloseEvent *ce)
{
	ce->accept();
	emit closing();
}

bool Tipo::eventFilter(QObject *obj, QEvent *e)
{
    QWidget* w_temp;
    w_temp = this;
    if(obj == w_temp){
        if(e->type() == QEvent::MouseButtonPress){
            if(focusWidget()){
                focusWidget()->setFocus();
            }else{
                ui->lineEdit_tipo->setFocus();
                ui->lineEdit_tipo->setCursorPosition(ui->lineEdit_tipo->text().length());
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
                    ui->lineEdit_tipo->setFocus();
                    ui->lineEdit_tipo->setCursorPosition(ui->lineEdit_tipo->text().length());
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
    w_temp = ui->lineEdit_tipo;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:{
				ui->pushButton_guardar->click();
                return true;
            case Qt::Key_Enter:{
                QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QApplication::sendEvent(w_temp, key);
                return true;
            }break;
			}

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
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {          
            case Qt::Key_Tab: {
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
    return false;
}
