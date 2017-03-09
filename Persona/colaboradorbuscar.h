#ifndef COLABORADORBUSCAR_H
#define COLABORADORBUSCAR_H

#include "persona_global.h"

namespace Ui {
class ColaboradorBuscar;
}

class PERSONASHARED_EXPORT ColaboradorBuscar : public QWidget
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

public slots:
    void on_colaborador_closing();

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

private:
    Ui::ColaboradorBuscar *ui;

public:
    //TemplateReporte* temp_rep;

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
