#ifndef COLABSELECTION_H
#define COLABSELECTION_H

#include "persona_global.h"

namespace Ui {
class ColabSelection;
}

class PERSONASHARED_EXPORT ColabSelection : public QWidget
{
    Q_OBJECT
public:
    explicit ColabSelection(QWidget *parent = 0);
    ~ColabSelection();

    void set_widget_previous(QWidget* widget_previous);        

    void set_modo(int modo);

    QListWidget* get_lw();

private slots:
    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_pushButton_seleccionar_clicked();

    void on_pushButton_salir_clicked();

private:
    void showEvent(QShowEvent *event);

    void closeEvent(QCloseEvent *event);

    bool eventFilter(QObject *obj, QEvent *e);

signals:
    void closing();

private:
    Ui::ColabSelection *ui;

    QWidget* widget_previous;

    int modo;
};

#endif // COLABSELECTION_H
