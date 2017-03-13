#ifndef COMPRAFLETE_H
#define COMPRAFLETE_H

#include "compra_global.h"

namespace Ui {
class CompraFlete;
}

class COMPRASHARED_EXPORT CompraFlete : public QWidget
{
    Q_OBJECT

public:
    explicit CompraFlete(QWidget *parent = 0);
    ~CompraFlete();

    QString getID(){ return id; }
    QString getPersonaID(){ return persona_id; }
    QString getFechaDeclaracion(){ return fecha_declaracion; }
    QString getFechaEmision(){ return fecha_emision; }
    QString getSerie(){ return serie; }
    QString getNumero(){ return numero; }
    QString getCodigo(){ return codigo; }
    QString getNombre(){ return nombre; }
    QVector<QVector<QString> > getFacturas(){ return facturas; }
    QVector<QVector<QString> > getBoletas(){ return boletas; }

    enum{INDEX_ID = 0, INDEX_CANTIDAD, INDEX_UNIDAD, INDEX_DESCRIPCION, INDEX_P_UNIT
          , INDEX_P_TOTAL, INDEX_P_U_FLETE, INDEX_P_T_FLETE};

    void set_widget_previous(QWidget* w);

    bool select(QString id
                , QString persona_id, QString fecha_emision
                , QString serie, QString numero
                , QString codigo, QString nombre);

    void set_transportista(QString persona_id, QString codigo, QString nombre);

    void set_producto(QString producto_id
                      , QString cantidad, QString unidad
                      , QString descripcion, QString p_total
                      , QString p_total_flete);

private:
    bool guardar();

    bool remove();

private slots:
    void set_time();

    void on_transportista_closing();

    void on_pushButton_transportista_clicked();

    void on_pushButton_calcular_clicked();

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

signals:
    void showing();

    void hiding();

    void closing();

protected:
    void showEvent(QShowEvent *se);

    void closeEvent(QCloseEvent *ce);

    bool eventFilter(QObject *obj, QEvent *e);

private:
    Ui::CompraFlete *ui;

    bool afterShow;

    QWidget* widget_previous;

    int mes_declaracion;
    int mes_emision;

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

#endif // COMPRAFLETE_H
