#ifndef TRANSPORTISTAOP_H
#define TRANSPORTISTAOP_H

#include "facturacion_global.h"
#include "persona_headers.h"

namespace Ui {
class TransportistaOp;
}

class FACTURACIONSHARED_EXPORT TransportistaOp : public QWidget
{
    Q_OBJECT
public:
    explicit TransportistaOp(QWidget *parent = 0);
    ~TransportistaOp();

    QString get_ID();
    QString get_ruc();
    QString get_razon_social();
    QString get_direccion();

    void set_widget_previous(QWidget* widget_previous);

    int getOp(){ return op; }

    void set_captcha();    

    bool select(const QString& id);

    void set_establecimiento(QString codigo, QString tipo, QString direccion, QString actividad);

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
    Ui::TransportistaOp *ui;

    bool afterShow;

    QWidget* widget_previous;

    int op;

    SunatConsultaRUC consulta_ruc;

    QString id;


};

#endif // TRANSPORTISTAOP_H
