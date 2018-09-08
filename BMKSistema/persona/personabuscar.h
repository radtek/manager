#ifndef PERSONABUSCAR_H
#define PERSONABUSCAR_H

#include "mylibrary/share_static_include.h"

#include "persona.h"

namespace Ui {
class PersonaBuscar;
}

class PersonaBuscar : public QWidget
{
    Q_OBJECT

public:
    explicit PersonaBuscar(QWidget *parent = 0);
    ~PersonaBuscar();

    void setOpcPersona(QString opcPersona);

private slots:
    void on_pushButton_ingresar_clicked();

    void on_pushButton_modificar_clicked();

    void on_lineEdit_codigo_textChanged(const QString &arg1);

    void on_lineEdit_nombre_textChanged(const QString &arg1);

    void on_lineEdit_direccion_textChanged(const QString &arg1);

    void on_lw_tipo_itemClicked(QListWidgetItem* item);

    void on_checkBox_1_stateChanged(int arg1);

    void on_checkBox_2_stateChanged(int arg1);

    void on_checkBox_3_stateChanged(int arg1);

private:
    Ui::PersonaBuscar *ui;

    QString opcPersona;

    QString itemPersona;

    int newTipoSelected;
    QString newStringChanged;

    QVector<int> states;
};

#endif // PERSONABUSCAR_H
