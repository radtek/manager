#ifndef VENTACONFIGURACION_H
#define VENTACONFIGURACION_H

#include "venta_global.h"

namespace Ui {
class VentaConfiguracion;
}

class VENTASHARED_EXPORT VentaConfiguracion : public QWidget
{
    Q_OBJECT

public:
    explicit VentaConfiguracion(QWidget *parent = 0);
    ~VentaConfiguracion();

    void set_size(int width, int height);

    QVector<QLabel*> get_labels();

    static QLabel *createDragLabel(const QString &text, QWidget *parent)
    {
        QLabel *label = new QLabel(text, parent);
        label->setAutoFillBackground(true);
        label->setFrameShape(QFrame::Panel);
        label->setFrameShadow(QFrame::Raised);
        return label;
    }

    static QString hotSpotMimeDataKey() { return QStringLiteral("application/x-hotspot"); }

    void set_tipo(int tipo, QString serie, QString series_id
                  , QVector<QString>& object_name, QVector<QString>& data);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    Ui::VentaConfiguracion *ui;

    int width;
    int height;

    int tipo;

    QVector<QLabel*> labels;

    bool isPress;
    bool isRelease;

    QPoint press;
    QPoint release;
};

#endif // VENTACONFIGURACION_H
