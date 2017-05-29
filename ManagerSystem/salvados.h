#ifndef SALVADOS_H
#define SALVADOS_H

#include <QWidget>

namespace Ui {
class Salvados;
}

class Salvados : public QWidget
{
    Q_OBJECT

public:
    explicit Salvados(QWidget *parent = 0);
    ~Salvados();

private:
    Ui::Salvados *ui;
};

#endif // SALVADOS_H
