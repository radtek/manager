#ifndef DOUBLESPINBOXDELEGATE_H
#define DOUBLESPINBOXDELEGATE_H

#include "mylibrary_global.h"

class MYLIBRARYSHARED_EXPORT DoubleSpinBoxDelegate : public QItemDelegate
{
	Q_OBJECT
public:
    explicit DoubleSpinBoxDelegate(QObject *parent = 0);
    
    void setMinimum(int value);
    void setMaximum(int value);

    void setDecimals(int value);

    void setSingleStep(double singleStep);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                               const QModelIndex &index) const;


    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                    const QModelIndex &index);

    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index) const;
signals:

public slots:
    
private:
    int minimum;
    int maximum;
    int decimals;

    double singleStep;

    double value;
};

#endif // DOUBLESPINBOXDELEGATE_H
