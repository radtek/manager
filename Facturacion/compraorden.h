#ifndef COMPRAORDEN_H
#define COMPRAORDEN_H

#include "facturacion_global.h"
#include "compra_headers.h"

namespace Ui {
class CompraOrden;
}

class FACTURACIONSHARED_EXPORT CompraOrden : public QWidget
{
    Q_OBJECT

public:
    explicit CompraOrden(QWidget *parent = 0);
    ~CompraOrden();

    QString getID();
    QString getPersonaID();
    QString getFechaEmision();
    QString getSerie();
    QString getNumero();
    QString getCodigo();
    QString getNombre();

    void set_igv(double igv){ this->igv = igv; }

    void set_dolar(double dolar){ this->dolar = dolar; }

    enum{INDEX_ID=0, INDEX_CANTIDAD, INDEX_UNIDAD, INDEX_DESCRIPCION
         , INDEX_P_UNIT, INDEX_P_TOTAL};

    void set_widget_previous(QWidget* w);

    int getOp(){ return op; }

    bool select(QString id
                , QString proveedor_id, QString fecha_emision
                , QString serie, QString numero
                , QString codigo, QString nombre);

    void set_proveedor(QString persona_id, QString codigo, QString nombre);

    void set_producto(QString producto_id
                      , QString cantidad, QString unidad
                      , QString descripcion, QString p_total);

private:    
    bool guardar();

    bool remove();

private slots:
    void set_time();

    void on_proveedor_closing();

    void on_producto_closing();

    void on_pushButton_proveedor_clicked();    

    void on_pushButton_ing_prod_clicked();

    void on_comboBox_modalidad_currentIndexChanged(int index);

    void on_comboBox_moneda_currentIndexChanged(int index);

    void on_tableWidget_itemChanged(QTableWidgetItem *item);

    void on_pushButton_amarres_clicked();

    void on_pushButton_guardar_clicked();

    void on_pushButton_eliminar_clicked();

    void on_pushButton_salir_clicked();

    void on_dateTimeEdit_emision_dateChanged(const QDate &date);    

    void on_lineEdit_cod_textEdited(const QString &arg1);

    void on_pushButton_down_clicked();

    void on_pushButton_up_clicked();

    void on_pushButton_borrar_clicked();

    void on_pushButton_canjear_clicked();

signals:
    void showing();

    void hiding();

    void closing();

protected:
    void showEvent(QShowEvent *se);

    void closeEvent(QCloseEvent *ce);

    bool eventFilter(QObject *obj, QEvent *e);

private:
    Ui::CompraOrden *ui;        

    bool afterShow;

    QWidget* widget_previous;

    int op;

    int mes_emision;

    double mes;
    double igv;
    double dolar;

    bool hasIGV;
    bool hasRate;

    QSqlQuery query;

    QString id;
    QString persona_id;
    QString fecha_emision;
    QString serie;
    QString numero;
    QString codigo;
    QString nombre;
};

#endif // COMPRAORDEN_H
