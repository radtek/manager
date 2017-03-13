#ifndef TIPOBUSCAR_H
#define TIPOBUSCAR_H

#include "producto_global.h"

namespace Ui {
class TipoBuscar;
}

class PRODUCTOSHARED_EXPORT TipoBuscar : public QWidget
{
	Q_OBJECT
public:
	explicit TipoBuscar(QWidget *parent = 0);
	~TipoBuscar();

	QString getID();
	QString getTipo();

	void set_widget_previous(QWidget* widget_previous);

private slots:
	void on_tipo_closing();

	void on_lineEdit_tipo_buscar_textChanged(const QString& arg);
	
	void on_lineEdit_tipo_buscar_returnPressed();

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
	Ui::TipoBuscar *ui;

    bool firstShow;
    bool afterShow;

	QWidget* widget_previous;

	int pos;
	int size_query;

	QString id;
	QString tipo;
};

#endif // TIPOBUSCAR_H
