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

    void process_cambio(const QDate& date);

    void llenarDatos();

    bool update_cambio();
    bool update_igv();

    QPushButton* get_pb_ver_cambio();
    QPushButton* get_pb_ver_igv();

private:
	void setCambio(const QDate& date);

private slots:
	void show_timeCambio();

    void loadStarted();

    void loadFinished(bool b);

    void on_pushButton_guardar_cambio_clicked();

    void on_pushButton_guardar_igv_clicked();

    void on_pushButton_salir_clicked();

    void on_pushButton_ver_internet_clicked();

    void on_dateEdit_cambio_dateChanged(const QDate &date);

protected:
    bool eventFilter(QObject *, QEvent *);

	void showEvent(QShowEvent* se);

private:
    Ui::Configuracion *ui;

    QWebEngineView* webView_tipo_cambio;
    QDate date_tipo_cambio;
    bool is_wait;

    QSqlQuery query_config;

	QTimer* timer_cambio;

	bool isShow;

	int countReload;
	int countDatos;
	
	QTextEdit* textEdit;
};

template class Q_DECL_EXPORT singleton<Configuracion>;

#endif // CONFIGURACION_H

