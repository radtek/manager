#ifndef COLABORADORBUSCAR_H
#define COLABORADORBUSCAR_H

#include "facturacion_global.h"
#include "persona_headers.h"

namespace Ui {
class ColaboradorBuscar;
}

class FACTURACIONSHARED_EXPORT ColaboradorBuscar : public QWidget
{
    Q_OBJECT
public:
    explicit ColaboradorBuscar(QWidget *parent = 0);
    ~ColaboradorBuscar();

    void set_tipo(int tipo);

    void set_widget_previous(QWidget* widget_previous);

    QString get_ID();
    QString get_codigo();
    QString get_nombre();
    QString get_direccion();

    void setTipoProveedor();
    void setTipoTransportista();
    void setTipoClienteRUC();
    void setTipoClienteDNI();
    void setTipoUsuario();

    void hideOptProveedor();
    void hideOptTransportista();
    void hideOptClienteRUC();
    void hideOptClienteDNI();
    void hideOptUsuario();

public slots:
    void on_colaborador_closing();

private:
    void set_buscar();

public slots:
    void on_lineEdit_buscar_textChanged(const QString& arg);

    void on_lineEdit_buscar_returnPressed();

    void on_pushButton_ok_clicked();

    void on_pushButton_salir_clicked();

    void on_pushButton_agregar_clicked();

    void on_tableWidget_itemDoubleClicked(QTableWidgetItem *item);

    void on_radioButton_proveedor_clicked();

    void on_radioButton_transportista_clicked();

    void on_radioButton_cliente_ruc_clicked();

    void on_radioButton_cliente_dni_clicked();

    void on_radioButton_usuario_clicked();

protected:
    void showEvent(QShowEvent *se);

    void hideEvent(QHideEvent *he);

    void closeEvent(QCloseEvent *ce);

    bool eventFilter(QObject *obj, QEvent *e);

private slots:
    void on_pushButton_editar_clicked();

private:
    void editarItem(QTableWidgetItem* item);

private:
    Ui::ColaboradorBuscar *ui;

    bool firstShow;
    bool afterShow;

    QWidget* widget_previous;

    int tipo;

    QString id;
    QString codigo;
    QString nombre;
    QString direccion;

    int pos;
    int size_query;
};

#endif // COLABORADORBUSCAR_H
