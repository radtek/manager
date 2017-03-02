#ifndef VENTANOTACREDITO_H
#define VENTANOTACREDITO_H

#include "venta_global.h"

namespace Ui {
class VentaNotaCredito;
}

class VENTASHARED_EXPORT VentaNotaCredito : public QWidget
{
    Q_OBJECT

public:
    explicit VentaNotaCredito(QWidget *parent = 0);
    ~VentaNotaCredito();

    QString getID(){ return id; }
    QString getPersonaID(){ return persona_id; }
    QString getTipoPersonaID(){ return tipo_persona_id; }
    QString getFechaEmision(){ return fecha_emision; }
    QString getSerie(){ return serie; }
    QString getNumero(){ return numero; }
    QString getCodigo(){ return codigo; }
    QString getNombre(){ return nombre; }
    QString getDireccion(){ return direccion; }

    enum{INDEX_ID = 0, INDEX_CANTIDAD, INDEX_UNIDAD, INDEX_DESCRIPCION, INDEX_P_UNIT
          , INDEX_P_TOTAL, INDEX_DESC_CANT, INDEX_DESC_P_TOTAL};

    void set_widget_previous(QWidget* widget_previous);

private slots:
    void on_ventaSNSelector_closing();

public:
    void next_serie_numero();

    bool select(QString id
                , QString persona_id, QString tipo_persona_id
                , QString fecha_emision
                , QString serie, QString numero
                , QString codigo, QString nombre, QString direccion);

    void set_cliente(QString persona_id, QString codigo, QString nombre, QString direccion);

    void set_producto(QString producto_id
                      , QString cantidad, QString unidad
                      , QString descripcion, QString p_total
                      , QString desc_cantidad, QString desc_p_total);

    void set_data(QString persona_id, QString tipo_persona_id
                  , QDateTime dt_emision, QDateTime dt_sistema, QString serie, QString numero
                  , QString codigo, QString nombre, QString direccion
                  , QVector<QVector<QString>> productos);

    void set_data(int tipo, QString id, QString persona_id, QString tipo_persona_id
                   , QString fecha_emision, QString fecha_sistema, QString serie, QString numero
                   , QString codigo, QString nombre, QString direccion
                   , QVector<QVector<QString>> productos);

private:
    bool guardar();

    bool remove();

private slots:
    void set_time();

    void on_pushButton_transformar_clicked();

    void on_cliente_closing();

    void on_producto_closing();

    void on_pushButton_cliente_clicked();

    void on_pushButton_ing_prod_clicked();

    void on_pushButton_guardar_clicked();

    void on_pushButton_eliminar_clicked();

    void on_pushButton_imprimir_clicked();

    void on_pushButton_amarres_clicked();

    void on_pushButton_salir_clicked();

    void on_pushButton_up_clicked();

    void on_pushButton_down_clicked();

    void on_pushButton_borrar_clicked();

    void on_lineEdit_serie_returnPressed();

    void on_tableWidget_itemChanged(QTableWidgetItem *item);

    void on_boleta_buscar_closing();

    void on_pushButton_buscar_boleta_clicked();

    void on_pushButton_jalar_boleta_clicked();

    void on_pushButton_quitar_boleta_clicked();

    void on_factura_buscar_closing();

    void on_pushButton_buscar_factura_clicked();

    void on_pushButton_jalar_factura_clicked();

    void on_pushButton_quitar_factura_clicked();

    void on_pushButton_anular_clicked();

    void on_dateEdit_declaracion_dateChanged(const QDate &date);

    void on_dateEdit_emision_dateChanged(const QDate &date);

    void on_doubleSpinBox_monto_valueChanged(double arg1);

protected:
    void showEvent(QShowEvent *se);

    void hideEvent(QHideEvent *he);

    void closeEvent(QCloseEvent *ce);

    bool eventFilter(QObject *obj, QEvent *e);

signals:
    void hiding();

    void closing();

private:
    Ui::VentaNotaCredito *ui;

    QWidget* widget_previous;

    double igv;

    int mes_declaracion;
    int mes_emision;

    QString id;
    QString persona_id;
    QString tipo_persona_id;

    QString fecha_emision;
    QString serie;
    QString numero;
    QString codigo;
    QString nombre;
    QString direccion;

    QString id_series;

    QString share_id;

    int anulado;

};

#endif // VENTANOTACREDITO_H
