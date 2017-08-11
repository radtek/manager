#include "comprobante.h"
#include "ui_comprobante.h"
#include "starmicronics/StarIOPort.h"
#include <cwchar>
#include <comdef.h>

Comprobante::Comprobante(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Comprobante)
{
    ui->setupUi(this);    

    //connect(ui->splitter_vertical, SIGNAL(splitterMoved(int,int)), this, SLOT(on_splitter_moved(int, int)));
    //connect(ui->splitter_horizontal, SIGNAL(splitterMoved(int,int)), this, SLOT(on_splitter_moved(int, int)));


    m_pSocket   = new QTcpSocket();
    //m_pSocket->setSocketOption(64, 1);
    m_pSocket->connectToHost(QHostAddress("192.168.1.36"), 9100, QIODevice::WriteOnly);

    m_ConnectStatus = true;

    //ui->tableWidget->clear();
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);

    LineEditNumberDelegate* delegate = new LineEditNumberDelegate;
    delegate->setDecimals(0);
    delegate->setMinimumWidth(100);
    delegate->setMaximumWidth(100);
    delegate->setMinimumHeight(100);
    delegate->setMaximumHeight(100);
    ui->tableWidget->setItemDelegateForColumn(0, delegate);
    delegate = new LineEditNumberDelegate;
    delegate->setDecimals(1);
    delegate->setMinimumWidth(100);
    delegate->setMaximumWidth(100);
    delegate->setMinimumHeight(100);
    delegate->setMaximumHeight(100);
    ui->tableWidget->setItemDelegateForColumn(2, delegate);

    afterShow = false;

    firstShow = false;

    QRegExp regExp_ruc("[0-9]{11,11}");
    ui->lineEdit_codigo->setValidator(new QRegExpValidator(regExp_ruc));

    ui->lineEdit_codigo->setFocus();

    ui->spinBox_familia_filas->setValue(3);
    ui->spinBox_familia_filas->setValue(2);
    //ui->spinBox_plato_filas->setValue(7);

    this->installEventFilter(this);
    ui->comboBox_familia->installEventFilter(this);

    select_familias();
    ui->comboBox_familia->setCurrentIndex(0);
    select_platos(ui->comboBox_familia->currentText());
}

Comprobante::~Comprobante()
{
    qDebug()<<"delete comprobante"<<endl;
    //delete ui;
    //delete m_pSocket;
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

            ui->comboBox_familia->addItem(nombre);
            SYSTEM->set_toolButton_css(ui->gridLayout_familias, nombre
                                       , x, y, font
                                       , color, background_color);
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
    str_query += " FROM plato";
    str_query += " JOIN familia ON familia.item_nombre = plato.familia_item_nombre";
    str_query += " WHERE familia.item_nombre = '"+familia+"'";

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

            ui->comboBox_plato->addItem(nombre);            
            SYSTEM->set_toolButton_css(ui->gridLayout_platos, nombre
                                       , x, y, font
                                       , color, background_color);
        }
        if(ui->comboBox_plato->count() > 0){
            //ui->comboBox_plato->setCurrentText(platito);
            //if(ui->comboBox_plato->currentText().compare("") == 0){
                ui->comboBox_plato->setCurrentIndex(0);
            //}
        }else{
            ui->comboBox_plato->clear();
            ui->comboBox_plato->clearEditText();
        }
    }
}

void Comprobante::on_familia_closing()
{
    qDebug()<<"familia_closing"<<endl;
    select_familias();
}
void Comprobante::on_plato_closing()
{
    qDebug()<<"plato_closing"<<endl;
    select_platos(ui->comboBox_familia->currentText());
}
void Comprobante::on_familia_fontAct_triggered(QAction* act)
{
    if(act->text().compare("&Fuente") == 0){        
        bool ok;
        QFont font = QFontDialog::getFont(
                      &ok, QFont("MS Shell Dlg 2", 8, -1, false), this);
        if (ok) {
            QToolButton* tButton = (QToolButton*)sender();
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

        QToolButton* tButton = (QToolButton*)sender();

        QString bg_color = SYSTEM->extract_background_color(tButton->styleSheet());
        QString str_color = color.name();

        QString ss = "QToolButton{ color: "+str_color+"; background-color: "+bg_color+"; }";
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

        QToolButton* tButton = (QToolButton*)sender();

        QString bg_color = color.name();
        QString str_color = SYSTEM->extract_color(tButton->styleSheet());

        QString ss = "QToolButton{ color: "+str_color+"; background-color: "+bg_color+"; }";
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
void Comprobante::on_plato_fontAct_triggered(QAction* act)
{
    if(act->text().compare("&Fuente") == 0){
        bool ok;
        QFont font = QFontDialog::getFont(
                      &ok, QFont("MS Shell Dlg 2", 8, -1, false), this);
        if (ok) {
            QToolButton* tButton = (QToolButton*)sender();
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

        QToolButton* tButton = (QToolButton*)sender();

        QString bg_color = SYSTEM->extract_background_color(tButton->styleSheet());
        QString str_color = color.name();

        QString ss = "QToolButton{ color: "+str_color+"; background-color: "+bg_color+"; }";
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

        QToolButton* tButton = (QToolButton*)sender();

        QString bg_color = color.name();
        QString str_color = SYSTEM->extract_color(tButton->styleSheet());

        QString ss = "QToolButton{ color: "+str_color+"; background-color: "+bg_color+"; }";
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
void Comprobante::on_pushButton_edit_familia_clicked()
{
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

    connect(familia, SIGNAL(closing()), this, SLOT(on_familia_closing()));

    familia->show();
}

void Comprobante::on_pushButton_edit_plato_clicked()
{
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

    plato->set_familia(ui->comboBox_familia->currentText());

    connect(plato, SIGNAL(closing()), this, SLOT(on_plato_closing()));

    plato->show();
}

void Comprobante::on_comboBox_familia_activated(const QString &arg1)
{
    ui->spinBox_plato_filas->setValue(ui->spinBox_plato_filas->value()+1);
    ui->spinBox_plato_filas->setValue(ui->spinBox_plato_filas->value()-1);
    select_platos(arg1);
}

void Comprobante::on_comboBox_plato_activated(const QString &arg1)
{    
    QString str_query = "";
    QSqlQuery query;
    str_query += "SELECT precio FROM plato";
    str_query += " WHERE item_nombre = '"+arg1+"'";
    if(query.exec(str_query)){
        if(query.next()){
            disconnect(ui->tableWidget, SIGNAL(itemChanged(QTableWidgetItem*))
                       , this, SLOT(on_tableWidget_itemChanged(QTableWidgetItem*)));

            int rowCount = ui->tableWidget->rowCount();
            ui->tableWidget->setRowCount(rowCount+1);

            QTableWidgetItem* item_cant = new QTableWidgetItem("1");
            QTableWidgetItem* item_desc = new QTableWidgetItem(arg1);
            QTableWidgetItem* item_precio = new QTableWidgetItem("");
            ui->tableWidget->setItem(rowCount, 0, item_cant);
            ui->tableWidget->setItem(rowCount, 1, item_desc);

            ui->tableWidget->item(rowCount, 1)->setFlags(Qt::ItemIsSelectable
                                                         | Qt::ItemIsEnabled);
            QString precio = query.value(0).toString();
            item_precio->setText(precio);
            ui->tableWidget->setItem(rowCount, 2, item_precio);
            ui->tableWidget->item(rowCount, 2)->setFlags(Qt::ItemIsSelectable
                                                         | Qt::ItemIsEnabled);

            connect(ui->tableWidget, SIGNAL(itemChanged(QTableWidgetItem*))
                       , this, SLOT(on_tableWidget_itemChanged(QTableWidgetItem*)));

            SYSTEM->table_resize_to_contents(0, ui->tableWidget);
            double total = 0.0;
            for(int i = 0; i < ui->tableWidget->rowCount(); i++){
                total += ui->tableWidget->item(i, 2)->text().toDouble();
            }
            ui->doubleSpinBox_total->setValue(total);
        }
    }

}
void Comprobante::on_spinBox_familia_filas_valueChanged(int arg1)
{
    for(int i = 0; i < 99; i++){
        for(int j = 0; j < 99; j++){
            SYSTEM->remove(ui->gridLayout_familias, i, j, true);
        }
    }
    int x = arg1;
    int y = ui->spinBox_familia_columnas->value();
    for(int i = 0; i < x; i++){
        for(int j = 0; j < y; j++){
            QToolButton* tb = new QToolButton;
            tb->setText("____");
            tb->setPopupMode(QToolButton::MenuButtonPopup);
            QAction* fontAct = new QAction(tr("&Fuente"), this);
            //fontAct->setCheckable(true);
            fontAct->setShortcut(tr("Alt+f"));
            fontAct->setStatusTip(tr("Cambiar fuente..."));
            tb->addAction(fontAct);            
            connect(tb, SIGNAL(triggered(QAction*)), this, SLOT(on_familia_fontAct_triggered(QAction*)));

            QAction* colorAct = new QAction(tr("&Color"), this);
            //fontAct->setCheckable(true);
            colorAct->setShortcut(tr("Alt+c"));
            colorAct->setStatusTip(tr("Cambiar color..."));
            tb->addAction(colorAct);
            connect(tb, SIGNAL(triggered(QAction*)), this, SLOT(on_familia_colorAct_triggered(QAction*)));

            QAction* colorFondoAct = new QAction(tr("&Color de Fondo"), this);
            //fontAct->setCheckable(true);
            colorFondoAct->setShortcut(tr("Alt+b"));
            colorFondoAct->setStatusTip(tr("Cambiar color de fondo..."));
            tb->addAction(colorFondoAct);
            connect(tb, SIGNAL(triggered(QAction*)), this, SLOT(on_familia_color_de_fondo_Act_triggered(QAction*)));

            connect(tb, SIGNAL(clicked()), this, SLOT(on_toolButton_familia_clicked()));

            ui->gridLayout_familias->addWidget(tb, i, j);
        }
    }    
    select_familias();
}

void Comprobante::on_spinBox_familia_columnas_valueChanged(int arg1)
{
    for(int i = 0; i < 99; i++){
        for(int j = 0; j < 99; j++){
            SYSTEM->remove(ui->gridLayout_familias, i, j, true);
        }
    }
    int x = ui->spinBox_familia_filas->value();
    int y = arg1;
    for(int i = 0; i < x; i++){
        for(int j = 0; j < y; j++){
            QToolButton* tb = new QToolButton;
            tb->setText("____");
            tb->setPopupMode(QToolButton::MenuButtonPopup);

            QAction* fontAct = new QAction(tr("&Fuente"), this);
            //fontAct->setCheckable(true);
            fontAct->setShortcut(tr("Alt+f"));
            fontAct->setStatusTip(tr("Cambiar fuente..."));
            tb->addAction(fontAct);            
            connect(tb, SIGNAL(triggered(QAction*)), this, SLOT(on_familia_fontAct_triggered(QAction*)));

            QAction* colorAct = new QAction(tr("&Color"), this);
            //fontAct->setCheckable(true);
            colorAct->setShortcut(tr("Alt+c"));
            colorAct->setStatusTip(tr("Cambiar color..."));
            tb->addAction(colorAct);
            connect(tb, SIGNAL(triggered(QAction*)), this, SLOT(on_familia_colorAct_triggered(QAction*)));

            QAction* colorFondoAct = new QAction(tr("&Color de Fondo"), this);
            //fontAct->setCheckable(true);
            colorFondoAct->setShortcut(tr("Alt+b"));
            colorFondoAct->setStatusTip(tr("Cambiar color de fondo..."));
            tb->addAction(colorFondoAct);
            connect(tb, SIGNAL(triggered(QAction*)), this, SLOT(on_familia_color_de_fondo_Act_triggered(QAction*)));

            connect(tb, SIGNAL(clicked()), this, SLOT(on_toolButton_familia_clicked()));

            ui->gridLayout_familias->addWidget(tb, i, j);
        }
    }
    select_familias();
}

void Comprobante::on_spinBox_plato_filas_valueChanged(int arg1)
{
    for(int i = 0; i < 99; i++){
        for(int j = 0; j < 99; j++){
            SYSTEM->remove(ui->gridLayout_platos, i, j, true);
        }
    }
    int x = arg1;
    int y = ui->spinBox_plato_columnas->value();
    for(int i = 0; i < x; i++){
        for(int j = 0; j < y; j++){
            QToolButton* tb = new QToolButton;
            tb->setText("____");
            tb->setPopupMode(QToolButton::MenuButtonPopup);
            QAction* fontAct = new QAction(tr("&Fuente"), this);
            //fontAct->setCheckable(true);
            fontAct->setShortcut(tr("Alt+f"));
            fontAct->setStatusTip(tr("Cambiar fuente..."));
            tb->addAction(fontAct);
            connect(tb, SIGNAL(triggered(QAction*)), this, SLOT(on_plato_fontAct_triggered(QAction*)));

            QAction* colorAct = new QAction(tr("&Color"), this);
            //fontAct->setCheckable(true);
            colorAct->setShortcut(tr("Alt+c"));
            colorAct->setStatusTip(tr("Cambiar color..."));
            tb->addAction(colorAct);
            connect(tb, SIGNAL(triggered(QAction*)), this, SLOT(on_plato_colorAct_triggered(QAction*)));

            QAction* colorFondoAct = new QAction(tr("&Color de Fondo"), this);
            //fontAct->setCheckable(true);
            colorFondoAct->setShortcut(tr("Alt+b"));
            colorFondoAct->setStatusTip(tr("Cambiar color de fondo..."));
            tb->addAction(colorFondoAct);
            connect(tb, SIGNAL(triggered(QAction*)), this, SLOT(on_plato_color_de_fondo_Act_triggered(QAction*)));

            connect(tb, SIGNAL(clicked()), this, SLOT(on_toolButton_plato_clicked()));

            ui->gridLayout_platos->addWidget(tb, i, j);
        }
    }
    select_platos(ui->comboBox_familia->currentText());
}

void Comprobante::on_spinBox_plato_columnas_valueChanged(int arg1)
{
    for(int i = 0; i < 99; i++){
        for(int j = 0; j < 99; j++){
            SYSTEM->remove(ui->gridLayout_platos, i, j, true);
        }
    }
    int x = ui->spinBox_plato_filas->value();
    int y = arg1;
    for(int i = 0; i < x; i++){
        for(int j = 0; j < y; j++){
            QToolButton* tb = new QToolButton;
            tb->setText("____");
            tb->setPopupMode(QToolButton::MenuButtonPopup);

            QAction* fontAct = new QAction(tr("&Fuente"), this);
            //fontAct->setCheckable(true);
            fontAct->setShortcut(tr("Alt+f"));
            fontAct->setStatusTip(tr("Cambiar fuente..."));
            tb->addAction(fontAct);
            connect(tb, SIGNAL(triggered(QAction*)), this, SLOT(on_plato_fontAct_triggered(QAction*)));

            QAction* colorAct = new QAction(tr("&Color"), this);
            //fontAct->setCheckable(true);
            colorAct->setShortcut(tr("Alt+c"));
            colorAct->setStatusTip(tr("Cambiar color..."));
            tb->addAction(colorAct);
            connect(tb, SIGNAL(triggered(QAction*)), this, SLOT(on_plato_colorAct_triggered(QAction*)));

            QAction* colorFondoAct = new QAction(tr("&Color de Fondo"), this);
            //fontAct->setCheckable(true);
            colorFondoAct->setShortcut(tr("Alt+b"));
            colorFondoAct->setStatusTip(tr("Cambiar color de fondo..."));
            tb->addAction(colorFondoAct);

            connect(tb, SIGNAL(triggered(QAction*)), this, SLOT(on_plato_color_de_fondo_Act_triggered(QAction*)));

            connect(tb, SIGNAL(clicked()), this, SLOT(on_toolButton_plato_clicked()));

            ui->gridLayout_platos->addWidget(tb, i, j);
        }
    }
    select_platos(ui->comboBox_familia->currentText());
}
void Comprobante::on_pushButton_efectivo_clicked()
{
    int ret = QMessageBox::warning(this, "Advertencia", "¿Esta seguro de esta venta?", "Si", "Cancelar");
    switch(ret){
    case 0:{

    }break;
    case 1:{
        return;
    }break;
    }

    if(ui->tableWidget->rowCount() == 0){
        QMessageBox::warning(this, "Advertencia", "No hay detalle en la venta.", "Aceptar");
        return;
    }
    QString str_query = "";
    QSqlQuery query;

    str_query += "INSERT INTO comprobante(serie, numero, fecha_emision";
    str_query += ", anulado, pago_item_nombre, estado_item_nombre";
    str_query += ", operacion_item_nombre, tipo_item_nombre, nombre)";
    str_query += "VALUES(";
    str_query += QString() + "'" + "0000" + "'";
    str_query += QString() + ", '" + "00000000" + "'";
    str_query += ", '" + QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm-ss") + "'";
    str_query += QString() + ", '" + "1" + "'";
    str_query += QString() + ", '" + "Efectivo" + "'";
    str_query += QString() + ", '" + "Activo" + "'";
    str_query += QString() + ", '" + "Venta" + "'";
    str_query += QString() + ", '" + "Ticket" + "'";
    str_query += ", '" + ui->lineEdit_nombre->text() + "'";
    str_query += ")";
    str_query += "&&END_QUERY&&";
    if(ui->lineEdit_codigo->text().length() == 11){
        str_query += "INSERT INTO persona(cod, nombre, direccion";
        str_query += ", estado_item_nombre, tipo_item_nombre)";
        str_query += "VALUES(";
        str_query += "'" + ui->lineEdit_codigo->text() + "'";
        str_query += ", '" + ui->lineEdit_nombre->text() + "'";
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
        str_query += ", '" + ui->tableWidget->item(i, 1)->text() + "'";
        str_query += ", '" + ui->tableWidget->item(i, 0)->text() + "'";
        str_query += ")";
        str_query += "&&END_QUERY&&";
    }

    SYSTEM->multiple_query(str_query);

    qDebug()<<str_query<<endl;
    if(query.exec(str_query)){
        qDebug()<<"query ok"<<endl;

        const int Timeout = 5 * 1000;

        if (!m_ConnectStatus)
        {
            //m_pSocket->setSocketOption(QAbstractSocket::KeepAliveOption, 1);
            m_pSocket->connectToHost(QHostAddress("192.168.1.36"), 9100, QIODevice::WriteOnly);
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


        //print_caja();
        print_comanda();
        //QMessageBox::information(this, "Información", "Se guardo la venta.", "Aceptar");
    }else{
        QMessageBox::critical(this, "Error", "Error inesperado", "Ok");
    }
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
}

void Comprobante::on_pushButton_masterCard_clicked()
{
    on_pushButton_efectivo_clicked();
}

void Comprobante::on_pushButton_visa_clicked()
{
    on_pushButton_efectivo_clicked();
}
void Comprobante::showEvent(QShowEvent *event)
{
    event->accept();

    afterShow = true;    

    if(isMaximized()){
        qDebug()<<"isMaximized"<<endl;

        QString str_query = "";
        QSqlQuery query;

        str_query += "SELECT splitter_1, splitter_2, splitter_3";
        str_query += ", familia_filas, familia_columnas";
        str_query += ", plato_filas, plato_columnas FROM datos";

        qDebug()<<str_query<<endl;
        if(query.exec(str_query)){
            qDebug()<<"query ok"<<endl;
            if(!query.next()){
                return;
            }

            int splitter_1_val = query.value(0).toInt();
            int splitter_2_val = query.value(1).toInt();
            int splitter_3_val = query.value(2).toInt();
            int familia_filas = query.value(3).toInt();
            int familia_columnas = query.value(4).toInt();
            int plato_filas = query.value(5).toInt();
            int plato_columnas = query.value(6).toInt();

            ui->spinBox_familia_filas->setValue(familia_filas);
            ui->spinBox_familia_columnas->setValue(familia_columnas);
            ui->spinBox_plato_filas->setValue(plato_filas);
            ui->spinBox_plato_columnas->setValue(plato_columnas);
        }else{
            qDebug()<<"query bad"<<endl;
        }
    }
}
void Comprobante::closeEvent(QCloseEvent *event)
{
    event->accept();

    QString str_query = "";
    QSqlQuery query;

    str_query += "INSERT INTO datos(";
    str_query += "familia_filas, familia_columnas";
    str_query += ", plato_filas, plato_columnas";
    str_query += ")VALUES(";
    str_query += ""+QString(ui->spinBox_familia_filas->value())+"";
    str_query += ", "+QString(ui->spinBox_familia_columnas->value())+"";
    str_query += ", "+QString(ui->spinBox_plato_filas->value())+"";
    str_query += ", "+QString(ui->spinBox_plato_columnas->value())+"";
    str_query += ")";

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
        double cantidad = ui->tableWidget->item(i, 0)->text().toDouble();
        double precio = ui->tableWidget->item(i, 2)->text().toDouble();
        total += cantidad * precio;
    }
    ui->doubleSpinBox_total->setValue(total);
}

void Comprobante::on_toolButton_familia_clicked()
{
    QToolButton* toolB = (QToolButton*)sender();

    ui->comboBox_familia->setCurrentText(toolB->text());

    ui->spinBox_plato_filas->setValue(ui->spinBox_plato_filas->value()+1);
    ui->spinBox_plato_filas->setValue(ui->spinBox_plato_filas->value()-1);

    select_platos(toolB->text());
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
void Comprobante::print_caja()
{
    QFile file("print_caja.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
      return;

    QTextStream out(&file);

    QVector<QString> centerTexts;
    centerTexts.push_back("BILL MC QUACK");
    centerTexts.push_back("de Giancarlo Miguel Figueroa Nuñes");
    centerTexts.push_back("Coop. Daniel Alcides Carrion M-14");
    centerTexts.push_back("RUC: 10430252718");
    centerTexts.push_back("----------------------------------");
    centerTexts.push_back("Ticket Boleta");
    centerTexts.push_back("Maq.Regist.No: ");
    centerTexts.push_back("Autorizacion Sunat: ");
    centerTexts.push_back("Ticket Nro.: ");
    centerTexts.push_back("NOM CLI : " + ui->lineEdit_nombre->text());
    centerTexts.push_back("Fecha: " + QDate::currentDate().toString("dd/MM/yyyy"));
    centerTexts.push_back("Hora: " + QDateTime::currentDateTime().toString("hh:mm:ss"));
    centerTexts.push_back("==================================");
    centerTexts.push_back("Cant: Descripcion        : Precio");
    centerTexts.push_back("----------------------------------");
    SYSTEM->centerTexts(centerTexts);

    SYSTEM->epson_lineFeed(out);
    SYSTEM->epson_lineFeed(out);
    for(int i = 0; i < centerTexts.size(); i++){
        //SYSTEM->insert_left_spaces(centerTexts[i], 3);
        //SYSTEM->insert_right_spaces(centerTexts[i], 3);

        SYSTEM->epson_printText(out, centerTexts[i]);
        SYSTEM->epson_lineFeed(out);
    }
    SYSTEM->epson_lineFeed(out);
    double total = 0.0, igv = 0.18;
    for(int i = 0; i < ui->tableWidget->rowCount(); i++){
        QString cantidad = ui->tableWidget->item(i, 0)->text();
        QString descripcion = ui->tableWidget->item(i, 1)->text();
        double p_total = cantidad.toDouble() * ui->tableWidget->item(i, 2)->text().toDouble();

        total += cantidad.toDouble() * p_total;

        SYSTEM->justified(cantidad, 5);

        SYSTEM->epson_printText(out, cantidad + " " + descripcion);

        SYSTEM->epson_lineFeed(out);
    }
    QVector<QString> totales;
    totales.push_back("----------------------------------");
    totales.push_back("Sub-Total: " + QString().setNum(total - (total * igv), ' ', 1) );
    totales.push_back("IGV: " + QString().setNum(total * igv, ' ', 1));
    totales.push_back("Total: " + QString().setNum(total, ' ', 1));

    SYSTEM->rightTexts(totales);

    SYSTEM->epson_lineFeed(out);

    for(int i = 0; i < totales.size(); i++){
        SYSTEM->epson_printText(out, totales[i]);
        SYSTEM->epson_lineFeed(out);
    }

    SYSTEM->epson_lineFeed(out);
    SYSTEM->epson_lineFeed(out);

    SYSTEM->epson_paperCut(out);
    SYSTEM->epson_lineFeed(out);

    file.close();

    QProcess *myProcess = new QProcess(this);
    QString command = "copy";
    QStringList arguments = QStringList()
        << QCoreApplication::applicationFilePath() + "/" + file.fileName()
        << "LPT1";

    connect(myProcess, SIGNAL(finished(int, QProcess::ExitStatus))
        , this, SLOT(on_myProccess_finished(int, QProcess::ExitStatus)));

    qDebug() << command << " " << arguments << endl;
    myProcess->start(command, arguments);
}
void Comprobante::print_comanda()
{
    QByteArray block;

    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);

    //StarMicronics sm;
    //sm.openPortHelper("tcp:192.168.1.36", "", 1 * 1000);
    //sm.printSampleReceipt();
    //out << '/n';
    //out << '/n';
    //out << '/n';
    //out << '/n';
    //out << '/n';
    //out << '/n';


    QVector<QString> topText;
    SYSTEM->star_line_feed(out);
    SYSTEM->star_line_feed(out);
    topText.push_back("DESPACHAR: " + ui->lineEdit_nombre->text());
    topText.push_back("HORA: " + QDateTime::currentDateTime().toString("hh:mm:ss"));
    topText.push_back("==================================");

    SYSTEM->centerTexts(topText);

    for(int i = 0; i < topText.size(); i++){
        SYSTEM->star_print_text(out, topText[i]);
        SYSTEM->star_line_feed(out);
    }
    SYSTEM->star_line_feed(out);
    SYSTEM->star_print_text(out, "Cant: Descripcion");
    SYSTEM->star_line_feed(out);
    SYSTEM->star_print_text(out, "----------------------------------");
    SYSTEM->star_line_feed(out);

    for(int i = 0; i < ui->tableWidget->rowCount(); i++){
        QString cantidad = ui->tableWidget->item(i, 0)->text();
        QString descripcion = ui->tableWidget->item(i, 1)->text();

        QString().setNum(cantidad.toDouble(), ' ', 0);

        SYSTEM->justified(cantidad, 5);
        SYSTEM->star_print_text(out, cantidad + " " + descripcion);
        SYSTEM->star_line_feed(out);
    }
    //SYSTEM->star_print_text(out, "----------------------------------");
    SYSTEM->star_line_feed(out);
    SYSTEM->star_line_feed(out);    


    //SYSTEM->star_line_feed(out);
    //SYSTEM->star_line_feed(out);

    //const WCHAR* wc = L"\\x1bm" ;
    //_bstr_t b(wc);
    //const char* c = b;

    out << (qint64)0x001D5601;

    SYSTEM->star_line_feed(out);
    SYSTEM->star_line_feed(out);

    m_pSocket->write(block);



    //m_pSocket->write(b);

    m_pSocket->flush();

    m_pSocket->waitForBytesWritten();

    m_pSocket->disconnectFromHost();

    m_ConnectStatus = false;

}
