#ifndef UNIDADBUSCAR_H
#define UNIDADBUSCAR_H

#include "producto_global.h"

namespace Ui {
class UnidadBuscar;
}

class PRODUCTOSHARED_EXPORT UnidadBuscar : public QWidget
{
	Q_OBJECT
public:
    explicit UnidadBuscar(QWidget *parent = 0);
    ~UnidadBuscar();

	QString getID();
	QString getUnidad();

	void set_widget_previous(QWidget* widget_previous);

private slots:
	void on_unidad_closing();

	void on_lineEdit_unidad_buscar_textChanged(const QString& arg);

	void on_lineEdit_unidad_buscar_returnPressed();

	void on_pushButton_nuevo_clicked();

	void on_pushButton_ok_clicked();

	void on_pushButton_salir_clicked();

    void on_tableWidget_itemDoubleClicked(QTableWidgetItem *item);

private:
	void showEvent(QShowEvent *se);

	void closeEvent(QCloseEvent * ce);

	bool eventFilter(QObject *obj, QEvent *e);

signals:
	void closing();

private:
	Ui::UnidadBuscar *ui;

    bool firstShow;
    bool afterShow;

	QWidget* widget_previous;

	int pos;
	int size_query;

	QString id;
	QString unidad;
};

#endif // UNIDADBUSCAR_H
