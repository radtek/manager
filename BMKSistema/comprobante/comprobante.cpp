#include "comprobante.h"
#include "ui_comprobante.h"
#include <cwchar>
#include <comdef.h>

Comprobante::Comprobante(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Comprobante)
{
    ui->setupUi(this);    

    countPrintComanda = 0;

    blockWritten = 0;

    ui->tableWidget->verticalHeader()->setVisible(false);

    ui->comboBox_familia->hide();
    ui->comboBox_plato->hide();

    ui->label_familia_filas->hide();
    ui->spinBox_familia_filas->hide();
    ui->label_familia_columnas->hide();
    ui->spinBox_familia_columnas->hide();
    ui->horizontalSpacer_familia->changeSize(0, 0);

    ui->label_plato_filas->hide();
    ui->spinBox_plato_filas->hide();
    ui->label_plato_columnas->hide();
    ui->spinBox_plato_columnas->hide();
    ui->horizontalSpacer_plato->changeSize(0, 0);

    ui->tableWidget->hideColumn(3);

    //ui->doubleSpinBox_pago->hide();
    ui->lineEdit_pago->hide();

    ui->lineEdit_pago->setDecimals(2);
    //view_mapa = new QWebEngineView;
    //QWebEnginePage* page = view_mapa->page();
    //connect(page, SIGNAL(loadFinished(bool)), this, SLOT(loadFinished(bool)));
    //view_mapa->load(tr("https://www.google.com/maps/d/viewer?mid=1j-mQQ_g0HqMh7Xjvul9MWKUUzDI&hl=es&ll=-16.409078690356864%2C-71.51622014410395&z=14"));
    //connect(ui->splitter_vertical, SIGNAL(splitterMoved(int,int)), this, SLOT(on_splitter_moved(int, int)));
    //connect(ui->splitter_horizontal, SIGNAL(splitterMoved(int,int)), this, SLOT(on_splitter_moved(int, int)));    

    m_pSocket   = new QTcpSocket();
    //m_pSocket->setSocketOption(64, 1);

    connect(m_pSocket, SIGNAL(bytesWritten(qint64)), this, SLOT(on_socket_bytesWritten(qint64)));

    m_ConnectStatus = false;

    //ui->tableWidget->clear();
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);

    LineEditNumberDelegate* delegate = new LineEditNumberDelegate;
    delegate->setDecimals(0);
    delegate->setMinimumWidth(100);
    delegate->setMaximumWidth(100);
    delegate->setMinimumHeight(100);
    delegate->setMaximumHeight(100);
    ui->tableWidget->setItemDelegateForColumn(1, delegate);
    delegate = new LineEditNumberDelegate;
    delegate->setDecimals(1);
    delegate->setMinimumWidth(100);
    delegate->setMaximumWidth(100);
    delegate->setMinimumHeight(100);
    delegate->setMaximumHeight(100);
    ui->tableWidget->setItemDelegateForColumn(2, delegate);

    afterShow = false;

    firstShow = false;

    qDebug()<<"hola"<<endl;

    QRegExp regExp_ruc("[0-9]{11,11}");
    ui->lineEdit_codigo->setValidator(new QRegExpValidator(regExp_ruc));

    ui->lineEdit_codigo->setFocus();    

    this->installEventFilter(this);

    /*
    select_familias();
    ui->comboBox_familia->setCurrentIndex(1);
    select_platos(ui->comboBox_familia->currentText());
    */
}

Comprobante::~Comprobante()
{
    qDebug()<<"delete comprobante"<<endl;
    delete ui;
    if(m_pSocket){
        m_pSocket->disconnectFromHost();
    }
    //delete view_mapa;
    delete m_pSocket;
}
void Comprobante::delayedPopup()
{
    int val_x = ui->spinBox_familia_filas->value();
    int val_y = ui->spinBox_familia_columnas->value();
    for(int i = 0; i < val_x; i++) {
        for(int j = 0; j < val_y; j++) {
            QPushButton* tb = (QPushButton*)ui->gridLayout_familias->itemAtPosition(i, j)->widget();

        }
    }
    val_x = ui->spinBox_plato_filas->value();
    val_y = ui->spinBox_plato_columnas->value();
    for(int i = 0; i < val_x; i++) {
        for(int j = 0; j < val_y; j++) {
            QPushButton* tb = (QPushButton*)ui->gridLayout_platos->itemAtPosition(i, j)->widget();
            //tb->setPopupMode(QPushButton::DelayedPopup);
        }
    }
    repaint();
}
void Comprobante::menuButtonPopup()
{
    int val_x = ui->spinBox_familia_filas->value();
    int val_y = ui->spinBox_familia_columnas->value();
    for(int i = 0; i < val_x; i++) {
        for(int j = 0; j < val_y; j++) {
            QPushButton* tb = (QPushButton*)ui->gridLayout_familias->itemAtPosition(i, j)->widget();
            //tb->setPopupMode(QPushButton::MenuButtonPopup);
        }
    }
    val_x = ui->spinBox_plato_filas->value();
    val_y = ui->spinBox_plato_columnas->value();
    for(int i = 0; i < val_x; i++) {
        for(int j = 0; j < val_y; j++) {
            QPushButton* tb = (QPushButton*)ui->gridLayout_platos->itemAtPosition(i, j)->widget();
            //tb->setPopupMode(QPushButton::MenuButtonPopup);
        }
    }
    repaint();
}

void Comprobante::clean_familias()
{
    for(int i = 0; i < 99; i++){
        for(int j = 0; j < 99; j++){
            SYSTEM->remove(ui->gridLayout_familias, i, j, true);
        }
    }
}
void Comprobante::clean_platos()
{
    for(int i = 0; i < 99; i++){
        for(int j = 0; j < 99; j++){
            SYSTEM->remove(ui->gridLayout_platos, i, j, true);
        }
    }
}
void Comprobante::clean_void_familias()
{
    for(int i = 0; i < ui->spinBox_familia_filas->value(); i++){
        for(int j = 0; j < ui->spinBox_familia_columnas->value(); j++){

            QPushButton* pb = (QPushButton*)ui->gridLayout_familias->itemAtPosition(i, j)->widget();
            if(pb && pb->text().compare("____") == 0) {
                delete pb;
            }
        }
    }
}
void Comprobante::clean_void_platos()
{
    for(int i = 0; i < ui->spinBox_plato_filas->value(); i++){
        for(int j = 0; j < ui->spinBox_plato_columnas->value(); j++){
            QPushButton* pb = (QPushButton*)ui->gridLayout_platos->itemAtPosition(i, j)->widget();
            if(pb && pb->text().compare("____") == 0) {
                delete pb;
            }
        }
    }
}

void Comprobante::select_familias()
{
    QSqlQuery query;
    QString str_query = "";
    str_query += "SELECT item_nombre, x, y";
    str_query += ", font , color, background_color";
    str_query += " FROM familia";

    if(query.exec(str_query)){
        QString familita = ui->comboBox_familia->currentText();
        ui->comboBox_familia->clear();

        while(query.next()){
            QString nombre = query.value(0).toString();
            int x = query.value(1).toInt() - 1;
            int y = query.value(2).toInt() - 1;
            QString str_font = query.value(3).toString();
            QFont font;
            font.fromString(str_font);
            QString color = query.value(4).toString();
            QString background_color = query.value(5).toString();            

            int val_x = ui->spinBox_familia_filas->value();
            int val_y = ui->spinBox_familia_columnas->value();

            if (x < val_x && x >= 0 && y < val_y && y >= 0){
                ui->comboBox_familia->addItem(nombre);

                SYSTEM->set_pushButton_css(ui->gridLayout_familias, nombre
                                           , x, y, font
                                           , color, background_color);

                QPushButton* tb = (QPushButton*)ui->gridLayout_familias->itemAtPosition(x, y)->widget();
            }


            //delete tb->menu()->actions()[3];
        }

        ui->comboBox_familia->setCurrentText(familita);
    }
}
void Comprobante::select_platos(QString familia)
{
    QSqlQuery query;
    QString str_query = "";
    str_query += "SELECT plato.item_nombre, plato.x, plato.y";
    str_query += ", plato.font , plato.color, plato.background_color";
    str_query += ", plato.precio, plato.descripcion";
    str_query += " FROM plato";
    str_query += " JOIN familia ON familia.item_nombre = plato.familia_item_nombre";
    str_query += " WHERE familia.item_nombre = '"+familia+"'";

    if(query.exec(str_query)){
        ui->comboBox_plato->clear();

        QVector<QString> platos_existentes;
        while(query.next()){
            QString nombre = query.value(0).toString();
            int x = query.value(1).toInt() - 1;
            int y = query.value(2).toInt() - 1;
            QString str_font = query.value(3).toString();
            QFont font;
            font.fromString(str_font);
            QString color = query.value(4).toString();
            QString background_color = query.value(5).toString();
            QString precio = query.value(6).toString();
            QString descripcion = query.value(7).toString();

            for(int i = 0; i < ui->tableWidget->rowCount(); i++){
                if(nombre.compare(ui->tableWidget->item(i, 0)->text()) == 0){
                    platos_existentes.push_back(nombre);
                    double cant = ui->tableWidget->item(i, 1)->text().toDouble();
                    double value_precio = precio.toDouble() * cant;
                    ui->tableWidget->item(i, 2)->setText(QString().setNum(value_precio, ' ', 1));
                    ui->tableWidget->item(i, 3)->setText(descripcion);
                    break;
                }
            }

            int val_x = ui->spinBox_plato_filas->value();
            int val_y = ui->spinBox_plato_columnas->value();

            if(x < val_x && x >= 0 && y < val_y && y >= 0) {
                ui->comboBox_plato->addItem(nombre);

                SYSTEM->set_pushButton_css(ui->gridLayout_platos, nombre
                                           , x, y, font
                                           , color, background_color);

                QPushButton* tb = (QPushButton*)ui->gridLayout_platos->itemAtPosition(x, y)->widget();
            }
        }

        /*
        for(int i = 0; i < ui->tableWidget->rowCount(); i++){
            bool borrar = true;
            for(int j = 0; j < platos_existentes.size(); j++){
                if(platos_existentes[j].compare(ui->tableWidget->item(i, 0)->text()) == 0){
                    borrar = false;
                }
            }
            if(borrar){
                ui->tableWidget->removeRow(i);
                i--;
            }
        }*/

        if(ui->comboBox_plato->count() > 0){
            //ui->comboBox_plato->setCurrentText(platito);
            //if(ui->comboBox_plato->currentText().compare("") == 0){
                //ui->comboBox_plato->setCurrentIndex(0);
            //}
        }else{
            ui->comboBox_plato->clear();
            ui->comboBox_plato->clearEditText();
        }
    }
}
void Comprobante::insert_venta()
{
    /*
    int ret = QMessageBox::warning(this, "Advertencia", "¿Esta seguro de esta venta?", "Si", "Cancelar");
    switch(ret){
    case 0:{

    }break;
    case 1:{
        return;
    }break;
    }*/
    if(ui->lineEdit_nombre->text().compare("") == 0) {
        QMessageBox::warning(this, "Advertencia", "Ingrese nombre.", "Aceptar");
        return;
    }

    if(ui->tableWidget->rowCount() == 0){
        QMessageBox::warning(this, "Advertencia", "No hay detalle en la venta.", "Aceptar");
        return;
    }
    QString str_query = "";
    QSqlQuery query;

    str_query += "SELECT MAX(numero) + 1 FROM comprobante";
    if(query.exec(str_query)){
        if(query.next()){
            nro_ticket = QString().setNum(query.value(0).toInt());
            int n = 7 - nro_ticket.length();
            nro_ticket = SYSTEM->zeros(n) + nro_ticket;
        }
    }else{
        QMessageBox::critical(this, "Error", "Error inesperado", "Ok");
        return;
    }
    //str_query = "SET autocommit = 0";
    //str_query += "&&END_QUERY&&";

    QString nombre = ui->lineEdit_nombre->text();

    nombre = nombre.replace("\"", "");
    nombre = nombre.replace("'", "");

    str_query = "INSERT INTO comprobante(serie, numero, fecha_emision";
    str_query += ", anulado, pago_item_nombre, estado_item_nombre";
    str_query += ", operacion_item_nombre, tipo_item_nombre, nombre)";
    str_query += "VALUES(";
    str_query += QString() + "'" + "0000" + "'";
    str_query += QString() + ", '" + nro_ticket + "'";
    str_query += ", '" + QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm-ss") + "'";
    str_query += QString() + ", '" + "1" + "'";
    str_query += QString() + ", '" + pago + "'";
    str_query += QString() + ", '" + "Activo" + "'";
    str_query += QString() + ", '" + "Venta" + "'";
    str_query += QString() + ", '" + "Ticket" + "'";
    str_query += ", '" + nombre + "'";
    str_query += ")";
    str_query += "&&END_QUERY&&";
    if(ui->lineEdit_codigo->text().length() == 11){
        str_query += "INSERT INTO persona(cod, nombre, direccion";
        str_query += ", estado_item_nombre, tipo_item_nombre)";
        str_query += "VALUES(";
        str_query += "'" + ui->lineEdit_codigo->text() + "'";
        str_query += ", '" + nombre + "'";
        str_query += ", '" + ui->lineEdit_direccion->text() + "'";
        str_query += ", 'Activo'";
        str_query += ", 'Cliente_RUC'";
        str_query += ")";
        str_query += " ON DUPLICATE KEY UPDATE cod = '" + ui->lineEdit_codigo->text() + "'";
        str_query += "&&END_QUERY&&";

        str_query += "INSERT INTO comprobante_has_persona(";
        str_query += "comprobante_id, persona_cod)";
        str_query += "VALUES(";
        str_query += "(SELECT MAX(t.id) FROM comprobante AS t)";
        str_query += ", '" + ui->lineEdit_codigo->text() + "'";
        str_query += ")";
        str_query += "&&END_QUERY&&";
    }
    if(ui->lineEdit_codigo->text().length() == 8){
        str_query += "INSERT INTO persona(cod, nombre, direccion";
        str_query += ", estado_item_nombre, tipo_item_nombre)";
        str_query += "VALUES(";
        str_query += "'" + ui->lineEdit_codigo->text() + "'";
        str_query += ", '" + ui->lineEdit_nombre->text() + "'";
        str_query += ", '" + ui->lineEdit_direccion->text() + "'";
        str_query += ", 'Activo'";
        str_query += ", 'Cliente_DNI'";
        str_query += ")";
        str_query += " ON DUPLICATE KEY UPDATE cod = '" + ui->lineEdit_codigo->text() + "'";
        str_query += "&&END_QUERY&&";

        str_query += "INSERT INTO comprobante_has_persona(";
        str_query += "comprobante_id, persona_cod)";
        str_query += "VALUES(";
        str_query += "(SELECT MAX(t.id) FROM comprobante AS t)";
        str_query += ", '" + ui->lineEdit_codigo->text() + "'";
        str_query += ")";
        str_query += "&&END_QUERY&&";
    }
    if(ui->lineEdit_codigo->text().length() != 11 && ui->lineEdit_codigo->text().length() != 8){

    }

    for(int i = 0; i < ui->tableWidget->rowCount(); i++){
        str_query += "INSERT INTO comprobante_has_plato(comprobante_id, plato_item_nombre";
        str_query += ", cantidad)";
        str_query += "VALUES(";
        str_query += "(SELECT MAX(t.id) FROM comprobante AS t)";
        str_query += ", '" + ui->tableWidget->item(i, 0)->text() + "'";
        str_query += ", '" + ui->tableWidget->item(i, 1)->text() + "'";
        //str_query += ", '" + ui->tableWidget->item(i, 2)->text() + "'";
        str_query += ")";
        str_query += "&&END_QUERY&&";
    }

    SYSTEM->multiple_query(str_query);

    qDebug()<<str_query<<endl;
    if(query.exec(str_query)){
        qDebug()<<"query ok"<<endl;        

        //str_query = "SET autocommit = 1";
        //if(query.exec(str_query)){
            //print_caja();

            //print_caja();
            print_star_asa();
            print_comanda();

        //}else{
            //QMessageBox::critical(this, "Error", "Error inesperado", "Ok");
        //}


        //QMessageBox::information(this, "Información", "Se guardo la venta.", "Aceptar");
    }else{
        return;
    }
    /*
    str_query = "SET autocommit = 1";
    if(query.exec(str_query)){
        //print_caja();
        //print_comanda();

        print_star_asa();
    }else{
        return;
        QMessageBox::critical(this, "Error", "Error inesperado", "Ok");
    }
    */
    ui->lineEdit_codigo->clear();
    ui->lineEdit_direccion->clear();
    ui->lineEdit_nombre->clear();
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
}

void Comprobante::on_familia_closing()
{
    Familia* w = (Familia*) sender();
    if(w->isEliminar()) {
        for(int i=0; i < ui->tableWidget->rowCount(); i++){
            qDebug() << "i: " << i << endl;
            for(int ii = 0; ii < ui->spinBox_plato_filas->value(); ii++){
                bool break_ii = false;
                qDebug() << "ii: " << ii << endl;
                for(int jj = 0; jj < ui->spinBox_plato_columnas->value(); jj++){
                    qDebug() << "jj: " << jj << endl;
                    if(ui->gridLayout_platos->itemAtPosition(ii, jj)->widget() != 0){
                        QPushButton* tb = (QPushButton*)ui->gridLayout_platos->itemAtPosition(ii, jj)->widget();
                        if(tb){
                            QString plato = ui->tableWidget->item(i, 0)->text();
                            if(plato.compare(tb->text()) == 0){
                                ui->tableWidget->selectRow(i);
                                ui->pushButton_quitar->click();
                                break_ii = true;
                                break;
                            }
                        }
                    }
                }
                if(break_ii) break;
            }
        }
    }

    qDebug()<<"familia_closing"<<endl;
    ui->spinBox_familia_filas->setValue(ui->spinBox_familia_filas->value()+1);
    ui->spinBox_familia_filas->setValue(ui->spinBox_familia_filas->value()-1);
    select_familias();
}
void Comprobante::on_plato_closing()
{    
    Plato* w = (Plato*) sender();
    if(w->isEliminar()){
        for(int i=0; i < ui->tableWidget->rowCount(); i++){
            QString plato = ui->tableWidget->item(i, 0)->text();
            if(plato.compare(w->getPlato()) == 0){
                ui->tableWidget->selectRow(i);
                ui->pushButton_quitar->click();
                break;
            }
        }
    }

    qDebug()<<"plato_closing"<<endl;
    ui->spinBox_plato_filas->setValue(ui->spinBox_plato_filas->value()+1);
    ui->spinBox_plato_filas->setValue(ui->spinBox_plato_filas->value()-1);
    select_platos(ui->comboBox_familia->currentText());
}
void Comprobante::on_familia_fontAct_triggered(QAction* act)
{
    if(act->text().compare("&Fuente") == 0){        
        bool ok;
        QFont font = QFontDialog::getFont(
                      &ok, QFont("MS Shell Dlg 2", 8, -1, false), this);
        if (ok) {
            QPushButton* tButton = (QPushButton*)sender();
            tButton->setFont(font);

            QString str_query = "";
            QSqlQuery query;
            str_query += "UPDATE familia";
            str_query += " SET font = '" + font.toString() + "'";
            str_query += " WHERE item_nombre = '" + tButton->text() + "'";

            qDebug()<<str_query<<endl;
            if(query.exec(str_query)){
                qDebug()<<"query ok"<<endl;
            }
        } else {

        }
    }
}
void Comprobante::on_familia_colorAct_triggered(QAction* act)
{
    if(act->text().compare("&Color") == 0){
        QColor color = QColorDialog::getColor(Qt::black, this, "Pick Color");

        QPushButton* tButton = (QPushButton*)sender();

        QString bg_color = SYSTEM->extract_background_color(tButton->styleSheet());
        QString str_color = color.name();

        QString ss = "QPushButton{ color: "+str_color+"; background-color: "+bg_color+"; }";
        tButton->setStyleSheet(ss);

        QString str_query = "";
        QSqlQuery query;
        str_query += "UPDATE familia";
        str_query += " SET color = '" + color.name() + "'";
        str_query += " WHERE item_nombre = '" + tButton->text() + "'";

        qDebug()<<str_query<<endl;
        if(query.exec(str_query)){
            qDebug()<<"query ok"<<endl;
        }
    }
}
void Comprobante::on_familia_color_de_fondo_Act_triggered(QAction* act)
{
    if(act->text().compare("&Color de Fondo") == 0){
        QColor color = QColorDialog::getColor(Qt::black, this, "Pick Color");

        QPushButton* tButton = (QPushButton*)sender();

        QString bg_color = color.name();
        QString str_color = SYSTEM->extract_color(tButton->styleSheet());

        QString ss = "QPushButton{ color: "+str_color+"; background-color: "+bg_color+"; }";
        tButton->setStyleSheet(ss);

        QString str_query = "";
        QSqlQuery query;
        str_query += "UPDATE familia";
        str_query += " SET background_color = '" + color.name() + "'";
        str_query += " WHERE item_nombre = '" + tButton->text() + "'";

        qDebug()<<str_query<<endl;
        if(query.exec(str_query)){
            qDebug()<<"query ok"<<endl;
        }
    }
}
void Comprobante::on_familia_nuevoAct_triggered(QAction* act)
{
    if(act->text().compare("&Nuevo") == 0){
        AdminPass* ap = new AdminPass(this);
        //ap->setAttribute(Qt::WA_DeleteOnClose);
        ap->exec();
        bool confirmado = ap->get_confirmado();

        if(!confirmado){
            return;
        }
        delete ap;

        Familia* familia = new Familia(this);
        familia->setWindowFlag(Qt::Dialog);
        familia->setAttribute(Qt::WA_DeleteOnClose);
        familia->setWindowModality(Qt::WindowModal);

        QPushButton* tb = (QPushButton*)sender();

        int x = 0;
        int y = 0;
        for(int i = 0; i < ui->gridLayout_familias->rowCount(); i++){
            bool flag = false;
            for(int j = 0; j < ui->gridLayout_familias->columnCount(); j++){
                if(ui->gridLayout_familias->itemAtPosition(i, j)->widget() == tb){
                    x = i+1;
                    y = j+1;
                    flag = true;
                    break;
                }
            }
            if(flag)break;
        }
        familia->modo_ingresar(x, y);

        connect(familia, SIGNAL(closing()), this, SLOT(on_familia_closing()));

        familia->show();
    }
}
void Comprobante::on_familia_modificarAct_triggered(QAction* act)
{
    if(act->text().compare("&Modificar") == 0){
        QPushButton* tb = (QPushButton*)sender();

        if(tb->text().compare("____") == 0)
            return;

        AdminPass* ap = new AdminPass(this);
        //ap->setAttribute(Qt::WA_DeleteOnClose);
        ap->exec();
        bool confirmado = ap->get_confirmado();

        if(!confirmado){
            return;
        }

        Familia* familia = new Familia(this);
        familia->setWindowFlag(Qt::Dialog);
        familia->setAttribute(Qt::WA_DeleteOnClose);
        familia->setWindowModality(Qt::WindowModal);

        int x = 0;
        int y = 0;
        for(int i = 0; i < ui->gridLayout_familias->rowCount(); i++){
            bool flag = false;
            for(int j = 0; j < ui->gridLayout_familias->columnCount(); j++){
                if(ui->gridLayout_familias->itemAtPosition(i, j)->widget() == tb){
                    x = i+1;
                    y = j+1;
                    flag = true;
                    break;
                }
            }
            if(flag)break;
        }
        familia->modo_modificacion(tb->text(), x, y);

        connect(familia, SIGNAL(closing()), this, SLOT(on_familia_closing()));

        familia->show();
    }
}
void Comprobante::on_familia_quitarAct_triggered(QAction* act)
{
    if(act->text().compare("&Quitar") == 0){
        QPushButton* tb = (QPushButton*)sender();
        if(tb->text().compare("____") == 0)
            return;

        AdminPass* ap = new AdminPass(this);
        //ap->setAttribute(Qt::WA_DeleteOnClose);
        ap->exec();
        bool confirmado = ap->get_confirmado();

        if(!confirmado){
            return;
        }
        delete ap;        

        Familia* familia = new Familia(this);
        familia->setWindowFlag(Qt::Dialog);
        familia->setAttribute(Qt::WA_DeleteOnClose);
        familia->setWindowModality(Qt::WindowModal);

        familia->modo_eliminacion(tb->text());

        ui->comboBox_familia->setCurrentText(tb->text());

        connect(familia, SIGNAL(closing()), this, SLOT(on_familia_closing()));

        familia->show();
    }
}
void Comprobante::on_plato_fontAct_triggered(QAction* act)
{
    if(act->text().compare("&Fuente") == 0){
        bool ok;
        QFont font = QFontDialog::getFont(
                      &ok, QFont("MS Shell Dlg 2", 8, -1, false), this);
        if (ok) {
            QPushButton* tButton = (QPushButton*)sender();
            tButton->setFont(font);

            QString str_query = "";
            QSqlQuery query;
            str_query += "UPDATE plato";
            str_query += " SET font = '" + font.toString() + "'";
            str_query += " WHERE item_nombre = '" + tButton->text() + "'";

            qDebug()<<str_query<<endl;
            if(query.exec(str_query)){
                qDebug()<<"query ok"<<endl;
            }
        } else {

        }
    }
}
void Comprobante::on_plato_colorAct_triggered(QAction* act)
{
    if(act->text().compare("&Color") == 0){
        QColor color = QColorDialog::getColor(Qt::black, this, "Pick Color");

        QPushButton* tButton = (QPushButton*)sender();

        QString bg_color = SYSTEM->extract_background_color(tButton->styleSheet());
        QString str_color = color.name();

        QString ss = "QPushButton{ color: "+str_color+"; background-color: "+bg_color+"; }";
        tButton->setStyleSheet(ss);

        QString str_query = "";
        QSqlQuery query;
        str_query += "UPDATE plato";
        str_query += " SET color = '" + color.name() + "'";
        str_query += " WHERE item_nombre = '" + tButton->text() + "'";

        qDebug()<<str_query<<endl;
        if(query.exec(str_query)){
            qDebug()<<"query ok"<<endl;
        }
    }
}
void Comprobante::on_plato_color_de_fondo_Act_triggered(QAction* act)
{
    if(act->text().compare("&Color de Fondo") == 0){
        QColor color = QColorDialog::getColor(Qt::black, this, "Pick Color");

        QPushButton* tButton = (QPushButton*)sender();

        QString bg_color = color.name();
        QString str_color = SYSTEM->extract_color(tButton->styleSheet());

        QString ss = "QPushButton{ color: "+str_color+"; background-color: "+bg_color+"; }";
        tButton->setStyleSheet(ss);

        QString str_query = "";
        QSqlQuery query;
        str_query += "UPDATE plato";
        str_query += " SET background_color = '" + color.name() + "'";
        str_query += " WHERE item_nombre = '" + tButton->text() + "'";

        qDebug()<<str_query<<endl;
        if(query.exec(str_query)){
            qDebug()<<"query ok"<<endl;
        }
    }
}
void Comprobante::on_plato_nuevoAct_triggered(QAction* act)
{    
    if(act->text().compare("&Nuevo") == 0){
        AdminPass* ap = new AdminPass(this);
        //ap->setAttribute(Qt::WA_DeleteOnClose);
        ap->exec();
        bool confirmado = ap->get_confirmado();

        if(!confirmado){
            return;
        }
        delete ap;

        Plato* plato = new Plato(this);
        plato->setWindowFlag(Qt::Dialog);
        plato->setAttribute(Qt::WA_DeleteOnClose);
        plato->setWindowModality(Qt::WindowModal);

        QPushButton* tb = (QPushButton*)sender();

        int x = 0;
        int y = 0;
        for(int i = 0; i < ui->gridLayout_platos->rowCount(); i++){
            bool flag = false;
            for(int j = 0; j < ui->gridLayout_platos->columnCount(); j++){
                if(ui->gridLayout_platos->itemAtPosition(i, j)->widget() == tb){
                    x = i+1;
                    y = j+1;
                    flag = true;
                    break;
                }
            }
            if(flag)break;
        }
        plato->modo_ingresar(ui->comboBox_familia->currentText(), x, y);

        //plato->set_familia_(ui->comboBox_familia->currentText());

        connect(plato, SIGNAL(closing()), this, SLOT(on_plato_closing()));

        plato->show();
    }

}
void Comprobante::on_plato_modificarAct_triggered(QAction* act)
{    
    if(act->text().compare("&Modificar") == 0){
        QPushButton* tb = (QPushButton*)sender();
        if(tb->text().compare("____") == 0)
            return;

        AdminPass* ap = new AdminPass(this);
        //ap->setAttribute(Qt::WA_DeleteOnClose);
        ap->exec();
        bool confirmado = ap->get_confirmado();

        if(!confirmado){
            return;
        }
        delete ap;

        Plato* plato = new Plato(this);
        plato->setWindowFlag(Qt::Dialog);
        plato->setAttribute(Qt::WA_DeleteOnClose);
        plato->setWindowModality(Qt::WindowModal);


        int x = 0;
        int y = 0;
        for(int i = 0; i < ui->gridLayout_platos->rowCount(); i++){
            bool flag = false;
            for(int j = 0; j < ui->gridLayout_platos->columnCount(); j++){
                if(ui->gridLayout_platos->itemAtPosition(i, j)->widget() == tb){
                    x = i+1;
                    y = j+1;
                    flag = true;
                    break;
                }
            }
            if(flag)break;
        }
        plato->modo_modificacion(ui->comboBox_familia->currentText(), tb->text(), x, y);

        connect(plato, SIGNAL(closing()), this, SLOT(on_plato_closing()));

        plato->show();
    }
}
void Comprobante::on_plato_quitarAct_triggered(QAction* act)
{
    if(act->text().compare("&Quitar") == 0){
        QPushButton* tb = (QPushButton*)sender();
        if(tb->text().compare("____") == 0)
            return;

        AdminPass* ap = new AdminPass(this);
        //ap->setAttribute(Qt::WA_DeleteOnClose);
        ap->exec();
        bool confirmado = ap->get_confirmado();

        if(!confirmado){
            return;
        }
        delete ap;

        Plato* plato = new Plato(this);
        plato->setWindowFlag(Qt::Dialog);
        plato->setAttribute(Qt::WA_DeleteOnClose);
        plato->setWindowModality(Qt::WindowModal);

        plato->modo_eliminacion(ui->comboBox_familia->currentText(), tb->text());

        connect(plato, SIGNAL(closing()), this, SLOT(on_plato_closing()));

        plato->show();
    }
}
void Comprobante::on_cliente_closing()
{
    Cliente* cliente = (Cliente*)sender();
    QString cod = cliente->get_cod();

    if(cod.compare("") == 0)
        return;

    QString nombre = cliente->get_nombre();
    QString direccion = cliente->get_direccion();

    ui->lineEdit_codigo->setText(cod);
    ui->lineEdit_nombre->setText(nombre);
    ui->lineEdit_direccion->setText(direccion);

    ui->lineEdit_direccion->setFocus();
}

void Comprobante::on_pushButton_edit_cliente_clicked()
{
    Cliente* cliente = new Cliente(this);
    cliente->setWindowFlag(Qt::Dialog);
    cliente->setAttribute(Qt::WA_DeleteOnClose);
    cliente->setWindowModality(Qt::WindowModal);

    connect(cliente, SIGNAL(closing()), this, SLOT(on_cliente_closing()));

    cliente->show();
}

void Comprobante::on_comboBox_familia_activated(const QString &arg1)
{
    disconnect(ui->spinBox_plato_filas, SIGNAL(valueChanged(int))
               , this, SLOT(on_spinBox_plato_filas_valueChanged()));
    ui->spinBox_plato_filas->setValue(ui->spinBox_plato_filas->value()+1);
    connect(ui->spinBox_plato_filas, SIGNAL(valueChanged(int))
               , this, SLOT(on_spinBox_plato_filas_valueChanged()));
    ui->spinBox_plato_filas->setValue(ui->spinBox_plato_filas->value()-1);

    select_platos(arg1);
}

void Comprobante::on_comboBox_plato_activated(const QString &arg1)
{    
    QString str_query = "";
    QSqlQuery query;
    str_query += "SELECT precio, descripcion FROM plato";
    str_query += " WHERE item_nombre = '"+arg1+"'";
    if(query.exec(str_query)){
        if(query.next()){
            disconnect(ui->tableWidget, SIGNAL(itemChanged(QTableWidgetItem*))
                       , this, SLOT(on_tableWidget_itemChanged(QTableWidgetItem*)));

            for(int i = 0; i < ui->tableWidget->rowCount(); i++){
                if(arg1.compare(ui->tableWidget->item(i, 0)->text()) == 0){
                    int cant = ui->tableWidget->item(i, 1)->text().toInt();

                    ui->tableWidget->item(i, 1)->setText(QString().setNum(cant+1));
                    double precio = ui->tableWidget->item(i, 2)->text().toDouble();
                    precio /= cant;
                    precio *= (cant+1);
                    ui->tableWidget->item(i, 2)->setText(QString().setNum(precio, ' ', 1));

                    double total = 0.0;
                    for(int i = 0; i < ui->tableWidget->rowCount(); i++){                        
                        double precio = ui->tableWidget->item(i, 2)->text().toDouble();
                        total += precio;
                    }
                    ui->doubleSpinBox_total->setValue(total);
                    return;
                }
            }

            int rowCount = ui->tableWidget->rowCount();
            ui->tableWidget->setRowCount(rowCount+1);

            QTableWidgetItem* item_desc = new QTableWidgetItem(arg1);
            QTableWidgetItem* item_cant = new QTableWidgetItem("1");
            QTableWidgetItem* item_precio = new QTableWidgetItem("");
            QTableWidgetItem* item_descripcion = new QTableWidgetItem("");

            ui->tableWidget->setItem(rowCount, 0, item_desc);
            ui->tableWidget->item(rowCount, 0)->setFlags(Qt::ItemIsSelectable
                                                         | Qt::ItemIsEnabled);

            ui->tableWidget->setItem(rowCount, 1, item_cant);
            ui->tableWidget->item(rowCount, 1)->setFlags(Qt::ItemIsSelectable
                                                         | Qt::ItemIsEnabled);

            QString precio = query.value(0).toString();
            item_precio->setText(precio);
            ui->tableWidget->setItem(rowCount, 2, item_precio);
            ui->tableWidget->item(rowCount, 2)->setFlags(Qt::ItemIsSelectable
                                                         | Qt::ItemIsEnabled);
                                                         //| Qt::ItemIsEditable);

            QString descripcion = query.value(1).toString();
            item_descripcion->setText(descripcion);
            ui->tableWidget->setItem(rowCount, 3, item_descripcion);

            connect(ui->tableWidget, SIGNAL(itemChanged(QTableWidgetItem*))
                       , this, SLOT(on_tableWidget_itemChanged(QTableWidgetItem*)));

            SYSTEM->table_resize_to_contents(0, ui->tableWidget);

            double total = 0.0;
            for(int i = 0; i < ui->tableWidget->rowCount(); i++){
                double precio = ui->tableWidget->item(i, 2)->text().toDouble();
                total += precio;
            }
            ui->doubleSpinBox_total->setValue(total);
        }
    }

}
void Comprobante::on_spinBox_familia_filas_valueChanged(int arg1)
{    
    clean_familias();

    int x = arg1;
    int y = ui->spinBox_familia_columnas->value();
    for(int i = 0; i < x; i++){
        for(int j = 0; j < y; j++){
            QPushButton* tb = new QPushButton;
            tb->setText("____");

            connect(tb, SIGNAL(clicked()), this, SLOT(on_toolButton_familia_clicked()));

            ui->gridLayout_familias->addWidget(tb, i, j);
        }
    }    
    select_familias();

    clean_void_familias();
}

void Comprobante::on_spinBox_familia_columnas_valueChanged(int arg1)
{
    arg1 = 0;
    on_spinBox_familia_filas_valueChanged(ui->spinBox_familia_filas->value());
}

void Comprobante::on_spinBox_plato_filas_valueChanged(int arg1)
{    
    clean_platos();

    int x = arg1;
    int y = ui->spinBox_plato_columnas->value();
    for(int i = 0; i < x; i++){
        for(int j = 0; j < y; j++){
            QPushButton* tb = new QPushButton;
            tb->setText("____");

            connect(tb, SIGNAL(clicked()), this, SLOT(on_toolButton_plato_clicked()));

            ui->gridLayout_platos->addWidget(tb, i, j);
        }
    }
    qDebug() << "select plato" << endl;
    select_platos(ui->comboBox_familia->currentText());
    qDebug() << "fin" << endl;

    clean_void_platos();
}

void Comprobante::on_spinBox_plato_columnas_valueChanged(int arg1)
{
    arg1 = 0;
    on_spinBox_plato_filas_valueChanged(ui->spinBox_plato_filas->value());
}
void Comprobante::on_pushButton_efectivo_clicked()
{
    this->pago = "Efectivo";
    insert_venta();
}

void Comprobante::on_pushButton_masterCard_clicked()
{
    this->pago = "Master Card";
    insert_venta();
}

void Comprobante::on_pushButton_visa_clicked()
{
    this->pago = "Visa";
    insert_venta();
}
void Comprobante::showEvent(QShowEvent *event)
{
    event->accept();

    afterShow = true;    

    //if(isMaximized()){
        qDebug()<<"isMaximized"<<endl;

        QString str_query = "";
        QSqlQuery query;

        str_query += "SELECT splitter_1, splitter_2, splitter_3";
        str_query += ", familia_filas, familia_columnas";
        str_query += ", plato_filas, plato_columnas, clave_wifi, fondo_tapiz FROM datos";

        qDebug()<<str_query<<endl;
        if(query.exec(str_query)){
            qDebug()<<"query ok"<<endl;
            if(!query.next()){
                return;
            }

            int splitter_1_val = query.value(0).toInt();
            int splitter_2_val = query.value(1).toInt();
            //int splitter_3_val = query.value(2).toInt();
            int familia_filas = query.value(3).toInt();
            int familia_columnas = query.value(4).toInt();
            int plato_filas = query.value(5).toInt();
            int plato_columnas = query.value(6).toInt();
            QString clave_wifi = query.value(7).toString();
            QString fondoTapiz = query.value(8).toString();

            //const QRect mainScreenSize = this->rect();
            ui->splitter_mid->setSizes(QList<int>() << splitter_1_val << splitter_2_val);

            disconnect(ui->spinBox_familia_filas, SIGNAL(valueChanged(int))
                       , this, SLOT(on_spinBox_familia_filas_valueChanged()));
            disconnect(ui->spinBox_familia_columnas, SIGNAL(valueChanged(int))
                       , this, SLOT(on_spinBox_familia_columnas_valueChanged()));

            disconnect(ui->spinBox_plato_filas, SIGNAL(valueChanged(int))
                       , this, SLOT(on_spinBox_plato_filas_valueChanged()));
            disconnect(ui->spinBox_plato_columnas, SIGNAL(valueChanged(int))
                       , this, SLOT(on_spinBox_plato_columnas_valueChanged()));

            ui->spinBox_familia_filas->setValue(familia_filas);
            ui->spinBox_familia_columnas->setValue(familia_columnas);
            ui->spinBox_plato_filas->setValue(plato_filas);
            ui->spinBox_plato_columnas->setValue(plato_columnas);

            ui->spinBox_familia_filas->setValue(ui->spinBox_familia_filas->value()+1);
            connect(ui->spinBox_familia_filas, SIGNAL(valueChanged(int))
                       , this, SLOT(on_spinBox_familia_filas_valueChanged()));
            ui->spinBox_familia_filas->setValue(ui->spinBox_familia_filas->value()-1);

            ui->spinBox_plato_filas->setValue(ui->spinBox_plato_filas->value()+1);
            connect(ui->spinBox_plato_filas, SIGNAL(valueChanged(int))
                       , this, SLOT(on_spinBox_plato_filas_valueChanged()));
            ui->spinBox_plato_filas->setValue(ui->spinBox_plato_filas->value()-1);

            connect(ui->spinBox_familia_columnas, SIGNAL(valueChanged(int))
                       , this, SLOT(on_spinBox_familia_columnas_valueChanged()));

            connect(ui->spinBox_plato_columnas, SIGNAL(valueChanged(int))
                       , this, SLOT(on_spinBox_plato_columnas_valueChanged()));

            this->clave = clave_wifi;            

            this->fondoTapiz = fondoTapiz;
            qDebug() << "comprobante show: " << fondoTapiz << endl;

            QString ss;
            ss = "QSplitter::handle{ background-color: black }\n"
                 "QWidget{ background-color: " + fondoTapiz +" }\n"
                 "QLineEdit{ background-color: rgb(255, 255, 255) }\n"
                 "QLineEdit{ color: rgb(0, 0, 0) }\n"
                 "QTableWidget{ background-color: rgb(255, 255, 255) }\n"
                 "QComboBox{ background-color: rgb(255, 255, 255) }\n"
                 "QSpinBox{ background-color: rgb(255, 255, 255) }\n"
                 "QDoubleSpinBox{ background-color: rgb(255, 255, 255) }\n"
                 "QHeaderView{ background-color: rgb(255, 255, 255) }\n"
                 "QScrollBar{ background-color: rgb(255, 255, 255) }\n"
                 "QPushButton{ background-color: rgb(255, 255, 255) }\n"
                 "QToolButton{ background-color: rgb(255, 255, 255) }\n"
                 "QMenu{ background-color: rgb(255, 255, 255) }";
            this->setStyleSheet(ss);
        }else{
            qDebug()<<"query bad"<<endl;

        }
    //}
}
void Comprobante::closeEvent(QCloseEvent *event)
{
    event->accept();

    QString str_query = "";
    QSqlQuery query;

    str_query += "UPDATE datos SET";
    str_query += " splitter_1 = " + QString().setNum(ui->splitter_mid->sizes()[0]);
    str_query += ", splitter_2 = " + QString().setNum(ui->splitter_mid->sizes()[1]);
    str_query += ", familia_filas = " + QString().setNum(ui->spinBox_familia_filas->value());
    str_query += ", familia_columnas = " + QString().setNum(ui->spinBox_familia_columnas->value());
    str_query += ", plato_filas = " + QString().setNum(ui->spinBox_plato_filas->value());
    str_query += ", plato_columnas = " + QString().setNum(ui->spinBox_plato_columnas->value());
    str_query += ", fondo_tapiz = '" + fondoTapiz + "'";

    qDebug() << ui->splitter_mid->sizes()[0] << endl;
    qDebug()<<str_query<<endl;
    if(query.exec(str_query)){        
        qDebug()<<"query ok"<<endl;
    }else{
        qDebug()<<"query bad"<<endl;
    }
}

bool Comprobante::eventFilter(QObject *watched, QEvent *event)
{    
    QWidget* w_temp;
    w_temp = this;
    if(watched == w_temp){
        if (event->type() == QEvent::Resize)
        {
            QResizeEvent* resizeEv = static_cast<QResizeEvent*>(event);
            resizeEv->size();
            //qDebug() << resizeEv->size()<<endl;
            //qDebug()<<this->size().height()<<endl;
            return true;
        }
        if (event->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent* mousePress = static_cast<QMouseEvent*>(event);
            mousePress->pos();
            return true;
        }
        if (event->type() == QEvent::MouseButtonRelease)
        {
            QMouseEvent* mouseRelease = static_cast<QMouseEvent*>(event);
            mouseRelease->pos();
            return true;
        }

        if(event->type() == QEvent::Paint){
            if(focusWidget()){
                focusWidget()->setFocus();
            }
            if(afterShow) {                

                afterShow = false;
            }
            if(!firstShow) {
                firstShow = true;

                QLabel* label_captcha = new QLabel(this);

                QLineEdit* lineEdit_captcha = new QLineEdit(this);


                QTableWidget* tableWidget_establecimientos = new QTableWidget(this);


                sunat_consultaRUC.set_data(label_captcha, ui->lineEdit_codigo
                                           , lineEdit_captcha, ui->lineEdit_nombre, 0, 0
                                           , ui->lineEdit_direccion, 0, 0
                                           , tableWidget_establecimientos);

                label_captcha->hide();
                lineEdit_captcha->hide();
                tableWidget_establecimientos->hide();

                reniec_consultDNI.set_data(label_captcha, ui->lineEdit_codigo
                                           , lineEdit_captcha, ui->lineEdit_nombre
                                           , ui->lineEdit_direccion);
            }

            return true;
        }
        if(event->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)event;
            switch(KeyEvent->key())
            {
            case Qt::Key_Escape:{

                return true;
            }break;
            }
        }
        return false;
    }
    if(watched == ui->comboBox_familia){
        QKeyEvent* ke = (QKeyEvent*)event;
        switch(ke->key()){
        case Qt::Key_Return:{
            qDebug()<<"return comboBox"<<endl;

            return false;
        }break;
        }
        return false;
    }

    return eventFilter(watched, event);
}

void Comprobante::on_pushButton_subir_clicked()
{
    disconnect(ui->tableWidget, SIGNAL(itemChanged(QTableWidgetItem*))
               , this, SLOT(on_tableWidget_itemChanged(QTableWidgetItem*)));

    SYSTEM->table_button_up(ui->tableWidget);

    connect(ui->tableWidget, SIGNAL(itemChanged(QTableWidgetItem*))
               , this, SLOT(on_tableWidget_itemChanged(QTableWidgetItem*)));
}

void Comprobante::on_pushButton_bajar_clicked()
{
    disconnect(ui->tableWidget, SIGNAL(itemChanged(QTableWidgetItem*))
               , this, SLOT(on_tableWidget_itemChanged(QTableWidgetItem*)));

    SYSTEM->table_button_down(ui->tableWidget);

    connect(ui->tableWidget, SIGNAL(itemChanged(QTableWidgetItem*))
               , this, SLOT(on_tableWidget_itemChanged(QTableWidgetItem*)));
}

void Comprobante::on_pushButton_quitar_clicked()
{
    disconnect(ui->tableWidget, SIGNAL(itemChanged(QTableWidgetItem*))
               , this, SLOT(on_tableWidget_itemChanged(QTableWidgetItem*)));

    SYSTEM->table_button_borrar(ui->tableWidget);

    connect(ui->tableWidget, SIGNAL(itemChanged(QTableWidgetItem*))
               , this, SLOT(on_tableWidget_itemChanged(QTableWidgetItem*)));

    double total = 0.0;
    for(int i = 0; i < ui->tableWidget->rowCount(); i++){
        double precio = ui->tableWidget->item(i, 2)->text().toDouble();
        total += precio;
    }
    ui->doubleSpinBox_total->setValue(total);
}

void Comprobante::on_toolButton_familia_clicked()
{
    QPushButton* toolB = (QPushButton*)sender();

    ui->comboBox_familia->setCurrentText(toolB->text());


    //ui->spinBox_plato_filas->setValue(ui->spinBox_plato_filas->value()+1);
    //ui->spinBox_plato_filas->setValue(ui->spinBox_plato_filas->value()-1);

    //select_platos(toolB->text());
}

void Comprobante::on_toolButton_plato_clicked()
{
    QPushButton* toolB = (QPushButton*)sender();
    on_comboBox_plato_activated(toolB->text());
}

void Comprobante::on_tableWidget_itemChanged(QTableWidgetItem *item)
{
    disconnect(ui->tableWidget, SIGNAL(itemChanged(QTableWidgetItem*))
               , this, SLOT(on_tableWidget_itemChanged(QTableWidgetItem*)));

    if(item->column() == 0){
        double total = 0.0;
        for(int i = 0; i < ui->tableWidget->rowCount(); i++){
            double cantidad = ui->tableWidget->item(i, 0)->text().toDouble();
            double precio = ui->tableWidget->item(i, 2)->text().toDouble();
            total += cantidad * precio;
        }
        ui->doubleSpinBox_total->setValue(total);
    }
    connect(ui->tableWidget, SIGNAL(itemChanged(QTableWidgetItem*))
               , this, SLOT(on_tableWidget_itemChanged(QTableWidgetItem*)));
}

void Comprobante::on_doubleSpinBox_pago_valueChanged(double arg1)
{
    double pago = arg1;
    double total = ui->doubleSpinBox_total->value();
    double vuelto = pago - total;

    ui->doubleSpinBox_vuelto->setValue(vuelto);
}

void Comprobante::on_doubleSpinBox_total_valueChanged(double arg1)
{
    double pago = ui->doubleSpinBox_pago->value();
    double total = arg1;
    double vuelto = pago - total;

    ui->doubleSpinBox_vuelto->setValue(vuelto);
}
void Comprobante::on_thread_finished()
{
    delete sender();

}
void Comprobante::on_myProccess_started()
{
    qDebug() << "started" << endl;
}
void Comprobante::on_myProccess_finished(int exitCode, QProcess::ExitStatus exitStatus)
{
    qDebug() << "Exit Code: " << exitCode;
    qDebug() << "Exit Status: " << exitStatus;

    if (exitCode == 0) {
        QMessageBox::information(this, "Información", "Felicidades. Finalizo la operacion sin errores.", "Ok");
    } else {
        QMessageBox::critical(this, "Error", "No finalizo corretamente.");
    }

    delete sender();
}
void Comprobante::print_caja()
{
    QFile file("print_caja.dat");
    if (!file.open(QFile::WriteOnly)) {
        qDebug() << "Could not open file for writing";
        return;
    }
    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_4_8);    

    QVector<QString> centerTexts;
    centerTexts.push_back("BILL MC QUACK");
    centerTexts.push_back("de Giancarlo Miguel Figueroa Nuñes");
    centerTexts.push_back("Coop. Daniel Alcides Carrion M-14");
    centerTexts.push_back("RUC: 10430252718");
    centerTexts.push_back("----------------------------------------");
    centerTexts.push_back("Ticket Boleta");
    centerTexts.push_back("Maq.Regist.No: FFGF006292");
    centerTexts.push_back("Autorizacion Sunat: 0053845106569");
    centerTexts.push_back("Ticket Nro.: 012-2-" + nro_ticket);
    QString nombre = "NOM CLI : " + ui->lineEdit_nombre->text();
    if(nombre.length() > 40){
        centerTexts.push_back(nombre.mid(0, 40));
        centerTexts.push_back(nombre.mid(40, nombre.length() - 40));
    }else{
        centerTexts.push_back(nombre);
    }


    /*
    while((nombre.length() % 40) == 0){
        QString temp = nombre.mid(0, 40);
        centerTexts.push_back(temp);
        nombre.remove(0, 40);
    }*/

    centerTexts.push_back("Fecha: " + QDate::currentDate().toString("dd/MM/yyyy"));
    centerTexts.push_back("Hora: " + QDateTime::currentDateTime().toString("hh:mm:ss"));
    centerTexts.push_back("========================================");
    centerTexts.push_back("Cant  Descripcion                Importe");
    centerTexts.push_back("----------------------------------------");

    SYSTEM->centerTexts(centerTexts);

    out << (qint64)0x1D4C0002;

    SYSTEM->epson_lineFeed(out);

    for(int i = 0; i < centerTexts.size(); i++){
        //SYSTEM->insert_left_spaces(centerTexts[i], 3);
        //SYSTEM->insert_right_spaces(centerTexts[i], 3);

        SYSTEM->epson_printText(out, centerTexts[i]);
        //SYSTEM->epson_lineFeed(out);
    }
    //SYSTEM->epson_lineFeed(out);
    double total = 0.0, igv = 0.18;
    for(int i = 0; i < ui->tableWidget->rowCount(); i++){
        QString descripcion = ui->tableWidget->item(i, 0)->text();
        QString cantidad = ui->tableWidget->item(i, 1)->text();
        double p_total = ui->tableWidget->item(i, 2)->text().toDouble();
        QString str_p_total = QString().setNum(p_total, ' ', 1);

        SYSTEM->epson_just_descripcion(descripcion),
        total += p_total;

        SYSTEM->justified(cantidad, 5);
        SYSTEM->justified(str_p_total, 6);

        QString text = cantidad + " "
                + descripcion + " " + str_p_total;
        SYSTEM->epson_printText(out, text);

        //SYSTEM->epson_lineFeed(out);
    }
    QString str_st = QString().setNum(total - (total * igv), ' ', 1);
    SYSTEM->justified(str_st, 6);
    QString str_i = QString().setNum(total * igv, ' ', 1);
    SYSTEM->justified(str_i, 6);
    QString str_t = QString().setNum(total, ' ', 1);
    SYSTEM->justified(str_t, 6);
    QString str_v = QString().setNum(ui->doubleSpinBox_vuelto->value(), ' ', 1);
    SYSTEM->justified(str_v, 6);
    SYSTEM->justified(str_t, 6);
    QString str_sub_total = "Sub-Total: " + str_st;
    QString str_igv = "IGV: " + str_i;
    QString str_total = "Total: " + str_t;
    QString str_vuelto = "Vuelto: " + str_v;
    QVector<QString> totales;
    totales.push_back("----------------------------------------");
    totales.push_back(str_sub_total);
    totales.push_back(str_igv);
    totales.push_back(str_total);
    totales.push_back("----------------------------------------");
    totales.push_back(str_vuelto);
    QString forma_de_pago = QString("Forma de Pago:" + pago.toUpper());
    totales.push_back(SYSTEM->centerText(forma_de_pago));
    totales.push_back(" ");
    QString clave_wifi = "*** Clave Wifi: " + clave + " ***";
    totales.push_back(SYSTEM->centerText(clave_wifi));

    SYSTEM->rightTexts(totales);

    SYSTEM->epson_lineFeed(out);

    for(int i = 0; i < totales.size(); i++){
        SYSTEM->epson_printText(out, totales[i]);
        //SYSTEM->epson_lineFeed(out);
    }
    //SYSTEM->epson_lineFeed(out);

    SYSTEM->epson_lineFeed(out);
    SYSTEM->epson_lineFeed(out);
    SYSTEM->epson_lineFeed(out);
    SYSTEM->epson_lineFeed(out);
    SYSTEM->epson_lineFeed(out);
    SYSTEM->epson_lineFeed(out);
    SYSTEM->epson_lineFeed(out);
    SYSTEM->epson_lineFeed(out);
    //SYSTEM->epson_lineFeed(out);

    SYSTEM->epson_paperCut(out);

    file.close();

    QString app_dir = QCoreApplication::applicationDirPath();

    QString cmp_release = app_dir.mid(app_dir.length()-8, 9);
    QString str_rel = "";
    qDebug() << cmp_release << endl;
    if(cmp_release.compare("/release") == 0){
        str_rel = "/release";
        qDebug() << str_rel << endl;
    }

    app_dir = app_dir.mid(0, app_dir.length() - str_rel.length());
    app_dir.replace("/","\\");
    app_dir = "\"" + app_dir + "\\" + file.fileName() + "\"";

    //app_dir = QDir::fromNativeSeparators(app_dir);

    //QString lpt_print = QDir::fromNativeSeparators("\\\\localhost\\caja001");
    QString lpt_print = "\\\\localhost\\caja001";

    //FILE * pFile;
    //char buffer[] = { (char)0x0a };
    //pFile = fopen ("c:\\test.txt", "wb");
    //fwrite (buffer , sizeof(char), sizeof(buffer), pFile);
    //fclose (pFile);
    //system(command.toStdString().c_str());

    //QProcess *myProcess = new QProcess();
    QString command = "copy /b " + app_dir
                        + " " + lpt_print + " < nul";

    /*
    QStringList arguments = QStringList();
    arguments << argument_1;
    connect(myProcess, SIGNAL(started())
        , this, SLOT(on_myProccess_started()));
    connect(myProcess, SIGNAL(finished(int, QProcess::ExitStatus))
        , this, SLOT(on_myProccess_finished(int, QProcess::ExitStatus)));
*/
    //WinExec(command.toStdString().c_str(), SW_HIDE);
    qDebug() << command << endl;
    system(command.toStdString().c_str());
    //qDebug() << command << " " << argument_1 << endl;
    //myProcess->waitForFinished(10000);
    //myProcess->start(command, QStringList(), QIODevice::ReadWrite | QIODevice::Text);
    //myProcess->waitForFinished();
    //myProcess->waitForReadyRead();

}
void Comprobante::print_comanda()
{
    const int Timeout = 5 * 1000;

    if (!m_ConnectStatus)
    {
        //m_pSocket->setSocketOption(QAbstractSocket::KeepAliveOption, 1);
        m_pSocket->connectToHost(QHostAddress("192.168.1.39"), 9100, QIODevice::WriteOnly);
        //m_pSocket->connectToHost(QHostAddress("192.168.1.36"), 9100, QIODevice::WriteOnly);
    }

    if (!m_pSocket->waitForConnected(Timeout))
    {
        //sent error
        qDebug ("error in waitForConnected()");
        qDebug (qPrintable(m_pSocket->errorString()));
        m_ConnectStatus = false;
        return;
    }
    m_ConnectStatus = true;    

    QByteArray block;

    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);

    //out << (qint64)0x1B3F0A00;
    // COMANDO SET TRANSMITION DATA AND INITIALIZE PRINTER
    out << (qint64)0x00001B40;
    out << (qint64)0x1B1E6101;

    SYSTEM->star_print_text(out, SYSTEM->centerText(QString("COMANDA - COCINA")));
    SYSTEM->star_line_feed(out);
    SYSTEM->star_print_text(out, SYSTEM->centerText(QString("PEDIDO")));
    SYSTEM->star_line_feed(out);
    SYSTEM->star_line_feed(out);

    QVector<QString> topText;    
    QString nombre = "NOM CLI : " + ui->lineEdit_nombre->text();
    if(nombre.length() > 40){
        SYSTEM->star_print_text(out, nombre.mid(0, 40));
        SYSTEM->star_line_feed(out);
        SYSTEM->star_print_text(out, nombre.mid(40, nombre.length() - 40));
    }else{
        SYSTEM->star_print_text(out, nombre);
    }
    SYSTEM->star_line_feed(out);
    SYSTEM->star_line_feed(out);
    SYSTEM->star_print_text(out, "FECHA: " + QDate::currentDate().toString("dd-MM-yyyy"));
    SYSTEM->star_line_feed(out);
    SYSTEM->star_line_feed(out);

    SYSTEM->star_print_text(out, "========================================");

    SYSTEM->centerTexts(topText);

    for(int i = 0; i < topText.size(); i++){
        SYSTEM->star_print_text(out, topText[i]);
        SYSTEM->star_line_feed(out);
    }
    SYSTEM->star_line_feed(out);

    for(int i = 0; i < ui->tableWidget->rowCount(); i++){        
        QString descripcion = ui->tableWidget->item(i, 0)->text();
        QString cantidad = ui->tableWidget->item(i, 1)->text();
        QString descripcion_ = ui->tableWidget->item(i, 3)->text();

        QString().setNum(cantidad.toDouble(), ' ', 0);

        SYSTEM->justified(cantidad, 5);
        SYSTEM->star_print_text(out, cantidad + " " + descripcion);
        SYSTEM->star_line_feed(out);

        if(descripcion_.length() == 0) continue;

        if(descripcion_.length() <= 40){
            SYSTEM->star_print_text(out, descripcion_);
            SYSTEM->star_line_feed(out);
        }else{
            SYSTEM->star_print_text(out, descripcion_.mid(0, 40));
            SYSTEM->star_line_feed(out);
            SYSTEM->star_print_text(out, descripcion_.mid(40, 40));
            SYSTEM->star_line_feed(out);
            if(descripcion_.length() > 80) {
                SYSTEM->star_print_text(out, descripcion_.mid(80, 40));
                SYSTEM->star_line_feed(out);
            }
        }
    }    

    SYSTEM->star_line_feed(out);
    QVector<QString> bottomText;
    bottomText.push_back("HORA: " + QDateTime::currentDateTime().toString("hh:mm:ss"));
    SYSTEM->centerTexts(bottomText);
    for(int i = 0; i < bottomText.size(); i++){
        SYSTEM->star_print_text(out, bottomText[i]);
        SYSTEM->star_line_feed(out);
    }

    SYSTEM->star_line_feed(out);
    SYSTEM->star_line_feed(out);
    SYSTEM->star_line_feed(out);
    SYSTEM->star_line_feed(out);
    SYSTEM->star_line_feed(out);

    // COMANDA ALAS PERUANAS
    //out << (qint64)0x001D5601;
    // COMANDA ASA
    out << (qint64)0x001B6401;

    SYSTEM->star_line_feed(out);
    SYSTEM->star_line_feed(out);

    qDebug() << block.length() << endl;
    countPrintComanda = 0;
    blockWritten = block;


    m_pSocket->write(block);



    //m_pSocket->write(b);

    m_pSocket->flush();

    m_pSocket->waitForBytesWritten();

    m_pSocket->disconnectFromHost();

    m_ConnectStatus = false;

}
void Comprobante::print_star_asa()
{
    QFile file("print_caja_asa.dat");
    if (!file.open(QFile::WriteOnly)) {
        qDebug() << "Could not open file for writing";
        return;
    }
    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_4_8);

    QVector<QString> centerTexts;
    centerTexts.push_back("BILL MC QUACK");
    centerTexts.push_back("de Patricia Cristal Guerra Guerra");
    centerTexts.push_back("RUC: 10702899457");
    centerTexts.push_back("LOS ANDES 503");
    centerTexts.push_back("A.S.A. - AREQUIPA - AREQUIPA");
    centerTexts.push_back("----------------------------------------");
    centerTexts.push_back("Ticket Boleta");
    //centerTexts.push_back("Maq.Regist.No: ");
    centerTexts.push_back("Aut: 0053845087990  Delivery; 38 49 29");
    centerTexts.push_back("Ticket Nro.: " + nro_ticket);
    centerTexts.push_back("Serie: 150230600349P");
    QString nombre = "NOM CLI : " + ui->lineEdit_nombre->text();
    if(nombre.length() > 40){
        centerTexts.push_back(nombre.mid(0, 40));
        centerTexts.push_back(nombre.mid(40, nombre.length() - 40));
    }else{
        centerTexts.push_back(nombre);
    }

    centerTexts.push_back("Fecha: " + QDate::currentDate().toString("dd/MM/yyyy"));
    centerTexts.push_back("Hora: " + QDateTime::currentDateTime().toString("hh:mm:ss"));
    centerTexts.push_back("========================================");
    centerTexts.push_back("Cant: Descripcion                Importe");
    centerTexts.push_back("----------------------------------------");

    SYSTEM->centerTexts(centerTexts);

    //out << (qint64)0x1D4C0002;

    SYSTEM->star_line_feed(out);

    for(int i = 0; i < centerTexts.size(); i++){
        //SYSTEM->insert_left_spaces(centerTexts[i], 3);
        //SYSTEM->insert_right_spaces(centerTexts[i], 3);

        SYSTEM->star_print_text(out, centerTexts[i]);
        SYSTEM->star_line_feed(out);
    }
    //SYSTEM->epson_lineFeed(out);
    double total = 0.0, igv = 0.18;
    for(int i = 0; i < ui->tableWidget->rowCount(); i++){        
        QString descripcion = ui->tableWidget->item(i, 0)->text();
        QString cantidad = ui->tableWidget->item(i, 1)->text();
        double p_total = ui->tableWidget->item(i, 2)->text().toDouble();
        QString str_p_total = QString().setNum(p_total, ' ', 1);

        SYSTEM->epson_just_descripcion(descripcion),
        total += p_total;

        SYSTEM->justified(cantidad, 5);
        SYSTEM->justified(str_p_total, 6);

        QString text = cantidad + " "
                + descripcion + " " + str_p_total;
        SYSTEM->star_print_text(out, text);

        SYSTEM->star_line_feed(out);
    }
    QString str_st = QString().setNum(total - (total * igv), ' ', 1);
    SYSTEM->justified(str_st, 6);
    QString str_i = QString().setNum(total * igv, ' ', 1);
    SYSTEM->justified(str_i, 6);
    QString str_t = QString().setNum(total, ' ', 1);
    SYSTEM->justified(str_t, 6);
    QString str_v = QString().setNum(ui->doubleSpinBox_vuelto->value(), ' ', 1);
    SYSTEM->justified(str_v, 6);
    SYSTEM->justified(str_t, 6);
    QString str_sub_total = "Sub-Total: " + str_st;
    QString str_igv = "IGV: " + str_i;
    QString str_total = "Total: " + str_t;
    QString str_vuelto = "Vuelto: " + str_v;
    QVector<QString> totales;
    totales.push_back("----------------------------------------");
    //totales.push_back(str_sub_total);
    //totales.push_back(str_igv);
    totales.push_back(str_total);
    totales.push_back("----------------------------------------");
    totales.push_back(str_vuelto);
    QString forma_de_pago = QString("Forma de Pago:" + pago.toUpper());
    totales.push_back(SYSTEM->centerText(forma_de_pago));
    totales.push_back(" ");
    QString clave_wifi = "*** Clave Wifi: " + clave + " ***";
    totales.push_back(SYSTEM->centerText(clave_wifi));

    SYSTEM->rightTexts(totales);

    SYSTEM->star_line_feed(out);

    for(int i = 0; i < totales.size(); i++){
        SYSTEM->star_print_text(out, totales[i]);
        SYSTEM->star_line_feed(out);
    }
    //SYSTEM->epson_lineFeed(out);    

    SYSTEM->star_line_feed(out);
    SYSTEM->star_line_feed(out);
    SYSTEM->star_line_feed(out);
    SYSTEM->star_line_feed(out);
    SYSTEM->star_line_feed(out);
    SYSTEM->star_line_feed(out);
    SYSTEM->star_line_feed(out);
    SYSTEM->star_line_feed(out);

    out << (qint64)0x001B6401;
    //SYSTEM->star_line_feed(out);
    //SYSTEM->star_line_feed(out);

    //SYSTEM->epson_lineFeed(out);

    file.close();

    QString app_dir = QCoreApplication::applicationDirPath();

    QString cmp_release = app_dir.mid(app_dir.length()-8, 9);
    QString str_rel = "";
    qDebug() << cmp_release << endl;
    if(cmp_release.compare("/release") == 0){
        str_rel = "/release";
        qDebug() << str_rel << endl;
    }

    app_dir = app_dir.mid(0, app_dir.length() - str_rel.length());
    app_dir.replace("/","\\");
    app_dir = "\"" + app_dir + "\\" + file.fileName() + "\"";

    //app_dir = QDir::fromNativeSeparators(app_dir);

    //QString lpt_print = QDir::fromNativeSeparators("\\\\localhost\\caja001");
    QString lpt_print = "LPT1";

    //FILE * pFile;
    //char buffer[] = { (char)0x0a };
    //pFile = fopen ("c:\\test.txt", "wb");
    //fwrite (buffer , sizeof(char), sizeof(buffer), pFile);
    //fclose (pFile);
    //system(command.toStdString().c_str());

    //QProcess *myProcess = new QProcess();
    QString command = "copy /b " + app_dir
                        + " " + lpt_print + " < nul";

    /*
    QStringList arguments = QStringList();
    arguments << argument_1;
    connect(myProcess, SIGNAL(started())
        , this, SLOT(on_myProccess_started()));
    connect(myProcess, SIGNAL(finished(int, QProcess::ExitStatus))
        , this, SLOT(on_myProccess_finished(int, QProcess::ExitStatus)));
*/
    //WinExec(command.toStdString().c_str(), SW_HIDE);
    qDebug() << command << endl;
    system(command.toStdString().c_str());
    //qDebug() << command << " " << argument_1 << endl;
    //myProcess->waitForFinished(10000);
    //myProcess->start(command, QStringList(), QIODevice::ReadWrite | QIODevice::Text);
    //myProcess->waitForFinished();
    //myProcess->waitForReadyRead();

}
void Comprobante::print_msg()
{

}

void Comprobante::on_pushButton_google_maps_clicked()
{
    //QString link = "https://www.google.com/maps/d/viewer?mid=1j-mQQ_g0HqMh7Xjvul9MWKUUzDI&hl=es&ll=-16.409078690356864%2C-71.51622014410395&z=14";
    //QDesktopServices::openUrl(QUrl(link));


    qDebug() << "gg" << endl;
    //if(view_mapa){

    //}
    system("start chrome https://www.google.com/maps/d/viewer?mid=1j-mQQ_g0HqMh7Xjvul9MWKUUzDI&hl=es&ll=-16.409078690356864%2C-71.51622014410395&z=14");

    qDebug() << "uhm" << endl;

    //view_mapa->show();
    //view_mapa->raise();
    /*
    QThread* thread = new QThread;
    connect(thread, SIGNAL(started()), this, SLOT(on_thread_loadWeb()));
    connect(thread, SIGNAL(finished()), this, SLOT(on_thread_finished()));

    thread->start();
    */
}

void Comprobante::on_thread_loadWeb()
{
    /*
    view_mapa->load(tr("https://www.google.com/maps/d/viewer?mid=1j-mQQ_g0HqMh7Xjvul9MWKUUzDI&hl=es&ll=-16.409078690356864%2C-71.51622014410395&z=14"));
    view_mapa->showMaximized();
    view_mapa->raise();
    */
}
void Comprobante::loadFinished(bool b)
{
    /*
    if(b){
        view_mapa->showMaximized();
        view_mapa->raise();
    }*/

}

void Comprobante::print_comanda_aux()
{
    const int Timeout = 5 * 1000;

    if (!m_ConnectStatus)
    {
        //m_pSocket->setSocketOption(QAbstractSocket::KeepAliveOption, 1);
        m_pSocket->connectToHost(QHostAddress("192.168.1.39"), 9100, QIODevice::WriteOnly);
        //m_pSocket->connectToHost(QHostAddress("192.168.1.36"), 9100, QIODevice::WriteOnly);
    }

    if (!m_pSocket->waitForConnected(Timeout))
    {
        //sent error
        qDebug ("error in waitForConnected()");
        qDebug (qPrintable(m_pSocket->errorString()));
        m_ConnectStatus = false;
        return;
    }
    m_ConnectStatus = true;

    m_pSocket->write(blockWritten);



    //m_pSocket->write(b);

    m_pSocket->flush();

    m_pSocket->waitForBytesWritten();

    m_pSocket->disconnectFromHost();

    m_ConnectStatus = false;
}

void Comprobante::on_socket_bytesWritten(qint64 bytes)
{
    qDebug()<<bytes<<endl;
    if(bytes < blockWritten.length() && countPrintComanda <= 5){
        countPrintComanda++;
        //print_comanda_aux();
    }
}

void Comprobante::on_comboBox_familia_currentTextChanged(const QString &arg1)
{
    on_comboBox_familia_activated(arg1);
}

void Comprobante::on_pushButton_imageLogo_clicked()
{
    ConfigGeneral* w = new ConfigGeneral;
    w->setAttribute(Qt::WA_DeleteOnClose);

    w->setFondoTapiz(fondoTapiz);
    qDebug() << "comprobante image logo clicked: " << fondoTapiz << endl;

    connect(w, SIGNAL(closing()), this, SLOT(on_configGeneral_closing()));
    w->showMaximized();
}

void Comprobante::on_configGeneral_closing()
{
    ConfigGeneral* w = (ConfigGeneral*) sender();

    this->clave = w->getClave();

    disconnect(ui->spinBox_familia_filas, SIGNAL(valueChanged(int))
               , this, SLOT(on_spinBox_familia_filas_valueChanged()));
    disconnect(ui->spinBox_familia_columnas, SIGNAL(valueChanged(int))
               , this, SLOT(on_spinBox_familia_columnas_valueChanged()));
    disconnect(ui->spinBox_plato_filas, SIGNAL(valueChanged(int))
               , this, SLOT(on_spinBox_plato_filas_valueChanged()));
    disconnect(ui->spinBox_plato_columnas, SIGNAL(valueChanged(int))
               , this, SLOT(on_spinBox_plato_columnas_valueChanged()));

    ui->spinBox_familia_filas->setValue(w->getFamiliaFilas());
    ui->spinBox_familia_columnas->setValue(w->getFamiliaColumnas());
    ui->spinBox_plato_filas->setValue(w->getPlatoFilas());
    ui->spinBox_plato_columnas->setValue(w->getPlatoColumnas());

    ui->spinBox_familia_filas->setValue(ui->spinBox_familia_filas->value()+1);
    connect(ui->spinBox_familia_filas, SIGNAL(valueChanged(int))
               , this, SLOT(on_spinBox_familia_filas_valueChanged()));
    ui->spinBox_familia_filas->setValue(ui->spinBox_familia_filas->value()-1);

    ui->spinBox_plato_filas->setValue(ui->spinBox_plato_filas->value()+1);
    connect(ui->spinBox_plato_filas, SIGNAL(valueChanged(int))
               , this, SLOT(on_spinBox_plato_filas_valueChanged()));
    ui->spinBox_plato_filas->setValue(ui->spinBox_plato_filas->value()-1);

    connect(ui->spinBox_familia_columnas, SIGNAL(valueChanged(int))
               , this, SLOT(on_spinBox_familia_columnas_valueChanged()));
    connect(ui->spinBox_plato_columnas, SIGNAL(valueChanged(int))
               , this, SLOT(on_spinBox_plato_columnas_valueChanged()));

    this->fondoTapiz = w->getFondoTapiz();

    QString ss;
    ss = "QSplitter::handle{ background-color: black }\n"
         "QWidget{ background-color: " + fondoTapiz +" }\n"
         "QLineEdit{ background-color: rgb(255, 255, 255) }\n"
         "QLineEdit{ color: rgb(0, 0, 0) }\n"
         "QTableWidget{ background-color: rgb(255, 255, 255) }\n"
         "QComboBox{ background-color: rgb(255, 255, 255) }\n"
         "QSpinBox{ background-color: rgb(255, 255, 255) }\n"
         "QDoubleSpinBox{ background-color: rgb(255, 255, 255) }\n"
         "QHeaderView{ background-color: rgb(255, 255, 255) }\n"
         "QScrollBar{ background-color: rgb(255, 255, 255) }\n"
         "QPushButton{ background-color: rgb(255, 255, 255) }\n"
         "QToolButton{ background-color: rgb(255, 255, 255) }\n"
         "QMenu{ background-color: rgb(255, 255, 255) }";
    this->setStyleSheet(ss);
}

void Comprobante::on_lineEdit_pago_textEdited(const QString &arg1)
{
    qDebug() << arg1 << endl;

    double pago = arg1.toDouble();
    double total = ui->doubleSpinBox_total->value();
    double vuelto = pago - total;

    ui->doubleSpinBox_vuelto->setValue(vuelto);
}

void Comprobante::on_lineEdit_pago_textChanged(const QString &arg1)
{
    qDebug() << arg1 << endl;

    double pago = arg1.toDouble();
    double total = ui->doubleSpinBox_total->value();
    double vuelto = pago - total;

    ui->doubleSpinBox_vuelto->setValue(vuelto);
}
