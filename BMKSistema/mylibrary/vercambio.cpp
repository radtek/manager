#include "vercambio.h"
#include "ui_vercambio.h"

VerCambio::VerCambio(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VerCambio)
{
    ui->setupUi(this);

    firstShow = false;
    afterShow = false;

    this->widget_previous = NULL;

    ui->dateEdit_inicio->setDate(QDate(1, 1, QDate::currentDate().year()));
    ui->dateEdit_fin->setDate(QDate::currentDate());

    pos = 0;
    size_query = 10;

    ui->radioButton_sin_ingreso->hide();
    ui->pushButton_editar->hide();

    this->installEventFilter(this);
    ui->comboBox_moneda->installEventFilter(this);
    ui->dateEdit_inicio->installEventFilter(this);
    ui->dateEdit_fin->installEventFilter(this);
    ui->radioButton_sin_ingreso->installEventFilter(this);
    ui->pushButton_editar->installEventFilter(this);
    ui->tableWidget->installEventFilter(this);
    ui->pushButton_salir->installEventFilter(this);
}

VerCambio::~VerCambio()
{
    delete ui;
}
void VerCambio::set_widget_previous(QWidget *widget_previous)
{
    this->widget_previous = widget_previous;
}
void VerCambio::on_pushButton_editar_clicked()
{

}
void VerCambio::on_pushButton_salir_clicked()
{
    if(widget_previous){
        this->setAttribute(Qt::WA_DeleteOnClose);
        SYSTEM->change_center_w(this, widget_previous);
    }else{
        SYSTEM->clear_center_w(this);
    }
}
void VerCambio::on_dateEdit_fin_editingFinished()
{
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(0);
    ui->tableWidget->clear();

    pos = 0;

    jalar_cambio();
}
void VerCambio::jalar_cambio()
{
    QString str_query = "SELECT ïd, moneda_id, num, fecha FROM tipo_cambio";
    str_query += " WHERE moneda_id = "+QString().setNum(moneda_items::DOLAR);
    str_query += " ORDER BY fecha DESC";
    str_query += " LIMIT "+QString().setNum(pos)+", "+QString().setNum(size_query);
    QSqlQuery query;
    qDebug()<<str_query<<endl;
    if(query.exec(str_query)){
        int rowCount = ui->tableWidget->rowCount();
        ui->tableWidget->setRowCount(rowCount + query.size());

        int columnCount = query.record().count();
        ui->tableWidget->setColumnCount(columnCount);

        ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "ID"
                                                   << "MONEDA_ID" << "CAMBIO"
                                                   << "FECHA");
        ui->tableWidget->setColumnHidden(0, true);
        ui->tableWidget->setColumnHidden(1, true);

        while(query.next()){
            QString id = query.value(0).toString();
            QString moneda_id = query.value(1).toString();
            QString num = query.value(2).toString();
            QString fecha = query.value(3).toString();
            ui->tableWidget->setItem(pos, 0, new QTableWidgetItem(id));
            ui->tableWidget->setItem(pos, 1, new QTableWidgetItem(moneda_id));
            ui->tableWidget->setItem(pos, 2, new QTableWidgetItem(num));
            ui->tableWidget->setItem(pos, 3, new QTableWidgetItem(fecha));

            for(int j=0; j<ui->tableWidget->columnCount(); j++)
                ui->tableWidget->item(pos, j)->setFlags(Qt::ItemIsEnabled
                                                             | Qt::ItemIsSelectable);

            pos++;
        }
        SYSTEM->table_resize_to_contents(0, ui->tableWidget, size_query);
    }else{

    }
}

void VerCambio::showEvent(QShowEvent *event)
{
    event->accept();

    afterShow = true;

    if(!firstShow){
        on_dateEdit_fin_editingFinished();
        //on_lineEdit_buscar_returnPressed();
        firstShow = true;
    }
}
bool VerCambio::eventFilter(QObject *watched, QEvent *event)
{
    QWidget* w_temp = this;
    w_temp = this;
    if(watched == w_temp){
        if(event->type() == QEvent::MouseButtonPress){
            if(focusWidget()){
                focusWidget()->setFocus();
            }else{
                ui->dateEdit_inicio->setFocus();
                //ui->dateEdit_inicio->setCursorPosition(ui->lineEdit_buscar->text().length());
            }
            return true;
        }
        if(event->type() == QEvent::MouseButtonDblClick){
            if(focusWidget()){
                focusWidget()->setFocus();
            }
            return true;
        }
        if(event->type() == QEvent::Paint){
            if(afterShow) {
                if(focusWidget()){
                    if(focusWidget() == ui->pushButton_editar){
                        ui->dateEdit_inicio->setFocus();
                        //ui->lineEdit_buscar->setCursorPosition(ui->lineEdit_buscar->text().length());
                    }else{
                        focusWidget()->setFocus();
                    }
                }else{
                    ui->dateEdit_inicio->setFocus();
                    //ui->lineEdit_buscar->setCursorPosition(ui->lineEdit_buscar->text().length());
                }
                afterShow = false;
            }
            return true;
        }
        if(event->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)event;

            switch(KeyEvent->key())
            {
            case Qt::Key_Escape:{
                ui->pushButton_salir->click();
                return true;
            }break;
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
    w_temp = ui->comboBox_moneda;
    if(watched == w_temp){
        if(event->type() == QEvent::KeyPress){
            QKeyEvent* ke = (QKeyEvent*)event;
            switch(ke->key()){
            case Qt::Key_Return:{
                ui->dateEdit_inicio->setFocus();
            }break;
            case Qt::Key_Enter:{
                QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QApplication::sendEvent(w_temp, key);
                return true;
            }break;
            }
        }
        return false;
    }
    w_temp = ui->dateEdit_inicio;
    if(watched == w_temp){
        if(event->type() == QEvent::KeyPress){
            QKeyEvent* ke = (QKeyEvent*)event;
            switch(ke->key()){
            case Qt::Key_Return:{
                ui->dateEdit_fin->setFocus();
            }break;
            case Qt::Key_Enter:{
                QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QApplication::sendEvent(w_temp, key);
                return true;
            }break;
            }
        }
        return false;
    }
    w_temp = ui->dateEdit_fin;
    if(watched == w_temp){
        if(event->type() == QEvent::KeyPress){
            QKeyEvent* ke = (QKeyEvent*)event;
            switch(ke->key()){
            case Qt::Key_Return:{
                ui->tableWidget->setFocus();
            }break;
            case Qt::Key_Enter:{
                QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QApplication::sendEvent(w_temp, key);
                return true;
            }break;
            }
        }
        return false;
    }
    w_temp = ui->radioButton_sin_ingreso;
    if(watched == w_temp){
        if(event->type() == QEvent::KeyPress){
            QKeyEvent* ke = (QKeyEvent*)event;
            switch(ke->key()){
            case Qt::Key_Return:{
                ui->pushButton_editar->setFocus(Qt::TabFocusReason);
            }break;
            case Qt::Key_Enter:{
                QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QApplication::sendEvent(w_temp, key);
                return true;
            }break;
            }
        }
        return false;
    }
    w_temp = ui->pushButton_editar;
    if(watched == w_temp){
        if(event->type() == QEvent::KeyPress){
            QKeyEvent* ke = (QKeyEvent*)event;
            switch(ke->key()){
            case Qt::Key_Return:{
                ui->tableWidget->setFocus();
            }break;
            case Qt::Key_Enter:{
                QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QApplication::sendEvent(w_temp, key);
                return true;
            }break;
            }
        }
        return false;
    }
    w_temp = ui->tableWidget;
    if(watched == w_temp){
        if(event->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)event;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:
                //ui->pushButton_salir->click();
                return true;
            case Qt::Key_Enter:{
                QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QApplication::sendEvent(w_temp, key);
                return true;
            }break;
            case Qt::Key_Down:{
                int index = ui->tableWidget->currentRow();
                if (index == ui->tableWidget->rowCount() - 1) {
                    on_dateEdit_fin_editingFinished();
                    return true;
                }
            }break;
            }

        }else{

        }
        return false;
    }
    w_temp = ui->pushButton_salir;
    if(watched == w_temp){
        if(event->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)event;

            switch(KeyEvent->key())
            {
            case Qt::Key_Tab:{
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
    return eventFilter(watched, event);
}
