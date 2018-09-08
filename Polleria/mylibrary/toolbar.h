#ifndef TOOLBAR_H
#define TOOLBAR_H

#include "mylibrary_global.h"
#include "modules_library.h"
#include "managersystem.h"
#include "comprobante/comprobantebuscar.h"
#include "persona/personamenu.h"
#include "producto/productomenu.h"

namespace Ui {
class ToolBar;
}

class MYLIBRARYSHARED_EXPORT ToolBar : public QWidget
{
	Q_OBJECT

public:
	explicit ToolBar(QWidget *parent = 0);
	~ToolBar();

    QToolButton* tb_comprobantes();
    QToolButton* tb_productos();
    QToolButton* tb_personas();
	QToolButton* tb_reportes();
    QToolButton* tb_izquierda();
    QToolButton* tb_derecha();
	QToolButton* tb_configuracion();

private:
    void push_center_w(QWidget* center);

	void showEvent(QShowEvent *se);

    bool eventFilter(QObject *watched, QEvent *event);

public slots:	
    void on_toolButton_comprobantes_clicked();

    void on_toolButton_productos_clicked();

    void on_toolButton_personas_clicked();

    void on_toolButton_reportes_clicked();

    void on_toolButton_configuracion_clicked();	

private slots:
    void on_toolButton_izquierda_clicked();

    void on_toolButton_derecha_clicked();

private:
    Ui::ToolBar *ui;

    bool afterShow;

    QWidget* curr_widget;
    QVector<QWidget*> widgets;
};

template class Q_DECL_EXPORT singleton<ToolBar>;

#endif // TOOLBAR_H
