#include "ventaconfiguracion.h"
#include "ui_ventaconfiguracion.h"

VentaConfiguracion::VentaConfiguracion(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VentaConfiguracion)
{
    ui->setupUi(this);

    width = 1500;
    height = 1500;

    isPress = false;
    isRelease = false;    

    this->installEventFilter(this);
}

VentaConfiguracion::~VentaConfiguracion()
{
    qDebug()<<"delete venta configuracion"<<endl;
    delete ui;
}
void VentaConfiguracion::set_size(int width, int height)
{
    this->width = width;
    this->height = height;
}

QVector<QLabel*> VentaConfiguracion::get_labels()
{
    return labels;
}

void VentaConfiguracion::set_tipo(int tipo, QString serie, QString series_id
                                  , QVector<QLabel*>& labels
                                  , int margen_fila, int margen_columna)
{
    QSqlQuery query;
    QString str_query;

    str_query = "SELECT config_hoja.nombre, X(config_hoja.point), Y(config_hoja.point), config_hoja.stylesheet, config_hoja.family, config_hoja.point_size, config_hoja.style_name, config_hoja.weight FROM config_hoja";
    str_query += " WHERE config_hoja.series_id = "+series_id;

    for(int i=0; i < labels.length(); i++){
        labels[i]->setParent(this);
        labels[i]->adjustSize();
        this->labels.push_back(labels[i]);
        selected_labels.push_back(false);
        this->labels[i]->setAttribute(Qt::WA_TransparentForMouseEvents, true);
        if(labels[i]->objectName().compare("tableWidget") == 0) {
            QTextDocument* doc = new QTextDocument(this);
            doc->setHtml(labels[i]->text());

            QTextCursor cursor(doc);            
            cursor.setPosition(1);
            QTextTable *table = cursor.currentTable();

            for(int i=0; i<table->rows()-1; i++) {
                for(int j=0; j<table->columns()-1; j++) {
                    QTextBlockFormat block_format;
                    block_format.setLineHeight(margen_fila, QTextBlockFormat::LineDistanceHeight);
                    block_format.setRightMargin(margen_columna);
                    table->cellAt(i, j).lastCursorPosition().setBlockFormat(block_format);
                }
            }

            QTextTableFormat format;
            format.setCellPadding(0);
            format.setCellSpacing(0);
            format.setBorder(0);
            table->setFormat(format);

            labels[i]->setText(doc->toHtml());
            labels[i]->adjustSize();
        }
    }

    qDebug()<<str_query<<endl;
    if(query.exec(str_query)){
        int i=0;
        while(query.next()){
            int x = query.value(1).toInt();
            int y = query.value(2).toInt();
            QString stylesheet = query.value(3).toString();
            QString family = query.value(4).toString();
            int pointSize = query.value(5).toInt();
            QString styleName = query.value(6).toString();
            int weight = query.value(7).toInt();

            this->labels[i]->move(x, y);
            this->labels[i]->setStyleSheet(stylesheet);

            this->labels[i]->text();
            QFont font;
            font.setFamily(family);
            font.setPointSize(pointSize);
            font.setStyleName(styleName);
            font.setWeight(weight);

            this->labels[i]->setFont(font);
            this->labels[i]->adjustSize();

            if(labels[i]->objectName().compare("tableWidget") == 0) {
                QTextDocument* doc = new QTextDocument(this);
                doc->setHtml(labels[i]->text());

                doc->setDefaultFont(font);

                QTextCursor cursor(doc);
                cursor.setPosition(1);
                cursor.select(QTextCursor::Document);
                QTextCharFormat format;
                format.setFont(font);                
                cursor.setCharFormat(format);

                labels[i]->setText(doc->toHtml());
                labels[i]->adjustSize();
            }

            //selected_labels[i] = false;

            i++;
        }
    }else{

    }

    setAcceptDrops(true);
}
void VentaConfiguracion::mouseDoubleClickEvent(QMouseEvent *event)
{    
    if ( event->button() == Qt::LeftButton ) {
        QLabel* label = labels[labels.length()-1];
        selected_labels[labels.length()-1] = !selected_labels[labels.length()-1];
        int shape = label->frameShape();
        if(shape == QLabel::Box) {
            label->setFrameShape(QLabel::NoFrame);
        }else{
            label->setFrameShape(QLabel::Box);
        }
        label->adjustSize();
        if(label->styleSheet().compare("") == 0){
            label->setStyleSheet(tr("background-color: rgb(0, 255, 255);"));
        }else{
            label->setStyleSheet(tr(""));
        }
    }
}

void VentaConfiguracion::mousePressEvent(QMouseEvent *event)
{
    press = event->pos();
    this->isPress = true;
    qDebug()<<"press: "<<press<<endl;
    int n = -1;
    foreach(QObject* obj, children()) {
        if(obj->isWidgetType()) {
            if(typeid(*obj) == typeid(QLabel)){
                QLabel* label = (QLabel*)obj;
                QPoint p = label->pos();
                QPoint q = label->pos() + QPoint(label->width(), label->height());
                if(press.x() >= p.x() && press.y() >= p.y() && press.x() <= q.x() && press.y() <= q.y()) {
                    for(int i=0; i<labels.length(); i++) {
                        if(labels[i] == label) {
                            if(i > n)
                                n = i;
                        }
                    }
                }
            }
        }
    }

    if ( event->button() == Qt::LeftButton ) {
        if(n != -1) {
            selected_labels[n] = !selected_labels[n];
            bool selected = selected_labels[n];
            selected_labels[n] = selected_labels[labels.length()-1];
            selected_labels[labels.length()-1] = selected;

            int shape = labels[n]->frameShape();
            if(shape == QLabel::Box) {
                labels[n]->setFrameShape(QLabel::NoFrame);
            }else{
                labels[n]->setFrameShape(QLabel::Box);
            }
            labels[n]->adjustSize();
            labels[n]->raise();
            QLabel* label = labels[n];
            labels[n] = labels[labels.length()-1];
            labels[labels.length()-1] = label;
            if((n+1) < labels.length())
                labels[n]->stackUnder(labels[n+1]);
        }
    }
    if ( event->button() == Qt::RightButton ) {
        if(n != -1) {
            QLabel* label = labels[n];

            bool ok;
            QFont font = QFontDialog::getFont(&ok);
            if(ok) {
                label->setFont(font);
                label->adjustSize();
                qDebug()<<font.family()<<endl;
                qDebug()<<font.pointSize()<<endl;
                qDebug()<<font.weight()<<endl;
                qDebug()<<font.styleName()<<endl;

                if(label->objectName().compare("tableWidget") == 0) {
                    QTextDocument* doc = new QTextDocument(this);
                    doc->setHtml(label->text());

                    doc->setDefaultFont(font);

                    QTextCursor cursor(doc);
                    cursor.setPosition(1);
                    cursor.select(QTextCursor::Document);
                    QTextCharFormat format;
                    format.setFont(font);
                    cursor.setCharFormat(format);

                    label->setText(doc->toHtml());
                    label->adjustSize();
                }
            }
        }
    }
}
void VentaConfiguracion::mouseReleaseEvent(QMouseEvent *event)
{
    this->isPress = false;
    release = event->pos();
    qDebug()<<"release: "<<release<<endl;
    if ( event->button() == Qt::LeftButton ) {

    }
}

void VentaConfiguracion::mouseMoveEvent(QMouseEvent *event)
{
    qDebug()<<"move"<<endl;
    if(isPress) {
        QPoint old_p = press;
        QPoint new_p = event->pos();
        qDebug()<<"new_p: "<<new_p<<endl;

        for(int i=0; i<selected_labels.length(); i++) {
            if(selected_labels[i]) {
                QLabel* label = labels[i];

                QPoint move_p = new_p-(old_p - label->pos());
                label->move(move_p);
                qDebug()<<"move_p: "<<move_p<<endl;
                press = new_p;
            }
        }
    }
}
