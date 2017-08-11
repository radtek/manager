#ifndef TABLEMOVES_H
#define TABLEMOVES_H

#include <QWidget>

namespace Ui {
class TableMoves;
}

class TableMoves : public QWidget
{
    Q_OBJECT

public:
    explicit TableMoves(QWidget *parent = 0);
    ~TableMoves();

private:
    Ui::TableMoves *ui;
};

#endif // TABLEMOVES_H
