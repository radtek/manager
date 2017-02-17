#ifndef _STARTPROGRAM_H
#define _STARTPROGRAM_H

#include "locallibrary.h"

class _StartProgram : QWidget
{
    Q_OBJECT
public:
    _StartProgram();
    ~_StartProgram();       

    QWidget* widget_home;
    QWidget* widget_toolbar;

    void run();

public slots:
    void on_pb_login_aceptar();

private slots:

};

#endif // _STARTPROGRAM_H