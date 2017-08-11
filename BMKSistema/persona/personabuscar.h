#ifndef PERSONABUSCAR_H
#define PERSONABUSCAR_H

#include "persona_headers.h"

namespace Ui {
class PersonaBuscar;
}

class MYLIBRARYSHARED_EXPORT PersonaBuscar : public QWidget
{
    Q_OBJECT

public:
    explicit PersonaBuscar(QWidget *parent = 0);
    ~PersonaBuscar();

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
    void on_verticalScrollBar_actionTriggered(int value);

    void on_colaborador_closing();

private:
    void set_buscarNombre();

    void set_buscarCodigo();

public slots:
    void on_lineEdit_buscarNombre_textChanged(const QString& arg);

    void on_lineEdit_buscarNombre_returnPressed();

    void on_pushButton_ok_clicked();

    void on_pushButton_salir_clicked();

    void on_pushButton_nuevo_clicked();

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

    void on_lineEdit_buscarCodigo_textChanged(const QString &arg1);

    void on_lineEdit_buscarCodigo_returnPressed();

private:
    void editarItem(QTableWidgetItem* item);

private:
    Ui::PersonaBuscar *ui;

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

#endif // PERSONABUSCAR_H
