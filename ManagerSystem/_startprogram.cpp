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
    // Cargar config database (configuracion de base de datos de un archivo)
    DATABASE_CONFIG(C_DB_FILE_CONFIG_NAME);

    qDebug()<<"entro"<<endl;
    // Intentando conectar
    if(DATABASE_CONNECT)// Se conecto a la base de datos.
    {
        QString str_query;
        str_query += "SET autocommit = 0";

        QSqlQuery query;

        if(query.exec(str_query)){
            qDebug()<<"autocommit = 0"<<endl;
        }else{
            return;
        }

        connect(APP_LOGIN->pb_aceptar(), SIGNAL(clicked()), this, SLOT(on_pb_login_aceptar()));

        APP_LOGIN->setAttribute(Qt::WA_DeleteOnClose);
        APP_LOGIN->show();
    }else{
        OpcionesDB *db_form = new OpcionesDB;
        db_form->setAttribute(Qt::WA_DeleteOnClose);
        db_form->show();
    }

}

void _StartProgram::on_pb_login_aceptar()
{
    if(APP_LOGIN->online){
        qDebug()<<"ONLINE"<<endl;
        APP_LOGIN->close();

        APP_MAINWINDOW->setAttribute(Qt::WA_DeleteOnClose);

        APP_MAINWINDOW->set_toolBar(APP_TOOLBAR);

        APP_TOOLBAR->setContextMenuPolicy(Qt::PreventContextMenu);

        APP_MAINWINDOW->setCentralWidget(new Home);

        APP_MAINWINDOW->showMaximized();
    }
}

