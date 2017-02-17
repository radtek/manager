#ifndef COMPRATRANSPORTISTA_H
#define COMPRATRANSPORTISTA_H

#include "compra_global.h"

class COMPRASHARED_EXPORT CompraTransportista : public ColaboradorBuscar
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
