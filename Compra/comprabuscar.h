#ifndef COMPRABUSCAR_H
#define COMPRABUSCAR_H

#include "compra_global.h"

namespace Ui {
class CompraBuscar;
}

class COMPRASHARED_EXPORT CompraBuscar : public QWidget
{
    Q_OBJECT

public:
    enum{BUSCAR_DETALLE, AGREGAR_A_LISTA};
public:
    explicit CompraBuscar(QWidget *parent = 0);
    ~CompraBuscar();

    QString getID(){ return id; }
    QString getPersonaID(){ return persona_id; }
    QString getFechaEmision(){ return fecha_emision; }
    QString getSerie(){ return serie; }
    QString getNumero(){ return numero; }
    QString getCodigo(){ return codigo; }
    QString getNombre(){ return nombre; }

    void set_tipo(int tipo);

    void set_widget_previous(QWidget* w);

    void set_ruc(QString ruc);

private slots:
    void on_compra_closing();

    void on_lineEdit_buscar_textEdited(const QString &arg1);

    void on_lineEdit_buscar_returnPressed();

    void on_pushButton_ok_clicked();

    void on_pushButton_salir_clicked();

    void on_pushButton_agregar_clicked();

    void on_pushButton_modificar_clicked();    

protected:
    bool eventFilter(QObject *obj, QEvent *e);

    void showEvent(QShowEvent *event);

    void closeEvent(QCloseEvent *event);

signals:
    void closing();

private:
    Ui::CompraBuscar *ui;

    QWidget* widget_previous;

    bool modo_only_date;

    int tipo;

    int pos;
    int size_query;

    QString id;
    QString persona_id;

    QString fecha_emision;
    QString serie;
    QString numero;
    QString codigo;
    QString nombre;

};

#endif // COMPRABUSCAR_H
