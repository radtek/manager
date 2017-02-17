#ifndef UNIDAD_H
#define UNIDAD_H

#include "producto_global.h"

namespace Ui {
class Unidad;
}

class PRODUCTOSHARED_EXPORT Unidad : public QWidget
{
	Q_OBJECT
public:
    explicit Unidad(QWidget *parent = 0);
    ~Unidad();

	QString getID();
	QString getUnidad();

	void set_widget_previous(QWidget* widget_previous);

	void set_data(QString id, QString unidad);

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
	Ui::Unidad *ui;

	QWidget* widget_previous;

	QString id;
};

#endif // UNIDAD_H
