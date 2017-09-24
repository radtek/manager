#include "plato.h"
#include "ui_plato.h"

Plato::Plato(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Plato)
{
    ui->setupUi(this);

    is_eliminar = false;
    /*
    QString str_regExp_ruc = "[a-zA-Z "+QString(char(160))
            +QString(QChar(130))
            +QString(QChar(161))
            +QString(QChar(162))
            +QString(QChar(163))
            +QString(QChar(181))
            +QString(QChar(144))
            +QString(QChar(214))
            +QString(QChar(224))
            +QString(QChar(233))
            +"]{0,45}";
    QRegExp regExp_ruc(str_regExp_ruc);*/

    //qDebug()<<str_regExp_ruc<<endl;


    //ui->lineEdit_nombre->setValidator(new QRegExpValidator(regExp_ruc));

    //ui->pushButton_guardar->setDisabled(true);
    //ui->pushButton_eliminar->setDisabled(true);
    //ui->comboBox_buscar->hide();
    //ui->label_buscar->hide();
    //ui->pushButton_eliminar->hide();
    ui->pushButton_nuevo->hide();
}

Plato::~Plato()
{
    qDebug()<<"delete plato"<<endl;
    delete ui;
}
void Plato::modo_ingresar(QString familia, int x, int y)
{
    ui->label_buscar->hide();

    this->familia = familia;

    ui->label_familia->setText(familia);

    ui->spinBox_x->setValue(x);
    ui->spinBox_y->setValue(y);
    ui->label_x->hide();
    ui->label_y->hide();
    ui->spinBox_x->hide();
    ui->spinBox_y->hide();
    ui->comboBox_buscar->hide();

    //ui->pushButton_guardar->hide();
    ui->pushButton_modificar->hide();
    ui->pushButton_eliminar->hide();
}
void Plato::modo_modificacion(QString familia, QString plato, int x, int y)
{
    ui->label_buscar->hide();

    this->familia = familia;

    current_plato = plato;

    ui->label_titulo->setText(familia + " - " + plato);
    ui->lineEdit_nombre->setText(plato);

    QString str_query;
    QSqlQuery query;
    str_query += "SELECT precio, descripcion FROM plato WHERE item_nombre = '" + ui->lineEdit_nombre->text() + "'";

    if(query.exec(str_query)){
        query.next();
        double precio = query.value(0).toDouble();
        QString descripcion = query.value(1).toString();
        ui->doubleSpinBox_precio->setValue(precio);
        ui->lineEdit_descripcion->setText(descripcion);
    }
    ui->spinBox_x->setValue(x);
    ui->spinBox_y->setValue(y);
    ui->label_x->hide();
    ui->label_y->hide();
    ui->spinBox_x->hide();
    ui->spinBox_y->hide();
    ui->comboBox_buscar->hide();

    ui->pushButton_guardar->hide();
    //ui->pushButton_modificar->hide();
    ui->pushButton_eliminar->hide();
}
void Plato::modo_eliminacion(QString familia, QString plato)
{
    ui->label_buscar->hide();

    this->familia = familia;

    current_plato = plato;

    ui->label_titulo->setText(familia + " - " + plato);
    ui->lineEdit_nombre->setText(plato);

    ui->label_x->hide();
    ui->label_y->hide();
    ui->spinBox_x->hide();
    ui->spinBox_y->hide();

    ui->comboBox_buscar->hide();
    ui->pushButton_guardar->hide();
    ui->pushButton_modificar->hide();
    //ui->pushButton_eliminar->hide();
}
void Plato::select_all()
{
    QSqlQuery query;
    QString str_query = "";
    str_query += "SELECT plato.item_nombre, plato.x, plato.y";
    str_query += " FROM plato";
    str_query += " JOIN familia ON familia.item_nombre = plato.familia_item_nombre";
    str_query += " WHERE familia.item_nombre = '"+this->familia+"'";

    if(query.exec(str_query)){
        ui->comboBox_buscar->clear();
        while(query.next()){
            QString nombre = query.value(0).toString();
            //int x = query.value(1).toInt();
            //int y = query.value(2).toInt();

            ui->comboBox_buscar->addItem(nombre);
            //ui->spinBox_x->setValue(x);
            //ui->spinBox_y->setValue(y);
        }
        ui->comboBox_buscar->setCurrentText(ui->lineEdit_nombre->text());
    }
}
void Plato::set_familia_(QString familia)
{
    this->familia = familia;
}

void Plato::set_familia(QString familia)
{
    this->familia = familia;
    ui->label_familia->setText(familia);

    select_all();
}

void Plato::ingresar()
{
    QString nombre = ui->lineEdit_nombre->text();
    int x = ui->spinBox_x->value();
    int y = ui->spinBox_y->value();
    QString descripcion = ui->lineEdit_descripcion->text();
    QString str_query = "";
    QSqlQuery query;
    str_query += "INSERT INTO item(nombre, grupo_nombre)";
    str_query += "VALUES(";
    str_query += "'"+nombre+"'";
    str_query += ", 'familia'";
    str_query += ")";
    str_query += " ON DUPLICATE KEY UPDATE nombre = '" + ui->lineEdit_nombre->text() + "'";
    str_query += "&&END_QUERY&&";
    str_query += "INSERT INTO plato(item_nombre";
    str_query += ", familia_item_nombre, marca_item_nombre, unidad_item_nombre, x, y";
    str_query += ", precio, descripcion)";
    str_query += "VALUES(";
    str_query += "'"+nombre+"'";
    str_query += ", '"+this->familia+"'";
    str_query += ", 'General'";
    str_query += ", 'ONZ'";
    str_query += ", "+QString().setNum(x)+"";
    str_query += ", "+QString().setNum(y)+"";
    str_query += ", "+QString().setNum(ui->doubleSpinBox_precio->value(), ' ', 2)+"";
    str_query += ", '"+descripcion+"'";
    str_query += ")";
    str_query += "&&END_QUERY&&";

    SYSTEM->multiple_query(str_query);
    qDebug()<<str_query<<endl;
    if(query.exec(str_query)){
        QMessageBox::information(this, "Info", "Se guardo.", "ok");
        //select_all();
    }else{
        QMessageBox::warning(this, "Advertencia", "Ingrese correctamente los datos.", "ok");
    }
}
void Plato::modificar()
{
    QString nombre_old = this->current_plato;
    QString nombre = ui->lineEdit_nombre->text();
    QString descripcion = ui->lineEdit_descripcion->text();
    QString precio = QString().setNum(ui->doubleSpinBox_precio->value(), ' ', 2);
    int x = ui->spinBox_x->value();
    int y = ui->spinBox_y->value();
    QString str_query = "";
    QSqlQuery query;
    str_query += "UPDATE item";
    str_query += " SET";
    str_query += " nombre = '"+nombre+"'";
    str_query += " WHERE nombre = '"+nombre_old+"'";
    str_query += "&&END_QUERY&&";
    str_query += "UPDATE plato";
    str_query += " SET";
    str_query += " x = "+QString().setNum(x)+"";
    str_query += ", y = "+QString().setNum(y)+"";
    str_query += ", precio = '"+precio+"'";
    str_query += ", descripcion = '"+descripcion+"'";
    str_query += " WHERE item_nombre = '"+nombre+"'";
    str_query += "&&END_QUERY&&";

    SYSTEM->multiple_query(str_query);
    qDebug()<<str_query<<endl;
    if(query.exec(str_query)){
        QMessageBox::information(this, "Info", "Se guardo.", "ok");
        //select_all();
    }else{
        QMessageBox::warning(this, "Advertencia", "Ingrese correctamente los datos.", "ok");
    }
}
void Plato::eliminar()
{
    QString nombre = ui->lineEdit_nombre->text();

    QString str_query = "";
    QSqlQuery query;
    str_query += "DELETE FROM item";
    str_query += " WHERE nombre = '"+nombre+"'";

    if(query.exec(str_query)){
        QMessageBox::information(this, "Info", "Se elimino.", "ok");
        //select_all();
    }else{
        QMessageBox::warning(this, "Advertencia", "Ingrese correctamente los datos.", "ok");
    }
}
void Plato::closeEvent(QCloseEvent *event)
{
    emit closing();
    event->accept();
}
void Plato::on_comboBox_buscar_activated(const QString &arg1)
{
    ui->lineEdit_nombre->setText(arg1);
    QString str_query = "";
    QSqlQuery query;
    str_query += "SELECT x, y, precio, descripcion FROM plato";
    str_query += " WHERE item_nombre = '" + arg1 + "'";

    qDebug()<<str_query<<endl;
    if(query.exec(str_query)){
        qDebug()<<"query ok"<<endl;
        if(query.next()){
            int x = query.value(0).toInt();
            int y = query.value(1).toInt();
            double precio = query.value(2).toDouble();
            QString descripcion = query.value(3).toString();

            ui->spinBox_x->setValue(x);
            ui->spinBox_y->setValue(y);
            ui->doubleSpinBox_precio->setValue(precio);
            ui->lineEdit_descripcion->setText(descripcion);
        }
    }
}
void Plato::on_spinBox_x_valueChanged(int arg1)
{

}

void Plato::on_spinBox_y_valueChanged(int arg1)
{

}

void Plato::on_pushButton_eliminar_clicked()
{
    int ret = QMessageBox::warning(this, "Advertencia", "¿Esta seguro de eliminar?", "Si", "Cancelar");
    switch(ret){
    case 0:{
        AdminPass* ap = new AdminPass(this);
        //ap->setAttribute(Qt::WA_DeleteOnClose);
        ap->exec();
        bool confirmado = ap->get_confirmado();

        if(!confirmado){
            return;
        }
        delete ap;

        is_eliminar = true;
        eliminar();
        this->close();
    }break;
    case 1:{

    }break;
    }
}

void Plato::on_pushButton_nuevo_clicked()
{

}
void Plato::on_pushButton_modificar_clicked()
{
    int ret = QMessageBox::warning(this, "Advertencia", "¿Esta seguro de modificar?", "Si", "Cancelar");
    switch(ret){
    case 0:{
        modificar();
        this->close();
    }break;
    case 1:{

    }break;
    }
}
void Plato::on_pushButton_guardar_clicked()
{
    int ret = QMessageBox::warning(this, "Advertencia", "¿Esta seguro de guardar?", "Si", "Cancelar");
    switch(ret){
    case 0:{
        ingresar();
        this->close();
    }break;
    case 1:{

    }break;
    }
}

void Plato::on_pushButton_salir_clicked()
{
    this->close();
}
