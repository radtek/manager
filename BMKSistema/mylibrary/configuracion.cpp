#include "configuracion.h"
#include "ui_configuracion.h"

Configuracion::Configuracion(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Configuracion)
{
    ui->setupUi(this);

    afterShow = false;
    firstShow = false;

    widget_previous = NULL;

    ui->pushButton_rollback->hide();
    ui->pushButton_commit->hide();

    disconnect(ui->dateEdit_igv, SIGNAL(dateChanged(QDate))
               , this, SLOT(on_dateEdit_igv_dateChanged(QDate)));

    sunatCambio.set_data(ui->lineEdit_id_tipo_cambio
                         , ui->lineEdit_tipo_cambio
                         , ui->dateEdit_cambio
                         , ui->label_loading
                         , ui->pushButton_reload
                         , ui->pushButton_guardar_cambio
                         , ui->pushButton_ver_internet);    

    ui->dateEdit_igv->setDate(QDate::currentDate());
    connect(ui->dateEdit_igv, SIGNAL(dateChanged(QDate))
               , this, SLOT(on_dateEdit_igv_dateChanged(QDate)));

    ui->lineEdit_id_tipo_cambio->hide();

    process_cambio(ui->dateEdit_cambio->date());

    this->installEventFilter(this);

    ui->pushButton_ver_internet->installEventFilter(this);

    ui->lineEdit_tipo_cambio->installEventFilter(this);
    ui->dateEdit_cambio->installEventFilter(this);
    ui->pushButton_guardar_cambio->installEventFilter(this);
    ui->pushButton_ver_cambio->installEventFilter(this);

    ui->lineEdit_igv->installEventFilter(this);
    ui->dateEdit_igv->installEventFilter(this);
    ui->pushButton_guardar_igv->installEventFilter(this);
    ui->pushButton_ver_igv->installEventFilter(this);

    ui->pushButton_salir->installEventFilter(this);
}

Configuracion::~Configuracion()
{
    qDebug()<<"delete configuracion"<<endl;
    delete ui;
}
void Configuracion::set_widget_previous(QWidget *widget_previous)
{
    this->widget_previous = widget_previous;
}

void Configuracion::process_cambio(const QDate& date)
{	
	//ui->dateEdit_cambio->setDisabled(true);	          

    QString str_cmp_tipo_cambio = "SELECT '' FROM tipo_cambio WHERE fecha = '"+date.toString("yyyy-MM-dd")+"' AND moneda_id = "+QString().setNum(moneda_items::DOLAR);

    QString columns_str_dolar = "c.num, c.fecha, c.id";
    QString str_tipo_cambio = "SELECT "+columns_str_dolar+" FROM tipo_cambio c";
    str_tipo_cambio += " WHERE c.moneda_id = "+QString().setNum(moneda_items::DOLAR)+" ORDER BY c.fecha DESC LIMIT 1";

    QString str_cmp_igv = "SELECT '' FROM igv LIMIT 1";

    QString columns_str_igv = "ig.num, ig.fecha, ig.id";
    QString str_igv = "SELECT "+columns_str_igv+" FROM igv ig";
    str_igv += " WHERE ig.moneda_id = "+QString().setNum(moneda_items::SOL)+" ORDER BY ig.fecha DESC LIMIT 1";

    QString str_query = "(SELECT IF(EXISTS("+str_cmp_tipo_cambio+"), 1, 0), '', '')";
    str_query +=  " UNION ALL (SELECT IF(EXISTS("+str_cmp_igv+"), 1, 0), '', '')";
    str_query +=  " UNION ALL ("+str_tipo_cambio+")";
    str_query +=  " UNION ALL ("+str_igv+")";

    qDebug()<<str_query<<endl;
    if(query_config.exec(str_query)){
        query_config.seek(-1);
        llenarDatos();
    }else{

    }     
}

void Configuracion::llenarDatos()
{    
    query_config.seek(0);
    int exists_tipo_cambio = query_config.value(0).toInt();
    if(exists_tipo_cambio == 1){
        query_config.seek(2);

        ui->label_dolar_value->setText(QString().setNum(query_config.value(0).toDouble(), 'f', 3));

        if(query_config.value(1).toString() == QDate::currentDate().toString("yyyy-MM-dd")){
            ui->label_dolar_date->setText("Ingresado el día de hoy.");
        }else{
            ui->label_dolar_date->setText("Ultima fecha, el: "+query_config.value(1).toString());
        }
    }

    query_config.seek(1);
    int exists_igv = query_config.value(0).toInt();
    if(exists_igv == 1){
        query_config.seek(3);

        ui->label_igv_value->setText(QString().setNum(query_config.value(0).toDouble(), 'f', 3));

        if(query_config.value(1).toString() == QDate::currentDate().toString("yyyy-MM-dd")){
            ui->label_igv_date->setText("Ingresado el día de hoy.");
        }else{
            ui->label_igv_date->setText("Ultima fecha, el: "+query_config.value(1).toString());
        }
    }
}
void Configuracion::showEvent(QShowEvent* se)
{
    se->accept();

    afterShow = true;

    if(!firstShow){
        firstShow = true;
    }
}
bool Configuracion::eventFilter(QObject *obj, QEvent *e)
{
    QWidget* w_temp;
    w_temp = this;
    if(obj == w_temp){
        if(e->type() == QEvent::MouseButtonPress){
            if(focusWidget()){
                focusWidget()->setFocus();
            }else{
                ui->lineEdit_tipo_cambio->setFocus();
                ui->lineEdit_tipo_cambio->setCursorPosition(ui->lineEdit_tipo_cambio->text().length());
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
                    focusWidget()->setFocus();
                }else{
                    ui->lineEdit_tipo_cambio->setFocus();
                    ui->lineEdit_tipo_cambio->setCursorPosition(ui->lineEdit_tipo_cambio->text().length());
                }
                afterShow = false;
            }
            return true;
        }
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Escape:{
                ui->pushButton_salir->click();
                return true;
            }break;
            }
        }else{

        }
        return false;
    }
    w_temp = ui->pushButton_ver_internet;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:{
                ui->pushButton_ver_internet->click();
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
    w_temp = ui->lineEdit_tipo_cambio;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:
                ui->dateEdit_cambio->setFocus();
                return true;
            case Qt::Key_Enter:{
                QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QApplication::sendEvent(w_temp, key);
                return true;
            }break;
            case Qt::Key_Period:{
                QString str = ui->lineEdit_tipo_cambio->text();
                int periodPosition = str.indexOf('.');
                int cursorPosition;
                if(periodPosition == -1){
                    cursorPosition = ui->lineEdit_tipo_cambio->cursorPosition();
                    str.insert(cursorPosition, '.');
                    cursorPosition += 1;
                }else{
                    cursorPosition = ui->lineEdit_tipo_cambio->cursorPosition();
                    str.replace(".", " ");
                    str.insert(ui->lineEdit_tipo_cambio->cursorPosition(), '.');
                    str.replace(" ", "");
                    if(periodPosition >= cursorPosition)
                        cursorPosition += 1;
                }
                ui->lineEdit_tipo_cambio->setText(str);
                ui->lineEdit_tipo_cambio->setCursorPosition(cursorPosition);
                return true;
            }break;
            }

        }else{

        }
        return false;
    }
    w_temp = ui->dateEdit_cambio;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:
                ui->pushButton_guardar_cambio->setFocus(Qt::TabFocusReason);
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
    w_temp = ui->pushButton_guardar_cambio;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:
                ui->pushButton_ver_cambio->setFocus(Qt::TabFocusReason);
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
    w_temp = ui->pushButton_ver_cambio;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:
                ui->lineEdit_igv->setFocus();
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
    w_temp = ui->lineEdit_igv;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:
                ui->dateEdit_igv->setFocus();
                return true;
            case Qt::Key_Enter:{
                QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QApplication::sendEvent(w_temp, key);
                return true;
            }break;
            case Qt::Key_Period:{
                QString str = ui->lineEdit_igv->text();
                int periodPosition = str.indexOf('.');
                int cursorPosition;
                if(periodPosition == -1){
                    cursorPosition = ui->lineEdit_igv->cursorPosition();
                    str.insert(cursorPosition, '.');
                    cursorPosition += 1;
                }else{
                    cursorPosition = ui->lineEdit_igv->cursorPosition();
                    str.replace(".", " ");
                    str.insert(ui->lineEdit_igv->cursorPosition(), '.');
                    str.replace(" ", "");
                    if(periodPosition >= cursorPosition)
                        cursorPosition += 1;
                }
                ui->lineEdit_igv->setText(str);
                ui->lineEdit_igv->setCursorPosition(cursorPosition);
                return true;
            }break;
            }

        }else{

        }
        return false;
    }
    w_temp = ui->dateEdit_igv;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:
                ui->pushButton_guardar_igv->setFocus(Qt::TabFocusReason);
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
    w_temp = ui->pushButton_guardar_igv;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:
                ui->pushButton_ver_igv->setFocus(Qt::TabFocusReason);
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
    w_temp = ui->pushButton_ver_igv;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:
                ui->pushButton_salir->setFocus(Qt::TabFocusReason);
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
    return eventFilter(obj, e);
}
bool Configuracion::update_cambio()
{
	QString str_query;
	QSqlQuery query;

    str_query = "SELECT IF(EXISTS(SELECT '' FROM tipo_cambio";
    str_query += " WHERE fecha = '"+ui->dateEdit_cambio->date().toString("yyyy-MM-dd")+"'";
    str_query += " AND moneda_id = "+QString().setNum(moneda_items::DOLAR);
    str_query += "), 1, 0)";

    int exists = 0;
    qDebug()<<str_query<<endl;
    if(query.exec(str_query)){
        query.next();
        exists = query.value(0).toInt();
    }else{
        return false;
    }
	QString num;
    num = ui->lineEdit_tipo_cambio->text();

	QString str_fecha = ui->dateEdit_cambio->date().toString("yyyy-MM-dd");

    if (exists == 0) {
        str_query = "INSERT INTO tipo_cambio(moneda_id, num, fecha)VALUES(";
        str_query += QString().setNum(moneda_items::DOLAR) + ", '" + num + "', '" + str_fecha + "')";
        str_query += "&&END_QUERY&&";
    }else{
        str_query = "UPDATE tipo_cambio";
        str_query += " SET";
        str_query += " num = '" + num +"'";
        str_query += " WHERE moneda_id = "+QString().setNum(moneda_items::DOLAR);
        str_query += " AND fecha = '"+str_fecha+"'";
        str_query += "&&END_QUERY&&";
	}
    str_query += "COMMIT";
    str_query += "&&END_QUERY&&";

	SYSTEM->multiple_query(str_query);

	qDebug() << str_query << endl;
	if (query.exec(str_query)) {
		return true;
    } else {
        if(query.exec("ROLLBACK")){

        }else{

        }
		return false;
	}
}

bool Configuracion::update_igv()
{
	QString str_query;
	QSqlQuery query;

    str_query = "SELECT IF(EXISTS(SELECT '' FROM igv";
    str_query += " WHERE fecha = '"+ui->dateEdit_igv->date().toString("yyyy-MM-dd")+"'";
    str_query += " AND moneda_id = "+QString().setNum(moneda_items::SOL);
    str_query += "), 1, 0)";

    int exists = 0;
    qDebug()<<str_query<<endl;
    if(query.exec(str_query)){
        query.next();
        exists = query.value(0).toInt();
    }else{
        return false;
    }

	QString num;
    num = ui->lineEdit_igv->text();

	QString str_fecha = ui->dateEdit_igv->date().toString("yyyy-MM-dd");

    if (exists == 0) {
        str_query = "INSERT INTO igv(moneda_id, num, fecha)VALUES(";
        str_query += QString().setNum(moneda_items::SOL) + ", '" + num + "', '" + str_fecha + "')";
        str_query += "&&END_QUERY&&";

    } else {
        str_query = "UPDATE igv";
        str_query += " SET";
        str_query += " num = '"+num+"'";
        str_query += " WHERE moneda_id = "+QString().setNum(moneda_items::SOL);
        str_query += " AND fecha = '"+str_fecha+"'";
        str_query += "&&END_QUERY&&";
	}
    str_query += "COMMIT";
    str_query += "&&END_QUERY&&";

	SYSTEM->multiple_query(str_query);

	qDebug() << str_query << endl;
	if (query.exec(str_query)) {
		return true;
    }else{
        if(query.exec("ROLLBACK")){

        }else{

        }
		return false;
	}

}
void Configuracion::on_pushButton_guardar_cambio_clicked()
{
    if(update_cambio()){
        int ret= QMessageBox::information(this, "Guardar cambio", "Transacción lista.");
        ret= ret;

        query_config.seek(2);

        if(ui->dateEdit_cambio->date() == QDate::currentDate()){
            ui->label_dolar_value->setText(ui->lineEdit_tipo_cambio->text());
            ui->label_dolar_date->setText("Ingresado el día de hoy.");
        }else{
            QDate fecha_ingreso = ui->dateEdit_cambio->date();
            QDate fecha_ultima = query_config.value(1).toDate();
            if(fecha_ingreso < QDate::currentDate() && fecha_ingreso >= fecha_ultima){
                ui->label_dolar_value->setText(ui->lineEdit_tipo_cambio->text());
                ui->label_dolar_date->setText("Ultima fecha, el: "+fecha_ingreso.toString("yyyy-MM-dd"));
            }
        }
    }else{
        int ret= QMessageBox::warning(this, "Guardar cambio", "Ocurrio un inconveniente.");
        ret= ret;
    }
}

void Configuracion::on_pushButton_guardar_igv_clicked()
{
    if(update_igv()){
        int ret= QMessageBox::information(this, "Guardar IGV", "Transacción lista.");
        ret= ret;

        query_config.seek(3);

        if(ui->dateEdit_igv->date() == QDate::currentDate()){
            ui->label_igv_value->setText(ui->lineEdit_igv->text());
            ui->label_igv_date->setText("Ingresado el día de hoy.");
        }else{
            QDate fecha_ingreso = ui->dateEdit_igv->date();
            QDate fecha_ultima = query_config.value(1).toDate();
            if(fecha_ingreso < QDate::currentDate() && fecha_ingreso >= fecha_ultima){
                ui->label_igv_value->setText(ui->lineEdit_igv->text());
                ui->label_igv_date->setText("Ultima fecha, el: "+fecha_ingreso.toString("yyyy-MM-dd"));
            }
        }
    }else{
        int ret= QMessageBox::warning(this, "Guardar IGV", "Ocurrio un inconveniente.");
        ret= ret;
    }
}

void Configuracion::on_pushButton_salir_clicked()
{
    if(widget_previous){
        setAttribute(Qt::WA_DeleteOnClose);
        SYSTEM->change_center_w(this, widget_previous);
    }else{
        SYSTEM->clear_center_w(this);
    }
}

void Configuracion::on_pushButton_ver_cambio_clicked()
{
    VerCambio* w = new VerCambio;
    w->set_widget_previous(widget_previous);

    SYSTEM->change_center_w(this, w);
}

void Configuracion::on_pushButton_ver_igv_clicked()
{
    VerIGV* w = new VerIGV;
    w->set_widget_previous(widget_previous);

    SYSTEM->change_center_w(this, w);
}

void Configuracion::on_dateEdit_cambio_dateChanged(const QDate &date)
{
    process_cambio(date);
}

void Configuracion::on_dateEdit_igv_dateChanged(const QDate &date)
{
    process_cambio(date);
}
void Configuracion::on_pushButton_rollback_clicked()
{
    QString str_query = "ROLLBACK";
    str_query += "&&END_QUERY&&";
    SYSTEM->multiple_query(str_query);

    QSqlQuery query;
    if (query.exec(str_query)) {
        QMessageBox::warning(this, "Información", "Se deshizo la previa con éxito.");
    } else {
        QMessageBox::critical(this, "Error", "No está disponible la base de datos.");
    }
}

void Configuracion::on_pushButton_commit_clicked()
{
    QString str_query = "COMMIT";
    str_query += "&&END_QUERY&&";
    SYSTEM->multiple_query(str_query);

    QSqlQuery query;
    if (query.exec(str_query)) {
        QMessageBox::information(this, "Información", "Se comitearon las transacciones.");
    } else {
        QMessageBox::critical(this, "Error", "No está disponible la base de datos.");
    }
}

void Configuracion::on_pushButton_backup_clicked()
{
    QString curPath = QDir::currentPath();
    QFileDialog dialog(this, "Guardar Archivo", curPath);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setNameFilter(tr("Archivo sql (*.sql)"));

    QString fileName = "";

    if (dialog.exec()) {
        //QDateTime dt = QDateTime::currentDateTime();
        //QString str_dt = dt.toString("dd-MM-yyyy");
        fileName = dialog.selectedFiles().front();
        /*
        if (fileName.compare("") == 0) {
            fileName += str_dt + ".sql";
        }
        */
    } else {
        return;
    }
    qDebug() << fileName << endl;

    QString db_name = QSqlDatabase::database().databaseName();

    QProcess *myProcess = new QProcess(this);
    QString command = "\"C:/Program Files/MySQL/MySQL Server 5.7/bin/mysqldump\"";
    QStringList arguments = QStringList() << "--host=127.0.0.1" << "--user=root" << "--password=1234"
        //<< "--add-drop-database" << "--routines"
        << "--no-create-db"
        << "--no-create-info"
        << "--no-set-names"
        << "--no-tablespaces"
        << "--skip-add-locks"
        << "--skip-disable-keys"
        << "--databases" << db_name
        << QString()+"--result-file="+""+fileName+"";

    connect(myProcess, SIGNAL(finished(int, QProcess::ExitStatus))
        , this, SLOT(on_myProccess_finished(int, QProcess::ExitStatus)));

    qDebug() << command << " " << arguments << endl;
    myProcess->start(command, arguments);
}
void Configuracion::on_myProccess_started()
{
    qDebug() << "START" << endl;

}
void Configuracion::on_myProccess_finished(int exitCode, QProcess::ExitStatus exitStatus)
{
    qDebug() << "Exit Code: " << exitCode;
    qDebug() << "Exit Status: " << exitStatus;

    if (exitCode == 0) {
        QMessageBox::information(this, "Información", "Felicidades. Finalizo la operacion sin errores.", "Ok");
    } else {
        QMessageBox::critical(this, "Error", "No finalizo corretamente.");
    }

    delete sender();
    //QProcess* p = (QProcess*)QObject::sender();

    //qDebug() << p->errorString() << endl;
}
void Configuracion::on_pushButton_restore_clicked()
{
    int ret = QMessageBox::warning(this, "ATENCION", "Va restaurar una copia anterior.\n"
                                   "¿Esta seguro de realizar esta operación?", "Si", "No");
    switch (ret) {
    case 0: {


    }break;
    case 1: {
        return;
    }
    }

    QString curPath = QDir::currentPath();
    QFileDialog dialog(this, "Abrir Archivo", curPath);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setNameFilter(tr("Archivo sql (*.sql)"));

    QString fileName = "";
    if (dialog.exec()) {
        fileName = dialog.selectedFiles().front();
    }
    else {
        return;
    }
    qDebug() << fileName << endl;

    if (fileName.compare("") == 0)
        return;
    QString str_query = "DELETE FROM familia";
    str_query += "&&END_QUERY&&";
    str_query += "DELETE FROM comprobante";
    str_query += "&&END_QUERY&&";
    str_query += "ALTER TABLE comprobante AUTO_INCREMENT = 1";
    str_query += "&&END_QUERY&&";
    str_query += "DELETE FROM usuario";
    str_query += "&&END_QUERY&&";
    str_query += "DELETE FROM persona";
    str_query += "&&END_QUERY&&";
    str_query += "DELETE FROM grupo";
    str_query += "&&END_QUERY&&";
    str_query += "DELETE FROM item";
    str_query += "&&END_QUERY&&";
    str_query += "DELETE FROM direccion";
    str_query += "&&END_QUERY&&";
    str_query += "DELETE FROM plato";
    str_query += "&&END_QUERY&&";    

    QSqlQuery query;

    SYSTEM->multiple_query(str_query);

    qDebug() << str_query << endl;
    if (query.exec(str_query)) {

    } else {
        QMessageBox::critical(this, "Error", "No finalizo corretamente.");
        return;
    }
    QString db_name = QSqlDatabase::database().databaseName();

    QProcess *myProcess = new QProcess(this);
    QString command = "\"C:/Program Files/MySQL/MySQL Server 5.7/bin/mysql.exe\"";
    QStringList arguments = QStringList()
            << "--host=127.0.0.1" << "--user=root" << "--password=1234"
            << "--database = " + db_name;

        //<< "--execute=\"source " + fileName + "\"";
    myProcess->waitForFinished(100000);
    myProcess->setStandardInputFile(fileName);
    connect(myProcess, SIGNAL(started())
        , this, SLOT(on_myProccess_started()));
    connect(myProcess, SIGNAL(finished(int, QProcess::ExitStatus))
        , this, SLOT(on_myProccess_finished(int, QProcess::ExitStatus)));

    qDebug() << command << endl;//<< arguments << endl;
    myProcess->start(command, arguments);
}
