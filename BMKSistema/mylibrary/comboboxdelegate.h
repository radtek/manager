#ifndef COMBOBOXDELEGATE_H
#define COMBOBOXDELEGATE_H

#include "mylibrary_global.h"

class MYLIBRARYSHARED_EXPORT ComboBoxDelegate : public QItemDelegate
{    
	Q_OBJECT
public:
    ComboBoxDelegate(QObject *parent = 0);

    void set_items(QVector<QString> v);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    QVector<QString> Items;

    QComboBox* cb;

public:
    bool hasPaint;
};
#endif
