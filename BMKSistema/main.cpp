#include "mylibrary/mylibrary_global.h"
#include "mylibrary/_startprogram.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);    

    QCoreApplication::setAttribute(Qt::AA_UseSoftwareOpenGL);

    _StartProgram* s = new _StartProgram;
    s->run();

    return a.exec();
}
