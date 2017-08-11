#ifndef PERSONAUSUARIO_H
#define PERSONAUSUARIO_H

#include "persona_headers.h"

namespace Ui {
class PersonaUsuario;
}

class MYLIBRARYSHARED_EXPORT PersonaUsuario : public QWidget
{
    Q_OBJECT

public:
    explicit PersonaUsuario(QWidget *parent = 0);
    ~PersonaUsuario();

    QString get_ID();
    QString get_DNI();
    QString get_nombre();
    QString get_ID_rol();
    QString get_rol();
    QString get_usuario();

    void set_widget_previous(QWidget* widget_previous);

    int getOp(){ return op; }

    bool guardar();

    bool remove();

    bool select(const QString& id_p);

private slots:
    void on_pushButton_guardar_clicked();

    void on_pushButton_eliminar_clicked();

    void on_pushButton_salir_clicked();

protected:
    void showEvent(QShowEvent *event);

    void closeEvent(QCloseEvent *event);

    bool eventFilter(QObject *obj, QEvent *e);

signals:
    void closing();

private:
    Ui::PersonaUsuario *ui;

    bool afterShow;

    QWidget* widget_previous;

    int op;

    QString id;

};

#endif // PERSONAUSUARIO_H
