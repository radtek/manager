#ifndef CONFIGURACION_H
#define CONFIGURACION_H

#include "locallibrary.h"

namespace Ui {
class Configuracion;
}

class Q_DECL_EXPORT Configuracion : public QWidget
{
    Q_OBJECT

public:
    explicit Configuracion(QWidget *parent = 0);
    ~Configuracion();

    void set_widget_previous(QWidget* widget_previous);

    void process_cambio(const QDate& date);

    void llenarDatos();

    bool update_cambio();
    bool update_igv();

private:

private slots:
    void on_pushButton_guardar_cambio_clicked();

    void on_pushButton_guardar_igv_clicked();

    void on_pushButton_salir_clicked();

    void on_pushButton_ver_cambio_clicked();

    void on_pushButton_ver_igv_clicked();

    void on_dateEdit_cambio_dateChanged(const QDate &date);

    void on_dateEdit_igv_dateChanged(const QDate &date);

    void on_pushButton_rollback_clicked();

    void on_pushButton_commit_clicked();

    void on_pushButton_backup_clicked();

    void on_myProccess_started();

    void on_myProccess_finished(int exitCode, QProcess::ExitStatus exitStatus);

    void on_pushButton_restore_clicked();

protected:
	void showEvent(QShowEvent* se);

    bool eventFilter(QObject *, QEvent *);

private:
    Ui::Configuracion *ui;

    QWidget* widget_previous;

    SunatTipoCambio sunatCambio;

    bool is_wait;

    QSqlQuery query_config;

    bool afterShow;
    bool firstShow;

	int countReload;
	int countDatos;	
};

template class Q_DECL_EXPORT singleton<Configuracion>;

#endif // CONFIGURACION_H

