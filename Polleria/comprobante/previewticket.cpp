#include "previewticket.h"
#include "ui_previewticket.h"

PreviewTicket::PreviewTicket(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PreviewTicket)
{
    ui->setupUi(this);
}

PreviewTicket::~PreviewTicket()
{
    delete ui;
}
void PreviewTicket::set_cabecera(QString numero, QString fecha, QString nombre, QString pago)
{
    this->numero = numero;
    this->fecha = fecha;
    this->nombre = nombre;
    this->pago_item = pago;
}
void PreviewTicket::set_detalle(QString id)
{
    QString str_query = "";

    QSqlQuery query;

    str_query += "(SELECT pago, delivery, '', '' FROM comprobante WHERE id = "+id;
    str_query += ") UNION ALL (";
    str_query += "SELECT c_h_p.cantidad, p.nombre, c_h_p.precio, f.nombre  FROM comprobante c";
    str_query += " JOIN comprobante_has_producto c_h_p ON c_h_p.comprobante_id = c.id";
    str_query += " JOIN producto p ON p.id = c_h_p.producto_id";
    str_query += " JOIN familia f ON f.id = p.familia_id";
    str_query += " WHERE c.id = " + id;
    str_query += ")";

    qDebug()<<str_query<<endl;
    if(query.exec(str_query)){
        query.next();
        pago = query.value(0).toDouble();
        delivery = query.value(1).toDouble();
        while(query.next()){
            QString cantidad = QString().setNum(query.value(0).toDouble(), ' ', 1);
            QString nombre = query.value(1).toString();
            QString precio = QString().setNum(query.value(2).toDouble(), ' ', 1);
            QString familia = query.value(3).toString();

            QVector<QString> fila;
            fila << cantidad << nombre << precio << familia;
            detalle.push_back(fila);
        }
    }
}
void PreviewTicket::ver_documento()
{
    QString text = "<tt><center>";
    QString reimpresion = "********REIMPRESION********";
    SYSTEM->centerText(reimpresion, 42);

    reimpresion = "<b>" + reimpresion + "</b>";

    text += reimpresion;
    text += "<br>" + SYSTEM->centerText(QString("BILL MC QUACK"), 42);
    text += "<br>" + SYSTEM->centerText(QString("de Giancarlo Miguel Figueroa Nuñes"), 42);
    text += "<br>" + SYSTEM->centerText(QString("Coop. Daniel Alcides Carrion M-14"), 42);
    text += "<br>" + SYSTEM->centerText(QString("RUC: 10430252718"), 42);
    text += "<br>" + SYSTEM->centerText(QString("----------------------------------------"), 42);
    text += "<br>" + SYSTEM->centerText(QString("Ticket Boleta"), 42);
    text += "<br>" + SYSTEM->centerText(QString("Maq.Regist.No: FFGF006292"), 42);
    text += "<br>" + SYSTEM->centerText(QString("Autorizacion Sunat: 0053845106569"), 42);
    text += "<br>" + SYSTEM->centerText(QString("Ticket Nro. : 012-2-" + numero), 42);
    QString nombre = "<br>" + SYSTEM->centerText(QString("NOM CLI : " + this->nombre), 42);
    nombre.replace("\t", " ");
    nombre.replace("\n", " ");
    nombre.replace("\r", " ");
    text += nombre;
    QString str_fecha;
    QString str_hora;
    QStringList list = fecha.split(" ", QString::SkipEmptyParts);
    if(list.size() > 1){
        str_fecha =list[0];
        str_hora = list[1];
    }

    text += "<br>" + SYSTEM->centerText(QString("Fecha: " + str_fecha), 42);
    text += "<br>" + SYSTEM->centerText(QString("Hora: " + str_hora), 42);
    text += "<br>" + SYSTEM->centerText(QString("========================================"), 42);
    text += "<br>" + SYSTEM->centerText(QString("Cant  Descripcion                Importe"), 42);
    text += "<br>" + SYSTEM->centerText(QString("========================================"), 42);

    //SYSTEM->epson_lineFeed(out);
    double total = 0.0, igv = 0.18;
    for(int i = 0; i < detalle.size(); i++){
        QString cantidad = detalle[i][0];
        QString descripcion = detalle[i][1];
        double p_total = detalle[i][2].toDouble();
        QString str_p_total = QString().setNum(p_total, ' ', 1);

        SYSTEM->epson_just_descripcion(descripcion, 29),
        total += p_total;

        SYSTEM->justified(cantidad, 5);
        SYSTEM->justified(str_p_total, 6);

        text += "<br>" + SYSTEM->centerText(QString(cantidad + " " + descripcion + " " + str_p_total), 42);

        //SYSTEM->epson_lineFeed(out);
    }
    QString str_st = QString().setNum(total - (total * igv), ' ', 1);
    SYSTEM->justified(str_st, 6);
    QString str_i = QString().setNum(total * igv, ' ', 1);
    SYSTEM->justified(str_i, 6);
    QString str_t = QString().setNum(total, ' ', 1);
    SYSTEM->justified(str_t, 6);
    QString str_d = QString().setNum(delivery, ' ', 1);
    SYSTEM->justified(str_d, 6);
    QString str_v = QString().setNum(pago - total - delivery, ' ', 1);
    SYSTEM->justified(str_v, 6);
    QString str_sub_total = "Sub-Total: " + str_st;
    QString str_igv = "IGV: " + str_i;
    QString str_total = "Total: " + str_t;
    QString str_delivery = "Delivery: " + str_d;
    QString str_vuelto = "Vuelto: " + str_v;
    text += "<br>========================================";
    text += "<br>" + SYSTEM->rightText(QString(str_sub_total), 42);
    text += "<br>" + SYSTEM->rightText(QString(str_igv), 42);
    text += "<br>" + SYSTEM->rightText(QString(str_total), 42);
    text += "<br>" + SYSTEM->rightText(QString(str_delivery), 42);
    text += "<br>" + SYSTEM->rightText(QString(str_vuelto), 42);
    text += "<br>========================================";
    text += "<br>" + SYSTEM->centerText(QString(this->pago_item), 42);

    text += "</center></tt>";

    text.replace(" " , "&nbsp;");

    ui->textEdit->setHtml(text);    
}
void PreviewTicket::ver_documento_asa()
{
    QString text = "<tt><center>";
    QString reimpresion = "**REIMPRESION**";
    SYSTEM->centerText(reimpresion, 42);

    reimpresion = "<b>" + reimpresion + "</b>";

    text += reimpresion;
    text += "<br>" + SYSTEM->centerText(QString("BILL MC QUACK"), 42);
    text += "<br>" + SYSTEM->centerText(QString("de Patricia Cristal Guerra Guerra"), 42);
    text += "<br>" + SYSTEM->centerText(QString("RUC: 10702899457"), 42);
    text += "<br>" + SYSTEM->centerText(QString("LOS ANDES 503"), 42);
    text += "<br>" + SYSTEM->centerText(QString("A.S.A. - AREQUIPA - AREQUIPA"), 42);
    text += "<br>" + SYSTEM->centerText(QString("----------------------------------------"), 42);
    text += "<br>" + SYSTEM->centerText(QString("Ticket Boleta"), 42);

    text += "<br>" + SYSTEM->centerText(QString("Aut: 0053845087990  Delivery; 38 49 29"), 42);
    text += "<br>" + SYSTEM->centerText(QString("Ticket Nro.: " + numero), 42);
    text += "<br>" + SYSTEM->centerText(QString("Serie: 150230600349P"), 42);

    QString nombre = "<br>" + SYSTEM->centerText(QString("NOM CLI : " + this->nombre), 42);
    nombre.replace("\t", " ");
    nombre.replace("\n", " ");
    nombre.replace("\r", " ");
    text += nombre;

    QString str_fecha;
    QString str_hora;
    QStringList list = fecha.split(" ", QString::SkipEmptyParts);
    if(list.size() > 1){
        str_fecha =list[0];
        str_hora = list[1];
    }
    text += "<br>" + SYSTEM->centerText(QString("Fecha: " + str_fecha), 42);
    text += "<br>" + SYSTEM->centerText(QString("Hora: " + str_hora), 42);
    text += "<br>" + SYSTEM->centerText(QString("========================================"), 42);
    text += "<br>" + SYSTEM->centerText(QString("Cant  Descripcion                Importe"), 42);
    text += "<br>" + SYSTEM->centerText(QString("========================================"), 42);

    //SYSTEM->epson_lineFeed(out);
    double total = 0.0, igv = 0.18;
    for(int i = 0; i < detalle.size(); i++){
        QString cantidad = detalle[i][0];
        QString descripcion = detalle[i][1];
        double p_total = detalle[i][2].toDouble();
        QString str_p_total = QString().setNum(p_total, ' ', 1);

        SYSTEM->epson_just_descripcion(descripcion, 29),
        total += p_total;

        SYSTEM->justified(cantidad, 5);
        SYSTEM->justified(str_p_total, 6);

        text += "<br>" + SYSTEM->centerText(QString(cantidad + " " + descripcion + " " + str_p_total), 42);

        //SYSTEM->epson_lineFeed(out);
    }
    QString str_st = QString().setNum(total - (total * igv), ' ', 1);
    SYSTEM->justified(str_st, 6);
    QString str_i = QString().setNum(total * igv, ' ', 1);
    SYSTEM->justified(str_i, 6);
    QString str_t = QString().setNum(total, ' ', 1);
    SYSTEM->justified(str_t, 6);
    //QString str_v = QString().setNum(ui->doubleSpinBox_vuelto->value(), ' ', 1);
    //SYSTEM->justified(str_v, 6);
    SYSTEM->justified(str_t, 6);
    QString str_sub_total = "Sub-Total: " + str_st;
    QString str_igv = "IGV: " + str_i;
    QString str_total = "Total: " + str_t;
    //QString str_vuelto = "Vuelto: " + str_v;
    text += "<br>========================================";
    text += "<br>" + SYSTEM->rightText(QString(str_sub_total), 42);
    text += "<br>" + SYSTEM->rightText(QString(str_igv), 42);
    text += "<br>" + SYSTEM->rightText(QString(str_total), 42);
    text += "<br>========================================";
    text += "<br>" + SYSTEM->centerText(QString(this->pago_item), 42);

    text += "</center></tt>";

    text.replace(" " , "&nbsp;");

    ui->textEdit->setHtml(text);
}
void PreviewTicket::ver_documento_machelo()
{
    QString text = "<tt><center>";
    QString reimpresion = "********REIMPRESION********";
    SYSTEM->centerText(reimpresion, 42);

    reimpresion = "<b>" + reimpresion + "</b>";

    text += reimpresion;
    text += "<br>" + SYSTEM->centerText(QString("Numero: " + this->numero), 48);
    QString nombre = "<br>" + SYSTEM->centerText(QString("NOM CLI : " + this->nombre), 42);
    nombre.replace("\t", " ");
    nombre.replace("\n", " ");
    nombre.replace("\r", " ");
    text += nombre;
    QString str_fecha;
    QString str_hora;
    QStringList list = fecha.split(" ", QString::SkipEmptyParts);
    if(list.size() > 1){
        str_fecha =list[0];
        str_hora = list[1];
    }

    text += "<br>" + SYSTEM->centerText(QString("Fecha: " + str_fecha), 48);
    text += "<br>" + SYSTEM->centerText(QString("Hora: " + str_hora), 48);
    text += "<br>" + SYSTEM->centerText(QString("================================================"), 48);
    text += "<br>" + SYSTEM->centerText(QString("Cant      Unidad  Descripcion                   "), 48);
    text += "<br>" + SYSTEM->centerText(QString("                                   P.Total      "), 48);
    text += "<br>" + SYSTEM->centerText(QString("------------------------------------------------"), 48);

    //SYSTEM->epson_lineFeed(out);
    double total = 0.0, igv = 0.18;
    for(int i = 0; i < detalle.size(); i++){
        QString cantidad = detalle[i][0];
        QString unidad = "UND";
        QString descripcion = detalle[i][1];
        //QString peso = ui->tableWidget->item(i, 3)->text();
        //QString p_unit = ui->tableWidget->item(i, 4)->text();
        QString p_total = detalle[i][2];
        //double p_total = ui->tableWidget->item(i, 5)->text().toDouble();
        //QString str_p_total = QString().setNum(p_total, ' ', 1);

        SYSTEM->align_left(cantidad, 10);
        SYSTEM->align_left(unidad, 8);
        SYSTEM->align_left(descripcion, 55);
        //SYSTEM->align_left(peso, 13);
        //SYSTEM->align_left(p_unit, 12);
        SYSTEM->justified(p_total, 13);

        QString text_prod = cantidad + unidad
                + descripcion + "          "
                + p_total;

        text += "<br>" + SYSTEM->centerText(QString(text_prod.mid(0, 48)), 48);
        text += "<br>" + SYSTEM->centerText(QString(text_prod.mid(48, 48)), 48);

        total += p_total.toDouble();
        //SYSTEM->epson_lineFeed(out);
    }
    QString str_st = QString().setNum(total / (1.0 + igv), ' ', 1);
    SYSTEM->justified(str_st, 6);
    QString str_i = QString().setNum(total / (1.0 + igv) * igv, ' ', 1);
    SYSTEM->justified(str_i, 6);
    QString str_t = QString().setNum(total, ' ', 1);
    SYSTEM->justified(str_t, 6);
    QString str_d = QString().setNum(delivery, ' ', 1);
    SYSTEM->justified(str_d, 6);
    QString str_v = QString().setNum(pago - total - delivery, ' ', 1);
    SYSTEM->justified(str_v, 6);
    QString str_sub_total = "Sub-Total: " + str_st;
    QString str_igv = "IGV: " + str_i;
    QString str_total = "Total: " + str_t;
    QString str_delivery = "Delivery: " + str_d;
    QString str_vuelto = "Vuelto: " + str_v;
    text += "<br>========================================";
    text += "<br>" + SYSTEM->rightText(QString(str_sub_total), 48);
    text += "<br>" + SYSTEM->rightText(QString(str_igv), 48);
    text += "<br>" + SYSTEM->rightText(QString(str_total), 48);
    text += "<br>" + SYSTEM->rightText(QString("------------------------"), 48);
    text += "<br>" + SYSTEM->rightText(QString(str_delivery), 48);
    text += "<br>" + SYSTEM->rightText(QString(str_vuelto), 48);
    text += "<br>================================================";
    text += "<br>" + SYSTEM->centerText(QString(this->pago_item), 48);

    text += "<br>" + SYSTEM->centerText(QString(""), 48);
    text += "<br>" + SYSTEM->centerText(QString(""), 48);
    {
        text += "<br>" + SYSTEM->centerText(QString("BEBIDAS"), 48);
        text += "<br>" + SYSTEM->centerText(QString("================================================"), 48);
        text += "<br>" + SYSTEM->centerText(QString("Cant      Unidad  Descripcion                   "), 48);
        text += "<br>" + SYSTEM->centerText(QString("                                   P.Total      "), 48);
        text += "<br>" + SYSTEM->centerText(QString("------------------------------------------------"), 48);

        //SYSTEM->epson_lineFeed(out);
        double total = 0.0, igv = 0.18;
        for(int i = 0; i < detalle.size(); i++){
            QString categoria = detalle[i][3];
            qDebug()<<categoria<<endl;
            if(categoria.compare("bebidas", Qt::CaseInsensitive) != 0) continue;
            QString cantidad = detalle[i][0];
            QString unidad = "UND";
            QString descripcion = detalle[i][1];
            //QString peso = ui->tableWidget->item(i, 3)->text();
            //QString p_unit = ui->tableWidget->item(i, 4)->text();
            QString p_total = detalle[i][2];
            //double p_total = ui->tableWidget->item(i, 5)->text().toDouble();
            //QString str_p_total = QString().setNum(p_total, ' ', 1);

            SYSTEM->align_left(cantidad, 10);
            SYSTEM->align_left(unidad, 8);
            SYSTEM->align_left(descripcion, 55);
            //SYSTEM->align_left(peso, 13);
            //SYSTEM->align_left(p_unit, 12);
            SYSTEM->justified(p_total, 13);

            QString text_prod = cantidad + unidad
                    + descripcion + "          "
                    + p_total;

            text += "<br>" + SYSTEM->centerText(QString(text_prod.mid(0, 48)), 48);
            text += "<br>" + SYSTEM->centerText(QString(text_prod.mid(48, 48)), 48);

            total += p_total.toDouble();
            //SYSTEM->epson_lineFeed(out);
        }

        QString str_t = QString().setNum(total, ' ', 1);
        SYSTEM->justified(str_t, 6);
        QString str_total = "Total: " + str_t;

        text += "<br>========================================";
        text += "<br>" + SYSTEM->rightText(QString(str_total), 48);
        text += "<br>" + SYSTEM->rightText(QString("------------------------"), 48);
        text += "<br>================================================";
    }

    text += "</center></tt>";

    text.replace(" " , "&nbsp;");

    ui->textEdit->setHtml(text);
}
void PreviewTicket::on_pushButton_salir_clicked()
{
    this->close();
}

void PreviewTicket::on_pushButton_imprimir_clicked()
{
    //print_caja();
    print_machelo();
    //print_frap();
}

void PreviewTicket::showEvent(QShowEvent *event)
{
    event->accept();

    //ui->textEdit->setText("");
}

void PreviewTicket::print_caja()
{
    QFile file("print_caja_reimpresion.dat");
    if (!file.open(QFile::WriteOnly)) {
        qDebug() << "Could not open file for writing";
        return;
    }
    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_4_8);

    //out << (qint64)0x1D211000;
    //out << (qint64)0x1D210100;
    QString reimpresion = "*********REIMPRESION*********";
    SYSTEM->centerText(reimpresion, 40);
    SYSTEM->epson_printText(out, reimpresion);

    QVector<QString> centerTexts;
    centerTexts.push_back("BILL MC QUACK");
    centerTexts.push_back("de Giancarlo Miguel Figueroa Nuñes");
    centerTexts.push_back("Coop. Daniel Alcides Carrion M-14");
    centerTexts.push_back("RUC: 10430252718");
    centerTexts.push_back("----------------------------------------");
    centerTexts.push_back("Ticket Boleta");
    centerTexts.push_back("Maq.Regist.No: FFGF006292");
    centerTexts.push_back("Autorizacion Sunat: 0053845106569");
    centerTexts.push_back("Ticket Nro.: 012-2-" + numero);
    QString nombre = "NOM CLI : " + this->nombre;
    nombre.replace("\t", " ");
    nombre.replace("\n", " ");
    nombre.replace("\r", " ");
    while((nombre.length() % 40) == 0){
        QString temp = nombre.mid(0, 40);
        centerTexts.push_back(temp);
        nombre.remove(0, 40);
    }

    QString str_fecha;
    QString str_hora;
    QStringList list = fecha.split(" ", QString::SkipEmptyParts);
    if(list.size() > 1){
        str_fecha =list[0];
        str_hora = list[1];
    }
    centerTexts.push_back(QString("Fecha: " + str_fecha));
    centerTexts.push_back(QString("Hora: " + str_hora));
    centerTexts.push_back("========================================");
    centerTexts.push_back("Cant  Descripcion                Importe");
    centerTexts.push_back("========================================");

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
    for(int i = 0; i < detalle.size(); i++){
        QString cantidad = detalle[i][0];
        QString descripcion = detalle[i][1];
        double p_total = detalle[i][2].toDouble();
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
    //QString str_v = QString().setNum(ui->doubleSpinBox_vuelto->value(), ' ', 1);
    //SYSTEM->justified(str_v, 6);
    SYSTEM->justified(str_t, 6);
    QString str_sub_total = "Sub-Total: " + str_st;
    QString str_igv = "IGV: " + str_i;
    QString str_total = "Total: " + str_t;
    //QString str_vuelto = "Vuelto: " + str_v;
    QVector<QString> totales;
    totales.push_back("========================================");
    totales.push_back(str_sub_total);
    totales.push_back(str_igv);
    totales.push_back(str_total);
    totales.push_back("========================================");
    QString forma_de_pago = QString("Forma de Pago: " + pago_item.toUpper());
    totales.push_back(SYSTEM->centerText(forma_de_pago, 40));
    totales.push_back(" ");
    //totales.push_back(str_vuelto);
    //totales.push_back(" ");
    //totales.push_back(QString() + "*** Clave Wifi: " + ui->lineEdit_clave->text() + " ***");

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
    //SYSTEM->epson_lineFeed(out);
    //SYSTEM->epson_lineFeed(out);
    //SYSTEM->epson_lineFeed(out);
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
void PreviewTicket::print_frap()
{
    QFile file("print_caja_frap_reimpresion.dat");
    if (!file.open(QFile::WriteOnly)) {
        qDebug() << "Could not open file for writing";
        return;
    }
    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_4_8);

    QVector<QString> centerTexts;
    centerTexts.push_back("*********REIMPRESION*********");
    centerTexts.push_back("BILL MC QUACK");
    centerTexts.push_back("de Giancarlo Miguel Figueroa Nuñes");
    centerTexts.push_back("Coop. Daniel Alcides Carrion M-14");
    centerTexts.push_back("RUC: 10430252718");
    centerTexts.push_back("----------------------------------------");
    centerTexts.push_back("Ticket Boleta");
    //centerTexts.push_back("Maq.Regist.No: ");
    centerTexts.push_back("Aut: 0053845087990  Delivery; 38 49 29");
    centerTexts.push_back("Ticket Nro.: " + numero);
    centerTexts.push_back("Serie: 150230600349P");
    QString nombre = "NOM CLI : " + this->nombre;
    nombre.replace("\t", " ");
    nombre.replace("\n", " ");
    nombre.replace("\r", " ");
    if(nombre.length() > 42){
        centerTexts.push_back(nombre.mid(0, 42));
        centerTexts.push_back(nombre.mid(42, nombre.length() - 42));
    }else{
        centerTexts.push_back(nombre);
    }

    QString str_fecha;
    QString str_hora;
    QStringList list = fecha.split(" ", QString::SkipEmptyParts);
    if(list.size() > 1){
        str_fecha =list[0];
        str_hora = list[1];
    }
    centerTexts.push_back("Fecha: " + str_fecha);
    centerTexts.push_back("Hora: " + str_hora);
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
    for(int i = 0; i < detalle.size(); i++){
        QString cantidad = detalle[i][0];
        QString descripcion = detalle[i][1];
        double p_total = detalle[i][2].toDouble();
        QString str_p_total = QString().setNum(p_total, ' ', 1);

        SYSTEM->epson_just_descripcion(descripcion, 29),
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
    //QString str_v = QString().setNum(ui->doubleSpinBox_vuelto->value(), ' ', 1);
    //SYSTEM->justified(str_v, 6);
    SYSTEM->justified(str_t, 6);
    //QString str_sub_total = "Sub-Total: " + str_st;
    //QString str_igv = "IGV: " + str_i;
    QString str_total = "Total: " + str_t;
    //QString str_vuelto = "Vuelto: " + str_v;
    QVector<QString> totales;
    totales.push_back("----------------------------------------");
    //totales.push_back(str_sub_total);
    //totales.push_back(str_igv);
    totales.push_back(str_total);
    totales.push_back("----------------------------------------");
    //totales.push_back(str_vuelto);
    QString forma_de_pago = QString("Forma de Pago:" + pago_item.toUpper());
    totales.push_back(SYSTEM->centerText(forma_de_pago, 42));
    totales.push_back(" ");
    //QString clave_wifi = "*** Clave Wifi: " + clave + " ***";
    //totales.push_back(SYSTEM->centerText(clave_wifi));

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
    qDebug() << cmp_release << endl;
    if(cmp_release.compare("/release") == 0){
        str_rel = "/release";
        qDebug() << str_rel << endl;
    }

    app_dir = app_dir.mid(0, app_dir.length() - str_rel.length());
    app_dir.replace("/","\\");
    app_dir = "\"" + app_dir + "\\" + file.fileName() + "\"";

    QString lpt_print = "LPT1";

    QString command = "copy /b " + app_dir
                        + " " + lpt_print + " < nul";

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
    QProcess::execute(command);
}
void PreviewTicket::print_caja_asa()
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
    centerTexts.push_back("Ticket Nro.: " + this->numero);
    centerTexts.push_back("Serie: 150230600349P");
    QString nombre = "NOM CLI : " + this->nombre;
    nombre.replace("\t", " ");
    nombre.replace("\n", " ");
    nombre.replace("\r", " ");
    if(nombre.length() > 40){
        centerTexts.push_back(nombre.mid(0, 40));
        centerTexts.push_back(nombre.mid(40, nombre.length() - 40));
    }else{
        centerTexts.push_back(nombre);
    }

    QString str_fecha;
    QString str_hora;
    QStringList list = fecha.split(" ", QString::SkipEmptyParts);
    if(list.size() > 1){
        str_fecha =list[0];
        str_hora = list[1];
    }
    centerTexts.push_back(QString("Fecha: " + str_fecha));
    centerTexts.push_back(QString("Hora: " + str_hora));
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
    for(int i = 0; i < detalle.size(); i++){
        QString descripcion = detalle[i][0];
        QString cantidad = detalle[i][1];
        double p_total = detalle[i][2].toDouble();
        QString str_p_total = QString().setNum(p_total, ' ', 1);

        SYSTEM->epson_just_descripcion(descripcion, 27),
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
    //QString str_v = QString().setNum(ui->doubleSpinBox_vuelto->value(), ' ', 1);
    //SYSTEM->justified(str_v, 6);
    //SYSTEM->justified(str_t, 6);
    QString str_sub_total = "Sub-Total: " + str_st;
    QString str_igv = "IGV: " + str_i;
    QString str_total = "Total: " + str_t;
    //QString str_vuelto = "Vuelto: " + str_v;
    QVector<QString> totales;
    totales.push_back("----------------------------------------");
    totales.push_back(str_sub_total);
    totales.push_back(str_igv);
    totales.push_back(str_total);
    //totales.push_back("----------------------------------------");
    //totales.push_back(str_vuelto);
    QString forma_de_pago = QString("Forma de Pago: " + pago_item.toUpper());
    totales.push_back(SYSTEM->centerText(forma_de_pago, 40));
    totales.push_back(" ");
    //QString clave_wifi = "*** Clave Wifi: " + ui->lineEdit_clave->text() + " ***";
    //totales.push_back(SYSTEM->centerText(clave_wifi));

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
void PreviewTicket::print_machelo()
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


    QFile file("print_caja.dat");
    if (!file.open(QFile::WriteOnly)) {
        qDebug() << "Could not open file for writing";
        return;
    }
    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_4_8);

    SYSTEM->insertImage(out, "npedido.png");
    /*
    for(int j = 0; j < 4; j++){
        out<<(qint64)0x0A;
    }*/
    //qDebug()<<getchar()<<endl;
    SYSTEM->epson_lineFeed(out);


    QVector<QString> centerTexts;
/*
    if(str_tipo_item == str_npedido)
        centerTexts.push_back("Ticket Nota de Pedido");

    if(str_tipo_item == str_salida)
        centerTexts.push_back("Ticket Salida");
*/
    centerTexts.push_back("Numero: " + this->numero);
    centerTexts.push_back("                                                ");
    QString nombre = "NOM CLI : " + this->nombre;
    if(nombre.length() > 48){
        int recorrido = 0;
        while(recorrido < nombre.length()) {
            centerTexts.push_back(nombre.mid(recorrido, 48));
            recorrido += 48;
        }

    }else{
        centerTexts.push_back(nombre);
    }


    centerTexts.push_back("                                                ");
    QString str_fecha;
    QString str_hora;
    QStringList list = fecha.split(" ", QString::SkipEmptyParts);
    if(list.size() > 1){
        str_fecha =list[0];
        str_hora = list[1];
    }
    centerTexts.push_back(QString("Fecha: " + str_fecha));
    centerTexts.push_back(QString("Hora: " + str_hora));
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
    double total = 0.0, igv = 0.18;
    for(int i = 0; i < detalle.size(); i++){
        QString cantidad = detalle[i][0];
        QString unidad = "UND";
        QString descripcion = detalle[i][1];
        //QString peso = ui->tableWidget->item(i, 3)->text();
        //QString p_unit = ui->tableWidget->item(i, 4)->text();
        QString p_total = detalle[i][2];
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
    QString str_st = QString().setNum(total - (total * igv), ' ', 1);
    SYSTEM->justified(str_st, 6);
    QString str_i = QString().setNum(total * igv, ' ', 1);
    SYSTEM->justified(str_i, 6);
    QString str_t = QString().setNum(total, ' ', 2);
    SYSTEM->justified(str_t, 6);
    //QString str_v = QString().setNum(ui->doubleSpinBox_vuelto->value(), ' ', 1);
    //SYSTEM->justified(str_v, 6);
    SYSTEM->justified(str_t, 13);
    QString str_sub_total = "Sub-Total: " + str_st;
    QString str_igv = "IGV: " + str_i;
    QString str_total = "Total: " + str_t;
    //QString str_vuelto = "Vuelto: " + str_v;
    QVector<QString> totales;
    totales.push_back("------------------------");
    totales.push_back(str_sub_total);
    totales.push_back(str_igv);
    totales.push_back(str_total);
    totales.push_back("------------------------");
    //totales.push_back(str_vuelto);
    QString forma_de_pago = QString("Forma de Pago:" + pago_item.toUpper());
    totales.push_back(SYSTEM->centerText(forma_de_pago, 48));
    totales.push_back(" ");
    //QString clave_wifi = "*** Clave Wifi: " + clave + " ***";
    //totales.push_back(SYSTEM->centerText(clave_wifi));

    SYSTEM->rightTexts(totales, 48);

    SYSTEM->epson_lineFeed(out);

    /*
    for(int i = 0; i < totales.size(); i++){
        SYSTEM->epson_printText(out, totales[i]);
        //SYSTEM->epson_lineFeed(out);
    }*/
    //SYSTEM->epson_lineFeed(out);

    //SYSTEM->insertImage(out, "footer.png");

    SYSTEM->epson_lineFeed(out);
    //SYSTEM->epson_printText(out, SYSTEM->centerText(QString("PAGOS LLAMAR A: CRISS: 967252119")));
    //SYSTEM->epson_lineFeed(out);
    SYSTEM->epson_lineFeed(out);
    //SYSTEM->epson_printText(out, SYSTEM->centerText(QString("")));
    SYSTEM->epson_printText(out, SYSTEM->centerText(QString("\"¡Gracias por su preferencia!\""), 48));

    SYSTEM->epson_lineFeed(out);
    SYSTEM->epson_lineFeed(out);
    SYSTEM->epson_lineFeed(out);
    SYSTEM->epson_lineFeed(out);
    SYSTEM->epson_lineFeed(out);


    SYSTEM->epson_paperCut(out);

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
    QString lpt_print = "\"\\\\localhost\\POS-80-Series\"";
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
void PreviewTicket::on_textEdit_textChanged()
{
    QString text = ui->textEdit->toHtml();
    qDebug() << text << endl;
}
