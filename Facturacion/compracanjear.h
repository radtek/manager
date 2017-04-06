#ifndef COMPRACANJEAR_H
#define COMPRACANJEAR_H

#include "facturacion_global.h"
#include "compra_headers.h"

namespace Ui {
class CompraCanjear;
}

class FACTURACIONSHARED_EXPORT CompraCanjear : public QWidget
{
    Q_OBJECT

public:
    explicit CompraCanjear(QWidget *parent = 0);
    ~CompraCanjear();

    void set_widget_previous(QWidget* widget_previous);

    void set_data(int id_tipo, QString id
                  , QString codigo, QString nombre);

private slots:
    void on_formDocumento_closing();

    void on_pushButton_ok_clicked();

    void on_pushButton_salir_clicked();

    void on_tableWidget_itemDoubleClicked(QTableWidgetItem *item);

protected:
    void showEvent(QShowEvent *event);

    bool eventFilter(QObject *watched, QEvent *event);

private:
    Ui::CompraCanjear *ui;

    QWidget* widget_previous;

    bool afterShow;

    int id_tipo_sender;
    QString id_sender;
    QString codigo_sender;
    QString nombre_sender;

    int id_tipo_recipient;
    QString id_recipient;
    QString codigo_recipient;
    QString nombre_recipient;
};

#endif // COMPRACANJEAR_H
