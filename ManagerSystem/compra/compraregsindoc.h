#ifndef COMPRAREGSINDOC_H
#define COMPRAREGSINDOC_H

#include "facturacion_global.h"
#include "compra_headers.h"

namespace Ui {
class CompraRegSinDoc;
}

class FACTURACIONSHARED_EXPORT CompraRegSinDoc : public QWidget
{
    Q_OBJECT

public:
    explicit CompraRegSinDoc(QWidget *parent = 0);
    ~CompraRegSinDoc();

    QString getID();
    QString getFechaEmision();

    enum{INDEX_ID = 0, INDEX_CANTIDAD, INDEX_UNIDAD, INDEX_DESCRIPCION, INDEX_P_UNIT
          , INDEX_P_TOTAL};

    void set_widget_previous(QWidget* w);

    int getOp(){ return op; }

    bool select(QString id, QString fecha_emision);

public:
    void set_producto(QString producto_id
                      , QString cantidad, QString unidad
                      , QString descripcion, QString p_total);

private:
    bool guardar();

    bool remove();

private slots:
    void set_time();

    void on_producto_closing();

    void on_pushButton_ing_prod_clicked();

    void on_tableWidget_itemChanged(QTableWidgetItem *item);

    void on_pushButton_guardar_clicked();

    void on_pushButton_eliminar_clicked();

    void on_pushButton_salir_clicked();

    void on_dateTimeEdit_emision_dateChanged(const QDate &date);

    void on_pushButton_borrar_clicked();

    void on_pushButton_up_clicked();

    void on_pushButton_down_clicked();

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
    Ui::CompraRegSinDoc *ui;

    bool afterShow;

    QWidget* widget_previous;

    int op;

    QSqlQuery query;

    QString id;

    QString fecha_emision;
};

#endif // COMPRAREGSINDOC_H
