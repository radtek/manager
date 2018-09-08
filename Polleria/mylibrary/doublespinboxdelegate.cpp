#include "doublespinboxdelegate.h"

DoubleSpinBoxDelegate::DoubleSpinBoxDelegate(QObject *parent) :
    QItemDelegate(parent)
{
    minimum = 0;
    maximum = 999999999;
    decimals = 2;
    singleStep = 1;

    value = 0;
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

    editor->setFrame(false);
    editor->setMinimum(minimum);
    editor->setMaximum(maximum);

    editor->setDecimals(decimals);
    editor->setSingleStep(singleStep);
    editor->setValue(value);

    qDebug()<<"Create Editor: "<<editor->value()<<endl;

    return editor;
}

void DoubleSpinBoxDelegate::setEditorData(QWidget *editor,
                                     const QModelIndex &index) const
{        
    qDebug()<<"Editor data: "<<index.data(Qt::EditRole).toString()<<endl;
    double value = index.data(Qt::EditRole).toString().toDouble();
    qDebug()<<value<<endl;

    QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
    spinBox->setValue(value);
}

void DoubleSpinBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                    const QModelIndex &index)
{
    QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
    spinBox->interpretText();

    double value = spinBox->value();
    qDebug()<<"Model data: "<<endl;
    qDebug()<<value<<endl;

    model->setData(index, value, Qt::EditRole);    
}

void DoubleSpinBoxDelegate::updateEditorGeometry(QWidget *editor,
     const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}
