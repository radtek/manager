#include "managersystem.h"
#include "ui_managersystem.h"

ManagerSystem::ManagerSystem(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ManagerSystem)
{
    ui->setupUi(this);

    //ui->statusBar->addWidget(new SnackBarInfo);
    afterShow = false;


    //QThread* thread = new QThread;
    //connect(thread, SIGNAL (started()), worker, SLOT (process()));

    /*
    QWebEngineView view_consultaRuc;
    view_consultaRuc.load(tr("http://www.sunat.gob.pe/cl-ti-itmrconsruc/jcrS00Alias"));
    QWebEngineView view_consultaDNI;
    view_consultaDNI.load(tr("https://cel.reniec.gob.pe/valreg/valreg.do?accion=ini"));
    */

    shortCut_salir = new QShortcut(QKeySequence(Qt::Key_Alt+Qt::Key_F4), this);
    shortCut_salir->setEnabled(true);
    connect(shortCut_salir, SIGNAL(activated()), this, SLOT(on_shortCut_salir()));
    connect((QGuiApplication*)QGuiApplication::instance(), SIGNAL(applicationStateChanged(Qt::ApplicationState))
            , this, SLOT(on_application_state_changed(Qt::ApplicationState)));

    this->installEventFilter(this);
}

ManagerSystem::~ManagerSystem()
{
    delete SYSTEM;
    delete DATABASE;
    delete shortCut_salir;
    delete ui;
}

void ManagerSystem::execReportes()
{
    ui->menuReportes->showTearOffMenu();
}

void ManagerSystem::set_toolBar(QWidget* w)
{
    //ui->mainToolBar->hide();
    ui->mainToolBar->addWidget(w);
    ui->mainToolBar->adjustSize();
    ui->mainToolBar->setFloatable(false);
    ui->mainToolBar->setAllowedAreas(Qt::TopToolBarArea
                                 //|Qt::RightToolBarArea
                                 //|Qt::LeftToolBarArea
                                 |Qt::BottomToolBarArea);

}
void ManagerSystem::showToolBar()
{
    ui->mainToolBar->show();
}
void ManagerSystem::hideToolBar()
{
    ui->mainToolBar->hide();
}
void ManagerSystem::on_shortCut_salir()
{
    this->close();
}

void ManagerSystem::on_application_state_changed(Qt::ApplicationState state)
{
    /*
    if(state == Qt::ApplicationSuspended){
        //qDebug()<<"application suspended"<<endl;
    }
    if(state == Qt::ApplicationHidden){
        //qDebug()<<"application hidden"<<endl;
    }
    if(state == Qt::ApplicationInactive){
        //qDebug()<<"application inactive"<<endl;
        if(QApplication::focusWidget()){
            //qDebug()<<typeid(*QApplication::focusWidget()).name()<<endl;
            if(QString(typeid(*QApplication::focusWidget()).name()) == "18QExpandingLineEdit"){
                //qDebug()<<"QTableWidget"<<endl;
                //qDebug()<<typeid(*QApplication::focusWidget()->parentWidget()).name()<<endl;
                QApplication::focusWidget()->parentWidget()->setFocus();
            }
            /*
            if(typeid(*QApplication::focusWidget()) == typeid(QTableWidget)){
                qDebug()<<"QTableWidget"<<endl;
                QApplication::focusWidget()->parentWidget()->setFocus();
            */
        //}
    //}
/*
    if(state == Qt::ApplicationActive){
        //qDebug()<<"application active"<<endl;

        /*
        if(QApplication::focusWidget()){
            if(typeid(*QApplication::focusWidget()) == typeid(QTableWidget)){
                QApplication::focusWidget()->setFocus(Qt::TabFocusReason);
            }
        }*/
    //}
}

void ManagerSystem::focusInEvent(QFocusEvent *fe)
{

}
void ManagerSystem::focusOutEvent(QFocusEvent *fe)
{

}

void ManagerSystem::showEvent(QShowEvent *se)
{
    se->accept();

    afterShow = true;
}

void ManagerSystem::closeEvent(QCloseEvent *ce)
{

    if(centralWidget()){
        int ret = QMessageBox::warning(this, "ManagerSystem", "¿Esta seguro que desea salir del sistema?", "Aceptar", "Salir");
        switch(ret){
        case 0:{
            QApplication::closeAllWindows();
            ce->accept();
        }break;
        case 1:{
            ce->ignore();
        }break;
        }
    }else{
        QApplication::closeAllWindows();
        ce->accept();
    }

}

bool ManagerSystem::eventFilter(QObject *obj, QEvent *e)
{

    if(e->type() == QEvent::Paint){
        if(afterShow) {

            afterShow = false;
        }
        return true;
    }

    if (e->type() == QEvent::KeyPress){
        if (obj == this){
            QKeyEvent *ke = static_cast<QKeyEvent*>(e);

            if (ke->key()== Qt::Key_F1){
                if(ke->isAutoRepeat()) {
                    ke->ignore();
                    return QWidget::eventFilter(obj, e);
                } else {
                    ke->accept();
                }

                return false;
            }
            /*
            if (ke->key()== Qt::Key_Escape){
                this->close();
                return false;
            }*/
        }
    }
    return QWidget::eventFilter(obj, e);
}


void ManagerSystem::on_actionPor_Producto_triggered()
{
    ComprobanteBuscar* w = new ComprobanteBuscar;
    w->setWindowFlag(Qt::Dialog);
    w->setAttribute(Qt::WA_DeleteOnClose);

    w->showMaximized();
    w->raise();
}
