#include "familia.h"
#include "ui_familia.h"

Familia::Familia(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Familia)
{
    ui->setupUi(this);

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
    QRegExp regExp_ruc(str_regExp_ruc);

    qDebug()<<str_regExp_ruc<<endl;*/
    is_ingresar = false;
    is_modificar = false;
    is_eliminar = false;
    //select_all();

    //ui->lineEdit_nombre->setValidator(new QRegExpValidator(regExp_ruc));

    //ui->pushButton_eliminar->hide();
    ui->lineEdit_nombre->setFocus();
    //ui->pushButton_guardar->setDisabled(true);
    //ui->pushButton_eliminar->setDisabled(true);
    //ui->comboBox_buscar->hide();
    //ui->label_buscar->hide();
    ui->pushButton_nuevo->hide();
}

Familia::~Familia()
{
    qDebug()<<"delete familia"<<endl;
    delete ui;
}
void Familia::modo_ingresar(int x, int y)
{
    ui->label_buscar->hide();

    ui->spinBox_x->setValue(x);
    ui->spinBox_y->setValue(y);

    ui->spinBox_x->hide();
    ui->spinBox_y->hide();
    ui->label_x->hide();
    ui->label_y->hide();
    ui->comboBox_buscar->hide();
    //ui->pushButton_guardar->hide();
    ui->pushButton_modificar->hide();
    ui->pushButton_eliminar->hide();
}
void Familia::modo_modificacion(QString familia, int x, int y)
{
    ui->label_buscar->hide();

    current_familia = familia;

    ui->label_titulo->setText(familia);

    ui->lineEdit_nombre->setText(familia);

    ui->spinBox_x->setValue(x);
    ui->spinBox_y->setValue(y);
    //ui->lineEdit_nombre->setText(familia);

    //on_comboBox_buscar_activated(familia);

    ui->comboBox_buscar->hide();
    ui->pushButton_guardar->hide();
    ui->spinBox_x->hide();
    ui->spinBox_y->hide();
    ui->label_x->hide();
    ui->label_y->hide();
    //ui->pushButton_modificar->hide();
    ui->pushButton_eliminar->hide();
}
void Familia::modo_eliminacion(QString familia)
{
    ui->label_buscar->hide();

    current_familia = familia;

    ui->label_titulo->setText(familia);

    ui->lineEdit_nombre->setText(familia);

    //on_comboBox_buscar_activated(familia);

    ui->spinBox_x->hide();
    ui->spinBox_y->hide();
    ui->label_x->hide();
    ui->label_y->hide();

    ui->comboBox_buscar->hide();
    ui->pushButton_guardar->hide();
    ui->pushButton_modificar->hide();
    //ui->pushButton_eliminar->hide();
}
void Familia::select_all()
{
    QSqlQuery query;
    QString str_query = "";
    str_query += "SELECT item_nombre, x, y";
    str_query += " FROM familia";

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
void Familia::ingresar()
{
    QString nombre = ui->lineEdit_nombre->text();
    int x = ui->spinBox_x->value();
    int y = ui->spinBox_y->value();
    QString str_query = "";
    QSqlQuery query;
    str_query += "INSERT INTO item(nombre, grupo_nombre)";
    str_query += "VALUES(";
    str_query += "'"+nombre+"'";
    str_query += ", 'familia'";
    str_query += ")";
    str_query += "&&END_QUERY&&";
    str_query += "INSERT INTO familia(item_nombre, x, y)";
    str_query += "VALUES(";
    str_query += "'"+nombre+"'";
    str_query += ", "+QString().setNum(x)+"";
    str_query += ", "+QString().setNum(y)+"";
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
void Familia::modificar()
{
    QString nombre_old = current_familia;
    QString nombre = ui->lineEdit_nombre->text();
    int x = ui->spinBox_x->value();
    int y = ui->spinBox_y->value();
    QString str_query = "";
    QSqlQuery query;
    str_query += "UPDATE item";
    str_query += " SET";
    str_query += " nombre = '"+nombre+"'";
    str_query += " WHERE nombre = '"+nombre_old+"'";
    str_query += "&&END_QUERY&&";
    str_query += "UPDATE familia";
    str_query += " SET";
    str_query += " x = "+QString().setNum(x)+"";
    str_query += ", y = "+QString().setNum(y)+"";
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
void Familia::eliminar()
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
void Familia::closeEvent(QCloseEvent *event)
{
    emit closing();
    event->accept();
}
void Familia::on_comboBox_buscar_activated(const QString &arg1)
{
    ui->lineEdit_nombre->setText(arg1);
    QString str_query = "";
    QSqlQuery query;
    str_query += "SELECT x, y FROM familia";
    str_query += " WHERE item_nombre = '" + arg1 + "'";

    qDebug()<<str_query<<endl;
    if(query.exec(str_query)){
        qDebug()<<"query ok"<<endl;
        if(query.next()){
            int x = query.value(0).toInt();
            int y = query.value(1).toInt();

            ui->spinBox_x->setValue(x);
            ui->spinBox_y->setValue(y);
        }
    }
}
void Familia::on_spinBox_x_valueChanged(int arg1)
{

}

void Familia::on_spinBox_y_valueChanged(int arg1)
{

}
void Familia::on_pushButton_guardar_clicked()
{
    int ret = QMessageBox::warning(this, "Advertencia", "¿Esta seguro de guardar?", "Si", "Cancelar");
    switch(ret){
    case 0:{
        is_ingresar = true;
        ingresar();
        this->close();
    }break;
    case 1:{

    }break;
    }
}

void Familia::on_pushButton_salir_clicked()
{
    this->close();
}

void Familia::on_pushButton_modificar_clicked()
{
    int ret = QMessageBox::warning(this, "Advertencia", "¿Esta seguro de modificar?", "Si", "Cancelar");
    switch(ret){
    case 0:{
        is_modificar = true;
        modificar();
        this->close();
    }break;
    case 1:{

    }break;
    }
}

void Familia::on_pushButton_nuevo_clicked()
{
    //ui->pushButton_guardar->setEnabled(true);
    //ui->pushButton_modificar->setDisabled(true);
    //ui->pushButton_eliminar->setDisabled(true);
    //ui->comboBox_buscar->hide();
    //ui->label_buscar->hide();
}

void Familia::on_pushButton_eliminar_clicked()
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
