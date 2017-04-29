#include "_startprogram.h"

_StartProgram::_StartProgram()
{

}
_StartProgram::~_StartProgram()
{
    qDebug()<<"DELETE _STARTPROGRAM"<<endl;
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

    //qDebug()<<"entro"<<endl;
    if(QDate::currentDate() >= QDate(2017,4,7) && QDate::currentDate() <= QDate(2017,5,6)){

    }else{
        QWidget* w = new QWidget();
        w->setAttribute(Qt::WA_DeleteOnClose);
        w->show();
        return;
    }

    APP_LOGIN->setAttribute(Qt::WA_DeleteOnClose);
    APP_LOGIN->show();
}
