#ifndef CPPSCRIPT_H
#define CPPSCRIPT_H

// Standart libraries
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
using namespace std;

// Qt libraries
#include <QDebug>
#include <QtSql/QtSql>
#include <QFile>
#include <QString>

const QString STR_OBJECT = "";
const QString STR_INCLUDE= "include/";

class CPPScript
{
public:
    CPPScript();            

    void setHost(QString host);
    void setUser(QString user);
    void setPass(QString pass);
    void setDB(QString db);
    void set_port(QString port);

    bool generar_clases();

    void setDirFolder(QString dir);

    QString getHost();
    QString getUser();
    QString getPass();
    QString getDB();

    QString getDirFolder();
private:
    QString md_o_name_class;

    QString host;
    QString user;
    QString pass;
    QString db;
    QString port;

    QString dirFolder;

    QVector<QPair<QString, QVector<QVector<QString> > > > data;

private:
    bool extractColumnsOfData(QVector<QPair<QString, QVector<QVector<QString> > > >& data);

    bool openFile_h(QString class_name);
    bool openFile_cpp(QString class_name);

    void fillFile_h(QString class_name, QVector<QString>& column_names, int i);
    void fillFile_cpp(QString class_name, QVector<QString>& column_names, int i);

    void generate_class_objectGeneral_h(QVector<QPair<QString, QVector<QVector<QString> > > >& data);
    void generate_class_objectGeneral_cpp();

};

#endif // CPPSCRIPT_H
