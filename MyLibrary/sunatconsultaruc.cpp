#include "sunatconsultaruc.h"

#include <baseapi.h>
#include <allheaders.h>
#include <QWebEngineProfile>

SunatConsultaRUC::SunatConsultaRUC(QWidget *parent) :
    QWidget(parent)
{	
	view_consultaRuc = new QWebEngineView;
	QWebEnginePage* page = view_consultaRuc->page();
    connect(page, SIGNAL(loadFinished(bool)), this, SLOT(loadFinished(bool)));


	timer_image = new QTimer(this);
	connect(timer_image, SIGNAL(timeout()), this, SLOT(show_time_image()));
	
	timer_datos = new QTimer(this);
	connect(timer_datos, SIGNAL(timeout()), this, SLOT(show_time_datos()));

    timer_consultar_loc_anexos = new QTimer(this);
    connect(timer_consultar_loc_anexos, SIGNAL(timeout()), this, SLOT(show_time_consultar_loc_anexos()));

    timer_establecimientos = new QTimer(this);
    connect(timer_establecimientos, SIGNAL(timeout()), this, SLOT(show_time_establecimientos()));
	
	isLoad = false;
    isImageRead = false;
    isDatosRead = false;
    count = 0;
	count_datos = 0;

    tt_image = 0;
	time_transcurred = 0;
    time_establecimientos_transcurred = 0;

	firstTime_load = 0;

    lock = false;
}

SunatConsultaRUC::~SunatConsultaRUC()
{    
	if (view_consultaRuc)
		delete view_consultaRuc;
	if (timer_image)
		delete timer_image;
	if (timer_datos)
		delete timer_datos;
    if (timer_consultar_loc_anexos)
        delete timer_consultar_loc_anexos;
    if (timer_establecimientos)
        delete timer_establecimientos;
}

void SunatConsultaRUC::set_data(QLabel* label_captcha
	, QLineEdit* le_ruc, QLineEdit* le_captcha
	, QLineEdit* le_razonSocial, QLineEdit* le_tipoContribuyente
	, QLineEdit* le_nombreComercial, QLineEdit* le_direccion
    , QLineEdit* le_estado, QLineEdit* le_condicion, QTableWidget* table_establecimientos)
{        
    this->le_ruc = le_ruc;
	QRegExp regExp_ruc("[0-9]{11,11}");
	this->le_ruc->setValidator(new QRegExpValidator(regExp_ruc));

	this->le_captcha = le_captcha;
    QRegExp regExp_codigo_captcha("[a-zA-Z]{4,4}");    
	this->le_captcha->setValidator(new QRegExpValidator(regExp_codigo_captcha));
	this->le_captcha->setText("1234");

    connect(this->le_captcha, SIGNAL(textEdited(QString)), this, SLOT(on_le_consulta_ruc_textEdited()));
	connect(this->le_ruc, SIGNAL(textEdited(QString)), this, SLOT(on_le_consulta_ruc_textEdited()));

    this->label_captcha= label_captcha;

	this->le_razonSocial = le_razonSocial;
	this->le_tipoContribuyente = le_tipoContribuyente;
	this->le_nombreComercial = le_nombreComercial;
	this->le_estado = le_estado;
	this->le_condicion = le_condicion;
	this->le_direccion = le_direccion;
    this->table_establecimientos = table_establecimientos;

    connect(view_consultaRuc, SIGNAL(renderProcessTerminated(QWebEnginePage::RenderProcessTerminationStatus,int))
            , this, SLOT(renderProccessTerminated()));

    view_consultaRuc->setUrl(tr("http://e-consultaruc.sunat.gob.pe/cl-ti-itmrconsruc/jcrS00Alias"));
    label_captcha->show();
    //qDebug()<<QDir::currentPath()<<endl;
    //qDebug()<<QApplication::applicationDirPath()<<endl;
    //qDebug()<<QApplication::applicationFilePath()<<endl;
    /*
    view_consultaRuc->page()->profile()->setCachePath(QApplication::applicationDirPath());
    view_consultaRuc->page()->profile()->setPersistentStoragePath(QApplication::applicationDirPath());
    view_consultaRuc->page()->profile()->clearAllVisitedLinks();
    view_consultaRuc->page()->profile()->clearHttpCache();
    view_consultaRuc->page()->profile()->setHttpCacheType(QWebEngineProfile::NoCache);
    view_consultaRuc->page()->profile()->setPersistentCookiesPolicy(QWebEngineProfile::NoPersistentCookies);
    */

    view_consultaRuc->load(tr("http://e-consultaruc.sunat.gob.pe/cl-ti-itmrconsruc/jcrS00Alias"));
    //view_consultaRuc->reload(tr("http://www.sunat.gob.pe/cl-ti-itmrconsruc/jcrS00Alias"));

    view_consultaRuc->show();
    view_consultaRuc->lower();
    //SYSTEM->get_mainw(this).show();
    //view_consultaRuc->hide();
}
void SunatConsultaRUC::consultar_anexos()
{
    QWebEngineView* view = view_consultaRuc;
    QWebEnginePage* page = view->page();

    QString strJS = "for(i=0; i<frames[1].document.body.getElementsByTagName('form').length; i++) { ";
    strJS += "if(frames[1].document.body.getElementsByTagName('form')[i].name.localeCompare('formLocAnex') == 0) {";
    strJS += "var text = frames[1].document.body.getElementsByTagName('form')[i].nroRuc.value;";
    strJS += "frames[1].document.body.getElementsByTagName('form')[i].submit.click();break;}};text;";
    qDebug()<<"strJS: "<<strJS<<endl;

    QString str_name_file = "sunat_ruc_e.txt";

    page->runJavaScript(strJS, [=](const QVariant &v)
    {
        QFile file;
        file.setFileName(str_name_file);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return;
        file.flush();        
        QTextStream out(&file);
        out << v.toString();
        file.close();
    });
}

void SunatConsultaRUC::show_time_datos()
{	
	//disconnect(timer_datos, SIGNAL(timeout()), this, SLOT(show_time_datos()));

	setDatos();
}
void SunatConsultaRUC::show_time_image()
{
    setImage();
}

void SunatConsultaRUC::show_time_consultar_loc_anexos()
{
    consultar_anexos();
    timer_consultar_loc_anexos->stop();
    timer_establecimientos->start(100);
}

void SunatConsultaRUC::show_time_establecimientos()
{
    setEstablecimientos();
}

void SunatConsultaRUC::loadFinished(bool b)
{
    //isLoad = b;

	if (firstTime_load == 0) {
		firstTime_load++;
		return;
	}
    qDebug()<<"BEGIN"<<endl;

    if(b){
        timer_image->start(300);
    }

}

void SunatConsultaRUC::on_le_consulta_ruc_textEdited()
{
	if (lock) return;

	QString captcha = le_captcha->text();
	QString ruc = le_ruc->text();

    if (ruc.length() == 11 && captcha.length() == 4){        
		QWebEngineView* view = view_consultaRuc;
		
        //if (isLoad) {
            //loadFinished(true);
        //} else {
            view->reload();
        //}

    }    		
}
void SunatConsultaRUC::renderProccessTerminated()
{
    qDebug()<<"haber"<<endl;
}

void SunatConsultaRUC::setImage()
{			
	//if (isImageRead) return;	
	qDebug() << "Read Image" << endl;
        
    /*
    if(tt_image > 2500){
        tt_image = 0;
        timer_image->stop();
    }
    tt_image += 100;
    */

	QWebEngineView* view = view_consultaRuc;
	QWebEnginePage* page = view->page();

	QString strJS = "var img = frames[0].imagen;"
		"var canvas = document.createElement('canvas');"
		"var width = canvas.width = img.width;"
		"var height = canvas.height = img.height;"
		"var ctx = canvas.getContext('2d');"
		"ctx.drawImage(img, 0, 0);"
		"var dataURL = canvas.toDataURL('image/png');"
		"var text = width + ' ' + height + ' ' + dataURL.replace(/^data:image\\/(png|jpg);base64,/, '');"
		"text;";
							
	qDebug() << strJS << endl;
	page->runJavaScript(strJS, [](const QVariant &v)
	{
        QFile file;
        file.setFileName("image.dat");
		if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
			return;

        //qDebug()<<v.toString()<<endl;
        file.flush();
        QTextStream out(&file);
        out << v.toString();
        file.close();
	});

	QFile file;
	file.setFileName("image.dat");
	if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
		return;
	QString str_img;
	str_img = file.readLine();

	int i_width = str_img.indexOf(' ', 0);
	int width = str_img.mid(0, i_width).toInt();

	str_img = str_img.mid(i_width+1, -1);

	int i_height = str_img.indexOf(' ', 0);	
	int height = str_img.mid(0, i_height).toInt();

	str_img = str_img.mid(i_height+1, -1);

	qDebug() << "str_img: " << str_img.length() << endl;

	if (str_img.length() < 500) {
		qDebug() << "no Image" << endl;		
		return;
	}

	QByteArray array = QByteArray::fromBase64(str_img.toStdString().c_str());
	
	QImage img_2;
	
	if (!img_2.loadFromData(array, "PNG")) {
		qDebug() << "no load image" << endl;
		return;
	} else {		

	}

    QImage img;
    if(label_captcha)
        if(label_captcha->pixmap())
			img = label_captcha->pixmap()->toImage();

    if (img != img_2) {
		qDebug() << "NO SON IGUALES" << endl;               

        if(count < 4){
            count++;
            return;
        }

        tt_image = 0;
        timer_image->stop();
        count = 0;


        label_captcha->setPixmap(QPixmap::fromImage(img_2));

        if (img_2.save("file.png"))
            qDebug() << "SAVE FILE" << endl;
        else
            qDebug() << "not SAVE FILE" << endl;

        QSize sizeImage = img_2.size();
        int width = sizeImage.width(), height = sizeImage.height();

        qDebug() << width << endl;
        qDebug() << height << endl;
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                QColor color = img_2.pixelColor(x, y);

                int countLightColor = 0;
                if (color.red() <= 255 && color.red() >= 216
                    && color.green() <= 255 && color.green() >= 216
                    && color.blue() <= 255 && color.blue() >= 216) countLightColor++;

                if (countLightColor == 1) img_2.setPixel(x, y, qRgb(255, 255, 255));

                int countHeavyColor = 0;
                if (!(color.red() <= 255 && color.red() >= 216
                    && color.green() <= 255 && color.green() >= 216
                    && color.blue() <= 255 && color.blue() >= 216)) countHeavyColor++;

                if (countHeavyColor == 1) img_2.setPixel(x, y, qRgb(0, 0, 0));
            }
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
            //qDebug() << "INITIALIZE TESSERACT" << endl;
        }


        // Open input image with leptonica library
        Pix *image = pixRead("file.png");

        api->SetImage(image);
        // Get OCR result
        outText = api->GetUTF8Text();

        qDebug() << "OUTPUT: " << outText << endl;

        QString str_output = outText;

        str_output.replace("\n", "");
        str_output.replace(" ", "");
        qDebug() << "OUTPUT_CORRECTNESS: " << str_output << endl;

        str_output = str_output.toUpper();

        qDebug() << "OUTPUT_CORRECTNESS TO UPPER: " << str_output << endl;

        if (str_output.length() == 4)
            le_captcha->setText(str_output);
        else
            le_captcha->setText("AAAA");

        // Destroy used object and release memory
        api->End();
        delete[] outText;

        QString ruc = le_ruc->text();
        if (ruc.length() != 11) {
            return;
        }
        QWebEngineView* view = view_consultaRuc;
        QWebEnginePage* page = view->page();

        QString captcha = le_captcha->text();

        QString strJS = QString() + "frames[0].document.mainForm.search1.value = "
        + "'" + ruc + "';";

        strJS += "frames[0].document.mainForm.codigo.value = "
        "'" + captcha + "';";

        strJS += "frames[0].document.getElementsByClassName('form-button')[0].click();";

        page->runJavaScript(strJS, [](const QVariant &v) { //qDebug() << v.toString();
        });

        le_ruc->selectAll();
        le_ruc->setFocus();

        timer_datos->start(100);
	} else {
		qDebug() << "SON IGUALES" << endl;
	}
}

void SunatConsultaRUC::setDatos()
{
	time_transcurred += 100;
	qDebug() << "time_transcurred: " << time_transcurred << endl;

    if (time_transcurred == 2500) {
		time_transcurred = 0;
		timer_datos->stop();

        /*
        QWebEngineView* view = view_consultaRuc;
        QWebEnginePage* page = view->page();

        QString strJS = "frames[0].goRefresh();";

        qDebug() << strJS << endl;
        page->runJavaScript(strJS, [](const QVariant &v) { //qDebug() << v.toString();
        });

        on_le_consulta_ruc_textEdited();
        */
		return;
	}
	
    QWebEngineView* view = view_consultaRuc;
    QWebEnginePage* page = view->page();
    
    int numItems = 19;

	QString strJS = "var text = '';"
		"for(i = 0; i < "+QString().setNum(numItems)+"; i++) {"
		"text += frames[1].document.body.getElementsByTagName('td')[i].innerText + '\\n\\n';"
		"}";
	qDebug()<<"strJS: "<<strJS<<endl;

	QFile file;
	QString str_name_file = "sunat_datos.txt";
		
	page->runJavaScript(strJS, [=](const QVariant &v)
	{
        QFile file;
        file.setFileName(str_name_file);
		if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
			return;
		file.flush();
        //qDebug() << v << endl;
		QTextStream out(&file);
		out << v.toString();
        //qDebug()<<v.toString()<<endl;
		file.close();
	});

	QByteArray bArray;
	QString str_document;
		
	file.setFileName(str_name_file);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        //return;
    }
	bArray = file.readAll();
	str_document = QString().fromLatin1(bArray);

	QList<QString> list_result;
	list_result = str_document.split("\n\n", QString::SkipEmptyParts);
	qDebug()<<"str_document: "<<str_document<<endl;        
    //file.close();
		
    QList<QString> list_search{"Número de RUC:", "Tipo Contribuyente:"
                            , "Nombre Comercial:", "Estado del Contribuyente:"
                            , "Condición del Contribuyente:", "Dirección del Domicilio Fiscal:"};

    QList<QString> list_items;
    for(int i = 0; i < list_search.size(); i++) {
        for(int j = 0; j < list_result.size()-1; j++) {
            if(list_result[j].compare(list_search[i]) == 0) {
                QString result = list_result[j+1];
                list_items.push_back(result);
                //qDebug()<<result<<endl;
                break;
            }else{
                //qDebug()<<"no result"<<endl;
            }
        }
    }

    if(list_items.size() < 6) return;                         

    QString ruc_razon= list_items[0];
    int posMinus = ruc_razon.indexOf('-');
    QString ruc = ruc_razon.mid(0, posMinus-1);

    if (this->le_ruc->text().compare(ruc) == 0) {
        this->ruc = le_ruc->text();

		time_transcurred = 0;
        timer_datos->stop();
	} else {
		return;
	}

    QString razon = ruc_razon.mid(posMinus+2, ruc_razon.length()-(posMinus+2));

	if (le_razonSocial)
		le_razonSocial->setText(razon);
	if (le_tipoContribuyente)
		le_tipoContribuyente->setText(list_items[1]);
	if (le_nombreComercial)
		le_nombreComercial->setText(list_items[2]);
	if (le_estado)
		le_estado->setText(list_items[3]);
	if (le_condicion)
		le_condicion->setText(list_items[4]);
	if (le_direccion)
		le_direccion->setText(list_items[5]);        

    table_establecimientos->setRowCount(0);
    table_establecimientos->clearContents();

    timer_consultar_loc_anexos->start(2000);
}

void SunatConsultaRUC::setEstablecimientos()
{        
    time_establecimientos_transcurred += 100;
    qDebug() << "time_establecimientos_transcurred: " << time_establecimientos_transcurred << endl;

    if (time_establecimientos_transcurred > 2500) {
        time_establecimientos_transcurred = 0;
        timer_establecimientos->stop();
        return;
    }

    QFile file;
    QString str_name_file = "sunat_ruc_e.txt";

    QByteArray bArray_ruc;
    QString str_ruc;

    file.setFileName(str_name_file);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    bArray_ruc = file.readAll();
    str_ruc = QString().fromLatin1(bArray_ruc);

    file.close();

    if(le_ruc->text().compare(str_ruc) == 0) {

    }else{
        return;
    }

    QString strJS = "var rows = frames[1].document.body.getElementsByClassName('beta')[0].getElementsByTagName('table')[0].rows.length;";
    strJS += "var columns = frames[1].document.body.getElementsByClassName('beta')[0].getElementsByTagName('table')[0].rows[0].cells.length;";
    strJS += "var text = rows + ' &&& ' + columns;";
    strJS += "for(i=0; i < rows; i++) {";
    strJS += "for(j=0; j < columns; j++) {";
    strJS += "text += ' &&& '+frames[1].document.body.getElementsByClassName('beta')[0].getElementsByTagName('table')[0].getElementsByTagName('tr')[i].cells[j].innerText;";
    strJS += "}";
    strJS += "};";
    strJS += "text=frames[1].document.body.getElementsByClassName('t1')[0].innerText + ' &&& ' + text";

    qDebug()<<"strJS: "<<strJS<<endl;    

    str_name_file = "sunat_establecimientos.txt";

    QWebEngineView* view = view_consultaRuc;
    QWebEnginePage* page = view->page();

    page->runJavaScript(strJS, [=](const QVariant &v)
    {
        QFile file;
        file.setFileName(str_name_file);
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
    list_result = str_document.split("&&&", QString::SkipEmptyParts);
    qDebug()<<"str_document: "<<str_document<<endl;
    file.close();

    if(list_result.size() < 3) {
        return;
    }

    QList<QString> list_title;
    QString str_title = list_result[0].simplified();
    list_title = str_title.split(" ", QString::SkipEmptyParts);

    if(list_title.size() < 4) {
        return;
    }
    QString ruc = list_title[3];

    qDebug()<<"ruccito: "<<ruc<<endl;
    if(le_ruc->text().compare(ruc) == 0){

    }else{
        return;
    }
    time_establecimientos_transcurred = 0;
    timer_establecimientos->stop();

    int rows = list_result[1].simplified().toInt();
    int columns = list_result[2].simplified().toInt();

    qDebug()<<list_result<<endl;
    if (rows <= 0) {
        return;
    }

    int count = 3;
    //table_establecimientos->setColumnCount(columns);
    QStringList list;
    for(int j = 0; j < columns; j++) {
        list<<list_result[count].simplified();
        count++;
    }
    //table_establecimientos->setHorizontalHeaderLabels(list);

    rows = rows - 1;
    table_establecimientos->setRowCount(rows);

    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < columns; j++) {
            QTableWidgetItem* item = new QTableWidgetItem(list_result[count].simplified());
            table_establecimientos->setItem(i, j, item);
            item->setFlags(Qt::ItemIsEnabled
                            | Qt::ItemIsSelectable);
            count++;
        }
    }
    SYSTEM->table_resize_to_contents(0, table_establecimientos);    
}
