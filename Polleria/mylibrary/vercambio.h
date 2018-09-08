#ifndef VERCAMBIO_H
#define VERCAMBIO_H

#include "mylibrary_global.h"

namespace Ui {
class VerCambio;
}

class MYLIBRARYSHARED_EXPORT VerCambio : public QWidget
{
    Q_OBJECT

public:
    explicit VerCambio(QWidget *parent = 0);
    ~VerCambio();

    void set_widget_previous(QWidget* widget_previous);

private slots:
    void on_pushButton_editar_clicked();

    void on_pushButton_salir_clicked();

    void on_dateEdit_fin_editingFinished();

private:
    void jalar_cambio();

protected:
    void showEvent(QShowEvent *event);

    bool eventFilter(QObject *watched, QEvent *event);

private:
    Ui::VerCambio *ui;

    QWidget* widget_previous;

    bool firstShow;
    bool afterShow;

    int pos;
    int size_query;
};

#endif // VERCAMBIO_H
