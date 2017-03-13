#ifndef COMPRABUSCAR_H
#define COMPRABUSCAR_H

#include "compra_global.h"

namespace Ui {
class CompraBuscar;
}

class COMPRASHARED_EXPORT CompraBuscar : public QWidget
{
    Q_OBJECT

public:
    enum{BUSCAR_DETALLE, AGREGAR_A_LISTA};
public:
    explicit CompraBuscar(QWidget *parent = 0);
    ~CompraBuscar();

    QString getID(){ return id; }
    QString getPersonaID(){ return persona_id; }
    QString getFechaEmision(){ return fecha_emision; }
    QString getSerie(){ return serie; }
    QString getNumero(){ return numero; }
    QString getCodigo(){ return codigo; }
    QString getNombre(){ return nombre; }

    void setTipoOrden();
    void setTipoFactura();
    void setTipoBoleta();
    void setTipoGuiaRR();
    void setTipoFlete();
    void setTipoNotaCredito();
    void setTipoNotaDebito();
    void setTipoSaldo();
    void setTipoRegSinDoc();

    void hideOptOrden();
    void hideOptFactura();
    void hideOptBoleta();
    void hideOptGuiaRR();
    void hideOptFlete();
    void hideOptNotaCredito();
    void hideOptNotaDebito();
    void hideOptSaldo();
    void hideOptRegSinDoc();

    void set_tipo(int tipo);

    void set_widget_previous(QWidget* w);

    void set_ruc(QString ruc);

private slots:
    void on_compra_closing();

    void on_lineEdit_buscar_textEdited(const QString &arg1);

    void on_lineEdit_buscar_returnPressed();

    void on_pushButton_ok_clicked();

    void on_pushButton_salir_clicked();

    void on_pushButton_agregar_clicked(); 

    void on_radioButton_orden_clicked();

    void on_radioButton_factura_clicked();

    void on_radioButton_boleta_clicked();

    void on_radioButton_guia_clicked();

    void on_radioButton_flete_clicked();

    void on_radioButton_nota_credito_clicked();

    void on_radioButton_nota_debito_clicked();

    void on_radioButton_saldo_clicked();

    void on_radioButton_reg_sin_doc_clicked();

    void on_tableWidget_itemDoubleClicked(QTableWidgetItem *item);

protected:
    bool eventFilter(QObject *obj, QEvent *e);

    void showEvent(QShowEvent *event);

    void closeEvent(QCloseEvent *event);

signals:
    void closing();

private:
    Ui::CompraBuscar *ui;

    bool firstShow;
    bool afterShow;

    QWidget* widget_previous;

    bool modo_only_date;

    int tipo;

    int pos;
    int size_query;

    QString id;
    QString persona_id;

    QString fecha_emision;
    QString serie;
    QString numero;
    QString codigo;
    QString nombre;

};

#endif // COMPRABUSCAR_H
