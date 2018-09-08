#ifndef _STARTPROGRAM_H
#define _STARTPROGRAM_H

#include "mylibrary_global.h"
#include "shares.h"
//#include "logindialog.h"

class MYLIBRARYSHARED_EXPORT _StartProgram : QWidget
{
    Q_OBJECT
public:
    _StartProgram();
    ~_StartProgram();       

    QWidget* widget_home;
    QWidget* widget_toolbar;

    void run();

    QString getMacAddress();

public slots:

private slots:

};

#endif // _STARTPROGRAM_H
