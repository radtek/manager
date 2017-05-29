#include "database_connection.h"

#include <QFile>
#include <QDebug>

database_connection::database_connection()
{
    md_driver = "";
    md_host = "";
    md_name = "";
    md_options = "";
    md_pass = "";
    md_port = "";
    md_user = "";
    QFile file(QString("config")+QString(DB_FILE_NAME_EXTENSION));

    if(!file.exists())
    {
        if(file.open(QIODevice::WriteOnly | QIODevice::Text)){
            //qDebug()<<"File created."<<endl;
        }
    }else{
        //qDebug()<<"File Exist"<<endl;
    }

}

database_connection::~database_connection()
{

}


void database_connection::mf_set_driver(QString driver)
{
    md_driver = driver;
}

void database_connection::mf_set_host(QString host)
{
    md_host = host;
}

void database_connection::mf_set_port(QString port)
{
    md_port = port;
}

void database_connection::mf_set_user(QString user)
{
    md_user = user;
}

void database_connection::mf_set_pass(QString pass)
{
    md_pass = pass;
}

void database_connection::mf_set_name(QString name)
{
    md_name = name;
}

void database_connection::mf_set_options(QString options)
{
    md_options = options;
}

QString database_connection::mf_get_driver()
{
    return md_driver;
}

QString database_connection::mf_get_host()
{
    return md_host;
}

QString database_connection::mf_get_port()
{
    return md_port;
}

QString database_connection::mf_get_user()
{
    return md_user;
}

QString database_connection::mf_get_pass()
{
    return md_pass;
}

QString database_connection::mf_get_name()
{
    return md_name;
}

QString database_connection::mf_get_options()
{
    return md_options;
}
void database_connection::mf_setConfig()
{
    QFile file(QString("config")+DB_FILE_NAME_EXTENSION);

    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        out <<md_host<<'\n'
           <<md_port<<'\n'
          <<md_name<<'\n'
         <<md_user<<'\n'
         <<md_pass;
        file.close();
    }
}

void database_connection::mf_setConfig(QString host, QString port, QString user, QString pass, QString name)
{

}

void database_connection::mf_load_configuration(QString file_config)
{
	QString name_file_cfg = QString() + file_config + DB_FILE_NAME_EXTENSION;

    //qDebug()<<name_file_cfg<<endl;
    QFile file(name_file_cfg);


    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        //qDebug()<<"open file"<<endl;

        QString str = file.readAll().toStdString().c_str();

        QList<QString> list_result;
        list_result = str.split("\n", QString::SkipEmptyParts);

        if (list_result.size() > 0) {
            md_host = list_result[0];
            md_port = list_result[1];
            md_name = list_result[2];
            md_user = list_result[3];
            md_pass = list_result[4];
        }
	}
}

bool database_connection::mf_connect()
{    
    QSqlDatabase my_database = QSqlDatabase::addDatabase("QODBC");

    //my_database.setHostName(md_host);
    //my_database.setPort(md_port.toInt());
    my_database.setDatabaseName(QString("DRIVER={%1};SERVER=%2;DATABASE=%3;UID=lorda;PWD=%4;Trusted_Connection=1;")
                                .arg(md_driver)
                                .arg(md_host)
                                .arg(md_name)
                                .arg(md_pass));
    //my_database.setUserName(md_user);
    //my_database.setPassword(md_pass);

    qDebug()<<"?"<<endl;
    if(my_database.open())
    {
        //qDebug()<<"Conection success...";
        return true;

    }else{
        //qDebug()<<"Conection error...";
        return false;
    }

}

