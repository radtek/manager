#include "productomarcabuscar.h"
#include "ui_productomarcabuscar.h"

ProductoMarcaBuscar::ProductoMarcaBuscar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProductoMarcaBuscar)
{
    ui->setupUi(this);

    firstShow = false;
    afterShow = false;

    widget_previous = NULL;

    id = "";
    marca = "";

    pos = 0;
    size_query = 10;

    //connect(ui->lineEdit_tipo_buscar, SIGNAL(textChanged(QString)), this, SLOT(on_lineEdit_tipo_buscar_textChanged(QString)));
    //connect(ui->lineEdit_tipo_buscar, SIGNAL(returnPressed()), this, SLOT(on_lineEdit_tipo_buscar_returnPressed()));

    //connect(ui->pushButton_opes, SIGNAL(clicked()), this, SLOT(on_pushButton_opes_clicked()));

    //disconnect(ui->lineEdit_marca_buscar, SIGNAL(returnPressed()), this, SLOT(on_lineEdit_marca_buscar_returnPressed()));

    QScrollBar* bar = ui->tableWidget->verticalScrollBar();
    connect(bar, SIGNAL(actionTriggered(int)), this, SLOT(on_verticalScrollBar_actionTriggered(int)));

    // INSTALL FILTERS
    this->installEventFilter(this);
    ui->lineEdit_marca_buscar->installEventFilter(this);
    ui->tableWidget->installEventFilter(this);
    ui->pushButton_ok->installEventFilter(this);
    ui->pushButton_salir->installEventFilter(this);
    ui->pushButton_nuevo->installEventFilter(this);
    ui->pushButton_editar->installEventFilter(this);
}

ProductoMarcaBuscar::~ProductoMarcaBuscar()
{
    qDebug() << "delete producto marca buscar" << endl;
    delete ui;
}
QString ProductoMarcaBuscar::getID()
{
    return id;
}
QString ProductoMarcaBuscar::getMarca()
{
    return marca;
}
void ProductoMarcaBuscar::set_widget_previous(QWidget *widget_previous)
{
    this->widget_previous = widget_previous;
}
void ProductoMarcaBuscar::on_verticalScrollBar_actionTriggered(int value)
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
void ProductoMarcaBuscar::on_marca_closing()
{
    ProductoMarca* widget_marca= (ProductoMarca*)QObject::sender();

    int op = widget_marca->getOp();
    switch(op){
    case INGRESAR:{
        if (widget_previous) {
            int ret = QMessageBox::information(this, "Consulta", "Tiene un item disponible para ingresar.", "Si", "No");
            switch(ret){
            case 0:{
                id = widget_marca->getID();
                marca = widget_marca->getMarca();

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
        QString id = widget_marca->getID();
        QString marca = widget_marca->getMarca();

        QTableWidgetItem* item = ui->tableWidget->currentItem();
        int row = item->row();
        ui->tableWidget->item(row, 0)->setText(id);
        ui->tableWidget->item(row, 1)->setText(marca);

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
void ProductoMarcaBuscar::set_buscar()
{
    QString marca = ui->lineEdit_marca_buscar->text();
    marca = marca.trimmed();

    QString str_query = "SELECT id, marca FROM marca WHERE marca LIKE '%" + marca + "%'";
    str_query += " LIMIT " + QString().setNum(pos) + ", " + QString().setNum(size_query) + "";

    QSqlQuery query;
    qDebug() << str_query << endl;
    if (query.exec(str_query)) {
        int rowCount = ui->tableWidget->rowCount();
        ui->tableWidget->setRowCount(rowCount + query.size());

        int columnCount = query.record().count();
        ui->tableWidget->setColumnCount(columnCount);

        ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "ID" << "Marca");
        ui->tableWidget->setColumnHidden(0, true);

        while (query.next()) {
            QString id = query.value(0).toString();
            QString marca = query.value(1).toString();
            ui->tableWidget->setItem(pos, 0, new QTableWidgetItem(id));
            ui->tableWidget->setItem(pos, 1, new QTableWidgetItem(marca));

            for(int j=0; j<ui->tableWidget->columnCount(); j++)
                ui->tableWidget->item(pos, j)->setFlags(Qt::ItemIsEnabled
                                                             | Qt::ItemIsSelectable);
            ++pos;
        }
        SYSTEM->table_resize_to_contents(0, ui->tableWidget, size_query);
    }else{

    }
}
void ProductoMarcaBuscar::on_lineEdit_marca_buscar_textChanged(const QString& arg)
{
    pos = 0;

    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(0);
    ui->tableWidget->clear();

    set_buscar();
}
void ProductoMarcaBuscar::on_lineEdit_marca_buscar_returnPressed()
{
    qDebug()<<"return pressed"<<endl;
    pos = 0;

    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(0);
    ui->tableWidget->clear();

    set_buscar();
}
void ProductoMarcaBuscar::on_pushButton_nuevo_clicked()
{
    //int ret = QMessageBox::warning(this, "Advertencia", "¿Desea agregar una MARCA?", "Si", "No");
    //switch (ret) {
    //case 0: {
        ProductoMarca* w = new ProductoMarca;
        w->set_widget_previous(this);
        connect(w, SIGNAL(closing()), this, SLOT(on_marca_closing()));
        SYSTEM->change_center_w(this, w);
    //}break;
    //case 1: {

    //}break;
    //}
}
void ProductoMarcaBuscar::on_pushButton_editar_clicked()
{
    QTableWidgetItem* item = ui->tableWidget->currentItem();
    editarItem(item);
}
void ProductoMarcaBuscar::on_tableWidget_itemDoubleClicked(QTableWidgetItem *item)
{
    if(widget_previous){
        on_pushButton_ok_clicked();
    }else{
        editarItem(item);
    }
}
void ProductoMarcaBuscar::on_pushButton_ok_clicked()
{
    QTableWidget* tb = ui->tableWidget;
    QTableWidgetItem* item = tb->currentItem();

    if (!item)return;

    if (widget_previous) {
        int row = item->row();

        id = tb->item(row, 0)->text();
        marca = tb->item(row, 1)->text();

        setAttribute(Qt::WA_DeleteOnClose);
        SYSTEM->change_center_w(this, widget_previous);
    }
    else {
        editarItem(item);
    }
}

void ProductoMarcaBuscar::on_pushButton_salir_clicked()
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
void ProductoMarcaBuscar::showEvent(QShowEvent *se)
{
    se->accept();

    afterShow = true;

    if(!firstShow){
        on_lineEdit_marca_buscar_textChanged(ui->lineEdit_marca_buscar->text());
        //on_lineEdit_marca_buscar_returnPressed();
        firstShow = true;
    }
}
void ProductoMarcaBuscar::closeEvent(QCloseEvent * ce)
{
    ce->accept();
    emit closing();
}
bool ProductoMarcaBuscar::eventFilter(QObject *obj, QEvent *e)
{
    QWidget* w_temp;
    w_temp = this;
    if (obj == w_temp) {
        if(e->type() == QEvent::MouseButtonPress){
            if(focusWidget()){
                focusWidget()->setFocus();
            }else{
                ui->lineEdit_marca_buscar->setFocus();
                ui->lineEdit_marca_buscar->setCursorPosition(ui->lineEdit_marca_buscar->text().length());
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
                        ui->lineEdit_marca_buscar->setFocus();
                        ui->lineEdit_marca_buscar->setCursorPosition(ui->lineEdit_marca_buscar->text().length());
                    }else{
                        focusWidget()->setFocus();
                    }
                }else{
                    ui->lineEdit_marca_buscar->setFocus();
                    ui->lineEdit_marca_buscar->setCursorPosition(ui->lineEdit_marca_buscar->text().length());
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
                on_lineEdit_marca_buscar_returnPressed();
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
                    set_buscar();
                    return true;
                }
            }break;
            }
        } else {

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

        }
        else {

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

        }
        else {

        }
        return false;
    }
    return eventFilter(obj, e);
}
void ProductoMarcaBuscar::editarItem(QTableWidgetItem *item)
{
    if(!item){
        QMessageBox::warning(this, "Advertencia", "Selecciona un item de la tabla.", "Ok");
        return;
    }
    QTableWidget* table = ui->tableWidget;

    ProductoMarca* w = new ProductoMarca;
    w->set_widget_previous(this);

    QString id = table->item(item->row(), 0)->text();
    QString marca = table->item(item->row(), 1)->text();

    w->set_data(id, marca);

    connect(w, SIGNAL(closing()), this, SLOT(on_marca_closing()));
    SYSTEM->change_center_w(this, w);
}
