#include "toolbar.h"
#include "ui_toolbar.h"

ToolBar::ToolBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ToolBar)
{
    ui->setupUi(this);

    afterShow = false;

    curr_widget = NULL;

    /*
    ui->toolButton_comprobantes->installEventFilter(this);
    ui->toolButton_productos->installEventFilter(this);
    ui->toolButton_personas->installEventFilter(this);
    ui->toolButton_reportes->installEventFilter(this);
    ui->toolButton_izquierda->installEventFilter(this);
    ui->toolButton_derecha->installEventFilter(this);
    ui->toolButton_configuracion->installEventFilter(this);
    */
}

ToolBar::~ToolBar()
{
    for(int i = 0; i < widgets.length(); i++){
        if(widgets[i])
            delete widgets[i];
    }
    qDebug()<<"TOOLBAR DELETE"<<endl;
    delete ui;
}

QToolButton* ToolBar::tb_comprobantes(){return ui->toolButton_comprobantes; }
QToolButton* ToolBar::tb_productos(){ return ui->toolButton_productos; }
QToolButton* ToolBar::tb_personas(){ return ui->toolButton_personas; }
QToolButton* ToolBar::tb_reportes(){ return ui->toolButton_reportes; }
QToolButton* ToolBar::tb_izquierda(){ return ui->toolButton_izquierda; }
QToolButton* ToolBar::tb_derecha(){ return ui->toolButton_derecha; }
QToolButton* ToolBar::tb_configuracion(){ return ui->toolButton_configuracion; }

void ToolBar::on_toolButton_comprobantes_clicked()
{
    Comprobante* w = new Comprobante;
    w->setAttribute(Qt::WA_DeleteOnClose);
    w->show();
    w->showMaximized();
    w->raise();
    /*
    ui->toolButton_comprobantes->setFocus();

    // memory background-color: rgb(151, 192, 36);
    QWidget* w = SYSTEM->set_center_w(new Comprobante, "background-color: rgb(211, 235, 148);");
    push_center_w(w);
    APP_MAINWINDOW->setCentralWidget(w);
    */
}

void ToolBar::on_toolButton_productos_clicked()
{
    /*
    ui->toolButton_productos->setFocus();

    // memory background-color: rgb(228, 193, 0);
    QWidget* w = SYSTEM->set_center_w(new Producto, "background-color: rgb(255, 236, 128);");
    push_center_w(w);
    APP_MAINWINDOW->setCentralWidget(w);
    */
}

void ToolBar::on_toolButton_personas_clicked()
{
    /*
    ui->toolButton_personas->setFocus();

    // memory background-color: rgb(0, 51, 153);
    QWidget* w = SYSTEM->set_center_w(new Persona, "background-color: rgb(51, 119, 255);");
    push_center_w(w);
    APP_MAINWINDOW->setCentralWidget(w);
    */
}
void ToolBar::on_toolButton_reportes_clicked()
{
    ComprobanteBuscar* w = new ComprobanteBuscar;
    //w->setWindowFlag(Qt::Dialog);
    w->setAttribute(Qt::WA_DeleteOnClose);
    //w->setWindowModality(Qt::WindowModal);

    //connect(cliente, SIGNAL(closing()), this, SLOT(on_cliente_closing()));

    w->show();
    /*
    ui->toolButton_reportes->setFocus();

    // memory background-color: rgb(0, 151, 68);
    QWidget* w = SYSTEM->set_center_w(new A_Reporte, "background-color: rgb(51, 255, 143)");
    push_center_w(w);
    APP_MAINWINDOW->setCentralWidget(w);
    */
}

void ToolBar::on_toolButton_configuracion_clicked()
{
    ui->toolButton_configuracion->setFocus();

    // memory background-color: rgb(110, 110, 110);
    QWidget* w = SYSTEM->set_center_w(new Configuracion, "background-color: rgb(191, 191, 191);");
    push_center_w(w);
    APP_MAINWINDOW->setCentralWidget(w);
}

void ToolBar::push_center_w(QWidget* center)
{
    curr_widget = center;
    if(curr_widget){
        widgets.push_back(curr_widget);
        if(widgets.count() > 5){
            QWidget* delete_w = widgets.first();
            widgets.removeFirst();
            delete delete_w;
        }
    }
    QWidget* centralw = APP_MAINWINDOW->centralWidget();

    centralw->hide();
    APP_MAINWINDOW->centralWidget()->setParent(0);

}

void ToolBar::showEvent(QShowEvent *se)
{
    se->accept();

    afterShow = true;
}
bool ToolBar::eventFilter(QObject *watched, QEvent *event)
{
    return eventFilter(watched, event);
}

void ToolBar::on_toolButton_izquierda_clicked()
{
    if(curr_widget){
        int indexOf = widgets.indexOf(curr_widget) - 1;
        if(indexOf >= 0) {
            QWidget* w = widgets[indexOf];
            curr_widget->hide();
            curr_widget = w;
            APP_MAINWINDOW->centralWidget()->setParent(0);
            APP_MAINWINDOW->setCentralWidget(w);
            w->show();
        }
    }
}

void ToolBar::on_toolButton_derecha_clicked()
{
    if(curr_widget){
        int indexOf = widgets.indexOf(curr_widget) + 1;
        if(indexOf >= 0 && indexOf < widgets.count()) {
            QWidget* w = widgets[indexOf];
            curr_widget->hide();
            curr_widget = w;
            APP_MAINWINDOW->centralWidget()->setParent(0);
            APP_MAINWINDOW->setCentralWidget(w);
            w->show();
        }
    }
}
