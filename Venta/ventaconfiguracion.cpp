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

void VentaConfiguracion::set_tipo(int tipo, QString serie, QString series_id)
{
    QSqlQuery query;
    QString str_query;
    QVector<QString> nombres;

    switch(tipo){
    case tipo_documento::REGISTRO_SIN_DOCUMENTO:{
        str_query = "SELECT config_hoja.nombre, X(config_hoja.point), Y(config_hoja.point), config_hoja.stylesheet FROM config_hoja";
        str_query += " WHERE config_hoja.series_id = "+series_id;

        VentaRegistroSinDoc* w = new VentaRegistroSinDoc;
        w->setAttribute(Qt::WA_DeleteOnClose);
        foreach(QObject* w, w->children()){
            if(w->isWidgetType()){
                if(typeid(*w) == typeid(QDateTimeEdit)){
                    nombres.push_back(w->objectName());
                }
                if(typeid(*w) == typeid(QDateEdit)){
                    nombres.push_back(w->objectName());
                }
                if(typeid(*w) == typeid(QLineEdit)){
                    nombres.push_back(w->objectName());
                }
                if(typeid(*w) == typeid(QTableWidget)){
                    nombres.push_back(w->objectName());
                }
            }
        }
        w->close();
    }break;
    case tipo_documento::BOLETA:{
        str_query = "SELECT config_hoja.nombre, X(config_hoja.point), Y(config_hoja.point), config_hoja.stylesheet FROM config_hoja";
        str_query += " WHERE config_hoja.series_id = "+series_id;

        VentaBoleta* w = new VentaBoleta;
        w->setAttribute(Qt::WA_DeleteOnClose);
        foreach(QObject* w, w->children()){
            if(w->isWidgetType()){
                if(typeid(*w) == typeid(QDateTimeEdit)){
                    nombres.push_back(w->objectName());
                }
                if(typeid(*w) == typeid(QDateEdit)){
                    nombres.push_back(w->objectName());
                }
                if(typeid(*w) == typeid(QLineEdit)){
                    nombres.push_back(w->objectName());
                }
                if(typeid(*w) == typeid(QTableWidget)){
                    nombres.push_back(w->objectName());
                }
            }
        }
        w->close();
    }break;
    case tipo_documento::FACTURA:{
        str_query = "SELECT config_hoja.nombre, X(config_hoja.point), Y(config_hoja.point), config_hoja.stylesheet FROM config_hoja";
        str_query += " WHERE config_hoja.series_id = "+series_id;

        VentaFactura* w = new VentaFactura;
        w->setAttribute(Qt::WA_DeleteOnClose);
        foreach(QObject* w, w->children()){
            if(w->isWidgetType()){
                if(typeid(*w) == typeid(QDateTimeEdit)){
                    nombres.push_back(w->objectName());
                }
                if(typeid(*w) == typeid(QDateEdit)){
                    nombres.push_back(w->objectName());
                }
                if(typeid(*w) == typeid(QLineEdit)){
                    nombres.push_back(w->objectName());
                }
                if(typeid(*w) == typeid(QTableWidget)){
                    nombres.push_back(w->objectName());
                }
            }
        }
        w->close();
    }break;
    case tipo_documento::NOTA_PEDIDO:{
        str_query = "SELECT config_hoja.nombre, X(config_hoja.point), Y(config_hoja.point), config_hoja.stylesheet FROM config_hoja";
        str_query += " WHERE config_hoja.series_id = "+series_id;

        VentaNotaPedido* w = new VentaNotaPedido;
        w->setAttribute(Qt::WA_DeleteOnClose);
        foreach(QObject* w, w->children()){
            if(w->isWidgetType()){
                if(typeid(*w) == typeid(QDateTimeEdit)){
                    nombres.push_back(w->objectName());
                }
                if(typeid(*w) == typeid(QDateEdit)){
                    nombres.push_back(w->objectName());
                }
                if(typeid(*w) == typeid(QLineEdit)){
                    nombres.push_back(w->objectName());
                }
                if(typeid(*w) == typeid(QTableWidget)){
                    nombres.push_back(w->objectName());
                }
            }
        }
        w->close();
    }break;
    case tipo_documento::GUIA_REMISION_REMITENTE:{
        str_query = "SELECT config_hoja.nombre, X(config_hoja.point), Y(config_hoja.point), config_hoja.stylesheet FROM config_hoja";
        str_query += " WHERE config_hoja.series_id = "+series_id;

        VentaGuiaRR* w = new VentaGuiaRR;
        w->setAttribute(Qt::WA_DeleteOnClose);
        foreach(QObject* w, w->children()){
            if(w->isWidgetType()){
                if(typeid(*w) == typeid(QDateTimeEdit)){
                    nombres.push_back(w->objectName());
                }
                if(typeid(*w) == typeid(QDateEdit)){
                    nombres.push_back(w->objectName());
                }
                if(typeid(*w) == typeid(QLineEdit)){
                    nombres.push_back(w->objectName());
                }
                if(typeid(*w) == typeid(QTableWidget)){
                    nombres.push_back(w->objectName());
                }
            }
        }
        w->close();
    }break;
    case tipo_documento::COTIZACION:{
        str_query = "SELECT config_hoja.nombre, X(config_hoja.point), Y(config_hoja.point), config_hoja.stylesheet FROM config_hoja";
        str_query += " WHERE config_hoja.series_id = "+series_id;

        VentaCotizacion* w = new VentaCotizacion;
        w->setAttribute(Qt::WA_DeleteOnClose);
        foreach(QObject* w, w->children()){
            if(w->isWidgetType()){
                if(typeid(*w) == typeid(QDateTimeEdit)){
                    nombres.push_back(w->objectName());
                }
                if(typeid(*w) == typeid(QDateEdit)){
                    nombres.push_back(w->objectName());
                }
                if(typeid(*w) == typeid(QLineEdit)){
                    nombres.push_back(w->objectName());
                }
                if(typeid(*w) == typeid(QTableWidget)){
                    nombres.push_back(w->objectName());
                }
            }
        }
        w->close();
    }break;
    case tipo_documento::NOTA_CREDITO:{
        str_query = "SELECT config_hoja.nombre, X(config_hoja.point), Y(config_hoja.point), config_hoja.stylesheet FROM config_hoja";
        str_query += " WHERE config_hoja.series_id = "+series_id;

        VentaNotaCredito* w = new VentaNotaCredito;
        w->setAttribute(Qt::WA_DeleteOnClose);
        foreach(QObject* w, w->children()){
            if(w->isWidgetType()){
                if(typeid(*w) == typeid(QDateTimeEdit)){
                    nombres.push_back(w->objectName());
                }
                if(typeid(*w) == typeid(QDateEdit)){
                    nombres.push_back(w->objectName());
                }
                if(typeid(*w) == typeid(QLineEdit)){
                    nombres.push_back(w->objectName());
                }
                if(typeid(*w) == typeid(QTableWidget)){
                    nombres.push_back(w->objectName());
                }
            }
        }
        w->close();
    }break;
    case tipo_documento::NOTA_DEBITO:{
        str_query = "SELECT config_hoja.nombre, X(config_hoja.point), Y(config_hoja.point), config_hoja.stylesheet FROM config_hoja";
        str_query += " WHERE config_hoja.series_id = "+series_id;

        VentaNotaDebito* w = new VentaNotaDebito;
        w->setAttribute(Qt::WA_DeleteOnClose);
        foreach(QObject* w, w->children()){
            if(w->isWidgetType()){
                if(typeid(*w) == typeid(QDateTimeEdit)){
                    nombres.push_back(w->objectName());
                }
                if(typeid(*w) == typeid(QDateEdit)){
                    nombres.push_back(w->objectName());
                }
                if(typeid(*w) == typeid(QLineEdit)){
                    nombres.push_back(w->objectName());
                }
                if(typeid(*w) == typeid(QTableWidget)){
                    nombres.push_back(w->objectName());
                }
            }
        }
        w->close();
    }break;
    }

    QWidget* widget = new QWidget(this);
    widget->setMinimumSize(width, height);
    widget->setStyleSheet(tr("background-color: rgb(255, 255, 255);"));

    widget->setAutoFillBackground(true);

    for(int i=0; i < nombres.length(); i++){
        int x = 0;
        int y = 0;

        QLabel *wordLabel = createDragLabel(nombres[i], this);
        QFont font;
        font.setBold(true);
        font.setPointSize(10);
        wordLabel->setFont(font);
        wordLabel->move(x, y);
        wordLabel->show();
        //wordLabel->setAttribute(Qt::WA_DeleteOnClose);      
        labels.push_back(wordLabel);
    }

    qDebug()<<str_query<<endl;
    if(query.exec(str_query)){
        int i=0;
        while(query.next()){
            int x = query.value(1).toInt();
            int y = query.value(2).toInt();
            QString stylesheet = query.value(3).toString();

            labels[i]->move(x, y);
            labels[i]->setStyleSheet(stylesheet);
            i++;
        }
    }else{

    }

    setAcceptDrops(true);
    this->setMinimumSize(width, height);
}

void VentaConfiguracion::mousePressEvent(QMouseEvent *event)
{
    press = event->pos();
    this->isPress = true;

    if ( event->button() == Qt::RightButton ) {
        if(typeid(*this->childAt(press)) == typeid(QWidget))
            return;
        if(typeid(*this->childAt(press)) == typeid(QLabel)){
            QLabel* label = (QLabel*)this->childAt(press);
            label->raise();
            qDebug()<<label->styleSheet()<<endl;
            if(label->styleSheet().compare("") == 0){
                label->setStyleSheet(tr("background-color: rgb(0, 255, 255);"));
            }else{
                label->setStyleSheet(tr(""));
            }
            qDebug()<<label->styleSheet()<<endl;
        }
    }
}
void VentaConfiguracion::mouseReleaseEvent(QMouseEvent *event)
{
    this->isPress = false;
    release = event->pos();
}

void VentaConfiguracion::mouseMoveEvent(QMouseEvent *event)
{
    if(isPress) {
        QPoint old_p = press;
        QPoint new_p = event->pos();
        if(!(new_p.x() >= 0 && new_p.x() < width && new_p.y() >= 0 && new_p.y() < height)){
            //qDebug()<<"old_p: "<<old_p<<endl;
            return;
        }        
        if(typeid(*this->childAt(old_p)) == typeid(QWidget))
            return;        
        if(typeid(*this->childAt(old_p)) == typeid(QLabel)){
            QLabel* label = (QLabel*)this->childAt(old_p);
            label->raise();
            QPoint move_p = new_p-(old_p-label->pos());
            if(new_p.x() >= 0 && new_p.x() < width && new_p.y() >= 0 && new_p.y() < height
                    && move_p.x() >= 0 && move_p.x() < width && move_p.y() >= 0 && move_p.y() < height){
                label->move(move_p);
                //qDebug()<<"move_p: "<<move_p<<endl;
                press = new_p;
            }
        }
    }

}
