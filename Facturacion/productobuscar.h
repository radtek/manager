#ifndef PRODUCTOBUSCAR_H
#define PRODUCTOBUSCAR_H

#include "facturacion_global.h"
#include "producto_headers.h"

//#include "comprachartcosto.h"
//#include "ventachartprecio.h"

namespace Ui {
class ProductoBuscar;
}

class FACTURACIONSHARED_EXPORT ProductoBuscar : public QWidget
{
	Q_OBJECT
public:
    explicit ProductoBuscar(QWidget *parent = 0);
    ~ProductoBuscar();

	QString getID();
	QString getIDTipo();
	QString getIDMarca();
	QString getIDUnidad();

    QString getTipo();
    QString getMarca();
    QString getUnidad();
	QString getCodigo();
	QString getDescripcion();
	QString getPrecio();
	QString getCantidad();

    void set_widget_previous(QWidget* widget_previous);    

public slots:
    void on_verticalScrollBar_actionTriggered(int value);

    void on_verticalScrollBar_valueChanged(int value);

	void on_productoFormTransaction_closing();

    void on_pushButton_ok_clicked();

    void on_pushButton_salir_clicked();    

	void on_pushButton_nuevo_clicked();

    void on_pushButton_editar_clicked();

    void on_tableWidget_itemDoubleClicked(QTableWidgetItem *item);

private:
    void set_buscar_codigo();

    void set_buscar();

public slots:
	void on_lineEdit_codigo_textChanged(const QString& arg);

	void on_lineEdit_codigo_returnPressed();

	void on_lineEdit_tipo_buscar_textChanged(const QString& arg);

	void on_lineEdit_tipo_buscar_returnPressed();

	void on_lineEdit_descripcion_buscar_textChanged(const QString& arg);

	void on_lineEdit_descripcion_buscar_returnPressed();

	void on_lineEdit_marca_buscar_textChanged(const QString& arg);

	void on_lineEdit_marca_buscar_returnPressed();

	void on_lineEdit_unidad_buscar_textChanged(const QString& arg);

	void on_lineEdit_unidad_buscar_returnPressed();    

protected:
    void showEvent(QShowEvent *se);

	void closeEvent(QCloseEvent *ce);

    bool eventFilter(QObject *obj, QEvent *e);

signals:
	void closing();

private:
    void editarItem(QTableWidgetItem* item);

private:
    Ui::ProductoBuscar *ui;

    bool firstShow;
    bool afterShow;

    QWidget* widget_previous;

	int pos;
	int size_query;

	QString id;	
	QString id_tipo;	
	QString id_marca;
	QString id_unidad;

	QString codigo;
	QString tipo;
	QString descripcion;
	QString marca;
	QString unidad;
	QString precio;
	QString cantidad;
};

#endif // PRODUCTOBUSCAR_H
