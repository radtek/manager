#ifndef CLIENTEDNI_H
#define CLIENTEDNI_H

#include "facturacion_global.h"
#include "persona_headers.h"

namespace Ui {
class ClienteDNI;
}

class FACTURACIONSHARED_EXPORT ClienteDNI : public QWidget
{
    Q_OBJECT
public:
    explicit ClienteDNI(QWidget *parent = 0);
    ~ClienteDNI();

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
    Ui::ClienteDNI *ui;

    bool afterShow;

	ReniecConsultaDNI consulta_dni;

    QWidget* widget_previous;

    int op;

    QString id;

	
};

#endif // CLIENTEDNI_H
