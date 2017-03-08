#include "unidad.h"
#include "ui_unidad.h"

Unidad::Unidad(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::Unidad)
{
	ui->setupUi(this);

	id = "";

	widget_previous = NULL;

	//QRegExp regExp_in("[a-zA-Z ]{45,45}");
	//ui->lineEdit_tipo->setValidator(new QRegExpValidator(regExp_in));

	//connect(ui->pushButton_guardar, SIGNAL(clicked()), this, SLOT(on_pushButton_guardar_clicked()));
	//connect(ui->pushButton_eliminar, SIGNAL(clicked()), this, SLOT(on_pushButton_eliminar_clicked()));
	// INSTALL FILTERS
	this->installEventFilter(this);
	ui->lineEdit_unidad->installEventFilter(this);
	ui->pushButton_guardar->installEventFilter(this);
	ui->pushButton_eliminar->installEventFilter(this);
	ui->pushButton_salir->installEventFilter(this);
}

Unidad::~Unidad()
{
    qDebug()<<"delete unidad"<<endl;
	delete ui;
}
QString Unidad::getID() {
	return id;
}
QString Unidad::getUnidad() {
	return ui->lineEdit_unidad->text();
}
void Unidad::set_widget_previous(QWidget* widget_previous)
{
	this->widget_previous = widget_previous;
}

void Unidad::set_data(QString id, QString unidad)
{
	ui->lineEdit_unidad->setText(unidad);
	this->id = id;
}

bool Unidad::guardar()
{
	QSqlQuery query;

	SYSTEM->fixString(ui->lineEdit_unidad);
	QString unidad = ui->lineEdit_unidad->text();
	unidad = SYSTEM->replace_quotes(unidad);

    QString str_query;
    if (id.compare("") == 0) {
        id = "NULL";

        str_query = "INSERT INTO unidad(id, unidad)VALUES";
        str_query += "(" + id + ", '" + unidad + "')";
        str_query += "&&END_QUERY&&";
    }else{
        str_query = "UPDATE tipo SET unidad = '" + unidad + "'";
        str_query += " WHERE id = "+id;
        str_query += "&&END_QUERY&&";
    }
    str_query += "COMMIT";
    str_query += "&&END_QUERY&&";

	SYSTEM->multiple_query(str_query);
	qDebug() << str_query << endl;
	if (query.exec(str_query)) {
        //query.next();
        //this->id = query.value(0).toString();
		return true;
	}
	else {
		return false;
	}
}
bool Unidad::remove()
{
	QSqlQuery query;

    QString str_query = "DELETE FROM unidad WHERE id = " + id;
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

void Unidad::on_pushButton_guardar_clicked()
{
	int ret = QMessageBox::warning(this, "Advertencia", "¿Desea guardar los datos?", "Si", "No");
	switch (ret) {
	case 0: {
		if (guardar()) {
			QMessageBox::information(this, "Información", "Se guardaron los datos con éxito.", "Ok");

			if (widget_previous) {
                setAttribute(Qt::WA_DeleteOnClose);
				SYSTEM->change_center_w(this, widget_previous);
			} else {
				SYSTEM->clear_center_w(this);
			}
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

void Unidad::on_pushButton_eliminar_clicked()
{
    if(id.compare("") == 0) {
        QMessageBox::warning(this, "Advertencia", "No se puede eliminar aún.", "Ok");
        return;
    }

	int ret = QMessageBox::warning(this, "Advertencia", "Esta apunto de eliminar los datos. ¿Desea de todas formas eliminar los datos?", "Si", "No");
	switch (ret) {
	case 0: {
		if (remove()) {
			QMessageBox::information(this, "Información", "Se eliminaron los datos con éxito.");

			if (widget_previous) {
                id = "";
                setAttribute(Qt::WA_DeleteOnClose);
				SYSTEM->change_center_w(this, widget_previous);
			}
			else {
				SYSTEM->clear_center_w(this);
			}
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

void Unidad::on_pushButton_salir_clicked()
{
	int ret = QMessageBox::warning(this, "Advertencia", "¿Esta seguro que desea salir?", "Si", "No");

	switch (ret) {
	case 0: {

		if (widget_previous) {
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
void Unidad::showEvent(QShowEvent *se)
{
	se->accept();

	if (focusWidget()) {
		focusWidget()->setFocus();
	}
	else {
		ui->lineEdit_unidad->setFocus(Qt::TabFocusReason);
	}
}
void Unidad::closeEvent(QCloseEvent *ce)
{
	ce->accept();
	emit closing();
}

bool Unidad::eventFilter(QObject *obj, QEvent *e)
{
	QWidget* w_temp;
	w_temp = this;
	if (obj == w_temp) {
		if (e->type() == QEvent::KeyPress) {
			QKeyEvent *KeyEvent = (QKeyEvent*)e;

			switch (KeyEvent->key())
			{
			case Qt::Key_Escape:
				ui->pushButton_salir->click();
                return true;
			}

		}
		else {

		}
		return false;
	}
	w_temp = ui->lineEdit_unidad;
	if (obj == w_temp) {
		if (e->type() == QEvent::KeyPress) {
			QKeyEvent *KeyEvent = (QKeyEvent*)e;

			switch (KeyEvent->key())
			{
			case Qt::Key_Return: {
				ui->pushButton_guardar->click();
                return true;
			}

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
			}

		}
		else {

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
			}

		}
		else {

		}
		return false;
	}
	return false;
}
