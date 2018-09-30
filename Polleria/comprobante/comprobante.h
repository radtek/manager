#ifndef COMPROBANTE_H
#define COMPROBANTE_H

#include "mylibrary/mylibrary_global.h"
#include "mylibrary/sunatconsultaruc.h"
#include "mylibrary/reniecconsultadni.h"
#include "configgeneral.h"
#include "familia.h"
#include "plato.h"
#include "mylibrary/adminpass.h"
#include "persona/persona.h"
#include "persona/personabuscar.h"
#include "comprobante/pagar.h"
#include <QSslKey>

namespace Ui {
class Comprobante;
}

class MYLIBRARYSHARED_EXPORT Comprobante : public QWidget
{
    Q_OBJECT

public:
    explicit Comprobante(QWidget *parent = 0);
    ~Comprobante();

    static QString compra;
    static QString venta;
    static QString egresos;

    enum COLUMNAS{NOMBRE = 0, UNIDAD = 1, CANTIDAD = 2, PRECIO = 3, DESCRIPCION_PLATO = 4, ID = 5, CATEGORIA = 6};

    void when_detalleModified();

    void delayedPopup();

    void menuButtonPopup();

    void clean_familias();

    void clean_platos();

    void clean_void_familias();

    void clean_void_platos();

    void select_familias();

    void select_platos(QString familia);

    void insert_venta();

private slots:    
    void on_familia_closing();

    void on_plato_closing();    

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

    void on_myProccess_started();

    void on_myProccess_finished(int, QProcess::ExitStatus);

    void on_pushButton_google_maps_clicked();

    void on_thread_loadWeb();

    void loadFinished(bool b);

    void print_comanda_aux();

    void on_socket_bytesWritten(qint64 bytes);

    void on_comboBox_familia_currentTextChanged(const QString &arg1);

    void on_configGeneral_closing();

    void on_pushButton_imageLogo_clicked();

    void on_lineEdit_pago_textEdited(const QString &arg1);

    void on_lineEdit_pago_textChanged(const QString &arg1);

    void on_pushButton_liberarColaImpresion_clicked();

    void on_checkBox_habilitarEdicion_stateChanged(int arg1);

    void on_comboBox_operacion_currentTextChanged(const QString &arg1);

    void on_pushButton_pagar_clicked();

    void on_pagar_closing();

    void on_doubleSpinBox_delivery_valueChanged(double arg1);

    void on_checkBox_switch_caja_clicked();

    void on_checkBox_switch_caja_toggled(bool checked);

    void on_pushButton_clicked();

    void on_comboBox_documento_currentTextChanged(const QString &arg1);

    void on_lineEdit_numero_textChanged(const QString &arg1);

private:
    void print_caja();

    void print_caja_frap();

    void print_comanda();

    void print_comanda_alas();

    void print_star_asa();

    void print_msg();

    void print_machelo();    

    void print_epson();

    void clear_form();
protected:
    void showEvent(QShowEvent *event);

    void closeEvent(QCloseEvent *event);

    bool eventFilter(QObject *watched, QEvent *event);

private:
    Ui::Comprobante *ui;

    SunatConsultaRUC sunat_consultaRUC;
    ReniecConsultaDNI reniec_consultDNI;

    QString clave;

    QString digestValue;

    bool afterShow;
    bool firstShow;

    QTcpSocket* m_pSocket;
    bool m_ConnectStatus;

    QString pago;
    QString nro_ticket;
    //QWebEngineView* view_mapa;

    QByteArray blockWritten;

    int countPrintComanda;

    QString fondoTapiz;

    QString tipo_persona;
};

#endif // COMPROBANTE_H
