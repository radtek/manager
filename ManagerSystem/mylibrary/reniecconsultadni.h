﻿#ifndef RENIECCONSULTADNI_H
#define RENIECCONSULTADNI_H

#include "mylibrary_global.h"

class MYLIBRARYSHARED_EXPORT ReniecConsultaDNI : public QWidget
{
	Q_OBJECT
public:
	explicit ReniecConsultaDNI(QWidget * parent = 0);
	~ReniecConsultaDNI();

	void set_data(QLabel* label_captcha, QLineEdit* lineEdit_dni
	, QLineEdit* lineEdit_codigoCaptcha, QLineEdit* lineEdit_nombre);

private:

signals :

private slots :
	void setImage();

	void setDatos();

	void show_time_datos();

	void show_time_image();

	void loadFinished(bool b);

    void on_le_consulta_dni_textEdited();

public:
    QTimer* timer_image;
    QTimer* timer_datos;

    QLineEdit* le_dni;

    QPixmap pixmap;
    QLabel* label_captcha;
    QLineEdit* le_captcha;
    QLineEdit* le_nombre;

    QWebEngineView* view_consultaDNI;

    bool isLoad;
    int count;
    int time_transcurred;
};

#endif RENIECCONSULTADNI_H
