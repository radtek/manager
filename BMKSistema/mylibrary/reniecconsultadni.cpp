#include "reniecconsultadni.h"

#include "baseapi.h"
#include "allheaders.h"
#include <QWebEngineProfile>

ReniecConsultaDNI::ReniecConsultaDNI(QWidget *parent) :
    QWidget(parent)
{
    view_consultaDNI = new QWebEngineView;
    QWebEnginePage* page = view_consultaDNI->page();
    connect(page, SIGNAL(loadFinished(bool)), this, SLOT(loadFinished(bool)));


    timer_image = new QTimer;
    connect(timer_image, SIGNAL(timeout()), this, SLOT(show_time_image()));

    timer_datos = new QTimer;
    connect(timer_datos, SIGNAL(timeout()), this, SLOT(show_time_datos()));

    isLoad = false;
    time_transcurred = 0;
    count = 0;
    tt_image = 0;
}
// document.getElementsByClassName('tecla')[10].click();
ReniecConsultaDNI::~ReniecConsultaDNI()
{
    if (view_consultaDNI)
        delete view_consultaDNI;
    if (timer_image)
        delete timer_image;
    if (timer_datos)
        delete timer_datos;
}

void ReniecConsultaDNI::set_data(QLabel* label_captcha, QLineEdit* lineEdit_dni
    , QLineEdit* lineEdit_codigoCaptcha, QLineEdit* lineEdit_nombre
    , QLineEdit* lineEdit_direccion)
{
    this->le_dni = lineEdit_dni;
    this->le_direccion = lineEdit_direccion;

    this->le_captcha = lineEdit_codigoCaptcha;
    this->le_nombre = lineEdit_nombre;

    //QRegExp regExp_nombre("[0-9]{8,8}");
    QRegExp regExp_codigo_captcha("[a-zA-Z]{4,4}");
    //this->le_nombre->setValidator(new QRegExpValidator(regExp_nombre));
    this->le_captcha->setValidator(new QRegExpValidator(regExp_codigo_captcha));

    this->le_captcha->setText("1234");

    //connect(this->le_captcha, SIGNAL(textEdited(QString)), this, SLOT(on_le_consulta_nombre_textEdited()));
    connect(this->le_dni, SIGNAL(textEdited(QString)), this, SLOT(on_le_consulta_dni_textEdited()));

    this->label_captcha = label_captcha;

    QThread* thread = new QThread;
    connect(thread, SIGNAL(started()), this, SLOT(on_thread_loadWeb()));
    connect(thread, SIGNAL(finished()), this, SLOT(on_thread_finished()));

    thread->start();
    /*
    view_consultaDNI->page()->profile()->setPersistentCookiesPolicy(QWebEngineProfile::NoPersistentCookies);
    view_consultaDNI->page()->profile()->setHttpCacheType(QWebEngineProfile::NoCache);
    view_consultaDNI->page()->profile()->clearHttpCache();
    view_consultaDNI->page()->profile()->cookieStore()->deleteAllCookies();
    */

    //view_consultaDNI->load(tr("https://cel.reniec.gob.pe/valreg/valreg.do?accion=ini"));

    //view_consultaDNI->show();
}
void ReniecConsultaDNI::on_thread_loadWeb()
{
    view_consultaDNI->load(tr("https://cel.reniec.gob.pe/valreg/valreg.do?accion=ini"));
    //view_consultaDNI->show();
}
void ReniecConsultaDNI::on_thread_finished()
{
    delete sender();
}
void ReniecConsultaDNI::setImage()
{
    if(tt_image > 2500){
        tt_image = 0;
        timer_image->stop();
        isLoad = false;
    }
    tt_image += 300;

    QWebEngineView* view = view_consultaDNI;
    QWebEnginePage* page = view->page();

    QString strJS = "var img = document.getElementsByTagName('td')[27].children[0];"
        "var canvas = document.createElement('canvas');"
        "var width = canvas.width = img.width;"
        "var height = canvas.height = img.height;"
        "var ctx = canvas.getContext('2d');"
        "ctx.drawImage(img, 0, 0);"
        "var dataURL = canvas.toDataURL('image/png');"
        "var text = width + ' ' + height + ' ' + dataURL.replace(/^data:image\\/(png|jpg);base64,/, '');"
        "text;";

    //qDebug() << strJS << endl;
    page->runJavaScript(strJS, [](const QVariant &v)
    {
        QFile file("image_dni.dat");
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return;
        file.flush();
        QTextStream out(&file);
        out << v.toString();
        file.close();
    });
    QFile file;
    file.setFileName("image_dni.dat");
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
        return;
    QString str_img;
    str_img = file.readLine();

    int i_width = str_img.indexOf(' ', 0);

    str_img = str_img.mid(i_width + 1, -1);

    int i_height = str_img.indexOf(' ', 0);

    str_img = str_img.mid(i_height + 1, -1);

    //qDebug() << "str_img: " << str_img.length() << endl;
    if (str_img.length() < 500) {
        //qDebug() << "no Image" << endl;
        return;
    }

    QByteArray array = QByteArray::fromBase64(str_img.toStdString().c_str());

    QImage img_2;

    if (!img_2.loadFromData(array, "PNG")) {
        //qDebug() << "no load image" << endl;
        return;
    }
    else {

    }

    QImage img;
    if (label_captcha)
        if (label_captcha->pixmap())
            img = label_captcha->pixmap()->toImage();

    //if (img != img_2) {
        //qDebug() << "NO SON IGUALES" << endl;
    {
        if (count < 5) {
            count++;
            return;
        }
        isLoad = false;
        tt_image = 0;
        timer_image->stop();
        count = 0;

        QSize sizeImage = img_2.size();
        int width = sizeImage.width(), height = sizeImage.height();

        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                QColor color = img_2.pixelColor(x, y);

                int countLightColor = 0;
                if (color.red() <= 100 && color.red() >= 0
                    && color.green() <= 100 && color.green() >= 0
                    && color.blue() <= 255 && color.blue() >= 140) countLightColor++;

                if (countLightColor == 1) img_2.setPixel(x, y, qRgb(0, 0, 0));

                int countHeavyColor = 0;
                if (!(color.red() <= 100 && color.red() >= 0
                    && color.green() <= 100 && color.green() >= 0
                    && color.blue() <= 255 && color.blue() >= 140)) countHeavyColor++;

                if (countHeavyColor == 1) img_2.setPixel(x, y, qRgb(255, 255, 255));

            }
        }

        label_captcha->setPixmap(QPixmap::fromImage(img_2));

        if (img_2.save("image_dni_pre.png")){
            //qDebug() << "SAVE FILE" << endl;
        }else{
            //qDebug() << "not SAVE FILE" << endl;
        }

        char *outText;

        tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();

        // Initialize tesseract-ocr with English, without specifying tessdata path
        int r = api->Init("C:/Users/lorda/Desktop/VS2015_Tesseract-master/tesseract_3.04/tessdata/", "eng", tesseract::OEM_DEFAULT, NULL, 0, NULL, NULL, true);
        //qDebug() << "r: " << r << endl;
        if (r) {
            qDebug() << "NOT initialize TESSERACT" << endl;
            qDebug() << "3" << endl;
            fprintf(stderr, "Could not initialize tesseract.\n");
            exit(1);
        } else {
            qDebug() << "INITIALIZE TESSERACT" << endl;
        }

        // Open input image with leptonica library
        Pix *image = pixRead("image_dni_pre.png");

        api->SetImage(image);
        // Get OCR result
        outText = api->GetUTF8Text();

        //qDebug() << "OUTPUT: " << outText << endl;

        QString str_output = outText;

        str_output.replace("\n", "");
        str_output.replace(" ", "");
        //qDebug() << "OUTPUT_CORRECTNESS: " << str_output << endl;

        str_output = str_output.toUpper();

        //qDebug() << "OUTPUT_CORRECTNESS TO UPPER: " << str_output << endl;

        if (str_output.length() == 4)
            le_captcha->setText(str_output);
        else
            le_captcha->setText("AAAA");

        // Destroy used object and release memory
        api->End();
        delete[] outText;

        QString dni = le_dni->text();

        //qDebug() << "dni: " << dni << endl;
        if (dni.length() != 8) {
            return;
        }

        QWebEngineView* view = view_consultaDNI;
        QWebEnginePage* page = view->page();

        QString captcha = le_captcha->text();

        QString strJS = QString() + "var t = '" + dni + "';"
            "var tds = document.getElementsByTagName('td');"
            "tds[23].children[0].click();"
            "for (i=0; i<8; i++) {"
            "for (j=13; j<23; j++) {"
            "if (t[i] == tds[j].children[0].children[0].value) {"
            "tds[j].children[0].click();"
            "break;"
            "}"
            "}"
            "}"
            "tds[28].children[0].value = '" + captcha + "';"
            "tds[29].children[0].click();";

        //qDebug() << strJS << endl;

        page->runJavaScript(strJS, [](const QVariant &v) { //qDebug() << v.toString();
        });

        //le_nombre->selectAll();
        //le_nombre->setFocus();

        timer_datos->start(100);
    }
    //} else {
        //qDebug() << "SON IGUALES" << endl;
    //}
}

void ReniecConsultaDNI::setDatos()
{
    time_transcurred += 100;
    //qDebug() << "time_transcurred: " << time_transcurred << endl;

    if (time_transcurred == 2800) {
        time_transcurred = 0;
        timer_datos->stop();

        //on_le_consulta_dni_textEdited();
        return;
    }

    QWebEngineView* view = view_consultaDNI;
    QWebEnginePage* page = view->page();

    QString strJS = "var text = document.getElementsByTagName('td')[33].innerText;"
        "var end = text.indexOf('\\n');"
        "text.substring(0, end) + '\\n' + text.substring(end + 1, end + 9);";

    //qDebug() << "strJS: " << strJS << endl;

    QFile file;
    QString str_name_file = "reniec_datos.txt";

    page->runJavaScript(strJS, [=](const QVariant &v)
    {
        QFile file(str_name_file);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return;
        file.flush();
        //qDebug() << v << endl;
        QTextStream out(&file);
        out << v.toString();
        file.close();
    });

    QByteArray bArray;
    QString str_document;

    file.setFileName(str_name_file);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    bArray = file.readAll();
    str_document = QString().fromLatin1(bArray);

    QList<QString> list_result;
    list_result = str_document.split("\n", QString::SkipEmptyParts);
    //qDebug() << "str_document: " << str_document << endl;
    file.close();

    if (list_result.size() < 2) return;

    QString nombre = list_result[0];
    QString dni = list_result[1];

    if (le_dni->text().compare(dni) == 0) {
        time_transcurred = 0;
        timer_datos->stop();        
    } else {
        return;
    }
    if (le_nombre)
        le_nombre->setText(nombre);
}

void ReniecConsultaDNI::show_time_datos()
{
    setDatos();
}

void ReniecConsultaDNI::show_time_image()
{
    setImage();
}

void ReniecConsultaDNI::loadFinished(bool b)
{
    if (!isLoad) {
        isLoad = b;
        return;
    }

    if (b) {
        timer_image->start(300);
    }
}

void ReniecConsultaDNI::on_le_consulta_dni_textEdited()
{
    QString dni = le_dni->text();
    QString captcha = le_captcha->text();

    if (dni.length() == 8){
        QString str_query = "";
        QSqlQuery query;
        str_query += "SELECT nombre, direccion FROM persona";
        str_query += " WHERE cod = '" + dni + "'";

        if(query.exec(str_query)){
            if(query.next()){
                QString nombre = query.value(0).toString();
                QString direccion = query.value(1).toString();

                le_nombre->setText(nombre);
                if(le_direccion)
                    le_direccion->setText(direccion);
                return;
            }
        }

    }

    if (dni.length() == 8 && captcha.length() == 4) {
        //qDebug() << "on_le_consulta_dni_textEdited()" << endl;
        //QWebEngineView* view = view_consultaDNI;

        //loadFinished(isLoad);
        view_consultaDNI->reload();

    }else{
        timer_image->stop();
        timer_datos->stop();

        time_transcurred = 0;
        count = 0;
        tt_image = 0;
    }
}
