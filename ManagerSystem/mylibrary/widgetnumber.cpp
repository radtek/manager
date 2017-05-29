#include "widgetnumber.h"

WidgetNumber::WidgetNumber(QWidget *parent) : QWidget(parent)
{
    box = new QVBoxLayout(this);
    lineEdit = new QLineEdit;

    box->layout()->setContentsMargins(0, 0, 0, 0);
    box->addSpacing(0);
    box->addStretch(0);

    box->addWidget(lineEdit);

    QRegExp regExp_decimal("\\d*\\.{0,1}\\d*");
    lineEdit->setValidator(new QRegExpValidator(regExp_decimal));

    connect(lineEdit, SIGNAL(textChanged(QString)), this, SLOT(textChanged(QString)));
    connect(lineEdit, SIGNAL(editingFinished()), this, SLOT(editingFinished()));

    lineEdit->setPlaceholderText("0");
    lineEdit->setText("0");

    decimals = 10;


    this->installEventFilter(this);
    lineEdit->installEventFilter(this);
}
WidgetNumber::~WidgetNumber()
{
    delete box;
    delete lineEdit;
}
void WidgetNumber::_setMinimumWidth(int width)
{
    this->setMinimumWidth(width);
    lineEdit->setMinimumWidth(width);
}
void WidgetNumber::_setMinimumHeight(int height)
{
    this->setMinimumHeight(height);
    lineEdit->setMinimumHeight(height);
}
void WidgetNumber::_setMaximumWidth(int width)
{
    this->setMaximumWidth(width);
    lineEdit->setMaximumWidth(width);
}
void WidgetNumber::_setMaximumHeight(int height)
{
    this->setMaximumHeight(height);
    lineEdit->setMaximumHeight(height);
}
void WidgetNumber::setDecimals(int decimals)
{
    this->decimals = decimals;
}
void WidgetNumber::selectAll()
{
    lineEdit->selectAll();
}

void WidgetNumber::setText(const QString& text)
{
    lineEdit->setText(text);
}
QString WidgetNumber::text()
{
    return lineEdit->text();
}

void WidgetNumber::textChanged(const QString& arg)
{

}
void WidgetNumber::editingFinished()
{
    int count = 0;
    for(int i=0; i<lineEdit->text().length(); i++){
        if(QString(lineEdit->text()[i]).compare("0") == 0){
            count++;
        }else{
            break;
        }
    }
    lineEdit->setText(lineEdit->text().remove(0, count));

    bool hasDot = false;
    for(int i=0; i<lineEdit->text().length(); i++){
        if(QString(lineEdit->text()[i]).compare(".") == 0){
            hasDot = true;
            break;
        }
    }
    if(!hasDot){
        if(lineEdit->text().compare("") == 0){
            lineEdit->setText("0");
            return;
        }
        return;
    }else{
        if(QString(lineEdit->text()[0]).compare(".") == 0){
            lineEdit->setText(lineEdit->text().insert(0, '0'));
        }
    }
    count = 0;
    for(int i=lineEdit->text().length()-1; i>=0; i--){
        if(QString(lineEdit->text()[i]).compare("0") == 0){
            count++;
        }else{
            break;
        }
    }
    lineEdit->setText(lineEdit->text().remove(lineEdit->text().length()-count, count));

    count = 0;
    for(int i=lineEdit->text().length()-1; i>=0; i--){
        if(QString(lineEdit->text()[i]).compare(".") == 0){
            break;
        }else{
            count++;
        }
    }
    if(count <= decimals){
        QString round = QString().setNum(lineEdit->text().toDouble(), 'f', count);
        lineEdit->setText(round);
    }else{
        QString round = QString().setNum(lineEdit->text().toDouble(), 'f', decimals);
        lineEdit->setText(round);
    }
}
bool WidgetNumber::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == this){
        if(event->type() == QFocusEvent::FocusIn){
            lineEdit->setFocus();
        }
        return true;
    }
    if(watched == lineEdit){
        if(event->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)event;

            switch(KeyEvent->key())
            {
            case Qt::Key_Period:{
                QString str = lineEdit->text();
                int periodPosition = str.indexOf('.');
                int cursorPosition;
                if(periodPosition == -1){
                    cursorPosition = lineEdit->cursorPosition();
                    str.insert(cursorPosition, '.');
                    cursorPosition += 1;
                }else{
                    cursorPosition = lineEdit->cursorPosition();
                    str.replace(".", " ");
                    str.insert(lineEdit->cursorPosition(), '.');
                    str.replace(" ", "");
                    if(periodPosition >= cursorPosition)
                        cursorPosition += 1;
                }
                lineEdit->setText(str);
                lineEdit->setCursorPosition(cursorPosition);
                return true;
            }break;
            }

        }else{

        }
        return false;
    }
    return eventFilter(watched, event);

}
