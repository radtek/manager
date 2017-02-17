#include "colaboradorbuscar.h"
#include "ui_colaboradorbuscar.h"

ColaboradorBuscar::ColaboradorBuscar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ColaboradorBuscar)
{
    ui->setupUi(this);

    this->widget_previous = NULL;    

    pos = 0;
    size_query = 10;

    id = "";
    nombre = "";
    direccion = "";

    //disconnect(ui->pushButton_agregar, SIGNAL(clicked()), this, SLOT(on_pushButton_agregar_clicked()));
    //disconnect(ui->pushButton_modificar, SIGNAL(clicked()), this, SLOT(on_pushButton_modificar_clicked()));
    // INSTALL EVENTFILTER

    this->installEventFilter(this);
    ui->lineEdit_buscar->installEventFilter(this);
    ui->tableWidget->installEventFilter(this);
    ui->pushButton_ok->installEventFilter(this);
    ui->pushButton_salir->installEventFilter(this);
    ui->pushButton_agregar->installEventFilter(this);
    ui->pushButton_modificar->installEventFilter(this);
}

ColaboradorBuscar::~ColaboradorBuscar()
{
    qDebug()<<"delete colaborador buscar"<<endl;
    delete ui;
}
void ColaboradorBuscar::set_tipo(int tipo)
{
    this->tipo = tipo;    
}

void ColaboradorBuscar::set_widget_previous(QWidget *widget_previous)
{
    this->widget_previous = widget_previous;
}
QString ColaboradorBuscar::get_ID()
{
    return id;
}
QString ColaboradorBuscar::get_codigo()
{
    return codigo;
}
QString ColaboradorBuscar::get_nombre()
{
    return nombre;
}
QString ColaboradorBuscar::get_direccion()
{
    return direccion;
}
void ColaboradorBuscar::on_colaborador_closing()
{
    switch(tipo)
    {
    case persona_items::PROVEEDOR:{
        ProveedorOp* widget= (ProveedorOp*)QObject::sender();
        id = widget->get_ID();
        codigo = widget->get_ruc();
        nombre = widget->get_razon_social();
        direccion = widget->get_direccion();
    }break;
    case persona_items::TRANSPORTISTA:{
        TransportistaOp* widget= (TransportistaOp*)QObject::sender();
        id = widget->get_ID();
        codigo = widget->get_ruc();
        nombre = widget->get_razon_social();
        direccion = widget->get_direccion();
    }break;
    case persona_items::CLIENTE_RUC:{
        ClienteRUC* widget= (ClienteRUC*)QObject::sender();
        id = widget->get_ID();
        codigo = widget->get_ruc();
        nombre = widget->get_razon_social();
        direccion = widget->get_direccion();
    }break;
    case persona_items::CLIENTE_DNI:{
        ClienteDNI* widget= (ClienteDNI*)QObject::sender();
        id = widget->get_ID();
        codigo = widget->get_DNI();
        nombre = widget->get_nombre();
        direccion = widget->get_direccion();
    }break;
    case persona_items::USUARIO:{
        UsuarioOp* widget= (UsuarioOp*)QObject::sender();
        id = widget->get_ID();
        codigo = widget->get_DNI();
        nombre = widget->get_nombre();
    }break;

    }
    on_lineEdit_buscar_textChanged("");
}

void ColaboradorBuscar::on_lineEdit_buscar_textChanged(const QString& arg)
{
    connect(ui->lineEdit_buscar, SIGNAL(returnPressed()), this, SLOT(on_lineEdit_buscar_returnPressed()));
    pos = 0;

    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(0);
    ui->tableWidget->clear();
}

void ColaboradorBuscar::on_lineEdit_buscar_returnPressed()
{
    disconnect(ui->lineEdit_buscar, SIGNAL(returnPressed()), this, SLOT(on_lineEdit_buscar_returnPressed()));
    QString str_query;

    QString arg = ui->lineEdit_buscar->text();

    switch(tipo)
    {
    case persona_items::PROVEEDOR:{
        QString match = "";
        bool ok;
        arg.left(11).toFloat(&ok);
        if(ok){
            match = "ruc";
            arg = arg.mid(0, 11)+"%";
        }else{
            match = "razon_social";
            arg = arg+"%";
        }
        str_query = "SELECT persona.id";
        str_query += ", juridica.ruc";
        str_query += ", juridica.razon_social";
        str_query += ", juridica.domicilio_fiscal";
        str_query += " FROM persona";
        str_query += " JOIN juridica ON persona.id = juridica.persona_id";
        str_query += " JOIN proveedor ON persona.id = proveedor.juridica_persona_id";
        str_query += " WHERE juridica."+match+" LIKE '"+arg+"'";
        str_query += " ORDER BY juridica.razon_social";
        str_query += " LIMIT " + QString().setNum(pos) + ", " + QString().setNum(size_query) + "";
    }break;
    case persona_items::TRANSPORTISTA:{
        QString match = "";
        bool ok;
        arg.left(11).toFloat(&ok);
        if(ok){
            match = "ruc";
            arg = arg.mid(0, 11)+"%";
        }else{
            match = "razon_social";
            arg = arg+"%";
        }
        str_query = "SELECT persona.id";
        str_query += ", juridica.ruc";
        str_query += ", juridica.razon_social";
        str_query += ", juridica.domicilio_fiscal";
        str_query += " FROM persona";
        str_query += " JOIN juridica ON persona.id = juridica.persona_id";
        str_query += " JOIN transportista ON persona.id = transportista.juridica_persona_id";
        str_query += " WHERE juridica."+match+" LIKE '"+arg+"'";
        str_query += " ORDER BY juridica."+match;
        str_query += " LIMIT " + QString().setNum(pos) + ", " + QString().setNum(size_query) + "";
    }break;
    case persona_items::CLIENTE_RUC:{
        QString match = "";
        bool ok;
        arg.left(11).toFloat(&ok);
        if(ok){
            match = "ruc";
            arg = arg.mid(0, 11)+"%";
        }else{
            match = "razon_social";
            arg = arg+"%";
        }
        str_query = "SELECT persona.id";
        str_query += ", juridica.ruc";
        str_query += ", juridica.razon_social";
        str_query += ", juridica.domicilio_fiscal";
        str_query += " FROM persona";
        str_query += " JOIN juridica ON persona.id = juridica.persona_id";
        str_query += " JOIN cliente_ruc ON persona.id = cliente_ruc.juridica_persona_id";
        str_query += " WHERE juridica."+match+" LIKE '"+arg+"'";
        str_query += " ORDER BY juridica."+match;
        str_query += " LIMIT " + QString().setNum(pos) + ", " + QString().setNum(size_query) + "";
    }break;
    case persona_items::CLIENTE_DNI:{
        QString match = "";
        bool ok;
        arg.left(8).toFloat(&ok);
        if(ok){
            match = "dni";
            arg = arg.mid(0, 8)+"%";
        }else{
            match = "nombre";
            arg = arg+"%";
        }
        str_query = "SELECT persona.id";
        str_query += ", naturales.dni";
        str_query += ", naturales.nombre";
        str_query += ", naturales.direccion";
        str_query += " FROM persona";
        str_query += " JOIN naturales ON persona.id = naturales.persona_id";
        str_query += " JOIN cliente_dni ON persona.id = cliente_dni.naturales_persona_id";
        str_query += " WHERE naturales."+match+" LIKE '"+arg+"'";
        str_query += " ORDER BY naturales."+match;
        str_query += " LIMIT " + QString().setNum(pos) + ", " + QString().setNum(size_query) + "";
    }break;
    case persona_items::USUARIO:{
        QString match = "";
        bool ok;
        arg.left(8).toFloat(&ok);
        if(ok){
            match = "naturales.dni";
            arg = arg.mid(0, 8)+"%";
        }else{
            match = "usuario.usuario";
            arg = arg+"%";
        }
        str_query = "SELECT persona.id";
        str_query += ", rol.id";
        str_query += ", rol.rol";
        str_query += ", usuario.usuario";
        str_query += ", naturales.dni";
        str_query += ", naturales.nombre";
        str_query += " FROM persona";
        str_query += " JOIN naturales ON persona.id = naturales.persona_id";
        str_query += " JOIN usuario ON persona.id = usuario.naturales_persona_id";
        str_query += " JOIN rol ON rol.id = usuario.rol_id";
        str_query += " WHERE "+match+" LIKE '"+arg+"'";
        str_query += " ORDER BY "+match;
        str_query += " LIMIT " + QString().setNum(pos) + ", " + QString().setNum(size_query) + "";
    }break;

    }

    QSqlQuery query;
    qDebug() << str_query << endl;
    if (query.exec(str_query)) {
        int rowCount = ui->tableWidget->rowCount();
        ui->tableWidget->setRowCount(rowCount + query.size());

        int columnCount = query.record().count();
        ui->tableWidget->setColumnCount(columnCount);

        switch(tipo)
        {
        case persona_items::PROVEEDOR:{
            ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "ID"
                                                       << "RUC" << "RAZON SOCIAL"
                                                       << "DIRECCION");
            ui->tableWidget->setColumnHidden(0, true);
        }break;
        case persona_items::TRANSPORTISTA:{
            ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "ID"
                                                       << "RUC" << "RAZON SOCIAL"
                                                       << "DIRECCION");
            ui->tableWidget->setColumnHidden(0, true);
        }break;
        case persona_items::CLIENTE_RUC:{
            ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "ID"
                                                       << "RUC" << "RAZON SOCIAL"
                                                       << "DIRECCION");
            ui->tableWidget->setColumnHidden(0, true);
        }break;
        case persona_items::CLIENTE_DNI:{
            ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "ID"
                                                       << "DNI" << "NOMBRE"
                                                       << "DIRECCION");
            ui->tableWidget->setColumnHidden(0, true);
        }break;
        case persona_items::USUARIO:{
            ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "ID"
                                                       << "ID_ROL"
                                                       << "ROL" << "USUARIO"
                                                       << "DNI" << "PASS");
            ui->tableWidget->setColumnHidden(0, true);
            ui->tableWidget->setColumnHidden(1, true);
        }break;

        }        

        while (query.next()) {

            switch(tipo)
            {
            case persona_items::PROVEEDOR:{
                QString id = query.value(0).toString();
                QString ruc = query.value(1).toString();
                QString razon_social = query.value(2).toString();
                QString direccion = query.value(3).toString();
                ui->tableWidget->setItem(pos, 0, new QTableWidgetItem(id));
                ui->tableWidget->setItem(pos, 1, new QTableWidgetItem(ruc));
                ui->tableWidget->setItem(pos, 2, new QTableWidgetItem(razon_social));
                ui->tableWidget->setItem(pos, 3, new QTableWidgetItem(direccion));
            }break;
            case persona_items::TRANSPORTISTA:{
                QString id = query.value(0).toString();
                QString ruc = query.value(1).toString();
                QString razon_social = query.value(2).toString();
                QString direccion = query.value(3).toString();
                ui->tableWidget->setItem(pos, 0, new QTableWidgetItem(id));
                ui->tableWidget->setItem(pos, 1, new QTableWidgetItem(ruc));
                ui->tableWidget->setItem(pos, 2, new QTableWidgetItem(razon_social));
                ui->tableWidget->setItem(pos, 3, new QTableWidgetItem(direccion));
            }break;
            case persona_items::CLIENTE_RUC:{
                QString id = query.value(0).toString();
                QString ruc = query.value(1).toString();
                QString razon_social = query.value(2).toString();
                QString direccion = query.value(3).toString();
                ui->tableWidget->setItem(pos, 0, new QTableWidgetItem(id));
                ui->tableWidget->setItem(pos, 1, new QTableWidgetItem(ruc));
                ui->tableWidget->setItem(pos, 2, new QTableWidgetItem(razon_social));
                ui->tableWidget->setItem(pos, 3, new QTableWidgetItem(direccion));
            }break;
            case persona_items::CLIENTE_DNI:{
                QString id = query.value(0).toString();
                QString dni = query.value(1).toString();
                QString nombre = query.value(2).toString();
                QString direccion = query.value(3).toString();
                ui->tableWidget->setItem(pos, 0, new QTableWidgetItem(id));
                ui->tableWidget->setItem(pos, 1, new QTableWidgetItem(dni));
                ui->tableWidget->setItem(pos, 2, new QTableWidgetItem(nombre));
                ui->tableWidget->setItem(pos, 3, new QTableWidgetItem(direccion));
            }break;
            case persona_items::USUARIO:{
                QString id = query.value(0).toString();
                QString id_rol = query.value(1).toString();
                QString rol = query.value(2).toString();
                QString usuario = query.value(3).toString();
                QString dni = query.value(4).toString();
                QString nombre = query.value(5).toString();
                ui->tableWidget->setItem(pos, 0, new QTableWidgetItem(id));
                ui->tableWidget->setItem(pos, 1, new QTableWidgetItem(id_rol));
                ui->tableWidget->setItem(pos, 2, new QTableWidgetItem(rol));
                ui->tableWidget->setItem(pos, 3, new QTableWidgetItem(usuario));
                ui->tableWidget->setItem(pos, 4, new QTableWidgetItem(dni));
                ui->tableWidget->setItem(pos, 5, new QTableWidgetItem(nombre));
            }break;

            }            
            ++pos;
        }
        SYSTEM->table_resize_to_contents(0, ui->tableWidget, size_query);
    }else{

    }
}

void ColaboradorBuscar::on_pushButton_ok_clicked()
{
    QTableWidget* tb = ui->tableWidget;
    QTableWidgetItem* item = tb->currentItem();

    if(!item)return;

    if(widget_previous){
        int row = item->row();

        switch(tipo)
        {
        case persona_items::PROVEEDOR:{
            id = tb->item(row, 0)->text();
            codigo = tb->item(row, 1)->text();
            nombre = tb->item(row, 2)->text();
            direccion = tb->item(row, 3)->text();
        }break;
        case persona_items::TRANSPORTISTA:{
            id = tb->item(row, 0)->text();
            codigo = tb->item(row, 1)->text();
            nombre = tb->item(row, 2)->text();
            direccion = tb->item(row, 3)->text();
        }break;
        case persona_items::CLIENTE_RUC:{
            id = tb->item(row, 0)->text();
            codigo = tb->item(row, 1)->text();
            nombre = tb->item(row, 2)->text();
            direccion = tb->item(row, 3)->text();
        }break;
        case persona_items::CLIENTE_DNI:{
            id = tb->item(row, 0)->text();
            codigo = tb->item(row, 1)->text();
            nombre = tb->item(row, 2)->text();
            direccion = tb->item(row, 3)->text();
        }break;
        case persona_items::USUARIO:{
            id = tb->item(row, 0)->text();
            codigo = tb->item(row, 4)->text();
            nombre = tb->item(row, 3)->text();
        }break;
        }
        setAttribute(Qt::WA_DeleteOnClose);
        SYSTEM->change_center_w(this, widget_previous);
    }else{
        SYSTEM->clear_center_w(this);
    }
}
void ColaboradorBuscar::on_pushButton_salir_clicked()
{
    //int ret = QMessageBox::warning(this, "Advertencia", "¿Esta seguro que desea salir?", "Si", "No");
    //switch(ret){
    //case 0:{
        if(widget_previous){
            setAttribute(Qt::WA_DeleteOnClose);
            SYSTEM->change_center_w(this, widget_previous);
        }else{
            SYSTEM->clear_center_w(this);
        }
    //}break;
    //case 1:{

    //}
    //}
}
void ColaboradorBuscar::on_pushButton_agregar_clicked()
{
    int ret = QMessageBox::warning(this, "Advertencia", "¿Desea agregar un colaborador?", "Si", "No");
    switch(ret){
    case 0:{
        int i = tipo;
        switch(i)
        {
        case persona_items::PROVEEDOR:{
            ProveedorOp* w = new ProveedorOp;
            w->set_widget_previous(this);
            connect(w, SIGNAL(closing()), this, SLOT(on_colaborador_closing()));
            w->set_captcha();
            SYSTEM->change_center_w(this, w);
        }break;
        case persona_items::TRANSPORTISTA:{
            TransportistaOp* w = new TransportistaOp;
            w->set_widget_previous(this);
            connect(w, SIGNAL(closing()), this, SLOT(on_colaborador_closing()));
            w->set_captcha();
            SYSTEM->change_center_w(this, w);
        }break;
        case persona_items::CLIENTE_RUC:{
            ClienteRUC* w = new ClienteRUC;
            w->set_widget_previous(this);
            connect(w, SIGNAL(closing()), this, SLOT(on_colaborador_closing()));
            w->set_captcha();
            SYSTEM->change_center_w(this, w);
        }break;
        case persona_items::CLIENTE_DNI:{            
            ClienteDNI* w = new ClienteDNI;
            w->set_widget_previous(this);
            connect(w, SIGNAL(closing()), this, SLOT(on_colaborador_closing()));
            w->set_captcha();
            SYSTEM->change_center_w(this, w);            
        }break;
        case persona_items::USUARIO:{
            UsuarioOp* w = new UsuarioOp;
            w->set_widget_previous(this);
            connect(w, SIGNAL(closing()), this, SLOT(on_colaborador_closing()));            
            SYSTEM->change_center_w(this, w);
        }break;

        }
    }break;
    case 1:{

    }break;
    }
}
void ColaboradorBuscar::on_pushButton_modificar_clicked()
{
    QTableWidget* tb = ui->tableWidget;
    QTableWidgetItem* item = tb->currentItem();

    if(!item) return;

    int ret = QMessageBox::warning(this, "Advertencia", "¿Desea modificar colaborador?", "Si", "No");
    switch(ret){
    case 0:{
        int i = tipo;
        switch(i)
        {
        case persona_items::PROVEEDOR:{
            QString id = tb->item(item->row(), 0)->text();
            ProveedorOp* w = new ProveedorOp;
            w->set_widget_previous(this);            
            w->select(id);
            w->set_captcha();
            connect(w, SIGNAL(closing()), this, SLOT(on_colaborador_closing()));
            SYSTEM->change_center_w(this, w);
        }break;
        case persona_items::TRANSPORTISTA:{
            QString id = tb->item(item->row(), 0)->text();
            TransportistaOp* w = new TransportistaOp;
            w->set_widget_previous(this);
            w->select(id);
            w->set_captcha();
            connect(w, SIGNAL(closing()), this, SLOT(on_colaborador_closing()));
            SYSTEM->change_center_w(this, w);
        }break;
        case persona_items::CLIENTE_RUC:{
            QString id = tb->item(item->row(), 0)->text();
            ClienteRUC* w = new ClienteRUC;
            w->set_widget_previous(this);
            w->select(id);
            w->set_captcha();
            connect(w, SIGNAL(closing()), this, SLOT(on_colaborador_closing()));
            SYSTEM->change_center_w(this, w);
        }break;
        case persona_items::CLIENTE_DNI:{
            QString id = tb->item(item->row(), 0)->text();
            ClienteDNI* w = new ClienteDNI;
            w->set_widget_previous(this);
            w->select(id);
            w->set_captcha();
            connect(w, SIGNAL(closing()), this, SLOT(on_colaborador_closing()));
            SYSTEM->change_center_w(this, w);
        }break;
        case persona_items::USUARIO:{
            QString id = tb->item(item->row(), 0)->text();
            UsuarioOp* w = new UsuarioOp;
            w->set_widget_previous(this);
            w->select(id);
            connect(w, SIGNAL(closing()), this, SLOT(on_colaborador_closing()));
            SYSTEM->change_center_w(this, w);
        }break;

        }
    }break;
    case 1:{

    }break;
    }
}
void ColaboradorBuscar::showEvent(QShowEvent *se)
{
    se->accept();

    ui->lineEdit_buscar->setFocus(Qt::TabFocusReason);
}

void ColaboradorBuscar::hideEvent(QHideEvent* he)
{
    he->accept();
}
void ColaboradorBuscar::closeEvent(QCloseEvent* ce)
{
    ce->accept();
}
bool ColaboradorBuscar::eventFilter(QObject *obj, QEvent *e)
{
    QWidget* w_temp;
    w_temp = this;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Escape:{
                ui->pushButton_salir->click();
                return true;
            }break;
            case Qt::Key_Up: {
                if (this->focusWidget() != ui->tableWidget) {
                    ui->tableWidget->setFocus(Qt::TabFocusReason);
                    if (ui->tableWidget->currentItem())
                        ui->tableWidget->currentItem()->setSelected(true);
                }
            }break;
            case Qt::Key_Down: {
                if (this->focusWidget() != ui->tableWidget) {
                    ui->tableWidget->setFocus(Qt::TabFocusReason);
                    if (ui->tableWidget->currentItem())
                        ui->tableWidget->currentItem()->setSelected(true);
                }
            }break;
            case Qt::Key_F3:{
                ui->pushButton_agregar->setFocus(Qt::TabFocusReason);
                ui->pushButton_agregar->click();
                return true;
            }break;
            case Qt::Key_F4:{
                ui->pushButton_modificar->setFocus(Qt::TabFocusReason);
                ui->pushButton_modificar->click();
                return true;
            }break;
            }

        }else{

        }
        return false;
    }
    w_temp = ui->lineEdit_buscar;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {            
            case Qt::Key_Return:{

            }break;
            }

        }else{

        }
        return false;
    }
    w_temp = ui->tableWidget;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:
                ui->pushButton_ok->click();
                return true;
            }

        }else{

        }
        return false;
    }
    w_temp = ui->pushButton_ok;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:
                ui->pushButton_ok->click();
                return true;
            }

        }else{

        }
        return false;
    }
    w_temp = ui->pushButton_salir;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Tab:{
                this->setFocus();
            }break;
            case Qt::Key_Return:{
                ui->pushButton_salir->click();
                return true;
            }break;
            }

        }else{

        }
        return false;
    }
    w_temp = ui->pushButton_agregar;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:{
                ui->pushButton_agregar->click();
                return true;
            }break;
            }

        }else{

        }
        return false;
    }
    w_temp = ui->pushButton_modificar;
    if(obj == w_temp){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;

            switch(KeyEvent->key())
            {            
            case Qt::Key_Return:{
                ui->pushButton_modificar->click();
                return true;
            }break;
            }

        }else{

        }
        return false;
    }
    return eventFilter(obj, e);
}
