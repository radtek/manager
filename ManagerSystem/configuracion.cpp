#include "configuracion.h"
#include "ui_configuracion.h"

Configuracion::Configuracion(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Configuracion)
{
    ui->setupUi(this);

	isShow = false;

	textEdit = NULL;
	webView_tipo_cambio = NULL;
	countReload = 0;
	countDatos = 0;
	timer_cambio = NULL;

	if (!timer_cambio) {
		timer_cambio = new QTimer();
		connect(timer_cambio, SIGNAL(timeout()), this, SLOT(show_timeCambio()));
	}	

	disconnect(ui->dateEdit_cambio, SIGNAL(dateChanged(QDate)), this, SLOT(on_dateEdit_cambio_dateChanged(QDate)));
	ui->dateEdit_cambio->setDate(QDate::currentDate());
	connect(ui->dateEdit_cambio, SIGNAL(dateChanged(QDate)), this, SLOT(on_dateEdit_cambio_dateChanged(QDate)));

	ui->dateEdit_igv->setDate(QDate::currentDate());

	if (!webView_tipo_cambio) {
		webView_tipo_cambio = new QWebEngineView;
		connect(webView_tipo_cambio, SIGNAL(loadStarted()), this, SLOT(loadStarted()));
		connect(webView_tipo_cambio, SIGNAL(loadFinished(bool)), this, SLOT(loadFinished(bool)));
	}
	webView_tipo_cambio->load(tr("http://www.sunat.gob.pe/cl-at-ittipcam/tcS01Alias"));
    //webView_tipo_cambio->show();
}

Configuracion::~Configuracion()
{
	delete webView_tipo_cambio;
	delete timer_cambio;
	delete textEdit;
    delete ui;
}

void Configuracion::process_cambio(const QDate& date)
{	
	//ui->dateEdit_cambio->setDisabled(true);	          

    QString str_cmp_tipo_cambio = "SELECT EXISTS (SELECT '' FROM tipo_cambio WHERE fecha = '"+date.toString("yyyy-MM-dd")+"' AND moneda_id = "+QString().setNum(moneda_items::SOL)+"), '', ''";
    QString str_cmp_igv = "SELECT EXISTS (SELECT '' FROM igv LIMIT 1), '', ''";

    QString columns_str_dolar = "c.num, c.fecha, c.id";
    QString str_dolar = "SELECT "+columns_str_dolar+" FROM tipo_cambio c";
    str_dolar += " WHERE c.moneda_id = "+QString().setNum(moneda_items::SOL)+" ORDER BY c.fecha DESC LIMIT 1";
    QString columns_str_igv = "ig.num, ig.fecha, ig.id";
    QString str_igv = "SELECT "+columns_str_igv+" FROM igv ig ORDER BY ig.fecha DESC LIMIT 1";
    str_igv += "";

    QString str_query = "("+str_cmp_tipo_cambio+")"
                        " UNION ALL ("+str_cmp_igv+")"
                        " UNION ALL ("+str_dolar+")"
                        " UNION ALL ("+str_igv+")";

    qDebug()<<str_query<<endl;
    if(query_config.exec(str_query)){
        query_config.seek(-1);
        llenarDatos();
    }else{

    }

    this->installEventFilter(this);    
}

void Configuracion::llenarDatos()
{    
    QString str_dolar;

    query_config.seek(0);
    str_dolar = query_config.value(0).toString();
    if(str_dolar == "1"){
        query_config.seek(2);

        if(query_config.value(1).toString() == QDate::currentDate().toString("yyyy-MM-dd")){
			//timer_cambio->start(100);
			delete ui->label_loading->movie();
			ui->label_loading->hide();
			ui->label_loading->show();
        }else{
			qDebug() << "SET CAMBIO" << endl;
			//timer_cambio->start();
			setCambio(ui->dateEdit_cambio->date());
        }

    }else{
        str_dolar = "";
		qDebug() << "SET CAMBIO" << endl;
		//timer_cambio->start();
		setCambio(ui->dateEdit_cambio->date());
    }

    if(str_dolar != ""){
        ui->label_dolar_value->setText(QString().setNum(query_config.value(0).toDouble()));

        if(query_config.value(1).toString() == QDate::currentDate().toString("yyyy-MM-dd")){
            ui->label_dolar_date->setText("Ingresado el día de hoy.");
        }else{
            ui->label_dolar_date->setText(query_config.value(1).toString());
        }
    }else{
        ui->label_dolar_value->setText("No ingresado el día de hoy.");
        ui->label_dolar_value->setStyleSheet("QLabel { background-color : red; color : blue; }");
        ui->label_dolar_date->hide();
    }

    QString str_igv;

    query_config.seek(1);
    str_igv = query_config.value(0).toString();
    if(str_igv == "1"){
        query_config.seek(3);
    }else{
        str_igv = "";
    }

    if(str_igv != ""){
        ui->label_igv_value->setText(QString().setNum(query_config.value(0).toDouble()));

        if(query_config.value(1).toString() == QDate::currentDate().toString("yyyy-MM-dd")){
            ui->label_igv_date->setText("Ingresado el día de hoy.");
        }else{
            ui->label_igv_date->setText("Ingresado el "+query_config.value(1).toString());
        }
    }else{
        ui->label_igv_value->setText("No ingresado el día de hoy.");
        ui->label_igv_value->setStyleSheet("QLabel { background-color : red; color : blue; }");
        ui->label_igv_date->hide();
    }

}

void Configuracion::setCambio(const QDate& date)
{
	if (countReload > 0) return;
	countReload++;

	QWebEngineView* view = webView_tipo_cambio;
	QWebEnginePage* page = view->page();
	QString strJS;

	strJS = "document.getElementsByClassName('lb')[0].selectedIndex = "
		+ QString().setNum(date.month());
    page->runJavaScript(strJS, [=](const QVariant &v) { /*qDebug()<<"set img: "<<endl<< v.toString();*/});

	strJS = "document.getElementsByClassName('lb')[1].value = "
		+ QString().setNum(date.year());
    page->runJavaScript(strJS, [=](const QVariant &v) { /*qDebug()<<"set canvas: "<<endl<< v.toString();*/});

	strJS = "document.getElementsByClassName('button')[0].click()";
    page->runJavaScript(strJS, [=](const QVariant &v) { /*qDebug()<<"set canvas: "<<endl<< v.toString();*/});

	countDatos = 0;

	timer_cambio->start(100);
}

void Configuracion::show_timeCambio()
{
	QWebEngineView* view = webView_tipo_cambio;
	QWebEnginePage* page = view->page();
	
	QString strJS = "var mes = document.getElementById(\"mesElegido\").value;"
					"var anio = document.getElementById(\"anioElegido\").value;"
					"var table = document.body.getElementsByClassName('class=\"form-table\"')[0];"
					"var length = table.rows.length;"
					"var text = mes + \" \" + anio + \" \";"
					"for(i = 1; i < length; i++){ text += table.rows[i].innerText + \" \"; };"
					"text;";

	qDebug() << strJS << endl;
	QString str_file = "cambio.dat";

	page->runJavaScript(strJS, [=](const QVariant &v)
	{
		QFile file(str_file);
		if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
			return;
		file.flush();
		QTextStream out(&file);
		out << v.toString();
		file.close();
	});
	QFile file(str_file);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;	
	QString output = QString(" ") + file.readAll();
	file.close();

	QStringList list = output.split(QRegularExpression("\\s+"), QString::SkipEmptyParts);

	if (list.count() > 0)
	{
		int month = ui->dateEdit_cambio->date().month();
		int year = ui->dateEdit_cambio->date().year();

		if (list.count() < 2) return;
		int mes = list[0].toInt();
		int anio = list[1].toInt();

		qDebug() << month << " !=" << mes << endl;
		qDebug() << year << " !=" << anio << endl;

		if (month != mes || year != anio) return;
		
		QString msg = list[2];
		if (msg.compare("No") == 0) {
			qDebug() << "No" << endl;
			timer_cambio->stop();
			//delete ui->label_loading->movie();
			//ui->label_loading->hide();
			//ui->label_loading->show();
			//ui->pushButton_guardar_cambio->setEnabled(true);

			int year = ui->dateEdit_cambio->date().year();
			int month = ui->dateEdit_cambio->date().month();
			int day = 1;
			if (month == 1) {
				year--;
				month = 12;
			}
			else {
				month--;
			}
			QDate date;
			date.setDate(year, month, day);
			date.setDate(year, month, date.daysInMonth());

			qDebug() << date.toString() << endl;
			ui->dateEdit_cambio->setDate(date);

			return;
		}

		QString r;
		int day = ui->dateEdit_cambio->date().day();
		int day_hint = 0;
		int i = 2;
		for (; i < list.count(); i = i + 3) {
			day_hint = list[i].toInt();
			if (day <= day_hint) {
				break;
			}
			r += " " + list[i];
			qDebug() << list[i] << endl;
		}
		qDebug() << "r: " << r << endl;

		double cambio_compra = 0.0;
		if (day < day_hint) {
			qDebug() << "day < day_hint" << endl;
			if (i != 2) {
				cambio_compra = list[i - 2].toDouble();
				ui->doubleSpinBox_cambio->setValue(cambio_compra);
				delete ui->label_loading->movie();
				ui->label_loading->hide();
				ui->label_loading->show();
				//ui->pushButton_guardar_cambio->setEnabled(true);

				timer_cambio->stop();
			}
			else {
				delete ui->label_loading->movie();
				ui->label_loading->hide();
				ui->label_loading->show();
				//ui->pushButton_guardar_cambio->setEnabled(true);

				timer_cambio->stop();

				int year = ui->dateEdit_cambio->date().year();
				int month = ui->dateEdit_cambio->date().month();
				int day = 1;
				if (month == 1) {
					year--;
					month = 12;
				}
				else {
					month--;
				}
				QDate date;
				date.setDate(year, month, day);
				date.setDate(year, month, date.daysInMonth());

				qDebug() << date.toString() << endl;
				ui->dateEdit_cambio->setDate(date);
			}
		}
		if (day == day_hint) {
			qDebug() << "day == day_hint" << endl;
			cambio_compra = list[i + 1].toDouble();
			ui->doubleSpinBox_cambio->setValue(cambio_compra);
			delete ui->label_loading->movie();
			ui->label_loading->hide();
			ui->label_loading->show();
			//ui->pushButton_guardar_cambio->setEnabled(true);

			timer_cambio->stop();
		}
		if (day > day_hint) {
			qDebug() << "day > day_hint" << endl;
			cambio_compra = list[i - 2].toDouble();
			ui->doubleSpinBox_cambio->setValue(cambio_compra);
			delete ui->label_loading->movie();
			ui->label_loading->hide();
			ui->label_loading->show();
			//ui->pushButton_guardar_cambio->setEnabled(true);

			timer_cambio->stop();
		}
	}	
}


void Configuracion::loadStarted()
{
    QMovie *movie = new QMovie(":/new/Iconos/loading_4.gif");
    ui->label_loading->setMovie(movie);
    movie->start();
    //ui->pushButton_guardar_cambio->setDisabled(true);
}

void Configuracion::loadFinished(bool b)
{
    if(b){		
		//timer_cambio->stop();
		//timer_cambio->start(100);
		//setCambio();
		
		process_cambio(ui->dateEdit_cambio->date());
    }else{
        ui->doubleSpinBox_cambio->setValue(0.0);
        delete ui->label_loading->movie();
        ui->label_loading->hide();
        ui->label_loading->show();
        //ui->pushButton_guardar_cambio->setEnabled(true);
    }
}

bool Configuracion::eventFilter(QObject *obj, QEvent *e)
{
    QWidget* w_temp;
    w_temp = this;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Escape:
                ui->pushButton_salir->click();
            }

        }else{

        }
        return false;
    }

    if(e->type()== QEvent::FocusOut){
        /*
        if(obj== ui->lineEdit_cambio){
            qDebug()<<"coño de tu madre"<<endl;
            timer->stop();
            ui->label_dolar->setStyleSheet("");
            return false;
        }*/
    }
    return eventFilter(obj, e);
}

void Configuracion::showEvent(QShowEvent* se)
{
    ui->dateEdit_cambio->setFocus(Qt::TabFocusReason);

}
bool Configuracion::update_cambio()
{
	QString str_query;
	QSqlQuery query;

	QString num;
	num.setNum(ui->doubleSpinBox_cambio->value());
	query_config.seek(0);

	QString str_fecha = ui->dateEdit_cambio->date().toString("yyyy-MM-dd");

	if (query_config.value(0).toString() == "0") {
        str_query = "INSERT INTO tipo_cambio(moneda_id, num, fecha)VALUES(";

        str_query += QString().setNum(moneda_items::SOL) + ", " + num + ", '" + str_fecha + "')";
        str_query += "&&END_QUERY&&";
	}
	else {
        str_query = "UPDATE tipo_cambio";
        str_query += " SET";
        str_query += " num = " + QString().setNum(moneda_items::SOL);
        str_query += " WHERE moneda_id = "+QString().setNum(moneda_items::SOL)+" AND fecha = '"+str_fecha+"'";
        str_query += "&&END_QUERY&&";
	}

	SYSTEM->multiple_query(str_query);

	qDebug() << str_query << endl;
	if (query.exec(str_query)) {
		return true;
	}
	else {
		return false;
	}
}

bool Configuracion::update_igv()
{
	QString str_query;
	QSqlQuery query;

	QString num;
	num.setNum(ui->doubleSpinBox_igv->value());
	query_config.seek(1);

	QString str_fecha = ui->dateEdit_igv->date().toString("yyyy-MM-dd");

	if (query_config.value(0).toString() == "0") {
        str_query = "INSERT INTO igv(moneda_id, num, fecha)VALUES(";
        str_query += QString().setNum(moneda_items::SOL) + ", " + num + ", '" + str_fecha + "')";
        str_query += "&&END_QUERY&&";

    } else {
        str_query = "UPDATE igv";
        str_query += "SET";
        str_query += " num = "+num;
        str_query += " WHERE moneda_id = "+QString().setNum(moneda_items::SOL)+" AND fecha = '"+str_fecha+"'";
        str_query += "&&END_QUERY&&";
	}

	SYSTEM->multiple_query(str_query);

	qDebug() << str_query << endl;
	if (query.exec(str_query)) {
		return true;
	}
	else {
		return false;
	}

}
QPushButton* Configuracion::get_pb_ver_cambio()
{
	return ui->pushButton_ver_cambio;
}
QPushButton* Configuracion::get_pb_ver_igv()
{
	return ui->pushButton_ver_igv;
}
void Configuracion::on_pushButton_guardar_cambio_clicked()
{
    if(update_cambio()){
        int ret= QMessageBox::information(this, "Guardar cambio", "Transacción lista.");
        ret= ret;

        ui->label_dolar_value->setText(QString().setNum(ui->doubleSpinBox_cambio->value()));
        ui->label_dolar_date->setText("Ingresado el día de hoy.");
        ui->label_dolar_value->setStyleSheet("");
        ui->label_dolar_date->show();
        /*
        QString num;
        num.setNum(ui->lineEdit_cambio->text().toDouble());
        ui->lineEdit_cambio->setText(num);
        */
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
        ui->label_igv_value->setText(QString().setNum(ui->doubleSpinBox_igv->value()));
        ui->label_igv_date->setText("Ingresado el día de hoy.");
        ui->label_igv_value->setStyleSheet("");
        ui->label_igv_date->show();

        /*
        QString num;
        num.setNum(ui->lineEdit_igv->text().toDouble());
        ui->lineEdit_igv->setText(num);
        */
    }else{
        int ret= QMessageBox::warning(this, "Guardar IGV", "Ocurrio un inconveniente.");
        ret= ret;
    }
}

void Configuracion::on_pushButton_salir_clicked()
{
    this->close();
}

void Configuracion::on_pushButton_ver_internet_clicked()
{
    //webView_tipo_cambio->showMaximized();

}

void Configuracion::on_dateEdit_cambio_dateChanged(const QDate &date)
{	
	qDebug() << "cambio data changed" << endl;
	countReload = 0;
	webView_tipo_cambio->reload();
}
