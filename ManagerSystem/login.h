#ifndef LOGIN_H
#define LOGIN_H

#include "locallibrary.h"

namespace Ui {
class Login;
}

class Q_DECL_EXPORT Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);
    ~Login();

    bool online;

    QPushButton* pb_aceptar();

private slots:
    void on_pushButton_aceptar_clicked();

    void on_pushButton_cancelar_clicked();

protected:
    void showEvent(QShowEvent *se);

    bool eventFilter(QObject *obj, QEvent *e);

private:
    Ui::Login *ui;
};

template class Q_DECL_EXPORT singleton<Login>;

#endif // LOGIN_H
