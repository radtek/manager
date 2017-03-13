#include "toolbar.h"
#include "ui_toolbar.h"

ToolBar::ToolBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ToolBar)
{
    ui->setupUi(this);

    afterShow = false;

    cur_label = NULL;

    ui->pushButton_rollback->hide();
    ui->pushButton_commit->hide();

    this->installEventFilter(this);
    ui->toolButton_colaboradores->installEventFilter(this);
    ui->toolButton_compras->installEventFilter(this);
    ui->toolButton_home->installEventFilter(this);
    ui->toolButton_configuracion->installEventFilter(this);
    ui->toolButton_productos->installEventFilter(this);
    ui->toolButton_reportes->installEventFilter(this);
    ui->toolButton_ventas->installEventFilter(this);
    ui->pushButton_backup->installEventFilter(this);
    ui->pushButton_restore->installEventFilter(this);
}

ToolBar::~ToolBar()
{
    qDebug()<<"TOOLBAR DELETE"<<endl;
    delete ui;
}

QToolButton* ToolBar::tb_home(){ return ui->toolButton_home; }
QToolButton* ToolBar::tb_ventas(){ return ui->toolButton_ventas; }
QToolButton* ToolBar::tb_compras(){return ui->toolButton_compras; }
QToolButton* ToolBar::tb_productos(){ return ui->toolButton_productos; }
QToolButton* ToolBar::tb_colaboradores(){ return ui->toolButton_colaboradores; }
QToolButton* ToolBar::tb_reportes(){ return ui->toolButton_reportes; }
QToolButton* ToolBar::tb_configuracion(){ return ui->toolButton_configuracion; }

void ToolBar::set_labels_ssheet(int pos)
{
    /*
    SYSTEM->set_stylesheet(QVector<QWidget*>()<<ui->label_ventas<<ui->label_compras<<ui->label_reportes
                           <<ui->label_productos<<ui->label_colaboradores
                           <<ui->label_opciones, pos);
                           */
}

void ToolBar::on_toolButton_home_clicked()
{
    ui->toolButton_home->setFocus();

    QWidget* w = new Home;

    APP_MAINWINDOW->hideToolBar();
    APP_MAINWINDOW->setCentralWidget(w);
}

void ToolBar::on_toolButton_ventas_clicked()
{	    
    //bool b = false;
    double igv = SYSTEM->get_igv();
    if(igv == 0.0){
        QMessageBox::warning(0, "Ingrese IGV", "Usted puede ingresar el IGV en el módulo de Configuración", "Ok");
        //b = true;
    }
    double dolar = SYSTEM->get_dolar();
    if(dolar == 0.0){
        QMessageBox::warning(0, "Actualize Tipo de Cambio", "Usted puede actualizar el Tipo de Cambio en el módulo de Configuración", "Ok");
        //b = true;
    }	

    ui->toolButton_ventas->setFocus();

    QWidget* w = SYSTEM->set_center_w(new VentaBuscar);
    APP_MAINWINDOW->setCentralWidget(w);

    if(cur_label){
        cur_label->setStyleSheet("");
    }
    cur_label = ui->label_ventas;

    ui->label_ventas->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: rgb(0, 0, 0);");
}

void ToolBar::on_toolButton_compras_clicked()
{    
	//bool b = false;
    double igv = SYSTEM->get_igv();
    if(igv == 0.0){
        QMessageBox::warning(0, "Ingrese IGV", "Usted puede ingresar el IGV en el módulo de Configuración", "Ok");
        //b = true;
    }
    double dolar = SYSTEM->get_dolar();
    if(dolar == 0.0){
        QMessageBox::warning(0, "Actualize Tipo de Cambio", "Usted puede actualizar el Tipo de Cambio en el módulo de Configuración", "Ok");
        //b = true;
    }
	
    ui->toolButton_compras->setFocus();

    QWidget* w = SYSTEM->set_center_w(new CompraBuscar);
    APP_MAINWINDOW->setCentralWidget(w);

    if(cur_label){
        cur_label->setStyleSheet("");
    }
    cur_label = ui->label_compras;

    ui->label_compras->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: rgb(0, 0, 0);");
}
void ToolBar::on_toolButton_productos_clicked()
{
    ui->toolButton_productos->setFocus();

    QWidget* place = new ProductoBuscar;
    QWidget* w = SYSTEM->set_center_w(place);

    APP_MAINWINDOW->setCentralWidget(w);

    if(cur_label){
        cur_label->setStyleSheet("");
    }
    cur_label = ui->label_productos;

    ui->label_productos->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: rgb(0, 0, 0);");
}

void ToolBar::on_toolButton_colaboradores_clicked()
{
    ui->toolButton_colaboradores->setFocus();

    QWidget* w = SYSTEM->set_center_w(new ColaboradorBuscar);
    APP_MAINWINDOW->setCentralWidget(w);

    if(cur_label){
        cur_label->setStyleSheet("");
    }
    cur_label = ui->label_colaboradores;

    ui->label_colaboradores->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: rgb(0, 0, 0);");

}
void ToolBar::on_toolButton_reportes_clicked()
{
    ui->toolButton_reportes->setFocus();

    QWidget* w = SYSTEM->set_center_w(new A_Reporte);
    APP_MAINWINDOW->setCentralWidget(w);

    if(cur_label){
        cur_label->setStyleSheet("");
    }
    cur_label = ui->label_reportes;

    ui->label_reportes->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: rgb(0, 0, 0);");
}

void ToolBar::on_toolButton_configuracion_clicked()
{
    ui->toolButton_configuracion->setFocus();

    if(cur_label){
        cur_label->setStyleSheet("");
    }
    cur_label = ui->label_configuracion;

    ui->label_configuracion->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: rgb(0, 0, 0);");

    QWidget* w = new Configuracion;
	w->setAttribute(Qt::WA_DeleteOnClose);
    w->show();
}

void ToolBar::on_pushButton_rollback_clicked()
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

void ToolBar::on_pushButton_commit_clicked()
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

void ToolBar::on_pushButton_backup_clicked()
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
	QProcess *myProcess = new QProcess(this);
	QString command = "\"C:/Program Files/MySQL/MySQL Server 5.7/bin/mysqldump\"";
	//command += " -u root -p1234 --add-drop-database --routines --databases managersystem >";
	//command += " C:/Users/lorda/Desktop/programa/ManagerSystem/db.sql";
	QStringList arguments = QStringList() << "--host=127.0.0.1" << "--user=root" << "--password=1234"
        //<< "--add-drop-database" << "--routines"
        << "--no-create-db"
        << "--no-create-info"
        << "--no-set-names"
        << "--no-tablespaces"
        << "--skip-add-locks"
        << "--skip-disable-keys"
        << "--databases" << "managersystem"
		<< QString()+"--result-file="+""+fileName+"";

	connect(myProcess, SIGNAL(finished(int, QProcess::ExitStatus))
		, this, SLOT(on_myProccess_finished(int, QProcess::ExitStatus)));

	qDebug() << command << " " << arguments << endl;
	//myProcess->setProgram(command);
	//myProcess->setStandardOutputFile("C:/Users/lorda/Desktop/programa/ManagerSystem");
	myProcess->start(command, arguments);
}
void ToolBar::on_myProccess_started()
{
	qDebug() << "START" << endl;

}
void ToolBar::on_myProccess_finished(int exitCode, QProcess::ExitStatus exitStatus)
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
void ToolBar::on_pushButton_restore_clicked()
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
    QString str_query = "DELETE FROM documento";
    str_query += "&&END_QUERY&&";
    str_query += "DELETE FROM producto";
    str_query += "&&END_QUERY&&";
    str_query += "DELETE FROM persona";
    str_query += "&&END_QUERY&&";
    str_query += "DELETE FROM marca";
    str_query += "&&END_QUERY&&";
    str_query += "DELETE FROM unidad";
    str_query += "&&END_QUERY&&";
    str_query += "DELETE FROM tipo";
    str_query += "&&END_QUERY&&";

    str_query += "DELETE FROM delimitador";
    str_query += "&&END_QUERY&&";
    str_query += "DELETE FROM moneda";
    str_query += "&&END_QUERY&&";
    str_query += "DELETE FROM igv";
    str_query += "&&END_QUERY&&";
    str_query += "DELETE FROM tipo_cambio";
    str_query += "&&END_QUERY&&";

    str_query += "DELETE FROM operacion";
    str_query += "&&END_QUERY&&";

    str_query += "DELETE FROM tipo_documento";
    str_query += "&&END_QUERY&&";

    str_query += "DELETE FROM series";
    str_query += "&&END_QUERY&&";

    str_query += "DELETE FROM tipo_persona";
    str_query += "&&END_QUERY&&";

    str_query += "DELETE FROM amplitud";
    str_query += "&&END_QUERY&&";

    str_query += "DELETE FROM rol";
    str_query += "&&END_QUERY&&";

    QSqlQuery query;

    SYSTEM->multiple_query(str_query);

    qDebug() << str_query << endl;
    if (query.exec(str_query)) {

    } else {
        QMessageBox::critical(this, "Error", "No finalizo corretamente.");
        return;
    }

	QProcess *myProcess = new QProcess(this);
	QString command = "\"C:/Program Files/MySQL/MySQL Server 5.7/bin/mysql.exe\"";
    QStringList arguments = QStringList()
            << "--host=127.0.0.1" << "--user=root" << "--password=1234"
            << "--database=managersystem";

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

void ToolBar::showEvent(QShowEvent *se)
{
    se->accept();

    afterShow = true;
}
bool ToolBar::eventFilter(QObject *obj, QEvent *e)
{
    QWidget* w_temp;

    w_temp = this;
    if(obj == w_temp){
        if(e->type() == QEvent::MouseButtonPress){
            if(focusWidget()){
                focusWidget()->setFocus(Qt::TabFocusReason);
            }else{
                this->setFocus();
                QKeyEvent *event = new QKeyEvent(QEvent::KeyPress, Qt::Key_Tab, Qt::NoModifier);
                QApplication::sendEvent(this, event);
            }
            return true;
        }
        if(e->type() == QEvent::Paint){
            if(afterShow) {
                if(focusWidget()){
                    focusWidget()->setFocus(Qt::TabFocusReason);
                }else{
                    this->setFocus();
                    QKeyEvent *event = new QKeyEvent(QEvent::KeyPress, Qt::Key_Tab, Qt::NoModifier);
                    QApplication::sendEvent(this, event);
                }
                afterShow = false;
            }
            return true;
        }
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Tab:{
                if(cur_label){
                    cur_label->setStyleSheet("");
                }
                cur_label = ui->label_inicio;

                ui->label_inicio->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: rgb(0, 0, 0);");
                ui->toolButton_home->setFocus(Qt::TabFocusReason);
                return true;
            }break;
            }
        }
        return false;
    }

    w_temp = ui->toolButton_home;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Tab:{
                if(cur_label){
                    cur_label->setStyleSheet("");
                }
                cur_label = ui->label_ventas;

                ui->label_ventas->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: rgb(0, 0, 0);");
                ui->toolButton_ventas->setFocus(Qt::TabFocusReason);
                return true;
            }break;
            case Qt::Key_Return:{
                ui->toolButton_home->click();
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
    w_temp = ui->toolButton_ventas;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Tab:{
                if(cur_label){
                    cur_label->setStyleSheet("");
                }
                cur_label = ui->label_compras;

                ui->label_compras->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: rgb(0, 0, 0);");
                ui->toolButton_compras->setFocus(Qt::TabFocusReason);
                return true;
            }break;
            case Qt::Key_Return:{
                ui->toolButton_ventas->click();
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
    w_temp = ui->toolButton_compras;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Tab:{
                if(cur_label){
                    cur_label->setStyleSheet("");
                }
                cur_label = ui->label_productos;

                ui->label_productos->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: rgb(0, 0, 0);");
                ui->toolButton_productos->setFocus(Qt::TabFocusReason);
                return true;
            }break;
            case Qt::Key_Return:{
                ui->toolButton_compras->click();
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
    w_temp = ui->toolButton_productos;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Tab:{
                if(cur_label){
                    cur_label->setStyleSheet("");
                }
                cur_label = ui->label_colaboradores;

                ui->label_colaboradores->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: rgb(0, 0, 0);");
                ui->toolButton_colaboradores->setFocus(Qt::TabFocusReason);
                return true;
            }break;
            case Qt::Key_Return:{
                ui->toolButton_productos->click();
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
    w_temp = ui->toolButton_colaboradores;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Tab:{
                if(cur_label){
                    cur_label->setStyleSheet("");
                }
                cur_label = ui->label_reportes;

                ui->label_reportes->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: rgb(0, 0, 0);");
                ui->toolButton_reportes->setFocus(Qt::TabFocusReason);
                return true;
            }break;
            case Qt::Key_Return:{
                ui->toolButton_colaboradores->click();
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
    w_temp = ui->toolButton_reportes;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Tab:{
                if(cur_label){
                    cur_label->setStyleSheet("");
                }
                cur_label = ui->label_configuracion;

                ui->label_configuracion->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: rgb(0, 0, 0);");
                ui->toolButton_configuracion->setFocus(Qt::TabFocusReason);
                return true;
            }break;
            case Qt::Key_Return:{
                ui->toolButton_reportes->click();
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
    w_temp = ui->toolButton_configuracion;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Tab:{
                if(cur_label){
                    cur_label->setStyleSheet("");
                    ui->pushButton_backup->setFocus(Qt::TabFocusReason);
                }
                return true;
            }break;
            case Qt::Key_Return:{
                ui->toolButton_configuracion->click();
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
    w_temp = ui->pushButton_backup;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Tab:{
                ui->pushButton_restore->setFocus(Qt::TabFocusReason);
                return true;
            }break;
            case Qt::Key_Return:{
                ui->pushButton_backup->click();
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
    w_temp = ui->pushButton_restore;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Tab:{
                this->setFocus();
                QKeyEvent *event = new QKeyEvent ( QEvent::KeyPress, Qt::Key_Tab, Qt::NoModifier);
                QApplication::sendEvent(this, event);
                return true;
            }break;
            case Qt::Key_Return:{
                ui->pushButton_restore->click();
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
