#include "_startprogram.h"

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
    //QStringList list = out.split(QRegExp("\\s+"));
    /*
    for(int i=0; i<list.size(); i++){
        qDebug()<<list[i]<<endl;
    }
    */
    /*if(list.size() > 5){
        out = list[5];
        qDebug()<<out<<endl;
    }*/
    qDebug()<<out<<endl;
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

    qDebug()<<"entro"<<endl;    
    // GETMAC DE LAPTOP COREI7: 60-45-CB-1D-7E-EF
    // GETMAC DE LAPTOP ALEX: 20-16-D8-E7-D1-BC
    // GETMAC DE BILL MAC QUACK de ALAS PERUANAS caja1: 00-16-35-66-C3-09
    // GETMAC DE BILL MAC QUACK de ALAS PERUANAS caja2: 90-FB-A6-04-14-21
    // GETMAC DE BILL MAC QUACK de ASA: 00-21-5A-65-88-B2
    // GETMAC DE MACHELO: 94-DE-80-BB-CB-5E
    // GETMAC DE LEAL: 70-85-C2-18-5C-9B
    if(text.indexOf("70-85-C2-18-5C-9B") != -1){
            //&& QDate::currentDate() >= QDate(2018,7,25) && QDate::currentDate() <= QDate(2019,9,1)){

    }else{

        QWidget* w = new QWidget();
        w->setAttribute(Qt::WA_DeleteOnClose);
        w->show();
        return;
    }

    APP_LOGIN->setAttribute(Qt::WA_DeleteOnClose);
    APP_LOGIN->show();
}
