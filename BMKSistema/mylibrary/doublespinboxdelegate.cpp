#include "doublespinboxdelegate.h"

DoubleSpinBoxDelegate::DoubleSpinBoxDelegate(QObject *parent) :
    QItemDelegate(parent)
{
    minimum = 0;
    maximum = 999999999;
    decimals = 2;
}
void DoubleSpinBoxDelegate::setMinimum(int value)
{
    minimum = value;
}
void DoubleSpinBoxDelegate::setMaximum(int value)
{
    maximum = value;
}
void DoubleSpinBoxDelegate::setDecimals(int value)
{
    decimals = value;
}
void DoubleSpinBoxDelegate::setSingleStep(double singleStep)
{
    this->singleStep = singleStep;
}

QWidget *DoubleSpinBoxDelegate::createEditor(QWidget *parent,
     const QStyleOptionViewItem & option,
     const QModelIndex & index ) const
{
    QDoubleSpinBox *editor = new QDoubleSpinBox(parent);

    editor->setMinimum(minimum);
    editor->setMaximum(maximum);

    editor->setDecimals(decimals);
    editor->setSingleStep(singleStep);

    return editor;
}
void DoubleSpinBoxDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option
                                  , const QModelIndex &index) const
{

    //if (index.column() == durationColumn) {
            QString value = index.model()->data(index, Qt::DisplayRole).toString();

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
void DoubleSpinBoxDelegate::setEditorData(QWidget *editor,
                                     const QModelIndex &index) const
{
    double value = index.model()->data(index, Qt::EditRole).toDouble();

    QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
    spinBox->setValue(value);

}

void DoubleSpinBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                    const QModelIndex &index) const
{
    QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
    spinBox->interpretText();
    QString value = spinBox->text();

    model->setData(index, value, Qt::EditRole);    
}

void DoubleSpinBoxDelegate::updateEditorGeometry(QWidget *editor,
     const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}
