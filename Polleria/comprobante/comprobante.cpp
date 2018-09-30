#include "comprobante.h"
#include "ui_comprobante.h"

#include <QtXml>
#include "SoapSunatCall.h"
#include <openssl/applink.c>

QString Comprobante::venta = "Venta";
QString Comprobante::compra = "Compra";
QString Comprobante::egresos = "Egresos";

Comprobante::Comprobante(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Comprobante)
{
    ui->setupUi(this);

    ui->dateTimeEdit_fecha_emision->setDateTime(QDateTime::currentDateTime());

    if(SYSTEM->tipo_persona.compare(Persona::master) == 0){
        ui->dateTimeEdit_fecha_emision->setReadOnly(false);
    }else{
        ui->dateTimeEdit_fecha_emision->setReadOnly(true);
    }

    QSqlQuery query;
    QString str_query = "";
    QString tipo_operacion;
    if(ui->comboBox_operacion->currentText().compare("Venta") == 0){
        tipo_operacion = VENTA;
    }
    if(ui->comboBox_operacion->currentText().compare("Compra") == 0){
        tipo_operacion = COMPRA;
    }
    if(ui->comboBox_operacion->currentText().compare("Egresos") == 0){
        tipo_operacion = EGRESOS;
    }
    str_query += QString()+"SELECT serie, numero+1 FROM comprobante";
    str_query += QString()+" WHERE operacion_item_nombre = '"+tipo_operacion+"' AND tipo_item_nombre = '"+TICKET+"'";
    str_query += QString()+" ORDER BY numero DESC LIMIT 1";
    qDebug()<<str_query<<endl;
    if(query.exec(str_query)){
        if(query.next()){
            ui->lineEdit_serie->setText(query.value(0).toString());
            ui->lineEdit_numero->setText(query.value(1).toString());
        }else{
            ui->lineEdit_serie->setText("T004");
            ui->lineEdit_numero->setText("1");
        }
    }
    ui->pushButton_send_msg->hide();
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

    ui->tableWidget->hideColumn(DESCRIPCION_PLATO);
    ui->tableWidget->hideColumn(ID);
    ui->tableWidget->hideColumn(CATEGORIA);
    ui->tableWidget->setColumnWidth(NOMBRE, 240);
    ui->tableWidget->setColumnWidth(UNIDAD, 80);    
    ui->tableWidget->setColumnWidth(CANTIDAD, 80);
    ui->tableWidget->setColumnWidth(PRECIO, 120);

    //ui->doubleSpinBox_pago->hide();
    //ui->lineEdit_pago->hide();

    //ui->lineEdit_pago->setDecimals(2);
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

    DoubleSpinBoxDelegate* delegate = new DoubleSpinBoxDelegate;
    delegate->setMinimum(0);
    delegate->setMaximum(10000000);
    delegate->setDecimals(0);
    ui->tableWidget->setItemDelegateForColumn(CANTIDAD, delegate);

    delegate = new DoubleSpinBoxDelegate;
    delegate->setMinimum(0);
    delegate->setMaximum(10000000);
    delegate->setDecimals(1);
    ui->tableWidget->setItemDelegateForColumn(PRECIO, delegate);
    /*
    LineEditNumberDelegate* numberDelegate = new LineEditNumberDelegate;

    numberDelegate->setDecimals(1);
    ui->tableWidget->setItemDelegateForColumn(PRECIO, numberDelegate);*/

    afterShow = false;

    firstShow = false;

    QRegExp regExp_ruc("[0-9]{11,11}");
    ui->lineEdit_codigo->setValidator(new QRegExpValidator(regExp_ruc));
    QRegExp regExp_serie("[BF0-9]{4,4}");
    ui->lineEdit_serie->setValidator(new QRegExpValidator(regExp_serie));
    QRegExp regExp_numero("[BF0-9]{8,8}");
    ui->lineEdit_numero->setValidator(new QRegExpValidator(regExp_numero));

    ui->lineEdit_codigo->setFocus();    

    this->installEventFilter(this);
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

void Comprobante::when_detalleModified()
{
    double total = 0.0;
    for(int i = 0; i < ui->tableWidget->rowCount(); i++) {
        double precio = ui->tableWidget->item(i, PRECIO)->text().toDouble();
        total += precio;
    }
    ui->doubleSpinBox_total->setValue(total);

    on_doubleSpinBox_pago_valueChanged(ui->doubleSpinBox_pago->value());
}

void Comprobante::delayedPopup()
{
    int val_x = ui->spinBox_familia_filas->value();
    int val_y = ui->spinBox_familia_columnas->value();
    for(int i = 0; i < val_x; i++) {
        for(int j = 0; j < val_y; j++) {
            QToolButton* tb = (QToolButton*)ui->gridLayout_familias->itemAtPosition(i, j)->widget();

        }
    }
    val_x = ui->spinBox_plato_filas->value();
    val_y = ui->spinBox_plato_columnas->value();
    for(int i = 0; i < val_x; i++) {
        for(int j = 0; j < val_y; j++) {
            QToolButton* tb = (QToolButton*)ui->gridLayout_platos->itemAtPosition(i, j)->widget();
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
            QToolButton* tb = (QToolButton*)ui->gridLayout_familias->itemAtPosition(i, j)->widget();
            //tb->setPopupMode(QPushButton::MenuButtonPopup);
        }
    }
    val_x = ui->spinBox_plato_filas->value();
    val_y = ui->spinBox_plato_columnas->value();
    for(int i = 0; i < val_x; i++) {
        for(int j = 0; j < val_y; j++) {
            QToolButton* tb = (QToolButton*)ui->gridLayout_platos->itemAtPosition(i, j)->widget();
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

            QToolButton* pb = (QToolButton*)ui->gridLayout_familias->itemAtPosition(i, j)->widget();
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
            QToolButton* pb = (QToolButton*)ui->gridLayout_platos->itemAtPosition(i, j)->widget();
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
    str_query += "SELECT nombre, x, y";
    str_query += ", font , color, background_color";
    str_query += ", imagen_dir";
    str_query += " FROM familia";
    str_query += " WHERE operacion_item_nombre = '"+ui->comboBox_operacion->currentText()+"'";
    str_query += QString() + " AND estado_item_nombre = '"+ACTIVO+"'";

    qDebug()<<str_query<<endl;
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
            QString imagen_dir = query.value(6).toString();

            int val_x = ui->spinBox_familia_filas->value();
            int val_y = ui->spinBox_familia_columnas->value();

            if (x < val_x && x >= 0 && y < val_y && y >= 0){
                ui->comboBox_familia->addItem(nombre);

                SYSTEM->set_toolButton_css(ui->gridLayout_familias, nombre
                                           , x, y, font
                                           , color, background_color);

                QToolButton* tb = (QToolButton*)ui->gridLayout_familias->itemAtPosition(x, y)->widget();
                tb->setIcon(QIcon(imagen_dir));
                tb->setIconSize(QSize(100, 100));
                tb->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
                tb->setMinimumHeight(100);
                tb->setMaximumHeight(100);
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
    str_query += "SELECT producto.nombre, producto.x, producto.y";
    str_query += ", producto.font, producto.color, producto.background_color";
    str_query += ", producto.descripcion";
    str_query += " FROM producto";
    str_query += " JOIN familia ON familia.id = producto.familia_id";
    str_query += " WHERE familia.nombre = '"+familia+"'";
    str_query += " AND familia.operacion_item_nombre = '"+ui->comboBox_operacion->currentText()+"'";
    str_query += " AND producto.operacion_item_nombre = '"+ui->comboBox_operacion->currentText()+"'";
    str_query += QString() + " AND producto.estado_item_nombre = '"+ACTIVO+"'";

    qDebug()<<str_query<<endl;
    if(query.exec(str_query)){
        ui->comboBox_plato->clear();

        while(query.next()){
            QString nombre = query.value(0).toString();
            int x = query.value(1).toInt() - 1;
            int y = query.value(2).toInt() - 1;
            QString str_font = query.value(3).toString();
            QFont font;
            font.fromString(str_font);
            QString color = query.value(4).toString();
            QString background_color = query.value(5).toString();
            QString precio = "0.0";
            QString descripcion = query.value(6).toString();

            /*
            for(int i = 0; i < ui->tableWidget->rowCount(); i++){
                if(nombre.compare(ui->tableWidget->item(i, NOMBRE)->text()) == 0){
                    platos_existentes.push_back(nombre);
                    double cant = ui->tableWidget->item(i, CANTIDAD)->text().toDouble();
                    double value_precio = precio.toDouble() * cant;
                    ui->tableWidget->item(i, PRECIO)->setText(QString().setNum(value_precio, ' ', 1));
                    ui->tableWidget->item(i, DESCRIPCION_PLATO)->setText(descripcion);
                    break;
                }
            }*/

            int val_x = ui->spinBox_plato_filas->value();
            int val_y = ui->spinBox_plato_columnas->value();

            if(x < val_x && x >= 0 && y < val_y && y >= 0) {
                ui->comboBox_plato->addItem(nombre);

                SYSTEM->set_toolButton_css(ui->gridLayout_platos, nombre
                                           , x, y, font
                                           , color, background_color);

                QToolButton* tb = (QToolButton*)ui->gridLayout_platos->itemAtPosition(x, y)->widget();
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
    {
        QSqlQuery query;
        QString str_query;
        QString tipo_operacion, tipo_documento;
        if(ui->comboBox_operacion->currentText().compare("Venta") == 0){
            tipo_operacion = VENTA;
        }
        if(ui->comboBox_operacion->currentText().compare("Compra") == 0){
            tipo_operacion = COMPRA;
        }
        if(ui->comboBox_operacion->currentText().compare("Egresos") == 0){
            tipo_operacion = EGRESOS;
        }
        if(ui->comboBox_documento->currentText().compare("Pre-Venta") == 0){
            tipo_documento = TICKET;
        }
        if(ui->comboBox_documento->currentText().compare("Boleta") == 0){
            tipo_documento = BOLETA;
        }
        if(ui->comboBox_documento->currentText().compare("Factura") == 0){
            tipo_documento = FACTURA;
        }
        str_query += QString()+"SELECT serie, numero+1 FROM comprobante";
        str_query += QString()+" WHERE operacion_item_nombre = '"+tipo_operacion+"' AND tipo_item_nombre = '"+tipo_documento+"'";
        str_query += QString()+" ORDER BY numero DESC LIMIT 1";
        qDebug()<<str_query<<endl;
        if(query.exec(str_query)){
            if(query.next()){
                int numero = query.value(1).toInt();
                int num = ui->lineEdit_numero->text().toInt();
                if(num < numero){
                    QMessageBox::warning(this, "Advertencia", "Este número ya fue usado.", "Aceptar");
                    //disconnect(ui->lineEdit_numero, SIGNAL(textChanged(QString)), this, SLOT(on_lineEdit_numero_textChanged(QString)));
                    //ui->lineEdit_numero->setText(QString().setNum(numero));
                    //connect(ui->lineEdit_numero, SIGNAL(textChanged(QString)), this, SLOT(on_lineEdit_numero_textChanged(QString)));
                }
            }else{

            }
        }
    }
    if(SYSTEM->tipo_persona.compare(Persona::master) == 0){
        AdminPass* ap = new AdminPass(this);
        //ap->setAttribute(Qt::WA_DeleteOnClose);
        ap->setModalidad(AdminPass::CONFIRM_MY_PASS);
        ap->exec();
        bool confirmado = ap->get_confirmado();

        if(!confirmado){
            return;
        }
        delete ap;
    }
    if(ui->lineEdit_nombre->text().compare("") == 0) {
        QMessageBox::warning(this, "Advertencia", "Ingrese nombre.", "Aceptar");
        return;
    }

    if(ui->tableWidget->rowCount() == 0){
        QMessageBox::warning(this, "Advertencia", "No hay detalle en la venta.", "Aceptar");
        return;
    }
    QString opc_operacion
            , opc_documento;
    if(ui->comboBox_operacion->currentText().compare("Compra") == 0)
        opc_operacion = COMPRA;
    if(ui->comboBox_operacion->currentText().compare("Venta") == 0)
        opc_operacion = VENTA;
    if(ui->comboBox_operacion->currentText().compare("Egresos") == 0)
        opc_operacion = EGRESOS;

    if(ui->comboBox_documento->currentText().compare("Boleta") == 0)
        opc_documento = BOLETA;
    if(ui->comboBox_documento->currentText().compare("Factura") == 0)
        opc_documento = FACTURA;
    if(ui->comboBox_documento->currentText().compare("Pre-Venta") == 0)
        opc_documento = TICKET;

    QString documento;

    if(opc_operacion.compare(VENTA) == 0){
        if(opc_documento.compare(BOLETA) == 0){
            documento = BOLETA;

            if(ui->doubleSpinBox_total->value() >= 700.0){
                if(ui->lineEdit_codigo->text().length() != 8){
                    QMessageBox::warning(this, "Advertencia", "Ingrese DNI con 8 dígitos", "Aceptar");
                    return;
                }
            }
            if(ui->lineEdit_codigo->text().length() != 8 && ui->lineEdit_codigo->text().length() != 0){
                QMessageBox::warning(this, "Advertencia", "No es un DNI válido con 8 dígitos.\nAl menos deje el campo en blanco.", "Aceptar");
                return;
            }
            if(ui->lineEdit_codigo->text().length() == 8){
                QString str_query;
                str_query += "SELECT * FROM persona WHERE cod = '"+ui->lineEdit_codigo->text()+"'";
                QSqlQuery query;
                if(query.exec(str_query)){
                    if(!query.next()){
                        str_query  = "INSERT INTO persona(cod, nombre, direccion, estado_item_nombre, tipo_item_nombre)";
                        str_query += "VALUES('"+ui->lineEdit_codigo->text()+"'";
                        str_query += ", '"+ui->lineEdit_nombre->text()+"'";
                        str_query += ", '"+ui->lineEdit_direccion->text()+"'";
                        str_query += ", 'Activo'";
                        str_query += ", '"+Persona::cliente_dni+"')";
                        if(!query.exec(str_query)){
                            QMessageBox::warning(this, "Advertencia", "No se puede crear el cliente.", "Aceptar");
                        }
                    }
                }
            }
            QMessageBox msgBox(this);
            msgBox.setIcon(QMessageBox::Warning);
            //msgBox.setParent();
            //msgBox.setStandardButtons(QMessageBox::Ok);
            QFont font;
            font.setFamily("MS Shell Dlg 2");
            font.setBold(false);
            font.setPointSize(11);
            msgBox.setFont(font);
            //msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Yes
            //                          | QMessageBox::Cancel);
            //msgBox.setButtonText(QMessageBox::Ok, "Servirse");
            //msgBox.setButtonText(QMessageBox::Yes, "LLevar");
            //msgBox.setButtonText(QMessageBox::Cancel, "Cancelar");
            //QPushButton* bt_boleta = new QPushButton("Boleta");
            //QPushButton* bt_factura = new QPushButton("Factura");
            QPushButton* bt_guardar = new QPushButton("Boleta(Enter)");
            QPushButton* bt_cancelar = new QPushButton("Cancelar(Esc)");
            //bt_cancelar->hide();

            //bt_boleta->setFont(font);
            //bt_factura->setFont(font);
            bt_guardar->setFont(font);
            bt_cancelar->setFont(font);
            //msgBox.addButton(bt_boleta, QMessageBox::AcceptRole);
            //msgBox.addButton(bt_factura, QMessageBox::YesRole);
            msgBox.addButton(bt_guardar, QMessageBox::NoRole);
            msgBox.addButton(bt_cancelar, QMessageBox::RejectRole);
            msgBox.setDefaultButton(bt_guardar);
            msgBox.setEscapeButton(bt_cancelar);

            msgBox.setWindowTitle("Advertencia");
            msgBox.setText("¿Cual opción eligirá usted?");



            int ret = msgBox.exec();
            qDebug()<<"ret: "<<ret<<endl;
            switch(ret){
            /*
            case 0:{
                qDebug()<<"boleta"<<endl;
                documento = BOLETA;
            }break;
            case 1:{
                qDebug()<<"factura"<<endl;
                documento = FACTURA;
            }break;*/
            case 0:{
                //qDebug()<<"guardar"<<endl;
                //documento = TICKET;
            }break;
            case 1:{
                qDebug()<<"cancel"<<endl;
                return;
            }break;
            }
        }
        if(opc_documento.compare(FACTURA) == 0){
            documento = FACTURA;

            /*
            if(tipo_persona.compare(Persona::cliente_ruc) != 0){
                QMessageBox::warning(this, "Advertencia", "Solo cliente con ruc.", "Aceptar");
                return;
            }*/
            if(ui->lineEdit_codigo->text().length() != 11){
                QMessageBox::warning(this, "Advertencia", "No es un RUC válido con 11 dígitos.\nEste campo no puede ir vacío.", "Aceptar");
                return;
            }
            QString str_query;
            str_query += "SELECT * FROM persona WHERE cod = '"+ui->lineEdit_codigo->text()+"'";
            QSqlQuery query;
            if(query.exec(str_query)){
                if(!query.next()){
                    str_query  = "INSERT INTO persona(cod, nombre, direccion, estado_item_nombre, tipo_item_nombre)";
                    str_query += "VALUES('"+ui->lineEdit_codigo->text()+"'";
                    str_query += ", '"+ui->lineEdit_nombre->text()+"'";
                    str_query += ", '"+ui->lineEdit_direccion->text()+"'";
                    str_query += ", 'Activo'";
                    str_query += ", '"+Persona::cliente_ruc+"')";
                    if(!query.exec(str_query)){
                        QMessageBox::warning(this, "Advertencia", "No se puede crear el cliente.", "Aceptar");
                    }
                }
            }
            QMessageBox msgBox(this);
            msgBox.setIcon(QMessageBox::Warning);
            //msgBox.setParent();
            //msgBox.setStandardButtons(QMessageBox::Ok);
            QFont font;
            font.setFamily("MS Shell Dlg 2");
            font.setBold(false);
            font.setPointSize(11);
            msgBox.setFont(font);
            //msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Yes
            //                          | QMessageBox::Cancel);
            //msgBox.setButtonText(QMessageBox::Ok, "Servirse");
            //msgBox.setButtonText(QMessageBox::Yes, "LLevar");
            //msgBox.setButtonText(QMessageBox::Cancel, "Cancelar");
            //QPushButton* bt_boleta = new QPushButton("Boleta");
            //QPushButton* bt_factura = new QPushButton("Factura");
            QPushButton* bt_guardar = new QPushButton("Factura(Enter)");
            QPushButton* bt_cancelar = new QPushButton("Cancelar(Esc)");
            //bt_cancelar->hide();

            //bt_boleta->setFont(font);
            //bt_factura->setFont(font);
            bt_guardar->setFont(font);
            bt_cancelar->setFont(font);
            //msgBox.addButton(bt_boleta, QMessageBox::AcceptRole);
            //msgBox.addButton(bt_factura, QMessageBox::YesRole);
            msgBox.addButton(bt_guardar, QMessageBox::NoRole);
            msgBox.addButton(bt_cancelar, QMessageBox::RejectRole);
            msgBox.setDefaultButton(bt_guardar);
            msgBox.setEscapeButton(bt_cancelar);

            msgBox.setWindowTitle("Advertencia");
            msgBox.setText("¿Cual opción eligirá usted?");



            int ret = msgBox.exec();
            qDebug()<<"ret: "<<ret<<endl;
            switch(ret){
            /*
            case 0:{
                qDebug()<<"boleta"<<endl;
                documento = BOLETA;
            }break;
            case 1:{
                qDebug()<<"factura"<<endl;
                documento = FACTURA;
            }break;*/
            case 0:{
                //qDebug()<<"guardar"<<endl;
                //documento = TICKET;
            }break;
            case 1:{
                qDebug()<<"cancel"<<endl;
                return;
            }break;
            }
        }
        if(opc_documento.compare(TICKET) == 0){
            documento = TICKET;
        }
    }else{
        documento = TICKET;
    }


    QMessageBox msgBox(this);
    msgBox.setIcon(QMessageBox::Warning);
    //msgBox.setParent();
    //msgBox.setStandardButtons(QMessageBox::Ok);
    QFont font;
    font.setFamily("MS Shell Dlg 2");
    font.setBold(false);
    font.setPointSize(11);
    msgBox.setFont(font);
    //msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Yes
    //                          | QMessageBox::Cancel);
    //msgBox.setButtonText(QMessageBox::Ok, "Servirse");
    //msgBox.setButtonText(QMessageBox::Yes, "LLevar");
    //msgBox.setButtonText(QMessageBox::Cancel, "Cancelar");
    //QPushButton* bt_boleta = new QPushButton("Boleta");
    //QPushButton* bt_factura = new QPushButton("Factura");
    QPushButton* bt_guardar = new QPushButton("Guardar(Enter)");
    QPushButton* bt_cancelar = new QPushButton("Cancelar(Esc)");
    //bt_cancelar->hide();

    //bt_boleta->setFont(font);
    //bt_factura->setFont(font);
    bt_guardar->setFont(font);
    bt_cancelar->setFont(font);
    //msgBox.addButton(bt_boleta, QMessageBox::AcceptRole);
    //msgBox.addButton(bt_factura, QMessageBox::YesRole);
    msgBox.addButton(bt_guardar, QMessageBox::NoRole);
    msgBox.addButton(bt_cancelar, QMessageBox::RejectRole);
    msgBox.setDefaultButton(bt_guardar);
    msgBox.setEscapeButton(bt_cancelar);

    msgBox.setWindowTitle("Advertencia");
    msgBox.setText("¿Cual opción eligirá usted?");



    int ret = msgBox.exec();
    qDebug()<<"ret: "<<ret<<endl;
    switch(ret){
    /*
    case 0:{
        qDebug()<<"boleta"<<endl;
        documento = BOLETA;
    }break;
    case 1:{        
        qDebug()<<"factura"<<endl;
        documento = FACTURA;
    }break;*/
    case 0:{
        //qDebug()<<"guardar"<<endl;
        //documento = TICKET;
    }break;
    case 1:{
        qDebug()<<"cancel"<<endl;
        return;
    }break;
    }

    QString str_query = "";
    QSqlQuery query;

    str_query += "SELECT IFNULL(MAX(numero), 0) + 1 FROM comprobante";
    str_query += " WHERE operacion_item_nombre = '"+ui->comboBox_operacion->currentText()+"'";
    str_query += QString()+" AND tipo_item_nombre = '"+TICKET+"'";
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
    if(ui->radioButton_llevar->isChecked())
        ui->lineEdit_nombre->setText(ui->lineEdit_nombre->text()+" ¡PARA LLEVAR!");

    QString nombre = ui->lineEdit_nombre->text();

    nombre = nombre.replace("\"", "");
    nombre = nombre.replace("'", "");

    str_query = "INSERT INTO comprobante(serie, numero, fecha_emision";
    str_query += ", anulado, pago_item_nombre, estado_item_nombre, estado_sunat";
    str_query += ", operacion_item_nombre, tipo_item_nombre, nombre, direccion, delivery";
    str_query += ", delivery_estado_item_nombre, pago, vuelto)";
    str_query += "VALUES(";
    str_query += QString() + "'" + ui->lineEdit_serie->text() + "'";
    str_query += QString() + ", '" + ui->lineEdit_numero->text() + "'";
    str_query += ", '" + ui->dateTimeEdit_fecha_emision->dateTime().toString("yyyy-MM-dd hh-mm-ss") + "'";
    //str_query += ", '2018-07-23'";
    str_query += QString() + ", '" + "1" + "'";
    str_query += QString() + ", '" + pago + "'";
    str_query += QString() + ", '" + "Activo" + "'";
    if(opc_operacion.compare(VENTA) == 0 && (opc_documento.compare(BOLETA) == 0 || opc_documento.compare(FACTURA) == 0)){
        str_query += QString() + ", '" + "Pendiente" + "'";
    }else{
        str_query += QString() + ", '" + "Activo" + "'";
    }
    str_query += QString() + ", '" + ui->comboBox_operacion->currentText() + "'";
    str_query += QString() + ", '" + documento + "'";
    str_query += ", '" + nombre + "'";
    str_query += ", '" + ui->lineEdit_direccion->text() + "'";
    str_query += ", '" + QString().setNum(ui->doubleSpinBox_delivery->value(), ' ', 2) + "'";
    if(ui->doubleSpinBox_delivery->value() != 0.0){
        str_query += ", 'Activo'";
    }else{
        str_query += ", 'Inactivo'";
    }
    str_query += ", '" + QString().setNum(ui->doubleSpinBox_pago->value(), ' ', 1) + "'";
    str_query += ", '" + QString().setNum(ui->doubleSpinBox_vuelto->value(), ' ', 2) + "'";
    str_query += ")";

    str_query += "&&END_QUERY&&";


    if(ui->lineEdit_codigo->text().compare("") != 0){        
        str_query += "INSERT INTO comprobante_has_persona(persona_cod, comprobante_id)";
        str_query += "VALUES(";
        str_query += "'" + ui->lineEdit_codigo->text() + "'";
        str_query += ", (SELECT MAX(t.id) FROM comprobante AS t)";
        str_query += ")";
        str_query += "&&END_QUERY&&";
    }else{
        str_query += "INSERT INTO comprobante_has_persona(persona_cod, comprobante_id)";
        str_query += "VALUES(";
        str_query += "'00000002'";
        str_query += ", (SELECT MAX(t.id) FROM comprobante AS t)";
        str_query += ")";
        str_query += "&&END_QUERY&&";
    }

    if(ui->lineEdit_codigo->text().length() == 11){

    }
    if(ui->lineEdit_codigo->text().length() == 8){

    }
    if(ui->lineEdit_codigo->text().length() != 11 && ui->lineEdit_codigo->text().length() != 8){

    }

    for(int i = 0; i < ui->tableWidget->rowCount(); i++){
        str_query += "INSERT INTO comprobante_has_producto(comprobante_id, producto_id";
        str_query += ", cantidad, precio)";
        str_query += "VALUES(";
        str_query += "(SELECT MAX(t.id) FROM comprobante AS t)";
        str_query += ", '" + ui->tableWidget->item(i, ID)->text() + "'";
        //str_query += ", '" + ui->tableWidget->item(i, UNIDAD)->text() + "'";
        str_query += ", '" + ui->tableWidget->item(i, CANTIDAD)->text() + "'";
        str_query += ", '" + ui->tableWidget->item(i, PRECIO)->text() + "'";
        str_query += ")";
        str_query += "&&END_QUERY&&";
    }

    SYSTEM->multiple_query(str_query);

    SYSTEM->start_transaction();
    qDebug()<<str_query<<endl;
    if(query.exec(str_query)){
        qDebug()<<"query ok"<<endl;        
        if(ui->comboBox_operacion->currentText().compare(EGRESOS) == 0){
            clear_form();
        }
        if(ui->comboBox_operacion->currentText().compare(COMPRA) == 0){
            clear_form();
        }
        if(ui->comboBox_operacion->currentText().compare(VENTA) == 0){
            if(ui->comboBox_documento->currentText().compare("Pre-Venta") == 0){
                SYSTEM->commit();
                int numero = ui->lineEdit_numero->text().toInt();
                ui->lineEdit_numero->setText(QString().setNum(numero+1));
                print_machelo();
                clear_form();
            }
            if(ui->comboBox_documento->currentText().compare(BOLETA) == 0){
                SYSTEM->commit();
                double total = 0.0;
                QVector<QString> v_cantidad, v_precio, v_nombre, v_id;
                for(int i=0; i<ui->tableWidget->rowCount(); i++){
                    QString cantidad = ui->tableWidget->item(i, CANTIDAD)->text();
                    QString precio = ui->tableWidget->item(i, PRECIO)->text();
                    QString nombre = ui->tableWidget->item(i, NOMBRE)->text();
                    QString id = ui->tableWidget->item(i, ID)->text();
                    v_cantidad.push_back(cantidad);
                    v_precio.push_back(precio);
                    v_nombre.push_back(nombre);
                    v_id.push_back(id);
                    total += precio.toDouble();
                }
                if(!SYSTEM->create_boleta(ui->lineEdit_serie->text(), ui->lineEdit_numero->text()
                                          , total, ui->lineEdit_codigo->text()
                                          , ui->lineEdit_nombre->text(), ui->lineEdit_direccion->text()
                                          , v_cantidad, v_precio
                                          , v_nombre, v_id)){
                    clear_form();
                    return;
                }

                SoapSunatCall obj;
                obj.set_str_certificatekeyfile((char *)"mycertificate.pem");
                obj.set_str_keyfile((char *)"mykey.pem");
                obj.set_str_password((char *)"4t48gWXqdd358Msx");
                obj.set_str_cafile((char *)"myca.pem");
                obj.set_str_certfile((char *)"mycert.pem");
                obj.set_str_sec_username((char *)"20498590587ELBIBAM6");
                obj.set_str_sec_password((char *)"nArputo-");

                QString str_ruc = "20498590587";
                //QString str_razon = "20498590587";
                QString str_serie = ui->lineEdit_serie->text();
                QString str_numero = ui->lineEdit_numero->text();

                string fileName_zip = QString(str_ruc+"-"+"03"+"-"+str_serie+"-"+str_numero+".ZIP").toStdString();
                string fileName_xml = QString(str_ruc+"-"+"03"+"-"+str_serie+"-"+str_numero+".XML").toStdString();


                obj.set_str_zip_xml_filename((char *)fileName_zip.c_str());
                obj.set_str_zip_xml_out_filename((char *)"res.zip");
                obj.set_str_xml_file((char *)fileName_xml.c_str());
                obj.set_str_id_signature((char *)"SignatureKG");


                if(obj.xmlDSig() < 0){
                    //QMessageBox::information(this, "Informacion", "No se puede firmar el documento","Aceptar");
                    clear_form();
                    return;
                }else{
                    qDebug() << "Se firmo el doc" << endl;
                }
                {
                    QString app_dir = QCoreApplication::applicationDirPath();

                    QString cmp_release = app_dir.mid(app_dir.length()-8, 9);
                    QString str_rel = "";
                    //qDebug() << cmp_release << endl;
                    if(cmp_release.compare("/release") == 0){
                        str_rel = "/release";
                        //qDebug() << str_rel << endl;
                    }

                    app_dir = app_dir.mid(0, app_dir.length() - str_rel.length());
                    app_dir.replace("/","\\");
                    QString str_zip_dir = "\"" + app_dir + "\\"+QString(fileName_zip.c_str())+"\"";

                    QString command = "\"c:\\Program Files\\7-Zip\\7z.exe\" a -tzip"
                                      " "+str_zip_dir+" "+QString(fileName_xml.c_str());

                    qDebug()<<command<<endl;
                    QProcess process;
                    process.start(command);
                    //QProcess::execute(command);
                    process.waitForFinished();

                    QFile file(QString(fileName_zip.c_str()));
                    while (!file.open(QIODevice::ReadOnly | QIODevice::Text)){

                        //return;
                    }
                    file.close();
                }

                QFile file(QString(fileName_xml.c_str()));
                if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
                    clear_form();
                    return;
                }

                QXmlStreamReader xml_reader(&file);

                QString str_content = xml_reader.device()->readAll().toStdString().c_str();

                //str_content = SYSTEM->replace_quotes_simple(str_content);
                QSqlQuery query;
                QString str_query = "UPDATE comprobante SET xmlContent='"+str_content+"'";
                str_query += " WHERE serie = '"+ui->lineEdit_serie->text()+"'";
                str_query += " AND numero = '"+ui->lineEdit_numero->text()+"'";
                str_query += " AND DATE(fecha_emision) = '"
                        +ui->dateTimeEdit_fecha_emision->date().toString("yyyy-MM-dd")+"'";
                qDebug()<<str_query<<endl;
                if(query.exec(str_query)){
                    qDebug()<<"si copio"<<endl;
                }
                file.close();

                {
                    QDomDocument dom;
                    QFile file(QString(fileName_xml.c_str()));
                    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
                        clear_form();
                        return;
                    }
                    if (!dom.setContent(&file)) {
                        file.close();
                        clear_form();
                        return;
                    }

                    // print out the element names of all elements that are direct children
                    // of the outermost element.
                    QDomElement root = dom.firstChildElement("Invoice");
                    QDomNodeList items = root.elementsByTagName("ext:UBLExtensions");
                    QDomNode itemnode = items.at(1);

                    items = root.elementsByTagName("DigestValue");
                    itemnode = items.at(0);

                    QString str_digestValue;
                    if(itemnode.isElement()){
                        QDomElement iteme = itemnode.toElement();
                        str_digestValue = iteme.text();
                        this->digestValue = str_digestValue;
                    }

                    qDebug()<<"DigestValue: "<<str_digestValue<<endl;
                    //QString str_send_xml = dom.toString(-1);
                    //qDebug()<<"str_send_xml: "<<str_send_xml.mid(0,200)<<endl;

                    print_machelo();
                    print_epson();
                    file.setFileName(fileName_xml.c_str());
                    file.remove();
                    file.setFileName(fileName_zip.c_str());
                    file.remove();
                    clear_form();
                }
                return;

            }
            // ////////////////////////////////////////////////////////////////////////////////////
            if(ui->comboBox_documento->currentText().compare(FACTURA) == 0){
                SYSTEM->commit();

                double total = 0.0;
                QVector<QString> v_cantidad, v_precio, v_nombre, v_id;
                for(int i=0; i<ui->tableWidget->rowCount(); i++){
                    QString cantidad = ui->tableWidget->item(i, CANTIDAD)->text();
                    QString precio = ui->tableWidget->item(i, PRECIO)->text();
                    QString nombre = ui->tableWidget->item(i, NOMBRE)->text();
                    QString id = ui->tableWidget->item(i, ID)->text();
                    v_cantidad.push_back(cantidad);
                    v_precio.push_back(precio);
                    v_nombre.push_back(nombre);
                    v_id.push_back(id);
                    total += precio.toDouble();
                }
                if(!SYSTEM->create_factura(ui->lineEdit_serie->text(), ui->lineEdit_numero->text()
                                          , total, ui->lineEdit_codigo->text()
                                          , ui->lineEdit_nombre->text(), ui->lineEdit_direccion->text()
                                          , v_cantidad, v_precio
                                          , v_nombre, v_id)){
                    clear_form();
                    return;
                }
                /*
                print_machelo();
                print_epson();
                if(true){
                    return;
                }*/

                SoapSunatCall obj;
                obj.set_str_certificatekeyfile((char *)"mycertificate.pem");
                obj.set_str_keyfile((char *)"mykey.pem");
                obj.set_str_password((char *)"4t48gWXqdd358Msx");
                obj.set_str_cafile((char *)"myca.pem");
                obj.set_str_certfile((char *)"mycert.pem");
                obj.set_str_sec_username((char *)"20498590587ELBIBAM6");
                obj.set_str_sec_password((char *)"nArputo-");

                QString str_ruc = "20498590587";
                //QString str_razon = "20498590587";
                QString str_serie = ui->lineEdit_serie->text();
                QString str_numero = ui->lineEdit_numero->text();

                string fileName_zip = QString(str_ruc+"-"+"01"+"-"+str_serie+"-"+str_numero+".ZIP").toStdString();
                string fileName_xml = QString(str_ruc+"-"+"01"+"-"+str_serie+"-"+str_numero+".XML").toStdString();


                obj.set_str_zip_xml_filename((char *)fileName_zip.c_str());
                obj.set_str_zip_xml_out_filename((char *)"res.zip");
                obj.set_str_xml_file((char *)fileName_xml.c_str());
                obj.set_str_id_signature((char *)"SignatureSP");

                if(obj.xmlDSig() < 0){
                    //QMessageBox::information(this, "Informacion", "No se puede firmar el documento","Aceptar");
                    clear_form();
                    return;
                }else{
                    qDebug() << "Se firmo el doc" << endl;
                }
                {
                    QString app_dir = QCoreApplication::applicationDirPath();

                    QString cmp_release = app_dir.mid(app_dir.length()-8, 9);
                    QString str_rel = "";
                    //qDebug() << cmp_release << endl;
                    if(cmp_release.compare("/release") == 0){
                        str_rel = "/release";
                        //qDebug() << str_rel << endl;
                    }

                    app_dir = app_dir.mid(0, app_dir.length() - str_rel.length());
                    app_dir.replace("/","\\");
                    QString str_zip_dir = "\"" + app_dir + "\\"+QString(fileName_zip.c_str())+"\"";

                    QString command = "\"c:\\Program Files\\7-Zip\\7z.exe\" a -tzip"
                                      " "+str_zip_dir+" "+QString(fileName_xml.c_str());

                    qDebug()<<command<<endl;
                    QProcess process;
                    process.start(command);
                    //QProcess::execute(command);
                    process.waitForFinished();

                    QFile file(QString(fileName_zip.c_str()));
                    while (!file.open(QIODevice::ReadOnly | QIODevice::Text)){

                        //return;
                    }
                }

                QFile file(QString(fileName_xml.c_str()));
                if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
                    clear_form();
                    return;
                }

                QXmlStreamReader xml_reader(&file);

                QString str_content = xml_reader.device()->readAll().toStdString().c_str();

                //str_content = SYSTEM->replace_quotes_simple(str_content);
                QSqlQuery query;
                QString str_query = "UPDATE comprobante SET xmlContent='"+str_content+"'";
                str_query += " WHERE serie = '"+ui->lineEdit_serie->text()+"'";
                str_query += " AND numero = '"+ui->lineEdit_numero->text()+"'";
                str_query += " AND DATE(fecha_emision) = '"
                        +ui->dateTimeEdit_fecha_emision->date().toString("yyyy-MM-dd")+"'";
                qDebug()<<str_query<<endl;
                if(query.exec(str_query)){
                    qDebug()<<"si copio"<<endl;
                }
                file.close();

                {
                    QDomDocument dom;
                    QFile file(QString(fileName_xml.c_str()));
                    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
                        clear_form();
                        return;
                    }
                    if (!dom.setContent(&file)) {
                        file.close();
                        clear_form();
                        return;
                    }

                    // print out the element names of all elements that are direct children
                    // of the outermost element.
                    QDomElement root = dom.firstChildElement("Invoice");
                    QDomNodeList items = root.elementsByTagName("ext:UBLExtensions");
                    QDomNode itemnode = items.at(1);

                    items = root.elementsByTagName("DigestValue");
                    itemnode = items.at(0);

                    QString str_digestValue;
                    if(itemnode.isElement()){
                        QDomElement iteme = itemnode.toElement();
                        str_digestValue = iteme.text();
                        this->digestValue = str_digestValue;
                    }

                    qDebug()<<"DigestValue: "<<str_digestValue<<endl;
                    //QString str_send_xml = dom.toString(-1);
                    //qDebug()<<"str_send_xml: "<<str_send_xml.mid(0,200)<<endl;

                    print_machelo();
                    print_epson();

                    file.setFileName(fileName_xml.c_str());
                    file.remove();
                    file.setFileName(fileName_zip.c_str());
                    file.remove();
                    clear_form();
                }
                return;
            }
        }        
    }else{
        SYSTEM->rollback();
        return;
    }
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
                        QToolButton* tb = (QToolButton*)ui->gridLayout_platos->itemAtPosition(ii, jj)->widget();
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

void Comprobante::on_cliente_closing()
{    
    PersonaBuscar* w = (PersonaBuscar*)sender();
    QString cod = w->get_cod();

    if(cod.compare("") == 0)
        return;

    QString tipo_persona = w->get_tipoPersona();
    QString nombre = w->get_nombre();
    QString direccion = w->get_direccion();

    this->tipo_persona = tipo_persona;
    ui->lineEdit_codigo->setText(cod);
    ui->lineEdit_nombre->setText(nombre);
    ui->lineEdit_direccion->setText(direccion);

    ui->lineEdit_direccion->setFocus();
}

void Comprobante::on_pushButton_edit_cliente_clicked()
{    
    if(ui->comboBox_operacion->currentText().compare("Venta") == 0){
        PersonaBuscar* w = new PersonaBuscar(this);
        w->setWindowFlag(Qt::Dialog);
        w->setAttribute(Qt::WA_DeleteOnClose);
        w->setWindowModality(Qt::WindowModal);

        w->setOpcPersona(Persona::opc_clientes);

        connect(w, SIGNAL(closing()), this, SLOT(on_cliente_closing()));

        w->show();
    }
    if(ui->comboBox_operacion->currentText().compare("Compra") == 0){
        PersonaBuscar* w = new PersonaBuscar(this);
        w->setWindowFlag(Qt::Dialog);
        w->setAttribute(Qt::WA_DeleteOnClose);
        w->setWindowModality(Qt::WindowModal);

        w->setOpcPersona(Persona::opc_proveedores);

        connect(w, SIGNAL(closing()), this, SLOT(on_cliente_closing()));

        w->show();
    }
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
    QSqlQuery query;/*
    str_query = "SELECT com_h_pla.precio/com_h_pla.cantidad, plato.descripcion FROM plato plato";
    str_query += " JOIN comprobante_has_plato com_h_pla ON plato.nombre = com_h_pla.plato_nombre";
    str_query += " JOIN comprobante com ON com.id = com_h_pla.comprobante_id";
    str_query += " WHERE plato.nombre = '"+arg1+"'";
    str_query += " AND com_h_pla.unidad_item_nombre = 'UND'";
    str_query += " ORDER BY com.fecha_emision DESC";
    str_query += " LIMIT 1";*/
    str_query  = "SELECT producto.precio, producto.descripcion, producto.id";
    str_query += " FROM producto";
    str_query += " WHERE producto.nombre = '"+arg1+"'";
    str_query += " AND producto.familia_id = (SELECT t.id FROM familia t WHERE t.nombre = '"+ui->comboBox_familia->currentText()+"'";
    str_query += " AND t.operacion_item_nombre = '"+ui->comboBox_operacion->currentText()+"')";
    str_query += " AND producto.operacion_item_nombre = '"+ui->comboBox_operacion->currentText()+"'";
    str_query += QString() + " AND producto.estado_item_nombre = '"+ACTIVO+"'";

    qDebug()<<str_query<<endl;
    if(query.exec(str_query)){
        QString precio = "0.0", descripcion, id;
        if(query.next()){
            precio = QString().setNum(query.value(0).toDouble(), ' ', 1);
            descripcion = query.value(1).toString();
            id = query.value(2).toString();
        }else{
            /*
            str_query  = "SELECT plato.precio, plato.descripcion";
            str_query += " FROM plato";
            str_query += " WHERE plato.nombre = '"+arg1+"'";

            qDebug()<<str_query<<endl;
            if(query.exec(str_query)){
                if(query.next()){
                    precio = QString().setNum(query.value(0).toDouble(), ' ', 1);
                    descripcion = query.value(1).toString();
                }
            }*/
        }

        disconnect(ui->tableWidget, SIGNAL(itemChanged(QTableWidgetItem*))
                   , this, SLOT(on_tableWidget_itemChanged(QTableWidgetItem*)));

        for(int i = 0; i < ui->tableWidget->rowCount(); i++){
            if(arg1.compare(ui->tableWidget->item(i, NOMBRE)->text()) == 0){
                int cant = ui->tableWidget->item(i, CANTIDAD)->text().toInt();

                ui->tableWidget->item(i,  CANTIDAD)->setText(QString().setNum(cant+1));
                double precio = ui->tableWidget->item(i, PRECIO)->text().toDouble();
                precio /= cant;
                precio *= (cant+1);
                qDebug()<<cant<<endl;
                qDebug()<<precio<<endl;
                ui->tableWidget->item(i, PRECIO)->setText(QString().setNum(precio, ' ', 1));

                double total = 0.0;
                for(int i = 0; i < ui->tableWidget->rowCount(); i++){
                    double precio = ui->tableWidget->item(i, PRECIO)->text().toDouble();
                    total += precio;
                }
                ui->doubleSpinBox_total->setValue(total);

                connect(ui->tableWidget, SIGNAL(itemChanged(QTableWidgetItem*))
                           , this, SLOT(on_tableWidget_itemChanged(QTableWidgetItem*)));
                return;
            }
        }

        int rowCount = ui->tableWidget->rowCount();
        ui->tableWidget->setRowCount(rowCount+1);

        QTableWidgetItem* item_desc = new QTableWidgetItem(arg1);
        QTableWidgetItem* item_unidad = new QTableWidgetItem("UND");
        QTableWidgetItem* item_cant = new QTableWidgetItem("1");
        QTableWidgetItem* item_precio = new QTableWidgetItem("0.0");
        QTableWidgetItem* item_descripcion = new QTableWidgetItem("");
        QTableWidgetItem* item_id = new QTableWidgetItem("");
        QTableWidgetItem* item_categoria = new QTableWidgetItem(ui->comboBox_familia->currentText());

        ui->tableWidget->setItem(rowCount, NOMBRE, item_desc);
        ui->tableWidget->item(rowCount, NOMBRE)->setFlags(Qt::ItemIsSelectable
                                                          | Qt::ItemIsEnabled
                                                          | Qt::ItemIsEditable);

        ui->tableWidget->setItem(rowCount, UNIDAD, item_unidad);
        ui->tableWidget->item(rowCount, UNIDAD)->setFlags(Qt::ItemIsSelectable
                                                     | Qt::ItemIsEnabled);

        ui->tableWidget->setItem(rowCount, CANTIDAD, item_cant);
        ui->tableWidget->item(rowCount, CANTIDAD)->setFlags(Qt::ItemIsSelectable
                                                     | Qt::ItemIsEnabled
                                                     | Qt::ItemIsEditable);

        QString precio_final = precio;
        item_precio->setText(precio_final);
        ui->tableWidget->setItem(rowCount, PRECIO, item_precio);
        if(ui->checkBox_habilitarEdicion->checkState() == Qt::Unchecked){
            ui->tableWidget->item(rowCount, PRECIO)->setFlags(Qt::ItemIsSelectable
                                                         | Qt::ItemIsEnabled);
        }else{
            ui->tableWidget->item(rowCount, PRECIO)->setFlags(Qt::ItemIsSelectable
                                                         | Qt::ItemIsEnabled
                                                         | Qt::ItemIsEditable);
        }

        QString descripcion_final = descripcion;
        item_descripcion->setText(descripcion_final);
        ui->tableWidget->setItem(rowCount, DESCRIPCION_PLATO, item_descripcion);

        item_id->setText(id);
        ui->tableWidget->setItem(rowCount, ID, item_id);

        ui->tableWidget->setItem(rowCount, CATEGORIA, item_categoria);

        connect(ui->tableWidget, SIGNAL(itemChanged(QTableWidgetItem*))
                   , this, SLOT(on_tableWidget_itemChanged(QTableWidgetItem*)));

        //SYSTEM->table_resize_to_contents(0, ui->tableWidget);

        double total = 0.0;
        for(int i = 0; i < ui->tableWidget->rowCount(); i++){
            double precio = ui->tableWidget->item(i, PRECIO)->text().toDouble();
            total += precio;
        }
        ui->doubleSpinBox_total->setValue(total);
    }

}
void Comprobante::on_spinBox_familia_filas_valueChanged(int arg1)
{    
    clean_familias();

    int x = arg1;
    int y = ui->spinBox_familia_columnas->value();
    for(int i = 0; i < x; i++){
        for(int j = 0; j < y; j++){
            QToolButton* tb = new QToolButton;
            tb->setText("____");
            tb->setPopupMode(QToolButton::DelayedPopup);

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
            QToolButton* tb = new QToolButton;
            tb->setText("____");
            tb->setPopupMode(QToolButton::DelayedPopup);

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
            //ui->splitter_mid->setSizes(QList<int>() << splitter_1_val << splitter_2_val);

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
    //str_query += " splitter_1 = " + QString().setNum(0.0);
    //str_query += ", splitter_2 = " + QString().setNum(0.0);
    str_query += " familia_filas = " + QString().setNum(ui->spinBox_familia_filas->value());
    str_query += ", familia_columnas = " + QString().setNum(ui->spinBox_familia_columnas->value());
    str_query += ", plato_filas = " + QString().setNum(ui->spinBox_plato_filas->value());
    str_query += ", plato_columnas = " + QString().setNum(ui->spinBox_plato_columnas->value());
    str_query += ", fondo_tapiz = '" + fondoTapiz + "'";

    qDebug() << "0.0" << endl;
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
            case Qt::Key_Return:{
                ui->doubleSpinBox_pago->setFocus(Qt::TabFocusReason);
                ui->doubleSpinBox_pago->selectAll();
                return true;
            }break;
            case Qt::Key_Enter:{
                ui->doubleSpinBox_pago->setFocus(Qt::TabFocusReason);
                ui->doubleSpinBox_pago->selectAll();
                return true;
            }break;
            case Qt::Key_F1:{
                ui->pushButton_efectivo->click();
                return true;
            }break;
            case Qt::Key_F2:{
                ui->pushButton_masterCard->click();
                return true;
            }break;
            case Qt::Key_F3:{
                ui->pushButton_visa->click();
                return true;
            }break;
            case Qt::Key_F4:{
                ui->pushButton_liberarColaImpresion->click();
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
        double precio = ui->tableWidget->item(i, PRECIO)->text().toDouble();
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
    QToolButton* toolB = (QToolButton*)sender();
    on_comboBox_plato_activated(toolB->text());
}

void Comprobante::on_tableWidget_itemChanged(QTableWidgetItem *item)
{
    disconnect(ui->tableWidget, SIGNAL(itemChanged(QTableWidgetItem*))
               , this, SLOT(on_tableWidget_itemChanged(QTableWidgetItem*)));

    if(item->column() == CANTIDAD){                
        QString str_query = "";
        QSqlQuery query;
        QString nombre_plato = ui->tableWidget->item(item->row(), NOMBRE)->text();
        str_query  = "SELECT producto.precio";
        str_query += " FROM producto";
        str_query += " WHERE producto.nombre = '"+nombre_plato+"'";
        str_query += " AND producto.familia_id = (SELECT t.id FROM familia t WHERE t.nombre = '"+ui->comboBox_familia->currentText()+"'";
        str_query += " AND t.operacion_item_nombre = '"+ui->comboBox_operacion->currentText()+"')";
        str_query += " AND producto.operacion_item_nombre = '"+ui->comboBox_operacion->currentText()+"'";
        str_query += QString() + " AND producto.estado_item_nombre = '"+ACTIVO+"'";

        qDebug()<<str_query<<endl;
        if(query.exec(str_query)){
            QString precio = "0.0";
            if(query.next()){
                precio = QString().setNum(query.value(0).toDouble(), ' ', 1);

                double p_total = precio.toDouble();
                double cantidad = item->text().toDouble();
                double new_p_total = p_total*cantidad;
                ui->tableWidget->item(item->row(), PRECIO)->setText(QString().setNum(new_p_total, ' ', 1));
            }else{

            }
        }

        double total = 0.0;
        for(int i = 0; i < ui->tableWidget->rowCount(); i++){
            double precio = ui->tableWidget->item(i, PRECIO)->text().toDouble();
            total += precio;
        }
        ui->doubleSpinBox_total->setValue(total);
    }
    if(item->column() == PRECIO){
        double total = 0.0;
        for(int i = 0; i < ui->tableWidget->rowCount(); i++){
            double precio = ui->tableWidget->item(i, PRECIO)->text().toDouble();
            total += precio;
        }
        qDebug()<<total<<endl;
        ui->doubleSpinBox_total->setValue(total);
    }

    connect(ui->tableWidget, SIGNAL(itemChanged(QTableWidgetItem*))
               , this, SLOT(on_tableWidget_itemChanged(QTableWidgetItem*)));
}

void Comprobante::on_doubleSpinBox_pago_valueChanged(double arg1)
{
    double pago = arg1;
    double total = ui->doubleSpinBox_total->value();
    double vuelto = pago - total - ui->doubleSpinBox_delivery->value();

    ui->doubleSpinBox_vuelto->setValue(vuelto);
}

void Comprobante::on_doubleSpinBox_total_valueChanged(double arg1)
{
    /*
    double pago = ui->doubleSpinBox_pago->value();
    double total = arg1;
    double vuelto = pago - total - ui->doubleSpinBox_delivery->value();

    qDebug()<<pago<<" "<<total<<" "<<vuelto<<endl;
    ui->doubleSpinBox_vuelto->setValue(vuelto);*/
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
    nombre.replace("\t", " ");
    nombre.replace("\n", " ");
    nombre.replace("\r", " ");

    if(nombre.length() > 40){
        int recorrido = 0;
        while(recorrido < nombre.length()) {
            centerTexts.push_back(nombre.mid(recorrido, 40));
            recorrido += 40;
        }

    }else{
        centerTexts.push_back(nombre);
    }
    //if(ui->lineEdit_direccion->text().length() > 0){
        QString direccion = "DIRECCION : " + ui->lineEdit_direccion->text();
        direccion.replace("\t", " ");
        direccion.replace("\n", " ");
        direccion.replace("\r", " ");
        if(direccion.length() > 40){
            int recorrido = 0;
            while(recorrido < direccion.length()) {
                centerTexts.push_back(direccion.mid(recorrido, 40));
                recorrido += 40;
            }

        }else{
            centerTexts.push_back(direccion);
        }
    //}

    /*
    while((nombre.length() % 40) == 0){
        QString temp = nombre.mid(0, 40);
        centerTexts.push_back(temp);
        nombre.remove(0, 40);
    }*/

    centerTexts.push_back("Fecha: " + ui->dateTimeEdit_fecha_emision->date().toString("dd/MM/yyyy"));
    centerTexts.push_back("Hora: " + ui->dateTimeEdit_fecha_emision->time().toString("hh:mm:ss"));
    centerTexts.push_back("========================================");
    centerTexts.push_back("Cant  Descripcion                Importe");
    centerTexts.push_back("----------------------------------------");

    SYSTEM->centerTexts(centerTexts, 40);

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
        QString descripcion = ui->tableWidget->item(i, NOMBRE)->text();
        QString cantidad = ui->tableWidget->item(i, CANTIDAD)->text();
        double p_total = ui->tableWidget->item(i, PRECIO)->text().toDouble();
        QString str_p_total = QString().setNum(p_total, ' ', 1);

        SYSTEM->epson_just_descripcion(descripcion, 27),
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
    totales.push_back(SYSTEM->centerText(forma_de_pago, 40));
    totales.push_back(" ");
    QString clave_wifi = "*** Clave Wifi: " + clave + " ***";
    totales.push_back(SYSTEM->centerText(clave_wifi, 40));

    SYSTEM->rightTexts(totales, 40);

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
    QString lpt_print = "LPT1";
    //QString lpt_print = "\\\\localhost\\caja001";

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
    //ShellExecuteA(0, "open", "cmd.exe", command.toStdString().c_str(), 0, SW_HIDE);

    qDebug() << command << endl;

    {
        QString app_dir = QCoreApplication::applicationDirPath();

        QString cmp_release = app_dir.mid(app_dir.length()-8, 9);
        QString str_rel = "";
        qDebug() << cmp_release << endl;
        if(cmp_release.compare("/release") == 0){
            str_rel = "/release";
            qDebug() << str_rel << endl;
        }

        app_dir = app_dir.mid(0, app_dir.length() - str_rel.length());
        app_dir = "\""+ app_dir + "\"/chp.exe cmd /c ";
        command = app_dir + command;
    }

    //system(command.toStdString().c_str());
    QProcess::execute(command);
    //qDebug() << command << " " << argument_1 << endl;
    //myProcess->waitForFinished(10000);
    //myProcess->start(command, QStringList(), QIODevice::ReadWrite | QIODevice::Text);
    //myProcess->waitForFinished();
    //myProcess->waitForReadyRead();

}
void Comprobante::print_caja_frap()
{
    QFile file("print_caja_frap.dat");
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
    //centerTexts.push_back("Maq.Regist.No: ");
    centerTexts.push_back("Aut: 0053845087990  Delivery; 38 49 29");
    centerTexts.push_back("Ticket Nro.: " + nro_ticket);
    centerTexts.push_back("Serie: 150230600349P");
    QString nombre = "NOM CLI : " + ui->lineEdit_nombre->text();
    nombre.replace("\t", " ");
    nombre.replace("\n", " ");
    nombre.replace("\r", " ");
    if(nombre.length() > 42){
        centerTexts.push_back(nombre.mid(0, 42));
        centerTexts.push_back(nombre.mid(42, nombre.length() - 42));
    }else{
        centerTexts.push_back(nombre);
    }

    centerTexts.push_back("Fecha: " + ui->dateTimeEdit_fecha_emision->date().toString("dd/MM/yyyy"));
    centerTexts.push_back("Hora: " + ui->dateTimeEdit_fecha_emision->time().toString("hh:mm:ss"));
    centerTexts.push_back("========================================");
    centerTexts.push_back("Cant: Descripcion                Importe");
    centerTexts.push_back("----------------------------------------");


    SYSTEM->centerTexts(centerTexts, 42);

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
        QString descripcion = ui->tableWidget->item(i, NOMBRE)->text();
        QString cantidad = ui->tableWidget->item(i, CANTIDAD)->text();
        double p_total = ui->tableWidget->item(i, PRECIO)->text().toDouble();
        QString str_p_total = QString().setNum(p_total, ' ', 1);

        SYSTEM->epson_just_descripcion(descripcion, 29),
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
    totales.push_back(SYSTEM->centerText(forma_de_pago, 42));
    totales.push_back(" ");
    QString clave_wifi = "Clave Wifi: " + clave + "";
    totales.push_back(SYSTEM->centerText(clave_wifi, 42));

    SYSTEM->rightTexts(totales, 42);

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
    //qDebug() << cmp_release << endl;
    if(cmp_release.compare("/release") == 0){
        str_rel = "/release";
        //qDebug() << str_rel << endl;
    }

    app_dir = app_dir.mid(0, app_dir.length() - str_rel.length());
    app_dir.replace("/","\\");
    app_dir = "\"" + app_dir + "\\" + file.fileName() + "\"";

    QString lpt_print = "LPT1";

    QString command = "copy /b " + app_dir
                        + " " + lpt_print + " < nul";

    //qDebug() << command << endl;

    {
        QString app_dir = QCoreApplication::applicationDirPath();

        QString cmp_release = app_dir.mid(app_dir.length()-8, 9);
        QString str_rel = "";
        //qDebug() << cmp_release << endl;
        if(cmp_release.compare("/release") == 0){
            str_rel = "/release";
            //qDebug() << str_rel << endl;
        }

        app_dir = app_dir.mid(0, app_dir.length() - str_rel.length());
        app_dir = "\""+ app_dir + "\"/chp.exe cmd /c ";
        command = app_dir + command;
    }
    QProcess::execute(command);
}
void Comprobante::print_comanda()
{    
    QFile file("print_comanda.dat");
    if (!file.open(QFile::ReadWrite)) {
        qDebug() << "Could not open file for writing";
        return;
    }
    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_4_8);

    QByteArray block;

    //out << (qint64)0x1B3F0A00;
    // COMANDO ESC =
    //out << (qint64)0x001B3D01;
    // COMANDO MECHANICALLY INITIALIZE PRINTER
    //out << (qint64)0x00001D3C;

    // COMANDO INITIALIZE PRINTER
    out << (qint64)0x00001B40;

    //out << (qint64)0x1B530000;

    // COMANDO SET TRANSMITION DATA
    out << (qint64)0x1B1E6101;

    out << (qint64)0x00100402;
    // COMANDO Real-time request to printer
    out << (qint64)0x00100502;
    out << (qint64)0x00101401;
    // COMANDO INITIALIZE PRINTER
    //out << (qint64)0x00001B40;
    // COMANDO CAN
    //out << (qint64)0x00000018;
    // FF
    //out << (qint64)0x1B1D0304;


    SYSTEM->star_print_text(out, SYSTEM->centerText(QString("COMANDA - COCINA"), 40));
    SYSTEM->star_line_feed(out);
    SYSTEM->star_print_text(out, SYSTEM->centerText(QString("PEDIDO"), 40));
    SYSTEM->star_line_feed(out);
    SYSTEM->star_line_feed(out);

    QVector<QString> topText;    
    QString nombre = "NOM CLI : " + ui->lineEdit_nombre->text();
    nombre.replace("\t", " ");
    nombre.replace("\n", " ");
    nombre.replace("\r", " ");

    if(nombre.length() > 40){
        int recorrido = 0;
        while(recorrido < nombre.length()) {
            SYSTEM->star_print_text(out, nombre.mid(recorrido, 40));
            SYSTEM->star_line_feed(out);
            recorrido += 40;
        }

    }else{
        SYSTEM->star_print_text(out, nombre);
        SYSTEM->star_line_feed(out);
    }

    SYSTEM->star_line_feed(out);
    SYSTEM->star_print_text(out, "FECHA: " + ui->dateTimeEdit_fecha_emision->date().toString("dd-MM-yyyy"));
    SYSTEM->star_line_feed(out);
    SYSTEM->star_line_feed(out);

    SYSTEM->star_print_text(out, "========================================");

    SYSTEM->centerTexts(topText, 40);

    for(int i = 0; i < topText.size(); i++){
        SYSTEM->star_print_text(out, topText[i]);
        SYSTEM->star_line_feed(out);
    }
    SYSTEM->star_line_feed(out);

    for(int i = 0; i < ui->tableWidget->rowCount(); i++){        
        QString descripcion = ui->tableWidget->item(i, NOMBRE)->text();
        QString cantidad = ui->tableWidget->item(i, CANTIDAD)->text();
        QString descripcion_ = ui->tableWidget->item(i, DESCRIPCION_PLATO)->text();

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
    bottomText.push_back("HORA: " + ui->dateTimeEdit_fecha_emision->time().toString("hh:mm:ss"));
    SYSTEM->centerTexts(bottomText, 40);
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

    out >> block;

    file.close();

    QString app_dir = QCoreApplication::applicationDirPath();

    QString cmp_release = app_dir.mid(app_dir.length()-8, 9);
    QString str_rel = "";
    //qDebug() << cmp_release << endl;
    if(cmp_release.compare("/release") == 0){
        str_rel = "/release";
        //qDebug() << str_rel << endl;
    }

    app_dir = app_dir.mid(0, app_dir.length() - str_rel.length());
    app_dir.replace("/","\\");
    app_dir = "\"" + app_dir + "\\" + file.fileName() + "\"";

    QString lpt_print = "\\\\localhost\\COCINA_ASA";

    QString command = "copy /b " + app_dir
                        + " " + lpt_print + "";

    //qDebug() << command << endl;

    {
        QString app_dir = QCoreApplication::applicationDirPath();

        QString cmp_release = app_dir.mid(app_dir.length()-8, 9);
        QString str_rel = "";
        //qDebug() << cmp_release << endl;
        if(cmp_release.compare("/release") == 0){
            str_rel = "/release";
            //qDebug() << str_rel << endl;
        }

        app_dir = app_dir.mid(0, app_dir.length() - str_rel.length());
        app_dir = "\""+ app_dir + "\"/chp.exe cmd /c ";
        command = app_dir + command;
    }

    QProcess::execute(command);
}
void Comprobante::print_comanda_alas()
{
    QFile file("print_comanda.dat");
    if (!file.open(QFile::WriteOnly)) {
        qDebug() << "Could not open file for writing";
        return;
    }
    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_4_8);

    QByteArray block;

    //out << (qint64)0x1B3F0A00;
    // COMANDO ESC =
    //out << (qint64)0x001B3D01;
    // COMANDO MECHANICALLY INITIALIZE PRINTER
    //out << (qint64)0x00001D3C;

    // COMANDO INITIALIZE PRINTER
    //out << (qint64)0x00001B40;

    //out << (qint64)0x1B530000;

    // COMANDO SET TRANSMITION DATA
    //out << (qint64)0x1B1E6101;

    //out << (qint64)0x00100402;
    // COMANDO Real-time request to printer
    //out << (qint64)0x00100502;
    //out << (qint64)0x00101401;
    // COMANDO INITIALIZE PRINTER
    //out << (qint64)0x00001B40;
    // COMANDO CAN
    //out << (qint64)0x00000018;
    // FF
    //out << (qint64)0x1B1D0304;


    SYSTEM->star_print_text(out, SYSTEM->centerText(QString("COMANDA - COCINA"), 40));
    SYSTEM->star_line_feed(out);
    SYSTEM->star_print_text(out, SYSTEM->centerText(QString("PEDIDO"), 40));
    SYSTEM->star_line_feed(out);
    SYSTEM->star_line_feed(out);

    QVector<QString> topText;
    QString nombre = "NOM CLI : " + ui->lineEdit_nombre->text();
    nombre.replace("\t", " ");
    nombre.replace("\n", " ");
    nombre.replace("\r", " ");

    if(nombre.length() > 40){
        int recorrido = 0;
        while(recorrido < nombre.length()) {
            SYSTEM->star_print_text(out, nombre.mid(recorrido, 40));
            SYSTEM->star_line_feed(out);
            recorrido += 40;
        }

    }else{
        SYSTEM->star_print_text(out, nombre);
        SYSTEM->star_line_feed(out);
    }

    SYSTEM->star_line_feed(out);
    SYSTEM->star_print_text(out, "FECHA: " + ui->dateTimeEdit_fecha_emision->date().toString("dd-MM-yyyy"));
    SYSTEM->star_line_feed(out);
    SYSTEM->star_line_feed(out);

    SYSTEM->star_print_text(out, "========================================");

    SYSTEM->centerTexts(topText, 40);

    for(int i = 0; i < topText.size(); i++){
        SYSTEM->star_print_text(out, topText[i]);
        SYSTEM->star_line_feed(out);
    }
    SYSTEM->star_line_feed(out);

    for(int i = 0; i < ui->tableWidget->rowCount(); i++){
        QString descripcion = ui->tableWidget->item(i, NOMBRE)->text();
        QString cantidad = ui->tableWidget->item(i, CANTIDAD)->text();
        QString descripcion_ = ui->tableWidget->item(i, DESCRIPCION_PLATO)->text();

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
    bottomText.push_back("HORA: " + ui->dateTimeEdit_fecha_emision->time().toString("hh:mm:ss"));
    SYSTEM->centerTexts(bottomText, 40);
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
    out << (qint64)0x001D5601;
    // COMANDA ASA
    //out << (qint64)0x001B6401;

    SYSTEM->star_line_feed(out);
    SYSTEM->star_line_feed(out);

    file.close();

    out >> block;

    QString app_dir = QCoreApplication::applicationDirPath();

    QString cmp_release = app_dir.mid(app_dir.length()-8, 9);
    QString str_rel = "";
    //qDebug() << cmp_release << endl;
    if(cmp_release.compare("/release") == 0){
        str_rel = "/release";
        //qDebug() << str_rel << endl;
    }

    app_dir = app_dir.mid(0, app_dir.length() - str_rel.length());
    app_dir.replace("/","\\");
    app_dir = "\"" + app_dir + "\\" + file.fileName() + "\"";

    //app_dir = QDir::fromNativeSeparators(app_dir);

    //QString lpt_print = QDir::fromNativeSeparators("\\\\localhost\\caja001");
    QString lpt_print = "\\\\localhost\\COCINA002";
    //QString lpt_print = "\\\\localhost\\caja001";

    //FILE * pFile;
    //char buffer[] = { (char)0x0a };
    //pFile = fopen ("c:\\test.txt", "wb");
    //fwrite (buffer , sizeof(char), sizeof(buffer), pFile);
    //fclose (pFile);
    //system(command.toStdString().c_str());

    //QProcess *myProcess = new QProcess();
    QString command = "copy " + app_dir
                        + " " + lpt_print + " /B";

    /*
    QStringList arguments = QStringList();
    arguments << argument_1;
    connect(myProcess, SIGNAL(started())
        , this, SLOT(on_myProccess_started()));
    connect(myProcess, SIGNAL(finished(int, QProcess::ExitStatus))
        , this, SLOT(on_myProccess_finished(int, QProcess::ExitStatus)));
*/
    //WinExec(command.toStdString().c_str(), SW_HIDE);

    //ShowWindow(GetConsoleWindow(), SW_HIDE);

    //LPCWSTR open = L"open";

    //LPCWSTR program = L"cmd.exe";

    //WinExec(command.toStdWString().c_str(), SW_SHOWNORMAL);
    //ShellExecuteW(0, open, program, command.toStdWString().c_str(), 0, SW_HIDE);

    //qDebug() << command << endl;

    {
        QString app_dir = QCoreApplication::applicationDirPath();

        QString cmp_release = app_dir.mid(app_dir.length()-8, 9);
        QString str_rel = "";
        //qDebug() << cmp_release << endl;
        if(cmp_release.compare("/release") == 0){
            str_rel = "/release";
            //qDebug() << str_rel << endl;
        }

        app_dir = app_dir.mid(0, app_dir.length() - str_rel.length());
        app_dir = "\""+ app_dir + "\"/chp.exe cmd /c ";
        command = app_dir + command;
    }

    //system(command.toStdString().c_str());
    QProcess::execute(command);

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
    centerTexts.push_back("de Manuel");
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

    centerTexts.push_back("Fecha: " + ui->dateTimeEdit_fecha_emision->date().toString("dd/MM/yyyy"));
    centerTexts.push_back("Hora: " + ui->dateTimeEdit_fecha_emision->time().toString("hh:mm:ss"));
    centerTexts.push_back("========================================");
    centerTexts.push_back("Cant: Descripcion                Importe");
    centerTexts.push_back("----------------------------------------");

    SYSTEM->centerTexts(centerTexts, 40);

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
        QString descripcion = ui->tableWidget->item(i, NOMBRE)->text();
        QString cantidad = ui->tableWidget->item(i, CANTIDAD)->text();
        double p_total = ui->tableWidget->item(i, PRECIO)->text().toDouble();
        QString str_p_total = QString().setNum(p_total, ' ', 1);

        SYSTEM->epson_just_descripcion(descripcion, 40),
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
    totales.push_back(SYSTEM->centerText(forma_de_pago, 40));
    totales.push_back(" ");
    QString clave_wifi = "*** Clave Wifi: " + clave + " ***";
    totales.push_back(SYSTEM->centerText(clave_wifi, 40));

    SYSTEM->rightTexts(totales, 40);

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
    //qDebug() << cmp_release << endl;
    if(cmp_release.compare("/release") == 0){
        str_rel = "/release";
        //qDebug() << str_rel << endl;
    }

    app_dir = app_dir.mid(0, app_dir.length() - str_rel.length());
    app_dir.replace("/","\\");
    app_dir = "\"" + app_dir + "\\" + file.fileName() + "\"";

    QString lpt_print = "LPT1";

    QString command = "copy /b " + app_dir
                        + " " + lpt_print + " < nul";

    //qDebug() << command << endl;

    {
        QString app_dir = QCoreApplication::applicationDirPath();

        QString cmp_release = app_dir.mid(app_dir.length()-8, 9);
        QString str_rel = "";
        //qDebug() << cmp_release << endl;
        if(cmp_release.compare("/release") == 0){
            str_rel = "/release";
            //qDebug() << str_rel << endl;
        }

        app_dir = app_dir.mid(0, app_dir.length() - str_rel.length());
        app_dir = "\""+ app_dir + "\"/chp.exe cmd /c ";
        command = app_dir + command;
    }
    QProcess::execute(command);
}
void Comprobante::print_msg()
{

}
void Comprobante::print_machelo()
{
    /*
    if(ui->lineEdit_serie->text().length() != 4) {
        QMessageBox::warning(this, "Advertencia", "Ingrese serie de 4 dígitos.", "Aceptar");
        return;
    }
    if(ui->lineEdit_numero->text().length() != 8) {
        QMessageBox::warning(this, "Advertencia", "Ingrese número de 8 dígitos.", "Aceptar");
        return;
    }*/
    if(ui->lineEdit_nombre->text().compare("") == 0) {
        QMessageBox::warning(this, "Advertencia", "Ingrese nombre.", "Aceptar");
        return;
    }

    if(ui->tableWidget->rowCount() == 0){
        QMessageBox::warning(this, "Advertencia", "No hay detalle en la venta.", "Aceptar");
        return;
    }

    QFile file("print_caja.dat");
    if (!file.open(QFile::WriteOnly)) {
        qDebug() << "Could not open file for writing";
        return;
    }
    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_4_8);

    SYSTEM->insertImage(out, "npedido_leal_gris.png");
    /*
    for(int j = 0; j < 4; j++){
        out<<(qint64)0x0A;
    }*/
    //qDebug()<<getchar()<<endl;
    SYSTEM->epson_lineFeed(out);


    QVector<QString> centerTexts;

    centerTexts.push_back("RUC: 20498590587");
    centerTexts.push_back("R. Social: POLLERIA EL POLLO LEAL E.I.R.L.");
    centerTexts.push_back("Direccion: PZA.PLAZA UMACHIRI NRO. 402 (PARQUE ");
    centerTexts.push_back(" UMACHIRI) AREQUIPA - AREQUIPA - MARIANO MELGAR");
    centerTexts.push_back("                                                ");
    centerTexts.push_back("                                                ");
/*
    if(str_tipo_item == str_npedido)
        centerTexts.push_back("Ticket Nota de Pedido");

    if(str_tipo_item == str_salida)
        centerTexts.push_back("Ticket Salida");
*/
    centerTexts.push_back("Serie-Nro: " + ui->lineEdit_serie->text()+"-"+ui->lineEdit_numero->text());
    centerTexts.push_back("                                                ");
    QString nombre = "NOM CLI : " + ui->lineEdit_nombre->text();
    if(nombre.length() > 48){
        int recorrido = 0;
        while(recorrido < nombre.length()) {
            centerTexts.push_back(nombre.mid(recorrido, 48));
            recorrido += 48;
        }

    }else{
        centerTexts.push_back(nombre);
    }


    centerTexts.push_back("Fecha: " + ui->dateTimeEdit_fecha_emision->date().toString("dd/MM/yyyy"));
    centerTexts.push_back("Hora: " + QTime::currentTime().toString("hh:mm:ss"));
    centerTexts.push_back("================================================");
    centerTexts.push_back("Cant      Unidad  Descripcion                   ");
    centerTexts.push_back("                                   P.Total      ");
    // Peso 13 P.Unit. 12 P.Total 13
    centerTexts.push_back("------------------------------------------------");

    SYSTEM->centerTexts(centerTexts, 48);

    //out << (qint64)0x1D4C0002;

    for(int i = 0; i < centerTexts.size(); i++){
        //SYSTEM->insert_left_spaces(centerTexts[i], 3);
        //SYSTEM->insert_right_spaces(centerTexts[i], 3);

        SYSTEM->epson_printText(out, centerTexts[i]);
        //SYSTEM->epson_lineFeed(out);
    }
    //SYSTEM->epson_lineFeed(out);
    double total = ui->doubleSpinBox_total->value(), igv = 0.18;
    for(int i = 0; i < ui->tableWidget->rowCount(); i++){
        QString categoria = ui->tableWidget->item(i, CATEGORIA)->text();
        //if(categoria.compare("bebidas", Qt::CaseInsensitive) != 0) continue;
        QString cantidad = ui->tableWidget->item(i, CANTIDAD)->text();
        QString unidad = ui->tableWidget->item(i, UNIDAD)->text();
        QString descripcion = ui->tableWidget->item(i, NOMBRE)->text();
        //QString peso = ui->tableWidget->item(i, 3)->text();
        //QString p_unit = ui->tableWidget->item(i, 4)->text();
        QString p_total = ui->tableWidget->item(i, PRECIO)->text();
        //double p_total = ui->tableWidget->item(i, 5)->text().toDouble();
        //QString str_p_total = QString().setNum(p_total, ' ', 1);

        SYSTEM->align_left(cantidad, 10);
        SYSTEM->align_left(unidad, 8);
        SYSTEM->align_left(descripcion, 55);
        //SYSTEM->align_left(peso, 13);
        //SYSTEM->align_left(p_unit, 12);
        SYSTEM->justified(p_total, 13);

        QString text = cantidad + unidad
                + descripcion + "          "
                + p_total;
        SYSTEM->epson_printText(out, text);

        centerTexts.push_back("                                                ");
    }
    QString str_st = QString().setNum(total / (1.0 + igv), ' ', 2);
    SYSTEM->justified(str_st, 6);
    QString str_i = QString().setNum(total / (1.0 + igv) * igv, ' ', 2);
    SYSTEM->justified(str_i, 6);
    QString str_t = QString().setNum(total, ' ', 2);
    SYSTEM->justified(str_t, 6);
    QString str_d = QString().setNum(ui->doubleSpinBox_delivery->value(), ' ', 1);
    SYSTEM->justified(str_d, 6);
    QString str_v = QString().setNum(ui->doubleSpinBox_vuelto->value(), ' ', 1);
    SYSTEM->justified(str_v, 6);
    SYSTEM->justified(str_t, 13);
    QString str_sub_total = "Sub-Total: " + str_st;
    QString str_igv = "IGV: " + str_i;
    QString str_total = "Total: " + str_t;
    QString str_delivery = "Delivery: " + str_d;
    QString str_vuelto = "Vuelto: " + str_v;
    QVector<QString> totales;
    totales.push_back("------------------------");
    totales.push_back(str_sub_total);
    totales.push_back(str_igv);
    totales.push_back(str_total);
    totales.push_back("------------------------");
    totales.push_back(str_delivery);
    totales.push_back(str_vuelto);
    QString forma_de_pago = QString("Forma de Pago:" + pago.toUpper());
    totales.push_back(SYSTEM->centerText(forma_de_pago, 48));
    totales.push_back(" ");
    QString clave_wifi = "*** Clave Wifi: " + clave + " ***";
    totales.push_back(SYSTEM->centerText(clave_wifi, 48));

    SYSTEM->rightTexts(totales, 48);

    SYSTEM->epson_lineFeed(out);

    for(int i = 0; i < totales.size(); i++){
        SYSTEM->epson_printText(out, totales[i]);
        //SYSTEM->epson_lineFeed(out);
    }
    //SYSTEM->epson_lineFeed(out);

    //SYSTEM->insertImage(out, "footer.png");

    SYSTEM->epson_lineFeed(out);
    //SYSTEM->epson_printText(out, SYSTEM->centerText(QString("PAGOS LLAMAR A: CRISS: 967252119")));
    //SYSTEM->epson_lineFeed(out);
    SYSTEM->epson_lineFeed(out);
    //SYSTEM->epson_printText(out, SYSTEM->centerText(QString("")));
    SYSTEM->epson_printText(out, SYSTEM->centerText(QString("\"'Cambiar a Boleta o Factura'\""), 48));

    SYSTEM->epson_lineFeed(out);
    SYSTEM->epson_lineFeed(out);
    SYSTEM->epson_lineFeed(out);
    SYSTEM->epson_lineFeed(out);
    SYSTEM->epson_lineFeed(out);


    SYSTEM->epson_paperCut(out);
    if(ui->checkBox_switch_caja->checkState() == Qt::Checked){
        SYSTEM->epson_drawer(out);
    }

    {
        bool hay_bebidas = false;
        for(int i = 0; i < ui->tableWidget->rowCount(); i++){
            QString categoria = ui->tableWidget->item(i, CATEGORIA)->text();

            if(categoria.compare("bebidas", Qt::CaseInsensitive) == 0){
                hay_bebidas = true;
                break;
            }
        }
        if(hay_bebidas){
            QVector<QString> centerTexts;

            centerTexts.push_back("BEBIDAS");
            centerTexts.push_back("================================================");
            centerTexts.push_back("Cant      Unidad  Descripcion                   ");
            centerTexts.push_back("                                   P.Total      ");
            // Peso 13 P.Unit. 12 P.Total 13
            centerTexts.push_back("------------------------------------------------");

            SYSTEM->centerTexts(centerTexts, 48);

            for(int i = 0; i < centerTexts.size(); i++){
                SYSTEM->epson_printText(out, centerTexts[i]);
            }

            double total = 0.0, igv = 0.18;
            for(int i = 0; i < ui->tableWidget->rowCount(); i++){
                QString categoria = ui->tableWidget->item(i, CATEGORIA)->text();

                if(categoria.compare("bebidas", Qt::CaseInsensitive) != 0) continue;

                QString cantidad = ui->tableWidget->item(i, CANTIDAD)->text();
                QString unidad = ui->tableWidget->item(i, UNIDAD)->text();
                QString descripcion = ui->tableWidget->item(i, NOMBRE)->text();
                QString p_total = ui->tableWidget->item(i, PRECIO)->text();

                total += p_total.toDouble();

                SYSTEM->align_left(cantidad, 10);
                SYSTEM->align_left(unidad, 8);
                SYSTEM->align_left(descripcion, 55);
                SYSTEM->justified(p_total, 13);

                QString text = cantidad + unidad
                        + descripcion + "          "
                        + p_total;
                SYSTEM->epson_printText(out, text);

                centerTexts.push_back("                                                ");
            }
            QVector<QString> totales;

            QString str_t = QString().setNum(total, ' ', 2);
            SYSTEM->justified(str_t, 6);

            QString str_total = "Total: " + str_t;

            totales.push_back("------------------------");
            totales.push_back(str_total);
            totales.push_back("------------------------");


            SYSTEM->rightTexts(totales, 48);

            SYSTEM->epson_lineFeed(out);

            for(int i = 0; i < totales.size(); i++){
                SYSTEM->epson_printText(out, totales[i]);
                //SYSTEM->epson_lineFeed(out);
            }

            SYSTEM->epson_lineFeed(out);
            SYSTEM->epson_lineFeed(out);
            SYSTEM->epson_lineFeed(out);
            SYSTEM->epson_lineFeed(out);
            SYSTEM->epson_lineFeed(out);

            SYSTEM->epson_paperCut(out);
        }
    }


    file.close();

    QString app_dir = QCoreApplication::applicationDirPath();

    QString cmp_release = app_dir.mid(app_dir.length()-8, 9);
    QString str_rel = "";
    //qDebug() << cmp_release << endl;
    if(cmp_release.compare("/release") == 0){
        str_rel = "/release";
        //qDebug() << str_rel << endl;
    }

    app_dir = app_dir.mid(0, app_dir.length() - str_rel.length());
    app_dir.replace("/","\\");
    app_dir = "\"" + app_dir + "\\" + file.fileName() + "\"";

    //app_dir = QDir::fromNativeSeparators(app_dir);

    //QString lpt_print = QDir::fromNativeSeparators("\\\\localhost\\caja001");
    //QString lpt_print = "\"\\\\localhost\\Caja 01\"";
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
    //ShellExecuteA(0, "open", "cmd.exe", command.toStdString().c_str(), 0, SW_HIDE);

    qDebug() << command << endl;

    {
        QString app_dir = QCoreApplication::applicationDirPath();

        QString cmp_release = app_dir.mid(app_dir.length()-8, 9);
        QString str_rel = "";
        //qDebug() << cmp_release << endl;
        if(cmp_release.compare("/release") == 0){
            str_rel = "/release";
            //qDebug() << str_rel << endl;
        }

        app_dir = app_dir.mid(0, app_dir.length() - str_rel.length());
        app_dir = "\""+ app_dir + "\"/chp.exe cmd /c ";
        command = app_dir + command;
    }

    QProcess::execute(command);
}
void Comprobante::print_epson()
{
    qDebug()<<"print epson"<<endl;
    /*
    if(ui->lineEdit_serie->text().length() != 4) {
        QMessageBox::warning(this, "Advertencia", "Ingrese serie de 4 dígitos.", "Aceptar");
        return;
    }
    if(ui->lineEdit_numero->text().length() != 8) {
        QMessageBox::warning(this, "Advertencia", "Ingrese número de 8 dígitos.", "Aceptar");
        return;
    }*/
    if(ui->lineEdit_nombre->text().compare("") == 0) {
        QMessageBox::warning(this, "Advertencia", "Ingrese nombre.", "Aceptar");
        return;
    }

    if(ui->tableWidget->rowCount() == 0){
        QMessageBox::warning(this, "Advertencia", "No hay detalle en la venta.", "Aceptar");
        return;
    }

    QFile file("print_epson.dat");
    if (!file.open(QFile::WriteOnly)) {
        qDebug() << "Could not open file for writing";
        return;
    }
    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_4_8);

    SYSTEM->insertImage(out, "npedido_leal_gris_epson.png");
    /*
    for(int j = 0; j < 4; j++){
        out<<(qint64)0x0A;
    }*/
    //qDebug()<<getchar()<<endl;
    SYSTEM->epson_lineFeed(out);



    qDebug()<<"print epson 2"<<endl;
    QVector<QString> centerTexts;

    centerTexts.push_back("RUC: 20498590587");
    centerTexts.push_back("R. Social: POLLERIA EL POLLO LEAL E.I.R.L.");
    centerTexts.push_back("Direccion: PZA.PLAZA UMACHIRI");
    centerTexts.push_back(" NRO. 402 (PARQUE UMACHIRI)");
    centerTexts.push_back(" AREQUIPA - AREQUIPA - MARIANO MELGAR");
    centerTexts.push_back("                                          ");
    centerTexts.push_back("                                          ");
/*
    if(str_tipo_item == str_npedido)
        centerTexts.push_back("Ticket Nota de Pedido");

    if(str_tipo_item == str_salida)
        centerTexts.push_back("Ticket Salida");
*/
    QString operacion = ui->comboBox_operacion->currentText();
    QString documento = ui->comboBox_documento->currentText();
    if(documento.compare("Boleta") == 0){
        centerTexts.push_back("BOLETA ELECTRONICA");
        centerTexts.push_back("                                          ");
        QString numero = QString().setNum(ui->lineEdit_numero->text().toInt());
        centerTexts.push_back("Serie-Nro: " + ui->lineEdit_serie->text() + "-" + numero);
        if(ui->lineEdit_codigo->text().length() == 8){
            centerTexts.push_back("DNI: "+ui->lineEdit_codigo->text());
        }
        QString nombre = "Nombre: " + ui->lineEdit_nombre->text();
        nombre.replace("\t", " ");
        nombre.replace("\n", " ");
        nombre.replace("\r", " ");

        if(nombre.length() > 42){
            int recorrido = 0;
            while(recorrido < nombre.length()) {
                if(nombre[recorrido+42-1] == ' '){
                    centerTexts.push_back(nombre.mid(recorrido, 42));
                    recorrido += 42;
                }else{
                    if(nombre[recorrido+42] == ' '){
                        centerTexts.push_back(nombre.mid(recorrido, 42));
                        recorrido += 42;
                    }else{
                        int i = recorrido+42-2, count=0;
                        while(nombre[i] != ' '){
                            i--;
                            count++;
                        }
                        if(count == 41){
                            centerTexts.push_back(nombre.mid(recorrido, 42));
                            recorrido += 42;
                        }else{
                            centerTexts.push_back(nombre.mid(recorrido, 42-count-1));
                            recorrido += 42-count-1;
                        }
                    }
                }

            }

        }else{
            centerTexts.push_back(nombre);
        }
    }
    if(documento.compare("Factura") == 0){
        centerTexts.push_back("FACTURA ELECTRONICA");
        centerTexts.push_back("                                          ");
        QString numero = QString().setNum(ui->lineEdit_numero->text().toInt());
        centerTexts.push_back("Serie-Nro: " + ui->lineEdit_serie->text() + "-" + numero);

        centerTexts.push_back("RUC: " + ui->lineEdit_codigo->text());

        QString nombre = "Razon S.: " + ui->lineEdit_nombre->text();
        nombre.replace("\t", " ");
        nombre.replace("\n", " ");
        nombre.replace("\r", " ");

        if(nombre.length() > 42){
            int recorrido = 0;
            while(recorrido < nombre.length()) {
                if(nombre[recorrido+42-1] == ' '){
                    centerTexts.push_back(nombre.mid(recorrido, 42));
                    recorrido += 42;
                }else{
                    if(nombre[recorrido+42] == ' '){
                        centerTexts.push_back(nombre.mid(recorrido, 42));
                        recorrido += 42;
                    }else{
                        int i = recorrido+42-2, count=0;
                        while(nombre[i] != ' '){
                            i--;
                            count++;
                        }
                        if(count == 41){
                            centerTexts.push_back(nombre.mid(recorrido, 42));
                            recorrido += 42;
                        }else{
                            centerTexts.push_back(nombre.mid(recorrido, 42-count-1));
                            recorrido += 42-count-1;
                        }
                    }
                }

            }

        }else{
            centerTexts.push_back(nombre);
        }
    }

    qDebug()<<"print epson 3"<<endl;
    QString direccion = "Direccion: " + ui->lineEdit_direccion->text();
    direccion.replace("\t", " ");
    direccion.replace("\n", " ");
    direccion.replace("\r", " ");

    if(direccion.length() > 42){
        int recorrido = 0;
        while(recorrido < direccion.length()) {
            if(direccion[recorrido+42-1] == ' '){
                centerTexts.push_back(direccion.mid(recorrido, 42));
                recorrido += 42;
            }else{
                if(direccion[recorrido+42] == ' '){
                    centerTexts.push_back(direccion.mid(recorrido, 42));
                    recorrido += 42;
                }else{
                    int i = recorrido+42-2, count=0;
                    while(direccion[i] != ' '){
                        i--;
                        count++;
                    }
                    if(count == 41){
                        centerTexts.push_back(direccion.mid(recorrido, 42));
                        recorrido += 42;
                    }else{
                        centerTexts.push_back(direccion.mid(recorrido, 42-count-1));
                        recorrido += 42-count-1;
                    }
                }
            }

        }

    }else{
        centerTexts.push_back(direccion);
    }

    qDebug()<<"print epson 4"<<endl;
    centerTexts.push_back("                                          ");
    centerTexts.push_back("Fecha: " + ui->dateTimeEdit_fecha_emision->date().toString("dd/MM/yyyy"));
    centerTexts.push_back("Hora: " + QTime::currentTime().toString("hh:mm:ss"));
    centerTexts.push_back("==========================================");
    centerTexts.push_back("Cant      Unidad  Descripcion             ");
    centerTexts.push_back("                             P.Total      ");
    // Peso 13 P.Unit. 12 P.Total 13
    centerTexts.push_back("------------------------------------------");

    SYSTEM->centerTexts(centerTexts, 42);

    //out << (qint64)0x1D4C0002;

    for(int i = 0; i < centerTexts.size(); i++){
        //SYSTEM->insert_left_spaces(centerTexts[i], 3);
        //SYSTEM->insert_right_spaces(centerTexts[i], 3);

        SYSTEM->epson_printText(out, centerTexts[i]);
        //SYSTEM->epson_lineFeed(out);
    }
    //SYSTEM->epson_lineFeed(out);
    double total = ui->doubleSpinBox_total->value(), igv = 0.18;
    for(int i = 0; i < ui->tableWidget->rowCount(); i++){
        QString categoria = ui->tableWidget->item(i, CATEGORIA)->text();
        //if(categoria.compare("bebidas", Qt::CaseInsensitive) != 0) continue;
        QString cantidad = ui->tableWidget->item(i, CANTIDAD)->text();
        QString unidad = ui->tableWidget->item(i, UNIDAD)->text();
        QString descripcion = ui->tableWidget->item(i, NOMBRE)->text();
        //QString peso = ui->tableWidget->item(i, 3)->text();
        //QString p_unit = ui->tableWidget->item(i, 4)->text();
        QString p_total = ui->tableWidget->item(i, PRECIO)->text();
        //double p_total = ui->tableWidget->item(i, 5)->text().toDouble();
        //QString str_p_total = QString().setNum(p_total, ' ', 1);

        SYSTEM->align_left(cantidad, 10);
        SYSTEM->align_left(unidad, 8);
        SYSTEM->align_left(descripcion, 49);
        //SYSTEM->align_left(peso, 13);
        //SYSTEM->align_left(p_unit, 12);
        SYSTEM->justified(p_total, 13);

        QString text = cantidad + unidad
                + descripcion + "    "
                + p_total;
        SYSTEM->epson_printText(out, text);

        centerTexts.push_back("                                    ");
    }
    QString str_st = QString().setNum(total / (1.0 + igv), ' ', 2);
    SYSTEM->justified(str_st, 6);
    QString str_i = QString().setNum(total / (1.0 + igv) * igv, ' ', 2);
    SYSTEM->justified(str_i, 6);
    QString str_t = QString().setNum(total, ' ', 2);
    SYSTEM->justified(str_t, 6);
    QString str_d = QString().setNum(ui->doubleSpinBox_delivery->value(), ' ', 1);
    SYSTEM->justified(str_d, 6);
    QString str_v = QString().setNum(ui->doubleSpinBox_vuelto->value(), ' ', 1);
    SYSTEM->justified(str_v, 6);
    SYSTEM->justified(str_t, 13);
    QString str_sub_total = "Sub-Total: " + str_st;
    QString str_igv = "IGV: " + str_i;
    QString str_total = "Total: " + str_t;
    QString str_delivery = "Delivery: " + str_d;
    QString str_vuelto = "Vuelto: " + str_v;
    QVector<QString> totales;
    totales.push_back("------------------------");
    totales.push_back(str_sub_total);
    totales.push_back(str_igv);
    totales.push_back(str_total);
    totales.push_back("------------------------");
    totales.push_back(str_delivery);
    totales.push_back(str_vuelto);
    QString forma_de_pago = QString("Forma de Pago:" + pago.toUpper());
    totales.push_back(SYSTEM->centerText(forma_de_pago, 42));
    totales.push_back(" ");
    QString clave_wifi = "*** Clave Wifi: " + clave + " ***";
    totales.push_back(SYSTEM->centerText(clave_wifi, 42));

    SYSTEM->rightTexts(totales, 42);

    SYSTEM->epson_lineFeed(out);

    for(int i = 0; i < totales.size(); i++){
        SYSTEM->epson_printText(out, totales[i]);
        //SYSTEM->epson_lineFeed(out);
    }
    //SYSTEM->epson_lineFeed(out);

    //SYSTEM->insertImage(out, "footer.png");

    qDebug()<<"print epson 5"<<endl;
    SYSTEM->epson_lineFeed(out);
    //SYSTEM->epson_printText(out, SYSTEM->centerText(QString("PAGOS LLAMAR A: CRISS: 967252119")));
    //SYSTEM->epson_lineFeed(out);
    SYSTEM->epson_lineFeed(out);
    //SYSTEM->epson_printText(out, SYSTEM->centerText(QString("")));
    SYSTEM->epson_printText(out, SYSTEM->centerText(digestValue, 42));
    //SYSTEM->epson_printText(out, SYSTEM->centerText(QString("\"'Cambiar a Boleta o Factura'\""), 48));

    SYSTEM->epson_lineFeed(out);
    SYSTEM->epson_lineFeed(out);
    SYSTEM->epson_lineFeed(out);
    SYSTEM->epson_lineFeed(out);
    SYSTEM->epson_lineFeed(out);
    SYSTEM->epson_lineFeed(out);
    SYSTEM->epson_lineFeed(out);


    SYSTEM->epson_paperCut(out);
    /*
    if(ui->checkBox_switch_caja->checkState() == Qt::Checked){
        SYSTEM->epson_drawer(out);
    }*/

    qDebug()<<"print epson 6"<<endl;
    {
        bool hay_bebidas = false;
        for(int i = 0; i < ui->tableWidget->rowCount(); i++){
            QString categoria = ui->tableWidget->item(i, CATEGORIA)->text();

            if(categoria.compare("bebidas", Qt::CaseInsensitive) == 0){
                hay_bebidas = true;
                break;
            }
        }
        if(hay_bebidas){
            /*
            QVector<QString> centerTexts;

            centerTexts.push_back("BEBIDAS");
            centerTexts.push_back("==========================================");
            centerTexts.push_back("Cant      Unidad  Descripcion             ");
            centerTexts.push_back("                             P.Total      ");
            // Peso 13 P.Unit. 12 P.Total 13
            centerTexts.push_back("------------------------------------------");

            SYSTEM->centerTexts(centerTexts, 48);

            for(int i = 0; i < centerTexts.size(); i++){
                SYSTEM->epson_printText(out, centerTexts[i]);
            }

            double total = 0.0, igv = 0.18;
            for(int i = 0; i < ui->tableWidget->rowCount(); i++){
                QString categoria = ui->tableWidget->item(i, CATEGORIA)->text();

                if(categoria.compare("bebidas", Qt::CaseInsensitive) != 0) continue;

                QString cantidad = ui->tableWidget->item(i, CANTIDAD)->text();
                QString unidad = ui->tableWidget->item(i, UNIDAD)->text();
                QString descripcion = ui->tableWidget->item(i, NOMBRE)->text();
                QString p_total = ui->tableWidget->item(i, PRECIO)->text();

                total += p_total.toDouble();

                SYSTEM->align_left(cantidad, 10);
                SYSTEM->align_left(unidad, 8);
                SYSTEM->align_left(descripcion, 49);
                SYSTEM->justified(p_total, 13);

                QString text = cantidad + unidad
                        + descripcion + "    "
                        + p_total;
                SYSTEM->epson_printText(out, text);

                centerTexts.push_back("                                          ");
            }

            QVector<QString> totales;

            QString str_t = QString().setNum(total, ' ', 2);
            SYSTEM->justified(str_t, 6);

            QString str_total = "Total: " + str_t;

            totales.push_back("------------------------");
            totales.push_back(str_total);
            totales.push_back("------------------------");


            SYSTEM->rightTexts(totales, 48);

            SYSTEM->epson_lineFeed(out);

            for(int i = 0; i < totales.size(); i++){
                SYSTEM->epson_printText(out, totales[i]);
                //SYSTEM->epson_lineFeed(out);
            }

            SYSTEM->epson_lineFeed(out);
            SYSTEM->epson_lineFeed(out);
            SYSTEM->epson_lineFeed(out);
            SYSTEM->epson_lineFeed(out);
            SYSTEM->epson_lineFeed(out);

            SYSTEM->epson_paperCut(out);
            */
        }
    }

qDebug()<<"print epson 7"<<endl;
    file.close();

    QString app_dir = QCoreApplication::applicationDirPath();

    QString cmp_release = app_dir.mid(app_dir.length()-8, 9);
    QString str_rel = "";
    //qDebug() << cmp_release << endl;
    if(cmp_release.compare("/release") == 0){
        str_rel = "/release";
        //qDebug() << str_rel << endl;
    }

    app_dir = app_dir.mid(0, app_dir.length() - str_rel.length());
    app_dir.replace("/","\\");
    app_dir = "\"" + app_dir + "\\" + file.fileName() + "\"";

    //app_dir = QDir::fromNativeSeparators(app_dir);

    //QString lpt_print = QDir::fromNativeSeparators("\\\\localhost\\caja001");
    QString lpt_print = "\"\\\\localhost\\EPSON TM-T88V Receipt\"";
    //QString lpt_print = "\\\\localhost\\caja001";

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
    //ShellExecuteA(0, "open", "cmd.exe", command.toStdString().c_str(), 0, SW_HIDE);

    qDebug() << command << endl;

    {
        QString app_dir = QCoreApplication::applicationDirPath();

        QString cmp_release = app_dir.mid(app_dir.length()-8, 9);
        QString str_rel = "";
        //qDebug() << cmp_release << endl;
        if(cmp_release.compare("/release") == 0){
            str_rel = "/release";
            //qDebug() << str_rel << endl;
        }

        app_dir = app_dir.mid(0, app_dir.length() - str_rel.length());
        app_dir = "\""+ app_dir + "\"/chp.exe cmd /c ";
        command = app_dir + command;
    }

    QProcess::execute(command);
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
    if(ui->comboBox_operacion->currentText().compare(COMPRA) == 0){
        //return;
    }
    ConfigGeneral* w = new ConfigGeneral;
    w->setAttribute(Qt::WA_DeleteOnClose);

    w->setFondoTapiz(fondoTapiz);
    w->setOperacion(ui->comboBox_operacion->currentText());
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

    when_detalleModified();
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

void Comprobante::on_pushButton_liberarColaImpresion_clicked()
{
    QMessageBox msgBox(this);
    msgBox.setIcon(QMessageBox::Warning);
    //msgBox.setParent();
    //msgBox.setStandardButtons(QMessageBox::Ok);
    QFont font;
    font.setFamily("MS Shell Dlg 2");
    font.setBold(false);
    font.setPointSize(11);
    msgBox.setFont(font);
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setButtonText(QMessageBox::Ok, "Aceptar");
    msgBox.setButtonText(QMessageBox::Cancel, "Cancelar");
    msgBox.setWindowTitle("Advertencia");
    msgBox.setText("¿Esta seguro de reiniciar el servicio de impresión.?");

    int ret = msgBox.exec();
    qDebug()<<"ret: "<<ret<<endl;
    switch(ret){
    case QMessageBox::Ok:{

    }break;
    case QMessageBox::Cancel:{
        return;
    }break;
    }

    QProcess *myProcess = new QProcess(this);

    QString fileName = "LiberarColaImpresion.lnk";
    QString app_dir = QCoreApplication::applicationDirPath();

    QString cmp_release = app_dir.mid(app_dir.length()-8, 9);
    QString str_rel = "";
    //qDebug() << cmp_release << endl;
    if(cmp_release.compare("/release") == 0){
        str_rel = "/release";
        //qDebug() << str_rel << endl;
    }

    app_dir = app_dir.mid(0, app_dir.length() - str_rel.length());
    app_dir.replace("/","\\");

    app_dir = "\"" + app_dir + "\"\\" + fileName + "";

    //QString command = ""+ save_app_dir + "/chp.exe cmd.exe /C " + app_dir;
    //QString command = "cmd.exe /C " + app_dir;
    QString command = "cmd.exe /b cmd.exe /c " + app_dir;


    qDebug()<<command<<endl;

    connect(myProcess, SIGNAL(finished(int, QProcess::ExitStatus))
        , this, SLOT(on_myProccessLiberarColaImpresion_finished(int, QProcess::ExitStatus)));

    myProcess->start(command);
}

void Comprobante::on_checkBox_habilitarEdicion_stateChanged(int arg1)
{
    /*
    QTableWidgetItem* item = ui->tableWidget->currentItem();
    if(!item){
        return;
    }*/

    if(arg1 == Qt::Unchecked){
        for(int i=0; i<ui->tableWidget->rowCount(); i++){
            ui->tableWidget->item(i, PRECIO)->setFlags(Qt::ItemIsEnabled
                                                       |Qt::ItemIsSelectable);
        }
        return;
    }else{
        if(SYSTEM->tipo_persona.compare(Persona::cajero) == 0){
            AdminPass* ap = new AdminPass(this);
            ap->setModalidad(AdminPass::CONFIRM_MY_PASS);
            //ap->setAttribute(Qt::WA_DeleteOnClose);
            ap->exec();
            bool confirmado = ap->get_confirmado();

            qDebug()<<confirmado<<endl;
            if(confirmado){
                for(int i=0; i<ui->tableWidget->rowCount(); i++){
                    ui->tableWidget->item(i, PRECIO)->setFlags(Qt::ItemIsEnabled
                                                               |Qt::ItemIsSelectable
                                                               |Qt::ItemIsEditable);
                }
                delete ap;
            }
        }else{
            for(int i=0; i<ui->tableWidget->rowCount(); i++){
                ui->tableWidget->item(i, PRECIO)->setFlags(Qt::ItemIsEnabled
                                                           |Qt::ItemIsSelectable
                                                           |Qt::ItemIsEditable);
            }
        }
    }
}

void Comprobante::on_comboBox_operacion_currentTextChanged(const QString &arg1)
{
    if(arg1.compare("Compra") == 0){
        ui->comboBox_documento->clear();
        QSqlQuery query;
        QString str_query = "";
        str_query += QString()+"SELECT serie, numero+1 FROM comprobante";
        str_query += QString()+" WHERE operacion_item_nombre = '"+COMPRA"' AND tipo_item_nombre = '"+TICKET+"'";
        str_query += QString()+" ORDER BY numero DESC LIMIT 1";
        qDebug()<<str_query<<endl;
        if(query.exec(str_query)){
            if(query.next()){
                ui->lineEdit_serie->setText(query.value(0).toString());
                ui->lineEdit_numero->setText(query.value(1).toString());
            }else{
                ui->lineEdit_serie->setText("T004");
                ui->lineEdit_numero->setText("1");
            }
        }
    }
    if(arg1.compare("Venta") == 0){
        ui->comboBox_documento->addItem("Boleta");
        ui->comboBox_documento->addItem("Factura");
        ui->comboBox_documento->addItem("Pre-Venta");
        ui->comboBox_documento->setCurrentText("Pre-Venta");
        QSqlQuery query;
        QString str_query = "";
        str_query += QString()+"SELECT serie, numero+1 FROM comprobante";
        str_query += QString()+" WHERE operacion_item_nombre = '"+VENTA"' AND tipo_item_nombre = '"+TICKET+"'";
        str_query += QString()+" ORDER BY numero DESC LIMIT 1";
        qDebug()<<str_query<<endl;
        if(query.exec(str_query)){
            if(query.next()){
                ui->lineEdit_serie->setText(query.value(0).toString());
                ui->lineEdit_numero->setText(query.value(1).toString());
            }else{
                ui->lineEdit_serie->setText("T004");
                ui->lineEdit_numero->setText("1");
            }
        }
    }
    if(arg1.compare("Egresos") == 0){
        ui->comboBox_documento->clear();

        QSqlQuery query;
        QString str_query = "";
        str_query += QString()+"SELECT serie, numero+1 FROM comprobante";
        str_query += QString()+" WHERE operacion_item_nombre = '"+EGRESOS"' AND tipo_item_nombre = '"+TICKET+"'";
        str_query += QString()+" ORDER BY numero DESC LIMIT 1";
        qDebug()<<str_query<<endl;
        if(query.exec(str_query)){
            if(query.next()){
                ui->lineEdit_serie->setText(query.value(0).toString());
                ui->lineEdit_numero->setText(query.value(1).toString());
            }else{
                ui->lineEdit_serie->setText("T004");
                ui->lineEdit_numero->setText("1");
            }
        }
    }
    ui->lineEdit_codigo->clear();
    ui->lineEdit_direccion->clear();
    ui->lineEdit_nombre->clear();
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);

    ui->doubleSpinBox_pago->setValue(0.0);
    ui->doubleSpinBox_total->setValue(0.0);
    ui->doubleSpinBox_vuelto->setValue(0.0);

    disconnect(ui->spinBox_familia_filas, SIGNAL(valueChanged(int))
               , this, SLOT(on_spinBox_familia_filas_valueChanged()));
    disconnect(ui->spinBox_familia_columnas, SIGNAL(valueChanged(int))
               , this, SLOT(on_spinBox_familia_columnas_valueChanged()));

    disconnect(ui->spinBox_plato_filas, SIGNAL(valueChanged(int))
               , this, SLOT(on_spinBox_plato_filas_valueChanged()));
    disconnect(ui->spinBox_plato_columnas, SIGNAL(valueChanged(int))
               , this, SLOT(on_spinBox_plato_columnas_valueChanged()));

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

}

void Comprobante::on_pushButton_pagar_clicked()
{
    Pagar* w = new Pagar(this);
    w->setWindowFlag(Qt::Dialog);
    w->setWindowModality(Qt::WindowModal);
    w->setAttribute(Qt::WA_DeleteOnClose);

    connect(w, SIGNAL(closing()), this, SLOT(on_pagar_closing()));
    w->show();
}

void Comprobante::on_pagar_closing()
{
    Pagar* w = (Pagar*)sender();
    ui->doubleSpinBox_pago->setValue(w->getTotal());

    double pago = ui->doubleSpinBox_pago->value();
    double total = ui->doubleSpinBox_total->value();
    double vuelto = pago - total - ui->doubleSpinBox_delivery->value();

    ui->doubleSpinBox_vuelto->setValue(vuelto);
}

void Comprobante::on_doubleSpinBox_delivery_valueChanged(double arg1)
{
    double pago = ui->doubleSpinBox_pago->value();
    double total = ui->doubleSpinBox_total->value();
    double vuelto = pago - total - arg1;

    ui->doubleSpinBox_vuelto->setValue(vuelto);
}

void Comprobante::on_checkBox_switch_caja_clicked()
{

}

void Comprobante::on_checkBox_switch_caja_toggled(bool checked)
{
    if(ui->checkBox_switch_caja->checkState() == Qt::Checked){
        AdminPass* ap = new AdminPass(this);
        //ap->setAttribute(Qt::WA_DeleteOnClose);
        ap->setModalidad(AdminPass::CONFIRM_MY_PASS);
        ap->exec();
        bool confirmado = ap->get_confirmado();

        if(!confirmado){
            disconnect(ui->checkBox_switch_caja, SIGNAL(toggled(bool)), this, SLOT(on_checkBox_switch_caja_toggled(bool)));
            ui->checkBox_switch_caja->setCheckState(Qt::Unchecked);
            connect(ui->checkBox_switch_caja, SIGNAL(toggled(bool)), this, SLOT(on_checkBox_switch_caja_toggled(bool)));
            return;
        }
        delete ap;
    }
}

void Comprobante::on_pushButton_clicked()
{
    QString password="123456";
    QFile pkcs("C:\\Users\\lorda\\Desktop\\new.pfx");
    pkcs.open(QFile::ReadOnly);
    QSslKey key;
    QSslCertificate cert;
    QList<QSslCertificate> imported_certs;
    static bool import=QSslCertificate::importPkcs12(&pkcs,&key,&cert,&imported_certs,QByteArray::fromStdString(password.toStdString()));

    pkcs.close();
    qDebug()<<import<<endl;;

    /*
    bool loadPfxCertifcate(QString certFilename, QString passphrase) {

        QFile certFile(certFilename);
        certFile.open(QFile::ReadOnly);
        QSslCertificate certificate;
        QSslKey key;
        QList<QSslCertificate> importedCerts;

        bool imported = QSslCertificate::importPkcs12(&certFile, &key, &certificate, &importedCerts, QByteArray::fromStdString(passphrase.toStdString()));

        certFile.close();

        qDebug() << "Imported cert:" << imported;

        if (imported) {
            QSslConfiguration sslConfig = QSslConfiguration::defaultConfiguration();
            QList<QSslCertificate> certs = sslConfig.caCertificates();
            QList<QSslCertificate> localCerts = sslConfig.localCertificateChain();
            localCerts.append(certificate);
            certs.append(importedCerts);

            sslConfig.setLocalCertificateChain(localCerts);
            sslConfig.setCaCertificates(certs);
            sslConfig.setPrivateKey(key);
            QSslConfiguration::setDefaultConfiguration(sslConfig);
        }

        return imported;
    }*/
}


void Comprobante::on_comboBox_documento_currentTextChanged(const QString &arg1)
{
    QSqlQuery query;
    QString str_query;
    QString tipo_operacion, tipo_documento;
    if(ui->comboBox_operacion->currentText().compare("Venta") == 0){
        tipo_operacion = VENTA;
    }
    if(ui->comboBox_operacion->currentText().compare("Compra") == 0){
        tipo_operacion = COMPRA;
    }
    if(ui->comboBox_operacion->currentText().compare("Egresos") == 0){
        tipo_operacion = EGRESOS;
    }
    if(ui->comboBox_documento->currentText().compare("Pre-Venta") == 0){
        tipo_documento = TICKET;
    }
    if(ui->comboBox_documento->currentText().compare("Boleta") == 0){
        tipo_documento = BOLETA;
    }
    if(ui->comboBox_documento->currentText().compare("Factura") == 0){
        tipo_documento = FACTURA;
    }
    str_query += QString()+"SELECT serie, numero+1 FROM comprobante";
    str_query += QString()+" WHERE operacion_item_nombre = '"+tipo_operacion+"' AND tipo_item_nombre = '"+tipo_documento+"'";
    str_query += QString()+" ORDER BY numero DESC LIMIT 1";
    qDebug()<<str_query<<endl;
    if(query.exec(str_query)){
        if(query.next()){
            ui->lineEdit_serie->setText(query.value(0).toString());
            ui->lineEdit_numero->setText(query.value(1).toString());
        }else{
            ui->lineEdit_serie->setText("T004");
            ui->lineEdit_numero->setText("1");
        }
    }
}

void Comprobante::on_lineEdit_numero_textChanged(const QString &arg1)
{
    QSqlQuery query;
    QString str_query;
    QString tipo_operacion, tipo_documento;
    if(ui->comboBox_operacion->currentText().compare("Venta") == 0){
        tipo_operacion = VENTA;
    }
    if(ui->comboBox_operacion->currentText().compare("Compra") == 0){
        tipo_operacion = COMPRA;
    }
    if(ui->comboBox_operacion->currentText().compare("Egresos") == 0){
        tipo_operacion = EGRESOS;
    }
    if(ui->comboBox_documento->currentText().compare("Pre-Venta") == 0){
        tipo_documento = TICKET;
    }
    if(ui->comboBox_documento->currentText().compare("Boleta") == 0){
        tipo_documento = BOLETA;
    }
    if(ui->comboBox_documento->currentText().compare("Factura") == 0){
        tipo_documento = FACTURA;
    }
    str_query += QString()+"SELECT serie, numero+1 FROM comprobante";
    str_query += QString()+" WHERE operacion_item_nombre = '"+tipo_operacion+"' AND tipo_item_nombre = '"+tipo_documento+"'";
    str_query += QString()+" ORDER BY numero DESC LIMIT 1";
    qDebug()<<str_query<<endl;
    if(query.exec(str_query)){
        if(query.next()){
            int numero = query.value(1).toInt();
            int num = arg1.toInt();
            if(num > (numero+1)){
                disconnect(ui->lineEdit_numero, SIGNAL(textChanged(QString)), this, SLOT(on_lineEdit_numero_textChanged(QString)));
                ui->lineEdit_numero->setText(QString().setNum(numero));
                connect(ui->lineEdit_numero, SIGNAL(textChanged(QString)), this, SLOT(on_lineEdit_numero_textChanged(QString)));
            }
        }else{

        }
    }
}
void Comprobante::clear_form()
{
    int numero = ui->lineEdit_numero->text().toInt();
    ui->lineEdit_numero->setText(QString().setNum(numero+1));

    ui->lineEdit_codigo->clear();
    ui->lineEdit_direccion->clear();
    ui->lineEdit_nombre->clear();
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);

    ui->doubleSpinBox_pago->setValue(0.0);
    ui->doubleSpinBox_total->setValue(0.0);
    ui->doubleSpinBox_vuelto->setValue(0.0);
    ui->radioButton_mesa->setChecked(true);

    tipo_persona = Persona::cliente_dni;
}
