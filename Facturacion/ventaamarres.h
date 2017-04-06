#ifndef VENTAAMARRES_H
#define VENTAAMARRES_H

#include "facturacion_global.h"
#include "venta_headers.h"

namespace Ui {
class VentaAmarres;
}

class FACTURACIONSHARED_EXPORT VentaAmarres : public QWidget
{
    Q_OBJECT

public:
    explicit VentaAmarres(QWidget *parent = 0);
    ~VentaAmarres();

    void set_widget_previous(QWidget* widget_previous);

    void set_documento_id(QString id);
private:
    Ui::VentaAmarres *ui;

    QWidget* widget_previous;
    QString documento_id;
};

#endif // VENTAAMARRES_H
