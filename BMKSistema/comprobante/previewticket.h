#ifndef PREVIEWTICKET_H
#define PREVIEWTICKET_H

#include "mylibrary/mylibrary_global.h"

namespace Ui {
class PreviewTicket;
}

class MYLIBRARYSHARED_EXPORT PreviewTicket : public QWidget
{
    Q_OBJECT

public:
    explicit PreviewTicket(QWidget *parent = 0);
    ~PreviewTicket();

    void set_cabecera(QString numero, QString fecha, QString nombre);

    void set_detalle(QString id);

    void ver_documento();

private slots:
    void on_pushButton_salir_clicked();

    void on_pushButton_imprimir_clicked();

    void on_textEdit_textChanged();

protected:
    void showEvent(QShowEvent *event);

private:
    void print_caja();

private:
    Ui::PreviewTicket *ui;

    QString numero;
    QString fecha;
    QString nombre;
    QVector<QVector<QString> > detalle;
};

#endif // PREVIEWTICKET_H
