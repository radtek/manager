#ifndef PERSONACLIENTEDNI_H
#define PERSONACLIENTEDNI_H

#include "facturacion_global.h"
#include "persona_headers.h"

namespace Ui {
class PersonaClienteDNI;
}

class FACTURACIONSHARED_EXPORT PersonaClienteDNI : public QWidget
{
    Q_OBJECT

public:
    explicit PersonaClienteDNI(QWidget *parent = 0);
    ~PersonaClienteDNI();

    QString get_ID();
    QString get_DNI();
    QString get_nombre();
    QString get_direccion();

    void set_widget_previous(QWidget* widget_previous);

    int getOp(){ return op; }

    void set_captcha();

    bool select(const QString& id);

    bool guardar();

    bool remove();

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
    Ui::PersonaClienteDNI *ui;

    bool afterShow;

    ReniecConsultaDNI consulta_dni;

    QWidget* widget_previous;

    int op;

    QString id;
};

#endif // PERSONACLIENTEDNI_H
