#ifndef ENVIARMECORREO_H
#define ENVIARMECORREO_H

#include "mylibrary/share_static_include.h"

#include "smtpclient.h"
#include "mimemessage.h"
#include "mimetext.h"
#include "mimehtml.h"

namespace Ui {
class EnviarmeCorreo;
}

class EnviarmeCorreo : public QWidget
{
    Q_OBJECT

public:
    explicit EnviarmeCorreo(QWidget *parent = 0);
    ~EnviarmeCorreo();

private slots:
    void on_pushButton_enviar_clicked();

    void on_pushButton_salir_clicked();

    void on_lineEdit_codigoVerificacion_textEdited(const QString &arg1);

    void on_pushButton_revelar_clicked();

private:
    Ui::EnviarmeCorreo *ui;

    QString codigoVerificacion;
};

#endif // ENVIARMECORREO_H
