#ifndef MANAGERSYSTEM_H
#define MANAGERSYSTEM_H

#include "mylibrary_global.h"
#include "shares.h"
#include <QTimer>

namespace Ui {
class ManagerSystem;
}

class MYLIBRARYSHARED_EXPORT ManagerSystem : public QMainWindow
{
    Q_OBJECT

public:
    explicit ManagerSystem(QWidget *parent = 0);
    ~ManagerSystem();

    void execReportes();

    void set_toolBar(QWidget* w);

    void showToolBar();
    void hideToolBar();

public slots:
    void on_shortCut_salir();

    void on_application_state_changed(Qt::ApplicationState state);

    void on_timer_timeout();

protected:
    void focusInEvent(QFocusEvent *fe);

    void focusOutEvent(QFocusEvent *fe);


    void showEvent(QShowEvent *se);

    void closeEvent(QCloseEvent* ce);

private slots:
    void on_actionPor_Producto_triggered();

private:
    bool eventFilter(QObject *, QEvent *);

private:
    Ui::ManagerSystem *ui;

    bool afterShow;

    QShortcut* shortCut_salir;

    QTimer* timer;
};

template class Q_DECL_EXPORT singleton<ManagerSystem>;

#endif // MANAGERSYSTEM_H
