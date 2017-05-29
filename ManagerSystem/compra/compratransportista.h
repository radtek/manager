#ifndef COMPRATRANSPORTISTA_H
#define COMPRATRANSPORTISTA_H

#include "facturacion_global.h"
#include "compra/compra_headers.h"
#include "persona/personabuscar.h"

class FACTURACIONSHARED_EXPORT CompraTransportista : public PersonaBuscar
{
    Q_OBJECT

public:
    explicit CompraTransportista(QWidget *parent = 0);
    ~CompraTransportista();

private slots:
    //void on_le_buscar_clicked();

    //void on_tb_clicked();


protected:
    void showEvent(QShowEvent *se);

    void closeEvent(QCloseEvent *event);

signals:
    void closing();


};

#endif // COMPRATRANSPORTISTA_H
