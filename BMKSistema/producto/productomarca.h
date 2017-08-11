#ifndef PRODUCTOMARCA_H
#define PRODUCTOMARCA_H

#include "producto_headers.h"

namespace Ui {
class ProductoMarca;
}

class MYLIBRARYSHARED_EXPORT ProductoMarca : public QWidget
{
    Q_OBJECT

public:
    explicit ProductoMarca(QWidget *parent = 0);
    ~ProductoMarca();

    QString getID();
    QString getMarca();

    void set_widget_previous(QWidget* widget_previous);

    int getOp(){ return op; }

    void set_data(QString id, QString marca);

private:
    bool guardar();

    bool remove();

private slots:
    void on_pushButton_guardar_clicked();

    void on_pushButton_eliminar_clicked();

    void on_pushButton_salir_clicked();

protected:
    void showEvent(QShowEvent *se);

    void closeEvent(QCloseEvent *ce);

    bool eventFilter(QObject *obj, QEvent *e);

signals:
    void closing();

private:
    Ui::ProductoMarca *ui;

    bool afterShow;

    int op;

    QWidget* widget_previous;

    QString id;
};

#endif // PRODUCTOMARCA_H
