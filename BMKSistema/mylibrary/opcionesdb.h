#ifndef OPCIONESDB_H
#define OPCIONESDB_H

#include "mylibrary_global.h"
#include "database_connection.h"

namespace Ui {
class OpcionesDB;
}

class MYLIBRARYSHARED_EXPORT OpcionesDB : public QWidget
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
