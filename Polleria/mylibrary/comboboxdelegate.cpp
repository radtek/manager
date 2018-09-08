#include "comboboxdelegate.h"

ComboBoxDelegate::ComboBoxDelegate(QObject *parent)
:QItemDelegate(parent)
{
    Items.push_back("Test0");
    Items.push_back("Test1");

    hasPaint = false;
}

void ComboBoxDelegate::set_items(QVector<QString> v)
{
    Items = v;
}

QWidget *ComboBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem & option , const QModelIndex & index ) const
{
    QComboBox* editor = new QComboBox(parent);
    for(unsigned int i = 0; i < Items.size(); ++i)
    {
        editor->addItem(Items[i]);
    }

    editor->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    editor->setStyleSheet("background-color: rgb(255, 255, 255);");
    qDebug()<<"create editor"<<endl;
    return editor;
}

void ComboBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    //int value = index.model()->data(index, Qt::EditRole).toInt();
    //qDebug()<<"value: "<<value<<endl;
    QString str = index.model()->data(index, Qt::EditRole).toString();
    comboBox->setCurrentText(str);
}

void ComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *comboBox = static_cast<QComboBox*>(editor);

    //qDebug()<<"model data: "<<comboBox->currentIndex()<<endl;
    model->setData(index, comboBox->currentText(), Qt::EditRole);
}

void ComboBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    qDebug()<<"update geometry"<<endl;
    int w = editor->width();
    int h = editor->height();
    //option.rect = QRect(QPoint(0, 0), QPoint(w, h));
    //editor->setGeometry(QRect(QPoint(option.rect.x(), option.rect.y()), QPoint(w, h)));
    editor->setGeometry(option.rect);
}

void ComboBoxDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem myOption = option;
    //initStyleOption(&opt, index);

    QWidget *widget = (QWidget*)option.widget;
    QString text;
    if(widget){
        text = index.data(Qt::EditRole).toString();
    }else{
        widget = new QComboBox;
        widget->show();
        text = Items[0];
    }
    /*
    QStyle *style = widget ? widget->style() : QApplication::style();


    if (option.state & QStyle::State_Selected)
    {
        // Whitee pen while selection
        painter->setPen(Qt::white);
        painter->setBrush(option.palette.highlightedText());
        // This call will take care to draw, dashed line while selecting
        style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, widget);
    }
    else
    {
        painter->setPen(QPen(option.palette.foreground(), 0));
        painter->setBrush(qvariant_cast<QBrush>(index.data(Qt::ForegroundRole)));
    }

    painter->drawText(option.rect, qvariant_cast<int>(index.data(Qt::TextAlignmentRole)), str);
    */

    //painter->drawText();
    drawBackground(painter, myOption, index);
    drawDisplay(painter, myOption, myOption.rect, text);
    drawFocus(painter, myOption, myOption.rect);
    //painter->save();
    //QItemDelegate::paint(painter, myOption, index);
    //QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &myOption, painter);

}
