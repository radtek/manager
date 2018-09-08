#include "lineedit.h"

LineEdit::LineEdit(QWidget* parent)
    :QLineEdit(parent)
{    
    QRegExp regExp_decimal("(\\d*\\.{0,1}\\d*)*");
    this->setValidator(new QRegExpValidator(regExp_decimal));

    connect(this, SIGNAL(textEdited(QString)), this, SLOT(textEdited(QString)));
    connect(this, SIGNAL(editingFinished()), this, SLOT(editingFinished()));

    setPlaceholderText("0");
    setText("0");

    decimals = 10;
}
LineEdit::LineEdit(const QString& str, QWidget* parent)
    :QLineEdit(str, parent)
{
    QRegExp regExp_decimal("(\\d*\\.{0,1}\\d*)*");
    this->setValidator(new QRegExpValidator(regExp_decimal));

    connect(this, SIGNAL(textEdited(QString)), this, SLOT(textEdited(QString)));
    connect(this, SIGNAL(editingFinished()), this, SLOT(editingFinished()));

    setPlaceholderText("0");
    setText("0");

    decimals = 10;
}
void LineEdit::setDecimals(int decimals)
{
    this->decimals = decimals;
}

void LineEdit::textEdited(const QString& arg)
{
    disconnect(this, SIGNAL(textEdited(QString)), this, SLOT(textEdited(QString)));
    QString str = arg;
    int posPeriod = this->cursorPosition()-1;
    if(posPeriod == -1){
        connect(this, SIGNAL(textEdited(QString)), this, SLOT(textEdited(QString)));
        return;
    }

    if(QString(str[posPeriod]).compare(".") != 0){
        connect(this, SIGNAL(textEdited(QString)), this, SLOT(textEdited(QString)));
        return;
    }

    for(int i=0; i<str.length(); i++){
        if(i == posPeriod){ continue; }
        if(QString(str[i]).compare(".") == 0){
            str.remove(i, 1);
            this->setText(str);
            if(i > posPeriod){
                this->setCursorPosition(posPeriod+1);
            }else{
                this->setCursorPosition(posPeriod);
            }
            break;
        }
    }
    connect(this, SIGNAL(textEdited(QString)), this, SLOT(textEdited(QString)));
}
void LineEdit::editingFinished()
{
    int cursorPosition = this->cursorPosition();

    int count = 0;
    for(int i=0; i<this->text().length(); i++){
        if(QString(this->text()[i]).compare("0") == 0){
            count++;
        }else{
            break;
        }
    }
    this->setText(this->text().remove(0, count));

    bool hasDot = false;
    for(int i=0; i<this->text().length(); i++){
        if(QString(this->text()[i]).compare(".") == 0){
            hasDot = true;
            break;
        }
    }
    if(!hasDot){
        if(this->text().compare("") == 0){
            this->setText("0");
            return;
        }
        return;
    }else{
        if(QString(this->text()[0]).compare(".") == 0){
            this->setText(this->text().insert(0, '0'));
        }
    }
    count = 0;
    for(int i=this->text().length()-1; i>=0; i--){
        if(QString(this->text()[i]).compare("0") == 0){
            count++;
        }else{
            break;
        }
    }
    this->setText(this->text().remove(this->text().length()-count, count));

    count = 0;
    for(int i=this->text().length()-1; i>=0; i--){
        if(QString(this->text()[i]).compare(".") == 0){
            break;
        }else{
            count++;
        }
    }
    if(count <= decimals){
        QString round = QString().setNum(this->text().toDouble(), 'f', count);
        this->setText(round);
    }else{
        QString round = QString().setNum(this->text().toDouble(), 'f', decimals);
        this->setText(round);
    }

    this->setCursorPosition(cursorPosition);
}
