#ifndef FUNCIONALIDAD_H
#define FUNCIONALIDAD_H

#include "mylibrary.h"

class MYLIBRARYSHARED_EXPORT Funcionalidad : public QWidget
{
    Q_OBJECT
public:
    explicit Funcionalidad(QWidget *parent = nullptr);

signals:

public slots:

};

template class MYLIBRARYSHARED_EXPORT singleton<Funcionalidad>;

#endif // FUNCIONALIDAD_H
