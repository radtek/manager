#ifndef COMPRACHARTCOSTO_H
#define COMPRACHARTCOSTO_H

#include <QWidget>
#include <QtCharts>
#include <QLineSeries>
using namespace QtCharts;

#include "mylibrary_global.h"

#include "compradefines.h"

//#include "compraboleta.h"
//#include "comprafactura.h"

namespace Ui {
class CompraChartCosto;
}

class COMPRASHARED_EXPORT CompraChartCosto : public QWidget
{
    Q_OBJECT

public:
    explicit CompraChartCosto(QWidget *parent = 0);
    ~CompraChartCosto();

    void set_widget_previous(QWidget* widget_previous);

    void set_producto(QString producto_id, QString unidad, QString descripcion);

    void set_value_min(QVector<qreal> v);

    void set_value_max(QVector<qreal> v);

private:
    void set_tw();

    void set_chart();

    void llenar_datos(QString producto_id);

    void llenar_tabla();

    void llenar_cantidad_precio();

    void llenar_grafica();

private slots:
    void on_year_up();

    void on_year_down();

    void on_pushButton_ok_clicked();

    void on_pushButton_salir_clicked();

    void on_dateEdit_dateChanged(const QDate &date);

    void on_pushButton_modificar_clicked();

protected:
    void showEvent(QShowEvent* se);

    bool eventFilter(QObject *watched, QEvent *event);

private:
    Ui::CompraChartCosto *ui;

    QWidget* widget_previous;

    int pos;
    int size_query;

    QString producto_id;

    QTableWidget* tw;
    QChartView* chartView;

    QVector<qreal> values_min;
    QVector<qreal> values_max;
};

#endif // COMPRACHARTCOSTO_H
