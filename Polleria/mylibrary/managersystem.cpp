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

    snackBar = NULL;
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
    timer->start(12000);

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
    if (!timer->isActive()) { return; }

    QEventLoop eventLoop;

    QThread* thread = new QThread();
    //thread->deleteLater();

    QTimer tT;

    tT.setSingleShot(true);
    connect(&tT, SIGNAL(timeout()), &eventLoop, SLOT(quit()));

    connect(thread, SIGNAL(started()), this, SLOT(on_thread_started()));
    connect(thread, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    connect(thread, SIGNAL(finished()), this, SLOT(on_thread_finished()));

    thread->start();

    tT.start(10000);

    eventLoop.exec();

    if(tT.isActive()){
        // download complete
        tT.stop();
        //thread->terminate();

    } else {
        // timeout
    }


}
void ManagerSystem::on_thread_started()
{
    QString str_query;
    str_query += "SELECT serie, numero, tipo_item_nombre, xmlContent, fecha_emision, id FROM comprobante";
    str_query += " WHERE operacion_item_nombre = '"+QString(VENTA)+"'";
    str_query += " AND (tipo_item_nombre = '"+QString(BOLETA)+"'";
    str_query += " OR tipo_item_nombre = '"+QString(FACTURA)+"')";
    str_query += " AND estado_sunat = 'Pendiente'";
    str_query += " ORDER BY id LIMIT 1";

    QSqlQuery query;
    //QMessageBox::information(this, "Informacion", str_query, "Aceptar");
    if(query.exec(str_query)){
        //QMessageBox::information(this, "Informacion", "si", "Aceptar");
        if(query.next()){

            QString str_ruc = "20498590587";
            string fileName_xml, fileName_zip;
            QString serie = query.value(0).toString();
            QString numero = query.value(1).toString();
            QString documento = query.value(2).toString();
            QDateTime fecha_emision = query.value(4).toDateTime();
            QString id = query.value(5).toString();

            if(documento.compare(BOLETA) == 0){
                fileName_xml = QString(str_ruc+"-"+"03"+"-"+serie+"-"+numero+".XML").toStdString();
                fileName_zip = QString(str_ruc+"-"+"03"+"-"+serie+"-"+numero+".ZIP").toStdString();
            }
            if(documento.compare(FACTURA) == 0){
                fileName_xml = QString(str_ruc+"-"+"01"+"-"+serie+"-"+numero+".XML").toStdString();
                fileName_zip = QString(str_ruc+"-"+"01"+"-"+serie+"-"+numero+".ZIP").toStdString();
            }


            SoapSunatCall obj;
            obj.set_str_certificatekeyfile((char *)"mycertificate.pem");
            obj.set_str_keyfile((char *)"mykey.pem");
            obj.set_str_password((char *)"4t48gWXqdd358Msx");
            obj.set_str_cafile((char *)"myca.pem");
            obj.set_str_certfile((char *)"mycert.pem");
            obj.set_str_sec_username((char *)"20498590587ELBIBAM6");
            obj.set_str_sec_password((char *)"nArputo-");

            obj.set_str_zip_xml_filename((char *)fileName_zip.c_str());
            obj.set_str_zip_xml_out_filename((char *)"res.zip");
            obj.set_str_xml_file((char *)fileName_xml.c_str());
            if(documento.compare(BOLETA) == 0){
                obj.set_str_id_signature((char *)"Signaturekg");
            }
            if(documento.compare(FACTURA) == 0){
                obj.set_str_id_signature((char *)"SignatureSP");
            }

            if(query.value(3).toString().compare("") == 0){
                //QMessageBox::information(this, "Informacion", "la conche", "Aceptar");
                str_query = "(SELECT serie, numero, fecha_emision, delivery, vuelto, pago_item_nombre, direccion, nombre FROM comprobante WHERE id='"+id+"')";
                str_query += "UNION ALL";
                str_query += "(SELECT per.cod, per.nombre, per.direccion, per.tipo_item_nombre, '', '', '', '' FROM comprobante_has_persona com_h_per";
                str_query += " JOIN persona per ON per.cod = com_h_per.persona_cod";
                str_query += " WHERE com_h_per.comprobante_id='"+id+"')";
                str_query += "UNION ALL";
                str_query += "(SELECT com_h_prod.cantidad, 'UND', prod.nombre, com_h_prod.precio, prod.id, '', '', '' FROM comprobante_has_producto com_h_prod";
                str_query += " JOIN producto prod ON prod.id = com_h_prod.producto_id";
                str_query += " WHERE com_h_prod.comprobante_id='"+id+"')";

                qDebug()<<str_query<<endl;
                if(query.exec(str_query)){
                    query.next();
                    QString serie = query.value(0).toString();
                    QString numero = query.value(1).toString();
                    QDateTime dt = query.value(2).toDateTime();
                    double delivery_query = query.value(3).toDouble();
                    double vuelto_query = query.value(4).toDouble();
                    QString pago_query = query.value(5).toString();
                    QString direccion_query = query.value(6).toString();
                    QString nombre_query = query.value(7).toString();

                    query.next();
                    QString cod = query.value(0).toString();
                    //QString nombre_query = query.value(1).toString();
                    //QString direccion_query = query.value(2).toString();
                    QString tipo_persona = query.value(3).toString();

                    if(tipo_persona.compare(Persona::cliente_eventual) == 0 || tipo_persona.compare(Persona::cliente_delivery) == 0){
                        cod = "";
                    }
                    QVector<QString> v_cantidad, v_precio, v_nombre, v_id;
                    double total = 0.0;
                    while(query.next()){
                        QString cantidad = QString().setNum(query.value(0).toDouble(), ' ', 0);
                        //QString unidad = query.value(1).toString();
                        QString nombre = query.value(2).toString();
                        QString precio = query.value(3).toString();
                        QString id = query.value(4).toString();
                        v_cantidad.push_back(cantidad);
                        v_precio.push_back(precio);
                        v_nombre.push_back(nombre);
                        v_id.push_back(id);
                        total += precio.toDouble();
                    }
                    if(documento.compare(BOLETA) == 0){
                        if(!SYSTEM->create_boleta(serie, numero
                                                  , total, dt, cod
                                                  , nombre_query, direccion_query
                                                  , v_cantidad, v_precio
                                                  , v_nombre, v_id)){
                            return;
                        }
                    }
                    if(documento.compare(FACTURA) == 0){
                        if(!SYSTEM->create_factura(serie, numero
                                                  , total, dt, cod
                                                  , nombre_query, direccion_query
                                                  , v_cantidad, v_precio
                                                  , v_nombre, v_id)){
                            return;
                        }
                    }
                    if(obj.xmlDSig() < 0){
                        //QMessageBox::information(this, "Informacion", "No se puede firmar el documento","Aceptar");
                        return;
                    }else{
                        //qDebug() << "Se firmo el doc" << endl;
                        //QMessageBox::warning(this, "Advertencia", "Se firmo el doc", "Aceptar");
                    }
                }else{
                    return;
                }
            }else{
                QFile file(fileName_xml.c_str());
                if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
                    return;
                }
                QXmlStreamWriter xmlWriter(&file);
                xmlWriter.setAutoFormatting(true);

                //xmlWriter.setCodec("UTF-8");
                //xmlWriter.writeStartDocument("1.0", false);

                xmlWriter.device()->write(query.value(3).toString().toStdString().c_str());
                xmlWriter.device()->close();
                //QTextStream out(&file);
                //out << textXML;
                //file.flush();
                file.close();
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

            }

            QXmlStreamReader xml_reader(&file);

            QString str_content = xml_reader.device()->readAll().toStdString().c_str();

            //str_content = SYSTEM->replace_quotes_simple(str_content);
            QSqlQuery query;
            QString str_query = "UPDATE comprobante SET xmlContent='"+str_content+"'";
            str_query += " WHERE id = '"+id+"'";
            //qDebug()<<str_query<<endl;
            if(query.exec(str_query)){
                //QMessageBox::warning(this, "Advertencia", "Si copio", "Aceptar");
            }else{
                //QMessageBox::warning(this, "Advertencia", "No copio", "Aceptar");
                return;
            }
            file.close();

            try{
                if(obj.sendXML()<0){
                    QFile file("error.txt");
                    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
                        QString error = file.readAll().toStdString().c_str();
                        //QMessageBox::critical(this, "Informacion", error, "Aceptar");

                        if(snackBar)
                            ui->statusBar->removeWidget(snackBar);
                        snackBar = new SnackBarInfo;
                        snackBar->set_data(error, ":/new/Iconos/exclamation.png");
                        ui->statusBar->addWidget(snackBar);

                        if(error.indexOf("1033") != -1 || error.indexOf("2335") != -1){
                            QSqlQuery query;
                            QString str_query = "UPDATE comprobante SET estado_sunat='Activo'";
                            str_query += " WHERE id = '"+id+"'";
                            if(query.exec(str_query)){
                                //QMessageBox::warning(this, "Advertencia", "Response Code: "+str_responseCode
                                //                     + "\nDescripción: "+str_description, "Aceptar");
                            }
                        }
                        file.close();
                    }
                    file.setFileName(fileName_xml.c_str());
                    file.remove();
                    file.setFileName(fileName_zip.c_str());
                    file.remove();

                }else{

                    //cout << "ha sido enviado a la sunat" <<endl;

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

                        //qDebug()<<command<<endl;

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

                        //qDebug()<<command<<endl;
                        QProcess process;
                        process.start(command);
                        //QProcess::execute(command);
                        process.waitForFinished();
                    }
                    QDomDocument dom;
                    QFile file(QString("R-")+QString(fileName_xml.c_str()));
                    while (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
                    }

                    while (!dom.setContent(&file)) {
                    }
                    file.close();

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

                    {
                        QFile file(QString("R-")+QString(fileName_xml.c_str()));
                        if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
                            //QMessageBox::warning(this, "Advertencia", "Si abrio", "Aceptar");
                        }
                        QXmlStreamReader xml_reader(&file);

                        QString str_response = xml_reader.device()->readAll().toStdString().c_str();

                        //str_response = SYSTEM->replace_quotes_simple(str_response);
                        QSqlQuery query;
                        QString str_query = "UPDATE comprobante SET xmlResponse='"+str_response+"'";
                        str_query += " WHERE id = '"+id+"'";

                        //QMessageBox::warning(this, "Advertencia", str_response, "Aceptar");
                        if(query.exec(str_query)){
                            //QMessageBox::warning(this, "Advertencia", "Si copio", "Aceptar");
                        }else{
                            //QMessageBox::warning(this, "Advertencia", "No copio", "Aceptar");
                            return;
                        }
                        file.close();
                    }

                    int responseCode = str_responseCode.toInt();
                    if(responseCode >= 100 && responseCode <= 1999){
                        if(snackBar)
                            ui->statusBar->removeWidget(snackBar);
                        snackBar = new SnackBarInfo;
                        snackBar->set_data("Response Code: "+str_responseCode
                                           + "\nDescripción: "+str_description, ":/new/Iconos/exclamation.png");
                        ui->statusBar->addWidget(snackBar);
                        /*
                        QMessageBox::warning(this, "Advertencia", "Response Code: "+str_responseCode
                                             + "\nDescripción: "+str_description, "Aceptar");*/

                    }
                    if(responseCode >= 2000 && responseCode <= 3999){
                        QSqlQuery query;
                        QString str_query = "UPDATE comprobante SET estado_sunat='Anulado'";
                        str_query += " WHERE id = '"+id+"'";
                        if(query.exec(str_query)){
                            if(snackBar)
                                ui->statusBar->removeWidget(snackBar);
                            snackBar = new SnackBarInfo;
                            snackBar->set_data("Response Code: "+str_responseCode
                                               + "\nDescripción: "+str_description, ":/new/Iconos/exclamation.png");
                            ui->statusBar->addWidget(snackBar);
                            /*
                            QMessageBox::warning(this, "Advertencia", "Response Code: "+str_responseCode
                                                 + "\nDescripción: "+str_description, "Aceptar");
                                                 */
                        }

                    }
                    if(responseCode >= 4000){
                        QSqlQuery query;
                        QString str_query = "UPDATE comprobante SET estado_sunat='Observacion'";
                        str_query += " WHERE id = '"+id+"'";
                        if(query.exec(str_query)){
                            if(snackBar)
                                ui->statusBar->removeWidget(snackBar);
                            snackBar = new SnackBarInfo;
                            snackBar->set_data("Response Code: "+str_responseCode
                                               + "\nDescripción: "+str_description, ":/new/Iconos/info.png");
                            ui->statusBar->addWidget(snackBar);
                            /*
                            QMessageBox::warning(this, "Advertencia", "Response Code: "+str_responseCode
                                                 + "\nDescripción: "+str_description, "Aceptar");
                                                 */
                        }

                    }
                    if(responseCode == 0){
                        QSqlQuery query;
                        QString str_query = "UPDATE comprobante SET estado_sunat='Activo'";
                        str_query += " WHERE id = '"+id+"'";
                        if(query.exec(str_query)){
                            if(snackBar)
                                ui->statusBar->removeWidget(snackBar);
                            snackBar = new SnackBarInfo;
                            snackBar->set_data("Response Code: "+str_responseCode
                                               + "\nDescripción: "+str_description, ":/new/Iconos/info.png");
                            ui->statusBar->addWidget(snackBar);
                            /*
                            QMessageBox::warning(this, "Advertencia", "Response Code: "+str_responseCode
                                                 + "\nDescripción: "+str_description, "Aceptar");*/
                        }
                    }
                    file.setFileName(fileName_xml.c_str());
                    file.remove();
                    file.setFileName(fileName_zip.c_str());
                    file.remove();
                    file.setFileName(QString("R-")+fileName_xml.c_str());
                    file.remove();
                    //file.setFileName("res");
                    //file.remove();
                    //file.setFileName("res.zip");
                    //file.remove();
                }
            }catch(QException& ex){
                std::cout << "No answer at all: " << ex.what() << std::endl;
            }
        }else{
            if(snackBar)
                ui->statusBar->removeWidget(snackBar);
            snackBar = new SnackBarInfo;
            snackBar->set_data("YA PASARON TODOS LOS DOCUMENTOS", ":/new/Iconos/info.png");
            ui->statusBar->addWidget(snackBar);

        }
    }else{


    }
}
void ManagerSystem::on_thread_finished()
{
    ((QThread*)sender())->exit(0);
    delete sender();
}
