#ifndef COMPRAGUIARR_H
#define COMPRAGUIARR_H

#include "compra_global.h"

namespace Ui {
class CompraGuiaRR;
}

class COMPRASHARED_EXPORT CompraGuiaRR : public QWidget
{
    Q_OBJECT

public:
    explicit CompraGuiaRR(QWidget *parent = 0);
    ~CompraGuiaRR();

    QString getID(){ return id; }
    QString getPersonaID(){ return persona_id; }
    QString getFechaEmision(){ return fecha_emision; }
    QString getSerie(){ return serie; }
    QString getNumero(){ return numero; }
    QString getCodigo(){ return codigo; }
    QString getNombre(){ return nombre; }

    enum{INDEX_ID = 0, INDEX_CANTIDAD, INDEX_UNIDAD, INDEX_DESCRIPCION};

    void set_widget_previous(QWidget* w);

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

    void on_pushButton_dame_productos_izq_clicked();

    void on_pushButton_dame_productos_der_clicked();

    void on_lineEdit_cod_textEdited(const QString &arg1);

    void on_pushButton_down_clicked();

    void on_pushButton_up_clicked();

    void on_pushButton_borrar_clicked();

    void on_pushButton_amarres_clicked();

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

    QSqlQuery query;    

    QString id;
    QString persona_id;
    QString fecha_emision;
    QString serie;
    QString numero;
    QString codigo;
    QString nombre;

    int pos_dame_productos;
};

#endif // COMPRAGUIARR_H
