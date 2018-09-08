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
}

Plato::~Plato()
{
    qDebug()<<"delete plato"<<endl;
    delete ui;
}
void Plato::modo_ingresar(QString familia, int x, int y)
{
    this->x = x;
    this->y = y;
    this->familia = familia;

    ui->label_familia->setText(familia);

    ui->pushButton_modificar->hide();
    ui->pushButton_eliminar->hide();
}
void Plato::modo_modificar(QString familia, QString plato, int x, int y)
{
    this->x = x;
    this->y = y;
    this->current_plato = plato;

    this->familia = familia;

    ui->label_familia->setText(familia);
    ui->label_titulo->setText(plato + "  - ");

    ui->lineEdit_nombre->setText(plato);

    ui->pushButton_guardar->hide();

    QString str_query  = "SELECT precio, conteo, medida FROM producto";
    str_query += " WHERE familia_id = (SELECT t.id FROM familia t WHERE t.nombre = '"+familia+"'";
    str_query += " AND t.operacion_item_nombre = '"+operacion+"')";
    str_query += " AND nombre = '"+plato+"'";
    str_query += " AND operacion_item_nombre = '"+operacion+"'";
    QSqlQuery query;
    if(query.exec(str_query)){
        query.next();
        ui->doubleSpinBox_precio->setValue(query.value(0).toDouble());
        ui->checkBox_conteo->setChecked(query.value(1).toBool());
        ui->doubleSpinBox_medida->setValue(query.value(2).toDouble());
    }
}

void Plato::set_familia(QString familia)
{
    this->familia = familia;
}
void Plato::setOperacion(QString operacion)
{
    this->operacion = operacion;
}
void Plato::ingresar()
{
    QString nombre = ui->lineEdit_nombre->text();
    int x = this->x;
    int y = this->y;
    QString descripcion = ui->lineEdit_descripcion->text();
    QString str_query = "";
    QSqlQuery query;

    str_query += "INSERT INTO producto(nombre";
    str_query += ", operacion_item_nombre";
    str_query += ", familia_id, x, y";
    str_query += ", descripcion, precio";
    str_query += ", estado_item_nombre";
    str_query += ", conteo";
    str_query += ", medida)";
    str_query += "VALUES(";
    str_query += "'"+nombre+"'";
    str_query += ", '"+operacion+"'";
    str_query += ", (SELECT t.id FROM familia t WHERE t.nombre = '"+this->familia+"'";
    str_query += " AND t.operacion_item_nombre = '"+operacion+"')";
    str_query += ", "+QString().setNum(x)+"";
    str_query += ", "+QString().setNum(y)+"";
    str_query += ", '"+descripcion+"'";
    str_query += ", '"+QString().setNum(ui->doubleSpinBox_precio->value(), ' ', 1)+"'";
    str_query += QString() + ", '"+ACTIVO+"'";
    str_query += ", '"+QString().setNum(ui->checkBox_conteo->isChecked())+"'";
    str_query += ", '"+QString().setNum(ui->doubleSpinBox_medida->value(), ' ', 4)+"'";
    str_query += ")";
    str_query += "&&END_QUERY&&";

    if(familia.compare("Bebidas", Qt::CaseInsensitive) == 0){
        /*
        str_query += "INSERT INTO producto(nombre";
        str_query += ", operacion_item_nombre";
        str_query += ", familia_id, x, y";
        str_query += ", descripcion, precio";
        str_query += ", estado_item_nombre";
        str_query += ", conteo";
        str_query += ", medida)";
        str_query += "VALUES(";
        str_query += "'"+nombre+"'";
        str_query += ", '"+QString(COMPRA)+"'";
        str_query += ", (SELECT t.id FROM familia t WHERE t.nombre = '"+this->familia+"'";
        str_query += " AND t.operacion_item_nombre = '"+QString(COMPRA)+"')";
        str_query += ", "+QString().setNum(x)+"";
        str_query += ", "+QString().setNum(y)+"";
        str_query += ", '"+descripcion+"'";
        str_query += ", '"+QString().setNum(0.0, ' ', 1)+"'";
        str_query += QString() + ", '"+ACTIVO+"'";
        str_query += ", '"+QString().setNum(ui->checkBox_conteo->isChecked())+"'";
        str_query += ", '"+QString().setNum(ui->doubleSpinBox_medida->value(), ' ', 4)+"'";
        str_query += ")";
        str_query += "&&END_QUERY&&";*/
    }

    SYSTEM->start_transaction();

    SYSTEM->multiple_query(str_query);
    qDebug()<<str_query<<endl;
    if(query.exec(str_query)){
        SYSTEM->commit();
        QMessageBox::information(this, "Info", "Se guardo.", "ok");        
    }else{
        SYSTEM->rollback();
        QMessageBox::warning(this, "Advertencia", "Ingrese correctamente los datos.", "ok");        
    }
}
void Plato::modificar()
{
    QString nombre_old = this->current_plato;
    QString nombre = ui->lineEdit_nombre->text();
    QString descripcion = ui->lineEdit_descripcion->text();
    int x = this->x;
    int y = this->y;
    QString str_query = "";
    QSqlQuery query;

    str_query += "UPDATE producto";
    str_query += " SET";
    str_query += " x = "+QString().setNum(x)+"";
    str_query += ", y = "+QString().setNum(y)+"";
    str_query += ", descripcion = '"+descripcion+"'";
    str_query += ", nombre = '"+nombre+"'";
    str_query += ", precio = '"+QString().setNum(ui->doubleSpinBox_precio->value(), ' ', 1)+"'";
    str_query += ", conteo = '"+QString().setNum(ui->checkBox_conteo->isChecked())+"'";
    str_query += ", medida = '"+QString().setNum(ui->doubleSpinBox_medida->value(), ' ', 4)+"'";
    str_query += " WHERE familia_id = (SELECT t.id FROM familia t WHERE t.nombre = '"+familia+"'";
    str_query += " AND t.operacion_item_nombre = '"+operacion+"')";
    str_query += " AND nombre = '"+nombre_old+"'";
    str_query += " AND operacion_item_nombre = '"+operacion+"'";
    str_query += "&&END_QUERY&&";

    if(familia.compare("Bebidas", Qt::CaseInsensitive) == 0){/*
        str_query += "UPDATE producto";
        str_query += " SET";
        str_query += " x = "+QString().setNum(x)+"";
        str_query += ", y = "+QString().setNum(y)+"";
        str_query += ", descripcion = '"+descripcion+"'";
        str_query += ", nombre = '"+nombre+"'";
        //str_query += ", precio = '"+QString().setNum(ui->doubleSpinBox_precio->value(), ' ', 1)+"'";
        str_query += ", conteo = '"+QString().setNum(ui->checkBox_conteo->isChecked())+"'";
        str_query += ", medida = '"+QString().setNum(ui->doubleSpinBox_medida->value(), ' ', 4)+"'";
        str_query += " WHERE familia_id = (SELECT t.id FROM familia t WHERE t.nombre = '"+familia+"'";
        str_query += " AND t.operacion_item_nombre = '"+QString(COMPRA)+"')";
        str_query += " AND nombre = '"+nombre_old+"'";
        str_query += " AND operacion_item_nombre = '"+QString(COMPRA)+"'";
        str_query += "&&END_QUERY&&";*/
    }

    SYSTEM->start_transaction();

    SYSTEM->multiple_query(str_query);
    qDebug()<<str_query<<endl;
    if(query.exec(str_query)){
        SYSTEM->commit();
        QMessageBox::information(this, "Info", "Se guardo.", "ok");
    }else{
        SYSTEM->rollback();
        QMessageBox::warning(this, "Advertencia", "Ingrese correctamente los datos.", "ok");
    }
}
void Plato::eliminar()
{
    QString nombre = ui->lineEdit_nombre->text();

    QString str_query = "";
    QSqlQuery query;
    str_query += "UPDATE producto";
    str_query += QString() + " SET estado_item_nombre = '"+INACTIVO+"'";
    str_query += " WHERE familia_id = (SELECT t.id FROM familia t WHERE t.nombre = '"+familia+"'";
    str_query += " AND t.operacion_item_nombre = '"+operacion+"')";
    str_query += " AND nombre = '"+nombre+"'";
    str_query += " AND operacion_item_nombre = '"+operacion+"'";
    str_query += "&&END_QUERY&&";

    if(familia.compare("Bebidas", Qt::CaseInsensitive) == 0){/*
        str_query += "UPDATE producto";
        str_query += QString() + " SET estado_item_nombre = '"+INACTIVO+"'";
        str_query += " WHERE familia_id = (SELECT t.id FROM familia t WHERE t.nombre = '"+familia+"'";
        str_query += " AND t.operacion_item_nombre = '"+QString(COMPRA)+"')";
        str_query += " AND nombre = '"+nombre+"'";
        str_query += " AND operacion_item_nombre = '"+QString(COMPRA)+"'";
        str_query += "&&END_QUERY&&";*/
    }

    SYSTEM->start_transaction();

    SYSTEM->multiple_query(str_query);
    if(query.exec(str_query)){
        SYSTEM->commit();
        QMessageBox::information(this, "Info", "Se elimino.", "ok");                
    }else{
        SYSTEM->rollback();
        QMessageBox::warning(this, "Advertencia", "Ingrese correctamente los datos.", "ok");
    }
}
void Plato::closeEvent(QCloseEvent *event)
{
    emit closing();
    event->accept();
}

void Plato::on_pushButton_eliminar_clicked()
{
    int ret = QMessageBox::warning(this, "Advertencia", "¿Esta seguro de eliminar?", "Si", "Cancelar");
    switch(ret){
    case 0:{
        AdminPass* ap = new AdminPass(this);
        ap->setModalidad(AdminPass::CONFIRM_MY_PASS);
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

void Plato::on_lineEdit_medida_textChanged(const QString &arg1)
{
    QString str_query;
    str_query  = "SELECT valor, unidad_item_nombre FROM medida";
    str_query += " WHERE concat(valor, ' ', unidad_item_nombre) LIKE '%" + arg1 + "%'";
    str_query += " ORDER BY unidad_item_nombre, valor";

    QSqlQuery query;
    qDebug()<<str_query<<endl;
    if(query.exec(str_query)) {
        QStringList list;
        while(query.next()) {
            double valor = query.value(0).toDouble();
            QString unidad_item_nombre = query.value(1).toString();

            QString medida = QString().setNum(valor, ' ', 3) + " " + unidad_item_nombre;

            list << medida;
        }
        //model->setStringList(list);
        QCompleter* c = new QCompleter(list);
        c->setCaseSensitivity(Qt::CaseInsensitive);
        c->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
        c->setModelSorting(QCompleter::UnsortedModel);
        c->setFilterMode(Qt::MatchContains);

        //connect(c, SIGNAL(activated(QString)), this, SLOT(on_completer_buscarMedida_activated(QString)));
        //c->popup()->installEventFilter(this);

        //ui->lineEdit_medida->setCompleter(c);
        c->complete();

        //ui->lineEdit_medida->setFocus(Qt::TabFocusReason);
    }else{
        QMessageBox::critical(this, "Error", "No se pudo realizar dicha acción", "Ok");
    }
}
