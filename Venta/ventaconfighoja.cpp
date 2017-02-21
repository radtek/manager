#include "ventaconfighoja.h"
#include "ui_ventaconfighoja.h"

VentaConfigHoja::VentaConfigHoja(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::VentaConfigHoja)
{
    ui->setupUi(this);

    width = 600;
    height = 400;
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
                                         , QVector<QString>& object_name, QVector<QString>& data)
{
    this->tipo = tipo;
    this->series_id = series_id;

    ui->widget->setMinimumSize(width, height);
    ui->widget->set_tipo(tipo, serie, series_id, object_name, data);

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
                    str_query_2 += ", '"+labels[i]->styleSheet()+"')";
                }

                if(labels.length() > 0){
                    str_query = "INSERT INTO config_hoja(series_id, nombre, point, stylesheet)VALUES";
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
                    str_query_2 += ", '"+labels[i]->styleSheet()+"')";
                }

                if(labels.length() > 0){
                    str_query = "DELETE FROM config_hoja WHERE series_id = "+series_id;
                    str_query += "&&END_QUERY&&";
                    str_query += "INSERT INTO config_hoja(series_id, nombre, point, stylesheet)VALUES";
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
