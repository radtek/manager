#ifndef PERSONAPROVEEDOR_H
#define PERSONAPROVEEDOR_H

#include "persona_headers.h"
#include "mylibrary/sunatconsultaruc.h"

namespace Ui {
class PersonaProveedor;
}

class MYLIBRARYSHARED_EXPORT PersonaProveedor : public QWidget
{
    Q_OBJECT

public:
    explicit PersonaProveedor(QWidget *parent = 0);
    ~PersonaProveedor();

    QString get_ID();
    QString get_ruc();
    QString get_razon_social();
    QString get_direccion();

    void set_widget_previous(QWidget* widget_previous);

    int getOp(){ return op; }

    void set_captcha();

    bool select(const QString& id_p);

    void set_establecimiento(QString codigo, QString tipo, QString direccion, QString actividad);

    bool guardar();

    bool remove();

private slots:
    void on_pushButton_guardar_clicked();

    void on_pushButton_eliminar_clicked();

    void on_pushButton_salir_clicked();

signals:
    void closing();

protected:
    void showEvent(QShowEvent *event);

    void closeEvent(QCloseEvent *event);

    bool eventFilter(QObject *obj, QEvent *e);

private:
    Ui::PersonaProveedor *ui;

    bool afterShow;

    QWidget* widget_previous;

    int op;

    SunatConsultaRUC consulta_ruc;

    QString id;
};

#endif // PERSONAPROVEEDOR_H
