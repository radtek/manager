#ifndef VENTABOLETA_H
#define VENTABOLETA_H

#include "facturacion_global.h"
#include "venta_headers.h"

namespace Ui {
class VentaBoleta;
}

class FACTURACIONSHARED_EXPORT VentaBoleta : public QWidget
{
    Q_OBJECT
public:
    explicit VentaBoleta(QWidget *parent = 0);
    ~VentaBoleta();

    QString getID(){ return id; }
    QString getPersonaID(){ return persona_id; }
    QString getTipoPersonaID(){ return tipo_persona_id; }
    QString getFechaDeclaracion(){ return fecha_declaracion; }
    QString getFechaEmision(){ return fecha_emision; }
    QString getSerie(){ return serie; }
    QString getNumero(){ return numero; }
    QString getCodigo(){ return codigo; }
    QString getNombre(){ return nombre; }
    QString getDireccion(){ return direccion; }

    enum{INDEX_ID = 0, INDEX_CANTIDAD, INDEX_UNIDAD, INDEX_DESCRIPCION, INDEX_P_UNIT
          , INDEX_P_TOTAL};

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
                      , QString descripcion, QString p_total);

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

    void on_pushButton_anular_clicked();

    void on_dateEdit_declaracion_dateChanged(const QDate &date);

    void on_dateEdit_emision_dateChanged(const QDate &date);

    void on_registroSinDoc_buscar_closing();

    void on_notaPedido_buscar_closing();

    void on_cotizacion_buscar_closing();

    void on_guiarr_buscar_closing();

    void on_pushButton_buscar_registroSinDoc_clicked();

    void on_pushButton_jalar_registroSinDoc_clicked();

    void on_pushButton_quitar_registroSinDoc_clicked();

    void on_pushButton_buscar_notaPedido_clicked();

    void on_pushButton_jalar_notaPedido_clicked();

    void on_pushButton_quitar_notaPedido_clicked();

    void on_pushButton_buscar_cotizacion_clicked();

    void on_pushButton_jalar_cotizacion_clicked();

    void on_pushButton_quitar_cotizacion_clicked();

    void on_pushButton_buscar_guiarr_clicked();

    void on_pushButton_jalar_guiarr_clicked();

    void on_pushButton_quitar_guiarr_clicked();

protected:
    void showEvent(QShowEvent *se);

    void hideEvent(QHideEvent *he);

    void closeEvent(QCloseEvent *ce);

    bool eventFilter(QObject *obj, QEvent *e);

signals:
    void hiding();

    void closing();

private:
    Ui::VentaBoleta *ui;

    QWidget* widget_previous;

    QSqlQuery query;

    int mes_declaracion;
    int mes_emision;

    QString id;
    QString persona_id;
    QString tipo_persona_id;

    QString fecha_declaracion;
    QString fecha_emision;
    QString serie;
    QString numero;
    QString codigo;
    QString nombre;
    QString direccion;

    QString id_series;

    QString share_id;

    int anulado;

    /*
    QVector<QVector<QString> > reg_sin_docs;
    QVector<QVector<QString> > notas_pedido;
    QVector<QVector<QString> > cotizaciones;
    */
};

#endif // VENTABOLETA_H
