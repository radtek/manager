#ifndef MARCABUSCAR_H
#define MARCABUSCAR_H

#include "producto_global.h"

namespace Ui {
class MarcaBuscar;
}

class PRODUCTOSHARED_EXPORT MarcaBuscar : public QWidget
{
	Q_OBJECT
public:
    explicit MarcaBuscar(QWidget *parent = 0);
    ~MarcaBuscar();

	QString getID();
	QString getMarca();

	void set_widget_previous(QWidget* widget_previous);

private slots:
	void on_marca_closing();

	void on_lineEdit_marca_buscar_textChanged(const QString& arg);

	void on_lineEdit_marca_buscar_returnPressed();

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
	Ui::MarcaBuscar *ui;

    bool firstShow;
    bool afterShow;

	QWidget* widget_previous;

	int pos;
	int size_query;

	QString id;
	QString marca;
};

#endif // MARCABUSCAR_H
