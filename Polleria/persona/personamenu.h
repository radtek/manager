#ifndef PERSONAMENU_H
#define PERSONAMENU_H

#include "mylibrary/share_static_include.h"
#include "persona.h"
#include "personabuscar.h"

namespace Ui {
class PersonaMenu;
}

class PersonaMenu : public QWidget
{
    Q_OBJECT

public:
    explicit PersonaMenu(QWidget *parent = 0);
    ~PersonaMenu();

private slots:
    void on_lw_tipo_itemClicked(QListWidgetItem* item);

    void on_treeWidget_menu_itemClicked(QTreeWidgetItem *item, int column);

private:
    Ui::PersonaMenu *ui;        
};

#endif // PERSONAMENU_H
