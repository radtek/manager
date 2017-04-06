#ifndef SUNATTIPOCAMBIO_H
#define SUNATTIPOCAMBIO_H

#include "mylibrary_global.h"

class LineEdit;

class MYLIBRARYSHARED_EXPORT SunatTipoCambio : public QWidget
{
    Q_OBJECT
public:
    explicit SunatTipoCambio(QWidget *parent = 0);
    ~SunatTipoCambio();

    void set_data(QLineEdit* lineEdit_id_cambio
                  , LineEdit* lineEdit_cambio
                  , QDateEdit *dateEdit_cambio
                  , QLabel* label_loading
                  , QPushButton* pushButton_reload
                  , QPushButton* pushButton_guardar_cambio
                  , QPushButton* pushButton_ver_internet);

    void consultar();

private:
    void setCambio(const QDate& date);

signals:

private slots:
    void show_timeCambio();

    void loadStarted();

    void loadFinished(bool b);

    void on_dateEdit_cambio_dateChanged(const QDate &date);

    void on_pushButton_reload_clicked();

    void on_pushButton_guardar_cambio_clicked();

public slots:
    void on_pushButton_ver_internet_clicked();

public:
    QWebEngineView* webView_tipo_cambio;

    bool is_wait;

    QSqlQuery query_config;

    int transcurred_time;
    QTimer* timer_cambio;

    QLineEdit* lineEdit_id_cambio;
    LineEdit* lineEdit_cambio;
    QDateEdit* dateEdit_cambio;
    QLabel* label_loading;
    QPushButton* pushButton_reload;
    QPushButton* pushButton_guardar_cambio;
    QPushButton* pushButton_ver_internet;

    int countReload;
    int countDatos;

    int countConsultar;
};

#endif // SUNATTIPOCAMBIO_H
