#ifndef COMPRABOLETA_H
#define COMPRABOLETA_H

#include "facturacion_global.h"
#include "compra_headers.h"

namespace Ui {
class CompraBoleta;
}

class FACTURACIONSHARED_EXPORT CompraBoleta : public QWidget
{
    Q_OBJECT

public:
    explicit CompraBoleta(QWidget *parent = 0);
    ~CompraBoleta();    

    enum{INDEX_ID = 0, INDEX_CANTIDAD, INDEX_UNIDAD, INDEX_DESCRIPCION, INDEX_P_UNIT
          , INDEX_P_TOTAL};

    QString getID();
    QString getPersonaID();
    QString getFechaDeclaracion();
    QString getFechaEmision();
    QString getSerie();
    QString getNumero();
    QString getCodigo();
    QString getNombre();
    QVector<QVector<QString> > getOrdenes();
    QVector<QVector<QString> > getGuias();

    void set_widget_previous(QWidget* w);

    int getOp(){ return op; }

    bool select(QString id
                , QString persona_id, QString fecha_emision
                , QString serie, QString numero
                , QString codigo, QString nombre);

public:
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

    void on_dateEdit_declaracion_dateChanged(const QDate &date);

    void on_dateTimeEdit_emision_dateChanged(const QDate &date);    

    void on_lineEdit_cod_textEdited(const QString &arg1);

    void on_pushButton_amarres_clicked();

    void on_orden_buscar_closing();

    void on_guia_buscar_closing();

    void on_pushButton_buscar_orden_clicked();

    void on_pushButton_buscar_guia_clicked();

    void on_pushButton_jalar_orden_clicked();

    void on_pushButton_jalar_guia_clicked();

    void on_pushButton_borrar_clicked();

    void on_pushButton_up_clicked();

    void on_pushButton_down_clicked();

    void on_pushButton_quitar_orden_clicked();

    void on_pushButton_quitar_guia_clicked();

    void on_pushButton_canjear_clicked();

protected:
    void showEvent(QShowEvent *se);

    void hideEvent(QHideEvent *he);

    void closeEvent(QCloseEvent *ce);

    bool eventFilter(QObject *obj, QEvent *e);

signals:
    void showing();

    void hiding();

    void closing();

private:
    Ui::CompraBoleta *ui;

    bool afterShow;

    QWidget* widget_previous;

    int op;

    int mes_declaracion;
    int mes_emision;

    bool firstShow;

    QSqlQuery query;

    QString id;
    QString persona_id;

    QString fecha_declaracion;
    QString fecha_emision;
    QString serie;
    QString numero;
    QString codigo;
    QString nombre;
    QVector<QVector<QString> > ordenes;
    QVector<QVector<QString> > guias;
};

#endif // COMPRABOLETA_H
