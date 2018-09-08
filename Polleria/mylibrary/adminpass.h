#ifndef ADMINPASS_H
#define ADMINPASS_H

#include "mylibrary/mylibrary_global.h"

namespace Ui {
class AdminPass;
}

class MYLIBRARYSHARED_EXPORT AdminPass : public QDialog
{
    Q_OBJECT

public:
    explicit AdminPass(QWidget *parent = 0);
    ~AdminPass();

    bool get_confirmado() { return confirmado; }

    enum modalidad{CONFIRM_MY_PASS, ADMIN_PASS};
    
    void setModalidad(int modalidad);
private slots:
    void on_pushButton_aceptar_clicked();

    void on_pushButton_cancelar_clicked();

signals:
    void closing();

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::AdminPass *ui;

    bool confirmado;
    
    int modalidad;
};

#endif // ADMINPASS_H
