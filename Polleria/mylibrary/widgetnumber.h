#ifndef WIDGETNUMBER_H
#define WIDGETNUMBER_H

#include "mylibrary_global.h"


class MYLIBRARYSHARED_EXPORT WidgetNumber : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetNumber(QWidget *parent = 0);
    ~WidgetNumber();

    void _setMinimumWidth(int width);
    void _setMinimumHeight(int height);
    void _setMaximumWidth(int width);
    void _setMaximumHeight(int height);

    void setDecimals(int decimals);

    void selectAll();

public slots:
    void setText(const QString& text);

public:
    QString text();

private slots:
    void textChanged(const QString& arg);
    void editingFinished();

signals:

public slots:
    bool eventFilter(QObject *watched, QEvent *event);

public:
    QVBoxLayout* box;
    QLineEdit* lineEdit;

    int decimals;
};

#endif // WIDGETNUMBER_H
