#ifndef SUNATCONSULTARUC_H
#define SUNATCONSULTARUC_H

#include "mylibrary_global.h"

class MYLIBRARYSHARED_EXPORT SunatConsultaRUC : public QWidget
{
	Q_OBJECT
public:
    explicit SunatConsultaRUC(QWidget *parent = 0);
    ~SunatConsultaRUC();

    void set_data(QLabel* label_captcha
		, QLineEdit* le_ruc, QLineEdit* le_captcha
		, QLineEdit* le_razonSocial, QLineEdit* le_tipoContribuyente
		, QLineEdit* le_nombreComercial, QLineEdit* le_direccion
        , QLineEdit* le_estado, QLineEdit* le_condicion, QTableWidget* table_establecimientos);

	void setLock(bool lock) { this->lock = lock;  }

    void consultar_anexos();

private:
    

signals:

private slots :
    void on_thread_loadWeb();

    void on_thread_finished();

	void setImage();

	void setDatos();

    void setEstablecimientos();

	void show_time_datos();

    void show_time_image();

    void show_time_consultar_loc_anexos();

    void show_time_establecimientos();

    void loadFinished(bool b);

    void on_le_consulta_ruc_textEdited();

public:
	QTimer* timer_image;
    QTimer* timer_datos;
    QTimer* timer_consultar_loc_anexos;
    QTimer* timer_establecimientos;

    QLineEdit* le_ruc;

    QPixmap pixmap;
    QLabel* label_captcha;
    QLineEdit* le_captcha;

    QLineEdit* le_razonSocial;
    QLineEdit* le_tipoContribuyente;
    QLineEdit* le_nombreComercial;
    QLineEdit* le_direccion;
    QLineEdit* le_estado;
    QLineEdit* le_condicion;

    QTableWidget* table_establecimientos;

    QWebEngineView* view_consultaRuc;

	bool isLoad;

    bool isImageRead;
    bool isDatosRead;

    int count;
	int count_datos;    

    int tt_image;
	int time_transcurred;
    int time_establecimientos_transcurred;

	int firstTime_load;

	bool lock;

    QString ruc;

    QString establecimientos;
};

#endif // SUNATCONSULTARUC_H
