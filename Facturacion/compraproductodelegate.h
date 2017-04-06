#ifndef COMPRAPRODUCTODELEGATE_H
#define COMPRAPRODUCTODELEGATE_H

#include "facturacion_global.h"
#include "compra_headers.h"

class FACTURACIONSHARED_EXPORT CompraProductoDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit CompraProductoDelegate(QObject *parent = 0);

    void set_table(QTableWidget* table);

    void setMaxVisibleItems(int maxItems);
    void setCompletionMode(QCompleter::CompletionMode mode);
    void setFilterMode(Qt::MatchFlags filterMode);

    void set_query(const QString& str_query);

    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                               const QModelIndex &index) const;

    void paint(QPainter * painter, const QStyleOptionViewItem & option
                              , const QModelIndex & index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                    const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index) const;

signals:

public slots:
    void on_comboBox_currentIndexChanged(int index);

    void on_comboBox_currentTextChanged(const QString& arg);

    void on_comboBox_editTextChanged(const QString& arg);

    void on_comboBox_activated(const QString& arg);

    void on_comboBox_highlighted(const QString& arg);

protected:
    bool eventFilter(QObject *obj, QEvent *e);

private:
    QTableWidget* table;

    QComboBox* comboBox;
    QCompleter* completer;

    int maxItems;
    int mode;
    int filterMode;

};

#endif // COMPRAPRODUCTODELEGATE_H
