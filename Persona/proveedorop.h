#ifndef PROVEEDOROP_H
#define PROVEEDOROP_H

#include "persona_global.h"

namespace Ui {
class ProveedorOp;
}

class PERSONASHARED_EXPORT ProveedorOp : public QWidget
{
    Q_OBJECT
public:
    explicit ProveedorOp(QWidget *parent = 0);
    ~ProveedorOp();

    QString get_ID();
    QString get_ruc();
    QString get_razon_social();
    QString get_direccion();

    void set_widget_previous(QWidget* widget_previous);

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
    Ui::ProveedorOp *ui;

    QWidget* widget_previous;

    SunatConsultaRUC consulta_ruc;

    QString id;

};

#endif // PROVEEDOROP_H
