#ifndef TOOLBAR_H
#define TOOLBAR_H

#include "locallibrary.h"
#include <QProcess>

namespace Ui {
class ToolBar;
}

class Q_DECL_EXPORT ToolBar : public QWidget
{
	Q_OBJECT

public:
	explicit ToolBar(QWidget *parent = 0);
	~ToolBar();

	QToolButton* tb_home();
	QToolButton* tb_ventas();
	QToolButton* tb_compras();
	QToolButton* tb_productos();
	QToolButton* tb_colaboradores();
	QToolButton* tb_reportes();
	QToolButton* tb_configuracion();

	void set_labels_ssheet(int pos);

private:
	void showEvent(QShowEvent *se);

	bool eventFilter(QObject *obj, QEvent *e);

public slots:	
    void on_toolButton_home_clicked();

    void on_toolButton_ventas_clicked();

    void on_toolButton_compras_clicked();

    void on_toolButton_reportes_clicked();

    void on_toolButton_productos_clicked();

    void on_toolButton_colaboradores_clicked();

    void on_toolButton_configuracion_clicked();	
	
    void on_toolButton_salvados_clicked();

private:
    Ui::ToolBar *ui;

    bool afterShow;

    QLabel* cur_label;
};

template class Q_DECL_EXPORT singleton<ToolBar>;

#endif // TOOLBAR_H
