#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <String>
#include <QMessageBox>
#include <QDebug>
#include <cppscript.h>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();



private slots:
    void on_btn_connect_clicked();

private:
    Ui::MainWindow *ui;

    //Conexion a BD


};

#endif // MAINWINDOW_H
