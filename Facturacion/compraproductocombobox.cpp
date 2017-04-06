#include "compraproductocombobox.h"

CompraProductoComboBox::CompraProductoComboBox(QWidget* parent)
    :QComboBox(parent)
{    
    //this->completer()->setCaseSensitivity(Qt::CaseInsensitive);
    //connect(this->completer(), SIGNAL(highlighted(QString)), this, SLOT(on_completer_highlighted(QString)));
    //connect(this->completer(), SIGNAL(activated(QString)), this, SLOT(on_completer_activated(QString)));

    connect(this, SIGNAL(editTextChanged(QString)), this, SLOT(on_this_editTextChanged(QString)));
    connect(this, SIGNAL(currentTextChanged(QString)), this, SLOT(on_this_currentTextChanged(QString)));

    //this->installEventFilter(this);

}
void CompraProductoComboBox::on_this_editTextChanged(const QString &arg)
{
    QComboBox* comboBox = (QComboBox*)sender();

    if(completer()->popup()->isVisible()){
        qDebug()<<"popup is visible"<<endl;
        this->addItem("                                                 ", Qt::DisplayRole);
    }else{
        qDebug()<<"popup is not visible"<<endl;
        //completer()->popup()->show();
    }

    qDebug()<<"edit text changed"<<endl;

    /*
    disconnect(this, SIGNAL(editTextChanged(QString)), this, SLOT(on_this_editTextChanged(QString)));

    //this->completer()->setPopup(0);

    this->setEditText(arg);

    connect(this, SIGNAL(editTextChanged(QString)), this, SLOT(on_this_editTextChanged(QString)));
*/


    //disconnect(comboBox, SIGNAL(editTextChanged(QString)), this, SLOT(on_comboBox_editTextChanged(QString)));

    /*
    for(int i = 0; i < comboBox->count(); i++){
        comboBox->removeItem(0);
    }
    */
    //comboBox->setCurrentText(arg);
    //comboBox->removeItem(0);
    //comboBox->removeItem(1);
    //comboBox->removeItem(2);
    //comboBox->removeItem(3);
    //comboBox->removeItem(4);
/*
    comboBox->addItem("", Qt::EditRole);
    comboBox->addItem("LATEX AMERICAN C BLANCO", Qt::EditRole);
    comboBox->addItem("LATEX AMERICAN C AMARILLO", Qt::EditRole);
    comboBox->addItem("LATEX PATO BLANCO", Qt::EditRole);
    comboBox->addItem("LATEX AMERICAN C NEGRO", Qt::EditRole);
*/


    //connect(comboBox, SIGNAL(editTextChanged(QString)), this, SLOT(on_comboBox_editTextChanged(QString)));
}
void CompraProductoComboBox::on_this_currentTextChanged(const QString &arg)
{
    QComboBox* comboBox = (QComboBox*)sender();


    //disconnect(comboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(on_comboBox_currentTextChanged(QString)));

    //comboBox->removeItem(0);
    //comboBox->removeItem(1);
    //comboBox->removeItem(2);
    //comboBox->removeItem(3);
    //comboBox->removeItem(4);

    qDebug()<<"current text changed: "<<arg<<endl;

    if(completer()->popup()->isVisible()){
        qDebug()<<"popup is visible"<<endl;
    }else{
        qDebug()<<"popup is not visible"<<endl;
    }
    //connect(comboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(on_comboBox_currentTextChanged(QString)));
}

void CompraProductoComboBox::on_completer_currentIndexChanged(int index)
{

}

void CompraProductoComboBox::on_completer_highlighted(const QString &arg)
{
    qDebug()<<"highlighted: "<<arg<<endl;
    //disconnect(completer(), SIGNAL(highlighted(QString)), this->lineEdit(), 0);
    //disconnect(this->lineEdit(), SIGNAL(textEdited(QString)), this, SLOT(setEditText(QString)));
    //disconnect(this->lineEdit(), SIGNAL(textEdited(QString)), this, SLOT(setEditText(QString)));
    //disconnect(this->completer(), SIGNAL(highlighted(QString)), this->lineEdit(), SLOT(setText(QString)));
    //disconnect(this->completer(), SIGNAL(highlighted(QString)), this->lineEdit(), SLOT());
    //disconnect(this->completer(), SIGNAL(highlighted(QString)), this->lineEdit(), 0);
    //disconnect(this->completer(), SIGNAL(highlighted(QString)), ((QComboBox*)this), SLOT(setEditText(QString)));
    //disconnect(this->completer(), SIGNAL(highlighted(QString)), ((QComboBox*)this), SLOT(currentText(QString)));
    //disconnect(this, SIGNAL(editTextChanged(QString)), this, SLOT(on_this_editTextChanged(QString)));
    //qDebug()<<"current index: "<<this->currentIndex()<<endl;
    //qDebug()<<"current row: "<<this->completer()->currentIndex().row()<<endl;
    //qDebug()<<"current column: "<<this->completer()->currentIndex().column()<<endl;
    //disconnect(this, SIGNAL(editTextChanged(QString)), this, SLOT());
}

void CompraProductoComboBox::on_completer_activated(const QString &arg)
{
    qDebug()<<"activated: "<<arg<<endl;

    delete this;

    //connect(this, SIGNAL(editTextChanged(QString)), this, SLOT(on_this_editTextChanged(QString)));
}

void CompraProductoComboBox::on_popup_activated(const QModelIndex &index)
{
    //qDebug()<<"popup activated index row: "<<index.row()<<endl;
    //qDebug()<<"popup activated index column: "<<index.column()<<endl;

}

bool CompraProductoComboBox::eventFilter(QObject *obj, QEvent *e)
{
    /*
    if(obj == this){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent* ke = (QKeyEvent*)e;
            switch(ke->key()){
            case Qt::Key_Return:{
                delete this;
            }break;
            }
            return false;
        }
    }*/
    /*
    if(obj == this->completer()->popup()){
        if(e->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)e;
            qDebug()<<"key press"<<endl;
            switch(KeyEvent->key())
            {
            case Qt::Key_Down:{
                qDebug()<<"key down"<<endl;
                int count = this->count();
                if(this->currentIndex() < count)
                    this->setCurrentIndex(this->currentIndex()+1);
                else{
                    this->setCurrentIndex(-1);
                }
                return true;
            }break;
            case Qt::Key_Up:{
                qDebug()<<"key up"<<endl;
                int count = this->count();
                if(this->currentIndex() > -1)
                    this->setCurrentIndex(this->currentIndex()-1);
                else{
                    this->setCurrentIndex(count-1);
                }
                return true;
            }break;
            }

        }else{

        }
        return false;
    }else{
        //qDebug()<<"event Filter else"<<endl;
    }
    */
    return eventFilter(obj, e);
}
