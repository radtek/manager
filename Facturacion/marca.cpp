#include "marca.h"
#include "ui_marca.h"

Marca::Marca(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::Marca)
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
	ui->lineEdit_marca->installEventFilter(this);
    ui->pushButton_eliminar->installEventFilter(this);
	ui->pushButton_guardar->installEventFilter(this);	
	ui->pushButton_salir->installEventFilter(this);
}

Marca::~Marca()
{
    qDebug()<<"delete marca"<<endl;
	delete ui;
}
QString Marca::getID() {
	return id;
}
QString Marca::getMarca() {
	return ui->lineEdit_marca->text();
}
void Marca::set_widget_previous(QWidget* widget_previous)
{
	this->widget_previous = widget_previous;
}

void Marca::set_data(QString id, QString marca)
{
	ui->lineEdit_marca->setText(marca);
	this->id = id;
}

bool Marca::guardar()
{
	QSqlQuery query;

    SYSTEM->fixString(ui->lineEdit_marca);
	QString marca = ui->lineEdit_marca->text();
    marca = SYSTEM->replace_quotes(marca);
	
	qDebug() << marca << endl;

    QString str_query;
    QString id = this->id;
    if (id.compare("") == 0) {
        id = "NULL";

        str_query = "INSERT INTO marca(id, marca)VALUES";
        str_query += "(" + id + ", '" + marca + "')";
        str_query += "&&END_QUERY&&";
        str_query += "SELECT MAX(id) FROM marca";
        str_query += "&&END_QUERY&&";
    }else{
        str_query = "UPDATE marca SET marca = '" + marca + "'";
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
		return false;
	}
}
bool Marca::remove()
{
	QSqlQuery query;

    QString str_query = "DELETE FROM marca WHERE id = " + id;
    str_query += "&&END_QUERY&&";
    str_query += "COMMIT";
    str_query += "&&END_QUERY&&";

    SYSTEM->multiple_query(str_query);
	qDebug() << str_query << endl;
	if (query.exec(str_query)) {
		id = "";
		return true;
	}
	else {
		return false;
	}
}

void Marca::on_pushButton_guardar_clicked()
{
	int ret = QMessageBox::warning(this, "Advertencia", "¿Desea guardar los datos?", "Si", "No");
	switch (ret) {
	case 0: {
		if (guardar()) {
			QMessageBox::information(this, "Información", "Se guardaron los datos con éxito.", "Ok");
            setAttribute(Qt::WA_DeleteOnClose);
            SYSTEM->change_center_w(this, widget_previous);
		}
		else {
			QMessageBox::critical(this, "Error", "No se pudieron guardar los datos.");
		}
		return;
	}break;
	case 1: {

	}
	}
}

void Marca::on_pushButton_eliminar_clicked()
{
    if(id.compare("") == 0){
        QMessageBox::warning(this, "Advertencia", "Aún no puede eliminar.", "Ok");
        return;
    }

	int ret = QMessageBox::warning(this, "Advertencia", "Esta apunto de eliminar los datos. ¿Desea de todas formas eliminar los datos?", "Si", "No");
	switch (ret) {
	case 0: {
		if (remove()) {
            op = ELIMINAR;
			QMessageBox::information(this, "Información", "Se eliminaron los datos con éxito.");
            id = "";
            setAttribute(Qt::WA_DeleteOnClose);
            SYSTEM->change_center_w(this, widget_previous);
		}
		else {
			QMessageBox::critical(this, "Error", "No se pudieron eliminar los datos.");
		}
		return;
	}break;
	case 1: {

	}
	}
}

void Marca::on_pushButton_salir_clicked()
{
	int ret = QMessageBox::warning(this, "Advertencia", "¿Esta seguro que desea salir?", "Si", "No");

	switch (ret) {
	case 0: {
		id = "";
		if (widget_previous) {
            op = SALIR;
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
void Marca::showEvent(QShowEvent *se)
{
	se->accept();

    afterShow = true;
}
void Marca::closeEvent(QCloseEvent *ce)
{
	ce->accept();
	emit closing();
}

bool Marca::eventFilter(QObject *obj, QEvent *e)
{
	QWidget* w_temp;
	w_temp = this;
	if (obj == w_temp) {
        if(e->type() == QEvent::MouseButtonPress){
            if(focusWidget()){
                focusWidget()->setFocus();
            }else{
                ui->lineEdit_marca->setFocus();
                ui->lineEdit_marca->setCursorPosition(ui->lineEdit_marca->text().length());
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
                    ui->lineEdit_marca->setFocus();
                    ui->lineEdit_marca->setCursorPosition(ui->lineEdit_marca->text().length());
                }
                afterShow = false;
            }
            return true;
        }
		if (e->type() == QEvent::KeyPress) {
			QKeyEvent *KeyEvent = (QKeyEvent*)e;

			switch (KeyEvent->key())
			{
			case Qt::Key_Escape:
				ui->pushButton_salir->click();
                return true;
			}

        } else {

		}
		return false;
	}
	w_temp = ui->lineEdit_marca;
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
	w_temp = ui->pushButton_guardar;
	if (obj == w_temp) {
		if (e->type() == QEvent::KeyPress) {
			QKeyEvent *KeyEvent = (QKeyEvent*)e;

			switch (KeyEvent->key())
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

		}
		else {

		}
		return false;
	}	
	return false;
}
