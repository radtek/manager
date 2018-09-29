#ifndef ANULARCPE_H
#define ANULARCPE_H

#include "mylibrary/mylibrary_global.h"

namespace Ui {
class AnularCPE;
}

class MYLIBRARYSHARED_EXPORT AnularCPE : public QWidget
{
    Q_OBJECT

public:
    explicit AnularCPE(QWidget *parent = 0);
    ~AnularCPE();

private slots:
    void on_pushButton_ver_clicked();

    void on_lineEdit_motivo_textChanged(const QString &arg1);

    void on_pushButton_ok_clicked();

    void on_pushButton_cancelar_clicked();

private:
    bool create_ncredito();

private:
    Ui::AnularCPE *ui;
};

#endif // ANULARCPE_H
