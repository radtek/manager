#include "lineeditdelegate.h"

LineEditDelegate::LineEditDelegate(QObject *parent) :
    QItemDelegate(parent)
{

}

QWidget *LineEditDelegate::createEditor(QWidget *parent,
     const QStyleOptionViewItem & option,
     const QModelIndex & index ) const
{
    QLineEdit *editor = new QLineEdit(parent);

    return editor;
}
void LineEditDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option
                                  , const QModelIndex &index) const
{

    //if (index.column() == durationColumn) {
            //QString value = index.model()->data(index, Qt::DisplayRole).toString();

            QStyleOptionViewItem myOption = option;
            myOption.displayAlignment = Qt::AlignRight | Qt::AlignVCenter;
            myOption.state = QStyle::State_Enabled | QStyle::State_Raised
                    | QStyle::State_Editing;

            //drawDisplay(painter, myOption, myOption.rect, text);
            //drawFocus(painter, myOption, myOption.rect);

        //} else{
            QItemDelegate::paint(painter, option, index);
        //}

}
void LineEditDelegate::setEditorData(QWidget *editor,
                                     const QModelIndex &index) const
{
    QString arg = index.model()->data(index, Qt::EditRole).toString();

    QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
    lineEdit->setText(arg);
    lineEdit->clearFocus();
}

void LineEditDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                    const QModelIndex &index) const
{
    QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
    QString str = lineEdit->text();

    model->setData(index, str, Qt::EditRole);
    lineEdit->clearFocus();
}

void LineEditDelegate::updateEditorGeometry(QWidget *editor,
     const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}

