#ifndef HOME_H
#define HOME_H

#include "locallibrary.h"

namespace Ui {
class Home;
}

class Home : public QWidget
{
    Q_OBJECT

public:
    explicit Home(QWidget *parent = 0);
    ~Home();

    QToolButton* tb_ventas();
    QToolButton* tb_compras();
    QToolButton* tb_reportes();
    QToolButton* tb_productos();
    QToolButton* tb_personas();
    QToolButton* tb_configuracion();

    void set_toolbar(QWidget* toolbar);

    void setFocus_ini();

protected:
    void showEvent(QShowEvent *se);

    bool eventFilter(QObject *obj, QEvent *e);

private slots:    
    void on_toolButton_ventas_clicked();

    void on_toolButton_compras_clicked();

    void on_toolButton_productos_clicked();

    void on_toolButton_colaboradores_clicked();

    void on_toolButton_reportes_clicked();

    void on_toolButton_configuracion_clicked();

    void on_pushButton_clicked();    

private:
    Ui::Home *ui;

    QWidget* mainw;
    QWidget* toolbar;
};

#endif // HOME_H
