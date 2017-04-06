#ifndef COMPRANOTADEBITO_H
#define COMPRANOTADEBITO_H

#include "facturacion_global.h"
#include "compra_headers.h"

namespace Ui {
class CompraNotaDebito;
}

class FACTURACIONSHARED_EXPORT CompraNotaDebito : public QWidget
{
    Q_OBJECT

public:
    explicit CompraNotaDebito(QWidget *parent = 0);
    ~CompraNotaDebito();

    QString getID();
    QString getPersonaID();
    QString getFechaDeclaracion();
    QString getFechaEmision();
    QString getSerie();
    QString getNumero();
    QString getCodigo();
    QString getNombre();
    QVector<QVector<QString> > getFacturas();
    QVector<QVector<QString> > getBoletas();

    enum{INDEX_ID = 0, INDEX_CANTIDAD, INDEX_UNIDAD, INDEX_DESCRIPCION, INDEX_P_UNIT, INDEX_P_TOTAL
          , INDEX_AUMENTO_P_TOTAL};

    void set_widget_previous(QWidget* w);

    int getOp(){ return op; }

    bool select(QString id
                      , QString persona_id, QString fecha_emision
                      , QString serie, QString numero
                      , QString codigo, QString nombre);

    void set_proveedor(QString persona_id, QString codigo, QString nombre);

    void set_producto(QString producto_id
                      , QString cantidad, QString unidad
                      , QString descripcion, QString p_total
                      , QString aumento_p_total);

private:
    bool guardar();

    bool remove();

private slots:
    void set_time();

    void on_proveedor_closing();

    void on_pushButton_proveedor_clicked();

    void on_producto_closing();

    void on_pushButton_ing_prod_clicked();

    void on_pushButton_amarres_clicked();

    void on_pushButton_guardar_clicked();

    void on_pushButton_eliminar_clicked();

    void on_pushButton_salir_clicked();

    void on_tableWidget_itemChanged(QTableWidgetItem *item);

    void on_factura_buscar_closing();

    void on_boleta_buscar_closing();

    void on_pushButton_buscar_factura_clicked();

    void on_pushButton_buscar_boleta_clicked();

    void on_pushButton_jalar_factura_clicked();

    void on_pushButton_jalar_boleta_clicked();

    void on_pushButton_quitar_factura_clicked();

    void on_pushButton_quitar_boleta_clicked();

    void on_pushButton_up_clicked();

    void on_pushButton_down_clicked();

    void on_pushButton_borrar_clicked();

    void on_dateEdit_declaracion_dateChanged(const QDate &date);

    void on_dateTimeEdit_emision_dateChanged(const QDate &date);

    void on_lineEdit_codigo_textEdited(const QString &arg1);

    void on_comboBox_moneda_currentIndexChanged(int index);

    void on_comboBox_modalidad_currentIndexChanged(int index);    

    void on_pushButton_guardar_dolar_clicked();

    void on_lineEdit_monto_textChanged(const QString &arg1);

signals:
    void showing();

    void hiding();

    void closing();

protected:
    void showEvent(QShowEvent *se);

    void closeEvent(QCloseEvent *ce);

    bool eventFilter(QObject *obj, QEvent *e);

private:
    Ui::CompraNotaDebito *ui;

    bool afterShow;

    QWidget* widget_previous;

    int op;

    int mes_declaracion;
    int mes_emision;

    SunatTipoCambio sunat_cambio;

    double mes;
    double igv;
    double dolar;

    bool hasIGV;
    bool hasRate;

    QSqlQuery query;

    QString id;
    QString persona_id;
    QString fecha_declaracion;
    QString fecha_emision;
    QString serie;
    QString numero;
    QString codigo;
    QString nombre;

    QVector<QVector<QString> > facturas;
    QVector<QVector<QString> > boletas;
};

#endif // COMPRANOTADEBITO_H
