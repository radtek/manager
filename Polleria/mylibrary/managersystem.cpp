#include "managersystem.h"
#include "ui_managersystem.h"

#include <QtXml>
#include "SoapSunatCall.h"

ManagerSystem::ManagerSystem(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ManagerSystem)
{
    ui->setupUi(this);

    //ui->statusBar->addWidget(new SnackBarInfo);
    afterShow = false;


    //QThread* thread = new QThread;
    //connect(thread, SIGNAL (started()), worker, SLOT (process()));

    /*
    QWebEngineView view_consultaRuc;
    view_consultaRuc.load(tr("http://www.sunat.gob.pe/cl-ti-itmrconsruc/jcrS00Alias"));
    QWebEngineView view_consultaDNI;
    view_consultaDNI.load(tr("https://cel.reniec.gob.pe/valreg/valreg.do?accion=ini"));
    */

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(on_timer_timeout()));
    timer->start(60000);

    shortCut_salir = new QShortcut(QKeySequence(Qt::Key_Alt+Qt::Key_F4), this);
    shortCut_salir->setEnabled(true);
    connect(shortCut_salir, SIGNAL(activated()), this, SLOT(on_shortCut_salir()));
    connect((QGuiApplication*)QGuiApplication::instance(), SIGNAL(applicationStateChanged(Qt::ApplicationState))
            , this, SLOT(on_application_state_changed(Qt::ApplicationState)));

    this->installEventFilter(this);
}

ManagerSystem::~ManagerSystem()
{
    delete SYSTEM;
    delete DATABASE;
    delete shortCut_salir;
    delete ui;
    delete timer;
}

void ManagerSystem::execReportes()
{
    //ui->menuReportes->showTearOffMenu();
}

void ManagerSystem::set_toolBar(QWidget* w)
{
    //ui->mainToolBar->hide();
    ui->mainToolBar->addWidget(w);
    ui->mainToolBar->adjustSize();
    ui->mainToolBar->setFloatable(false);
    ui->mainToolBar->setAllowedAreas(Qt::TopToolBarArea
                                 //|Qt::RightToolBarArea
                                 //|Qt::LeftToolBarArea
                                 |Qt::BottomToolBarArea);

}
void ManagerSystem::showToolBar()
{
    ui->mainToolBar->show();
}
void ManagerSystem::hideToolBar()
{
    ui->mainToolBar->hide();
}
void ManagerSystem::on_shortCut_salir()
{
    this->close();
}

void ManagerSystem::on_application_state_changed(Qt::ApplicationState state)
{
    /*
    if(state == Qt::ApplicationSuspended){
        //qDebug()<<"application suspended"<<endl;
    }
    if(state == Qt::ApplicationHidden){
        //qDebug()<<"application hidden"<<endl;
    }
    if(state == Qt::ApplicationInactive){
        //qDebug()<<"application inactive"<<endl;
        if(QApplication::focusWidget()){
            //qDebug()<<typeid(*QApplication::focusWidget()).name()<<endl;
            if(QString(typeid(*QApplication::focusWidget()).name()) == "18QExpandingLineEdit"){
                //qDebug()<<"QTableWidget"<<endl;
                //qDebug()<<typeid(*QApplication::focusWidget()->parentWidget()).name()<<endl;
                QApplication::focusWidget()->parentWidget()->setFocus();
            }
            /*
            if(typeid(*QApplication::focusWidget()) == typeid(QTableWidget)){
                qDebug()<<"QTableWidget"<<endl;
                QApplication::focusWidget()->parentWidget()->setFocus();
            */
        //}
    //}
/*
    if(state == Qt::ApplicationActive){
        //qDebug()<<"application active"<<endl;

        /*
        if(QApplication::focusWidget()){
            if(typeid(*QApplication::focusWidget()) == typeid(QTableWidget)){
                QApplication::focusWidget()->setFocus(Qt::TabFocusReason);
            }
        }*/
    //}
}

void ManagerSystem::focusInEvent(QFocusEvent *fe)
{

}
void ManagerSystem::focusOutEvent(QFocusEvent *fe)
{

}

void ManagerSystem::showEvent(QShowEvent *se)
{
    se->accept();

    afterShow = true;
}

void ManagerSystem::closeEvent(QCloseEvent *ce)
{

    if(centralWidget()){
        int ret = QMessageBox::warning(this, "ManagerSystem", "¿Esta seguro que desea salir del sistema?", "Aceptar", "Salir");
        switch(ret){
        case 0:{
            QApplication::closeAllWindows();
            ce->accept();
        }break;
        case 1:{
            ce->ignore();
        }break;
        }
    }else{
        QApplication::closeAllWindows();
        ce->accept();
    }

}

bool ManagerSystem::eventFilter(QObject *obj, QEvent *e)
{

    if(e->type() == QEvent::Paint){
        if(afterShow) {

            afterShow = false;
        }
        return true;
    }

    if (e->type() == QEvent::KeyPress){
        if (obj == this){
            QKeyEvent *ke = static_cast<QKeyEvent*>(e);

            if (ke->key()== Qt::Key_F1){
                if(ke->isAutoRepeat()) {
                    ke->ignore();
                    return QWidget::eventFilter(obj, e);
                } else {
                    ke->accept();
                }

                return false;
            }
            /*
            if (ke->key()== Qt::Key_Escape){
                this->close();
                return false;
            }*/
        }
    }
    return QWidget::eventFilter(obj, e);
}


void ManagerSystem::on_actionPor_Producto_triggered()
{
    ComprobanteBuscar* w = new ComprobanteBuscar;
    w->setWindowFlag(Qt::Dialog);
    w->setAttribute(Qt::WA_DeleteOnClose);

    w->showMaximized();
    w->raise();
}

void ManagerSystem::on_timer_timeout()
{
    QString str_query;
    str_query += "SELECT serie, numero, tipo_item_nombre, xmlContent WHERE comprobante";
    str_query += " WHERE DATE(fecha_emision) = '"+QDate::currentDate().toString("yyyy-MM-dd")+"'";
    str_query += " AND operacion_item_nombre = '"+QString(VENTA)+"'";
    str_query += " AND (tipo_item_nombre = '"+QString(BOLETA)+"'";
    str_query += " OR tipo_item_nombre = '"+QString(FACTURA)+"')";
    str_query += " AND estado_sunat = 'Pendiente'";
    str_query += " ORDER BY id LIMIT 1";

    QSqlQuery query;
    if(query.exec(str_query)){
        if(query.next()){
            QString str_ruc = "20498590587";
            string fileName_xml;
            QString serie = query.value(0).toString();
            QString numero = query.value(1).toString();
            QString documento = query.value(2).toString();
            if(documento.compare(BOLETA) == 0){
                fileName_xml = QString(str_ruc+"-"+"03"+"-"+serie+"-"+numero+".XML").toStdString();
            }
            if(documento.compare(FACTURA) == 0){
                fileName_xml = QString(str_ruc+"-"+"01"+"-"+serie+"-"+numero+".XML").toStdString();
            }
            QFile file(fileName_xml.c_str());
            if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
                return;
            }
            QTextStream out(&file);
            out << query.value(3).toString();
            file.close();

            /*
            if(obj.sendXML()<0){

                QString error;
                if(!obj.get_str_error()){
                    error = "";
                }else{
                    error = obj.get_str_error();
                }
                QMessageBox::critical(this, "Informacion", error+"\nNO HAY INTERNET.", "Aceptar");
                ui->comboBox_documento->setCurrentText("Pre-Venta");


            }else{

                cout << "ha sido enviado a la sunat" <<endl;

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
                    QString str_zip_dir = "\"" + app_dir + "\\res.zip\"";

                    QString command = "\"C:\\Program Files\\7-Zip\\7z.exe\" x"
                                      " "+str_zip_dir+ " -aoa";

                    qDebug()<<command<<endl;

                    QProcess process;
                    process.start(command);
                    //QProcess::execute(command);
                    process.waitForFinished();
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
                    QString str_zip_dir = "\"" + app_dir + "\\res\"";

                    QString command = "\"C:\\Program Files\\7-Zip\\7z.exe\" x"
                                      " "+str_zip_dir+" -aoa";

                    qDebug()<<command<<endl;
                    QProcess process;
                    process.start(command);
                    //QProcess::execute(command);
                    process.waitForFinished();
                }
                QDomDocument dom;
                QFile file("R-"+QString(fileName_xml.c_str()));
                while (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
                }

                while (!dom.setContent(&file)) {
                }

                // print out the element names of all elements that are direct children
                // of the outermost element.
                QDomElement root = dom.firstChildElement("ar:ApplicationResponse");
                QDomNodeList items = root.elementsByTagName("cbc:ResponseCode");
                QDomNode itemnode = items.at(0);

                QString str_responseCode;
                if(itemnode.isElement()){
                    QDomElement iteme = itemnode.toElement();
                    str_responseCode = iteme.text();
                }
                items = root.elementsByTagName("cbc:Description");
                itemnode = items.at(0);

                QString str_description;
                if(itemnode.isElement()){
                    QDomElement iteme = itemnode.toElement();
                    str_description = iteme.text();
                }
                items = root.elementsByTagName("DigestValue");
                itemnode = items.at(0);

                QString str_digestValue;
                if(itemnode.isElement()){
                    QDomElement iteme = itemnode.toElement();
                    str_digestValue = iteme.text();
                    this->digestValue = str_digestValue;
                }


                QString str_response = dom.toString(-1);

                str_response = SYSTEM->replace_quotes_simple(str_response);
                QSqlQuery query;
                QString str_query = "UPDATE comprobante SET xmlResponse='"+str_response+"'";
                str_query += " WHERE serie = '"+ui->lineEdit_serie->text()+"'";
                str_query += " AND numero = '"+ui->lineEdit_numero->text()+"'";
                str_query += " AND DATE(fecha_emision) = '"
                        +ui->dateTimeEdit_fecha_emision->date().toString("yyyy-MM-dd")+"'";


                qDebug()<<str_query<<endl;
                if(query.exec(str_query)){
                    qDebug()<<"si copio"<<endl;
                }
                file.close();

                int responseCode = str_responseCode.toInt();
                if(responseCode >= 100 && responseCode <= 1999){
                    QMessageBox::warning(this, "Advertencia", "Response Code: "+str_responseCode
                                         + "\nDescripción: "+str_description, "Aceptar");
                    return;
                }
                if(responseCode >= 2000 && responseCode <= 3999){
                    QSqlQuery query;
                    QString str_query = "UPDATE comprobante SET estado_sunat='Anulado'";
                    str_query += " ORDER BY id DESC LIMIT 1";
                    if(query.exec(str_query)){
                        QMessageBox::warning(this, "Advertencia", "Response Code: "+str_responseCode
                                             + "\nDescripción: "+str_description, "Aceptar");
                    }
                    return;
                }
                if(responseCode >= 4000){
                    QSqlQuery query;
                    QString str_query = "UPDATE comprobante SET estado_sunat='Observacion'";
                    str_query += " ORDER BY id DESC LIMIT 1";
                    if(query.exec(str_query)){
                        QMessageBox::warning(this, "Advertencia", "Response Code: "+str_responseCode
                                             + "\nDescripción: "+str_description, "Aceptar");
                    }
                    print_machelo();
                    print_epson();
                }
                if(responseCode == 0){
                    SYSTEM->commit();
                    int numero = ui->lineEdit_numero->text().toInt();
                    ui->lineEdit_numero->setText(QString().setNum(numero+1));
                    QSqlQuery query;
                    QString str_query = "UPDATE comprobante SET estado_sunat='Activo'";
                    str_query += " ORDER BY id DESC LIMIT 1";
                    if(query.exec(str_query)){
                        QMessageBox::warning(this, "Advertencia", "Response Code: "+str_responseCode
                                             + "\nDescripción: "+str_description, "Aceptar");
                    }
                    print_machelo();
                    print_epson();
                }

            }*/
        }
    }else{

    }
}
