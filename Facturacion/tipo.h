#ifndef TIPO_H
#define TIPO_H

#include "facturacion_global.h"
#include "producto_headers.h"

namespace Ui {
class Tipo;
}

class FACTURACIONSHARED_EXPORT Tipo : public QWidget
{
	Q_OBJECT
public:
    explicit Tipo(QWidget *parent = 0);
    ~Tipo();

	QString getID();
	QString getTipo();

    void set_widget_previous(QWidget* widget_previous);

    int getOp(){ return op; }

	void set_data(QString id, QString tipo);

private:
    bool guardar();

    bool remove();

private slots:
    void on_pushButton_guardar_clicked();

    void on_pushButton_eliminar_clicked();

    void on_pushButton_salir_clicked();

protected:
    void showEvent(QShowEvent *se);

	void closeEvent(QCloseEvent *ce);

    bool eventFilter(QObject *obj, QEvent *e);

signals:
	void closing();

private:
    Ui::Tipo *ui;

    bool afterShow;

	QWidget* widget_previous;

    int op;

	QString id;
};

#endif // TIPO_H
