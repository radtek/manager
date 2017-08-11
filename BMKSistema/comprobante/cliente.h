#ifndef CLIENTE_H
#define CLIENTE_H

#include "mylibrary/mylibrary_global.h"

namespace Ui {
class Cliente;
}

class MYLIBRARYSHARED_EXPORT Cliente : public QWidget
{
    Q_OBJECT

public:
    explicit Cliente(QWidget *parent = 0);
    ~Cliente();

    QString get_cod(){ return cod; }
    QString get_nombre(){ return nombre; }
    QString get_direccion(){ return direccion; }

public slots:
    void on_verticalScrollBar_actionTriggered(int value);

    void on_verticalScrollBar_valueChanged(int value);

    void on_pushButton_salir_clicked();

    void on_tableWidget_itemDoubleClicked(QTableWidgetItem *item);

private:
    void set_buscar();

public slots:
    void on_lineEdit_buscar_textChanged(const QString& arg);

    void on_lineEdit_buscar_returnPressed();

protected:
    void showEvent(QShowEvent *se);

    void closeEvent(QCloseEvent *ce);

    bool eventFilter(QObject *obj, QEvent *e);

signals:
    void closing();

private slots:
    void on_pushButton_ingresar_clicked();

private:
    void editarItem(QTableWidgetItem* item);

private:
    Ui::Cliente *ui;

    bool firstShow;
    bool afterShow;

    int pos;
    int size_query;

    QString cod;
    QString nombre;
    QString direccion;

};

#endif // CLIENTE_H
