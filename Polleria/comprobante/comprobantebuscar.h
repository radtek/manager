#ifndef COMPROBANTEBUSCAR_H
#define COMPROBANTEBUSCAR_H

#include "mylibrary/mylibrary_global.h"
#include "comprobante/previewticket.h"
#include "comprobante/delivery.h"
#include "mylibrary/adminpass.h"
#include "persona/persona.h"
#include "comprobante/datosmodifcables.h"
#include "comprobante/anularcpe.h"

namespace Ui {
class ComprobanteBuscar;
}

class MYLIBRARYSHARED_EXPORT ComprobanteBuscar : public QWidget
{
    Q_OBJECT

public:
    explicit ComprobanteBuscar(QWidget *parent = 0);
    ~ComprobanteBuscar();

    QString get_cod(){ return cod; }
    QString get_nombre(){ return nombre; }
    QString get_direccion(){ return direccion; }

public slots:
    void on_verticalScrollBar_actionTriggered(int value);

    void on_verticalScrollBar_valueChanged(int value);

    void on_pushButton_salir_clicked();

    void on_tableWidget_itemDoubleClicked(QTableWidgetItem *item);

private:
    void set_buscar();

    void set_total();

public slots:
    void on_lineEdit_buscar_textChanged(const QString& arg);

    void on_lineEdit_buscar_returnPressed();

protected:
    void showEvent(QShowEvent *se);

    void closeEvent(QCloseEvent *ce);

    bool eventFilter(QObject *obj, QEvent *e);

signals:
    void closing();

private slots:
    void on_pushButton_ingresar_clicked();

    void on_dateTimeEdit_ini_dateTimeChanged(const QDateTime &dateTime);

    void on_dateTimeEdit_fin_dateTimeChanged(const QDateTime &dateTime);

    void on_checkBox_efectivo_toggled(bool checked);

    void on_checkBox_visa_toggled(bool checked);

    void on_checkBox_masterCard_toggled(bool checked);

    void on_pushButton_anular_clicked();

    void on_pushButton_ver_clicked();

    void on_pushButton_habilitar_clicked();

    void on_pushButton_exportar_clicked();

    void on_lineEdit_buscarProducto_textChanged(const QString &arg1);

    void on_myProccess_finished(int, QProcess::ExitStatus);

    void on_checkBox_compra_toggled(bool checked);

    void on_checkBox_venta_toggled(bool checked);

    void on_checkBox_egresos_toggled(bool checked);

    void on_checkBox_boleta_toggled(bool checked);

    void on_checkBox_factura_toggled(bool checked);

    void on_pushButton_deliverys_clicked();

    void on_datosModificables_closing();

    void on_checkBox_ticket_toggled(bool checked);

    void on_checkBox_ncredito_toggled(bool checked);

    void on_pushButton_enviarSUNAT_clicked();

    void on_pushButton_imprimir_clicked();

private:
    void editarItem(QTableWidgetItem* item);

private:
    Ui::ComprobanteBuscar *ui;

    bool firstShow;
    bool afterShow;

    int pos;
    int size_query;

    QString directory;

    QString cod;
    QString nombre;
    QString direccion;
};

#endif // COMPROBANTEBUSCAR_H
