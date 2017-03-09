#include "productobuscar.h"
#include "ui_productobuscar.h"

ProductoBuscar::ProductoBuscar(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::ProductoBuscar)
{
	ui->setupUi(this);

	// Inicializando datos miembro
	widget_previous = NULL;	

	pos = 0;
	size_query = 10;

	id = "";
	id_tipo = "";
	id_marca = "";
	id_unidad = "";

	codigo = "";
	tipo = "";
	descripcion = "";
	marca = "";
	unidad = "";
	precio = "";
	cantidad = "";        

	this->installEventFilter(this);

	ui->lineEdit_codigo->installEventFilter(this);
	ui->lineEdit_tipo_buscar->installEventFilter(this);
	ui->lineEdit_marca_buscar->installEventFilter(this);
	ui->lineEdit_unidad_buscar->installEventFilter(this);
	ui->lineEdit_descripcion_buscar->installEventFilter(this);

	ui->tableWidget->installEventFilter(this);

	ui->pushButton_ok->installEventFilter(this);
	ui->pushButton_salir->installEventFilter(this);

    ui->pushButton_nuevo->installEventFilter(this);
}

ProductoBuscar::~ProductoBuscar()
{
    qDebug()<<"delete producto buscar"<<endl;
    delete ui;
}
QString ProductoBuscar::getID()
{
	return id;
}
QString ProductoBuscar::getIDTipo()
{
	return id_tipo;
}
QString ProductoBuscar::getIDMarca()
{
	return id_marca;
}
QString ProductoBuscar::getIDUnidad()
{
	return id_unidad;
}
QString ProductoBuscar::getTipo()
{
    return tipo;
}
QString ProductoBuscar::getMarca()
{
    return marca;
}
QString ProductoBuscar::getUnidad()
{
    return unidad;
}
QString ProductoBuscar::getCodigo()
{
	return codigo;
}
QString ProductoBuscar::getDescripcion()
{
	return descripcion;
}
QString ProductoBuscar::getPrecio()
{
	return precio;
}
QString ProductoBuscar::getCantidad()
{
	return cantidad;
}
void ProductoBuscar::set_widget_previous(QWidget *widget_previous)
{
    this->widget_previous = widget_previous;
}

void ProductoBuscar::on_productoFormTransaction_closing()
{
	ProductoFormTransaction* widget_prod = (ProductoFormTransaction*)QObject::sender();
	id = widget_prod->get_ID();
	id_tipo = widget_prod->get_IDTipo();
	id_marca = widget_prod->get_IDMarca();
	id_unidad = widget_prod->get_IDUnidad();
	codigo = widget_prod->get_codigo();
	tipo = widget_prod->get_tipo();
	descripcion = widget_prod->get_descripcion();
	marca = widget_prod->get_marca();
	unidad = widget_prod->get_unidad();
	precio = widget_prod->get_precio();
	cantidad = widget_prod->get_cantidad();
}
void ProductoBuscar::on_pushButton_nuevo_clicked()
{
    int ret = QMessageBox::warning(this, "Advertencia", "¿Desea ingresar un nuevo PRODUCTO?", "Si", "No");
    switch(ret){
    case 0:{
        ProductoFormTransaction* w = new ProductoFormTransaction;
		connect(w, SIGNAL(closing()), this, SLOT(on_productoFormTransaction_closing()));
        w->set_widget_previous(this);

        SYSTEM->change_center_w(this, w);        
    }break;
    case 1:{

    }break;
    }
}

void ProductoBuscar::on_pushButton_ok_clicked()
{
    QTableWidgetItem* item = ui->tableWidget->currentItem();
    if(!item) return;

	if (widget_previous) {
		int row = item->row();
		QTableWidget* tb = ui->tableWidget;
				
		id = tb->item(row, 0)->text();
		id_tipo = tb->item(row, 1)->text();
		id_marca = tb->item(row, 2)->text();
		id_unidad = tb->item(row, 3)->text();
		codigo = tb->item(row, 4)->text();
		tipo = tb->item(row, 5)->text();
		descripcion = tb->item(row, 6)->text();
		marca = tb->item(row, 7)->text();
		unidad = tb->item(row, 8)->text();
		precio = tb->item(row, 9)->text();
		cantidad = tb->item(row, 10)->text();

        setAttribute(Qt::WA_DeleteOnClose);
		SYSTEM->change_center_w(this, widget_previous);
	} else {
		SYSTEM->clear_center_w(this);
	}        
}
void ProductoBuscar::on_pushButton_salir_clicked()
{    
    if(widget_previous){
        id = "";
        setAttribute(Qt::WA_DeleteOnClose);
        SYSTEM->change_center_w(this, widget_previous);
    }else{
        SYSTEM->clear_center_w(this);
    }
}
void ProductoBuscar::on_lineEdit_codigo_textChanged(const QString& arg)
{
    connect(ui->lineEdit_codigo, SIGNAL(returnPressed()), this, SLOT(on_lineEdit_codigo_returnPressed()));
	pos = 0;
	
	ui->tableWidget->setRowCount(0);
	ui->tableWidget->setColumnCount(0);
	ui->tableWidget->clear();
}
void ProductoBuscar::on_lineEdit_codigo_returnPressed()
{
    disconnect(ui->lineEdit_codigo, SIGNAL(returnPressed()), this, SLOT(on_lineEdit_codigo_returnPressed()));
	QString codigo = ui->lineEdit_codigo->text();
	QString str_query = "SELECT producto.id, tipo.id, marca.id, unidad.id, codigo, tipo.tipo, descripcion, marca.marca, unidad.unidad, precio, cantidad";
			str_query += " FROM producto";
			str_query += " LEFT JOIN tipo AS tipo ON tipo.id = tipo_id";
			str_query += " LEFT JOIN marca AS marca ON marca.id = marca_id";
			str_query += " LEFT JOIN unidad AS unidad ON unidad.id = unidad_id";
            str_query += " WHERE codigo = '" + codigo + "'";
            str_query += " ORDER BY producto.descripcion";
	//str_query += " LIMIT " + QString().setNum(pos) + ", " + QString().setNum(size_query) + "";

	QSqlQuery query;
	qDebug() << str_query << endl;
	if (query.exec(str_query)) {
		int rowCount = ui->tableWidget->rowCount();
		ui->tableWidget->setRowCount(rowCount + query.size());

		int columnCount = query.record().count();
		ui->tableWidget->setColumnCount(columnCount);

		ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "ID" << "tipo_id" << "marca_id" << "unidad_id" 
			<< "Codigo" << "Tipo" << "Descripción" << "Marca" << "Unidad" << "Precio" << "Cantidad");
		ui->tableWidget->setColumnHidden(0, true);
		ui->tableWidget->setColumnHidden(1, true);
		ui->tableWidget->setColumnHidden(2, true);
		ui->tableWidget->setColumnHidden(3, true);

		while (query.next()) {
			QString id = query.value(0).toString();
			QString tipo_id = query.value(1).toString();
			QString marca_id = query.value(2).toString();
			QString unidad_id = query.value(3).toString();
			QString codigo = query.value(4).toString();
			QString tipo = query.value(5).toString();
			QString descripcion = query.value(6).toString();
			QString marca = query.value(7).toString();
			QString unidad = query.value(8).toString();
			QString precio = query.value(9).toString();
			QString cantidad = query.value(10).toString();

			ui->tableWidget->setItem(pos, 0, new QTableWidgetItem(id));
			ui->tableWidget->setItem(pos, 1, new QTableWidgetItem(tipo_id));
			ui->tableWidget->setItem(pos, 2, new QTableWidgetItem(marca_id));
			ui->tableWidget->setItem(pos, 3, new QTableWidgetItem(unidad_id));
			ui->tableWidget->setItem(pos, 4, new QTableWidgetItem(codigo));
			ui->tableWidget->setItem(pos, 5, new QTableWidgetItem(tipo));
			ui->tableWidget->setItem(pos, 6, new QTableWidgetItem(descripcion));
			ui->tableWidget->setItem(pos, 7, new QTableWidgetItem(marca));
			ui->tableWidget->setItem(pos, 8, new QTableWidgetItem(unidad));
			ui->tableWidget->setItem(pos, 9, new QTableWidgetItem(precio));
			ui->tableWidget->setItem(pos, 10, new QTableWidgetItem(cantidad));

			SYSTEM->table_resize_to_contents(0, ui->tableWidget, size_query);			
			++pos;
		}
	}
	else {

	}
}
void ProductoBuscar::on_lineEdit_tipo_buscar_textChanged(const QString& arg)
{
    connect(ui->lineEdit_tipo_buscar, SIGNAL(returnPressed()), this, SLOT(on_lineEdit_tipo_buscar_returnPressed()));
	pos = 0;

	ui->tableWidget->setRowCount(0);
	ui->tableWidget->setColumnCount(0);
	ui->tableWidget->clear();
}
void ProductoBuscar::on_lineEdit_tipo_buscar_returnPressed()
{
    disconnect(ui->lineEdit_tipo_buscar, SIGNAL(returnPressed()), this, SLOT(on_lineEdit_tipo_buscar_returnPressed()));
	QString tipo = ui->lineEdit_tipo_buscar->text();
	QString descripcion = ui->lineEdit_descripcion_buscar->text();
	QString marca = ui->lineEdit_marca_buscar->text();
	QString unidad = ui->lineEdit_unidad_buscar->text();

	QString str_query = "SELECT producto.id, tipo.id, marca.id, unidad.id, codigo, tipo.tipo, descripcion, marca.marca, unidad.unidad, precio, cantidad";
	str_query += " FROM producto";
	str_query += " LEFT JOIN tipo AS tipo ON tipo.id = tipo_id";
	str_query += " LEFT JOIN marca AS marca ON marca.id = marca_id";
	str_query += " LEFT JOIN unidad AS unidad ON unidad.id = unidad_id";
	str_query += " WHERE (tipo LIKE '" + tipo + "%' OR tipo IS NULL)";
	str_query += " AND descripcion LIKE '" + descripcion + "%'";
	str_query += " AND (marca LIKE '" + marca + "%' OR marca IS NULL)";
	str_query += " AND (unidad LIKE '" + unidad + "%' OR unidad IS NULL)";
	str_query += " ORDER BY tipo, descripcion, marca, unidad";
	str_query += " LIMIT " + QString().setNum(pos) + ", " + QString().setNum(size_query) + "";

	QSqlQuery query;
	qDebug() << str_query << endl;
	if (query.exec(str_query)) {
		int rowCount = ui->tableWidget->rowCount();
		ui->tableWidget->setRowCount(rowCount + query.size());

		int columnCount = query.record().count();
		ui->tableWidget->setColumnCount(columnCount);

		ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "ID" << "tipo_id" << "marca_id" << "unidad_id"
			<< "Codigo" << "Tipo" << "Descripción" << "Marca" << "Unidad" << "Precio" << "Cantidad");
		ui->tableWidget->setColumnHidden(0, true);
		ui->tableWidget->setColumnHidden(1, true);
		ui->tableWidget->setColumnHidden(2, true);
		ui->tableWidget->setColumnHidden(3, true);

		while (query.next()) {
			QString id = query.value(0).toString();
			QString tipo_id = query.value(1).toString();
			QString marca_id = query.value(2).toString();
			QString unidad_id = query.value(3).toString();
			QString codigo = query.value(4).toString();
			QString tipo = query.value(5).toString();
			QString descripcion = query.value(6).toString();
			QString marca = query.value(7).toString();
			QString unidad = query.value(8).toString();
			QString precio = query.value(9).toString();
			QString cantidad = query.value(10).toString();

			ui->tableWidget->setItem(pos, 0, new QTableWidgetItem(id));
			ui->tableWidget->setItem(pos, 1, new QTableWidgetItem(tipo_id));
			ui->tableWidget->setItem(pos, 2, new QTableWidgetItem(marca_id));
			ui->tableWidget->setItem(pos, 3, new QTableWidgetItem(unidad_id));
			ui->tableWidget->setItem(pos, 4, new QTableWidgetItem(codigo));
			ui->tableWidget->setItem(pos, 5, new QTableWidgetItem(tipo));
			ui->tableWidget->setItem(pos, 6, new QTableWidgetItem(descripcion));
			ui->tableWidget->setItem(pos, 7, new QTableWidgetItem(marca));
			ui->tableWidget->setItem(pos, 8, new QTableWidgetItem(unidad));
			ui->tableWidget->setItem(pos, 9, new QTableWidgetItem(precio));
			ui->tableWidget->setItem(pos, 10, new QTableWidgetItem(cantidad));

			SYSTEM->table_resize_to_contents(0, ui->tableWidget, size_query);
			++pos;
		}
	}
	else {

	}
}
void ProductoBuscar::on_lineEdit_descripcion_buscar_textChanged(const QString& arg)
{
	on_lineEdit_tipo_buscar_textChanged(arg);
}
void ProductoBuscar::on_lineEdit_descripcion_buscar_returnPressed()
{
	on_lineEdit_tipo_buscar_returnPressed();
}
void ProductoBuscar::on_lineEdit_marca_buscar_textChanged(const QString& arg)
{
	on_lineEdit_tipo_buscar_textChanged(arg);
}
void ProductoBuscar::on_lineEdit_marca_buscar_returnPressed()
{
	on_lineEdit_tipo_buscar_returnPressed();
}
void ProductoBuscar::on_lineEdit_unidad_buscar_textChanged(const QString& arg)
{
	on_lineEdit_tipo_buscar_textChanged(arg);
}
void ProductoBuscar::on_lineEdit_unidad_buscar_returnPressed()
{
	on_lineEdit_tipo_buscar_returnPressed();
}

void ProductoBuscar::on_tableWidget_itemDoubleClicked(QTableWidgetItem *item)
{
    QTableWidget* tb = ui->tableWidget;

    int row = item->row();

    id = tb->item(row, 0)->text();
    id_tipo = tb->item(row, 1)->text();
    id_marca = tb->item(row, 2)->text();
    id_unidad = tb->item(row, 3)->text();
    codigo = tb->item(row, 4)->text();
    tipo = tb->item(row, 5)->text();
    descripcion = tb->item(row, 6)->text();
    marca = tb->item(row, 7)->text();
    unidad = tb->item(row, 8)->text();
    precio = tb->item(row, 9)->text();
    cantidad = tb->item(row, 10)->text();

    ProductoFormTransaction* w = new ProductoFormTransaction;
    w->set_data(id, id_tipo, id_marca, id_unidad, codigo, tipo, descripcion, marca, unidad, precio, cantidad);
    connect(w, SIGNAL(closing()), this, SLOT(on_productoFormTransaction_closing()));
    w->set_widget_previous(this);

    SYSTEM->change_center_w(this, w);
}

void ProductoBuscar::showEvent(QShowEvent *se)
{
    se->accept();

    ui->lineEdit_descripcion_buscar->setFocus(Qt::TabFocusReason);

    on_lineEdit_descripcion_buscar_textChanged(ui->lineEdit_descripcion_buscar->text());
    on_lineEdit_descripcion_buscar_returnPressed();
}
void ProductoBuscar::closeEvent(QCloseEvent *ce)
{
	ce->accept();

    if(focusWidget()){
        focusWidget()->clearFocus();
    }
    //ui->lineEdit_descripcion_buscar->setFocus(Qt::TabFocusReason);
	emit closing();
}
bool ProductoBuscar::eventFilter(QObject *obj, QEvent *e)
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
        }else{

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
                    on_lineEdit_descripcion_buscar_returnPressed();
                    return true;
				}
			}
            case Qt::Key_F3:{
                QTableWidgetItem* item = ui->tableWidget->currentItem();
                if(item) {
                    CompraChartCosto* w = new CompraChartCosto();
                    QString producto_id = ui->tableWidget->item(item->row(), 0)->text();
                    QString unidad = ui->tableWidget->item(item->row(), 8)->text();
                    QString descripcion = ui->tableWidget->item(item->row(), 6)->text();
                    w->set_producto(producto_id, unidad, descripcion);
                    w->set_widget_previous(this);
                    SYSTEM->change_center_w(this, w);
                    return true;
                }
            }break;
            case Qt::Key_F4:{
                QTableWidgetItem* item = ui->tableWidget->currentItem();
                if(item) {
                    VentaChartPrecio* w = new VentaChartPrecio();
                    QString producto_id = ui->tableWidget->item(item->row(), 0)->text();
                    QString unidad = ui->tableWidget->item(item->row(), 8)->text();
                    QString descripcion = ui->tableWidget->item(item->row(), 6)->text();
                    w->set_producto(producto_id, unidad, descripcion);
                    w->set_widget_previous(this);
                    SYSTEM->change_center_w(this, w);
                    return true;
                }
            }break;
			}

		}
		else {

		}
		return false;
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
            case Qt::Key_Tab:{
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
	w_temp = ui->lineEdit_codigo;
	if (obj == w_temp) {
		if (e->type() == QEvent::KeyPress) {
			QKeyEvent *KeyEvent = (QKeyEvent*)e;

			switch (KeyEvent->key())
			{
			case Qt::Key_Return: {
                //ui->tableWidget->setFocus(Qt::TabFocusReason);
			}break;
			}

		}
		else {

		}
		return false;
	}
	w_temp = ui->lineEdit_descripcion_buscar;
	if (obj == w_temp) {
		if (e->type() == QEvent::KeyPress) {
			QKeyEvent *KeyEvent = (QKeyEvent*)e;

			switch (KeyEvent->key())
			{
			case Qt::Key_Return: {
                //ui->tableWidget->setFocus(Qt::TabFocusReason);
			}break;
			}

		}
		else {

		}
		return false;
	}
	w_temp = ui->lineEdit_tipo_buscar;
	if (obj == w_temp) {
		if (e->type() == QEvent::KeyPress) {
			QKeyEvent *KeyEvent = (QKeyEvent*)e;

			switch (KeyEvent->key())
			{
			case Qt::Key_Return: {
                //ui->tableWidget->setFocus(Qt::TabFocusReason);
			}break;
			}

		}
		else {

		}
		return false;
	}
	w_temp = ui->lineEdit_marca_buscar;
	if (obj == w_temp) {
		if (e->type() == QEvent::KeyPress) {
			QKeyEvent *KeyEvent = (QKeyEvent*)e;

			switch (KeyEvent->key())
			{
			case Qt::Key_Return: {
                //ui->tableWidget->setFocus(Qt::TabFocusReason);
			}break;
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
                //ui->tableWidget->setFocus(Qt::TabFocusReason);
			}break;
			}

		}
		else {

		}
		return false;
	}    
    w_temp = ui->pushButton_nuevo;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:{
                ui->pushButton_nuevo->click();
                return true;
            }break;
            }

        }else{

        }
        return false;
    }

    return eventFilter(obj, e);
}
