#include "lineeditnumberdelegate.h"

LineEditNumberDelegate::LineEditNumberDelegate(QObject *parent) :
    QItemDelegate(parent)
{
    minimumWidth = -1;
    minimumHeight = -1;
    maximumWidth = -1;
    maximumHeight = -1;
}
void LineEditNumberDelegate::setMinimumWidth(int width)
{
    minimumWidth = width;
}
void LineEditNumberDelegate::setMinimumHeight(int height)
{
    minimumHeight = height;
}
void LineEditNumberDelegate::setMaximumWidth(int width)
{
    maximumWidth = width;
}
void LineEditNumberDelegate::setMaximumHeight(int height)
{
    maximumHeight = height;
}
void LineEditNumberDelegate::setDecimals(int decimals)
{
    this->decimals = decimals;
}
QWidget *LineEditNumberDelegate::createEditor(QWidget *parent,
     const QStyleOptionViewItem & option,
     const QModelIndex & index ) const
{
    LineEdit *editor = new LineEdit(parent);

    editor->setDecimals(decimals);

    /*
    if(minimumWidth != -1)
        editor->_setMinimumWidth(minimumWidth);
    if(minimumHeight != -1)
        editor->_setMinimumHeight(minimumHeight);
    if(maximumWidth != -1)
        editor->_setMaximumWidth(maximumWidth);
    if(minimumHeight != -1)
        editor->_setMaximumHeight(maximumHeight);
        */
    return editor;
}
void LineEditNumberDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option
                                  , const QModelIndex &index) const
{
    //if (index.column() == durationColumn) {

            QStyleOptionViewItem myOption = option;
            myOption.displayAlignment = Qt::AlignLeft | Qt::AlignVCenter;
            myOption.state = QStyle::State_Enabled | QStyle::State_Selected
                    | QStyle::State_Editing;
            //myOption.state = QStyle::State_Enabled | QStyle::State_Raised
                    //| QStyle::State_Editing;

            //drawDisplay(painter, myOption, myOption.rect, value);
            //drawFocus(painter, myOption, myOption.rect);



        //} else{
            QItemDelegate::paint(painter, option, index);
        //}
}
void LineEditNumberDelegate::setEditorData(QWidget *editor,
                                     const QModelIndex &index) const
{
    QString num = index.data(Qt::EditRole).toString();

    LineEdit *wNumber = static_cast<LineEdit*>(editor);
    wNumber->setText(num);
    //wNumber->selectAll();
    //wNumber->clearFocus();
}

void LineEditNumberDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                    const QModelIndex &index) const
{
    LineEdit *wNumber = static_cast<LineEdit*>(editor);
    QString str = wNumber->text();

    model->setData(index, str, Qt::EditRole);
    wNumber->clearFocus();
}

void LineEditNumberDelegate::updateEditorGeometry(QWidget *editor,
     const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    ((LineEdit*)editor)->setMinimumWidth(option.rect.size().width());
    ((LineEdit*)editor)->setMinimumHeight(option.rect.size().height());
    ((LineEdit*)editor)->setMaximumWidth(option.rect.size().width());
    ((LineEdit*)editor)->setMaximumHeight(option.rect.size().height());
    editor->setGeometry(option.rect);
}

void LineEditNumberDelegate::commitAndCloseEditor()
{
    WidgetNumber* editor = qobject_cast<WidgetNumber *>(sender());
    emit commitData(editor);
    emit closeEditor(editor);
}
