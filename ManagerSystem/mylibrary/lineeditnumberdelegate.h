#ifndef LINEEDITNUMBERDELEGATE_H
#define LINEEDITNUMBERDELEGATE_H

#include "mylibrary_global.h"

class WidgetNumber;
class MYLIBRARYSHARED_EXPORT LineEditNumberDelegate : public QItemDelegate
{
public:
    explicit LineEditNumberDelegate(QObject *parent = 0);
    
    void setMinimumWidth(int width);
    void setMinimumHeight(int height);
    void setMaximumWidth(int width);
    void setMaximumHeight(int height);

    void setDecimals(int decimals);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                               const QModelIndex &index) const;

    void paint(QPainter * painter, const QStyleOptionViewItem & option
                              , const QModelIndex & index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                    const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &index) const;    

signals:

private slots:
    void commitAndCloseEditor();

private:
    int minimumWidth;
    int minimumHeight;
    int maximumWidth;
    int maximumHeight;
    int decimals;    
};

#endif // LINEEDITNUMBERDELEGATE_H
