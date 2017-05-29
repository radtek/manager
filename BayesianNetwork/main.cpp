#include <QCoreApplication>

#include <dlib/bayes_utils.h>
#include <dlib/graph_utils.h>
#include <dlib/graph.h>
#include <dlib/directed_graph.h>
#include <iostream>
#include <conio.h>

using namespace dlib;
using namespace std;

#include <QFile>
#include <QString>
#include <QDebug>
#include <QStringList>
#include <QSqlQuery>
#include <QMap>
#include <QSet>

#include "database_connection.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);


    database_connection db_connection;
    db_connection.mf_set_driver("SQL SERVER");

    db_connection.mf_set_host("DESKTOP-S5KN297\\SERVER");
    //db_connection.mf_set_host("DESKTOP-TJMNUQT");
    db_connection.mf_set_name("restaurante");
    db_connection.mf_set_pass("");

    if(db_connection.mf_connect()){
        //qDebug()<<"1"<<endl;
    }else{
        qDebug()<<"0"<<endl;
        return 0;
    }

    QSqlQuery query;
    QString str_query;
    str_query += "SELECT plato.id AS plato, plato.nombre AS plato_nombre";
    str_query += ", YEAR(venta.fecha) AS annio";
    str_query += ", MONTH(venta.fecha) AS mes, DATENAME(wk, venta.fecha) AS semana";
    str_query += ", DATEPART(dw, venta.fecha) AS dia, SUM(venta_h_plato.cantidad) AS cantidad FROM venta_h_plato";
    str_query += " JOIN plato ON plato.id = venta_h_plato.plato_id";
    str_query += " JOIN venta ON venta.id = venta_h_plato.venta_id";
    str_query += " GROUP BY plato.id, plato.nombre, YEAR(venta.fecha), MONTH(venta.fecha), DATENAME(wk, venta.fecha), DATEPART(dw, venta.fecha)";
    str_query += " ORDER BY plato.id, plato.nombre, YEAR(venta.fecha), MONTH(venta.fecha), DATENAME(wk, venta.fecha), DATEPART(dw, venta.fecha)";

    QMap<int, int> annios;
    QMap<int, int> meses;
    QMap<int, int> semanas;
    QMap<int, int> dias;
    QMap<int, QPair<int, QString> > platos;

    QMap<int, double> annio_cantidad;

    QMap<int, QMap<int, double> > annio_mes_cantidad;
    QMap<int, QMap<int, double> > mes_semana_cantidad;
    QMap<int, QMap<int, double> > semana_dia_cantidad;
    QMap<int, QMap<int, double> > dia_plato_cantidad;

    if(query.exec(str_query)){
        while(query.next()){
            int plato_id = query.value(0).toInt();
            QString plato_nombre = query.value(1).toString();
            double cantidad = query.value(6).toDouble();
            int annio = query.value(2).toInt();
            int mes = query.value(3).toInt();
            int semana = query.value(4).toInt();
            int dia = query.value(5).toInt();

            annios[annio] = 0;
            meses[mes] = 0;
            semanas[semana] = 0;
            dias[dia] = 0;
            platos[plato_id] = qMakePair<int, QString>(0, plato_nombre);

            annio_cantidad[annio] += cantidad;
            annio_mes_cantidad[annio][mes] += cantidad;
            mes_semana_cantidad[mes][semana] += cantidad;
            semana_dia_cantidad[semana][dia] += cantidad;
            dia_plato_cantidad[dia][plato_id] += cantidad;
        }

        int i = 0;

        QMap<int, int>::const_iterator i_annio = annios.constBegin();
        while (i_annio != annios.constEnd()) {
            annios[i_annio.key()] = i;
            ++i;
            ++i_annio;
        }

        i = 0;
        QMap<int, int>::const_iterator i_mes = meses.constBegin();
        while (i_mes != meses.constEnd()) {
            meses[i_mes.key()] = i;
            ++i;
            ++i_mes;
        }

        i = 0;
        QMap<int, int>::const_iterator i_semana = semanas.constBegin();
        while (i_semana != semanas.constEnd()) {
            semanas[i_semana.key()] = i;
            i++;
            ++i_semana;
        }

        i = 0;
        QMap<int, int>::const_iterator i_dia = dias.constBegin();
        while (i_dia != dias.constEnd()) {
            dias[i_dia.key()] = i;
            i++;
            ++i_dia;
        }
        i = 0;
        QMap<int, QPair<int, QString> >::const_iterator i_plato = platos.constBegin();
        while (i_plato != platos.constEnd()) {
            platos[i_plato.key()].first = i;
            i++;
            ++i_plato;
        }

        // COMPLETANDO

        i_annio = annios.constBegin();
        while (i_annio != annios.constEnd()) {
            QMap<int, int>::const_iterator i_mes = meses.constBegin();
            while (i_mes != meses.constEnd()) {
                double annio_mes_cantidad_value = annio_mes_cantidad[i_annio.key()][i_mes.key()];
                if(annio_mes_cantidad_value == 0.0)
                    annio_mes_cantidad[i_annio.key()][i_mes.key()] = 0.0;
                ++i_mes;
            }
            ++i_annio;
        }

        i_mes = meses.constBegin();
        while (i_mes != meses.constEnd()) {
            QMap<int, int>::const_iterator i_semana = semanas.constBegin();
            while (i_semana != semanas.constEnd()) {
                double mes_semana_cantidad_value = mes_semana_cantidad[i_mes.key()][i_semana.key()];
                if(mes_semana_cantidad_value == 0.0){
                    mes_semana_cantidad[i_mes.key()][i_semana.key()] = 0.0;
                }
                ++i_semana;
            }
            ++i_mes;
        }

        i_semana = semanas.constBegin();
        while (i_semana != semanas.constEnd()) {
            QMap<int, int>::const_iterator i_dia = dias.constBegin();
            while (i_dia != dias.constEnd()) {
                double semana_dia_cantidad_value = semana_dia_cantidad[i_semana.key()][i_dia.key()];
                if(semana_dia_cantidad_value == 0.0){
                    semana_dia_cantidad[i_semana.key()][i_dia.key()] = 0.0;
                }
                ++i_dia;
            }
            ++i_semana;
        }

        i_dia = dias.constBegin();
        while (i_dia != dias.constEnd()) {
            QMap<int, QPair<int, QString> >::const_iterator i_plato = platos.constBegin();
            while (i_plato != platos.constEnd()) {
                double dia_plato_cantidad_value = dia_plato_cantidad[i_dia.key()][i_plato.key()];
                if(dia_plato_cantidad_value == 0.0){
                    dia_plato_cantidad[i_dia.key()][i_plato.key()] = 0.0;
                }
                ++i_plato;
            }
            ++i_dia;
        }
        qDebug()<<annio_mes_cantidad<<endl;
        qDebug()<<mes_semana_cantidad<<endl;
        qDebug()<<semana_dia_cantidad<<endl;
        qDebug()<<dia_plato_cantidad<<endl;
        /*
        qDebug()<<platos<<endl;
        qDebug()<<annios<<endl;
        qDebug()<<meses<<endl;
        qDebug()<<semanas<<endl;
        qDebug()<<dias<<endl;*/
    }else{
        return 0;
    }
    try{
        using namespace bayes_node_utils;

        directed_graph<bayes_node>::kernel_1a_c bn;

        enum nodes
        {
            ANNIO = 0,
            MES = 1,
            SEMANA = 2,
            DIA = 3,
            PLATO = 4
        };

        bn.set_number_of_nodes(5);

        bn.add_edge(ANNIO, MES);
        bn.add_edge(MES, SEMANA);
        bn.add_edge(SEMANA, DIA);
        bn.add_edge(DIA, PLATO);

        set_node_num_values(bn, ANNIO, annios.size());
        set_node_num_values(bn, MES, meses.size());
        set_node_num_values(bn, SEMANA, semanas.size());
        set_node_num_values(bn, DIA, dias.size());
        set_node_num_values(bn, PLATO, platos.size());

        assignment parent_state;

        // /////////////////////////////////////

        // ANNIO
        double annio_cantidad_value = 0.0;
        QMap<int, double>::const_iterator i_annio_cantidad = annio_cantidad.constBegin();
        while (i_annio_cantidad != annio_cantidad.constEnd()) {
            annio_cantidad_value += i_annio_cantidad.value();
            ++i_annio_cantidad;
        }
        i_annio_cantidad = annio_cantidad.constBegin();
        while (i_annio_cantidad != annio_cantidad.constEnd()) {
            double prob = i_annio_cantidad.value() / annio_cantidad_value;
            set_node_probability(bn, ANNIO, annios[i_annio_cantidad.key()], parent_state, prob);
            ++i_annio_cantidad;
        }

        // ANNIO_MES
        QMap<int, QMap<int, double> >::const_iterator i_annio_mes_cantidad = annio_mes_cantidad.constBegin();
        while (i_annio_mes_cantidad != annio_mes_cantidad.constEnd()) {
            QMap<int, double> mes_cantidad = i_annio_mes_cantidad.value();

            double annio_mes_cantidad_value = 0.0;
            QMap<int, double>::const_iterator i_mes_cantidad = mes_cantidad.constBegin();
            while(i_mes_cantidad != mes_cantidad.constEnd()){
                annio_mes_cantidad_value += i_mes_cantidad.value();
                i_mes_cantidad++;
            }
            parent_state.add(ANNIO, annios[i_annio_mes_cantidad.key()]);

            i_mes_cantidad = mes_cantidad.constBegin();
            while(i_mes_cantidad != mes_cantidad.constEnd()){
                double prob = i_mes_cantidad.value() / annio_mes_cantidad_value;
                qDebug() << annios[i_annio_mes_cantidad.key()] << "  " << meses[i_mes_cantidad.key()]
                         << "  " << i_mes_cantidad.value() << " / " << annio_mes_cantidad_value
                         << " = " << prob << endl;
                set_node_probability(bn, MES, meses[i_mes_cantidad.key()], parent_state, prob);
                i_mes_cantidad++;
            }

            parent_state.clear();
            i_annio_mes_cantidad++;
        }

        // MES_SEMANA
        QMap<int, QMap<int, double> >::const_iterator i_mes_semana_cantidad = mes_semana_cantidad.constBegin();
        while (i_mes_semana_cantidad != mes_semana_cantidad.constEnd()) {
            QMap<int, double> semana_cantidad = i_mes_semana_cantidad.value();

            double mes_semana_cantidad_value = 0.0;
            QMap<int, double>::const_iterator i_semana_cantidad = semana_cantidad.constBegin();
            while(i_semana_cantidad != semana_cantidad.constEnd()){
                mes_semana_cantidad_value += i_semana_cantidad.value();
                i_semana_cantidad++;
            }

            parent_state.add(MES, meses[i_mes_semana_cantidad.key()]);

            i_semana_cantidad = semana_cantidad.constBegin();
            while(i_semana_cantidad != semana_cantidad.constEnd()){
                double prob = i_semana_cantidad.value() / mes_semana_cantidad_value;
                qDebug() << meses[i_mes_semana_cantidad.key()] << "  " << semanas[i_semana_cantidad.key()]
                         << "  " << i_semana_cantidad.value() << " / " << mes_semana_cantidad_value
                         << " = " << prob << endl;
                set_node_probability(bn, SEMANA, semanas[i_semana_cantidad.key()], parent_state, prob);
                i_semana_cantidad++;
            }

            parent_state.clear();
            i_mes_semana_cantidad++;
        }

        // SEMANA_DIA
        QMap<int, QMap<int, double> >::const_iterator i_semana_dia_cantidad = semana_dia_cantidad.constBegin();
        while (i_semana_dia_cantidad != semana_dia_cantidad.constEnd()) {
            QMap<int, double> dia_cantidad = i_semana_dia_cantidad.value();

            double semana_dia_cantidad_value = 0.0;
            QMap<int, double>::const_iterator i_dia_cantidad = dia_cantidad.constBegin();
            while(i_dia_cantidad != dia_cantidad.constEnd()){
                semana_dia_cantidad_value += i_dia_cantidad.value();
                i_dia_cantidad++;
            }

            parent_state.add(SEMANA, semanas[i_semana_dia_cantidad.key()]);

            i_dia_cantidad = dia_cantidad.constBegin();
            while(i_dia_cantidad != dia_cantidad.constEnd()){
                double prob = i_dia_cantidad.value() / semana_dia_cantidad_value;
                set_node_probability(bn, DIA, dias[i_dia_cantidad.key()], parent_state, prob);
                i_dia_cantidad++;
            }

            parent_state.clear();
            i_semana_dia_cantidad++;
        }

        // DIA_PLATO
        QMap<int, QMap<int, double> >::const_iterator i_dia_plato_cantidad = dia_plato_cantidad.constBegin();
        while (i_dia_plato_cantidad != dia_plato_cantidad.constEnd()) {
            QMap<int, double> plato_cantidad = i_dia_plato_cantidad.value();

            double dia_plato_cantidad_value = 0.0;
            QMap<int, double>::const_iterator i_plato_cantidad = plato_cantidad.constBegin();
            while(i_plato_cantidad != plato_cantidad.constEnd()){
                dia_plato_cantidad_value += i_plato_cantidad.value();
                i_plato_cantidad++;
            }

            parent_state.add(DIA, dias[i_dia_plato_cantidad.key()]);

            i_plato_cantidad = plato_cantidad.constBegin();
            while(i_plato_cantidad != plato_cantidad.constEnd()){
                double prob = i_plato_cantidad.value() / dia_plato_cantidad_value;
                set_node_probability(bn, PLATO, platos[i_plato_cantidad.key()].first, parent_state, prob);
                i_plato_cantidad++;
            }

            parent_state.clear();
            i_dia_plato_cantidad++;
        }

        qDebug()<<"calcular: "<<endl;

        typedef dlib::set<unsigned long>::compare_1b_c set_type;
        typedef graph<set_type, set_type>::kernel_1a_c join_tree_type;
        join_tree_type join_tree;

        // Now we need to populate the join_tree with data from our bayesian network.  The next
        // function calls do this.  Explaining exactly what they do is outside the scope of this
        // example.  Just think of them as filling join_tree with information that is useful
        // later on for dealing with our bayesian network.
        create_moral_graph(bn, join_tree);
        create_join_tree(join_tree, join_tree);

        // Now that we have a proper join_tree we can use it to obtain a solution to our
        // bayesian network.  Doing this is as simple as declaring an instance of
        // the bayesian_network_join_tree object as follows:
        bayesian_network_join_tree solution(bn, join_tree);

        qDebug() << "Using the join tree algorithm:\n";

        QMap<int, int>::const_iterator i_annio = annios.constBegin();
        while (i_annio != annios.constEnd()) {
            qDebug() << "p(ANNIO="+QString().setNum(i_annio.key())+") = "
                     << solution.probability(ANNIO)(i_annio.value()) << endl;
            ++i_annio;
        }
        QMap<int, int>::const_iterator i_mes = meses.constBegin();
        while (i_mes != meses.constEnd()) {
            qDebug() << "p(MES="+QString().setNum(i_mes.key())+") = "
                     << solution.probability(MES)(i_mes.value()) << endl;
            ++i_mes;
        }
        QMap<int, int>::const_iterator i_semana = semanas.constBegin();
        while (i_semana != semanas.constEnd()) {
            qDebug() << "p(SEMANA="+QString().setNum(i_semana.key())+") = "
                     << solution.probability(SEMANA)(i_semana.value()) << endl;
            ++i_semana;
        }
        QMap<int, int>::const_iterator i_dia = dias.constBegin();
        while (i_dia != dias.constEnd()) {
            qDebug() << "p(DIA="+QString().setNum(i_dia.key())+") = "
                     << solution.probability(DIA)(i_dia.value()) << endl;
            ++i_dia;
        }
        QMap<int, QPair<int, QString> >::const_iterator i_plato = platos.constBegin();
        while (i_plato != platos.constEnd()) {
            qDebug() << "p(PLATO="+i_plato.value().second+") = "
                     << solution.probability(PLATO)(i_plato.value().first) << endl;
            ++i_plato;
        }        
    }
    catch (std::exception& e)
    {
        cout << "exception thrown: " << endl;
        cout << e.what() << endl;
        cout << "hit enter to terminate" << endl;
        //cin.get();
    }
    cin.get();
    //std::getchar();
    exit(0);

    return a.exec();
}
