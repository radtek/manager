#ifndef DELIVERY_H
#define DELIVERY_H

#include "mylibrary/share_static_include.h"
#include "mylibrary/sistema.h"
#include "persona/persona.h"

namespace Ui {
class Delivery;
}

class MYLIBRARYSHARED_EXPORT Delivery : public QWidget
{
    Q_OBJECT

public:
    explicit Delivery(QWidget *parent = 0);
    ~Delivery();

    void select_clientes();

private slots:
    void on_lw_tipo_itemClicked(QListWidgetItem *item);

    void on_verticalScrollBar_actionTriggered(int value);

    void on_verticalScrollBar_valueChanged(int value);

    void on_pushButton_salir_clicked();

    void on_tableWidget_itemDoubleClicked(QTableWidgetItem *item);

    void on_lineEdit_nombre_textChanged(const QString &arg1);

    void on_tableWidget_itemChanged(QTableWidgetItem *item);

    void on_checkBox_activo_toggled(bool checked);

    void on_checkBox_inactivo_toggled(bool checked);

    void on_checkBox_pagado_toggled(bool checked);

    void on_dateEdit_dateChanged(const QDate &date);

protected:
    bool eventFilter(QObject *watched, QEvent *event);

    void showEvent(QShowEvent *se);

    void closeEvent(QCloseEvent *ce);
signals:
    void closing();

private:
    Ui::Delivery *ui;

    bool firstShow;
    bool afterShow;

    int pos;
    int size_query;
};

#endif // DELIVERY_H
