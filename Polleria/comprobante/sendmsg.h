#ifndef SENDMSG_H
#define SENDMSG_H

#include "mylibrary/mylibrary_global.h"

namespace Ui {
class SendMsg;
}

class MYLIBRARYSHARED_EXPORT SendMsg : public QWidget
{
    Q_OBJECT

public:
    explicit SendMsg(QWidget *parent = 0);
    ~SendMsg();

    void print_comanda();

private slots:
    void on_plainTextEdit_textChanged();

private:
    Ui::SendMsg *ui;
};

#endif // SENDMSG_H
