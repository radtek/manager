#include "cliente.h"
#include "ui_cliente.h"

Cliente::Cliente(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Cliente)
{
    ui->setupUi(this);

    firstShow = false;
    afterShow = false;

    pos = 0;
    size_query = 10;

    cod = "";
    nombre = "";
    direccion = "";
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

    ui->tableWidget->installEventFilter(this);
    ui->lineEdit_buscar->installEventFilter(this);
    ui->pushButton_ingresar->installEventFilter(this);
    ui->pushButton_salir->installEventFilter(this);
}

Cliente::~Cliente()
{
    qDebug()<<"delete cliente"<<endl;
    delete ui;
}

void Cliente::on_verticalScrollBar_actionTriggered(int value)
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
void Cliente::on_verticalScrollBar_valueChanged(int value)
{
    /*
    QScrollBar* bar = ui->tableWidget->verticalScrollBar();

    if(value == bar->maximum())
        set_buscar();
*/
}

void Cliente::on_tableWidget_itemDoubleClicked(QTableWidgetItem *item)
{
    //if(widget_previous){
        on_pushButton_ingresar_clicked();
    //}else{
        //editarItem(item);
    //}

}

void Cliente::on_pushButton_salir_clicked()
{
    cod = "";
    this->close();
}
void Cliente::set_buscar()
{
    QString nombre = ui->lineEdit_buscar->text();
    nombre = nombre.trimmed();

    QString str_query = "SELECT cod, nombre, direccion";
            str_query += " FROM persona";
            str_query += " WHERE nombre LIKE '%" + nombre + "%'";
            str_query += " AND (tipo_item_nombre = 'Cliente_DNI' OR tipo_item_nombre = 'Cliente_RUC')";
            str_query += " ORDER BY nombre";
            str_query += " LIMIT " + QString().setNum(pos) + ", " + QString().setNum(size_query) + "";

    QSqlQuery query;
    qDebug() << str_query << endl;
    if (query.exec(str_query)) {
        int rowCount = ui->tableWidget->rowCount();
        ui->tableWidget->setRowCount(rowCount + query.size());

        int columnCount = query.record().count();
        ui->tableWidget->setColumnCount(columnCount);

        ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "Código" << "Nombre" << "Direccion");


        while (query.next()) {
            QString codigo = query.value(0).toString();
            QString nombre = query.value(1).toString();
            QString direccion = query.value(2).toString();

            ui->tableWidget->setItem(pos, 0, new QTableWidgetItem(codigo));
            ui->tableWidget->setItem(pos, 1, new QTableWidgetItem(nombre));
            ui->tableWidget->setItem(pos, 2, new QTableWidgetItem(direccion));

            for(int j=0; j<ui->tableWidget->columnCount(); j++)
                ui->tableWidget->item(pos, j)->setFlags(Qt::ItemIsEnabled
                                                             | Qt::ItemIsSelectable);
            ++pos;
        }
        SYSTEM->table_resize_to_contents(0, ui->tableWidget, size_query);
    }else{

    }
}
void Cliente::on_lineEdit_buscar_textChanged(const QString& arg)
{
    pos = 0;

    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(0);
    ui->tableWidget->clear();

    set_buscar();
}
void Cliente::on_lineEdit_buscar_returnPressed()
{
    qDebug()<<"return pressed"<<endl;
    pos = 0;

    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(0);
    ui->tableWidget->clear();

    set_buscar();
}
void Cliente::showEvent(QShowEvent *se)
{
    se->accept();

    afterShow = true;

    if(!firstShow){
        on_lineEdit_buscar_textChanged(ui->lineEdit_buscar->text());
        //on_lineEdit_descripcion_buscar_returnPressed();
        firstShow = true;
    }
}
void Cliente::closeEvent(QCloseEvent *ce)
{
    ce->accept();
    emit closing();
}
bool Cliente::eventFilter(QObject *obj, QEvent *e)
{
    QWidget* w_temp;
    w_temp = this;
    if(obj == w_temp){
        if(e->type() == QEvent::MouseButtonPress){
            if(focusWidget()){
                focusWidget()->setFocus();
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
                    ui->lineEdit_buscar->setFocus();
                    ui->lineEdit_buscar->setCursorPosition(ui->lineEdit_buscar->text().length());
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
                ui->pushButton_ingresar->click();
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
            }

        } else {

        }
        return false;
    }
    w_temp = ui->lineEdit_buscar;
    if (obj == w_temp) {
        if (e->type() == QEvent::KeyPress) {
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch (KeyEvent->key())
            {
            case Qt::Key_Return: {
                on_lineEdit_buscar_returnPressed();
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
    w_temp = ui->pushButton_ingresar;
    if (obj == w_temp) {
        if (e->type() == QEvent::KeyPress) {
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch (KeyEvent->key())
            {
            case Qt::Key_Return:
                ui->pushButton_ingresar->click();
                return true;
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

        } else {

        }
        return false;
    }

    return eventFilter(obj, e);
}

void Cliente::on_pushButton_ingresar_clicked()
{
    QTableWidgetItem* item = ui->tableWidget->currentItem();
    if(!item){
        QMessageBox::warning(this, "Advertencia", "Selecciona un item de la tabla.", "Ok");
        return;
    }

    int row = item->row();
    QTableWidget* tb = ui->tableWidget;

    cod = tb->item(row, 0)->text();
    nombre = tb->item(row, 1)->text();
    direccion = tb->item(row, 2)->text();

    this->close();
}
