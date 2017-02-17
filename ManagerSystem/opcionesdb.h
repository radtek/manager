#ifndef OPCIONESDB_H
#define OPCIONESDB_H

#include "locallibrary.h"

namespace Ui {
class OpcionesDB;
}

class OpcionesDB : public QWidget
{
    Q_OBJECT

public:
    explicit OpcionesDB(QWidget *parent = 0);
    ~OpcionesDB();

private slots:
    void on_pushButton_guardar_clicked();

private:
    Ui::OpcionesDB *ui;
};

#endif // OPCIONESDB_H
