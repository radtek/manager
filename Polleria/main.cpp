#include "mylibrary/mylibrary_global.h"
#include "mylibrary/_startprogram.h"

#include <iostream>


using namespace std;

const char server[] = "https://e-factura.sunat.gob.pe/ol-ti-itcpfegem/billService?wsdl";

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //QCoreApplication::setAttribute(Qt::AA_UseSoftwareOpenGL);

    //QWidget w;
    //w.show();
    _StartProgram* s = new _StartProgram;
    s->run();

    return a.exec();
}
