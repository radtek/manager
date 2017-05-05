#ifndef COMPRAGUIARR_H
#define COMPRAGUIARR_H

#include "facturacion_global.h"
#include "compra_headers.h"

namespace Ui {
class CompraGuiaRR;
}

class FACTURACIONSHARED_EXPORT CompraGuiaRR : public QWidget
{
    Q_OBJECT

public:
    explicit CompraGuiaRR(QWidget *parent = 0);
    ~CompraGuiaRR();

    QString getID();
    QString getPersonaID();
    QString getFechaEmision();
    QString getSerie();
    QString getNumero();
    QString getCodigo();
    QString getNombre();
    QVector<QVector<QString> > getFacturas();
    QVector<QVector<QString> > getBoletas();

    enum{INDEX_ID = 0, INDEX_CANTIDAD, INDEX_UNIDAD, INDEX_DESCRIPCION};

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

    void on_tableWidget_itemChanged(QTableWidgetItem *item);

    void on_pushButton_guardar_clicked();

    void on_pushButton_eliminar_clicked();

    void on_pushButton_salir_clicked();

    void on_pushButton_down_clicked();

    void on_pushButton_up_clicked();

    void on_pushButton_borrar_clicked();

    void on_pushButton_amarres_clicked();

    void on_factura_buscar_closing();

    void on_boleta_buscar_closing();

    void on_pushButton_buscar_factura_clicked();

    void on_pushButton_jalar_factura_clicked();

    void on_pushButton_quitar_factura_clicked();

    void on_pushButton_buscar_boleta_clicked();

    void on_pushButton_jalar_boleta_clicked();

    void on_pushButton_quitar_boleta_clicked();

    void on_pushButton_canjear_clicked();

    void on_lineEdit_cod_textChanged(const QString &arg1);

signals:
    void closing();

protected:
    void showEvent(QShowEvent *se);

    void closeEvent(QCloseEvent *ce);

    bool eventFilter(QObject *obj, QEvent *e);

private:
    Ui::CompraGuiaRR *ui;

    bool afterShow;

    QWidget* widget_previous;

    int op;

    QSqlQuery query;    

    QString id;
    QString persona_id;
    QString fecha_emision;
    QString serie;
    QString numero;
    QString codigo;
    QString nombre;

    QVector<QVector<QString> > facturas;
    QVector<QVector<QString> > boletas;

    int pos_dame_productos;
};

#endif // COMPRAGUIARR_H
