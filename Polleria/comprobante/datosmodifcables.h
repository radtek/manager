#ifndef DATOSMODIFCABLES_H
#define DATOSMODIFCABLES_H

#include "mylibrary/share_static_include.h"
#include "mylibrary/mylibrary_global.h"
#include "mylibrary/adminpass.h"
#include "persona/persona.h"

namespace Ui {
class DatosModifcables;
}

class MYLIBRARYSHARED_EXPORT DatosModifcables : public QWidget
{
    Q_OBJECT

public:
    explicit DatosModifcables(QWidget *parent = 0);
    ~DatosModifcables();

    enum opc{SALIR, CONTINUAR};

    int getOpc(){ return opc; }

private:
    void set_buscar();

private slots:
    void on_pushButton_continuar_clicked();

    void on_pushButton_salir_clicked();

    void on_dateEdit_fecha_dateChanged(const QDate &date);

    void on_pushButton_clicked();

protected:
    void closeEvent(QCloseEvent *event);

signals:
    void closing();

private:
    Ui::DatosModifcables *ui;

    int opc;
};

#endif // DATOSMODIFCABLES_H
