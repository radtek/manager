#ifndef LINEEDITDELEGATE_H
#define LINEEDITDELEGATE_H

#include "mylibrary_global.h"

class MYLIBRARYSHARED_EXPORT LineEditDelegate : public QItemDelegate
{
	Q_OBJECT
public:
    explicit LineEditDelegate(QObject *parent = 0);

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

public slots:

private:

};

#endif // LINEEDITDELEGATE_H
