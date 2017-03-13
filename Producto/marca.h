#ifndef MARCA_H
#define MARCA_H

#include "producto_global.h"

namespace Ui {
class Marca;
}

class PRODUCTOSHARED_EXPORT Marca : public QWidget
{
	Q_OBJECT
public:
    explicit Marca(QWidget *parent = 0);
    ~Marca();

	QString getID();
	QString getMarca();

	void set_widget_previous(QWidget* widget_previous);

	void set_data(QString id, QString marca);

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
	Ui::Marca *ui;

    bool afterShow;

	QWidget* widget_previous;

	QString id;
};

#endif // MARCA_H
