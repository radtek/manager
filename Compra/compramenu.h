#ifndef COMPRAMENU_H
#define COMPRAMENU_H

#include "compra_global.h"

namespace Ui {
class CompraMenu;
}

class COMPRASHARED_EXPORT CompraMenu : public QWidget
{
    Q_OBJECT

public:
    explicit CompraMenu(QWidget *parent = 0);
    ~CompraMenu();

    void set_widget_previous(QWidget* widget_previous);

    void set_modo(int);

private slots:
    void on_listWidget_currentRowChanged(int currentRow);

    void on_pushButton_ok_clicked();

    void on_pushButton_salir_clicked();    

protected:
    void showEvent(QShowEvent *event);

    void closeEvent(QCloseEvent *event);

    bool eventFilter(QObject *obj, QEvent *e);

signals:
    void closing();

private:
    Ui::CompraMenu *ui;

    QWidget* widget_previous;

    int modo;

    int tipo;

};

#endif // COMPRAMENU_H
