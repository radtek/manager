#ifndef SNACKBARINFO_H
#define SNACKBARINFO_H

#include "mylibrary_global.h"

namespace Ui {
class SnackBarInfo;
}

class MYLIBRARYSHARED_EXPORT SnackBarInfo : public QWidget
{
    Q_OBJECT

public:
    explicit SnackBarInfo(QWidget *parent = 0);
    ~SnackBarInfo();

    void set_data(QString msg, QString urlIcon);

private slots:
    void on_timer_timeout();

private:
    Ui::SnackBarInfo *ui;

    QTimer* timer;

};

#endif // SNACKBARINFO_H

