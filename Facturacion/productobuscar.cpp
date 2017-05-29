#include "productobuscar.h"
#include "ui_productobuscar.h"

#include "comprachartcosto.h"
#include "ventachartprecio.h"

ProductoBuscar::ProductoBuscar(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::ProductoBuscar)
{
	ui->setupUi(this);

    firstShow = false;
    afterShow = false;

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
/*
    disconnect(ui->lineEdit_codigo, SIGNAL(returnPressed())
                , this, SLOT(on_lineEdit_codigo_returnPressed()));
    disconnect(ui->lineEdit_descripcion_buscar, SIGNAL(returnPressed())
                , this, SLOT(on_lineEdit_descripcion_buscar_returnPressed()));
    disconnect(ui->lineEdit_marca_buscar, SIGNAL(returnPressed())
                , this, SLOT(on_lineEdit_marca_buscar_returnPressed()));
    disconnect(ui->lineEdit_tipo_buscar, SIGNAL(returnPressed())
                , this, SLOT(on_lineEdit_tipo_buscar_returnPressed()));
    disconnect(ui->lineEdit_unidad_buscar, SIGNAL(returnPressed())
                , this, SLOT(on_lineEdit_unidad_buscar_returnPressed()));
*/
    QScrollBar* bar = ui->tableWidget->verticalScrollBar();
    connect(bar, SIGNAL(valueChanged(int)), this, SLOT(on_verticalScrollBar_valueChanged(int)));
    connect(bar, SIGNAL(actionTriggered(int)), this, SLOT(on_verticalScrollBar_actionTriggered(int)));

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
    ui->pushButton_editar->installEventFilter(this);
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

void ProductoBuscar::set_widget_previous(QWidget *widget_previous)
{
    this->widget_previous = widget_previous;
}
void ProductoBuscar::on_verticalScrollBar_actionTriggered(int value)
{
    QScrollBar* bar = ui->tableWidget->verticalScrollBar();

    /*
    qDebug()<<"activation value: "<<value<<endl;
    qDebug()<<"activation bar maximum: "<<bar->maximum()<<endl;
    qDebug()<<"activation bar value: "<<bar->value()<<endl;
    */
    if(bar->value() == bar->maximum()) {
        set_buscar();
    }
}
void ProductoBuscar::on_verticalScrollBar_valueChanged(int value)
{
    /*
    QScrollBar* bar = ui->tableWidget->verticalScrollBar();

    if(value == bar->maximum())
        set_buscar();
*/
}
void ProductoBuscar::on_productoFormTransaction_closing()
{
	ProductoFormTransaction* widget_prod = (ProductoFormTransaction*)QObject::sender();

    int op = widget_prod->getOp();
    switch(op){
    case INGRESAR:{        
        if (widget_previous) {
            int ret = QMessageBox::information(this, "Consulta", "Tiene un item disponible para ingresar.", "Si", "No");
            switch(ret){
            case 0:{
                id = widget_prod->get_ID();
                id_tipo = widget_prod->get_IDTipo();
                id_marca = widget_prod->get_IDMarca();
                id_unidad = widget_prod->get_IDUnidad();
                codigo = widget_prod->get_codigo();
                tipo = widget_prod->get_tipo();
                descripcion = widget_prod->get_descripcion();
                marca = widget_prod->get_marca();
                unidad = widget_prod->get_unidad();

                setAttribute(Qt::WA_DeleteOnClose);
                SYSTEM->change_center_w(this, widget_previous);
            }break;
            case 1:{
                pos = 0;

                ui->tableWidget->setRowCount(0);
                ui->tableWidget->setColumnCount(0);
                ui->tableWidget->clear();

                set_buscar();
            }break;
            }
        }else{
            pos = 0;

            ui->tableWidget->setRowCount(0);
            ui->tableWidget->setColumnCount(0);
            ui->tableWidget->clear();

            set_buscar();
        }
    }break;
    case MODIFICAR:{
        QString id = widget_prod->get_ID();
        QString tipo_id = widget_prod->get_IDTipo();
        QString marca_id = widget_prod->get_IDMarca();
        QString unidad_id = widget_prod->get_IDUnidad();
        QString codigo = widget_prod->get_codigo();
        QString tipo = widget_prod->get_tipo();
        QString descripcion = widget_prod->get_descripcion();
        QString marca = widget_prod->get_marca();
        QString unidad = widget_prod->get_unidad();

        QTableWidgetItem* item = ui->tableWidget->currentItem();
        int row = item->row();
        ui->tableWidget->item(row, 0)->setText(id);
        ui->tableWidget->item(row, 1)->setText(tipo_id);
        ui->tableWidget->item(row, 2)->setText(marca_id);
        ui->tableWidget->item(row, 3)->setText(unidad_id);
        ui->tableWidget->item(row, 4)->setText(codigo);
        ui->tableWidget->item(row, 5)->setText(tipo);
        ui->tableWidget->item(row, 6)->setText(descripcion);
        ui->tableWidget->item(row, 7)->setText(marca);
        ui->tableWidget->item(row, 8)->setText(unidad);

        SYSTEM->table_resize_to_contents(0, ui->tableWidget, size_query);
    }break;
    case ELIMINAR:{
        pos = 0;

        ui->tableWidget->setRowCount(0);
        ui->tableWidget->setColumnCount(0);
        ui->tableWidget->clear();

        set_buscar();
    }break;
    case SALIR:{

    }break;
    }
}
void ProductoBuscar::on_pushButton_nuevo_clicked()
{
    //int ret = QMessageBox::warning(this, "Advertencia", "¿Desea ingresar un nuevo PRODUCTO?", "Si", "No");
    //switch(ret){
    //case 0:{
        ProductoFormTransaction* w = new ProductoFormTransaction;
		connect(w, SIGNAL(closing()), this, SLOT(on_productoFormTransaction_closing()));
        w->set_widget_previous(this);

        SYSTEM->change_center_w(this, w);        
    //}break;
    //case 1:{

    //}break;
    //}
}
void ProductoBuscar::on_pushButton_editar_clicked()
{
    QTableWidgetItem* item = ui->tableWidget->currentItem();
    editarItem(item);
}
void ProductoBuscar::on_tableWidget_itemDoubleClicked(QTableWidgetItem *item)
{
    if(widget_previous){
        on_pushButton_ok_clicked();
    }else{
        editarItem(item);
    }

}
void ProductoBuscar::on_pushButton_ok_clicked()
{
    QTableWidgetItem* item = ui->tableWidget->currentItem();
    if(!item){
        QMessageBox::warning(this, "Advertencia", "Selecciona un item de la tabla.", "Ok");
        return;
    }
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

        setAttribute(Qt::WA_DeleteOnClose);
		SYSTEM->change_center_w(this, widget_previous);
	} else {
        editarItem(item);
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
void ProductoBuscar::set_buscar_codigo()
{
    QString codigo = ui->lineEdit_codigo->text();
    codigo = codigo.trimmed();
    
    QString str_query = "SELECT producto.id, tipo.id, marca.id, unidad.id, codigo, tipo.tipo, descripcion, marca.marca, unidad.unidad";
            str_query += " FROM producto";
            str_query += " LEFT JOIN tipo AS tipo ON tipo.id = tipo_id";
            str_query += " LEFT JOIN marca AS marca ON marca.id = marca_id";
            str_query += " LEFT JOIN unidad AS unidad ON unidad.id = unidad_id";
            str_query += " WHERE codigo LIKE '" + codigo + "%'";
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
            << "Codigo" << "Tipo" << "Descripción" << "Marca" << "Unidad");
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

            ui->tableWidget->setItem(pos, 0, new QTableWidgetItem(id));
            ui->tableWidget->setItem(pos, 1, new QTableWidgetItem(tipo_id));
            ui->tableWidget->setItem(pos, 2, new QTableWidgetItem(marca_id));
            ui->tableWidget->setItem(pos, 3, new QTableWidgetItem(unidad_id));
            ui->tableWidget->setItem(pos, 4, new QTableWidgetItem(codigo));
            ui->tableWidget->setItem(pos, 5, new QTableWidgetItem(tipo));
            ui->tableWidget->setItem(pos, 6, new QTableWidgetItem(descripcion));
            ui->tableWidget->setItem(pos, 7, new QTableWidgetItem(marca));
            ui->tableWidget->setItem(pos, 8, new QTableWidgetItem(unidad));

            for(int j=0; j<ui->tableWidget->columnCount(); j++)
                ui->tableWidget->item(pos, j)->setFlags(Qt::ItemIsEnabled
                                                             | Qt::ItemIsSelectable);
            ++pos;
        }
        SYSTEM->table_resize_to_contents(0, ui->tableWidget, size_query);
    }else{

    }
}
void ProductoBuscar::set_buscar()
{
    QString tipo = ui->lineEdit_tipo_buscar->text();
    tipo = tipo.trimmed();
    QString descripcion = ui->lineEdit_descripcion_buscar->text();
    descripcion = descripcion.trimmed();
    QString marca = ui->lineEdit_marca_buscar->text();
    marca = marca.trimmed();
    QString unidad = ui->lineEdit_unidad_buscar->text();
    unidad = unidad.trimmed();

    QString str_query = "SELECT producto.id, tipo.id, marca.id, unidad.id, codigo, tipo.tipo, descripcion, marca.marca, unidad.unidad";
    str_query += " FROM producto";
    str_query += " LEFT JOIN tipo AS tipo ON tipo.id = tipo_id";
    str_query += " LEFT JOIN marca AS marca ON marca.id = marca_id";
    str_query += " LEFT JOIN unidad AS unidad ON unidad.id = unidad_id";
    str_query += " WHERE (tipo LIKE '%" + tipo + "%' OR tipo IS NULL)";
    str_query += " AND descripcion LIKE '%" + descripcion + "%'";
    str_query += " AND (marca LIKE '%" + marca + "%' OR marca IS NULL)";
    str_query += " AND (unidad LIKE '%" + unidad + "%' OR unidad IS NULL)";
    str_query += " ORDER BY descripcion, marca, unidad, tipo";
    str_query += " LIMIT " + QString().setNum(pos) + ", " + QString().setNum(size_query) + "";

    QSqlQuery query;
    qDebug() << str_query << endl;
    if (query.exec(str_query)) {
        int rowCount = ui->tableWidget->rowCount();
        ui->tableWidget->setRowCount(rowCount + query.size());

        int columnCount = query.record().count();
        ui->tableWidget->setColumnCount(columnCount);

        ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "ID" << "tipo_id" << "marca_id" << "unidad_id"
            << "Codigo" << "Tipo" << "Descripción" << "Marca" << "Unidad");
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

            ui->tableWidget->setItem(pos, 0, new QTableWidgetItem(id));
            ui->tableWidget->setItem(pos, 1, new QTableWidgetItem(tipo_id));
            ui->tableWidget->setItem(pos, 2, new QTableWidgetItem(marca_id));
            ui->tableWidget->setItem(pos, 3, new QTableWidgetItem(unidad_id));
            ui->tableWidget->setItem(pos, 4, new QTableWidgetItem(codigo));
            ui->tableWidget->setItem(pos, 5, new QTableWidgetItem(tipo));
            ui->tableWidget->setItem(pos, 6, new QTableWidgetItem(descripcion));
            ui->tableWidget->setItem(pos, 7, new QTableWidgetItem(marca));
            ui->tableWidget->setItem(pos, 8, new QTableWidgetItem(unidad));

            for(int j=0; j<ui->tableWidget->columnCount(); j++)
                ui->tableWidget->item(pos, j)->setFlags(Qt::ItemIsEnabled
                                                             | Qt::ItemIsSelectable);
            ++pos;
        }
        SYSTEM->table_resize_to_contents(0, ui->tableWidget, size_query);
    }else{

    }
}
void ProductoBuscar::on_lineEdit_codigo_textChanged(const QString& arg)
{
	pos = 0;
	
	ui->tableWidget->setRowCount(0);
	ui->tableWidget->setColumnCount(0);
	ui->tableWidget->clear();

    set_buscar_codigo();
}
void ProductoBuscar::on_lineEdit_codigo_returnPressed()
{
    qDebug()<<"return pressed"<<endl;
    pos = 0;

    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(0);
    ui->tableWidget->clear();

    set_buscar_codigo();
}
void ProductoBuscar::on_lineEdit_tipo_buscar_textChanged(const QString& arg)
{    
	pos = 0;

	ui->tableWidget->setRowCount(0);
	ui->tableWidget->setColumnCount(0);
	ui->tableWidget->clear();

    set_buscar();
}
void ProductoBuscar::on_lineEdit_tipo_buscar_returnPressed()
{    
    qDebug()<<"return pressed"<<endl;
    pos = 0;

    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(0);
    ui->tableWidget->clear();

    set_buscar();
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
void ProductoBuscar::showEvent(QShowEvent *se)
{
    se->accept();

    afterShow = true;

    if(!firstShow){
        on_lineEdit_descripcion_buscar_textChanged(ui->lineEdit_descripcion_buscar->text());
        //on_lineEdit_descripcion_buscar_returnPressed();
        firstShow = true;
    }
}
void ProductoBuscar::closeEvent(QCloseEvent *ce)
{
	ce->accept();    
	emit closing();
}
bool ProductoBuscar::eventFilter(QObject *obj, QEvent *e)
{
    QWidget* w_temp;
    w_temp = this;
    if(obj == w_temp){
        if(e->type() == QEvent::MouseButtonPress){
            if(focusWidget()){
                focusWidget()->setFocus();
            }else{
                ui->lineEdit_descripcion_buscar->setFocus();
                ui->lineEdit_descripcion_buscar->setCursorPosition(ui->lineEdit_descripcion_buscar->text().length());
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
                        ui->lineEdit_descripcion_buscar->setFocus();
                        ui->lineEdit_descripcion_buscar->setCursorPosition(ui->lineEdit_descripcion_buscar->text().length());
                    }else{                        
                        focusWidget()->setFocus();
                    }
                }else{
                    ui->lineEdit_descripcion_buscar->setFocus();
                    ui->lineEdit_descripcion_buscar->setCursorPosition(ui->lineEdit_descripcion_buscar->text().length());                    
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
			case Qt::Key_Up: {
				if (this->focusWidget() != ui->tableWidget) {
					ui->tableWidget->setFocus(Qt::TabFocusReason);
					if (ui->tableWidget->currentItem())
						ui->tableWidget->currentItem()->setSelected(true);
                    return true;
                }
            }break;
			case Qt::Key_Down: {
				if (this->focusWidget() != ui->tableWidget) {
					ui->tableWidget->setFocus(Qt::TabFocusReason);
					if (ui->tableWidget->currentItem())
						ui->tableWidget->currentItem()->setSelected(true);
                    return true;
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
            case Qt::Key_Enter:{
                QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QApplication::sendEvent(w_temp, key);
                return true;
            }break;
			case Qt::Key_Down: {
				int index = ui->tableWidget->currentRow();
				if (index == ui->tableWidget->rowCount() - 1) {
                    set_buscar();
                    return true;
				}
            }break;

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

        } else {

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
            case Qt::Key_Tab:{
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
	w_temp = ui->lineEdit_codigo;
	if (obj == w_temp) {
		if (e->type() == QEvent::KeyPress) {
			QKeyEvent *KeyEvent = (QKeyEvent*)e;

			switch (KeyEvent->key())
			{
			case Qt::Key_Return: {
                on_lineEdit_codigo_returnPressed();
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
	w_temp = ui->lineEdit_descripcion_buscar;
	if (obj == w_temp) {
		if (e->type() == QEvent::KeyPress) {
			QKeyEvent *KeyEvent = (QKeyEvent*)e;

			switch (KeyEvent->key())
			{
			case Qt::Key_Return: {
                on_lineEdit_tipo_buscar_returnPressed();
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
	w_temp = ui->lineEdit_tipo_buscar;
	if (obj == w_temp) {
		if (e->type() == QEvent::KeyPress) {
			QKeyEvent *KeyEvent = (QKeyEvent*)e;

			switch (KeyEvent->key())
			{
			case Qt::Key_Return: {
                on_lineEdit_tipo_buscar_returnPressed();
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
	w_temp = ui->lineEdit_marca_buscar;
	if (obj == w_temp) {
		if (e->type() == QEvent::KeyPress) {
			QKeyEvent *KeyEvent = (QKeyEvent*)e;

			switch (KeyEvent->key())
			{
			case Qt::Key_Return: {
                on_lineEdit_tipo_buscar_returnPressed();
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
	w_temp = ui->lineEdit_unidad_buscar;
	if (obj == w_temp) {
		if (e->type() == QEvent::KeyPress) {
			QKeyEvent *KeyEvent = (QKeyEvent*)e;

			switch (KeyEvent->key())
			{
			case Qt::Key_Return: {
                on_lineEdit_tipo_buscar_returnPressed();
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
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:{
                ui->pushButton_nuevo->click();
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
    w_temp = ui->pushButton_editar;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:{
                ui->pushButton_editar->click();
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
void ProductoBuscar::editarItem(QTableWidgetItem *item)
{
    if(!item){
        QMessageBox::warning(this, "Advertencia", "Selecciona un item de la tabla.", "Ok");
        return;
    }

    QTableWidget* tb = ui->tableWidget;

    int row = item->row();

    QString id = tb->item(row, 0)->text();
    QString id_tipo = tb->item(row, 1)->text();
    QString id_marca = tb->item(row, 2)->text();
    QString id_unidad = tb->item(row, 3)->text();
    QString codigo = tb->item(row, 4)->text();
    QString tipo = tb->item(row, 5)->text();
    QString descripcion = tb->item(row, 6)->text();
    QString marca = tb->item(row, 7)->text();
    QString unidad = tb->item(row, 8)->text();

    ProductoFormTransaction* w = new ProductoFormTransaction;
    w->set_data(id, id_tipo, id_marca, id_unidad, codigo, tipo, descripcion, marca, unidad);
    connect(w, SIGNAL(closing()), this, SLOT(on_productoFormTransaction_closing()));
    w->set_widget_previous(this);

    SYSTEM->change_center_w(this, w);
}
