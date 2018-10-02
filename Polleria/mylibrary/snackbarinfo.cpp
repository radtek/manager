#include "snackbarinfo.h"
#include "ui_snackbarinfo.h"

SnackBarInfo::SnackBarInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SnackBarInfo)
{
    ui->setupUi(this);

    this->setAttribute(Qt::WA_DeleteOnClose);
    timer = NULL;

}

SnackBarInfo::~SnackBarInfo()
{
    if(timer){
        delete timer;
    }
    delete ui;
}
void SnackBarInfo::set_data(QString msg, QString urlIcon)
{
    ui->label_msg->setText(msg);
    ui->label_icon->setPixmap(QPixmap::fromImage(QImage(urlIcon)));
    timer = new QTimer;
    //connect(timer, SIGNAL(timeout()), this, SLOT(on_timer_timeout()));
    //timer->start(2400);
}

void SnackBarInfo::on_timer_timeout()
{
    //this->close();
}
