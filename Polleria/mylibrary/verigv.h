#ifndef VERIGV_H
#define VERIGV_H

#include "mylibrary_global.h"

namespace Ui {
class VerIGV;
}

class MYLIBRARYSHARED_EXPORT VerIGV : public QWidget
{
    Q_OBJECT

public:
    explicit VerIGV(QWidget *parent = 0);
    ~VerIGV();

    void set_widget_previous(QWidget* widget_previous);

private slots:
    void on_pushButton_editar_clicked();

    void on_pushButton_salir_clicked();

    void on_dateEdit_fin_editingFinished();

private:
    void jalar_igv();

protected:
    void showEvent(QShowEvent *event);

    bool eventFilter(QObject *watched, QEvent *event);

private:
    Ui::VerIGV *ui;

    QWidget* widget_previous;

    bool firstShow;
    bool afterShow;

    int pos;
    int size_query;
};

#endif // VERIGV_H
