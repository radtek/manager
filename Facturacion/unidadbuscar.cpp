#include "unidadbuscar.h"
#include "ui_unidadbuscar.h"

UnidadBuscar::UnidadBuscar(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::UnidadBuscar)
{
	ui->setupUi(this);

    firstShow = false;
    afterShow = false;

    widget_previous = NULL;

	id = "";
	unidad = "";

	pos = 0;
	size_query = 10;

	//connect(ui->lineEdit_tipo_buscar, SIGNAL(textChanged(QString)), this, SLOT(on_lineEdit_tipo_buscar_textChanged(QString)));
	//connect(ui->lineEdit_tipo_buscar, SIGNAL(returnPressed()), this, SLOT(on_lineEdit_tipo_buscar_returnPressed()));

	//connect(ui->pushButton_opes, SIGNAL(clicked()), this, SLOT(on_pushButton_opes_clicked()));

    //disconnect(ui->lineEdit_unidad_buscar, SIGNAL(returnPressed()), this, SLOT(on_lineEdit_unidad_buscar_returnPressed()));

	// INSTALL FILTERS
	this->installEventFilter(this);
	ui->lineEdit_unidad_buscar->installEventFilter(this);
	ui->tableWidget->installEventFilter(this);
	ui->pushButton_ok->installEventFilter(this);
	ui->pushButton_salir->installEventFilter(this);
	ui->pushButton_nuevo->installEventFilter(this);
    ui->pushButton_editar->installEventFilter(this);
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

    int op = widget_unidad->getOp();
    switch(op){
    case INGRESAR:{
        pos = 0;

        ui->tableWidget->setRowCount(0);
        ui->tableWidget->setColumnCount(0);
        ui->tableWidget->clear();

        int rowCount = 1;
        ui->tableWidget->setRowCount(rowCount);

        int columnCount = 2;
        ui->tableWidget->setColumnCount(columnCount);

        ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "ID" << "Unidad");
        ui->tableWidget->setColumnHidden(0, true);

        QString id = widget_unidad->getID();
        QString unidad = widget_unidad->getUnidad();

        ui->tableWidget->setItem(pos, 0, new QTableWidgetItem(id));
        ui->tableWidget->setItem(pos, 1, new QTableWidgetItem(unidad));

        for(int j=0; j<ui->tableWidget->columnCount(); j++)
            ui->tableWidget->item(pos, j)->setFlags(Qt::ItemIsEnabled
                                                         | Qt::ItemIsSelectable);

        pos++;
        ui->tableWidget->setFocus();
        ui->tableWidget->selectRow(0);
        for(int j=0; j<ui->tableWidget->columnCount(); j++){
            ui->tableWidget->item(0, j)->setSelected(true);
        }
        SYSTEM->table_resize_to_contents(0, ui->tableWidget, size_query);
    }break;
    case MODIFICAR:{
        QString id = widget_unidad->getID();
        QString unidad = widget_unidad->getUnidad();

        QTableWidgetItem* item = ui->tableWidget->currentItem();
        int row = item->row();
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(id));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(unidad));

        SYSTEM->table_resize_to_contents(0, ui->tableWidget, size_query);
    }break;
    case ELIMINAR:{
        QTableWidgetItem* item = ui->tableWidget->currentItem();
        ui->tableWidget->removeRow(item->row());
    }break;
    case SALIR:{

    }break;
    }
}
void UnidadBuscar::set_buscar()
{
    QString unidad = ui->lineEdit_unidad_buscar->text();
    QString str_query = "SELECT id, unidad FROM unidad WHERE unidad LIKE '%" + unidad + "%'";
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

            for(int j=0; j<ui->tableWidget->columnCount(); j++)
                ui->tableWidget->item(pos, j)->setFlags(Qt::ItemIsEnabled
                                                             | Qt::ItemIsSelectable);
            ++pos;
        }
        SYSTEM->table_resize_to_contents(0, ui->tableWidget, size_query);
    } else {

    }
}
void UnidadBuscar::on_lineEdit_unidad_buscar_textChanged(const QString& arg)
{
	pos = 0;

	ui->tableWidget->setRowCount(0);
	ui->tableWidget->setColumnCount(0);
	ui->tableWidget->clear();

    set_buscar();
}
void UnidadBuscar::on_lineEdit_unidad_buscar_returnPressed()
{
    qDebug()<<"return pressed"<<endl;
    pos = 0;

    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(0);
    ui->tableWidget->clear();

    set_buscar();
}
void UnidadBuscar::on_pushButton_nuevo_clicked()
{
    //int ret = QMessageBox::warning(this, "Advertencia", "¿Desea agregar un UNIDAD?", "Si", "No");
    //switch (ret) {
    //case 0: {
		Unidad* w = new Unidad;
		w->set_widget_previous(this);
		connect(w, SIGNAL(closing()), this, SLOT(on_unidad_closing()));
		SYSTEM->change_center_w(this, w);

    //}break;
    //case 1: {

    //}break;
    //}
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
    }else{
        QTableWidgetItem* item = ui->tableWidget->currentItem();
        if(item)
            on_tableWidget_itemDoubleClicked(item);
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
void UnidadBuscar::on_pushButton_editar_clicked()
{
    QTableWidgetItem* item = ui->tableWidget->currentItem();

    editarItem(item);
}
void UnidadBuscar::on_tableWidget_itemDoubleClicked(QTableWidgetItem *item)
{
    if(widget_previous){
        on_pushButton_ok_clicked();
    }else{
        editarItem(item);
    }
}
void UnidadBuscar::showEvent(QShowEvent *se)
{
	se->accept();

    afterShow = true;

    if(!firstShow){
        on_lineEdit_unidad_buscar_textChanged(ui->lineEdit_unidad_buscar->text());
        //on_lineEdit_unidad_buscar_returnPressed();
        firstShow = true;
    }
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
        if(e->type() == QEvent::MouseButtonPress){
            if(focusWidget()){
                focusWidget()->setFocus();
            }else{
                ui->lineEdit_unidad_buscar->setFocus();
                ui->lineEdit_unidad_buscar->setCursorPosition(ui->lineEdit_unidad_buscar->text().length());
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
                    if(focusWidget() == ui->pushButton_nuevo){
                        ui->lineEdit_unidad_buscar->setFocus();
                        ui->lineEdit_unidad_buscar->setCursorPosition(ui->lineEdit_unidad_buscar->text().length());
                    }else{
                        focusWidget()->setFocus();
                    }
                }else{
                    ui->lineEdit_unidad_buscar->setFocus();
                    ui->lineEdit_unidad_buscar->setCursorPosition(ui->lineEdit_unidad_buscar->text().length());
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
			}

        } else {

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
                on_lineEdit_unidad_buscar_returnPressed();
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
	w_temp = ui->tableWidget;
	if (obj == w_temp) {
		if (e->type() == QEvent::KeyPress) {
			QKeyEvent *KeyEvent = (QKeyEvent*)e;

			switch (KeyEvent->key())
			{
			case Qt::Key_Return:
				ui->pushButton_ok->click();
                return true;
            case Qt::Key_Enter:{
                QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QApplication::sendEvent(w_temp, key);
                return true;
            }break;
			case Qt::Key_Down: {
				int index = ui->tableWidget->currentRow();
				if (index == ui->tableWidget->rowCount() - 1) {
					on_lineEdit_unidad_buscar_returnPressed();
                    return true;
				}
            }break;
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
	w_temp = ui->pushButton_nuevo;
	if (obj == w_temp) {
		if (e->type() == QEvent::KeyPress) {
			QKeyEvent *KeyEvent = (QKeyEvent*)e;

			switch (KeyEvent->key())
			{
			case Qt::Key_Return: {
				ui->pushButton_nuevo->click();
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
    w_temp = ui->pushButton_editar;
    if (obj == w_temp) {
        if (e->type() == QEvent::KeyPress) {
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch (KeyEvent->key())
            {
            case Qt::Key_Return: {
                ui->pushButton_editar->click();
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
void UnidadBuscar::editarItem(QTableWidgetItem *item)
{
    if(!item){
        QMessageBox::warning(this, "Advertencia", "Selecciona un item de la tabla.", "Ok");
        return;
    }

    QTableWidget* table = ui->tableWidget;

    Unidad* w = new Unidad;
    w->set_widget_previous(this);

    QString id = table->item(item->row(), 0)->text();
    QString unidad = table->item(item->row(), 1)->text();

    w->set_data(id, unidad);

    connect(w, SIGNAL(closing()), this, SLOT(on_unidad_closing()));
    SYSTEM->change_center_w(this, w);
}
