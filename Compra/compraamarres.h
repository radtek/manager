#ifndef COMPRAAMARRES_H
#define COMPRAAMARRES_H

#include "compra_global.h"

namespace Ui {
class CompraAmarres;
}

class COMPRASHARED_EXPORT CompraAmarres : public QWidget
{
    Q_OBJECT

public:
    explicit CompraAmarres(QWidget *parent = 0);
    ~CompraAmarres();

    void set_widget_previous(QWidget* widget_previous);

    void set_documento(QString id, int tipo);

private slots:
    void on_compra_closing();

    void on_pushButton_modificar_clicked();

    void on_pushButton_salir_clicked();    

protected:
    void showEvent(QShowEvent *se);

    void closeEvent(QCloseEvent *ce);

    bool eventFilter(QObject *obj, QEvent *e);

signals:
    void closing();

private:
    Ui::CompraAmarres *ui;

    QWidget* widget_previous;

    int tipo;

    QString id;
};

#endif // COMPRAAMARRES_H
