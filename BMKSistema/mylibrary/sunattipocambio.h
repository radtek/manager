#ifndef SUNATTIPOCAMBIO_H
#define SUNATTIPOCAMBIO_H

#include "mylibrary_global.h"
#include "lineedit.h"

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
    void on_thread_loadWeb();

    void on_thread_finished();

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

    QSqlQuery query_config;

    QTimer* timer_cambio;
    int transcurred_time;
    int countDatos;
    int countReload;

    QLineEdit* lineEdit_id_cambio;
    LineEdit* lineEdit_cambio;
    QDateEdit* dateEdit_cambio;
    QLabel* label_loading;
    QPushButton* pushButton_reload;
    QPushButton* pushButton_guardar_cambio;
    QPushButton* pushButton_ver_internet;
};

#endif // SUNATTIPOCAMBIO_H
