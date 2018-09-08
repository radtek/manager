#include "configgeneral.h"
#include "ui_configgeneral.h"

ConfigGeneral::ConfigGeneral(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConfigGeneral)
{
    ui->setupUi(this);

    afterShow = false;

    firstShow = false;

    /*
    disconnect(ui->spinBox_familia_filas, SIGNAL(valueChanged(int))
               , this, SLOT(on_spinBox_familia_filas_valueChanged()));
    ui->spinBox_familia_filas->setValue(3);
    connect(ui->spinBox_familia_filas, SIGNAL(valueChanged(int))
               , this, SLOT(on_spinBox_familia_filas_valueChanged()));
    ui->spinBox_familia_filas->setValue(2);
    disconnect(ui->spinBox_plato_filas, SIGNAL(valueChanged(int))
               , this, SLOT(on_spinBox_plato_filas_valueChanged()));
    ui->spinBox_plato_filas->setValue(8);
    connect(ui->spinBox_plato_filas, SIGNAL(valueChanged(int))
               , this, SLOT(on_spinBox_plato_filas_valueChanged()));
    ui->spinBox_plato_filas->setValue(7);
    */

    this->installEventFilter(this);
    ui->comboBox_familia->installEventFilter(this);
}

ConfigGeneral::~ConfigGeneral()
{
    delete ui;
}

void ConfigGeneral::setFondoTapiz(QString fondoTapiz)
{
    this->fondoTapiz = fondoTapiz;
}

void ConfigGeneral::setOperacion(QString operacion)
{
    this->operacion = operacion;
}

QString ConfigGeneral::getClave()
{
    return ui->lineEdit_clave->text();
}

int ConfigGeneral::getFamiliaFilas()
{
    return ui->spinBox_familia_filas->value();
}

int ConfigGeneral::getFamiliaColumnas()
{
    return ui->spinBox_familia_columnas->value();
}

int ConfigGeneral::getPlatoFilas()
{
    return ui->spinBox_plato_filas->value();
}

int ConfigGeneral::getPlatoColumnas()
{
    return ui->spinBox_plato_columnas->value();
}

QString ConfigGeneral::getFondoTapiz()
{
    return fondoTapiz;
}
/*
void ConfigGeneral::delayedPopup()
{
    int val_x = ui->spinBox_familia_filas->value();
    int val_y = ui->spinBox_familia_columnas->value();
    for(int i = 0; i < val_x; i++) {
        for(int j = 0; j < val_y; j++) {
            QToolButton* tb = (QToolButton*)ui->gridLayout_familias->itemAtPosition(i, j)->widget();
            tb->setPopupMode(QToolButton::DelayedPopup);
        }
    }
    val_x = ui->spinBox_plato_filas->value();
    val_y = ui->spinBox_plato_columnas->value();
    for(int i = 0; i < val_x; i++) {
        for(int j = 0; j < val_y; j++) {
            QToolButton* tb = (QToolButton*)ui->gridLayout_platos->itemAtPosition(i, j)->widget();
            tb->setPopupMode(QToolButton::DelayedPopup);
        }
    }
    repaint();
}
void Comprobante::menuButtonPopup()
{
    int val_x = ui->spinBox_familia_filas->value();
    int val_y = ui->spinBox_familia_columnas->value();
    for(int i = 0; i < val_x; i++) {
        for(int j = 0; j < val_y; j++) {
            QToolButton* tb = (QToolButton*)ui->gridLayout_familias->itemAtPosition(i, j)->widget();
            tb->setPopupMode(QToolButton::MenuButtonPopup);
        }
    }
    val_x = ui->spinBox_plato_filas->value();
    val_y = ui->spinBox_plato_columnas->value();
    for(int i = 0; i < val_x; i++) {
        for(int j = 0; j < val_y; j++) {
            QToolButton* tb = (QToolButton*)ui->gridLayout_platos->itemAtPosition(i, j)->widget();
            tb->setPopupMode(QToolButton::MenuButtonPopup);
        }
    }
    repaint();
}
*/
void ConfigGeneral::clean_familias()
{
    for(int i = 0; i < 99; i++){
        for(int j = 0; j < 99; j++){
            SYSTEM->remove(ui->gridLayout_familias, i, j, true);
        }
    }
}
void ConfigGeneral::clean_platos()
{
    for(int i = 0; i < 99; i++){
        for(int j = 0; j < 99; j++){
            SYSTEM->remove(ui->gridLayout_platos, i, j, true);
        }
    }
}
void ConfigGeneral::select_familias()
{
    QSqlQuery query;
    QString str_query = "";
    str_query += "SELECT nombre, x, y";
    str_query += ", font , color, background_color, imagen_dir";
    str_query += " FROM familia";
    str_query += " WHERE operacion_item_nombre = '"+operacion+"'";
    str_query += QString() + " AND estado_item_nombre = '" + ACTIVO + "'";

    if(query.exec(str_query)){
        //QString familita = ui->comboBox_familia->currentText();
        ui->comboBox_familia->clear();

        while(query.next()){
            QString nombre = query.value(0).toString();
            int x = query.value(1).toInt() - 1;
            int y = query.value(2).toInt() - 1;
            QString str_font = query.value(3).toString();
            QFont font;
            font.fromString(str_font);
            QString color = query.value(4).toString();
            QString background_color = query.value(5).toString();
            QString imagen_dir = query.value(6).toString();

            int val_x = ui->spinBox_familia_filas->value();
            int val_y = ui->spinBox_familia_columnas->value();

            if (x < val_x && x >= 0 && y < val_y && y >= 0){
                ui->comboBox_familia->addItem(nombre);

                SYSTEM->set_toolButton_css(ui->gridLayout_familias, nombre
                                           , x, y, font
                                           , color, background_color);

                QToolButton* tb = (QToolButton*)ui->gridLayout_familias->itemAtPosition(x, y)->widget();

                tb->setIcon(QIcon(imagen_dir));
                tb->setIconSize(QSize(100, 100));
                tb->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
                tb->setMinimumHeight(100);
                tb->setMaximumHeight(100);
                if(tb){
                    disconnect(tb, SIGNAL(triggered(QAction*))
                               , this, SLOT(on_familia_nuevoAct_triggered(QAction*)));
                }
            }


            //delete tb->menu()->actions()[3];
        }

        //ui->comboBox_familia->setCurrentText(familita);
    }
}
void ConfigGeneral::select_platos(QString familia)
{
    QSqlQuery query;
    QString str_query = "";
    str_query += "SELECT producto.nombre, producto.x, producto.y";
    str_query += ", producto.font, producto.color, producto.background_color";
    str_query += ", producto.descripcion";
    str_query += " FROM producto";
    str_query += " JOIN familia ON familia.id = producto.familia_id";
    str_query += " WHERE familia.nombre = '"+familia+"'";
    str_query += " AND familia.operacion_item_nombre = '"+operacion+"'";
    str_query += " AND producto.operacion_item_nombre = '"+operacion+"'";
    str_query += QString() + " AND producto.estado_item_nombre = '" + ACTIVO + "'";

    qDebug()<<str_query<<endl;
    if(query.exec(str_query)){
        ui->comboBox_plato->clear();

        while(query.next()){
            QString nombre = query.value(0).toString();
            int x = query.value(1).toInt() - 1;
            int y = query.value(2).toInt() - 1;
            QString str_font = query.value(3).toString();
            QFont font;
            font.fromString(str_font);
            QString color = query.value(4).toString();
            QString background_color = query.value(5).toString();
            //QString precio = query.value(6).toString();
            QString descripcion = query.value(6).toString();

            int val_x = ui->spinBox_plato_filas->value();
            int val_y = ui->spinBox_plato_columnas->value();

            if(x < val_x && x >= 0 && y < val_y && y >= 0) {
                ui->comboBox_plato->addItem(nombre);

                SYSTEM->set_toolButton_css(ui->gridLayout_platos, nombre
                                           , x, y, font
                                           , color, background_color);


                QToolButton* tb = (QToolButton*)ui->gridLayout_platos->itemAtPosition(x, y)->widget();
                if(tb){
                    disconnect(tb, SIGNAL(triggered(QAction*)), this, SLOT(on_plato_nuevoAct_triggered(QAction*)));
                }
            }
        }

        /*
        for(int i = 0; i < ui->tableWidget->rowCount(); i++){
            bool borrar = true;
            for(int j = 0; j < platos_existentes.size(); j++){
                if(platos_existentes[j].compare(ui->tableWidget->item(i, 0)->text()) == 0){
                    borrar = false;
                }
            }
            if(borrar){
                ui->tableWidget->removeRow(i);
                i--;
            }
        }*/

        if(ui->comboBox_plato->count() > 0){
            //ui->comboBox_plato->setCurrentText(platito);
            //if(ui->comboBox_plato->currentText().compare("") == 0){
                //ui->comboBox_plato->setCurrentIndex(0);
            //}
        }else{
            ui->comboBox_plato->clear();
            ui->comboBox_plato->clearEditText();
        }
    }
}

void ConfigGeneral::on_familia_closing()
{
    Familia* w = (Familia*) sender();

    qDebug()<<"familia_closing"<<endl;
    disconnect(ui->spinBox_familia_filas, SIGNAL(valueChanged(int))
               , this, SLOT(on_spinBox_familia_filas_valueChanged()));
    ui->spinBox_familia_filas->setValue(ui->spinBox_familia_filas->value()+1);
    connect(ui->spinBox_familia_filas, SIGNAL(valueChanged(int))
               , this, SLOT(on_spinBox_familia_filas_valueChanged()));
    ui->spinBox_familia_filas->setValue(ui->spinBox_familia_filas->value()-1);
    select_familias();
}
void ConfigGeneral::on_plato_closing()
{
    Plato* w = (Plato*) sender();

    disconnect(ui->spinBox_plato_filas, SIGNAL(valueChanged(int))
               , this, SLOT(on_spinBox_plato_filas_valueChanged()));
    ui->spinBox_plato_filas->setValue(ui->spinBox_plato_filas->value()+1);
    connect(ui->spinBox_plato_filas, SIGNAL(valueChanged(int))
               , this, SLOT(on_spinBox_plato_filas_valueChanged()));
    ui->spinBox_plato_filas->setValue(ui->spinBox_plato_filas->value()-1);
    select_platos(ui->comboBox_familia->currentText());
}
void ConfigGeneral::on_familia_fontAct_triggered(QAction* act)
{
    if(act->text().compare("&Fuente") == 0){
        bool ok;
        QFont font = QFontDialog::getFont(
                      &ok, QFont("MS Shell Dlg 2", 8, -1, false), this);
        if (ok) {
            QToolButton* tButton = (QToolButton*)sender();
            tButton->setFont(font);

            QString str_query = "";
            QSqlQuery query;
            str_query += "UPDATE familia";
            str_query += " SET font = '" + font.toString() + "'";
            str_query += " WHERE nombre = '" + tButton->text() + "'";
            str_query += " AND operacion_item_nombre = '" + operacion + "'";

            qDebug()<<str_query<<endl;
            if(query.exec(str_query)){
                qDebug()<<"query ok"<<endl;
            }
        } else {

        }
    }
}
void ConfigGeneral::on_familia_colorAct_triggered(QAction* act)
{
    if(act->text().compare("&Color") == 0){
        QColor color = QColorDialog::getColor(Qt::black, this, "Pick Color");

        QToolButton* tButton = (QToolButton*)sender();

        QString bg_color = SYSTEM->extract_background_color(tButton->styleSheet());
        QString str_color = color.name();

        QString ss = "QToolButton{ color: "+str_color+"; background-color: "+bg_color+"; }";
        tButton->setStyleSheet(ss);

        QString str_query = "";
        QSqlQuery query;
        str_query += "UPDATE familia";
        str_query += " SET color = '" + color.name() + "'";
        str_query += " WHERE nombre = '" + tButton->text() + "'";
        str_query += " AND operacion_item_nombre = '" + operacion + "'";

        qDebug()<<str_query<<endl;
        if(query.exec(str_query)){
            qDebug()<<"query ok"<<endl;
        }
    }
}
void ConfigGeneral::on_familia_color_de_fondo_Act_triggered(QAction* act)
{
    if(act->text().compare("&Color de Fondo") == 0){
        QColor color = QColorDialog::getColor(Qt::black, this, "Pick Color");

        QToolButton* tButton = (QToolButton*)sender();

        QString bg_color = color.name();
        QString str_color = SYSTEM->extract_color(tButton->styleSheet());

        QString ss = "QToolButton{ color: "+str_color+"; background-color: "+bg_color+"; }";
        tButton->setStyleSheet(ss);

        QString str_query = "";
        QSqlQuery query;
        str_query += "UPDATE familia";
        str_query += " SET background_color = '" + color.name() + "'";
        str_query += " WHERE nombre = '" + tButton->text() + "'";
        str_query += " AND operacion_item_nombre = '" + operacion + "'";

        qDebug()<<str_query<<endl;
        if(query.exec(str_query)){
            qDebug()<<"query ok"<<endl;
        }
    }
}
void ConfigGeneral::on_familia_nuevoAct_triggered(QAction* act)
{
    if(act->text().compare("&Nuevo") == 0){
        AdminPass* ap = new AdminPass(this);
        ap->setModalidad(AdminPass::CONFIRM_MY_PASS);
        //ap->setAttribute(Qt::WA_DeleteOnClose);
        ap->exec();
        bool confirmado = ap->get_confirmado();

        if(!confirmado){
            return;
        }
        delete ap;

        Familia* familia = new Familia(this);
        familia->setWindowFlag(Qt::Dialog);
        familia->setAttribute(Qt::WA_DeleteOnClose);
        familia->setWindowModality(Qt::WindowModal);
        familia->setOperacion(operacion);

        QToolButton* tb = (QToolButton*)sender();

        int x = 0;
        int y = 0;
        for(int i = 0; i < ui->gridLayout_familias->rowCount(); i++){
            bool flag = false;
            for(int j = 0; j < ui->gridLayout_familias->columnCount(); j++){
                if(ui->gridLayout_familias->itemAtPosition(i, j)->widget() == tb){
                    x = i+1;
                    y = j+1;
                    flag = true;
                    break;
                }
            }
            if(flag)break;
        }
        familia->modo_ingresar(x, y);

        connect(familia, SIGNAL(closing()), this, SLOT(on_familia_closing()));

        familia->show();
    }
}
void ConfigGeneral::on_familia_modificarAct_triggered(QAction* act)
{
    if(act->text().compare("&Modificar") == 0){
        QToolButton* tb = (QToolButton*)sender();
        if(tb->text().compare("____") == 0)
            return;

        AdminPass* ap = new AdminPass(this);
        ap->setModalidad(AdminPass::CONFIRM_MY_PASS);
        //ap->setAttribute(Qt::WA_DeleteOnClose);
        ap->exec();
        bool confirmado = ap->get_confirmado();

        if(!confirmado){
            return;
        }

        Familia* familia = new Familia(this);
        familia->setWindowFlag(Qt::Dialog);
        familia->setAttribute(Qt::WA_DeleteOnClose);
        familia->setWindowModality(Qt::WindowModal);
        familia->setOperacion(operacion);

        int x = 0;
        int y = 0;
        for(int i = 0; i < ui->gridLayout_familias->rowCount(); i++){
            bool flag = false;
            for(int j = 0; j < ui->gridLayout_familias->columnCount(); j++){
                if(ui->gridLayout_familias->itemAtPosition(i, j)->widget() == tb){
                    x = i+1;
                    y = j+1;
                    flag = true;
                    break;
                }
            }
            if(flag)break;
        }
        familia->modo_modificacion(tb->text(), x, y);

        connect(familia, SIGNAL(closing()), this, SLOT(on_familia_closing()));

        familia->show();
    }
}

void ConfigGeneral::on_plato_fontAct_triggered(QAction* act)
{
    if(act->text().compare("&Fuente") == 0){
        bool ok;
        QFont font = QFontDialog::getFont(
                      &ok, QFont("MS Shell Dlg 2", 8, -1, false), this);
        if (ok) {
            QToolButton* tButton = (QToolButton*)sender();
            tButton->setFont(font);

            QString str_query = "";
            QSqlQuery query;
            str_query += "UPDATE producto";
            str_query += " SET font = '" + font.toString() + "'";
            str_query += " WHERE familia_id = (SELECT t.id FROM familia t WHERE t.nombre = '"+ui->comboBox_familia->currentText()+"'";
            str_query += " AND t.operacion_item_nombre = '"+operacion+"')";
            str_query += " AND nombre = '"+tButton->text()+"'";
            str_query += " AND operacion_item_nombre = '"+operacion+"'";

            qDebug()<<str_query<<endl;
            if(query.exec(str_query)){
                qDebug()<<"query ok"<<endl;
            }
        } else {

        }
    }
}
void ConfigGeneral::on_plato_colorAct_triggered(QAction* act)
{
    if(act->text().compare("&Color") == 0){
        QColor color = QColorDialog::getColor(Qt::black, this, "Pick Color");

        QToolButton* tButton = (QToolButton*)sender();

        QString bg_color = SYSTEM->extract_background_color(tButton->styleSheet());
        QString str_color = color.name();

        QString ss = "QToolButton{ color: "+str_color+"; background-color: "+bg_color+"; }";
        tButton->setStyleSheet(ss);

        QString str_query = "";
        QSqlQuery query;
        str_query += "UPDATE producto";
        str_query += " SET color = '" + color.name() + "'";
        str_query += " WHERE familia_id = (SELECT t.id FROM familia t WHERE t.nombre = '"+ui->comboBox_familia->currentText()+"'";
        str_query += " AND t.operacion_item_nombre = '"+operacion+"')";
        str_query += " AND nombre = '"+tButton->text()+"'";
        str_query += " AND operacion_item_nombre = '"+operacion+"'";

        qDebug()<<str_query<<endl;
        if(query.exec(str_query)){
            qDebug()<<"query ok"<<endl;
        }
    }
}
void ConfigGeneral::on_plato_color_de_fondo_Act_triggered(QAction* act)
{
    if(act->text().compare("&Color de Fondo") == 0){
        QColor color = QColorDialog::getColor(Qt::black, this, "Pick Color");

        QToolButton* tButton = (QToolButton*)sender();

        QString bg_color = color.name();
        QString str_color = SYSTEM->extract_color(tButton->styleSheet());

        QString ss = "QToolButton{ color: "+str_color+"; background-color: "+bg_color+"; }";
        tButton->setStyleSheet(ss);

        QString str_query = "";
        QSqlQuery query;
        str_query += "UPDATE producto";
        str_query += " SET background_color = '" + color.name() + "'";
        str_query += " WHERE familia_id = (SELECT t.id FROM familia t WHERE t.nombre = '"+ui->comboBox_familia->currentText()+"'";
        str_query += " AND t.operacion_item_nombre = '"+operacion+"')";
        str_query += " AND nombre = '"+tButton->text()+"'";
        str_query += " AND operacion_item_nombre = '"+operacion+"'";

        qDebug()<<str_query<<endl;
        if(query.exec(str_query)){
            qDebug()<<"query ok"<<endl;
        }
    }
}
void ConfigGeneral::on_plato_nuevoAct_triggered(QAction* act)
{
    if(act->text().compare("&Nuevo") == 0){
        AdminPass* ap = new AdminPass(this);
        ap->setModalidad(AdminPass::CONFIRM_MY_PASS);
        //ap->setAttribute(Qt::WA_DeleteOnClose);
        ap->exec();
        bool confirmado = ap->get_confirmado();

        if(!confirmado){
            return;
        }
        delete ap;

        Plato* plato = new Plato(this);
        plato->setWindowFlag(Qt::Dialog);
        plato->setAttribute(Qt::WA_DeleteOnClose);
        plato->setWindowModality(Qt::WindowModal);
        plato->setOperacion(operacion);

        QToolButton* tb = (QToolButton*)sender();

        int x = 0;
        int y = 0;
        for(int i = 0; i < ui->gridLayout_platos->rowCount(); i++){
            bool flag = false;
            for(int j = 0; j < ui->gridLayout_platos->columnCount(); j++){
                if(ui->gridLayout_platos->itemAtPosition(i, j)->widget() == tb){
                    x = i+1;
                    y = j+1;
                    flag = true;
                    break;
                }
            }
            if(flag)break;
        }
        plato->modo_ingresar(ui->comboBox_familia->currentText(), x, y);

        //plato->set_familia_(ui->comboBox_familia->currentText());

        connect(plato, SIGNAL(closing()), this, SLOT(on_plato_closing()));

        plato->show();
    }

}
void ConfigGeneral::on_plato_modificarAct_triggered(QAction* act)
{
    if(act->text().compare("&Modificar") == 0){
        QToolButton* tb = (QToolButton*)sender();
        if(tb->text().compare("____") == 0)
            return;

        AdminPass* ap = new AdminPass(this);
        ap->setModalidad(AdminPass::CONFIRM_MY_PASS);
        //ap->setAttribute(Qt::WA_DeleteOnClose);
        ap->exec();
        bool confirmado = ap->get_confirmado();

        if(!confirmado){
            return;
        }
        delete ap;

        Plato* plato = new Plato(this);
        plato->setWindowFlag(Qt::Dialog);
        plato->setAttribute(Qt::WA_DeleteOnClose);
        plato->setWindowModality(Qt::WindowModal);
        plato->setOperacion(operacion);

        int x = 0;
        int y = 0;
        for(int i = 0; i < ui->gridLayout_platos->rowCount(); i++){
            bool flag = false;
            for(int j = 0; j < ui->gridLayout_platos->columnCount(); j++){
                if(ui->gridLayout_platos->itemAtPosition(i, j)->widget() == tb){
                    x = i+1;
                    y = j+1;
                    flag = true;
                    break;
                }
            }
            if(flag)break;
        }
        plato->modo_modificar(ui->comboBox_familia->currentText(), tb->text(), x, y);

        connect(plato, SIGNAL(closing()), this, SLOT(on_plato_closing()));

        plato->show();
    }
}

void ConfigGeneral::on_cliente_closing()
{

}

void ConfigGeneral::on_comboBox_familia_activated(const QString &arg1)
{
    disconnect(ui->spinBox_plato_filas, SIGNAL(valueChanged(int))
               , this, SLOT(on_spinBox_plato_filas_valueChanged()));
    ui->spinBox_plato_filas->setValue(ui->spinBox_plato_filas->value()+1);
    connect(ui->spinBox_plato_filas, SIGNAL(valueChanged(int))
               , this, SLOT(on_spinBox_plato_filas_valueChanged()));
    ui->spinBox_plato_filas->setValue(ui->spinBox_plato_filas->value()-1);
    select_platos(arg1);
}

void ConfigGeneral::on_comboBox_plato_activated(const QString &arg1)
{
    /*
    QString str_query = "";
    QSqlQuery query;
    str_query += "SELECT precio, descripcion FROM plato";
    str_query += " WHERE item_nombre = '"+arg1+"'";
    if(query.exec(str_query)){
        if(query.next()){


        }
    }
    */
}
void ConfigGeneral::on_spinBox_familia_filas_valueChanged(int arg1)
{
    clean_familias();

    int x = arg1;
    int y = ui->spinBox_familia_columnas->value();
    for(int i = 0; i < x; i++){
        for(int j = 0; j < y; j++){
            QToolButton* tb = new QToolButton;
            tb->setText("____");

            tb->setPopupMode(QToolButton::MenuButtonPopup);

            tb->setArrowType(Qt::NoArrow);
            QAction* fontAct = new QAction(tr("&Fuente"), this);
            //fontAct->setCheckable(true);
            //fontAct->setShortcut(QKeySequence(tr("ctrl+f","&Fuente")));
            fontAct->setStatusTip(tr("Cambiar fuente..."));
            tb->addAction(fontAct);


            QAction* colorAct = new QAction(tr("&Color"), this);
            //fontAct->setCheckable(true);
            //colorAct->setShortcut(QKeySequence(tr("ctrl+c","Color")));
            colorAct->setStatusTip(tr("Cambiar color..."));
            tb->addAction(colorAct);


            QAction* colorFondoAct = new QAction(tr("&Color de Fondo"), this);
            //fontAct->setCheckable(true);
            //colorFondoAct->setShortcutContext(Qt::WidgetWithChildrenShortcut);
            //colorFondoAct->setShortcut(QKeySequence(tr("ctrl+d","Color de Fon&do")));
            colorFondoAct->setStatusTip(tr("Cambiar color de fondo..."));
            tb->addAction(colorFondoAct);


            QAction* nuevoAct = new QAction(tr("&Nuevo"), this);
            //fontAct->setCheckable(true);
            //colorAct->setShortcut(QKeySequence(tr("ctrl+c","Color")));
            nuevoAct->setStatusTip(tr("Nueva familia"));
            tb->addAction(nuevoAct);

            QAction* modificarAct = new QAction(tr("&Modificar"), this);
            //fontAct->setCheckable(true);
            //colorAct->setShortcut(QKeySequence(tr("ctrl+c","Color")));
            modificarAct->setStatusTip(tr("Modificar familia"));
            tb->addAction(modificarAct);

            connect(tb, SIGNAL(triggered(QAction*)), this, SLOT(on_familia_nuevoAct_triggered(QAction*)));
            connect(tb, SIGNAL(triggered(QAction*)), this, SLOT(on_familia_modificarAct_triggered(QAction*)));

            connect(tb, SIGNAL(triggered(QAction*)), this, SLOT(on_familia_fontAct_triggered(QAction*)));
            connect(tb, SIGNAL(triggered(QAction*)), this, SLOT(on_familia_colorAct_triggered(QAction*)));
            connect(tb, SIGNAL(triggered(QAction*)), this, SLOT(on_familia_color_de_fondo_Act_triggered(QAction*)));

            connect(tb, SIGNAL(clicked()), this, SLOT(on_toolButton_familia_clicked()));

            ui->gridLayout_familias->addWidget(tb, i, j);
        }
    }
    select_familias();
}

void ConfigGeneral::on_spinBox_familia_columnas_valueChanged(int arg1)
{
    arg1 = 0;
    on_spinBox_familia_filas_valueChanged(ui->spinBox_familia_filas->value());
}

void ConfigGeneral::on_spinBox_plato_filas_valueChanged(int arg1)
{
    clean_platos();

    int x = arg1;
    int y = ui->spinBox_plato_columnas->value();
    for(int i = 0; i < x; i++){
        for(int j = 0; j < y; j++){
            QToolButton* tb = new QToolButton;
            tb->setText("____");

            tb->setPopupMode(QToolButton::MenuButtonPopup);

            tb->setArrowType(Qt::NoArrow);
            QAction* fontAct = new QAction(tr("&Fuente"), this);
            //fontAct->setCheckable(true);
            //fontAct->setShortcut(QKeySequence(tr("ctrl+f","Fuente")));
            fontAct->setStatusTip(tr("Cambiar fuente..."));
            tb->addAction(fontAct);


            QAction* colorAct = new QAction(tr("&Color"), this);
            //fontAct->setCheckable(true);
            //colorAct->setShortcut(QKeySequence(tr("ctrl+c","Color")));
            colorAct->setStatusTip(tr("Cambiar color..."));
            tb->addAction(colorAct);

            QAction* colorFondoAct = new QAction(tr("&Color de Fondo"), this);
            //fontAct->setCheckable(true);
            //colorFondoAct->setShortcutContext(Qt::WidgetWithChildrenShortcut);
            //colorFondoAct->setShortcut(QKeySequence(tr("ctrl+d","Color de Fon&do")));
            colorFondoAct->setStatusTip(tr("Cambiar color de fondo..."));
            tb->addAction(colorFondoAct);


            QAction* nuevoAct = new QAction(tr("&Nuevo"), this);
            //fontAct->setCheckable(true);
            //colorAct->setShortcut(QKeySequence(tr("ctrl+c","Color")));
            nuevoAct->setStatusTip(tr("Nuevo plato"));
            tb->addAction(nuevoAct);

            QAction* modificarAct = new QAction(tr("&Modificar"), this);
            //fontAct->setCheckable(true);
            //colorAct->setShortcut(QKeySequence(tr("ctrl+c","Color")));
            modificarAct->setStatusTip(tr("Modificar plato"));
            tb->addAction(modificarAct);

            connect(tb, SIGNAL(triggered(QAction*)), this, SLOT(on_plato_nuevoAct_triggered(QAction*)));
            connect(tb, SIGNAL(triggered(QAction*)), this, SLOT(on_plato_modificarAct_triggered(QAction*)));

            connect(tb, SIGNAL(triggered(QAction*)), this, SLOT(on_plato_fontAct_triggered(QAction*)));
            connect(tb, SIGNAL(triggered(QAction*)), this, SLOT(on_plato_colorAct_triggered(QAction*)));
            connect(tb, SIGNAL(triggered(QAction*)), this, SLOT(on_plato_color_de_fondo_Act_triggered(QAction*)));

            connect(tb, SIGNAL(clicked()), this, SLOT(on_toolButton_plato_clicked()));

            ui->gridLayout_platos->addWidget(tb, i, j);
        }
    }
    select_platos(ui->comboBox_familia->currentText());
}

void ConfigGeneral::on_spinBox_plato_columnas_valueChanged(int arg1)
{
    arg1 = 0;
    on_spinBox_plato_filas_valueChanged(ui->spinBox_plato_filas->value());
}
void ConfigGeneral::showEvent(QShowEvent *event)
{
    event->accept();

    afterShow = true;

    //if(isMaximized()){
        qDebug()<<"isMaximized"<<endl;

        QString str_query = "";
        QSqlQuery query;

        str_query += "SELECT splitter_1, splitter_2, splitter_3";
        str_query += ", familia_filas, familia_columnas";
        str_query += ", plato_filas, plato_columnas, clave_wifi FROM datos";

        qDebug()<<str_query<<endl;
        if(query.exec(str_query)){
            qDebug()<<"query ok config general"<<endl;
            if(!query.next()){
                return;
            }

            int splitter_1_val = query.value(0).toInt();
            int splitter_2_val = query.value(1).toInt();
            //int splitter_3_val = query.value(2).toInt();
            int familia_filas = query.value(3).toInt();
            int familia_columnas = query.value(4).toInt();
            int plato_filas = query.value(5).toInt();
            int plato_columnas = query.value(6).toInt();
            QString clave_wifi = query.value(7).toString();
            //QString fondoTapiz = query.value(8).toString();

            //const QRect mainScreenSize = this->rect();
            //ui->splitter_mid->setSizes(QList<int>() << splitter_1_val << splitter_2_val);

            disconnect(ui->spinBox_familia_filas, SIGNAL(valueChanged(int))
                       , this, SLOT(on_spinBox_familia_filas_valueChanged()));
            disconnect(ui->spinBox_familia_columnas, SIGNAL(valueChanged(int))
                       , this, SLOT(on_spinBox_familia_columnas_valueChanged()));

            disconnect(ui->spinBox_plato_filas, SIGNAL(valueChanged(int))
                       , this, SLOT(on_spinBox_plato_filas_valueChanged()));
            disconnect(ui->spinBox_plato_columnas, SIGNAL(valueChanged(int))
                       , this, SLOT(on_spinBox_plato_columnas_valueChanged()));

            ui->spinBox_familia_filas->setValue(familia_filas);
            ui->spinBox_familia_columnas->setValue(familia_columnas);
            ui->spinBox_plato_filas->setValue(plato_filas);
            ui->spinBox_plato_columnas->setValue(plato_columnas);

            ui->spinBox_familia_filas->setValue(ui->spinBox_familia_filas->value()+1);
            connect(ui->spinBox_familia_filas, SIGNAL(valueChanged(int))
                       , this, SLOT(on_spinBox_familia_filas_valueChanged()));
            ui->spinBox_familia_filas->setValue(ui->spinBox_familia_filas->value()-1);

            ui->spinBox_plato_filas->setValue(ui->spinBox_plato_filas->value()+1);
            connect(ui->spinBox_plato_filas, SIGNAL(valueChanged(int))
                       , this, SLOT(on_spinBox_plato_filas_valueChanged()));
            ui->spinBox_plato_filas->setValue(ui->spinBox_plato_filas->value()-1);

            connect(ui->spinBox_familia_columnas, SIGNAL(valueChanged(int))
                       , this, SLOT(on_spinBox_familia_columnas_valueChanged()));

            connect(ui->spinBox_plato_columnas, SIGNAL(valueChanged(int))
                       , this, SLOT(on_spinBox_plato_columnas_valueChanged()));

            //this->fondoTapiz = fondoTapiz;
            ui->lineEdit_clave->setText(clave_wifi);

            qDebug() << "config general show: " << fondoTapiz << endl;
            QString ss;
            ss = "QSplitter::handle{ background-color: black }\n"
                 "QWidget{ background-color: " + fondoTapiz +" }\n"
                 "QLineEdit{ background-color: rgb(255, 255, 255) }\n"
                 "QLineEdit{ color: rgb(0, 0, 0) }\n"
                 "QTableWidget{ background-color: rgb(255, 255, 255) }\n"
                 "QComboBox{ background-color: rgb(255, 255, 255) }\n"
                 "QSpinBox{ background-color: rgb(255, 255, 255) }\n"
                 "QDoubleSpinBox{ background-color: rgb(255, 255, 255) }\n"
                 "QHeaderView{ background-color: rgb(255, 255, 255) }\n"
                 "QScrollBar{ background-color: rgb(255, 255, 255) }\n"
                 "QPushButton{ background-color: rgb(255, 255, 255) }\n"
                 "QToolButton{ background-color: rgb(255, 255, 255) }\n"
                 "QMenu{ background-color: rgb(255, 255, 255) }";
            this->setStyleSheet(ss);
        }else{
            qDebug()<<"query bad"<<endl;

        }
    //}
}
void ConfigGeneral::closeEvent(QCloseEvent *event)
{
    event->accept();

    emit closing();
    /*
    QString str_query = "";
    QSqlQuery query;

    str_query += "UPDATE datos SET";
    str_query += " splitter_1 = " + QString().setNum(ui->splitter_mid->sizes()[0]);
    str_query += ", splitter_2 = " + QString().setNum(ui->splitter_mid->sizes()[1]);
    str_query += ", familia_filas = " + QString().setNum(ui->spinBox_familia_filas->value());
    str_query += ", familia_columnas = " + QString().setNum(ui->spinBox_familia_columnas->value());
    str_query += ", plato_filas = " + QString().setNum(ui->spinBox_plato_filas->value());
    str_query += ", plato_columnas = " + QString().setNum(ui->spinBox_plato_columnas->value());

    qDebug() << ui->splitter_mid->sizes()[0] << endl;
    qDebug()<<str_query<<endl;
    if(query.exec(str_query)){
        qDebug()<<"query ok"<<endl;
    }else{
        qDebug()<<"query bad"<<endl;
    }*/
}

bool ConfigGeneral::eventFilter(QObject *watched, QEvent *event)
{
    QWidget* w_temp;
    w_temp = this;
    if(watched == w_temp){
        if (event->type() == QEvent::Resize)
        {
            QResizeEvent* resizeEv = static_cast<QResizeEvent*>(event);
            resizeEv->size();
            //qDebug() << resizeEv->size()<<endl;
            //qDebug()<<this->size().height()<<endl;
            return true;
        }
        if (event->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent* mousePress = static_cast<QMouseEvent*>(event);
            mousePress->pos();
            return true;
        }
        if (event->type() == QEvent::MouseButtonRelease)
        {
            QMouseEvent* mouseRelease = static_cast<QMouseEvent*>(event);
            mouseRelease->pos();
            return true;
        }

        if(event->type() == QEvent::Paint){
            if(focusWidget()){
                focusWidget()->setFocus();
            }
            if(afterShow) {

                afterShow = false;
            }
            if(!firstShow) {
                firstShow = true;
            }

            return true;
        }
        if(event->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)event;
            switch(KeyEvent->key())
            {
            case Qt::Key_Escape:{

                return true;
            }break;
            }
        }
        return false;
    }
    if(watched == ui->comboBox_familia){
        QKeyEvent* ke = (QKeyEvent*)event;
        switch(ke->key()){
        case Qt::Key_Return:{
            qDebug()<<"return comboBox"<<endl;

            return false;
        }break;
        }
        return false;
    }

    return eventFilter(watched, event);
}

void ConfigGeneral::on_toolButton_familia_clicked()
{
    QToolButton* toolB = (QToolButton*)sender();

    ui->comboBox_familia->setCurrentText(toolB->text());

    clean_platos();

    ui->spinBox_plato_filas->setValue(ui->spinBox_plato_filas->value()+1);
    connect(ui->spinBox_plato_filas, SIGNAL(valueChanged(int))
               , this, SLOT(on_spinBox_plato_filas_valueChanged()));
    ui->spinBox_plato_filas->setValue(ui->spinBox_plato_filas->value()-1);

    connect(ui->spinBox_familia_columnas, SIGNAL(valueChanged(int))
               , this, SLOT(on_spinBox_familia_columnas_valueChanged()));

    connect(ui->spinBox_plato_columnas, SIGNAL(valueChanged(int))
               , this, SLOT(on_spinBox_plato_columnas_valueChanged()));

    //select_platos(toolB->text());
}

void ConfigGeneral::on_toolButton_plato_clicked()
{
    QToolButton* toolB = (QToolButton*)sender();
    on_comboBox_plato_activated(toolB->text());
}

void ConfigGeneral::on_pushButton_guardar_clave_clicked()
{
    QSqlQuery query;
    QString str_query = "";

    str_query += "UPDATE datos SET";
    str_query += " clave_wifi = \"" + ui->lineEdit_clave->text() + "\"";

    if(query.exec(str_query)){        
        QMessageBox::information(this, "Información", "Clave cambiada.", "Ya");
    }else{
        QMessageBox::critical(this, "Error", "!", "Ok");
    }
}

void ConfigGeneral::on_comboBox_familia_currentTextChanged(const QString &arg1)
{
    //on_comboBox_familia_activated(arg1);
}

void ConfigGeneral::on_pushButton_fondoTapiz_clicked()
{
    QColor color = QColorDialog::getColor(Qt::black, this, "Pick Color");

    QString bg_color = color.name();
    //QString str_color = SYSTEM->extract_color(tButton->styleSheet());

    fondoTapiz = bg_color;
    QString ss;
    ss = "QSplitter::handle{ background-color: black }\n"
         "QWidget{ background-color: " + bg_color +" }\n"
         "QLineEdit{ background-color: rgb(255, 255, 255) }\n"
         "QLineEdit{ color: rgb(0, 0, 0) }\n"
         "QTableWidget{ background-color: rgb(255, 255, 255) }\n"
         "QComboBox{ background-color: rgb(255, 255, 255) }\n"
         "QSpinBox{ background-color: rgb(255, 255, 255) }\n"
         "QDoubleSpinBox{ background-color: rgb(255, 255, 255) }\n"
         "QHeaderView{ background-color: rgb(255, 255, 255) }\n"
         "QScrollBar{ background-color: rgb(255, 255, 255) }\n"
         "QPushButton{ background-color: rgb(255, 255, 255) }\n"
         "QToolButton{ background-color: rgb(255, 255, 255) }\n"
         "QMenu{ background-color: rgb(255, 255, 255) }";
    this->setStyleSheet(ss);

    /*
    QString str_query = "";
    QSqlQuery query;
    str_query += "UPDATE plato";
    str_query += " SET background_color = '" + color.name() + "'";
    str_query += " WHERE item_nombre = '" + tButton->text() + "'";

    qDebug()<<str_query<<endl;
    if(query.exec(str_query)){
        qDebug()<<"query ok"<<endl;
    }*/
}

void ConfigGeneral::on_pushButton_formatoFamilia_clicked()
{
    bool ok = false;
    QFont font = QFontDialog::getFont(
                  &ok, QFont("MS Shell Dlg 2", 8, -1, false), this);

    QString str_query = "";
    QSqlQuery query;
    str_query += "UPDATE familia";
    str_query += " SET font = '" + font.toString() + "'";
    str_query += " WHERE operacion_item_nombre = '" + operacion + "'";

    qDebug()<<str_query<<endl;
    if(query.exec(str_query)){
        qDebug()<<"query ok"<<endl;
        QMessageBox::information(this, "Información", "Se hicieron los cambios correctamente.", "Ok");
        disconnect(ui->spinBox_familia_filas, SIGNAL(valueChanged(int))
                   , this, SLOT(on_spinBox_familia_filas_valueChanged()));
        ui->spinBox_familia_filas->setValue(ui->spinBox_familia_filas->value()+1);
        connect(ui->spinBox_familia_filas, SIGNAL(valueChanged(int))
                   , this, SLOT(on_spinBox_familia_filas_valueChanged()));
        ui->spinBox_familia_filas->setValue(ui->spinBox_familia_filas->value()-1);
    }
}

void ConfigGeneral::on_pushButton_colorFamilia_clicked()
{
    QColor color = QColorDialog::getColor(Qt::black, this, "Pick Color");

    QString bg_color = color.name();

    QString str_query = "";
    QSqlQuery query;
    str_query += "UPDATE familia";
    str_query += " SET color = '" + color.name() + "'";
    str_query += " WHERE operacion_item_nombre = '" + operacion + "'";

    qDebug()<<str_query<<endl;
    if(query.exec(str_query)){
        qDebug()<<"query ok"<<endl;
        QMessageBox::information(this, "Información", "Se hicieron los cambios correctamente.", "Ok");

        disconnect(ui->spinBox_familia_filas, SIGNAL(valueChanged(int))
                   , this, SLOT(on_spinBox_familia_filas_valueChanged()));
        ui->spinBox_familia_filas->setValue(ui->spinBox_familia_filas->value()+1);
        connect(ui->spinBox_familia_filas, SIGNAL(valueChanged(int))
                   , this, SLOT(on_spinBox_familia_filas_valueChanged()));
        ui->spinBox_familia_filas->setValue(ui->spinBox_familia_filas->value()-1);
    }
}

void ConfigGeneral::on_pushButton_fondoFamilia_clicked()
{
    QColor color = QColorDialog::getColor(Qt::black, this, "Pick Color");

    QString bg_color = color.name();

    QString str_query = "";
    QSqlQuery query;
    str_query += "UPDATE familia";
    str_query += " SET background_color = '" + color.name() + "'";
    str_query += " WHERE operacion_item_nombre = '" + operacion + "'";

    qDebug()<<str_query<<endl;
    if(query.exec(str_query)){
        qDebug()<<"query ok"<<endl;
        QMessageBox::information(this, "Información", "Se hicieron los cambios correctamente.", "Ok");

        disconnect(ui->spinBox_familia_filas, SIGNAL(valueChanged(int))
                   , this, SLOT(on_spinBox_familia_filas_valueChanged()));
        ui->spinBox_familia_filas->setValue(ui->spinBox_familia_filas->value()+1);
        connect(ui->spinBox_familia_filas, SIGNAL(valueChanged(int))
                   , this, SLOT(on_spinBox_familia_filas_valueChanged()));
        ui->spinBox_familia_filas->setValue(ui->spinBox_familia_filas->value()-1);
    }
}

void ConfigGeneral::on_pushButton_formatoPlato_clicked()
{
    bool ok = false;
    QFont font = QFontDialog::getFont(
                  &ok, QFont("MS Shell Dlg 2", 8, -1, false), this);

    QString str_query = "";
    QSqlQuery query;
    str_query += "UPDATE producto";
    str_query += " SET font = '" + font.toString() + "'";
    str_query += " WHERE familia_id = (SELECT t.id FROM familia t WHERE t.nombre = '"+ui->comboBox_familia->currentText()+"'";
    str_query += " AND t.operacion_item_nombre = '"+operacion+"')";
    str_query += " AND operacion_item_nombre = '"+operacion+"'";

    qDebug()<<str_query<<endl;
    if(query.exec(str_query)){
        qDebug()<<"query ok"<<endl;
        QMessageBox::information(this, "Información", "Se hicieron los cambios correctamente.", "Ok");

        disconnect(ui->spinBox_plato_filas, SIGNAL(valueChanged(int))
                   , this, SLOT(on_spinBox_plato_filas_valueChanged()));
        ui->spinBox_plato_filas->setValue(ui->spinBox_plato_filas->value()+1);
        connect(ui->spinBox_plato_filas, SIGNAL(valueChanged(int))
                   , this, SLOT(on_spinBox_plato_filas_valueChanged()));
        ui->spinBox_plato_filas->setValue(ui->spinBox_plato_filas->value()-1);
    }
}

void ConfigGeneral::on_pushButton_colorPlato_clicked()
{
    QColor color = QColorDialog::getColor(Qt::black, this, "Pick Color");

    QString bg_color = color.name();

    QString str_query = "";
    QSqlQuery query;
    str_query += "UPDATE producto";
    str_query += " SET color = '" + color.name() + "'";
    str_query += " WHERE familia_id = (SELECT t.id FROM familia t WHERE t.nombre = '"+ui->comboBox_familia->currentText()+"'";
    str_query += " AND t.operacion_item_nombre = '"+operacion+"')";
    str_query += " AND operacion_item_nombre = '"+operacion+"'";

    qDebug()<<str_query<<endl;
    if(query.exec(str_query)){
        qDebug()<<"query ok"<<endl;
        QMessageBox::information(this, "Información", "Se hicieron los cambios correctamente.", "Ok");

        disconnect(ui->spinBox_plato_filas, SIGNAL(valueChanged(int))
                   , this, SLOT(on_spinBox_plato_filas_valueChanged()));
        ui->spinBox_plato_filas->setValue(ui->spinBox_plato_filas->value()+1);
        connect(ui->spinBox_plato_filas, SIGNAL(valueChanged(int))
                   , this, SLOT(on_spinBox_plato_filas_valueChanged()));
        ui->spinBox_plato_filas->setValue(ui->spinBox_plato_filas->value()-1);
    }
}

void ConfigGeneral::on_pushButton_fondoPlato_clicked()
{
    QColor color = QColorDialog::getColor(Qt::black, this, "Pick Color");

    QString bg_color = color.name();

    QString str_query = "";
    QSqlQuery query;
    str_query += "UPDATE producto";
    str_query += " SET background_color = '" + color.name() + "'";
    str_query += " WHERE familia_id = (SELECT t.id FROM familia t WHERE t.nombre = '"+ui->comboBox_familia->currentText()+"'";
    str_query += " AND t.operacion_item_nombre = '"+operacion+"')";
    str_query += " AND operacion_item_nombre = '"+operacion+"'";

    qDebug()<<str_query<<endl;
    if(query.exec(str_query)){
        qDebug()<<"query ok"<<endl;
        QMessageBox::information(this, "Información", "Se hicieron los cambios correctamente.", "Ok");

        disconnect(ui->spinBox_plato_filas, SIGNAL(valueChanged(int))
                   , this, SLOT(on_spinBox_plato_filas_valueChanged()));
        ui->spinBox_plato_filas->setValue(ui->spinBox_plato_filas->value()+1);
        connect(ui->spinBox_plato_filas, SIGNAL(valueChanged(int))
                   , this, SLOT(on_spinBox_plato_filas_valueChanged()));
        ui->spinBox_plato_filas->setValue(ui->spinBox_plato_filas->value()-1);
    }
}
