#ifndef VENTACONFIGHOJA_H
#define VENTACONFIGHOJA_H

#include "venta_global.h"

namespace Ui {
class VentaConfigHoja;
}

class VENTASHARED_EXPORT VentaConfigHoja : public QWidget
{
    Q_OBJECT

public:
    explicit VentaConfigHoja(QWidget *parent = 0);
    ~VentaConfigHoja();

    void set_widget_previous(QWidget* widget_previous);

    void set_tipo_documento(int tipo, QString serie, QString series_id);

    void set_data(QVector<QVector<QString> > data);

private:
    bool guardar();

private slots:
    void on_pushButton_guardar_clicked();

    void on_pushButton_salir_clicked();

private:
    Ui::VentaConfigHoja *ui;

    QWidget* widget_previous;

    int width;
    int height;

    int tipo;

    QString series_id;

    QVector<QVector<QString> > data;
};

#endif // VENTACONFIGHOJA_H
