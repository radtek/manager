#ifndef SISTEMA_H
#define SISTEMA_H

#include "mylibrary_global.h"

class MYLIBRARYSHARED_EXPORT Sistema
{
private:
    double dolar;

    double igv;

public:

    QString rdy_for_trig;

    QString ssheet_select;
    QString ssheet_no_select;

public:    

    Sistema();

    // DOCUMENT_TEMPLATE

    // PERSON_TEMPLATE
    // PRODUCT_TEMPLATE
    QString& normalDecimal(QString& value);

    double get_igv();
    double get_dolar();

    double get_igv(const QDate& date);
    double get_dolar(const QDate& date);

    QSqlQuery get_id_igv(const QDate& date);
    QSqlQuery get_id_dolar(const QDate& date);

    void cpy_q_to_tb(QSqlQuery& q, QTableWidget* tb);

    QString parse_query(QVector<QVector<QString> >& vv);

    void print_vv(QVector<QVector<QString> >& vv);

    void moveRow(QTableWidget*, int, int);

	bool hasChildren(QFrame *f);

    double round(double num, int decimals);

    QString zeros(int n);

    QString get_condition(const QString&, const QString&);    

    void fixString(QLineEdit* le);

    QString fixString(QString str);

    void windowCentered(QWidget* w);

    void table_resize_to_contents(QWidget* p, QTableWidget* tb, int visible_rows = 12);

	void form_disable_widgets(QVector<QWidget*> v);

    QString get_mes(int mm);

    void v_normalized_strings_for_cb(QVector<QString>& v);
    
    QString replace_quotes(const QString& str);

    void set_stylesheet(const QVector<QWidget*>& v, int pos);

    void clear_center_w(QWidget* w);

    QWidget* change_center_w(QWidget* old_w = new QWidget(), QWidget* new_w = new QWidget(), bool isClose=true);

    QWidget* set_center_w(QWidget* w, QString styleSheet = "background-color: rgb(255, 255, 127)");

    QMainWindow* get_mainw(QWidget* w);

    QString build_argument(const QString& cmp_column, const QString& arg);

    QString multiple_query(QString& query);

    bool update_cambio_dolar(QWidget* sender, const QDate& date, double value);

    bool update_igv(QWidget* sender, const QDate& date, double igv);

    QString query_ultimo_costo(QString producto_id);

};

template class MYLIBRARYSHARED_EXPORT singleton<Sistema>;

#endif // SISTEMA_H
