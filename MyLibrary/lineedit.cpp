#include "lineedit.h"

LineEdit::LineEdit(QWidget* parent)
    :QLineEdit(parent)
{    
    QRegExp regExp_decimal("\\d*\\.{0,1}\\d*");
    this->setValidator(new QRegExpValidator(regExp_decimal));

    connect(this, SIGNAL(textChanged(QString)), this, SLOT(textChanged(QString)));
    connect(this, SIGNAL(editingFinished()), this, SLOT(editingFinished()));

    setPlaceholderText("0");
}
LineEdit::LineEdit(const QString& str, QWidget* parent)
    :QLineEdit(str, parent)
{
    QRegExp regExp_decimal("\\d*\\.{0,1}\\d*");
    this->setValidator(new QRegExpValidator(regExp_decimal));

    connect(this, SIGNAL(textChanged(QString)), this, SLOT(textChanged(QString)));
    connect(this, SIGNAL(editingFinished()), this, SLOT(editingFinished()));

    setPlaceholderText("0");
}
void LineEdit::textChanged(const QString& arg)
{

}
void LineEdit::editingFinished()
{
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
        return;
    }
    count = 0;
    for(int i=this->text().length()-1; i>=0; i--){
        if(QString(this->text()[i]).compare("0") == 0){
            count++;
        }else{
            if(QString(this->text()[i]).compare(".") == 0){
                count++;
            }
            break;
        }
    }
    this->setText(this->text().remove(this->text().length()-count, count));
}
