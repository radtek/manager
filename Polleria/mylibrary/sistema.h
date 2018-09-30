#ifndef SISTEMA_H
#define SISTEMA_H

#include "mylibrary_global.h"

class MYLIBRARYSHARED_EXPORT Sistema
{
private:
    double dolar;

    double igv;

    QWidget* curr_widget;
    QList<QWidget*> widgets;
public:

    QString rdy_for_trig;

    QString ssheet_select;
    QString ssheet_no_select;

    static QString name_persona;
    static QString pass_persona;
    static QString tipo_persona;

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

    void moveRow(QTableWidget*, int, int);

	bool hasChildren(QFrame *f);

    double round(double num, int decimals);

    QString zeros(int n);        

    QString& align_left(QString &str, int lenght);

    QString& justified(QString& str, int lenght);

    QString& epson_just_descripcion(QString& str, int n);

    QString& insert_left_spaces(QString& str, int n);

    QString& insert_right_spaces(QString& str, int n);

    void insertImage(QDataStream& out, QString fileName);

    QString& rightText(QString& text, int n);

    QString& centerText(QString& text, int n);

    QVector<QString>& centerTexts(QVector<QString>& v, int n);

    QVector<QString>& rightTexts(QVector<QString>& v, int n);

    void fixString(QLineEdit* le);

    QString& fixString(QString& str);

    void windowCentered(QWidget* w);

    void table_resize_to_contents(QWidget* p, QTableWidget* tb, int visible_rows = 12);

	void form_disable_widgets(QVector<QWidget*> v);

    QString get_mes(int mm);

    QString replace_quotes(const QString& str);

    QString replace_quotes_simple(const QString &str);

    void set_stylesheet(const QVector<QWidget*>& v, int pos);

    void clear_center_w(QWidget* w);

    QWidget* change_center_w(QWidget* old_w = new QWidget(), QWidget* new_w = new QWidget(), bool isClose=true);

    QWidget* set_center_w(QWidget* w, QString styleSheet = "background-color: rgb(255, 255, 127)");

    QMainWindow* get_mainw(QWidget* w);

    QWidget* get_centralw(QWidget *w);

    QWidget* get_centralw();

    QString build_argument(const QString& cmp_column, const QString& arg);

    QString multiple_query(QString& query);

    bool update_cambio_dolar(QWidget* sender, const QDate& date, double value);

    bool update_igv(QWidget* sender, const QDate& date, double igv);

    void table_button_up(QTableWidget* tableWidget);

    void table_button_down(QTableWidget* tableWidget);

    void table_button_borrar(QTableWidget* tableWidget);

    QString get_tipo_documento(int tipo);

    int get_tipo_documento(const QString str_tipo);

    void remove(QGridLayout *layout, int row, int column, bool deleteWidgets);

    void set_pushButton_css(QGridLayout *layout, QString text
                            , int row, int column, QFont font, QString color, QString bg_color);

    void set_toolButton_css(QGridLayout *layout, QString text
                            , int row, int column, QFont font, QString color, QString bg_color);

    QToolButton* return_tb(QGridLayout *layout
                                     , int row, int column);

    QString extract_background_color(QString styleSheet);

    QString extract_color(QString styleSheet);

    void deleteChildWidgets(QLayoutItem *item);

    void removeRow(QGridLayout *layout, int row, bool deleteWidgets);

    void removeColumn(QGridLayout *layout, int column, bool deleteWidgets);

    bool start_transaction();

    bool commit();

    bool rollback();

    bool confirmar_admin(QString nombre, QString pass);

    bool confirmar_my_pass(QString nombre, QString pass);

    void star_print_text(QDataStream& out, QString str);

    void star_set_font_7x9_77(QDataStream& out);

    void star_set_font_5x9_80(QDataStream& out);

    void star_set_font_5x9_58(QDataStream& out);

    void star_set_space(QDataStream& out, int n);

    void star_line_feed(QDataStream& out);

    void star_carriage_return(QDataStream& out);

    void star_line_feed_n(QDataStream& out, int n);

    void star_set_line_feed_1_8_inch(QDataStream& out);

    void star_set_line_feed_7_72_inch(QDataStream& out);

    void star_set_line_feed_1_12_inch(QDataStream& out);

    void star_set_line_feed_1_6_inch(QDataStream& out);

    void star_set_page_lines_1_6(QDataStream& out, int n);

    void star_set_bottom_margin(QDataStream& out, int n);

    void star_cancel_bottom_margin(QDataStream& out);

    void star_set_left_margin(QDataStream& out, int n);

    void star_set_right_margin(QDataStream& out, int n);

    void star_set_print_region(QDataStream& out, int n);

    void star_set_horizontal_tab(QDataStream& out, QVector<int>& digits);

    void star_move_horizontal_tab(QDataStream& out);

    void star_set_alignment(QDataStream& out, int n);

    void star_paper_cut(QDataStream& out, int n);

    void epson_lineFeed(QDataStream& out);

    void epson_linesFeed(QDataStream& out, int n);

    void epson_printText(QDataStream& out, QString str);

    void epson_paperCut(QDataStream& out);

    void epson_drawer(QDataStream& out);

    QString generate_cbc_value(double val);

    QString generate_unidades(QString num);

    QString generate_decenas(QString num);

    QString generate_centenas(QString num);

    bool create_boleta(QString serie, QString numero, double total
                                , QString codigo, QString nombre, QString direccion
                                , QVector<QString> v_cantidad, QVector<QString> v_precio
                                , QVector<QString> v_nombre, QVector<QString> v_id);

    bool create_factura(QString serie, QString numero, double total
                                , QString codigo, QString nombre, QString direccion
                                , QVector<QString> v_cantidad, QVector<QString> v_precio
                                , QVector<QString> v_nombre, QVector<QString> v_id);
};

template class MYLIBRARYSHARED_EXPORT singleton<Sistema>;

#endif // SISTEMA_H
