#include "persona.h"
#include "ui_persona.h"

QString Persona::opc_clientes = "Clientes";
QString Persona::opc_proveedores = "Proveedores";
QString Persona::opc_usuarios = "Usuarios";

QString Persona::master = "Master";
QString Persona::administrador = "Administrador";
QString Persona::empleado = "Empleado";
QString Persona::cajero = "Cajero";
QString Persona::proveedor = "Proveedor";
QString Persona::transportista = "Transportista";
QString Persona::cliente_ruc = "Cliente_RUC";
QString Persona::cliente_dni = "Cliente_DNI";
QString Persona::cliente_eventual = "Cliente_Eventual";
QString Persona::cliente_delivery = "Cliente_Delivery";

Persona::Persona(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Persona)
{
    ui->setupUi(this);

    QString str_query;
    str_query  = QString() + "SELECT nombre FROM item WHERE grupo_nombre = '" + GRUPO_ESTADO + "'";
    QSqlQuery query;
    if(query.exec(str_query)){
        while(query.next()){
            ui->comboBox_estado->addItem(query.value(0).toString());
        }
    }

    str_query = "(SELECT IFNULL(MAX(t.cod), 0) + 1 FROM (SELECT IFNULL(CAST(cod AS DECIMAL(11, 0)), 0) AS cod FROM persona";
    str_query += " WHERE LENGTH(cod) <> 8 AND LENGTH(cod) <> 11 AND cod <> '') AS t";
    str_query += " ORDER BY t.cod";
    str_query += ")";
    str_query += " UNION ALL";
    str_query += " (SELECT nombre FROM item";
    str_query += " WHERE grupo_nombre = 'Estado')";


    qDebug() << str_query << endl;
    if(query.exec(str_query)){
        query.next();
        QString cod = query.value(0).toString();
        ui->lineEdit_codigo->setText(cod);
        while(query.next()){
            QString nombre = query.value(0).toString();

            ui->comboBox_estado->addItem(nombre);
        }
    }else{
        QMessageBox::critical(this, "Error", "No se pudo realizar dicha acción", "Ok");
    }
}

Persona::~Persona()
{
    qDebug()<<"delete persona"<<endl;
    delete ui;
}

void Persona::setOperacion(int operacion)
{
    this->operacion = operacion;

    if(operacion == INGRESAR){
        ui->pushButton_guardar->setText("Ingresar");
        ui->pushButton_guardar->setIcon(QIcon(":/new/Iconos/colaborador_add.png"));
        ui->pushButton_eliminar->hide();
    }
    if(operacion == MODIFICAR){
        ui->pushButton_guardar->setText("Guardar");
        ui->pushButton_guardar->setIcon(QIcon(":/new/Iconos/colaborador_edit.png"));
    }
}
void Persona::setTipoPersona(QString tipoPersona)
{
    this->tipoPersona = tipoPersona;

    ui->lineEdit_codigo->setValidator(new QRegExpValidator(QRegExp("[0-9]{13,13}")));

    if(tipoPersona.compare(cliente_delivery) == 0){
        ui->pushButton_buscarNombre->hide();

        ui->label_codigo->hide();
        ui->lineEdit_codigo->hide();
        ui->horizontalSpacer_codigo->changeSize(0, 0);
        ui->pushButton_buscarCodigo->hide();

        ui->label_usuarioContrasenia->hide();
        ui->lineEdit_usuarioContrasenia->hide();
        ui->horizontalSpacer_contrasenia->changeSize(0, 0);
        ui->pushButton_mostrarContrasenia->hide();

        ui->label_usuarioNombre->hide();
        ui->lineEdit_usuarioNombre->hide();
        ui->horizontalSpacer_usuario->changeSize(0, 0);

        ui->label_fechaNacimiento->hide();
        ui->dateEdit_fechaNacimiento->hide();
        ui->horizontalSpacer_fechaNacimiento->changeSize(0, 0);
    }

    if(tipoPersona.compare(cliente_dni) == 0){
        ui->lineEdit_codigo->setValidator(new QRegExpValidator(QRegExp("[0-9]{8,8}")));
    }
    if(tipoPersona.compare(cliente_ruc) == 0){
        ui->lineEdit_codigo->setValidator(new QRegExpValidator(QRegExp("[0-9]{11,11}")));
    }


    this->setWindowTitle(tipoPersona.toUpper());
    ui->label_titulo->setText(tipoPersona.toUpper());
}
void Persona::setCodigo(QString codigo)
{
    ui->lineEdit_codigo->setText(codigo);
}
void Persona::select(QString cod)
{
    QString str_query;
    str_query  = "SELECT persona.nombre, persona.direccion, persona.nombre_corto";
    str_query += ", persona.fecha_nacimiento, persona.telefono, persona.celular";
    str_query += ", persona.estado_item_nombre, usuario.nombre, usuario.pass";
    str_query += ", persona.referencia";
    str_query += " FROM persona";
    str_query += " LEFT JOIN persona_has_usuario per_h_u ON per_h_u.persona_cod = persona.cod";
    str_query += " LEFT JOIN usuario ON usuario.id = per_h_u.usuario_id";
    str_query += " WHERE persona.cod = '"+cod+"'";

    QSqlQuery query;
    qDebug()<<str_query<<endl;
    if(query.exec(str_query)){
        if(query.next()){
            ui->lineEdit_codigo->setText(cod);
            ui->lineEdit_nombre->setText(query.value(0).toString());
            ui->lineEdit_direccion->setText(query.value(1).toString());            
            ui->lineEdit_nombreCorto->setText(query.value(2).toString());
            ui->dateEdit_fechaNacimiento->setDate(query.value(3).toDate());
            ui->lineEdit_telefono->setText(query.value(4).toString());
            ui->lineEdit_celular->setText(query.value(5).toString());
            ui->comboBox_estado->setCurrentText(query.value(6).toString());
            ui->lineEdit_usuarioNombre->setText(query.value(7).toString());
            ui->lineEdit_usuarioContrasenia->setText(query.value(8).toString());
            ui->lineEdit_referencia->setText(query.value(9).toString());
        }
    }else{
        this->close();
    }
}
void Persona::on_pushButton_eliminar_clicked()
{
    QString str_query;
    str_query  = "DELETE FROM usuario";
    str_query += " WHERE nombre='"+ui->lineEdit_usuarioNombre->text()+"'";
    str_query += "&&END_QUERY&&";

    str_query += "DELETE FROM persona";
    str_query += " WHERE cod = '"+ui->lineEdit_codigo->text()+"'";
    str_query += "&&END_QUERY&&";

    QSqlQuery query;
    if(query.exec(str_query)){
        QMessageBox::information(this, "Información", "Operación realizada con éxito.", "Ok");
        this->close();
    }else{
        QMessageBox::warning(this, "Advertencia", "Ocurrio un error. Quiza el nombre de usuario es repetido.", "Ok");
        this->close();
    }
}

void Persona::on_pushButton_guardar_clicked()
{
    if(operacion == INGRESAR){
        QString str_query;
        if(ui->lineEdit_usuarioNombre->text().compare("") != 0
                && ui->lineEdit_usuarioContrasenia->text().compare("") != 0){
            str_query  = "INSERT INTO usuario(nombre, pass)VALUES(";
            str_query += "'"+ui->lineEdit_usuarioNombre->text()+"'";
            str_query += ", '"+ui->lineEdit_usuarioContrasenia->text()+"')";
            str_query += "&&END_QUERY&&";
        }

        str_query += "INSERT INTO persona(cod";
        str_query += ", nombre";
        str_query += ", nombre_corto";
        str_query += ", fecha_nacimiento";
        str_query += ", telefono";
        str_query += ", celular";
        str_query += ", fecha_inicio";
        str_query += ", fecha_fin";
        str_query += ", direccion";
        str_query += ", referencia";
        str_query += ", estado_item_nombre";
        str_query += ", tipo_item_nombre)";
        str_query += "VALUES(";
        str_query += "'" + ui->lineEdit_codigo->text() + "'";
        str_query += ", '" + ui->lineEdit_nombre->text() + "'";
        str_query += ", '" + ui->lineEdit_nombreCorto->text() + "'";
        str_query += ", '" + ui->dateEdit_fechaNacimiento->date().toString("yyyy-MM-dd") + "'";
        str_query += ", '" + ui->lineEdit_telefono->text() + "'";
        str_query += ", '" + ui->lineEdit_celular->text() + "'";
        str_query += ", '"+QDate(2000, 10, 1).toString("yyyy-MM-dd")+"'";
        str_query += ", '"+QDate(2000, 10, 1).toString("yyyy-MM-dd")+"'";
        str_query += ", '"+ui->lineEdit_direccion->text()+"'";
        str_query += ", '"+ui->lineEdit_referencia->text()+"'";
        str_query += ", '"+ui->comboBox_estado->currentText()+"'";
        str_query += ", '"+tipoPersona+"')";
        str_query += "&&END_QUERY&&";

        if(ui->lineEdit_usuarioNombre->text().compare("") != 0
                && ui->lineEdit_usuarioContrasenia->text().compare("") != 0){
            str_query += "INSERT INTO personas_has_usuario(persona_cod, usuario_id)VALUES(";
            str_query += "'"+ui->lineEdit_codigo->text()+"'";
            str_query += ", (SELECT MAX(id) FROM usuario))";
            str_query += "&&END_QUERY&&";
        }

        SYSTEM->multiple_query(str_query);
        qDebug()<<str_query<<endl;
        QSqlQuery query;
        if(query.exec(str_query)){
            QMessageBox::information(this, "Información", "Operación realizada con éxito.", "Ok");
            this->close();
        }else{
            QMessageBox::warning(this, "Advertencia", "Ocurrio un error. Quiza el nombre de usuario es repetido.", "Ok");
            this->close();
        }
    }
    if(operacion == MODIFICAR){
        QString str_query;
        if(ui->lineEdit_usuarioNombre->text().compare("") != 0
                && ui->lineEdit_usuarioContrasenia->text().compare("") != 0){
            str_query  = "UPDATE usuario SET";
            str_query += " nombre = '"+ui->lineEdit_usuarioNombre->text()+"'";
            str_query += ", pass = '"+ui->lineEdit_usuarioContrasenia->text()+"'";
            str_query += " WHERE nombre = '"+ui->lineEdit_usuarioNombre->text()+"'";
            str_query += "&&END_QUERY&&";
        }

        str_query += "UPDATE persona SET";
        str_query += " cod = '"+ui->lineEdit_codigo->text()+"'";
        str_query += ", nombre = '" + ui->lineEdit_nombre->text() + "'";
        str_query += ", nombre_corto = '" + ui->lineEdit_nombreCorto->text() + "'";
        str_query += ", fecha_nacimiento = '" + ui->dateEdit_fechaNacimiento->date().toString("yyyy-MM-dd") + "'";
        str_query += ", telefono = '" + ui->lineEdit_telefono->text() + "'";
        str_query += ", celular = '" + ui->lineEdit_celular->text() + "'";
        str_query += ", fecha_inicio = '"+QDate(2000, 10, 1).toString("yyyy-MM-dd")+"'";
        str_query += ", fecha_fin = '"+QDate(2000, 10, 1).toString("yyyy-MM-dd")+"'";
        str_query += ", direccion = '"+ui->lineEdit_direccion->text()+"'";
        str_query += ", referencia = '"+ui->lineEdit_referencia->text()+"'";
        str_query += ", estado_item_nombre = '"+ui->comboBox_estado->currentText()+"'";
        str_query += ", tipo_item_nombre = '"+tipoPersona+"'";
        str_query += " WHERE cod = '"+ui->lineEdit_codigo->text()+"'";
        str_query += "&&END_QUERY&&";

        SYSTEM->multiple_query(str_query);
        qDebug()<<str_query<<endl;
        QSqlQuery query;
        if(query.exec(str_query)){
            QMessageBox::information(this, "Información", "Operación realizada con éxito.", "Ok");
            this->close();
        }else{
            QMessageBox::warning(this, "Advertencia", "Ocurrio un error. Quiza el nombre de usuario es repetido.", "Ok");
            this->close();
        }
    }
}

void Persona::on_pushButton_mostrarContrasenia_clicked()
{
    if(ui->lineEdit_usuarioContrasenia->echoMode() == QLineEdit::Password)
        ui->lineEdit_usuarioContrasenia->setEchoMode(QLineEdit::Normal);
    else
        ui->lineEdit_usuarioContrasenia->setEchoMode(QLineEdit::Password);
}

void Persona::on_pushButton_salir_clicked()
{
    this->close();
}
