#include "unidadbuscar.h"
#include "ui_unidadbuscar.h"

UnidadBuscar::UnidadBuscar(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::UnidadBuscar)
{
	ui->setupUi(this);

	id = "";
	unidad = "";

	pos = 0;
	size_query = 10;

	//connect(ui->lineEdit_tipo_buscar, SIGNAL(textChanged(QString)), this, SLOT(on_lineEdit_tipo_buscar_textChanged(QString)));
	//connect(ui->lineEdit_tipo_buscar, SIGNAL(returnPressed()), this, SLOT(on_lineEdit_tipo_buscar_returnPressed()));

	//connect(ui->pushButton_opes, SIGNAL(clicked()), this, SLOT(on_pushButton_opes_clicked()));

	// INSTALL FILTERS
	this->installEventFilter(this);
	ui->lineEdit_unidad_buscar->installEventFilter(this);
	ui->tableWidget->installEventFilter(this);
	ui->pushButton_ok->installEventFilter(this);
	ui->pushButton_salir->installEventFilter(this);
	ui->pushButton_nuevo->installEventFilter(this);
	ui->pushButton_modificar->installEventFilter(this);
}

UnidadBuscar::~UnidadBuscar()
{
	qDebug() << "delete unidad buscar" << endl;
	delete ui;
}
QString UnidadBuscar::getID()
{
	return id;
}
QString UnidadBuscar::getUnidad()
{
	return unidad;
}
void UnidadBuscar::set_widget_previous(QWidget *widget_previous)
{
	this->widget_previous = widget_previous;
}
void UnidadBuscar::on_unidad_closing()
{
	Unidad* widget_unidad = (Unidad*)QObject::sender();
	id = widget_unidad->getID();
	unidad = widget_unidad->getUnidad();

	on_lineEdit_unidad_buscar_textChanged("");
}
void UnidadBuscar::on_lineEdit_unidad_buscar_textChanged(const QString& arg)
{
    connect(ui->lineEdit_unidad_buscar, SIGNAL(returnPressed()), this, SLOT(on_lineEdit_unidad_buscar_returnPressed()));
	pos = 0;

	ui->tableWidget->setRowCount(0);
	ui->tableWidget->setColumnCount(0);
	ui->tableWidget->clear();
}
void UnidadBuscar::on_lineEdit_unidad_buscar_returnPressed()
{
    disconnect(ui->lineEdit_unidad_buscar, SIGNAL(returnPressed()), this, SLOT(on_lineEdit_unidad_buscar_returnPressed()));

	QString unidad = ui->lineEdit_unidad_buscar->text();
	QString str_query = "SELECT id, unidad FROM unidad WHERE unidad LIKE '" + unidad + "%'";
	str_query += " LIMIT " + QString().setNum(pos) + ", " + QString().setNum(size_query) + "";

	QSqlQuery query;
	qDebug() << str_query << endl;
	if (query.exec(str_query)) {
		int rowCount = ui->tableWidget->rowCount();
		ui->tableWidget->setRowCount(rowCount + query.size());

		int columnCount = query.record().count();
		ui->tableWidget->setColumnCount(columnCount);

		ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "ID" << "Unidad");
		ui->tableWidget->setColumnHidden(0, true);

		while (query.next()) {
			QString id = query.value(0).toString();
			QString unidad = query.value(1).toString();
			ui->tableWidget->setItem(pos, 0, new QTableWidgetItem(id));
			ui->tableWidget->setItem(pos, 1, new QTableWidgetItem(unidad));

			SYSTEM->table_resize_to_contents(0, ui->tableWidget, size_query);
			++pos;
		}
	} else {

	}
}
void UnidadBuscar::on_pushButton_nuevo_clicked()
{
	int ret = QMessageBox::warning(this, "Advertencia", "¿Desea agregar un UNIDAD?", "Si", "No");
	switch (ret) {
	case 0: {
		Unidad* w = new Unidad;
		w->set_widget_previous(this);
		connect(w, SIGNAL(closing()), this, SLOT(on_unidad_closing()));
		SYSTEM->change_center_w(this, w);

	}break;
	case 1: {

	}break;
	}
}
void UnidadBuscar::on_pushButton_modificar_clicked()
{
	QTableWidget* table = ui->tableWidget;
	QTableWidgetItem* item = table->currentItem();
	if (!item) {
		int ret = QMessageBox::warning(this, "Advertencia", "Seleccione unidad.", "Ok");
		return;
	}

	int ret = QMessageBox::warning(this, "Advertencia", "¿Esta seguro de modificar el item?", "Si", "No");
	switch (ret) {
	case 0: {
		Unidad* w = new Unidad;
		w->set_widget_previous(this);

		QString id = table->item(item->row(), 0)->text();
		QString unidad = table->item(item->row(), 1)->text();

		w->set_data(id, unidad);

		connect(w, SIGNAL(closing()), this, SLOT(on_unidad_closing()));
		SYSTEM->change_center_w(this, w);

	}break;
	case 1: {

	}break;
	}
}
void UnidadBuscar::on_pushButton_ok_clicked()
{
	QTableWidget* tb = ui->tableWidget;
	QTableWidgetItem* item = tb->currentItem();

	if (!item)return;

	if (widget_previous) {
		int row = item->row();

		id = tb->item(row, 0)->text();
		unidad = tb->item(row, 1)->text();

        setAttribute(Qt::WA_DeleteOnClose);
		SYSTEM->change_center_w(this, widget_previous);
	}
	else {
		SYSTEM->clear_center_w(this);
	}
}

void UnidadBuscar::on_pushButton_salir_clicked()
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
void UnidadBuscar::showEvent(QShowEvent *se)
{
	se->accept();

	ui->lineEdit_unidad_buscar->setFocus(Qt::TabFocusReason);
}
void UnidadBuscar::closeEvent(QCloseEvent * ce)
{
	ce->accept();
	emit closing();
}
bool UnidadBuscar::eventFilter(QObject *obj, QEvent *e)
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
			case Qt::Key_Up: {
				if (this->focusWidget() != ui->tableWidget) {
					ui->tableWidget->setFocus(Qt::TabFocusReason);
					if (ui->tableWidget->currentItem())
						ui->tableWidget->currentItem()->setSelected(true);
				}
            }break;
			case Qt::Key_Down: {
				if (this->focusWidget() != ui->tableWidget) {
					ui->tableWidget->setFocus(Qt::TabFocusReason);
					if (ui->tableWidget->currentItem())
						ui->tableWidget->currentItem()->setSelected(true);
				}
            }break;
            case Qt::Key_F3:
                ui->pushButton_nuevo->setFocus(Qt::TabFocusReason);
                ui->pushButton_nuevo->click();
                return true;
            case Qt::Key_F4:
                ui->pushButton_modificar->setFocus(Qt::TabFocusReason);
                ui->pushButton_modificar->click();
                return true;
			}

		}
		else {

		}
		return false;
	}
	w_temp = ui->lineEdit_unidad_buscar;
	if (obj == w_temp) {
		if (e->type() == QEvent::KeyPress) {
			QKeyEvent *KeyEvent = (QKeyEvent*)e;

			switch (KeyEvent->key())
			{
			case Qt::Key_Return: {
				//ui->tableWidget->setFocus();
				//ui->tableWidget->selectRow(0);
			}break;
			}
		}
		else {

		}
		return false;
	}
	w_temp = ui->tableWidget;
	if (obj == w_temp) {
		if (e->type() == QEvent::KeyPress) {
			QKeyEvent *KeyEvent = (QKeyEvent*)e;

			switch (KeyEvent->key())
			{
			case Qt::Key_Return:
				ui->pushButton_ok->click();
                return true;
			case Qt::Key_Down: {
				int index = ui->tableWidget->currentRow();
				if (index == ui->tableWidget->rowCount() - 1) {
					on_lineEdit_unidad_buscar_returnPressed();
				}
			}
			}

		}
		else {

		}
		return false;
	}
	else {

	}
	w_temp = ui->pushButton_ok;
	if (obj == w_temp) {
		if (e->type() == QEvent::KeyPress) {
			QKeyEvent *KeyEvent = (QKeyEvent*)e;

			switch (KeyEvent->key())
			{
			case Qt::Key_Return:
				ui->pushButton_ok->click();
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
	w_temp = ui->pushButton_nuevo;
	if (obj == w_temp) {
		if (e->type() == QEvent::
			KeyPress) {
			QKeyEvent *KeyEvent = (QKeyEvent*)e;

			switch (KeyEvent->key())
			{
			case Qt::Key_Return: {
				ui->pushButton_nuevo->click();
                return true;
			}break;
			}

		}
		else {

		}
		return false;
	}
	w_temp = ui->pushButton_modificar;
	if (obj == w_temp) {
		if (e->type() == QEvent::
			KeyPress) {
			QKeyEvent *KeyEvent = (QKeyEvent*)e;

			switch (KeyEvent->key())
			{			
			case Qt::Key_Return: {
				ui->pushButton_modificar->click();
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