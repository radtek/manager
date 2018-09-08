#ifndef USUARIO_H
#define USUARIO_H

#include <QWidget>

namespace Ui {
class Usuario;
}

class Usuario : public QWidget
{
    Q_OBJECT

public:
    explicit Usuario(QWidget *parent = 0);
    ~Usuario();

private:
    Ui::Usuario *ui;
};

#endif // USUARIO_H
