#ifndef MANAGERSYSTEM_H
#define MANAGERSYSTEM_H

#include "locallibrary.h"

namespace Ui {
class ManagerSystem;
}

class Q_DECL_EXPORT ManagerSystem : public QMainWindow
{
    Q_OBJECT

public:
    explicit ManagerSystem(QWidget *parent = 0);
    ~ManagerSystem();

    void set_toolBar(QWidget* w);

    void showToolBar();
    void hideToolBar();

public slots:
    void on_shortCut_salir();

    void on_application_state_changed(Qt::ApplicationState state);

protected:
    void focusInEvent(QFocusEvent *fe);

    void focusOutEvent(QFocusEvent *fe);


    void showEvent(QShowEvent *se);

    void closeEvent(QCloseEvent* ce);

private:
    bool eventFilter(QObject *, QEvent *);

private:
    Ui::ManagerSystem *ui;

    bool afterShow;

    QShortcut* shortCut_salir;

};

template class Q_DECL_EXPORT singleton<ManagerSystem>;

#endif // MANAGERSYSTEM_H
