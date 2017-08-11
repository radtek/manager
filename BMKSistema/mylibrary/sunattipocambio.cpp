#include "sunattipocambio.h"

SunatTipoCambio::SunatTipoCambio(QWidget* parent)
    : QWidget(parent)
{
    webView_tipo_cambio = NULL;

    timer_cambio = NULL;
    countDatos = 0;
    countReload = 0;
    transcurred_time = 0;

    label_loading = NULL;
    pushButton_reload = NULL;
    pushButton_guardar_cambio = NULL;
    pushButton_ver_internet = NULL;
    lineEdit_cambio = NULL;
}
SunatTipoCambio::~SunatTipoCambio()
{
    delete webView_tipo_cambio;
    delete timer_cambio;
}
void SunatTipoCambio::set_data(QLineEdit* lineEdit_id_cambio
                               , LineEdit* lineEdit_cambio
                               , QDateEdit *dateEdit_cambio
                               , QLabel* label_loading
                               , QPushButton* pushButton_reload
                               , QPushButton* pushButton_guardar_cambio
                               , QPushButton* pushButton_ver_internet)
{
    this->lineEdit_id_cambio = lineEdit_id_cambio;
    this->lineEdit_cambio = lineEdit_cambio;
    this->dateEdit_cambio = dateEdit_cambio;
    this->label_loading = label_loading;
    this->pushButton_reload = pushButton_reload;
    this->pushButton_guardar_cambio = pushButton_guardar_cambio;
    this->pushButton_ver_internet = pushButton_ver_internet;

    if (!timer_cambio) {
        timer_cambio = new QTimer();
        connect(timer_cambio, SIGNAL(timeout()), this, SLOT(show_timeCambio()));
    }

    disconnect(this->dateEdit_cambio, SIGNAL(dateChanged(QDate)), this, SLOT(on_dateEdit_cambio_dateChanged(QDate)));
    this->dateEdit_cambio->setDate(QDate::currentDate());
    connect(this->dateEdit_cambio, SIGNAL(dateChanged(QDate)), this, SLOT(on_dateEdit_cambio_dateChanged(QDate)));

    connect(this->pushButton_ver_internet, SIGNAL(clicked()), this, SLOT(on_pushButton_ver_internet_clicked()));

    connect(this->pushButton_reload, SIGNAL(clicked()), this, SLOT(on_pushButton_reload_clicked()));

    connect(this->pushButton_guardar_cambio, SIGNAL(clicked()), this, SLOT(on_pushButton_guardar_cambio_clicked()));

    if (!webView_tipo_cambio) {
        webView_tipo_cambio = new QWebEngineView();
        connect(webView_tipo_cambio, SIGNAL(loadStarted()), this, SLOT(loadStarted()));
        connect(webView_tipo_cambio, SIGNAL(loadFinished(bool)), this, SLOT(loadFinished(bool)));

        webView_tipo_cambio->setWindowModality(Qt::WindowModal);
    }

    QThread* thread = new QThread;
    connect(thread, SIGNAL(started()), this, SLOT(on_thread_loadWeb()));
    connect(thread, SIGNAL(finished()), this, SLOT(on_thread_finished()));

    thread->start();
}
void SunatTipoCambio::consultar()
{
    on_pushButton_reload_clicked();
}

void SunatTipoCambio::setCambio(const QDate& date)
{
    countDatos++;

    QWebEngineView* view = webView_tipo_cambio;
    QWebEnginePage* page = view->page();
    QString strJS;

    strJS = "document.getElementsByClassName('lb')[0].selectedIndex = "
        + QString().setNum(date.month());
    page->runJavaScript(strJS, [](const QVariant v) { /*qDebug()<<"set img: "<<endl<< v.toString();*/});

    strJS = "document.getElementsByClassName('lb')[1].value = "
        + QString().setNum(date.year());
    page->runJavaScript(strJS, [](const QVariant v) { /*qDebug()<<"set canvas: "<<endl<< v.toString();*/});

    strJS = "document.getElementsByClassName('button')[0].click()";
    page->runJavaScript(strJS, [](const QVariant v) { /*qDebug()<<"set canvas: "<<endl<< v.toString();*/});

    timer_cambio->start(300);
}
void SunatTipoCambio::on_thread_loadWeb()
{
    webView_tipo_cambio->load(tr("http://www.sunat.gob.pe/cl-at-ittipcam/tcS01Alias"));
}
void SunatTipoCambio::on_thread_finished()
{
    delete sender();
}
void SunatTipoCambio::show_timeCambio()
{
    if(transcurred_time > 2500){
        transcurred_time = 0;
        timer_cambio->stop();

        label_loading->clear();
        label_loading->setText("...");
        return;
    }
    transcurred_time += 300;

    QWebEngineView* view = webView_tipo_cambio;
    QWebEnginePage* page = view->page();

    QString strJS = "var mes = document.getElementById(\"mesElegido\").value;"
                    "var anio = document.getElementById(\"anioElegido\").value;"
                    "var table = document.body.getElementsByClassName('class=\"form-table\"')[0];"
                    "var length = table.rows.length;"
                    "var text = mes + \" \" + anio + \" \";"
                    "for(i = 1; i < length; i++){ text += table.rows[i].innerText + \" \"; };"
                    "text;";

    QString str_file = "cambio.dat";

    page->runJavaScript(strJS, [=](const QVariant &v)
    {
        QFile file(str_file);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return;
        file.flush();
        QTextStream out(&file);
        out << v.toString();
        file.close();
    });
    QFile file(str_file);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QString output = QString(" ") + file.readAll();
    file.close();

    QStringList list = output.split(QRegularExpression("\\s+"), QString::SkipEmptyParts);

    if (list.count() > 0)
    {
        int month = dateEdit_cambio->date().month();
        int year = dateEdit_cambio->date().year();

        if (list.count() < 2) return;
        int mes = list[0].toInt();
        int anio = list[1].toInt();

        if (month != mes || year != anio) return;

        QString msg = list[2];
        if (msg.compare("No") == 0) {
            timer_cambio->stop();
            label_loading->clear();
            label_loading->setText("...");

            return;
        }

        QString r;
        int day = dateEdit_cambio->date().day();
        int day_hint = 0;
        int i = 2;
        for (; i < list.count(); i = i + 3) {
            day_hint = list[i].toInt();
            if (day <= day_hint) {
                break;
            }
            r += " " + list[i];
        }

        double cambio_compra = 0.0;
        if (day < day_hint) {
            if (i != 2){
                cambio_compra = list[i - 2].toDouble();
                lineEdit_cambio->setText(QString().setNum(cambio_compra, 'f', 3));
            }
        }
        if (day == day_hint) {
            cambio_compra = list[i + 1].toDouble();
            lineEdit_cambio->setText(QString().setNum(cambio_compra, 'f', 3));
        }
        if (day > day_hint) {
            cambio_compra = list[i - 2].toDouble();
            lineEdit_cambio->setText(QString().setNum(cambio_compra, 'f', 3));
        }
        timer_cambio->stop();
        label_loading->clear();
        label_loading->setText("...");

        if(countReload == 0){
            countDatos = 0;
            countReload++;
            webView_tipo_cambio->reload();
        }

    }
}
void SunatTipoCambio::loadStarted()
{
    if(countDatos > 0){
        return;
    }
    if(!pushButton_guardar_cambio->isHidden()){
        QMovie *movie = new QMovie(":/new/Iconos/loading_4.gif");
        label_loading->setMovie(movie);
        movie->start();
    }
}

void SunatTipoCambio::loadFinished(bool b)
{
    if(countDatos > 0){
        return;
    }
    if(b){
        if(!pushButton_guardar_cambio->isHidden()){
            setCambio(this->dateEdit_cambio->date());
        }
    }else{
        this->lineEdit_cambio->setText("0");

        label_loading->clear();
        label_loading->setText("...");
        this->label_loading->hide();
        this->label_loading->show();
    }
}
void SunatTipoCambio::on_dateEdit_cambio_dateChanged(const QDate &date)
{
    countDatos = 0;
    countReload = 0;
    transcurred_time = 0;
    timer_cambio->stop();

    webView_tipo_cambio->reload();
}

void SunatTipoCambio::on_pushButton_reload_clicked()
{
    countDatos = 0;
    countReload = 0;
    transcurred_time = 0;
    timer_cambio->stop();

    setCambio(dateEdit_cambio->date());
}
void SunatTipoCambio::on_pushButton_guardar_cambio_clicked()
{
    /*
    countReload = 0;
    transcurred_time = 0;
    timer_cambio->stop();
    setCambio(dateEdit_cambio->date());
    */
}
void SunatTipoCambio::on_pushButton_ver_internet_clicked()
{
    webView_tipo_cambio->show();
}
