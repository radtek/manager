#include "sistema.h"
#include <cwchar>
#include <comdef.h>
#include <png.h>
#include <QPrinter>
#include <QPixmap>
#include <QByteRef>

QString Sistema::name_persona = "";
QString Sistema::pass_persona = "";
QString Sistema::tipo_persona = "";

Sistema::Sistema()
{
    dolar = 0.0;
    igv = 0.0;
    // use title for defect for validate events of wizardwidget
    rdy_for_trig = "Form";

    ssheet_select = "color: rgb(255, 255, 255);"
            "background-color: rgb(0, 0, 0);";
    ssheet_no_select = "";

    curr_widget = NULL;
}
QString& Sistema::normalDecimal(QString &value)
{
    int count = 0;
    for(int i=0; i<value.length(); i++){
        if(QString(value[i]).compare("0") == 0){
            count++;
        }else{
            break;
        }
    }
    value = value.remove(0, count);

    bool hasDot = false;
    for(int i=0; i<value.length(); i++){
        if(QString(value[i]).compare(".") == 0){
            hasDot = true;
            break;
        }
    }
    if(!hasDot){
        if(value.compare("") == 0){
            value = "0";
        }
    }else{
        if(QString(value[0]).compare(".") == 0){
            value = value.insert(0, '0');
        }

        count = 0;
        for(int i=value.length()-1; i>=0; i--){
            if(QString(value[i]).compare("0") == 0){
                count++;
            }else{                
                break;
            }
        }
        value = value.remove(value.length()-count, count);

        count = 0;
        for(int i=value.length()-1; i>=0; i--){
            if(QString(value[i]).compare(".") == 0){
                break;
            }else{
                count++;
            }
        }
        QString round = QString().setNum(value.toDouble(), 'f', count);
        value = round;
    }
    return value;
}

double Sistema::get_igv()
{
    if(igv == 0.0){
        QSqlQuery query;
        QString str_query = "SELECT num FROM igv WHERE moneda_id = " + QString().setNum(moneda_items::SOL) + " ORDER BY fecha DESC LIMIT 1";
        qDebug()<<str_query<<endl;
        if(query.exec(str_query)){
            query.next();
            igv = query.value(0).toDouble();
        }
    }
    return igv;
}
double Sistema::get_dolar()
{
    if(dolar == 0.0){
        QSqlQuery query;
        QString str_query = "SELECT num FROM tipo_cambio WHERE moneda_id = " + QString().setNum(moneda_items::DOLAR) + " ORDER BY fecha DESC LIMIT 1";
        qDebug()<<str_query<<endl;
        if(query.exec(str_query)){
            query.next();
            dolar = query.value(0).toDouble();
        }
    }
    return dolar;
}

double Sistema::get_igv(const QDate& date)
{
    double igv = 0.0;
    QSqlQuery query;
    QString str_query = "SELECT num FROM igv WHERE moneda_id = " + QString().setNum(moneda_items::SOL) + " AND fecha <= '"+date.toString("yyyy-MM-dd")+"'";
    str_query += " ORDER BY fecha DESC LIMIT 1";
    qDebug()<<str_query<<endl;
    if(query.exec(str_query)){
        query.next();
        igv = query.value(0).toDouble();
    }
    return igv;
}
double Sistema::get_dolar(const QDate& date)
{
    double dolar = 0.0;
    QSqlQuery query;
    QString str_query = "SELECT num FROM tipo_cambio WHERE moneda_id = "+QString().setNum(moneda_items::DOLAR)+" AND fecha <= '"+date.toString("yyyy-MM-dd")+"'";
    str_query += " ORDER BY fecha DESC LIMIT 1";
    qDebug()<<str_query<<endl;
    if(query.exec(str_query)){
        query.next();
        dolar = query.value(0).toDouble();
    }
    return dolar;
}
QSqlQuery Sistema::get_id_igv(const QDate &date)
{
    QSqlQuery query;
    QString str_query = "SELECT id, num FROM igv WHERE moneda_id = " + QString().setNum(moneda_items::SOL) + " AND fecha <= '"+date.toString("yyyy-MM-dd")+"'";
    str_query += " ORDER BY fecha DESC LIMIT 1";
    qDebug()<<str_query<<endl;
    if(query.exec(str_query)){

    }
    return query;
}
QSqlQuery Sistema::get_id_dolar(const QDate &date)
{
    QSqlQuery query;
    QString str_query = "SELECT id, num FROM tipo_cambio WHERE moneda_id = "+QString().setNum(moneda_items::DOLAR)+" AND fecha <= '"+date.toString("yyyy-MM-dd")+"'";
    str_query += " ORDER BY fecha DESC LIMIT 1";
    qDebug()<<str_query<<endl;
    if(query.exec(str_query)){

    }
    return query;
}
void Sistema::cpy_q_to_tb(QSqlQuery &q, QTableWidget *tb)
{
    if(!q.isSelect()) {
        qDebug()<<"don't select"<<endl;
        return;
    }else{
        if(q.size()== 0){
            qDebug()<<"no hay datos"<<endl;
            return;
        }else{
        }
    }

    int count= q.record().count(), i= tb->rowCount();

    tb->setColumnCount(count);
    //tb->setHorizontalHeaderLabels(labels);
    tb->setRowCount(i+q.size());

    while(q.next()){
        int count= q.record().count();
        for(int ii= 0; ii<count; ii++){
            QTableWidgetItem* item= new QTableWidgetItem(q.value(ii).toString());
            int flags= Qt::ItemIsDropEnabled|Qt::ItemIsDragEnabled|Qt::ItemIsEnabled| Qt::ItemIsSelectable;
            item->setFlags((Qt::ItemFlags)flags);
            tb->setItem(i, ii, item);;
        }
        i++;

    }

    q.seek(0);
    int columnCount= q.record().count();
    for(int j= 0; j< columnCount; j++){
        QTableWidgetItem* item= new QTableWidgetItem(q.record().fieldName(j));
        tb->setHorizontalHeaderItem(j, item);
    }
}

QString Sistema::parse_query(QVector<QVector<QString> >& vv)
{
    QString str= "";
        if(vv.size()> 0){
            int size= vv[0].size();
            str+= "(";
            str+= ""+vv[0][0]+"";
            for(int j= 1; j< size; j++){
                str+= ", "+ vv[0][j]+"";
            }
            str+= ")";

            for(int i= 1; i< vv.size(); i++){
                int size= vv[i].size();
                str+= ", (";
                str+= ""+vv[i][0]+"";
                for(int j= 1; j< size; j++){
                    str+= ", "+ vv[i][j]+"";
                }
                str+= ")";
            }
        }

    return str;
}

void Sistema::moveRow(QTableWidget *tw, int source, int destiny)
{
    if(!(destiny >= 0 && destiny < tw->rowCount())){
        return;
    }

    for(int i=0; i<tw->columnCount(); i++){
        QTableWidgetItem* item_source= tw->takeItem(source, i);
        QTableWidgetItem* item_destiny= tw->takeItem(destiny, i);
        tw->setItem(source, i, item_destiny);
        tw->setItem(destiny, i, item_source);
    }
    tw->selectRow(destiny);
}
bool Sistema::hasChildren(QFrame *f)
{
    int count= 0;
    foreach (QObject *obj, f->children()) {
        qDebug()<<"i"<<endl;
        if(obj->isWidgetType()){
            QWidget* w= (QWidget*)obj;
            if(w&& w->isVisible()){
                count++;
                break;
            }
        }
    }
    if(count>0){
        return true;
    }else{
        return false;
    }
}

double Sistema::round(double num, int decimals)
{
    double m= pow(10, decimals);

    num*= m;
    num+=0.5;
    num=(double)((int)num);
    num/=m;
    return num;
}
QString Sistema::zeros(int n)
{
    QString zeros;
    for(int i = 0; i < n; i++){
        zeros += "0";
    }
    return zeros;
}
QString& Sistema::align_left(QString &str, int lenght)
{
    int spaces = lenght - str.length();
    for(int i = 0; i < spaces; i++){
        str.insert(str.length(), ' ');
    }
    return str;
}
QString& Sistema::justified(QString& str, int lenght)
{
    int spaces = lenght - str.length();
    for(int i = 0; i < spaces; i++){
        str.insert(0, ' ');
    }
    return str;
}
QString& Sistema::epson_just_descripcion(QString& str, int n)
{        
    if(n > str.length()){
        insert_right_spaces(str, n-str.length());
    }else{
        str = str.mid(0, n);
    }
    return str;
}

QString& Sistema::insert_left_spaces(QString& str, int n)
{
    for(int i = 0; i < n; i++){
        str.insert(0, ' ');
    }
    return str;
}
QString& Sistema::insert_right_spaces(QString& str, int n)
{
    for(int i = 0; i < n; i++){
        str.insert(str.length(), ' ');
    }
    return str;
}
void Sistema::insertImage(QDataStream &out, QString fileName)
{
/*
    out << (qint64)0x1D500000;
    out << (qint64)0x1B3300;
    out << (qint64)0x1B2A00FF03;

    for(int i = 0; i < 36; i++){
        out << (quint64) 281474976710656*256*256-1;
    }
    */

    int width, height;
    png_byte color_type;
    png_byte bit_depth;
    png_bytep *row_pointers;

    QString image_dir;
    {
        QString app_dir = QCoreApplication::applicationDirPath();

        QString cmp_release = app_dir.mid(app_dir.length()-8, 9);
        QString str_rel = "";
        //qDebug() << cmp_release << endl;
        if(cmp_release.compare("/release") == 0){
            str_rel = "/release";
            //qDebug() << str_rel << endl;
        }

        app_dir = app_dir.mid(0, app_dir.length() - str_rel.length());
        app_dir.replace("/","\\");
        app_dir = app_dir;

        image_dir = app_dir;
    }
    image_dir += "\\" + fileName;

    FILE *fp = fopen(QString(image_dir).toStdString().c_str(), "rb");

    qDebug()<<image_dir<<endl;
    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(!png) abort();
    qDebug()<<image_dir<<endl;

    png_infop info = png_create_info_struct(png);
    if(!info) abort();
    qDebug()<<image_dir<<endl;

    if(setjmp(png_jmpbuf(png))) abort();
    qDebug()<<"jmpbuf"<<endl;

    png_init_io(png, fp);
    qDebug()<<"print init io"<<endl;

    png_read_info(png, info);
    qDebug()<<"read info"<<endl;


    width      = png_get_image_width(png, info);
    qDebug()<<width<<endl;
    height     = png_get_image_height(png, info);
    qDebug()<<height<<endl;
    color_type = png_get_color_type(png, info);
    bit_depth  = png_get_bit_depth(png, info);

    if(color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png);

    // PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
    if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        png_set_expand_gray_1_2_4_to_8(png);

    if(png_get_valid(png, info, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png);

    // These color_type don't have an alpha channel then fill it with 0xff.
    if(color_type == PNG_COLOR_TYPE_RGB ||
            color_type == PNG_COLOR_TYPE_GRAY ||
            color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

    if(color_type == PNG_COLOR_TYPE_GRAY ||
            color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(png);

    png_read_update_info(png, info);

    row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
    for(int y = 0; y < height; y++) {
        row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png,info));
    }

    png_read_image(png, row_pointers);

    fclose(fp);

    int mask = 0x80;
    int i = 0;
    int temp = 0;



    //out << (qint64)0x1B40;
    //out << (qint64)0x1D500000;
    //out << (qint64)0x1B3300;
    //out << (qint64)0x1B2A00FF03;
    //out << (qint64)0x1D5770FF;
    //out << (qint64)0x1D500000;
    //out << (qint64)0x1B3300;
    //out << (qint64)0x1B2A00FF00;
    //out << (qint64)0x1D7630006000FF03;


    /*
        out << (qint64)0x1D500000;
        out << (qint64)0x1B3300;
        out << (qint64)0x1B2A00FF03;*/

    /*
        for(int i = 0; i < 36; i++){
            out << (quint64) 281474976710656*256*256-1;
        }
        */

    //  Print image dimensions
/*
    QFile fileImage("npedido.png");
    if (!fileImage.open(QIODevice::ReadOnly)) return;
    QByteArray blob = fileImage.readAll();

    QPixmap image;
    image.loadFromData(blob, "PNG");

    qDebug()<<blob.toBase64()<<endl;
    ui->pushButton_imageLogo->setIcon(image);

    return;

    int width = image.width(), height = image.height();
*/
    unsigned short headerX = ( width + 7) / 8; // width in characters
    unsigned short headerY = height; // height in pixels

    qDebug() << ((qint8)(headerX >> 0) & 0xFF);
    qDebug() << ((qint8)(headerY >> 0) & 0xFF);

    //out << (qint64)0x1B40;
    out << ((qint64)0x1D << 4*14)
           +((qint64)0x76 << 4*12)
           +((qint64)0x30 << 4*10)
           +((qint64)0x01 << 4*8)
           +((qint64)((headerX >> 0) & 0xFF) << 4*6)
           +((qint64)((headerX >> 8) & 0xFF) << 4*4)
           +((qint64)((headerY >> 0) & 0xFF) << 4*2)
           +((qint64)((headerY >> 8) & 0xFF) << 4*0);
    QByteArray array;


    for(int i = 0; i < 4; i++){
        out << (quint64) 281474976710656*256;
    }

    //quint64 sum = 0;
    for (int y = 0; y < height; y++) {
        png_bytep row = row_pointers[y];
        for (int x = 0; x < width; x++) {
            png_bytep px = &(row[x * 4]);
            int value = px[0] + px[1] + px[2];
            if (px[3] < 128) {
                value = 255;
            } else if (value > 384) {
                value = 255;
            } else {
                value = 0;
            }
            value = (value << 8) | value;

            if (value == 0) {
                temp |= mask;
            }

            mask = mask >> 1;

            i++;
            if (i == 8) {
                array.append(temp);
                mask = 0x80;
                i = 0;
                temp = 0;
            }
        }
        if(i != 0) {
        array.append(temp);
        i = 0;
        }
    }



    //out<<(quint64)sum;
    out<<array;
}
QString& Sistema::rightText(QString& text, int n)
{
    int maxLenght = 0;
    /*
    for(int i = 0; i < v.length(); i++){
        if(v[i].length() > maxLenght){
            maxLenght = v[i].length();
        }
    }
    */
    //maxLenght /= 2;
    maxLenght = n;

    int spaces = maxLenght - text.length();

    insert_left_spaces(text, spaces);

    return text;
}
QString& Sistema::centerText(QString& text, int n)
{
    int maxLenght = 0;
    /*
    for(int i = 0; i < v.length(); i++){
        if(v[i].length() > maxLenght){
            maxLenght = v[i].length();
        }
    }
    */
    //maxLenght /= 2;
    maxLenght = n;

        int unit = (maxLenght - text.length()) % 2;
        int spaces = (maxLenght - text.length())/2;
        insert_left_spaces(text, spaces);
        insert_right_spaces(text, spaces + unit);

    return text;
}

QVector<QString>& Sistema::centerTexts(QVector<QString>& v, int n)
{
    int maxLenght = 0;
    /*
    for(int i = 0; i < v.length(); i++){
        if(v[i].length() > maxLenght){
            maxLenght = v[i].length();
        }
    }
    */
    //maxLenght /= 2;
    maxLenght = n;
    for(int i = 0; i < v.length(); i++){
        int unit = (maxLenght - v[i].length()) % 2;
        int spaces = (maxLenght - v[i].length())/2;
        insert_left_spaces(v[i], spaces);
        insert_right_spaces(v[i], spaces + unit);
    }

    return v;
}
QVector<QString>& Sistema::rightTexts(QVector<QString>& v, int n)
{
    int maxLenght = 0;
    /*
    for(int i = 0; i < v.length(); i++){
        if(v[i].length() > maxLenght){
            maxLenght = v[i].length();
        }
    }
    */
    //maxLenght /= 2;
    maxLenght = n;
    for(int i = 0; i < v.length(); i++){
        int spaces = maxLenght - v[i].length();
        insert_left_spaces(v[i], spaces);
    }

    return v;
}
void Sistema::fixString(QLineEdit *le)
{   
	int left;
	for (left = 0; left < le->text().length(); left++) {
		if (le->text()[left] != ' ') {
			break;
		}

	}
	le->setText(le->text().mid(left));

    int right;
    for(right = le->text().length()-1; right >=0 ; right--){
        if(le->text()[right]!= ' '){
            break;
        }

    }
    le->setText(le->text().mid(0, right +1));
}
QString& Sistema::fixString(QString& str)
{
	int left;
	for (left = 0; left < str.length(); left++) {
		if (str[left] != ' ') {
			break;
		}

	}
	str = str.mid(left);

	int right;
	for (right = str.length() - 1; right >= 0; right--) {
		if (str[right] != ' ') {
			break;
		}

	}
	str = str.mid(0, right + 1);

    return str;
}

void Sistema::windowCentered(QWidget *w)
{
    QRect rect= QApplication::desktop()->screenGeometry();
    qDebug()<<"x: "<<rect.x()<<endl;
    qDebug()<<"y: "<<rect.y()<<endl;
    qDebug()<<"width: "<<rect.width()<<endl;
    qDebug()<<"height: "<<rect.height()<<endl;
    w->resize(rect.width(), rect.height());
    w->move(0, 0);
}

void Sistema::table_resize_to_contents(QWidget *p, QTableWidget *tb, int visible_rows)
{
    tb->resizeColumnsToContents();
    tb->resizeRowsToContents();

    int columnCount= tb->columnCount();
    int rowCount= visible_rows;

    int new_w=0, new_h=0;
    for(int i= 0; i< columnCount; i++){
        new_w+= tb->columnWidth(i);
    }
    for(int i= 0; i< rowCount; i++){
        new_h+= tb->rowHeight(i);
    }

    int vs_w;

    vs_w= tb->verticalScrollBar()->sizeHint().width();

    int hs_h;
    hs_h= tb->horizontalScrollBar()->sizeHint().height();


    int vh_w= tb->verticalHeader()->sizeHint().width();
    int hh_h= tb->horizontalHeader()->sizeHint().height();

    int width=  (vs_w+vh_w+new_w)+10;
    int height= (hs_h+hh_h+new_h);

    tb->setMinimumWidth(width);    
    tb->setMaximumWidth(width);

    tb->setMinimumHeight(height);
    tb->setMaximumHeight(height);

    tb->setMaximumWidth(99999999);
    tb->setMaximumHeight(99999999);
}

void Sistema::form_disable_widgets(QVector<QWidget*> v)
{
    for(int i = 0; i < v.size(); i++){
        QWidget* widget = v[i];
        foreach(QObject* w, widget->children()){
            if(w && w->isWidgetType()){                
                if(typeid(*w) == typeid(QTableWidget)){
                    for(int i = 0; i < ((QTableWidget*)w)->rowCount(); i++){
                        for(int ii= 0; ii < ((QTableWidget*)w)->columnCount(); ii++){
                            int flags= Qt::ItemIsDropEnabled|Qt::ItemIsDragEnabled|Qt::ItemIsEnabled|Qt::ItemIsSelectable;
                            ((QTableWidget*)w)->item(i, ii)->setFlags((Qt::ItemFlags)flags);
                        }
                    }
                }

                if(typeid(*w) == typeid(QPushButton)){
                    ((QPushButton*)w)->setDisabled(true);
                }

                if(typeid(*w) == typeid(QComboBox)){
                    ((QComboBox*)w)->setDisabled(true);
                }

                if(typeid(*w) == typeid(QLineEdit)){
                    ((QLineEdit*)w)->setReadOnly(true);
                }

                if(typeid(*w) == typeid(QDateTimeEdit)){
                    ((QDateTimeEdit*)w)->setReadOnly(true);
                }

                if(typeid(*w) == typeid(QDoubleSpinBox)){
                    ((QDoubleSpinBox*)w)->setReadOnly(true);
                }
            }
        }
    }
}

QString Sistema::get_mes(int mm)
{
    QString month;
    if(mm==1)month = "Enero";
    if(mm==2)month = "Febrero";
    if(mm==3)month = "Marzo";
    if(mm==4)month = "Abril";
    if(mm==5)month = "Mayo";
    if(mm==6)month = "Junio";
    if(mm==7)month = "Julio";
    if(mm==8)month = "Agosto";
    if(mm==9)month = "Septiembre";
    if(mm==10)month = "Octubre";
    if(mm==11)month = "Noviembre";
    if(mm==12)month = "Diciembre";

    return month;
}

QString Sistema::replace_quotes(const QString &str)
{
    QString s= str;
    s = s.replace(QString("\""), QString("\\\\\\\""));
    s = s.replace(QString("'"), QString("\\\\'"));
    //qDebug()<<s<<endl;
    return s;
}
QString Sistema::replace_quotes_simple(const QString &str)
{
    QString s= str;
    s = s.replace(QString("\""), QString("\\\\\""));
    s = s.replace(QString("'"), QString("\\\\\\'"));
    //qDebug()<<s<<endl;
    return s;
}
void Sistema::set_stylesheet(const QVector<QWidget*>& v, int pos)
{
    for(int i= 0; i<v.size(); i++)
        v[i]->setStyleSheet(ssheet_no_select);
    v[pos]->setStyleSheet(ssheet_select);
}
void Sistema::clear_center_w(QWidget* w)
{    
    QWidget* gridLayout = w->parentWidget();
    if(gridLayout){
        QWidget* scrollArea = gridLayout->parentWidget();
        QWidget* centralWidget = scrollArea->parentWidget();
        QMainWindow* mainw = (QMainWindow*)centralWidget->parentWidget();
        centralWidget->hide();
        centralWidget->setParent(0);
        mainw->setCentralWidget(new QWidget());
    }else{

    }
}

QWidget* Sistema::change_center_w(QWidget* old_w, QWidget* new_w, bool isClose)
{
    curr_widget = new_w;

    if(!old_w){        
        return 0;
    }

    QWidget* temp = old_w->parentWidget();

    if(temp){        
        //QMainWindow* mw = (QMainWindow*)temp->parentWidget()->parentWidget()->parentWidget();
        //mw->statusBar()->addWidget(new QLabel("HOLA MAINWINDOW"));
    }

    QGridLayout* gridLayout = 0;
    foreach(QObject* obj, temp->children()){
        if(obj){
            QWidget* w = (QWidget*)obj;
            if(typeid(*w) == typeid(QGridLayout)){
                gridLayout = (QGridLayout*)w;
                //qDebug()<<typeid(*w).name()<<endl;
            }else{
                //qDebug()<<typeid(*w).name()<<endl;
            }
        }
    }

	if (old_w->isVisible()) old_w->hide();

    bool b = true;
    if(isClose)
        b = old_w->close();
    else
        old_w->hide();

	if (!b) return NULL;

    gridLayout->removeWidget(old_w);
    gridLayout->addWidget(new_w, 0, 1);

	if (old_w->isHidden()) new_w->show();

    return new_w;
}
QWidget* Sistema::set_center_w(QWidget* w, QString styleSheet)
{
    curr_widget = w;

    QWidget* widget= new QWidget;

    QGridLayout* grid= new QGridLayout(widget);
    grid->setHorizontalSpacing(0);
    grid->setVerticalSpacing(0);

        QString ss = styleSheet;
        QWidget* widget_00 = new QWidget;
        widget_00->setStyleSheet(ss);
        QSpacerItem* horizontalSpacer= new QSpacerItem(0, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
        QHBoxLayout* horizontalLayout= new QHBoxLayout();
        horizontalLayout->addItem(horizontalSpacer);
        grid->addItem(horizontalLayout,0,0);

        grid->addWidget(widget_00,0,0);

        grid->addWidget(w,0,1);

        QWidget* widget_02 = new QWidget;
        widget_02->setStyleSheet(ss);
        QSpacerItem* horizontalSpacer_1= new QSpacerItem(0, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
        QHBoxLayout* horizontalLayout_1= new QHBoxLayout();
        horizontalLayout_1->addItem(horizontalSpacer_1);
        grid->addItem(horizontalLayout_1,0,2);

        grid->addWidget(widget_02,0,2);

        QWidget* widget_10 = new QWidget;
        widget_10->setStyleSheet(ss);
        QSpacerItem* verticalSpacer= new QSpacerItem(20, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
        QVBoxLayout* verticalLayout= new QVBoxLayout();
        verticalLayout->addItem(verticalSpacer);
        grid->addItem(verticalLayout,1,0);

        grid->addWidget(widget_10,1,0);

        QWidget* widget_11 = new QWidget;
        widget_11->setStyleSheet(ss);
        QSpacerItem* verticalSpacer_1= new QSpacerItem(20, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
        QVBoxLayout* verticalLayout_1= new QVBoxLayout();
        verticalLayout_1->addItem(verticalSpacer_1);
        grid->addItem(verticalLayout_1,1,1);

        grid->addWidget(widget_11,1,1);

        QWidget* widget_12 = new QWidget;
        widget_12->setStyleSheet(ss);
        QSpacerItem* verticalSpacer_2= new QSpacerItem(20, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
        QVBoxLayout* verticalLayout_2= new QVBoxLayout();
        verticalLayout_2->addItem(verticalSpacer_2);
        grid->addItem(verticalLayout_2,1,2);

        grid->addWidget(widget_12,1,2);

    QScrollArea* area = new QScrollArea;
    area->setWidget(widget);
    area->setWidgetResizable(true);

    if(widgets.count() <= 5){
        widgets.push_back(area);
    }else{
        widgets.removeAt(0);
    }

    return area;
}
QMainWindow* Sistema::get_mainw(QWidget *w)
{
    QWidget* widget = w->parentWidget();
    QWidget* scrollArea = widget->parentWidget();
    QWidget* centralWidget = scrollArea->parentWidget();

    return (QMainWindow*)centralWidget->parentWidget();
}
QWidget* Sistema::get_centralw(QWidget *w)
{
    QWidget* widget = w->parentWidget();
    QWidget* scrollArea = widget->parentWidget();
    QWidget* centralWidget = scrollArea->parentWidget();

    return centralWidget;
}
QWidget* Sistema::get_centralw()
{
    QWidget* w = curr_widget;
    if(!w)return 0;

    QWidget* widget = w->parentWidget();
    QWidget* scrollArea = widget->parentWidget();
    QWidget* centralWidget = scrollArea->parentWidget();

    return centralWidget;
}
QString Sistema::build_argument(const QString& cmp_column, const QString &arg)
{
    QStringList list= arg.split(" ", QString::SkipEmptyParts);
    QString r;
    if(list.count()>0)
    {
        list[0] = list[0].replace("\"", "\\\"");
        list[0] = list[0].replace("'", "\'");
        r= " AND "+cmp_column+" LIKE '%"+list[0]+"%'";
        for(int i= 1; i< list.count(); i++){
            list[i] = list[i].replace("\"", "\\\"");
            list[i] = list[i].replace("'", "\'");
            r+= " AND "+cmp_column+" LIKE '%"+list[i]+"%'";
        }
    }
    return r;
}
QString Sistema::multiple_query(QString& query)
{    
    QString r = QString("CALL multiple_query(")+'"'+query+'"'+QString(")");
    //cout<<r.toStdString()<<endl;
    query = QString("CALL multiple_query(")+'"'+query+'"'+QString(")");

    return r;
}

bool Sistema::update_cambio_dolar(QWidget* sender, const QDate& date, double value)
{
    QSqlQuery query_config;
    QString str_query = "(SELECT EXISTS (SELECT '' FROM tipo_cambio WHERE fecha = '"+date.toString("yyyy-MM-dd")+"' AND moneda_item_id = "+QString().setNum(items::DOLAR)+"))"
                        " UNION ALL (SELECT id FROM tipo_cambio WHERE fecha = '"+date.toString("yyyy-MM-dd")+"' AND moneda_item_id = "+QString().setNum(items::DOLAR)+")";

    if(query_config.exec(str_query)){
        query_config.next();
    }else{
        QMessageBox::critical(sender, "Error", "Ocurrio un problema inesperado.", "Ok");
        return false;
    }

    QString num;
    num.setNum(value);

    QString str_fecha = date.toString("yyyy-MM-dd");

    query_config.seek(0);
    if(query_config.value(0).toString() == "0"){
        str_query = "INSERT INTO tipo_cambio(moneda_item_id, num, fecha)VALUES";

        str_query += QString()+"("+QString().setNum(items::DOLAR)+", "+num+", '"+str_fecha+"')&&END_QUERY&&";
    }else{
        str_query = "INSERT INTO tipo_cambio(id, moneda_item_id, num, fecha)VALUES";

        query_config.seek(1);
        str_query += QString()+"("+query_config.value(0).toString()+", "+QString().setNum(items::DOLAR)+", "+num+", '"+str_fecha+"')";
        str_query += " ON DUPLICATE KEY UPDATE id = VALUES(id), moneda_item_id=VALUES(moneda_item_id), num=VALUES(num), fecha=VALUES(fecha)&&END_QUERY&&";
    }

    multiple_query(str_query);

    qDebug()<<str_query<<endl;
    if(query_config.exec(str_query)){
        QMessageBox::information(sender, "Información", "Se guardo con éxito.", "Ok");
        return true;
    }else{
        QMessageBox::critical(sender, "Error", "Ocurrio un problema inesperado.", "Ok");
        return false;
    }
}
bool Sistema::update_igv(QWidget* sender, const QDate& date, double value)
{
    QSqlQuery query_config;
    QString str_query = "(SELECT EXISTS (SELECT '' FROM igv LIMIT 1))"
                        " UNION ALL (SELECT id FROM igv WHERE fecha = '"+date.toString("yyyy-MM-dd")+"')";

    if(query_config.exec(str_query)){
        query_config.next();
    }else{
        QMessageBox::critical(sender, "Error", "Ocurrio un problema inesperado.", "Ok");
        return false;
    }

    QString num;
    num.setNum(value);

    QString str_fecha = date.toString("yyyy-MM-dd");

    if(query_config.value(0).toString() == "0"){
        str_query = "INSERT INTO igv(num, fecha)VALUES";

        str_query += QString()+"("+num+", '"+str_fecha+"')&&END_QUERY&&";

    }else{
        str_query = "INSERT INTO igv(id, num, fecha)VALUES";

        str_query += QString()+"("+query_config.value(0).toString()+", "+num+", '"+str_fecha+"')";
        str_query += " ON DUPLICATE KEY UPDATE id = VALUES(id), num=VALUES(num), fecha=VALUES(fecha)&&END_QUERY&&";
    }

    multiple_query(str_query);

    qDebug()<<str_query<<endl;
    if(query_config.exec(str_query)){
        QMessageBox::information(sender, "Información", "Se guardo con éxito.", "Ok");
        return true;
    }else{
        QMessageBox::critical(sender, "Error", "Ocurrio un problema inesperado.", "Ok");
        return false;
    }
}

void Sistema::table_button_up(QTableWidget *tableWidget)
{
    QTableWidgetItem* item= tableWidget->currentItem();
    if(item){
        int column = item->column();
        int row_source= tableWidget->currentRow();
        int row_destiny= tableWidget->currentRow()-1;

        if(row_source == 0){
            tableWidget->setCurrentItem(tableWidget->item(row_source, column));
            tableWidget->item(row_source, column)->setSelected(true);
            tableWidget->setFocus(Qt::TabFocusReason);
            return;
        }

        for(int i=0; i<tableWidget->columnCount(); i++){
            QTableWidgetItem* item_source= tableWidget->takeItem(row_source, i);
            QTableWidgetItem* item_destiny= tableWidget->takeItem(row_destiny, i);
            tableWidget->setItem(row_source, i, item_destiny);
            tableWidget->setItem(row_destiny, i, item_source);
        }
        tableWidget->setCurrentItem(tableWidget->item(row_destiny, column));
        tableWidget->item(row_destiny, column)->setSelected(true);
        tableWidget->setFocus(Qt::TabFocusReason);
    }
}
void Sistema::table_button_down(QTableWidget *tableWidget)
{
    QTableWidgetItem* item = tableWidget->currentItem();
    if(item){
        int column = item->column();
        int row_source = tableWidget->currentRow();
        int row_destiny = tableWidget->currentRow()+1;

        if(row_source == tableWidget->rowCount()-1){
            tableWidget->setCurrentItem(tableWidget->item(row_source, column));
            tableWidget->item(row_source, column)->setSelected(true);
            tableWidget->setFocus(Qt::TabFocusReason);
            return;
        }

        for(int i=0; i<tableWidget->columnCount(); i++){
            QTableWidgetItem* item_source = tableWidget->takeItem(row_source, i);
            QTableWidgetItem* item_destiny = tableWidget->takeItem(row_destiny, i);
            tableWidget->setItem(row_source, i, item_destiny);
            tableWidget->setItem(row_destiny, i, item_source);
        }
        tableWidget->setCurrentItem(tableWidget->item(row_destiny, column));
        tableWidget->item(row_destiny, column)->setSelected(true);
        tableWidget->setFocus(Qt::TabFocusReason);
    }
}
void Sistema::table_button_borrar(QTableWidget *tableWidget)
{
    QTableWidgetItem* item = tableWidget->currentItem();
    if(!item)return;
    int row = item->row();
    int column = item->column();
    tableWidget->removeRow(row);
    int rowCount = tableWidget->rowCount();

    if(row >= 0){
        if(row > 0){
            tableWidget->setCurrentItem(tableWidget->item(row-1, column));
            tableWidget->item(row-1, column)->setSelected(true);
            tableWidget->setFocus(Qt::TabFocusReason);
        }
    }
}
QString Sistema::get_tipo_documento(int tipo)
{
    QString str_tipo;
    /*
    if(tipo == tipo_documento::REGISTRO_SIN_DOCUMENTO)str_tipo = STR_REG_SIN_DOC;
    if(tipo == tipo_documento::BOLETA)str_tipo = STR_BOLETA;
    if(tipo == tipo_documento::FACTURA)str_tipo = STR_FACTURA;
    if(tipo == tipo_documento::NOTA_PEDIDO)str_tipo = STR_NOTA_DE_PEDIDO;
    if(tipo == tipo_documento::GUIA_REMISION_REMITENTE)str_tipo = STR_GUIA;
    if(tipo == tipo_documento::COTIZACION)str_tipo = STR_COTIZACION;
    if(tipo == tipo_documento::NOTA_CREDITO)str_tipo = STR_NOTA_DE_CREDITO;
    if(tipo == tipo_documento::NOTA_DEBITO)str_tipo = STR_NOTA_DE_DEBITO;
    if(tipo == tipo_documento::FLETE)str_tipo = STR_FLETE;
    if(tipo == tipo_documento::ORDEN)str_tipo = STR_ORDEN;
    if(tipo == tipo_documento::SALDO)str_tipo = STR_SALDO;
    */

    return str_tipo;
}

int Sistema::get_tipo_documento(const QString str_tipo)
{
    int tipo;
    /*
    if(str_tipo.compare(STR_REG_SIN_DOC) == 0)tipo = tipo_documento::REGISTRO_SIN_DOCUMENTO;
    if(str_tipo.compare(STR_BOLETA) == 0)tipo = tipo_documento::BOLETA;
    if(str_tipo.compare(STR_FACTURA) == 0)tipo = tipo_documento::FACTURA;
    if(str_tipo.compare(STR_NOTA_DE_PEDIDO) == 0)tipo = tipo_documento::NOTA_PEDIDO;
    if(str_tipo.compare(STR_GUIA) == 0)tipo = tipo_documento::GUIA_REMISION_REMITENTE;
    if(str_tipo.compare(STR_COTIZACION) == 0)tipo = tipo_documento::COTIZACION;
    if(str_tipo.compare(STR_NOTA_DE_CREDITO) == 0)tipo = tipo_documento::NOTA_CREDITO;
    if(str_tipo.compare(STR_NOTA_DE_DEBITO) == 0)tipo = tipo_documento::NOTA_DEBITO;
    if(str_tipo.compare(STR_FLETE) == 0)tipo = tipo_documento::FLETE;
    if(str_tipo.compare(STR_ORDEN) == 0)tipo = tipo_documento::ORDEN;
    if(str_tipo.compare(STR_SALDO) == 0)tipo = tipo_documento::SALDO;
*/
    return tipo;
}
/**
 * Helper function. Removes all layout items within the given @a layout
 * which either span the given @a row or @a column. If @a deleteWidgets
 * is true, all concerned child widgets become not only removed from the
 * layout, but also deleted.
 */
void Sistema::remove(QGridLayout *layout, int row, int column, bool deleteWidgets) {
    // We avoid usage of QGridLayout::itemAtPosition() here to improve performance.
    for (int i = layout->count() - 1; i >= 0; i--) {
        int r, c, rs, cs;
        layout->getItemPosition(i, &r, &c, &rs, &cs);
        if ((r <= row && r + rs - 1 >= row) || (c <= column && c + cs - 1 >= column)) {
            // This layout item is subject to deletion.
            QLayoutItem *item = layout->takeAt(i);
            if (deleteWidgets) {
                deleteChildWidgets(item);
            }
            delete item;
        }
    }
}
void Sistema::set_pushButton_css(QGridLayout *layout, QString text
                                 , int row, int column, QFont font, QString color, QString bg_color)
{
    QLayoutItem *item = layout->itemAtPosition(row, column);
    if(!item){
        return;
    }
    QPushButton* w = (QPushButton*)item->widget();
    if(!w){
        return;
    }
    w->setText(text);
    w->setFont(font);
    w->setStyleSheet("QPushButton{ color: " + color + "; background-color: " + bg_color + "; }");
}
void Sistema::set_toolButton_css(QGridLayout *layout, QString text
                                 , int row, int column, QFont font, QString color, QString bg_color)
{
    QLayoutItem *item = layout->itemAtPosition(row, column);
    if(!item){
        return;
    }
    QToolButton* w = (QToolButton*)item->widget();
    if(!w){
        return;
    }
    w->setText(text);
    w->setFont(font);
    w->setStyleSheet("QToolButton{ color: " + color + "; background-color: " + bg_color + "; }");
}
QToolButton* Sistema::return_tb(QGridLayout *layout
                                 , int row, int column)
{
    QLayoutItem *item = layout->itemAtPosition(row, column);
    if(!item){
        return 0;
    }
    QToolButton* w = (QToolButton*)item->widget();
    if(!w){
        return w;
    }
    return 0;
}
QString Sistema::extract_background_color(QString styleSheet)
{
    int from = styleSheet.indexOf("background-color");
    int to = styleSheet.indexOf(";", from);

    return styleSheet.mid(from+17, to-(from+17)).simplified();
}
QString Sistema::extract_color(QString styleSheet)
{
    int from = styleSheet.indexOf("color");
    int to = styleSheet.indexOf(";", from);

    QString token = styleSheet.mid(from-1, 1);
    if(token.compare("-") == 0){
        from = styleSheet.indexOf("color", from + 1);
        to = styleSheet.indexOf(";", from);
    }

    return styleSheet.mid(from+6, to-(from+6)).simplified();
}
/**
 * Helper function. Deletes all child widgets of the given layout @a item.
 */
void Sistema::deleteChildWidgets(QLayoutItem *item) {
    if (item->layout()) {
        // Process all child items recursively.
        for (int i = 0; i < item->layout()->count(); i++) {
            deleteChildWidgets(item->layout()->itemAt(i));
        }
    }
    delete item->widget();
}

/**
 * Removes all layout items on the given @a row from the given grid
 * @a layout. If @a deleteWidgets is true, all concerned child widgets
 * become not only removed from the layout, but also deleted. Note that
 * this function doesn't actually remove the row itself from the grid
 * layout, as this isn't possible (i.e. the rowCount() and row indices
 * will stay the same after this function has been called).
 */
void Sistema::removeRow(QGridLayout *layout, int row, bool deleteWidgets) {
    remove(layout, row, -1, deleteWidgets);
    layout->setRowMinimumHeight(row, 0);
    layout->setRowStretch(row, 0);
}

/**
 * Removes all layout items on the given @a column from the given grid
 * @a layout. If @a deleteWidgets is true, all concerned child widgets
 * become not only removed from the layout, but also deleted. Note that
 * this function doesn't actually remove the column itself from the grid
 * layout, as this isn't possible (i.e. the columnCount() and column
 * indices will stay the same after this function has been called).
 */
void Sistema::removeColumn(QGridLayout *layout, int column, bool deleteWidgets) {
    remove(layout, -1, column, deleteWidgets);
    layout->setColumnMinimumWidth(column, 0);
    layout->setColumnStretch(column, 0);
}
bool Sistema::start_transaction()
{
    QString str_query = "START TRANSACTION";
    QSqlQuery query;

    qDebug()<<str_query<<endl;
    if(query.exec(str_query)){
        return true;
    }else{
        return false;
    }
}
bool Sistema::commit()
{
    QString str_query = "COMMIT";
    QSqlQuery query;

    qDebug()<<str_query<<endl;
    if(query.exec(str_query)){
        return true;
    }else{
        return false;
    }
}
bool Sistema::rollback()
{
    QString str_query = "ROLLBACK";
    QSqlQuery query;

    qDebug()<<str_query<<endl;
    if(query.exec(str_query)){
        return true;
    }else{
        return false;
    }
}
bool Sistema::confirmar_admin(QString nombre, QString pass)
{
    QString str_query = "";
    QSqlQuery query;
    str_query += "SELECT 1 FROM persona";
    str_query += " JOIN persona_has_usuario per_h_u ON per_h_u.persona_cod = persona.cod";
    str_query += " JOIN usuario ON per_h_u.usuario_id = usuario.id";
    str_query += " WHERE (persona.tipo_item_nombre = 'Administrador'";
    str_query += " OR persona.tipo_item_nombre = 'Master')";
    //str_query += " AND usuario.nombre = '" + nombre + "'";
    str_query += " AND usuario.pass = '" + pass + "'";

    qDebug()<<str_query<<endl;
    if(query.exec(str_query)){
        if(query.next()){
            qDebug()<<query.value(0).toInt()<<endl;
            bool r = false;
            if(query.value(0).toInt() == 1)
                r = true;
            return r;
        }else{
            return false;
        }
    }else{
        return false;
    }
}
bool Sistema::confirmar_my_pass(QString nombre, QString pass)
{
    QString str_query = "";
    QSqlQuery query;
    str_query += "SELECT 1 FROM persona";
    str_query += " JOIN persona_has_usuario per_h_u ON per_h_u.persona_cod = persona.cod";
    str_query += " JOIN usuario ON per_h_u.usuario_id = usuario.id";
    str_query += " WHERE usuario.nombre = '"+nombre+"'";
    str_query += " AND usuario.pass = '" + pass + "'";

    qDebug()<<str_query<<endl;
    if(query.exec(str_query)){
        if(query.next()){
            qDebug()<<query.value(0).toInt()<<endl;
            bool r = false;
            if(query.value(0).toInt() == 1)
                r = true;
            return r;
        }else{
            return false;
        }
    }else{
        return false;
    }
}

void Sistema::star_print_text(QDataStream& out, QString str)
{
    for(int i = 0; i < str.length(); i++){
        out << str[i];
    }
}
void Sistema::star_set_font_7x9_77(QDataStream& out)
{
    out << 27 << 77 << endl;
}
void Sistema::star_set_font_5x9_80(QDataStream& out)
{
    out << 27 << 80 << endl;
}
void Sistema::star_set_font_5x9_58(QDataStream& out)
{
    out << 27 << 58 << endl;
}
void Sistema::star_set_space(QDataStream& out, int n)
{
    out << 27 << 32 << n << endl;
}
void Sistema::star_line_feed(QDataStream& out)
{
    out << 10;
}
void Sistema::star_carriage_return(QDataStream& out)
{
    out << 13;
}
void Sistema::star_line_feed_n(QDataStream& out, int n)
{
    out << 27 << 97 << n;
}
void Sistema::star_set_line_feed_1_8_inch(QDataStream& out)
{
    out << 27 << 48;
}
void Sistema::star_set_line_feed_7_72_inch(QDataStream& out)
{
    out << 27 << 49;
}
void Sistema::star_set_line_feed_1_12_inch(QDataStream& out)
{
    out << 27 << 122 << 0;
}
void Sistema::star_set_line_feed_1_6_inch(QDataStream& out)
{
    out << 27 << 122 << 1;
}
void Sistema::star_set_page_lines_1_6(QDataStream& out, int n)
{
    out << 27 << 67 << n;
}
void Sistema::star_set_bottom_margin(QDataStream& out, int n)
{
    out << 27 << 78 << n;
}
void Sistema::star_cancel_bottom_margin(QDataStream& out)
{
    out << 27 << 79;
}
void Sistema::star_set_left_margin(QDataStream& out, int n)
{
    out << 27 << 108 << n;
}
void Sistema::star_set_right_margin(QDataStream& out, int n)
{
    out << 27 << 81 << n;
}
void Sistema::star_set_print_region(QDataStream& out, int n)
{
    // N DOTS      SPEC 1 - SPEC 2
    // 0 210 dots  0        0
    // 1 160 dots  0        0
    // 2 200 dots  0        0
    // 3 150 dots  0        0
    // 4 190 dots  -        0
    // 5 180 dots  -        0
    out << 27 << 30 << 65 << n;
}
void Sistema::star_set_horizontal_tab(QDataStream& out, QVector<int>& digits)
{
    if(digits.length() >= 0 && digits.length() < 32){
        return;
    }
    out << 27 << 68;
    for(int i = 0; i < digits.length(); i++){
        out << digits[i] << endl;
    }
}
void Sistema::star_move_horizontal_tab(QDataStream& out)
{
    out << 9 << endl;
}
void Sistema::star_set_alignment(QDataStream& out, int n)
{
    // left   center   right
    // 0      1        2
    // 48     49       50
    out << 27 << 29 << 97 << n;
}
void Sistema::star_paper_cut(QDataStream& out, int n)
{
    // 0 1 2 3
    // 48 49 50 51 SUPPORT TEAR BEAR
    QByteArray array;
    QString str;
    str.toLatin1();
    //out << 29 << 86 << 1;
    out << 27 << 100 << 1;
}
void Sistema::epson_lineFeed(QDataStream& out)
{
    out << 10;
}
void Sistema::epson_linesFeed(QDataStream& out, int n)
{

}
void Sistema::epson_printText(QDataStream& out, QString str)
{
    for(int i = 0; i < str.length(); i++){
        out << str[i];
    }
}
void Sistema::epson_paperCut(QDataStream& out)
{
    out << (qint64)0x001D5601;
}
void Sistema::epson_drawer(QDataStream& out)
{
    out << (qint64)0x1B700025FA;
}
QString Sistema::generate_cbc_value(double val)
{
    QString str_val = QString().setNum(val, ' ', 2);
    int posDot = str_val.indexOf(".");
    str_val = str_val.mid(0, posDot);

    QString str_total;
    qDebug()<<str_val<<endl;
    if(str_val.length() == 1){
        str_total = generate_unidades(QString(str_val[0]));
    }
    if(str_val.length() == 2){
        str_total = generate_decenas(str_val);
    }
    if(str_val.length() == 3){
        str_total = generate_centenas(str_val);
    }
    if(str_val.length() == 4){
        QString str_uDMillar = "";
        if(str_val[0] == "1"){
            str_uDMillar = "UN";
        }else{
            str_uDMillar = generate_unidades(QString(str_val[0]));
        }

        if(generate_centenas(str_val.mid(1,3)).compare("") == 0){
            str_total = str_uDMillar + " MIL";
        }else{
            str_total = str_uDMillar + " MIL "+generate_centenas(str_val.mid(1,3));
        }
    }
    if(str_val.length() == 5){
        if(generate_centenas(str_val.mid(2,3)).compare("") == 0){
            str_total = generate_decenas(str_val.mid(0,2)) + " MIL";
        }else{
            str_total = generate_decenas(str_val.mid(0,2)) + " MIL "+generate_centenas(str_val.mid(2,3));
        }
    }
    if(str_val.length() == 6){
        if(generate_centenas(str_val.mid(3,3)).compare("") == 0){
            str_total = generate_centenas(str_val.mid(0,3)) + " MIL";
        }else{
            str_total = generate_centenas(str_val.mid(0,3)) + " MIL "+generate_centenas(str_val.mid(3,3));
        }
    }
    // TERMINAR
    if(str_val.length() == 7){
        QString str_uDMillon = "";
        if(str_val[0] == "1"){
            str_uDMillon = "UN";
        }else{
            str_uDMillon = generate_unidades(QString(str_val[0]));
        }
        if(generate_centenas(str_val.mid(1,3)).compare("") == 0){
            str_total = str_uDMillon + " MIL";
        }else{
            str_total = str_uDMillon + " MIL "+generate_centenas(str_val.mid(1,3));
        }
    }
    if(str_val.length() == 8){

    }
    if(str_val.length() == 9){

    }
    if(str_val.length() == 10){

    }
    return "SON "+str_total + " Y "+QString().setNum(val, ' ', 2).mid(str_val.length()+1, 2)+"/100 SOLES";
}
QString Sistema::generate_unidades(QString num)
{
    if(num == "0"){
        return "CERO";
    }
    if(num == "1"){
        return "UNO";
    }
    if(num == "2"){
        return "DOS";
    }
    if(num == "3"){
        return "TRES";
    }
    if(num == "4"){
        return "CUATRO";
    }
    if(num == "5"){
        return "CINCO";
    }
    if(num == "6"){
        return "SEIS";
    }
    if(num == "7"){
        return "SIETE";
    }
    if(num == "8"){
        return "OCHO";
    }
    if(num == "9"){
        return "NUEVE";
    }
    return "";
}
QString Sistema::generate_decenas(QString num)
{
    if(num.compare("10") == 0){
        return "DIEZ";
    }
    if(num.compare("11") == 0){
        return "ONCE";
    }
    if(num.compare("12") == 0){
        return "DOCE";
    }
    if(num.compare("13") == 0){
        return "TRECE";
    }
    if(num.compare("14") == 0){
        return "CATORCE";
    }
    if(num.compare("15") == 0){
        return "QUINCE";
    }
    if(num.compare("16") == 0){
        return "DIECISEIS";
    }
    if(num.compare("17") == 0){
        return "DIECISIETE";
    }
    if(num.compare("18") == 0){
        return "DIECIOCHO";
    }
    if(num.compare("19") == 0){
        return "DIECINUEVE";
    }
    if(num[0] == "2"){
        if(num[1] == "0"){
            return "VEINTE";

        }else{
            return "VEINTI"+generate_unidades(QString(num[1]));
        }
    }
    if(num[0] == "3"){
        if(num[1] == "0"){
            return "TREINTA";
        }else{
            return "TREINTA Y "+generate_unidades(QString(num[1]));
        }
    }
    if(num[0] == "4"){
        if(num[1] == "0"){
            return "CUARENTA";
        }else{
            return "CUARENTA Y "+generate_unidades(QString(num[1]));
        }
    }
    if(num[0] == "5"){
        if(num[1] == "0"){
            return "CINCUENTA";
        }else{
            return "CINCUENTA Y "+generate_unidades(QString(num[1]));
        }
    }
    if(num[0] == "6"){
        if(num[1] == "0"){
            return "SESENTA";
        }else{
            return "SESENTA Y "+generate_unidades(QString(num[1]));
        }
    }
    if(num[0] == "7"){
        if(num[1] == "0"){
            return "SETENTA";
        }else{
            return "SETENTA Y "+generate_unidades(QString(num[1]));
        }
    }
    if(num[0] == "8"){
        if(num[1] == "0"){
            return "OCHENTA";
        }else{
            return"OCHENTA Y "+generate_unidades(QString(num[1]));
        }
    }
    if(num[0] == "9"){
        if(num[1] == "0"){
            return "NOVENTA";
        }else{
            return "NOVENTA Y "+generate_unidades(QString(num[1]));
        }
    }
    if(num[0] == "0"){
        if(num[1] == "0")
            return "";
        else
            return generate_unidades(num.mid(1, 1));
    }
    return "";
}
QString Sistema::generate_centenas(QString num)
{
    if(num.compare("100") == 0){
        return "CIEN";
    }else{
        if(num[0] == "0"){
            if(num[1] == "0"){
                if(num[2] == "0"){
                    return "";
                }
                else
                    return generate_unidades(num.mid(2, 1));
            }else{
                return generate_decenas(num.mid(1, 2));
            }
        }

        if(num[0] == "1"){
            return "CIENTO "+generate_decenas(num.mid(1, 2));
        }
        if(num[0] == "2"){
            return "DOSCIENTOS "+generate_decenas(num.mid(1, 2));
        }
        if(num[0] == "3"){
            return "TRESCIENTOS "+generate_decenas(num.mid(1, 2));
        }
        if(num[0] == "4"){
            return "CUATROCIENTOS "+generate_decenas(num.mid(1, 2));
        }
        if(num[0] == "5"){
            return "QUINIENTOS "+generate_decenas(num.mid(1, 2));
        }
        if(num[0] == "6"){
            return "SEISCIENTOS "+generate_decenas(num.mid(1, 2));
        }
        if(num[0] == "7"){
            return "SETECIENTOS "+generate_decenas(num.mid(1, 2));
        }
        if(num[0] == "8"){
            return "OCHOCIENTOS "+generate_decenas(num.mid(1, 2));
        }
        if(num[0] == "9"){
            return "NOVECIENTOS "+generate_decenas(num.mid(1, 2));
        }
    }
    return "";
}
