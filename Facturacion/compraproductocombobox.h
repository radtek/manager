#ifndef COMPRAPRODUCTOCOMBOBOX_H
#define COMPRAPRODUCTOCOMBOBOX_H

#include "facturacion_global.h"
#include "compra_headers.h"

class FACTURACIONSHARED_EXPORT CompraProductoComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit CompraProductoComboBox(QWidget* parent = 0);

public slots:
    void on_this_editTextChanged(const QString& arg);

    void on_this_currentTextChanged(const QString& arg);

    void on_completer_currentIndexChanged(int index);

    void on_completer_highlighted(const QString& arg);

    void on_completer_activated(const QString& arg);

    void on_popup_activated(const QModelIndex& index);

protected:
    bool eventFilter(QObject *obj, QEvent *e);

};

#endif // COMPRAPRODUCTOCOMBOBOX_H
