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

    //ui->lineEdit_nombre->setValidator(new QRegExpValidator(regExp_ruc));

    //ui->pushButton_eliminar->hide();
    ui->lineEdit_nombre->setFocus();
    //ui->pushButton_guardar->setDisabled(true);
    //ui->pushButton_eliminar->setDisabled(true);
    //ui->comboBox_buscar->hide();
    //ui->label_buscar->hide();
    //ui->pushButton_nuevo->hide();
}

Familia::~Familia()
{
    qDebug()<<"delete familia"<<endl;
    delete ui;
}
void Familia::setOperacion(QString operacion)
{
    this->operacion = operacion;
}
void Familia::modo_ingresar(int x, int y)
{
    this->x = x;
    this->y = y;
    ui->pushButton_modificar->hide();
    ui->pushButton_eliminar->hide();
}
void Familia::modo_modificacion(QString familia, int x, int y)
{
    this->x = x;
    this->y = y;
    current_familia = familia;

    ui->label_titulo->setText(familia);

    ui->lineEdit_nombre->setText(familia);

    ui->pushButton_guardar->hide();
}

void Familia::ingresar()
{
    QString nombre = ui->lineEdit_nombre->text();
    int x = this->x;
    int y = this->y;
    QString str_query = "";
    QSqlQuery query;

    str_query += "INSERT INTO familia(operacion_item_nombre, nombre, x, y, estado_item_nombre, imagen_dir)";
    str_query += "VALUES(";
    str_query += "'"+operacion+"'";
    str_query += ", '"+nombre+"'";
    str_query += ", "+QString().setNum(x)+"";
    str_query += ", "+QString().setNum(y)+"";
    str_query += QString()+", '"+ACTIVO+"'";
    str_query += ", '"+imagen_dir+"'";
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
    int x = this->x;
    int y = this->y;
    QString str_query = "";
    QSqlQuery query;
    str_query += "UPDATE familia";
    str_query += " SET";
    str_query += " x = "+QString().setNum(x)+"";
    str_query += ", y = "+QString().setNum(y)+"";
    str_query += ", nombre = '"+nombre+"'";
    str_query += ", imagen_dir = '"+imagen_dir+"'";
    str_query += " WHERE nombre = '"+nombre_old+"'";
    str_query += " AND operacion_item_nombre = '"+operacion+"'";
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
    str_query += "UPDATE familia";
    str_query += QString()+" SET estado_item_nombre = '"+INACTIVO+"'";
    str_query += " WHERE nombre = '"+nombre+"'";
    str_query += " AND operacion_item_nombre = '"+operacion+"'";

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

void Familia::on_pushButton_eliminar_clicked()
{
    int ret = QMessageBox::warning(this, "Advertencia", "¿Esta seguro de eliminar?", "Si", "Cancelar");
    switch(ret){
    case 0:{
        AdminPass* ap = new AdminPass(this);
        //ap->setAttribute(Qt::WA_DeleteOnClose);
        ap->setModalidad(AdminPass::CONFIRM_MY_PASS);
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

void Familia::on_pushButton_buscar_imagen_clicked()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter(tr("Images (*.png *.xpm *.jpg)"));
    if(dialog.exec() == 1){
        QString fileUrl = dialog.selectedFiles().first();
        qDebug()<<fileUrl<<endl;

        ui->label_imagen_visual->setPixmap(QPixmap(fileUrl));
        imagen_dir = fileUrl;
    }
    /*
    QString fileUrl = QFileDialog::getOpenFileUrl(this,
        , "", tr("Image Files (*.png *.jpg *.bmp)")).toString();

    ui->label_imagen_visual->setPixmap(QPixmap(fileUrl));
    imagen_dir = fileUrl;*/
}
