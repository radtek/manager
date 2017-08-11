#ifndef PERSONA_H
#define PERSONA_H

#include "mylibrary/mylibrary_global.h"
#include "mylibrary/widgetevents.h"

namespace Ui {
class Persona;
}

class Persona : public WidgetEvents
{
    Q_OBJECT

public:
    explicit Persona(QWidget *parent = 0);
    ~Persona();

private:
    Ui::Persona *ui;
};

#endif // PERSONA_H
