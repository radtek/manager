#ifndef PRODUCTOMENU_H
#define PRODUCTOMENU_H

#include <QWidget>

namespace Ui {
class ProductoMenu;
}

class ProductoMenu : public QWidget
{
    Q_OBJECT

public:
    explicit ProductoMenu(QWidget *parent = 0);
    ~ProductoMenu();

private:
    Ui::ProductoMenu *ui;
};

#endif // PRODUCTOMENU_H
