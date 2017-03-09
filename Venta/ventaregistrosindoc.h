#ifndef VENTAREGISTROSINDOC_H
#define VENTAREGISTROSINDOC_H

#include "venta_global.h"

#include "comprachartcosto.h"

namespace Ui {
class VentaRegistroSinDoc;
}

class VENTASHARED_EXPORT VentaRegistroSinDoc : public QWidget
{
    Q_OBJECT

public:
    explicit VentaRegistroSinDoc(QWidget *parent = 0);
    ~VentaRegistroSinDoc();

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

    void on_dateTimeEdit_emision_dateChanged(const QDate &date);

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

protected:
    void showEvent(QShowEvent *se);

    void hideEvent(QHideEvent *he);

    void closeEvent(QCloseEvent *ce);

    bool eventFilter(QObject *obj, QEvent *e);

signals:
    void hiding();

    void closing();

private:
    Ui::VentaRegistroSinDoc *ui;

    QWidget* widget_previous;

    QSqlQuery query;

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
};

#endif // VENTAREGISTROSINDOC_H
