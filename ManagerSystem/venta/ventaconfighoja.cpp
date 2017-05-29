#include "ventaconfighoja.h"
#include "ui_ventaconfighoja.h"

VentaConfigHoja::VentaConfigHoja(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::VentaConfigHoja)
{
    ui->setupUi(this);

    width = 600;
    height = 400;

    QString printerName = QPrinterInfo::defaultPrinterName();
    ui->label_printerName->setText(printerName);
}

VentaConfigHoja::~VentaConfigHoja()
{
    qDebug()<<"delete venta config hoja"<<endl;
    delete ui;
}

void VentaConfigHoja::set_widget_previous(QWidget *widget_previous)
{
    this->widget_previous = widget_previous;
}

void VentaConfigHoja::set_tipo_documento(int tipo, QString serie, QString series_id
                                         , QVector<QLabel*>& labels
                                         , int width, int height, int margen_fila, int margen_columna)
{
    this->tipo = tipo;
    this->series_id = series_id;

    ui->spinBox_width->setValue(width);
    ui->spinBox_height->setValue(height);

    ui->spinBox_margen_fila->setValue(margen_fila);
    ui->spinBox_margen_columna->setValue(margen_columna);

    ui->widget->setMinimumSize(width, height);
    ui->widget->set_tipo(tipo, serie, series_id, labels, margen_fila, margen_columna);

    ui->widget->adjustSize();
    this->adjustSize();

    ui->label_serie->setText(serie);
}
void VentaConfigHoja::set_data(QVector<QVector<QString> > data)
{
    this->data = data;
}

bool VentaConfigHoja::guardar()
{
    QString str_query;
    str_query = "SELECT IF(EXISTS(";
    str_query += "SELECT COUNT(*) FROM config_hoja";
    str_query += " WHERE config_hoja.series_id = "+series_id;
    str_query += " LIMIT 1), 1, 0)";

    QSqlQuery query;
    if(query.exec(str_query)){
        if(query.next()){
            int exists = query.value(0).toInt();
            QString str_query;
            if(exists == 0){
                QString str_query_2;
                QVector<QLabel*> labels = ui->widget->get_labels();

                for(int i = 0; i < labels.length(); i++){
                    str_query_2 += ",("+series_id;
                    str_query_2 += ", '"+labels[i]->objectName()+"'";
                    str_query_2 += ", point("+QString().setNum(labels[i]->x())+", "+QString().setNum(labels[i]->y())+")";
                    str_query_2 += ", '"+labels[i]->styleSheet()+"'";
                    str_query_2 += ", '"+labels[i]->font().family()+"'";
                    str_query_2 += ", '"+QString().setNum(labels[i]->font().pointSize())+"'";
                    str_query_2 += ", '"+labels[i]->font().styleName()+"'";
                    str_query_2 += ", '"+QString().setNum(labels[i]->font().weight())+"')";
                }

                if(labels.length() > 0){
                    str_query += "UPDATE series SET";
                    str_query += " width = "+QString().setNum(ui->spinBox_width->value());
                    str_query += ", height = "+QString().setNum(ui->spinBox_height->value());
                    str_query += ", margen_fila = "+QString().setNum(ui->spinBox_margen_fila->value());
                    str_query += ", margen_columna = "+QString().setNum(ui->spinBox_margen_columna->value());
                    str_query += " WHERE id = "+series_id;
                    str_query += "&&END_QUERY&&";
                    str_query += "INSERT INTO config_hoja(series_id, nombre, point, stylesheet, family, point_size, style_name, weight)VALUES";
                    str_query_2 = str_query_2.remove(0,1);
                    str_query += str_query_2;
                    str_query += "&&END_QUERY&&";
                }
            }else{
                QString str_query_2;
                QVector<QLabel*> labels = ui->widget->get_labels();

                for(int i = 0; i < labels.length(); i++){
                    str_query_2 += ",("+series_id;
                    str_query_2 += ", '"+labels[i]->objectName()+"'";
                    str_query_2 += ", point("+QString().setNum(labels[i]->x())+", "+QString().setNum(labels[i]->y())+")";
                    str_query_2 += ", '"+labels[i]->styleSheet()+"'";
                    str_query_2 += ", '"+labels[i]->font().family()+"'";
                    str_query_2 += ", '"+QString().setNum(labels[i]->font().pointSize())+"'";
                    str_query_2 += ", '"+labels[i]->font().styleName()+"'";
                    str_query_2 += ", '"+QString().setNum(labels[i]->font().weight())+"')";
                }

                if(labels.length() > 0){
                    str_query += "UPDATE series SET";
                    str_query += " width = "+QString().setNum(ui->spinBox_width->value());
                    str_query += ", height = "+QString().setNum(ui->spinBox_height->value());
                    str_query += ", margen_fila = "+QString().setNum(ui->spinBox_margen_fila->value());
                    str_query += ", margen_columna = "+QString().setNum(ui->spinBox_margen_columna->value());
                    str_query += " WHERE id = "+series_id;
                    str_query += "&&END_QUERY&&";
                    str_query += "DELETE FROM config_hoja WHERE series_id = "+series_id;
                    str_query += "&&END_QUERY&&";
                    str_query += "INSERT INTO config_hoja(series_id, nombre, point, stylesheet, family, point_size, style_name, weight)VALUES";
                    str_query_2 = str_query_2.remove(0,1);
                    str_query += str_query_2;
                    str_query += "&&END_QUERY&&";
                }
            }

            QSqlQuery query;
            SYSTEM->multiple_query(str_query);
            qDebug()<<str_query<<endl;
            if(query.exec(str_query)){
                return true;
            }else{
                return false;
            }
        }else{

        }
    }
    return false;

}

void VentaConfigHoja::on_pushButton_guardar_clicked()
{
    int ret = QMessageBox::warning(this, "Advertencia", "¿Desea guardar los puntos?", "Si", "No");
    switch(ret){
    case 0:{
        if(guardar()){
            QMessageBox::information(this, "Información", "Se guardo con éxito.");
            this->setAttribute(Qt::WA_DeleteOnClose);
            SYSTEM->change_center_w(this, widget_previous);
        }else{
            QMessageBox::critical(this, "Error", "No se pudieron guardar los puntos.");
        }
        return;
    }break;
    case 1:{

    }
    }
}

void VentaConfigHoja::on_pushButton_salir_clicked()
{
    this->setAttribute(Qt::WA_DeleteOnClose);
    SYSTEM->change_center_w(this, widget_previous);
}

void VentaConfigHoja::on_pushButton_imprimir_clicked()
{    
    QString filename = "file.pdf";
    QPdfWriter writer(filename);
    //writer.setPageSize(QPagedPaintDevice::A4);
    QPageSize pageSize(QSize(ui->spinBox_width->value(), ui->spinBox_height->value()));

    writer.setPageSize((const QPageSize)pageSize);

    QVector<QLabel*> labels = ui->widget->get_labels();
    QPainter painter(&writer);
    for(int i=0; i < labels.size(); i++) {
        qDebug()<<labels[i]->objectName()<<endl;
        if(labels[i]->objectName().compare("tableWidget") == 0) {
            QTextDocument* doc = new QTextDocument(this);
            doc->setHtml(labels[i]->text());

            QTextCursor cursor(doc);
            cursor.setPosition(1);
            QTextTable *table = cursor.currentTable();

            QVector<int> max_xs;
            QVector<int> max_ys;
            QVector<QVector<QString> > texts;

            for(int ii=0; ii<table->rows(); ii++) {
                texts.push_back(QVector<QString>());
                for(int j=0; j<table->columns(); j++) {
                    texts[ii].push_back("");
                }
            }
            for(int ii=0; ii<table->rows(); ii++) {
                max_ys.push_back(0);
            }
            for(int j=0; j<table->columns(); j++) {
                max_xs.push_back(0);
            }

            int y_space = 0;
            int margin_fila = 0;

            for(int ii=0; ii<table->rows(); ii++) {
                int x_space = 0;
                int temp_y_space = 0;
                int temp_margin_fila = 0;
                int margin_columna = 0;
                for(int j=0; j<table->columns(); j++) {
                    cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor, 1);
                    QString text = cursor.selectedText();
                    cursor.movePosition(QTextCursor::NextCell, QTextCursor::MoveAnchor, 1);

                    QFont font;
                    font.setFamily(labels[i]->font().family());
                    font.setPointSize(labels[i]->font().pointSize());
                    font.setWeight(labels[i]->font().weight());
                    font.setStyleName(labels[i]->font().styleName());
                    painter.setFont(font);

                    int x = (labels[i]->x() + x_space + margin_columna)*16.67226890756303;

                    QLabel* label = new QLabel(text);
                    label->setFont(font);
                    label->adjustSize();
                    x_space += label->width();
                    margin_columna += table->cellAt(ii, j).firstCursorPosition().blockFormat().rightMargin();

                    if(label->height() > temp_y_space)
                        temp_y_space = label->height();

                    delete label;

                    int lineHeight = table->cellAt(ii, j).firstCursorPosition().blockFormat().lineHeight();
                    if(lineHeight > temp_margin_fila)
                        temp_margin_fila = lineHeight;

                    if(x > max_xs[j])
                        max_xs[j] = x;

                    texts[ii][j] = text;
                }
                int y = (labels[i]->y() + y_space + margin_fila)*16.67226890756303;
                if(y > max_ys[ii])
                    max_ys[ii] = y;
                y_space += temp_y_space;
                margin_fila += temp_margin_fila;
            }

            for(int ii=0; ii<table->rows(); ii++) {
                for(int j=0; j<table->columns(); j++) {
                    QFont font;
                    font.setFamily(labels[i]->font().family());
                    font.setPointSize(labels[i]->font().pointSize());
                    font.setWeight(labels[i]->font().weight());
                    font.setStyleName(labels[i]->font().styleName());
                    painter.setFont(font);

                    painter.drawText(QPoint(max_xs[j], max_ys[ii]), texts[ii][j]);
                }
            }
        }else{
            QFont font;
            font.setFamily(labels[i]->font().family());
            font.setPointSize(labels[i]->font().pointSize());
            font.setWeight(labels[i]->font().weight());
            font.setStyleName(labels[i]->font().styleName());
            painter.setFont(font);

            int x = labels[i]->x()*16.67226890756303;
            int y = labels[i]->y()*16.67226890756303;

            qDebug()<<x<<endl;
            qDebug()<<y<<endl;

            QString text = labels[i]->text();

            painter.drawText(QPoint(x, y), text);
        }
    }
    painter.end();

    QDir dir(QCoreApplication::applicationDirPath());

    QStringList list = dir.path().split("/", QString::SkipEmptyParts);

    QString str_dir = "";
    for(int i=0; i<list.length()-1; i++) {
        str_dir += list[i] + "/";
    }
    QString fileName = str_dir + "file.pdf";

    qDebug()<<fileName<<endl;


    QProcess *myProcess = new QProcess(this);
    QString command = "\"C:/Program Files/SumatraPDF/SumatraPDF.exe\"";
    QStringList arguments = QStringList()
            << fileName << "-silent" << "-print-to-default"

        << "--execute=\"source " + fileName + "\"";
    //myProcess->waitForFinished(0);
    //myProcess->setStandardInputFile(fileName);

    //connect(myProcess, SIGNAL(started())
        //, this, SLOT(on_myProccess_started()));
    //connect(myProcess, SIGNAL(finished(int, QProcess::ExitStatus))
        //, this, SLOT(on_myProccess_finished(int, QProcess::ExitStatus)));


    qDebug() << command << endl;//<< arguments << endl;
    myProcess->start(command, arguments);    


}

void VentaConfigHoja::on_spinBox_width_valueChanged(int arg1)
{
    ui->widget->setMinimumWidth(arg1);
    ui->widget->setMaximumWidth(arg1);

    ui->widget->adjustSize();
    this->adjustSize();
}

void VentaConfigHoja::on_spinBox_height_valueChanged(int arg1)
{
    ui->widget->setMinimumHeight(arg1);
    ui->widget->setMaximumHeight(arg1);

    ui->widget->adjustSize();
    this->adjustSize();
}

void VentaConfigHoja::on_spinBox_margen_fila_valueChanged(int arg1)
{
    QVector<QLabel*> labels = ui->widget->get_labels();
    for(int i=0; i < labels.length(); i++){
        if(labels[i]->objectName().compare("tableWidget") == 0) {
            QTextDocument* doc = new QTextDocument(this);
            doc->setHtml(labels[i]->text());

            QTextCursor cursor(doc);
            cursor.setPosition(1);
            QTextTable *table = cursor.currentTable();

            for(int i=0; i<table->rows()-1; i++) {
                for(int j=0; j<table->columns()-1; j++) {
                    QTextBlockFormat block_format;
                    block_format.setLineHeight(arg1, QTextBlockFormat::LineDistanceHeight);
                    block_format.setRightMargin(ui->spinBox_margen_columna->value());
                    table->cellAt(i, j).lastCursorPosition().setBlockFormat(block_format);
                }
            }

            QTextTableFormat format;
            format.setCellPadding(0);
            format.setCellSpacing(0);
            format.setBorder(0);
            table->setFormat(format);

            qDebug()<<doc->toHtml()<<endl;
            labels[i]->setText(doc->toHtml());
            labels[i]->adjustSize();

            break;
        }
    }
}

void VentaConfigHoja::on_spinBox_margen_columna_valueChanged(int arg1)
{
    QVector<QLabel*> labels = ui->widget->get_labels();
    for(int i=0; i < labels.length(); i++){
        if(labels[i]->objectName().compare("tableWidget") == 0) {
            QTextDocument* doc = new QTextDocument(this);
            doc->setHtml(labels[i]->text());

            QTextCursor cursor(doc);
            cursor.setPosition(1);
            QTextTable *table = cursor.currentTable();

            for(int i=0; i<table->rows()-1; i++) {
                for(int j=0; j<table->columns()-1; j++) {
                    QTextBlockFormat block_format;
                    block_format.setLineHeight(ui->spinBox_margen_fila->value(), QTextBlockFormat::LineDistanceHeight);
                    block_format.setRightMargin(arg1);
                    table->cellAt(i, j).lastCursorPosition().setBlockFormat(block_format);
                }
            }

            QTextTableFormat format;
            format.setCellPadding(0);
            format.setCellSpacing(0);
            format.setBorder(0);
            table->setFormat(format);

            qDebug()<<doc->toHtml()<<endl;
            labels[i]->setText(doc->toHtml());
            labels[i]->adjustSize();

            break;
        }
    }
}


