#ifndef PRODUCTOTIPO_H
#define PRODUCTOTIPO_H

#include "producto_headers.h"

namespace Ui {
class ProductoTipo;
}

class MYLIBRARYSHARED_EXPORT ProductoTipo : public QWidget
{
    Q_OBJECT

public:
    explicit ProductoTipo(QWidget *parent = 0);
    ~ProductoTipo();

    QString getID();
    QString getTipo();

    void set_widget_previous(QWidget* widget_previous);

    int getOp(){ return op; }

    void set_data(QString id, QString tipo);

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
    Ui::ProductoTipo *ui;

    bool afterShow;

    QWidget* widget_previous;

    int op;

    QString id;
};

#endif // PRODUCTOTIPO_H
