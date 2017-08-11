#include "widgetevents.h"

WidgetEvents::WidgetEvents(QWidget *parent) : QWidget(parent)
{
    afterShow = false;

    pushButton_salir = NULL;

    this->installEventFilter(this);
}
void WidgetEvents::set_widget_previous(QWidget *widget_previous)
{
    this->widget_previous = widget_previous;
}
void WidgetEvents::set_pushButton_salir(QPushButton* pushButton_salir)
{
    this->pushButton_salir = pushButton_salir;
    this->installEventFilter(pushButton_salir);
}
void WidgetEvents::showEvent(QShowEvent *event)
{
    event->accept();

    afterShow = true;
}
bool WidgetEvents::eventFilter(QObject *watched, QEvent *event)
{
    QWidget* w_temp;
    w_temp = this;
    if(watched == w_temp){
        if(event->type() == QEvent::MouseButtonPress){
            focusWidget()->setFocus();
            return true;
        }
        if(event->type() == QEvent::MouseButtonDblClick){
            focusWidget()->setFocus();
            return true;
        }
        if(event->type() == QEvent::Paint){
            if(afterShow) {
                if(focusWidget()){
                    focusWidget()->setFocus();
                }
                afterShow = false;
            }
            return true;
        }
        if(event->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)event;
            switch(KeyEvent->key())
            {
            case Qt::Key_Escape:{
                if(pushButton_salir)
                    pushButton_salir->click();
                return true;
            }break;
            }
        }
        return false;
    }
    /*
    w_temp = pushButton_salir;
    if(pushButton_salir && watched == w_temp){
        if(event->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)event;

            switch(KeyEvent->key())
            {
            case Qt::Key_Tab:{
                this->setFocus();
            }break;
            case Qt::Key_Return:{
                pushButton_salir->click();
                return true;
            }break;
            case Qt::Key_Enter:{
                QKeyEvent* key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QApplication::sendEvent(w_temp, key);
                return true;
            }break;
            }
        }
        return false;
    }*/
    return eventFilter(watched, event);
}
