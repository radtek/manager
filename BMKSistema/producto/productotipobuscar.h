#ifndef PRODUCTOTIPOBUSCAR_H
#define PRODUCTOTIPOBUSCAR_H

#include "producto_headers.h"

namespace Ui {
class ProductoTipoBuscar;
}

class MYLIBRARYSHARED_EXPORT ProductoTipoBuscar : public QWidget
{
    Q_OBJECT

public:
    explicit ProductoTipoBuscar(QWidget *parent = 0);
    ~ProductoTipoBuscar();

    QString getID();
    QString getTipo();

    void set_widget_previous(QWidget* widget_previous);

private slots:
    void on_verticalScrollBar_actionTriggered(int value);

    void on_tipo_closing();

private:
    void set_buscar();

private slots:
    void on_lineEdit_tipo_buscar_textChanged(const QString& arg);

    void on_lineEdit_tipo_buscar_returnPressed();

    void on_pushButton_nuevo_clicked();

    void on_pushButton_ok_clicked();

    void on_pushButton_salir_clicked();

    void on_tableWidget_itemDoubleClicked(QTableWidgetItem *item);

    void on_pushButton_editar_clicked();

private:
    void showEvent(QShowEvent *se);

    void closeEvent(QCloseEvent * ce);

    bool eventFilter(QObject *obj, QEvent *e);

signals:
    void closing();

private:
    void editarItem(QTableWidgetItem* item);

private:
    Ui::ProductoTipoBuscar *ui;

    bool firstShow;
    bool afterShow;

    QWidget* widget_previous;

    int pos;
    int size_query;

    QString id;
    QString tipo;
};

#endif // PRODUCTOTIPOBUSCAR_H
