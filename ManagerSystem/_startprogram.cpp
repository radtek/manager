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


    // Cargar config database (configuracion de base de datos de un archivo)
    DATABASE_CONFIG(C_DB_FILE_CONFIG_NAME);

    qDebug()<<"entro"<<endl;
    // Intentando conectar
    if(DATABASE_CONNECT)// Se conecto a la base de datos.
    {
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

