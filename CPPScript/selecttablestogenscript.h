#ifndef SELECTTABLESTOGENSCRIPT_H
#define SELECTTABLESTOGENSCRIPT_H

#include <QWidget>

namespace Ui {
class SelectTablesToGenScript;
}

class SelectTablesToGenScript : public QWidget
{
    Q_OBJECT
    
public:
    explicit SelectTablesToGenScript(QWidget *parent = 0);
    ~SelectTablesToGenScript();
    
private slots:
    void on_pushButton_selectAll_clicked();

    void on_pushButton_generate_clicked();

private:
    Ui::SelectTablesToGenScript *ui;
};

#endif // SELECTTABLESTOGENSCRIPT_H
