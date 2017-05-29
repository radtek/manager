#include "compraproductodelegate.h"

CompraProductoDelegate::CompraProductoDelegate(QObject *parent) :
    QItemDelegate(parent)
{
    maxItems = 10;
    mode = QCompleter::PopupCompletion;
    filterMode = Qt::MatchContains;

}
void CompraProductoDelegate::set_table(QTableWidget *table)
{
    this->table = table;
}

void CompraProductoDelegate::setMaxVisibleItems(int maxItems)
{
    this->maxItems = maxItems;
}
void CompraProductoDelegate::setCompletionMode(QCompleter::CompletionMode mode)
{
    this->mode = mode;
}
void CompraProductoDelegate::setFilterMode(Qt::MatchFlags filterMode)
{
    this->filterMode = filterMode;
}

QWidget *CompraProductoDelegate::createEditor(QWidget *parent,
     const QStyleOptionViewItem & option,
     const QModelIndex & index ) const
{    
    CompraProductoComboBox* editor = new CompraProductoComboBox(parent);

    editor->setEditable(true);



    editor->setMaxVisibleItems(maxItems);

    editor->completer()->setCompletionMode((QCompleter::CompletionMode)mode);

    editor->completer()->setFilterMode((Qt::MatchFlags)filterMode);

    //editor->completer()->setCaseSensitivity(Qt::CaseInsensitive);
    //editor->completer()->setWrapAround(false);

    editor->setInsertPolicy(QComboBox::NoInsert);

    //editor->completer()->setCaseSensitivity(Qt::CaseSensitive);

    //editor->completer()->setCompletionColumn(0);

    editor->setDuplicatesEnabled(true);

    //editor->completer()->popup()->setEditTriggers(QAbstractItemView::EditKeyPressed);

    //editor->completer()->popup()->installEventFilter(editor);

    //editor->addItem("", Qt::DisplayRole);
    editor->addItem("LATEX AMERICAN C BLANCO", Qt::DisplayRole);
    editor->addItem("LATEX AMERICAN C AMARILLO", Qt::DisplayRole);
    editor->addItem("LATEX PATO BLANCO", Qt::DisplayRole);
    editor->addItem("LATEX AMERICAN C NEGRO", Qt::DisplayRole);

    editor->setSizeAdjustPolicy(QComboBox::AdjustToContents);


    connect(editor->completer(), SIGNAL(activated(QString)), editor, SLOT(on_completer_activated(QString)));
    connect(editor->completer(), SIGNAL(highlighted(QString)), editor, SLOT(on_completer_highlighted(QString)));

    //disconnect(editor->completer(), SIGNAL(highlighted(QString)), editor, SLOT(setEditText(QString)));
    //disconnect(editor->completer(), SIGNAL(highlighted(QString)), editor, SLOT(setCurrentText(QString)));
    /*
    disconnect(editor->completer(), SIGNAL(highlighted(QString))
               , editor, SLOT(setCurrentText(QString)));
    disconnect(editor->completer(), SIGNAL(highlighted(QString))
               , editor, SLOT(setEditText(QString)));*/
    if(disconnect(editor->completer(), SIGNAL(highlighted(QString)), editor->lineEdit(), 0)){
        qDebug()<<"disconnect highlighted"<<endl;
    }else{

    }
    //connect(editor->completer(), SIGNAL(highlighted(QString)), editor, SLOT(on_this_editTextChanged(QString)));
    //connect(editor->completer(), SIGNAL(highlighted(QString)), editor, SLOT(on_this_currentTextChanged(QString)));

    //disconnect(editor, SIGNAL(currentIndexChanged(QString)), editor, SLOT(setText(QString)));
    //disconnect(editor, SIGNAL(currentIndexChanged(QString)), editor->lineEdit(), SLOT(setText(QString)));
    //disconnect(editor, SIGNAL(editTextChanged(QString)), editor->lineEdit(), SLOT(setText(QString)));
    //disconnect(editor->completer(), SIGNAL(highlighted(QModelIndex)), editor, SLOT(setCurrentText(QString)));
    //connect(editor->completer()->popup(), SIGNAL(activated(QModelIndex)), editor, SLOT(on_popup_activated(QModelIndex)));
    return editor;
}
void CompraProductoDelegate::on_comboBox_currentIndexChanged(int index)
{
    //qDebug()<<"current index changed"<<endl;
}

void CompraProductoDelegate::on_comboBox_currentTextChanged(const QString& arg)
{

}

void CompraProductoDelegate::on_comboBox_editTextChanged(const QString& arg)
{

}
void CompraProductoDelegate::on_comboBox_activated(const QString &arg)
{
    qDebug()<<"item activated"<<endl;

    //delete sender();
    //table->setFocus(Qt::TabFocusReason);
    //QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
    //QApplication::sendEvent(this, key);
}
void CompraProductoDelegate::on_comboBox_highlighted(const QString &arg)
{
    qDebug()<<"item highlighted"<<endl;

    //QComboBox* comboBox = (QComboBox*)((QCompleter*)sender())->parent();

    //disconnect(comboBox, SIGNAL(editTextChanged(QString)), this, SLOT(on_comboBox_editTextChanged(QString)));
    //connect(comboBox, SIGNAL(editTextChanged(QString)), this, SLOT(on_comboBox_editTextChanged(QString)));
}
bool CompraProductoDelegate::eventFilter(QObject *obj, QEvent *e)
{
    return false;
    //return eventFilter(obj, e);
}

void CompraProductoDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option
                                  , const QModelIndex &index) const
{

    //if (index.column() == durationColumn) {
    /*
            QString value = index.model()->data(index, Qt::DisplayRole).toString();

            QStyleOptionViewItem myOption = option;
            myOption.displayAlignment = Qt::AlignRight | Qt::AlignVCenter;
            myOption.state = QStyle::State_Enabled | QStyle::State_Raised
                    | QStyle::State_Editing;
*/
            //drawDisplay(painter, myOption, myOption.rect, text);
            //drawFocus(painter, myOption, myOption.rect);

        //} else{
    QItemDelegate::paint(painter, option, index);
        //}

}
void CompraProductoDelegate::setEditorData(QWidget *editor,
                                     const QModelIndex &index) const
{
    QString str = index.model()->data(index, Qt::EditRole).toString();

    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    comboBox->setCurrentText(str);

    qDebug()<<"set editor data"<<endl;
}

void CompraProductoDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                    const QModelIndex &index) const
{
    QComboBox *comboBox = static_cast<QComboBox*>(editor);

    QString str = comboBox->currentText();

    model->setData(index, str, Qt::EditRole);

    qDebug()<<"set model data"<<endl;
}

void CompraProductoDelegate::updateEditorGeometry(QWidget *editor,
     const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}

