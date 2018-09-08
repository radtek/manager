#ifndef CONFIGGENERAL_H
#define CONFIGGENERAL_H

#include "mylibrary/mylibrary_global.h"
#include "mylibrary/sunatconsultaruc.h"
#include "mylibrary/reniecconsultadni.h"
#include "familia.h"
#include "plato.h"
#include "mylibrary/adminpass.h"

namespace Ui {
class ConfigGeneral;
}

class MYLIBRARYSHARED_EXPORT ConfigGeneral : public QWidget
{
    Q_OBJECT

public:
    explicit ConfigGeneral(QWidget *parent = 0);
    ~ConfigGeneral();

    void setFondoTapiz(QString fondoTapiz);

    QString getClave();

    int getFamiliaFilas();

    int getFamiliaColumnas();

    int getPlatoFilas();

    int getPlatoColumnas();

    QString getFondoTapiz();

    void delayedPopup();

    void menuButtonPopup();

    void clean_familias();

    void clean_platos();

    void select_familias();

    void select_platos(QString familia);

private slots:
    void on_familia_closing();

    void on_plato_closing();

    void on_familia_fontAct_triggered(QAction* act);

    void on_familia_colorAct_triggered(QAction* act);

    void on_familia_color_de_fondo_Act_triggered(QAction* act);

    void on_familia_nuevoAct_triggered(QAction* act);

    void on_familia_modificarAct_triggered(QAction* act);

    void on_plato_fontAct_triggered(QAction* act);

    void on_plato_colorAct_triggered(QAction* act);

    void on_plato_color_de_fondo_Act_triggered(QAction* act);

    void on_plato_nuevoAct_triggered(QAction* act);

    void on_plato_modificarAct_triggered(QAction* act);

    void on_comboBox_familia_activated(const QString &arg1);

    void on_comboBox_plato_activated(const QString &arg1);

    void on_spinBox_familia_filas_valueChanged(int arg1);

    void on_spinBox_familia_columnas_valueChanged(int arg1);

    void on_spinBox_plato_filas_valueChanged(int arg1);

    void on_spinBox_plato_columnas_valueChanged(int arg1);

    void on_cliente_closing();

    void on_toolButton_familia_clicked();

    void on_toolButton_plato_clicked();

    void on_pushButton_guardar_clave_clicked();

    void on_comboBox_familia_currentTextChanged(const QString &arg1);

    void on_pushButton_fondoTapiz_clicked();

    void on_pushButton_formatoFamilia_clicked();

    void on_pushButton_colorFamilia_clicked();

    void on_pushButton_fondoFamilia_clicked();

    void on_pushButton_formatoPlato_clicked();

    void on_pushButton_colorPlato_clicked();

    void on_pushButton_fondoPlato_clicked();

protected:
    void showEvent(QShowEvent *event);

    void closeEvent(QCloseEvent *event);

    bool eventFilter(QObject *watched, QEvent *event);

signals:
    void closing();

private:
    Ui::ConfigGeneral *ui;

    bool afterShow;
    bool firstShow;

    QString fondoTapiz;
};

#endif // CONFIGGENERAL_H
