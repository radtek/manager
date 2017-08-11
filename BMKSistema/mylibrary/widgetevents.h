#ifndef WIDGETEVENTS_H
#define WIDGETEVENTS_H

#include "mylibrary_global.h"

class MYLIBRARYSHARED_EXPORT WidgetEvents : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetEvents(QWidget *parent = nullptr);

    void set_widget_previous(QWidget* widget_previous);

    void set_pushButton_salir(QPushButton* pushButton_salir);    

signals:
    void closing();

public slots:

protected:
    void showEvent(QShowEvent *event);

    bool eventFilter(QObject *watched, QEvent *event);

private:
    bool afterShow;

    QWidget* widget_previous;

    QPushButton* pushButton_salir;
};

#endif // WIDGETEVENTS_H
