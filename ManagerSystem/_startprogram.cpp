#include "_startprogram.h"

#include <QHostInfo>
#include <QNetworkInterface>

_StartProgram::_StartProgram()
{

}
_StartProgram::~_StartProgram()
{
    qDebug()<<"DELETE _STARTPROGRAM"<<endl;
}
QString _StartProgram::getMacAddress()
{
    /*
    foreach(QNetworkInterface netInterface, QNetworkInterface::allInterfaces())
    {
        // Return only the first non-loopback MAC Address
        if (!(netInterface.flags() & QNetworkInterface::IsLoopBack))
            return netInterface.hardwareAddress();
    }*/
    QProcess process;
    process.start("getmac /v /nh");
    process.waitForFinished(-1);
    QString out = QString(process.readAllStandardOutput().toStdString().c_str());
    QStringList list = out.split(QRegExp("\\s+"));
    /*
    for(int i=0; i<list.size(); i++){
        qDebug()<<list[i]<<endl;
    }
    */
    if(list.size() > 5){
        out = list[5];
        qDebug()<<out<<endl;
    }
    return out;
}
void _StartProgram::run()
{
    /*
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("DRIVER={Microsoft Excel Driver (*.xls, *.xlsx, *.xlsm, *.xlsb)};DBQ=" + QString("D:\\file.xlsx"));
    if(db.open())
    {
        qDebug()<<"ok excel"<<endl;
        QSqlQuery query; // Select range, place A1:B5 after $
        query.exec("SELECT * FROM [Hoja1$A1:C5]");
        while (query.next()){
            QString column1= query.value(0).toString();
            QString column2= query.value(1).toString();
            QString column3= query.value(2).toString();
            qDebug() << column1 << endl;
            qDebug() << column2 << endl;
            qDebug() << column3 << endl;
        }
    }
    */
    // Cargar config database (configuracion de base de datos de un archivo)
    //DATABASE_CONFIG(C_DB_FILE_CONFIG_NAME);

    QString text = getMacAddress();
    /*
    foreach(QNetworkInterface interfac, QNetworkInterface::allInterfaces())
    {
        text = interfac.hardwareAddress();
        qDebug()<<text<<endl;
    }

    foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost))
             qDebug() << address.toString();
    }
    */
    //qDebug()<<"entro"<<endl;
    if(text.compare("D8-50-E6-3D-7B-B0") == 0
            && QDate::currentDate() >= QDate(2017,5,1) && QDate::currentDate() <= QDate(2017,6,6)){

    }else{
        QWidget* w = new QWidget();
        w->setAttribute(Qt::WA_DeleteOnClose);
        w->show();
        return;
    }

    APP_LOGIN->setAttribute(Qt::WA_DeleteOnClose);
    APP_LOGIN->show();
}
