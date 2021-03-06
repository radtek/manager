#ifndef VENTABUSCAR_H
#define VENTABUSCAR_H

#include "facturacion_global.h"
#include "venta_headers.h"

namespace Ui {
class VentaBuscar;
}

class FACTURACIONSHARED_EXPORT VentaBuscar : public QWidget
{
    Q_OBJECT

public:
    explicit VentaBuscar(QWidget *parent = 0);
    ~VentaBuscar();

    QString getID(){ return id; }
    QString getPersonaID(){ return persona_id; }
    QString getTipoPersonaID(){ return tipo_persona_id; }
    QString getFechaEmision(){ return fecha_emision; }
    QString getSerie(){ return serie; }
    QString getNumero(){ return numero; }
    QString getCodigo(){ return codigo; }
    QString getNombre(){ return nombre; }

    void set_tipo(int tipo);

    void set_widget_previous(QWidget* w);

    void set_ruc(QString ruc);

private slots:
    void on_compra_closing();

private:
    void set_buscar();

private slots:
    void on_lineEdit_buscarNombre_textChanged(const QString &arg1);

    void on_lineEdit_buscarNombre_returnPressed();

    void on_pushButton_ok_clicked();

    void on_pushButton_salir_clicked();

    void on_pushButton_nuevo_clicked();

    void on_radioButton_reg_sin_doc_clicked();

    void on_radioButton_boleta_clicked();

    void on_radioButton_factura_clicked();

    void on_radioButton_nota_pedido_clicked();

    void on_radioButton_guia_clicked();

    void on_radioButton_cotizacion_clicked();

    void on_radioButton_nota_credito_clicked();

    void on_radioButton_nota_debito_clicked();

    void on_pushButton_editar_clicked();

    void on_tableWidget_itemDoubleClicked(QTableWidgetItem *item);

    void on_lineEdit_buscarCodigo_textChanged(const QString &arg1);

    void on_lineEdit_buscarCodigo_returnPressed();

protected:
    bool eventFilter(QObject *obj, QEvent *e);

    void showEvent(QShowEvent *event);

    void closeEvent(QCloseEvent *event);

signals:
    void closing();

private:
    void editarItem(QTableWidgetItem* item);

private:
    Ui::VentaBuscar *ui;

    QWidget* widget_previous;

    bool firstShow;
    bool afterShow;

    bool modo_only_date;

    int tipo;

    int pos;
    int size_query;

    QString id;
    QString persona_id;
    QString tipo_persona_id;

    QString fecha_emision;
    QString serie;
    QString numero;
    QString codigo;
    QString nombre;
    QString direccion;
};

#endif // VENTABUSCAR_H
