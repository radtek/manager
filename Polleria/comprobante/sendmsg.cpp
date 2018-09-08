#include "sendmsg.h"
#include "ui_sendmsg.h"

SendMsg::SendMsg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SendMsg)
{
    ui->setupUi(this);
}

SendMsg::~SendMsg()
{
    delete ui;
}

void SendMsg::on_plainTextEdit_textChanged()
{
    QString text = ui->plainTextEdit->toPlainText();

    qDebug() << text << endl;
}
