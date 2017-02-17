#ifndef A_PERSONA_H
#define A_PERSONA_H

#include "persona_global.h"

namespace Ui {
class A_Persona;
}

class PERSONASHARED_EXPORT A_Persona : public QWidget
{
    Q_OBJECT
public:
    explicit A_Persona(QWidget *parent = 0);
    ~A_Persona();

    void set_widget_previous(QWidget* widget_previous);

private slots:
    void on_pushButton_ingresar_clicked();

    void on_pushButton_buscar_clicked();

    void on_pushButton_salir_clicked();

protected:
    void showEvent(QShowEvent *event);

    bool eventFilter(QObject *obj, QEvent *e);

private:
    Ui::A_Persona *ui;

    QWidget* widget_previous;

};

#endif // A_PERSONA_H
