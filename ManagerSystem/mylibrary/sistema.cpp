#include "sistema.h"

Sistema::Sistema()
{
    dolar = 0.0;
    igv = 0.0;
    // use title for defect for validate events of wizardwidget
    rdy_for_trig = "Form";

    ssheet_select = "color: rgb(255, 255, 255);"
            "background-color: rgb(0, 0, 0);";
    ssheet_no_select = "";

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

void Sistema::print_vv(QVector<QVector<QString> >& vv)
{
    QVector<QString> v;
    for(int i=0; i< vv.size(); i++){
        int size= ((QVector<QString>)vv[i]).size();
        QString str_out;
        for(int j= 0; j< size; j++){
            str_out+= ((QString)vv[i][j])+QString(" & ");
        }
        qDebug()<<str_out;
    }
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

QString Sistema::get_condition(const QString& op
                                              , const QString& exp)
{

    QStringList list= exp.split(" ", QString::SkipEmptyParts);

    QString r;
    if(list.count()>0){
        r+= ""+op+" LIKE '%"+list[0]+"%'";
        for(int i= 1; i< list.count(); i++){
            r+=" AND "+op+" LIKE '%"+list[i]+"%'";
        }
        //r+= "";
    }
    return r;
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
QString Sistema::fixString(QString str)
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

    //if(tb->verticalScrollBar()->sizeHint().width()){
        vs_w= tb->verticalScrollBar()->sizeHint().width();
    //}else{
        //vs_w= 0;
    //}
    int hs_h;
    //if(tb->horizontalScrollBar()->sizeHint().height()){
        hs_h= tb->horizontalScrollBar()->sizeHint().height();
    //}else{
      //  hs_h= 0;
    //}

    int vh_w= tb->verticalHeader()->sizeHint().width();
    int hh_h= tb->horizontalHeader()->sizeHint().height();

    /*
    qDebug()<<"vs_w width: "<<vs_w<<endl;
    qDebug()<<"hs_h height: "<<hs_h<<endl;
    qDebug()<<"vh_w width: "<<vh_w<<endl;
    qDebug()<<"hh_h height: "<<hh_h<<endl;
*/
    //int width=  parent->width()+(vs_w+vh_w+new_w)-old_tb_w+1;
    //int height= parent->height()+(hs_h+hh_h+new_h)-old_tb_h+1;

    int width=  (vs_w+vh_w+new_w)+10;
    int height= (hs_h+hh_h+new_h);

    tb->setMinimumWidth(width);    
    tb->setMaximumWidth(width);

    tb->setMinimumHeight(height);
    tb->setMaximumHeight(height);

    tb->setMaximumWidth(99999999);
    tb->setMaximumHeight(99999999);

    //p->parentWidget()->adjustSize();
    //if(width>= parent->sizeHint().width())
        //parent->resize(width, parent->height());
    //if(height>= parent->sizeHint().height())
        //parent->resize(parent->width(), height);
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
void Sistema::v_normalized_strings_for_cb(QVector<QString> &v)
{
    int n= 0;

    for(int i= 0; i< v.size(); i++){
        if(n < v[i].length())
            n= v[i].length();
    }
    for(int i= 0; i< v.size(); i++){
        for(int j= 0; j< (n - v[i].length()); j++){
            v[i].append(' ');
        }
    }
    for(int i= 0; i< v.size(); i++){
        for(int j= 0; j< 16; j++){
            v[i].append(' ');
        }
    }
}

QString Sistema::replace_quotes(const QString &str)
{
    QString s= str;
    s = s.replace(QString("\""), QString("\\\\\\\""));
    s = s.replace(QString("'"), QString("\\\\'"));
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
        mainw->setCentralWidget(0);
    }else{

    }
}

QWidget* Sistema::change_center_w(QWidget* old_w, QWidget* new_w, bool isClose)
{
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

    return area;
}
QMainWindow* Sistema::get_mainw(QWidget *w)
{
    QWidget* widget = w->parentWidget();
    QWidget* scrollArea = widget->parentWidget();
    QWidget* centralWidget = scrollArea->parentWidget();

    return (QMainWindow*)centralWidget->parentWidget();
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
QString Sistema::query_ultimo_costo(QString producto_id)
{
    QString str_query;
    str_query += "SELECT t.id, t.tipo_documento_id, t.fecha_emision";
    str_query += ", t.cantidad_bruto";
    str_query += ", t.cantidad_bruto - t.cantidad_nc";
    str_query += ", t.precio / t.cantidad_bruto AS precio_bruto";
    str_query += ", t.precio_flete / t.cantidad_bruto AS flete_unit";
    str_query += ", IF((t.cantidad_bruto - t.cantidad_nc) <> 0, t.desc_nc / (t.cantidad_bruto - t.cantidad_nc), 0) AS desc_nc_unit";
    str_query += ", IF((t.cantidad_bruto - t.cantidad_nc) <> 0, IF(t.total <> 0, (t.precio / t.total), 0) * t.nc_monto / (t.cantidad_bruto - t.cantidad_nc), 0) AS desc_nc_monto_unit";
    str_query += ", t.serie AS serie, t.numero AS numero";
    str_query += ", t.persona_id AS persona_id, t.ruc AS ruc, t.razon_social AS razon_social FROM";
    str_query += " (SELECT";
    str_query += " documento.id AS id";
    str_query += ", documento.tipo_documento_id AS tipo_documento_id";
    str_query += ", anexo.fecha_emision AS fecha_emision";
    str_query += ", SUM(documento_h_producto.cantidad) AS cantidad_bruto";
    str_query += ", IFNULL((SELECT SUM(d_h_prod_nc_cantidad.cantidad_aux) FROM documento d";
    str_query += " JOIN documento_h_documento d_h_d_nc ON d.id = d_h_d_nc.documento_id1";
    str_query += " JOIN documento d_nc ON (d_h_d_nc.documento_id = d_nc.id AND d_nc.tipo_documento_id = "+QString().setNum(tipo_documento::NOTA_CREDITO)+")";
    str_query += " JOIN documento_h_producto d_h_prod_nc_cantidad ON (d_h_prod_nc_cantidad.producto_id = "+producto_id+" AND d_h_prod_nc_cantidad.documento_id = d_nc.id)";
    str_query += " WHERE d.id = documento.id), 0) AS cantidad_nc";

    str_query += ", SUM(documento_h_producto.precio * IF(factura.modalidad IS NULL, 1, IF(factura.modalidad = 1, 1.18, 1))) AS precio";

    str_query += ", (SELECT SUM(d_h_prod_total.precio * IF(factura.modalidad IS NULL, 1, IF(factura.modalidad = 1, 1.18, 1))) FROM documento_h_producto d_h_prod_total";
    str_query += " WHERE documento.id = d_h_prod_total.documento_id) AS total";

    str_query += ", IFNULL((SELECT SUM(d_h_prod_flete.precio_aux) FROM documento d";
    str_query += " JOIN documento_h_documento d_h_d_flete ON d.id = d_h_d_flete.documento_id1";
    str_query += " JOIN documento d_flete ON (d_h_d_flete.documento_id = d_flete.id AND d_flete.tipo_documento_id = "+QString().setNum(tipo_documento::FLETE)+")";
    str_query += " JOIN documento_h_producto d_h_prod_flete ON (d_h_prod_flete.producto_id = "+producto_id+" AND d_h_prod_flete.documento_id = d_flete.id)";
    str_query += " WHERE d.id = documento.id), 0) AS precio_flete";

    str_query += ", IFNULL((SELECT SUM(d_h_prod_nc_desc.precio_aux * IF(nc.modalidad IS NULL, 1, IF(nc.modalidad = 1, 1.18, 1))) FROM documento d";
    str_query += " JOIN documento_h_documento d_h_d_nc ON d.id = d_h_d_nc.documento_id1";
    str_query += " JOIN documento d_nc ON (d_h_d_nc.documento_id = d_nc.id AND d_nc.tipo_documento_id = "+QString().setNum(tipo_documento::NOTA_CREDITO)+")";
    str_query += " JOIN nota_credito nc ON d_nc.id = nc.comprobante_documento_id";
    str_query += " JOIN documento_h_producto d_h_prod_nc_desc ON (d_h_prod_nc_desc.producto_id = "+producto_id+" AND d_h_prod_nc_desc.documento_id = d_nc.id)";
    str_query += " WHERE d.id = documento.id), 0) AS desc_nc";

    str_query += ", IFNULL((SELECT SUM(nc.monto * IF(nc.modalidad IS NULL, 1, IF(nc.modalidad = 1, 1.18, 1))) FROM documento d";
    str_query += " JOIN documento_h_documento d_h_d_nc ON d.id = d_h_d_nc.documento_id1";
    str_query += " JOIN documento d_nc ON (d_h_d_nc.documento_id = d_nc.id AND d_nc.tipo_documento_id = "+QString().setNum(tipo_documento::NOTA_CREDITO)+")";
    str_query += " JOIN nota_credito nc ON d_nc.id = nc.comprobante_documento_id";
    str_query += " WHERE d.id = documento.id), 0) AS nc_monto";

    str_query += ", anexo.serie AS serie";
    str_query += ", anexo.numero AS numero";
    str_query += ", persona.id AS persona_id";
    str_query += ", juridica.ruc AS ruc";
    str_query += ", juridica.razon_social AS razon_social";
    str_query += " FROM producto";
    str_query += " JOIN documento_h_producto ON producto.id = documento_h_producto.producto_id";

    str_query += " JOIN documento ON ((documento.tipo_documento_id = "+QString().setNum(tipo_documento::SALDO);
    str_query += " OR documento.tipo_documento_id = "+QString().setNum(tipo_documento::BOLETA);
    str_query += " OR documento.tipo_documento_id = "+QString().setNum(tipo_documento::FACTURA)+")";
    str_query += " AND documento.id = documento_h_producto.documento_id)";
    str_query += " JOIN comprobante ON (comprobante.operacion_id = "+QString().setNum(operacion_items::COMPRA)+" AND documento.id = comprobante.documento_id)";

    str_query += " LEFT JOIN saldo ON saldo.comprobante_documento_id = documento.id";
    str_query += " LEFT JOIN boleta ON boleta.comprobante_documento_id = documento.id";
    str_query += " LEFT JOIN factura ON factura.comprobante_documento_id = documento.id";

    str_query += " JOIN anexo ON documento.id = anexo.documento_id";
    str_query += " JOIN documento_h_persona ON documento.id = documento_h_persona.documento_id";
    str_query += " LEFT JOIN persona ON persona.id = documento_h_persona.persona_id";
    str_query += " LEFT JOIN juridica ON persona.id = juridica.persona_id";
    str_query += " LEFT JOIN proveedor ON persona.id = proveedor.juridica_persona_id";
    str_query += " WHERE producto.id = "+producto_id+" AND YEAR(anexo.fecha_emision) BETWEEN "+QString().setNum(QDate::currentDate().year()-4)+" AND "+QString().setNum(QDate::currentDate().year());
    str_query += " GROUP BY documento.id";
    str_query += " ORDER BY anexo.fecha_emision DESC, documento.id DESC";
    str_query += " LIMIT 1) AS t";

    return str_query;
}
