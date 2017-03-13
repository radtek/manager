#ifndef LINEEDIT_H
#define LINEEDIT_H

#include <QWidget>
#include <QLineEdit>
#include "mylibrary_global.h"

class MYLIBRARYSHARED_EXPORT LineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit LineEdit(QWidget* parent = 0);
    explicit LineEdit(const QString& str, QWidget *parent);

public slots:
    void textChanged(const QString& arg);
    void editingFinished();

protected:


};

#endif // LINEEDIT_H
