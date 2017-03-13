#include "tipobuscar.h"
#include "ui_tipobuscar.h"

TipoBuscar::TipoBuscar(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::TipoBuscar)
{
	ui->setupUi(this);

    firstShow = false;
    afterShow = false;

    widget_previous = NULL;

	id = "";
	tipo = "";

	pos = 0;
	size_query = 10;

	//connect(ui->lineEdit_tipo_buscar, SIGNAL(textChanged(QString)), this, SLOT(on_lineEdit_tipo_buscar_textChanged(QString)));
	//connect(ui->lineEdit_tipo_buscar, SIGNAL(returnPressed()), this, SLOT(on_lineEdit_tipo_buscar_returnPressed()));

	//connect(ui->pushButton_opes, SIGNAL(clicked()), this, SLOT(on_pushButton_opes_clicked()));
	
    disconnect(ui->lineEdit_tipo_buscar, SIGNAL(returnPressed()), this, SLOT(on_lineEdit_tipo_buscar_returnPressed()));

    // INSTALL FILTERS
    this->installEventFilter(this);
    ui->lineEdit_tipo_buscar->installEventFilter(this);
    ui->tableWidget->installEventFilter(this);
    ui->pushButton_ok->installEventFilter(this);
    ui->pushButton_salir->installEventFilter(this);
    ui->pushButton_nuevo->installEventFilter(this);
}

TipoBuscar::~TipoBuscar()
{    
	qDebug() << "delete tipo buscar" << endl;
    delete ui;
}
QString TipoBuscar::getID()
{
	return id;
}
QString TipoBuscar::getTipo()
{
	return tipo;
}
void TipoBuscar::set_widget_previous(QWidget *widget_previous)
{
    this->widget_previous = widget_previous;
}
void TipoBuscar::on_tipo_closing()
{
	Tipo* widget_tipo = (Tipo*)QObject::sender();
	id = widget_tipo->getID();
	tipo = widget_tipo->getTipo();

    if(id.compare("") == 0)
        return;

    QTableWidgetItem* item = ui->tableWidget->currentItem();

    ui->tableWidget->item(item->row(), 0)->setText(id);
    ui->tableWidget->item(item->row(), 1)->setText(tipo);

    SYSTEM->table_resize_to_contents(0, ui->tableWidget);
}
void TipoBuscar::on_lineEdit_tipo_buscar_textChanged(const QString& arg)
{
    //connect(ui->lineEdit_tipo_buscar, SIGNAL(returnPressed()), this, SLOT(on_lineEdit_tipo_buscar_returnPressed()));
	pos = 0;

	ui->tableWidget->setRowCount(0);
	ui->tableWidget->setColumnCount(0);
	ui->tableWidget->clear();

    on_lineEdit_tipo_buscar_returnPressed();
}
void TipoBuscar::on_lineEdit_tipo_buscar_returnPressed()
{	
    //disconnect(ui->lineEdit_tipo_buscar, SIGNAL(returnPressed()), this, SLOT(on_lineEdit_tipo_buscar_returnPressed()));
	QString tipo = ui->lineEdit_tipo_buscar->text();
    QString str_query = "SELECT id, tipo FROM tipo WHERE tipo LIKE '%" + tipo + "%'";
			str_query += " LIMIT "+QString().setNum(pos)+", "+QString().setNum(size_query)+"";
	
	QSqlQuery query;
	qDebug() << str_query << endl;
	if (query.exec(str_query)) {		
		int rowCount = ui->tableWidget->rowCount();
		ui->tableWidget->setRowCount(rowCount + query.size());

		int columnCount = query.record().count();
		ui->tableWidget->setColumnCount(columnCount);

		ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"ID" <<"Tipo");
		ui->tableWidget->setColumnHidden(0, true);

		while (query.next()) {
			QString id = query.value(0).toString();
			QString tipo = query.value(1).toString();
			ui->tableWidget->setItem(pos, 0, new QTableWidgetItem(id));
			ui->tableWidget->setItem(pos, 1, new QTableWidgetItem(tipo));

            for(int j=0; j<ui->tableWidget->columnCount(); j++)
                ui->tableWidget->item(pos, j)->setFlags(Qt::ItemIsEnabled
                                                             | Qt::ItemIsSelectable);
			++pos;
		}
        SYSTEM->table_resize_to_contents(0, ui->tableWidget, size_query);
	} else {

	}
}
void TipoBuscar::on_pushButton_nuevo_clicked()
{
    int ret = QMessageBox::warning(this, "Advertencia", "¿Desea agregar un TIPO?", "Si", "No");
    switch(ret){
    case 0:{
        Tipo* w = new Tipo;
        w->set_widget_previous(this);
		connect(w, SIGNAL(closing()), this, SLOT(on_tipo_closing()));
        SYSTEM->change_center_w(this, w);
    }break;
    case 1:{

    }break;
    }
}
void TipoBuscar::on_pushButton_ok_clicked()
{    
    if(widget_previous){
		QTableWidget* tb = ui->tableWidget;
		QTableWidgetItem* item = tb->currentItem();

		if (!item)return;

        int row = item->row();

        id = tb->item(row, 0)->text();
        tipo = tb->item(row, 1)->text();

        setAttribute(Qt::WA_DeleteOnClose);
        SYSTEM->change_center_w(this, widget_previous);
    }else{
        QTableWidgetItem* item = ui->tableWidget->currentItem();
        if(item)
            on_tableWidget_itemDoubleClicked(item);
    }
}

void TipoBuscar::on_pushButton_salir_clicked()
{
    int ret = QMessageBox::warning(this, "Advertencia", "¿Esta seguro que desea salir?", "Si", "No");

    switch(ret){
    case 0:{		
        if(widget_previous){
            id = "";
            setAttribute(Qt::WA_DeleteOnClose);
            SYSTEM->change_center_w(this, widget_previous);
        }else{
            SYSTEM->clear_center_w(this);
        }
    }
    case 1:{

    }
    }
}
void TipoBuscar::on_tableWidget_itemDoubleClicked(QTableWidgetItem *item)
{
    QTableWidget* table = ui->tableWidget;
    if (!item) {
        int ret = QMessageBox::warning(this, "Advertencia", "Seleccione un tipo.", "Ok");
        return;
    }

    Tipo* w = new Tipo;
    w->set_widget_previous(this);


    QString id = table->item(item->row(), 0)->text();
    QString tipo = table->item(item->row(), 1)->text();

    w->set_data(id, tipo);

    connect(w, SIGNAL(closing()), this, SLOT(on_tipo_closing()));
    SYSTEM->change_center_w(this, w);
}
void TipoBuscar::showEvent(QShowEvent *se)
{
    se->accept();

    afterShow = true;

    if(!firstShow){
        on_lineEdit_tipo_buscar_textChanged(ui->lineEdit_tipo_buscar->text());
        //on_lineEdit_tipo_buscar_returnPressed();
        firstShow = true;
    }
}
void TipoBuscar::closeEvent(QCloseEvent * ce)
{
	ce->accept();
	emit closing();
}
bool TipoBuscar::eventFilter(QObject *obj, QEvent *e)
{
    QWidget* w_temp;
    w_temp = this;
    if(obj == w_temp){
        if(e->type() == QEvent::MouseButtonPress){
            if(focusWidget()){
                focusWidget()->setFocus();
            }else{
                ui->lineEdit_tipo_buscar->setFocus();
                ui->lineEdit_tipo_buscar->setCursorPosition(ui->lineEdit_tipo_buscar->text().length());
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
                        ui->lineEdit_tipo_buscar->setFocus();
                        ui->lineEdit_tipo_buscar->setCursorPosition(ui->lineEdit_tipo_buscar->text().length());
                    }else{
                        focusWidget()->setFocus();
                    }
                }else{
                    ui->lineEdit_tipo_buscar->setFocus();
                    ui->lineEdit_tipo_buscar->setCursorPosition(ui->lineEdit_tipo_buscar->text().length());
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
    w_temp = ui->lineEdit_tipo_buscar;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:{
                //ui->tableWidget->setFocus();
                //ui->tableWidget->selectRow(0);
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
    w_temp = ui->tableWidget;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
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
					on_lineEdit_tipo_buscar_returnPressed();
				}
			}
            }

        }else{

        }
        return false;
	} else {
		
	}
    w_temp = ui->pushButton_ok;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
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
    return eventFilter(obj, e);
}
