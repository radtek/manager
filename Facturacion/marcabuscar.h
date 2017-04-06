#ifndef MARCABUSCAR_H
#define MARCABUSCAR_H

#include "facturacion_global.h"
#include "producto_headers.h"

namespace Ui {
class MarcaBuscar;
}

class FACTURACIONSHARED_EXPORT MarcaBuscar : public QWidget
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

private:
    void set_buscar();

private slots:
	void on_lineEdit_marca_buscar_textChanged(const QString& arg);

	void on_lineEdit_marca_buscar_returnPressed();

	void on_pushButton_nuevo_clicked();

    void on_pushButton_editar_clicked();

    void on_tableWidget_itemDoubleClicked(QTableWidgetItem *item);

	void on_pushButton_ok_clicked();

	void on_pushButton_salir_clicked();

private:
	void showEvent(QShowEvent *se);

	void closeEvent(QCloseEvent * ce);

	bool eventFilter(QObject *obj, QEvent *e);

signals:
	void closing();

private:
    void editarItem(QTableWidgetItem* item);

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
