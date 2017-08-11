#ifndef COMPROBANTE_H
#define COMPROBANTE_H

#include "mylibrary/mylibrary_global.h"
#include "mylibrary/widgetevents.h"
#include "mylibrary/sunatconsultaruc.h"
#include "mylibrary/reniecconsultadni.h"
#include "cliente.h"
#include "familia.h"
#include "plato.h"
#include "mylibrary/adminpass.h"
#include "mylibrary/starmicronics.h"

namespace Ui {
class Comprobante;
}

class MYLIBRARYSHARED_EXPORT Comprobante : public QWidget
{
    Q_OBJECT

public:
    explicit Comprobante(QWidget *parent = 0);
    ~Comprobante();

    void select_familias();

    void select_platos(QString familia);

private slots:    
    void on_familia_closing();

    void on_plato_closing();

    void on_familia_fontAct_triggered(QAction* act);

    void on_familia_colorAct_triggered(QAction* act);

    void on_familia_color_de_fondo_Act_triggered(QAction* act);

    void on_plato_fontAct_triggered(QAction* act);

    void on_plato_colorAct_triggered(QAction* act);

    void on_plato_color_de_fondo_Act_triggered(QAction* act);

    void on_pushButton_edit_familia_clicked();

    void on_pushButton_edit_plato_clicked();

    void on_comboBox_familia_activated(const QString &arg1);

    void on_comboBox_plato_activated(const QString &arg1);

    void on_spinBox_familia_filas_valueChanged(int arg1);

    void on_spinBox_familia_columnas_valueChanged(int arg1);

    void on_spinBox_plato_filas_valueChanged(int arg1);

    void on_spinBox_plato_columnas_valueChanged(int arg1);

    void on_pushButton_efectivo_clicked();

    void on_pushButton_masterCard_clicked();

    void on_pushButton_visa_clicked();

    void on_cliente_closing();

    void on_pushButton_edit_cliente_clicked();

    void on_pushButton_subir_clicked();

    void on_pushButton_bajar_clicked();

    void on_pushButton_quitar_clicked();

    void on_toolButton_familia_clicked();

    void on_toolButton_plato_clicked();

    void on_tableWidget_itemChanged(QTableWidgetItem *item);

    void on_doubleSpinBox_pago_valueChanged(double arg1);

    void on_doubleSpinBox_total_valueChanged(double arg1);

    void on_thread_finished();

private:
    void print_caja();

    void print_comanda();

protected:
    void showEvent(QShowEvent *event);

    void closeEvent(QCloseEvent *event);

    bool eventFilter(QObject *watched, QEvent *event);

private:
    Ui::Comprobante *ui;

    SunatConsultaRUC sunat_consultaRUC;
    ReniecConsultaDNI reniec_consultDNI;

    bool afterShow;
    bool firstShow;

    QTcpSocket* m_pSocket;
    bool m_ConnectStatus;
};

#endif // COMPROBANTE_H
