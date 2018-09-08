#include "enviarmecorreo.h"
#include "ui_enviarmecorreo.h"

EnviarmeCorreo::EnviarmeCorreo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EnviarmeCorreo)
{
    ui->setupUi(this);
}

EnviarmeCorreo::~EnviarmeCorreo()
{
    delete ui;
}

void EnviarmeCorreo::on_pushButton_enviar_clicked()
{
    SmtpClient smtp("smtp.gmail.com", 465, SmtpClient::SslConnection);

    // We need to set the username (your email address) and the password
    // for smtp authentification.

    smtp.setUser("billmcquack.peru@gmail.com");
    smtp.setPassword("Billmcquack6");

    QString str_query;
    str_query  = "SELECT persona.nombre, persona.email FROM persona";
    str_query += " JOIN persona_has_usuario per_h_u ON per_h_u.persona_cod = persona.cod";
    str_query += " JOIN usuario ON per_h_u.usuario_id = usuario.id";
    str_query += " WHERE usuario.nombre = '"+ui->lineEdit_usuario->text()+"'";
    // Now we create a MimeMessage object. This will be the email.

    QSqlQuery query;
    QString nombre, email;

    if(query.exec(str_query)){
        if(query.next()) {
            nombre = query.value(0).toString();
            email = query.value(1).toString();
        }else{
            return;
        }
    }else{
        return;
    }

    MimeMessage message;

    message.setSender(new EmailAddress("billmcquack.peru@gmail.com", "BMK PERU"));
    message.addRecipient(new EmailAddress(email, nombre));
    message.setSubject("Recuperación de contraseña");

    // Now we need to create a MimeHtml object for HTML content
    MimeHtml html;

    QString code;

    for(int i=0; i < 6 ; i++){
        code.push_back(((QChar)(qrand() % ((90+1)-65) + 65)));
    }
    codigoVerificacion = code;

    qDebug()<<code<<endl;
    html.setHtml("<h1> Hola! </h1>"
                "<h2> Ingrese el código de verificación que se encuentra en la parte de abajo. </h2>"
                "<h3> "+code+" </h3>");

    message.addPart(&html);

    // Now the email can be sended

    smtp.connectToHost();
    smtp.login();
    smtp.sendMail(message);
    smtp.quit();
}

void EnviarmeCorreo::on_pushButton_salir_clicked()
{
    this->close();
}

void EnviarmeCorreo::on_lineEdit_codigoVerificacion_textEdited(const QString &arg1)
{
    if(arg1.compare(codigoVerificacion) == 0){
        QString pass;
        QString str_query;
        str_query  = "SELECT usuario.pass FROM persona";
        str_query += " JOIN persona_has_usuario per_h_u ON per_h_u.persona_cod = persona.cod";
        str_query += " JOIN usuario ON per_h_u.usuario_id = usuario.id";
        str_query += " WHERE usuario.nombre = '"+ui->lineEdit_usuario->text()+"'";
        // Now we create a MimeMessage object. This will be the email.

        QSqlQuery query;
        if(query.exec(str_query)){
            query.next();
            pass = query.value(0).toString();
        }else{

        }
        ui->lineEdit_pass->setText(pass);
    }
}

void EnviarmeCorreo::on_pushButton_revelar_clicked()
{
    if(ui->lineEdit_pass->echoMode() == QLineEdit::Password)
        ui->lineEdit_pass->setEchoMode(QLineEdit::Normal);
    else
        ui->lineEdit_pass->setEchoMode(QLineEdit::Password);
}
