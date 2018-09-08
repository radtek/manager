#ifndef LOGIN_H
#define LOGIN_H

#include "mylibrary_global.h"
#include "database_connection.h"
#include "opcionesdb.h"
#include "shares.h"
#include "persona/persona.h"
#include "mylibrary/cambiarcorreo.h"
#include "mylibrary/cambiarpass.h"
#include "mylibrary/enviarmecorreo.h"

class OpcionesDB;

namespace Ui {
class Login;
}

class MYLIBRARYSHARED_EXPORT Login : public QWidget
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

    void on_pushButton_cambiarCorreo_clicked();

    void on_pushButton_cambiarPass_clicked();

    void on_pushButton_enviarmeCorreo_clicked();

protected:
    void showEvent(QShowEvent *se);

    bool eventFilter(QObject *obj, QEvent *e);

private:
    Ui::Login *ui;

    OpcionesDB *opciones;

    bool afterShow;
};

template class Q_DECL_EXPORT singleton<Login>;

#endif // LOGIN_H
