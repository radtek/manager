#include "locallibrary.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    _StartProgram* s = new _StartProgram;
    s->run();


    return a.exec();
}
