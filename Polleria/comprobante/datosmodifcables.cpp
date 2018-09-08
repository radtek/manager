#include "datosmodifcables.h"
#include "ui_datosmodifcables.h"

DatosModifcables::DatosModifcables(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DatosModifcables)
{
    ui->setupUi(this);

    ui->dateEdit_fecha->setDate(QDate::currentDate());

    ui->label_chicharron->hide();
    ui->doubleSpinBox_chicharron_pollo->hide();
    ui->horizontalSpacer_chicharron->changeSize(0,0);
    adjustSize();
}

DatosModifcables::~DatosModifcables()
{
    delete ui;
}
void DatosModifcables::set_buscar()
{
    QString str_query;
    str_query += "SELECT dinero_inicial, venta_otros_conceptos, ingreso_pollo, personal_pollo";
    str_query += ", baja_pollo, chicharron_pollo, cortesia_pollo";
    str_query += ", ingreso_broaster FROM modificables";
    str_query += " WHERE DATE(fecha) = '"+ui->dateEdit_fecha->date().toString("yyyy-MM-dd")+"'";

    QSqlQuery query;
    qDebug()<<str_query<<endl;

    if(query.exec(str_query)){
        query.next();

        QString dinero_inicial = query.value(0).toString();
        QString venta_otros_conceptos = query.value(1).toString();
        QString ingreso_pollo = query.value(2).toString();
        QString personal_pollo = query.value(3).toString();
        QString baja_pollo = query.value(4).toString();
        QString chicharron_pollo = query.value(5).toString();
        QString cortesia_pollo = query.value(6).toString();
        QString ingreso_broaster = query.value(7).toString();

        ui->doubleSpinBox_dinero_inicial->setValue(dinero_inicial.toDouble());
        ui->doubleSpinBox_venta_otros_conceptos->setValue(venta_otros_conceptos.toDouble());
        ui->doubleSpinBox_ingreso_pollo->setValue(ingreso_pollo.toDouble());
        ui->doubleSpinBox_personal_pollo->setValue(personal_pollo.toDouble());
        ui->doubleSpinBox_baja_pollo->setValue(baja_pollo.toDouble());
        ui->doubleSpinBox_chicharron_pollo->setValue(chicharron_pollo.toDouble());
        ui->doubleSpinBox_cortesia_pollo->setValue(cortesia_pollo.toDouble());
        ui->doubleSpinBox_ingreso_broaster->setValue(ingreso_broaster.toDouble());
    }else{
        QMessageBox::warning(this, "Advertencia", "No hay datos modificables.", "Aceptar");
    }
}
void DatosModifcables::on_pushButton_continuar_clicked()
{
    opc = CONTINUAR;

    QDate item_date = ui->dateEdit_fecha->date();
    qDebug()<<item_date<<endl;
    if(item_date != QDate::currentDate()){
        if(SYSTEM->tipo_persona.compare(Persona::master) != 0){
            QMessageBox::warning(this, "Advertencia", "Solo puede con fecha de hoy.\nSe requiere al usuario \"Master\".", "Aceptar");
            return;
        }
    }

    AdminPass* ap = new AdminPass(this);
    //ap->setAttribute(Qt::WA_DeleteOnClose);
    ap->setModalidad(AdminPass::CONFIRM_MY_PASS);
    ap->exec();
    bool confirmado = ap->get_confirmado();

    if(!confirmado){
        return;
    }
    delete ap;

    QMessageBox msgBox(this);
    msgBox.setIcon(QMessageBox::Warning);
    //msgBox.setParent();
    //msgBox.setStandardButtons(QMessageBox::Ok);
    QFont font;
    font.setFamily("MS Shell Dlg 2");
    font.setBold(false);
    font.setPointSize(11);
    msgBox.setFont(font);
    QPushButton* bt_guardar = new QPushButton("Guardar(Enter)");
    QPushButton* bt_cancelar = new QPushButton("Cancelar(Esc)");
    bt_guardar->setFont(font);
    bt_cancelar->setFont(font);
    msgBox.addButton(bt_guardar, QMessageBox::NoRole);
    msgBox.addButton(bt_cancelar, QMessageBox::RejectRole);
    msgBox.setDefaultButton(bt_guardar);
    msgBox.setEscapeButton(bt_cancelar);

    msgBox.setWindowTitle("Advertencia");
    msgBox.setText("¿Cual opción eligirá usted?");



    int ret = msgBox.exec();
    qDebug()<<"ret: "<<ret<<endl;
    switch(ret){

    case 0:{
        qDebug()<<"guardar"<<endl;
    }break;
    case 1:{
        qDebug()<<"cancel"<<endl;
        return;
    }break;
    }


    QString str_query;
    str_query = "SELECT IF(";
    str_query += "(SELECT 1 FROM modificables";
    str_query += " WHERE DATE(fecha) = '"+ui->dateEdit_fecha->date().toString("yyyy-MM-dd")+"')";
    str_query += " IS NULL, 0, 1)";

    ret = 0;
    QSqlQuery query;
    qDebug()<<str_query<<endl;
    if(query.exec(str_query)){
        query.next();
        ret = query.value(0).toInt();
    }else{
        QMessageBox::warning(this, "Advertencia", "No se guardo.", "Aceptar");
        return;
    }

    if(ret == 1){
        str_query = "UPDATE modificables";
        str_query += " SET";
        str_query += " dinero_inicial = '"+QString().setNum(ui->doubleSpinBox_dinero_inicial->value(), ' ', 3)+"'";
        str_query += ", venta_otros_conceptos = '"+QString().setNum(ui->doubleSpinBox_venta_otros_conceptos->value(), ' ', 3)+"'";
        str_query += ", ingreso_pollo = '"+QString().setNum(ui->doubleSpinBox_ingreso_pollo->value(), ' ', 3)+"'";
        str_query += ", personal_pollo = '"+QString().setNum(ui->doubleSpinBox_personal_pollo->value(), ' ', 3)+"'";
        str_query += ", baja_pollo = '"+QString().setNum(ui->doubleSpinBox_baja_pollo->value(), ' ', 3)+"'";
        str_query += ", chicharron_pollo = '"+QString().setNum(ui->doubleSpinBox_chicharron_pollo->value(), ' ', 3)+"'";
        str_query += ", cortesia_pollo = '"+QString().setNum(ui->doubleSpinBox_cortesia_pollo->value(), ' ', 3)+"'";
        str_query += ", ingreso_broaster = '"+QString().setNum(ui->doubleSpinBox_ingreso_broaster->value(), ' ', 3)+"'";
        str_query += " WHERE DATE(fecha) = '"+ui->dateEdit_fecha->date().toString("yyyy-MM-dd")+"'";
    }else{
        str_query = "INSERT INTO modificables";
        str_query += "(dinero_inicial, venta_otros_conceptos, ingreso_pollo, personal_pollo";
        str_query += ", baja_pollo, chicharron_pollo, cortesia_pollo, ingreso_broaster, fecha)VALUES(";
        str_query += "'"+QString().setNum(ui->doubleSpinBox_dinero_inicial->value(), ' ', 3)+"'";
        str_query += ", '"+QString().setNum(ui->doubleSpinBox_venta_otros_conceptos->value(), ' ', 3)+"'";
        str_query += ", '"+QString().setNum(ui->doubleSpinBox_ingreso_pollo->value(), ' ', 3)+"'";
        str_query += ", '"+QString().setNum(ui->doubleSpinBox_personal_pollo->value(), ' ', 3)+"'";
        str_query += ", '"+QString().setNum(ui->doubleSpinBox_baja_pollo->value(), ' ', 3)+"'";
        str_query += ", '"+QString().setNum(ui->doubleSpinBox_chicharron_pollo->value(), ' ', 3)+"'";
        str_query += ", '"+QString().setNum(ui->doubleSpinBox_cortesia_pollo->value(), ' ', 3)+"'";
        str_query += ", '"+QString().setNum(ui->doubleSpinBox_ingreso_broaster->value(), ' ', 3)+"'";
        str_query += ", '"+ui->dateEdit_fecha->date().toString("yyyy-MM-dd")+"'";
        str_query += ")";
    }

    //QSqlQuery query;
    qDebug()<<str_query<<endl;
    if(query.exec(str_query)){

    }else{
        QMessageBox::warning(this, "Advertencia", "No se guardo.", "Aceptar");
        return;
    }

}

void DatosModifcables::on_pushButton_salir_clicked()
{
    opc = SALIR;
    this->close();
}
void DatosModifcables::closeEvent(QCloseEvent *event)
{
    emit closing();
    event->accept();
}

void DatosModifcables::on_dateEdit_fecha_dateChanged(const QDate &date)
{
    set_buscar();
}

void DatosModifcables::on_pushButton_clicked()
{
    opc = CONTINUAR;
    this->close();
}
