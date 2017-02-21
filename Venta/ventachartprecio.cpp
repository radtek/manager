#include "ventachartprecio.h"
#include "ui_ventachartprecio.h"

VentaChartPrecio::VentaChartPrecio(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VentaChartPrecio)
{
    ui->setupUi(this);

    pos = 0;
    size_query = 10;

    ui->dateEdit->setDate(QDate::currentDate());

    values_min<<0.0<<0.0<<0.0<<0.0<<0.0;
    values_max<<0.0<<0.0<<0.0<<0.0<<0.0;
    //values_min << 31.4 << 35.4 << 36.24 << 42.14 << 50.10;
    //values_max << 32.0 << 37.0 << 38.23 << 43.60 << 50.11;

    ui->label_title_marca->hide();
    ui->label_marca->hide();
    //set_tw();
    //set_chart();
/*
    QAction *action_up = new QAction(this);
    QAction *action_down = new QAction(this);
    action_up->setShortcut(Qt::ALT + Qt::Key_Up);
    action_down->setShortcut(Qt::ALT + Qt::Key_Down);

    connect(action_up, SIGNAL(triggered(bool)), this, SLOT(on_year_up()));
    connect(action_down, SIGNAL(triggered(bool)), this, SLOT(on_year_down()));

    this->addAction(action_up);
    this->addAction(action_down);
*/
    ui->tableWidget->hideColumn(0);
    ui->tableWidget->hideColumn(1);
    ui->tableWidget->hideColumn(10);
    ui->tableWidget->hideColumn(11);
    ui->tableWidget->hideColumn(12);

    this->installEventFilter(this);
    ui->tableWidget->installEventFilter(this);
    ui->pushButton_ok->installEventFilter(this);
    ui->pushButton_salir->installEventFilter(this);
}

VentaChartPrecio::~VentaChartPrecio()
{
    qDebug()<<"delete compra chart costo"<<endl;
    delete ui;
}
void VentaChartPrecio::set_widget_previous(QWidget *widget_previous)
{
    this->widget_previous = widget_previous;
}
void VentaChartPrecio::set_producto(QString producto_id
                                    , QString unidad, QString descripcion)
{
    ui->label_unidad->setText(unidad);
    ui->label_producto->setText(descripcion);

    llenar_datos(producto_id);
}
void VentaChartPrecio::set_value_min(QVector<qreal> v)
{
    this->values_min = v;
}
void VentaChartPrecio::set_value_max(QVector<qreal> v)
{
    this->values_max = v;
}
void VentaChartPrecio::set_tw()
{
    if(!(values_min.length() == 5 && values_max.length() == 5))
        return;

    tw = new QTableWidget(5, 4);

    tw->setHorizontalHeaderLabels(QStringList()<<"Min."<<"Máx.");
    int year = QDate::currentDate().year();

    tw->setVerticalHeaderLabels(QStringList()
                                <<QString().setNum(year-4)
                                <<QString().setNum(year-3)
                                <<QString().setNum(year-2)
                                <<QString().setNum(year-1)
                                <<QString().setNum(year));

    tw->setItem(0, 0, new QTableWidgetItem(QString().setNum(values_min[0])));
    tw->item(0, 0)->setBackgroundColor(QColor("#209FDF"));
    tw->setItem(1, 0, new QTableWidgetItem(QString().setNum(values_min[1])));
    tw->item(1, 0)->setBackgroundColor(QColor("#209FDF"));
    tw->setItem(2, 0, new QTableWidgetItem(QString().setNum(values_min[2])));
    tw->item(2, 0)->setBackgroundColor(QColor("#209FDF"));
    tw->setItem(3, 0, new QTableWidgetItem(QString().setNum(values_min[3])));
    tw->item(3, 0)->setBackgroundColor(QColor("#209FDF"));
    tw->setItem(4, 0, new QTableWidgetItem(QString().setNum(values_min[4])));
    tw->item(4, 0)->setBackgroundColor(QColor("#209FDF"));

    tw->setItem(0, 1, new QTableWidgetItem(QString().setNum(values_max[0])));
    tw->item(0, 1)->setBackgroundColor(QColor("#99CA53"));
    tw->setItem(1, 1, new QTableWidgetItem(QString().setNum(values_max[1])));
    tw->item(1, 1)->setBackgroundColor(QColor("#99CA53"));
    tw->setItem(2, 1, new QTableWidgetItem(QString().setNum(values_max[2])));
    tw->item(2, 1)->setBackgroundColor(QColor("#99CA53"));
    tw->setItem(3, 1, new QTableWidgetItem(QString().setNum(values_max[3])));
    tw->item(3, 1)->setBackgroundColor(QColor("#99CA53"));
    tw->setItem(4, 1, new QTableWidgetItem(QString().setNum(values_max[4])));
    tw->item(4, 1)->setBackgroundColor(QColor("#99CA53"));

    ui->horizontalLayout->addWidget(tw);
}
void VentaChartPrecio::set_chart()
{
    if(!(values_min.length() == 5 && values_max.length() == 5))
        return;

    QBarSet *set0 = new QBarSet("Min.");
    QBarSet *set1 = new QBarSet("Máx.");

    *set0 << values_min[0] << values_min[1] << values_min[2] << values_min[3] << values_min[4];
    *set1 << values_max[0] << values_max[1] << values_max[2] << values_max[3] << values_max[4];

    QBarSeries *barseries = new QBarSeries();
    barseries->append(set0);
    barseries->append(set1);

    QChart *chart = new QChart();

    chart->setMinimumWidth(500);
    chart->setMinimumHeight(230);

    chart->addSeries(barseries);
    //chart->legend()->hide();

    chart->setTitle("Costo - Año");

    QStringList categories;
    int year = QDate::currentDate().year();
    categories <<QString().setNum(year-4)
              <<QString().setNum(year-3)
              <<QString().setNum(year-2)
              <<QString().setNum(year-1)
              <<QString().setNum(year);

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    axisX->setTitleText("Año");
    chart->createDefaultAxes();
    chart->setAxisX(axisX, barseries);
    axisX->setRange(QString("2013"), QString("2017"));

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chart->setLocalizeNumbers(true);

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    ui->horizontalLayout->addWidget(chartView);
}
void VentaChartPrecio::llenar_datos(QString producto_id)
{
    this->producto_id = producto_id;

    llenar_tabla();

    llenar_cantidad_precio();

    //llenar_grafica();

    //set_tw();

    //set_chart();

}
void VentaChartPrecio::llenar_tabla()
{
    QString str_query;
    str_query += "SELECT t.id, t.tipo_documento_id, t.fecha_emision, t.cantidad_bruto";
    str_query += ", t.cantidad_bruto - t.cantidad_nc";
    str_query += ", t.precio / t.cantidad_bruto AS precio_bruto";
    str_query += ", IF((t.cantidad_bruto - t.cantidad_nc) <> 0, t.desc_nc / (t.cantidad_bruto - t.cantidad_nc), 0) AS desc_nc_unit";
    str_query += ", IF((t.cantidad_bruto - t.cantidad_nc) <> 0, IF(t.total <> 0, (t.precio / t.total), 0) * t.nc_monto / (t.cantidad_bruto - t.cantidad_nc), 0) AS desc_nc_monto_unit";
    str_query += ", t.serie, t.numero";
    str_query += ", t.persona_id, t.codigo AS codigo, t.nombre AS nombre, t.direccion AS direccion FROM";
    str_query += " (SELECT";
    str_query += " documento.id AS id";
    str_query += ", documento.tipo_documento_id AS tipo_documento_id";
    str_query += ", anexo.fecha_emision AS fecha_emision";
    str_query += ", SUM(documento_h_producto.cantidad) AS cantidad_bruto";
    str_query += ", IFNULL((SELECT SUM(d_h_prod_nc_cantidad.cantidad_aux) FROM documento d";
    str_query += " JOIN documento_h_documento d_h_d_nc ON d.id = d_h_d_nc.documento_id1";
    str_query += " JOIN documento d_nc ON (d_h_d_nc.documento_id = d_nc.id AND d_nc.tipo_documento_id = "+QString().setNum(tipo_documento::NOTA_CREDITO)+")";
    str_query += " JOIN documento_h_producto d_h_prod_nc_cantidad ON (d_h_prod_nc_cantidad.producto_id = "+producto_id+" AND d_h_prod_nc_cantidad.documento_id = d_nc.id)";
    str_query += " WHERE d.id = documento.id), 0) AS cantidad_nc";
    str_query += ", SUM(documento_h_producto.precio) AS precio";
    str_query += ", (SELECT SUM(documento_h_producto.precio) FROM documento_h_producto";
    str_query += " WHERE documento.id = documento_h_producto.documento_id) AS total";
    str_query += ", IFNULL((SELECT SUM(d_h_prod_nc_desc.precio_aux) FROM documento d";
    str_query += " JOIN documento_h_documento d_h_d_nc ON d.id = d_h_d_nc.documento_id1";
    str_query += " JOIN documento d_nc ON (d_h_d_nc.documento_id = d_nc.id AND d_nc.tipo_documento_id = "+QString().setNum(tipo_documento::NOTA_CREDITO)+")";
    str_query += " JOIN documento_h_producto d_h_prod_nc_desc ON (d_h_prod_nc_desc.producto_id = "+producto_id+" AND d_h_prod_nc_desc.documento_id = d_nc.id)";
    str_query += " WHERE d.id = documento.id), 0) AS desc_nc";
    str_query += ", IFNULL((SELECT SUM(nc.monto) FROM documento d";
    str_query += " JOIN documento_h_documento d_h_d_nc ON d.id = d_h_d_nc.documento_id1";
    str_query += " JOIN documento d_nc ON (d_h_d_nc.documento_id = d_nc.id AND d_nc.tipo_documento_id = "+QString().setNum(tipo_documento::NOTA_CREDITO)+")";
    str_query += " JOIN nota_credito nc ON d_nc.id = nc.comprobante_documento_id";
    str_query += " WHERE d.id = documento.id), 0) AS nc_monto";
    str_query += ", anexo.serie AS serie";
    str_query += ", anexo.numero AS numero";
    str_query += ", cliente.id AS persona_id";
    str_query += ", cliente.codigo AS codigo";
    str_query += ", cliente.nombre AS nombre";
    str_query += ", cliente.direccion AS direccion";
    str_query += " FROM producto";
    str_query += " JOIN documento_h_producto ON producto.id = documento_h_producto.producto_id";

    str_query += " JOIN documento ON ((documento.tipo_documento_id = "+QString().setNum(tipo_documento::BOLETA);
    str_query += " OR documento.tipo_documento_id = "+QString().setNum(tipo_documento::FACTURA)+")";
    str_query += " AND documento.id = documento_h_producto.documento_id)";
    str_query += " JOIN comprobante ON (comprobante.operacion_id = "+QString().setNum(operacion_items::VENTA)+" AND documento.id = comprobante.documento_id)";

    str_query += " LEFT JOIN boleta ON boleta.comprobante_documento_id = documento.id";
    str_query += " LEFT JOIN factura ON factura.comprobante_documento_id = documento.id";

    str_query += " JOIN anexo ON documento.id = anexo.documento_id";
    str_query += " LEFT JOIN documento_h_persona ON documento.id = documento_h_persona.documento_id";
    str_query += " LEFT JOIN persona ON persona.id = documento_h_persona.persona_id";
    str_query += " LEFT JOIN ((SELECT naturales.persona_id AS id, naturales.dni AS codigo, naturales.nombre AS nombre, naturales.direccion AS direccion FROM naturales)";
    str_query += " UNION ALL (SELECT juridica.persona_id, juridica.ruc, juridica.razon_social, juridica.domicilio_fiscal FROM juridica))";
    str_query += " AS cliente ON cliente.id = persona.id";
    str_query += " WHERE producto.id = "+producto_id+" AND YEAR(anexo.fecha_emision) = "+QString().setNum(ui->dateEdit->date().year());
    str_query += " GROUP BY documento.id";
    str_query += " ORDER BY anexo.fecha_emision DESC, documento.id DESC";
    str_query += " LIMIT "+QString().setNum(pos)+", "+QString().setNum(size_query)+") AS t";

    qDebug()<<str_query<<endl;
    QSqlQuery query;
    if(query.exec(str_query)){
        while(query.next()){
            int rowCount = ui->tableWidget->rowCount();
            ui->tableWidget->setRowCount(rowCount+1);

            QString id = query.value(0).toString();
            int tipo_documento_id = query.value(1).toInt();
            QString documento;
            if(tipo_documento_id == tipo_documento::BOLETA){
                documento = "BOLETA";
            }
            if(tipo_documento_id == tipo_documento::FACTURA){
                documento = "FACTURA";
            }
            QString fecha_emision = query.value(2).toString();
            double cant = query.value(3).toDouble();
            QString cantidad = QString().setNum(cant, ' ', 4);

            double cant_final = query.value(4).toDouble();
            QString cantidad_final = QString().setNum(cant_final, ' ', 4);

            double precio = query.value(5).toDouble();
            QString precio_bruto = QString().setNum(precio, ' ', 3);

            double desc_nc = query.value(6).toDouble();
            QString descuento_nc = QString().setNum(desc_nc, ' ', 3);

            double desc_nc_monto = query.value(7).toDouble();
            QString descuento_nc_monto = QString().setNum(desc_nc_monto, ' ', 3);

            double precio_neto_val = precio - desc_nc - desc_nc_monto;
            QString precio_neto = QString().setNum(precio_neto_val, ' ', 3);

            QString serie = query.value(8).toString();
            QString numero = query.value(9).toString();
            QString persona_id = query.value(10).toString();
            QString codigo = query.value(11).toString();
            QString nombre = query.value(12).toString();
            QString direccion = query.value(13).toString();

            ui->tableWidget->setItem(rowCount, 0, new QTableWidgetItem(id));
            ui->tableWidget->setItem(rowCount, 1, new QTableWidgetItem(QString().setNum(tipo_documento_id)));
            ui->tableWidget->setItem(rowCount, 2, new QTableWidgetItem(documento));
            ui->tableWidget->setItem(rowCount, 3, new QTableWidgetItem(fecha_emision));
            ui->tableWidget->setItem(rowCount, 4, new QTableWidgetItem(cantidad));
            ui->tableWidget->setItem(rowCount, 5, new QTableWidgetItem(cantidad_final));
            ui->tableWidget->setItem(rowCount, 6, new QTableWidgetItem(precio_bruto));
            ui->tableWidget->setItem(rowCount, 7, new QTableWidgetItem(descuento_nc));
            ui->tableWidget->setItem(rowCount, 8, new QTableWidgetItem(descuento_nc_monto));
            ui->tableWidget->setItem(rowCount, 9, new QTableWidgetItem(precio_neto));
            ui->tableWidget->setItem(rowCount, 10, new QTableWidgetItem(serie));
            ui->tableWidget->setItem(rowCount, 11, new QTableWidgetItem(numero));
            ui->tableWidget->setItem(rowCount, 12, new QTableWidgetItem(persona_id));
            ui->tableWidget->setItem(rowCount, 13, new QTableWidgetItem(codigo));
            ui->tableWidget->setItem(rowCount, 14, new QTableWidgetItem(nombre));
            ui->tableWidget->setItem(rowCount, 15, new QTableWidgetItem(direccion));

            ui->tableWidget->item(rowCount, 0)->setFlags(Qt::ItemIsEnabled
                                                         | Qt::ItemIsSelectable);

            ui->tableWidget->item(rowCount, 1)->setFlags(Qt::ItemIsEnabled
                                                         | Qt::ItemIsSelectable);

            ui->tableWidget->item(rowCount, 2)->setFlags(Qt::ItemIsEnabled
                                                         | Qt::ItemIsSelectable);

            ui->tableWidget->item(rowCount, 3)->setFlags(Qt::ItemIsEnabled
                                                         | Qt::ItemIsSelectable);

            ui->tableWidget->item(rowCount, 4)->setFlags(Qt::ItemIsEnabled
                                                         | Qt::ItemIsSelectable);

            ui->tableWidget->item(rowCount, 5)->setFlags(Qt::ItemIsEnabled
                                                         | Qt::ItemIsSelectable);

            ui->tableWidget->item(rowCount, 6)->setFlags(Qt::ItemIsEnabled
                                                         | Qt::ItemIsSelectable);

            ui->tableWidget->item(rowCount, 7)->setFlags(Qt::ItemIsEnabled
                                                         | Qt::ItemIsSelectable);

            ui->tableWidget->item(rowCount, 8)->setFlags(Qt::ItemIsEnabled
                                                         | Qt::ItemIsSelectable);

            ui->tableWidget->item(rowCount, 9)->setFlags(Qt::ItemIsEnabled
                                                         | Qt::ItemIsSelectable);

            ui->tableWidget->item(rowCount, 10)->setFlags(Qt::ItemIsEnabled
                                                         | Qt::ItemIsSelectable);

            ui->tableWidget->item(rowCount, 11)->setFlags(Qt::ItemIsEnabled
                                                         | Qt::ItemIsSelectable);

            ui->tableWidget->item(rowCount, 12)->setFlags(Qt::ItemIsEnabled
                                                         | Qt::ItemIsSelectable);

            ui->tableWidget->item(rowCount, 13)->setFlags(Qt::ItemIsEnabled
                                                         | Qt::ItemIsSelectable);

            ui->tableWidget->item(rowCount, 14)->setFlags(Qt::ItemIsEnabled
                                                         | Qt::ItemIsSelectable);

            ui->tableWidget->item(rowCount, 15)->setFlags(Qt::ItemIsEnabled
                                                         | Qt::ItemIsSelectable);

            pos++;
        }
        SYSTEM->table_resize_to_contents(0, ui->tableWidget);
    }else{

    }
}
void VentaChartPrecio::llenar_cantidad_precio()
{
    QString str_query;
    str_query += "(SELECT t.cantidad_bruto";
    str_query += ", t.cantidad_nc";
    str_query += ", t.precio / t.cantidad_bruto AS precio_bruto";
    str_query += ", t.precio_flete / t.cantidad_bruto AS flete_unit";
    str_query += ", IF((t.cantidad_bruto - t.cantidad_nc) <> 0, t.desc_nc / (t.cantidad_bruto - t.cantidad_nc), 0) AS desc_nc_unit";
    str_query += ", IF((t.cantidad_bruto - t.cantidad_nc) <> 0, IF(t.total <> 0, (t.precio / t.total), 0) * t.nc_monto / (t.cantidad_bruto - t.cantidad_nc), 0) AS desc_nc_monto_unit";
    //str_query += ", t.precio_bruto + t.flete_unit -  desc_nc_unit - desc_monto_unit AS precio_neto";
    str_query += " FROM";
    str_query += " (SELECT";
    str_query += " documento.id AS id";
    str_query += ", documento.tipo_documento_id AS tipo_documento_id";
    str_query += ", anexo.fecha_emision AS fecha_emision";
    str_query += ", SUM(documento_h_producto.cantidad) AS cantidad_bruto";
    str_query += ", IFNULL((SELECT SUM(d_h_prod_nc_cantidad.cantidad_aux) FROM documento d";
    str_query += " JOIN documento_h_documento d_h_d_nc ON d.id = d_h_d_nc.documento_id1";
    str_query += " JOIN documento d_nc ON (d_h_d_nc.documento_id = d_nc.id AND d_nc.tipo_documento_id = "+QString().setNum(tipo_documento::NOTA_CREDITO)+")";
    str_query += " JOIN documento_h_producto d_h_prod_nc_cantidad ON (d_h_prod_nc_cantidad.producto_id = "+producto_id+" AND d_h_prod_nc_cantidad.documento_id = d_nc.id)";
    str_query += " WHERE d.id = documento.id), 0) AS cantidad_nc";
    str_query += ", SUM(documento_h_producto.precio) AS precio";
    str_query += ", (SELECT SUM(documento_h_producto.precio) FROM documento_h_producto";
    str_query += " WHERE documento.id = documento_h_producto.documento_id) AS total";
    str_query += ", IFNULL((SELECT SUM(d_h_prod_flete.precio_aux) FROM documento d";
    str_query += " JOIN documento_h_documento d_h_d_flete ON d.id = d_h_d_flete.documento_id1";
    str_query += " JOIN documento d_flete ON (d_h_d_flete.documento_id = d_flete.id AND d_flete.tipo_documento_id = "+QString().setNum(tipo_documento::FLETE)+")";
    str_query += " JOIN documento_h_producto d_h_prod_flete ON (d_h_prod_flete.producto_id = "+producto_id+" AND d_h_prod_flete.documento_id = d_flete.id)";
    str_query += " WHERE d.id = documento.id), 0) AS precio_flete";
    str_query += ", IFNULL((SELECT SUM(d_h_prod_nc_desc.precio_aux) FROM documento d";
    str_query += " JOIN documento_h_documento d_h_d_nc ON d.id = d_h_d_nc.documento_id1";
    str_query += " JOIN documento d_nc ON (d_h_d_nc.documento_id = d_nc.id AND d_nc.tipo_documento_id = "+QString().setNum(tipo_documento::NOTA_CREDITO)+")";
    str_query += " JOIN documento_h_producto d_h_prod_nc_desc ON (d_h_prod_nc_desc.producto_id = "+producto_id+" AND d_h_prod_nc_desc.documento_id = d_nc.id)";
    str_query += " WHERE d.id = documento.id), 0) AS desc_nc";
    str_query += ", IFNULL((SELECT SUM(nc.monto) FROM documento d";
    str_query += " JOIN documento_h_documento d_h_d_nc ON d.id = d_h_d_nc.documento_id1";
    str_query += " JOIN documento d_nc ON (d_h_d_nc.documento_id = d_nc.id AND d_nc.tipo_documento_id = "+QString().setNum(tipo_documento::NOTA_CREDITO)+")";
    str_query += " JOIN nota_credito nc ON d_nc.id = nc.comprobante_documento_id";
    str_query += " WHERE d.id = documento.id), 0) AS nc_monto";
    str_query += " FROM producto";
    str_query += " JOIN documento_h_producto ON producto.id = documento_h_producto.producto_id";

    str_query += " JOIN documento ON ((documento.tipo_documento_id = "+QString().setNum(tipo_documento::SALDO);
    str_query += " OR documento.tipo_documento_id = "+QString().setNum(tipo_documento::BOLETA);
    str_query += " OR documento.tipo_documento_id = "+QString().setNum(tipo_documento::FACTURA)+")";
    str_query += " AND documento.id = documento_h_producto.documento_id)";
    str_query += " JOIN comprobante ON (comprobante.operacion_id = "+QString().setNum(operacion_items::COMPRA)+" AND documento.id = comprobante.documento_id)";

    str_query += " LEFT JOIN saldo ON saldo.comprobante_documento_id = documento.id";
    str_query += " LEFT JOIN boleta ON boleta.comprobante_documento_id = documento.id";
    str_query += " LEFT JOIN factura ON factura.comprobante_documento_id = documento.id";

    str_query += " JOIN anexo ON documento.id = anexo.documento_id";
    str_query += " LEFT JOIN documento_h_persona ON documento.id = documento_h_persona.documento_id";
    str_query += " LEFT JOIN persona ON persona.id = documento_h_persona.persona_id";
    str_query += " LEFT JOIN juridica ON persona.id = juridica.persona_id";
    str_query += " LEFT JOIN proveedor ON persona.id = proveedor.juridica_persona_id";
    str_query += " WHERE producto.id = "+producto_id;
    str_query += ") AS t)";

    str_query += " UNION ALL";

    str_query += "(SELECT";
    str_query += " SUM(documento_h_producto.cantidad) AS cantidad_bruto";
    str_query += ", IFNULL((SELECT SUM(d_h_prod_nc_cantidad.cantidad_aux)";
    str_query += " FROM documento d";
    str_query += " JOIN documento_h_documento d_h_d_nc ON d.id = d_h_d_nc.documento_id1";
    str_query += " JOIN documento d_nc ON (d_h_d_nc.documento_id = d_nc.id AND d_nc.tipo_documento_id = "+QString().setNum(tipo_documento::NOTA_CREDITO)+")";
    str_query += " JOIN documento_h_producto d_h_prod_nc_cantidad ON (d_h_prod_nc_cantidad.producto_id = "+producto_id+" AND d_h_prod_nc_cantidad.documento_id = d_nc.id)";
    str_query += " WHERE d.id = documento.id), 0) AS cantidad_nc";
    str_query += ", '', '', '', ''";
    str_query += " FROM producto";
    str_query += " JOIN documento_h_producto ON producto.id = documento_h_producto.producto_id";

    str_query += " JOIN documento ON ((documento.tipo_documento_id = "+QString().setNum(tipo_documento::BOLETA);
    str_query += " OR documento.tipo_documento_id = "+QString().setNum(tipo_documento::FACTURA)+")";
    str_query += " AND documento.id = documento_h_producto.documento_id)";
    str_query += " JOIN comprobante ON (comprobante.operacion_id = "+QString().setNum(operacion_items::VENTA)+" AND documento.id = comprobante.documento_id)";

    str_query += " LEFT JOIN boleta ON boleta.comprobante_documento_id = documento.id";
    str_query += " LEFT JOIN factura ON factura.comprobante_documento_id = documento.id";

    str_query += " JOIN anexo ON documento.id = anexo.documento_id";
    str_query += " WHERE producto.id = "+producto_id+")";

    str_query += " UNION ALL";

    str_query += "(SELECT";
    str_query += " SUM(documento_h_producto.cantidad) AS cantidad_bruto";
    str_query += ", '', '', '', '', ''";
    str_query += " FROM producto";
    str_query += " JOIN documento_h_producto ON producto.id = documento_h_producto.producto_id";

    str_query += " JOIN documento ON ((documento.tipo_documento_id = "+QString().setNum(tipo_documento::REGISTRO_SIN_DOCUMENTO);
    str_query += " OR documento.tipo_documento_id = "+QString().setNum(tipo_documento::NOTA_PEDIDO)+")";
    str_query += " AND documento.id = documento_h_producto.documento_id)";

    str_query += " JOIN comprobante ON (comprobante.operacion_id = "+QString().setNum(operacion_items::VENTA)+" AND documento.id = comprobante.documento_id)";

    str_query += " JOIN anexo ON documento.id = anexo.documento_id";
    str_query += " WHERE producto.id = "+producto_id+")";

    str_query += " UNION ALL";

    str_query += "(SELECT";
    str_query += " SUM(documento_h_producto.cantidad) AS cantidad_bruto";
    str_query += ", '', '', '', '', ''";
    str_query += " FROM producto";
    str_query += " JOIN documento_h_producto ON producto.id = documento_h_producto.producto_id";

    str_query += " JOIN documento ON ((documento.tipo_documento_id = "+QString().setNum(tipo_documento::REGISTRO_SIN_DOCUMENTO);
    str_query += " OR documento.tipo_documento_id = "+QString().setNum(tipo_documento::NOTA_PEDIDO)+")";
    str_query += " AND documento.id = documento_h_producto.documento_id)";

    str_query += " JOIN comprobante ON (comprobante.operacion_id = "+QString().setNum(operacion_items::VENTA)+" AND documento.id = comprobante.documento_id)";

    str_query += " JOIN (SELECT * FROM documento_h_documento GROUP BY documento_id1) AS d_h_d ON d_h_d.documento_id1 = documento.id";

    str_query += " JOIN documento d_b ON (d_b.tipo_documento_id = "+QString().setNum(tipo_documento::BOLETA);
    str_query += " AND d_h_d.documento_id = d_b.id)";

    str_query += " JOIN documento d_f ON (d_f.tipo_documento_id = "+QString().setNum(tipo_documento::FACTURA);
    str_query += " AND d_h_d.documento_id = d_f.id)";

    str_query += " JOIN anexo ON documento.id = anexo.documento_id";
    str_query += " WHERE producto.id = "+producto_id+")";

    str_query += " UNION ALL";

    str_query += "(SELECT";
    str_query += " SUM(documento_h_producto.cantidad) AS cantidad_bruto";
    str_query += ", '', '', '', '', ''";
    str_query += " FROM producto";

    str_query += " JOIN documento_h_producto ON producto.id = documento_h_producto.producto_id";
    str_query += " JOIN documento ON ((documento.tipo_documento_id = "+QString().setNum(tipo_documento::BOLETA);
    str_query += " OR documento.tipo_documento_id = "+QString().setNum(tipo_documento::FACTURA)+")";
    str_query += " AND documento.id = documento_h_producto.documento_id)";

    str_query += " JOIN comprobante ON (comprobante.operacion_id = "+QString().setNum(operacion_items::VENTA)+" AND documento.id = comprobante.documento_id)";

    str_query += " JOIN (SELECT * FROM documento_h_documento GROUP BY documento_id) AS d_h_d ON d_h_d.documento_id = documento.id";

    str_query += " JOIN documento d_rsn ON (d_rsn.tipo_documento_id = "+QString().setNum(tipo_documento::REGISTRO_SIN_DOCUMENTO);
    str_query += " AND d_h_d.documento_id1 = d_rsn.id)";

    str_query += " JOIN documento d_np ON (d_np.tipo_documento_id = "+QString().setNum(tipo_documento::NOTA_PEDIDO);
    str_query += " AND d_h_d.documento_id1 = d_np.id)";

    str_query += " JOIN anexo ON documento.id = anexo.documento_id";
    str_query += " WHERE producto.id = "+producto_id+")";

    qDebug()<<str_query<<endl;
    QSqlQuery query;
    if(query.exec(str_query)){
        query.next();
        double cant_compra = query.value(0).toDouble();
        QString cantidad_compra = QString().setNum(cant_compra, ' ', 4);

        double cant_c_nc = query.value(1).toDouble();
        QString cantidad_c_nc = QString().setNum(cant_c_nc, ' ', 4);

        double precio_compra = query.value(2).toDouble();

        double flete_compra = query.value(3).toDouble();

        double desc_c_nc = query.value(4).toDouble();

        double desc_c_nc_monto = query.value(5).toDouble();

        double precio_neto_val = precio_compra + flete_compra - desc_c_nc - desc_c_nc_monto;
        QString precio_neto = QString().setNum(precio_neto_val, ' ', 3);
        double prec_ganancia = precio_neto_val*1.25;
        QString precio_ganancia = QString().setNum(prec_ganancia, ' ', 3);

        ui->doubleSpinBox_cantidad_compra->setValue(cantidad_compra.toDouble());
        ui->doubleSpinBox_cantidad_c_nc->setValue(cantidad_c_nc.toDouble());
        ui->doubleSpinBox_precio_neto->setValue(precio_neto.toDouble());
        ui->doubleSpinBox_precio_ganancia->setValue(precio_ganancia.toDouble());

        query.next();
        double cant_venta = query.value(0).toDouble();
        QString cantidad_venta = QString().setNum(cant_venta, ' ', 4);

        double cant_v_nc = query.value(1).toDouble();
        QString cantidad_v_nc = QString().setNum(cant_v_nc, ' ', 4);
        ui->doubleSpinBox_cantidad_venta->setValue(cantidad_venta.toDouble());
        ui->doubleSpinBox_cantidad_v_nc->setValue(cantidad_v_nc.toDouble());

        query.next();
        double cant_venta_rsd_np = query.value(0).toDouble();

        query.next();
        double cant_venta_rsd_np_b_f = query.value(0).toDouble();

        query.next();
        double cant_venta_b_f_rsd_np = query.value(0).toDouble();

        double cant_v_nor = cant_venta_rsd_np - cant_venta_rsd_np_b_f;
        if((cant_venta_rsd_np_b_f - cant_venta_b_f_rsd_np) > 0.0) {
            cant_v_nor += (cant_venta_rsd_np_b_f - cant_venta_b_f_rsd_np);
        }
        QString cantidad_v_nor = QString().setNum(cant_v_nor, ' ', 3);
        ui->doubleSpinBox_cantidad_v_rsn_np->setValue(cantidad_v_nor.toDouble());

        double cant_total = cant_compra - cant_c_nc - cant_venta - cant_v_nor + cant_v_nc;
        //double cant_total = cant_compra - cant_c_nc - cant_venta + cant_v_nc;
        QString cantidad_total = QString().setNum(cant_total, ' ', 4);
        ui->doubleSpinBox_cantidad_total->setValue(cantidad_total.toDouble());
    }else{

    }
}

void VentaChartPrecio::llenar_grafica()
{
    QString str_query;
    str_query += "SELECT t.cantidad_bruto";
    str_query += ", t.cantidad_bruto - t.cantidad_nc";
    str_query += ", t.precio / t.cantidad_bruto AS precio_bruto";
    str_query += ", t.precio_flete / t.cantidad_bruto AS flete_unit";
    str_query += ", IF((t.cantidad_bruto - t.cantidad_nc) <> 0, t.desc_nc / (t.cantidad_bruto - t.cantidad_nc), 0) AS desc_nc_unit";
    str_query += ", IF((t.cantidad_bruto - t.cantidad_nc) <> 0, IF(t.total <> 0, (t.precio / t.total), 0) * t.nc_monto / (t.cantidad_bruto - t.cantidad_nc), 0) AS desc_nc_monto_unit";
    str_query += " FROM";
    str_query += " (SELECT";
    str_query += " documento.id AS id";
    str_query += ", documento.tipo_documento_id AS tipo_documento_id";
    str_query += ", anexo.fecha_emision AS fecha_emision";
    str_query += ", SUM(documento_h_producto.cantidad) AS cantidad_bruto";
    str_query += ", IFNULL((SELECT SUM(d_h_prod_nc_cantidad.cantidad_aux) FROM documento d";
    str_query += " JOIN documento_h_documento d_h_d_nc ON d.id = d_h_d_nc.documento_id1";
    str_query += " JOIN documento d_nc ON (d_h_d_nc.documento_id = d_nc.id AND d_nc.tipo_documento_id = "+QString().setNum(tipo_documento::NOTA_CREDITO)+")";
    str_query += " JOIN documento_h_producto d_h_prod_nc_cantidad ON (d_h_prod_nc_cantidad.producto_id = "+producto_id+" AND d_h_prod_nc_cantidad.documento_id = d_nc.id)";
    str_query += " WHERE d.id = documento.id), 0) AS cantidad_nc";
    str_query += ", SUM(documento_h_producto.precio) AS precio";
    str_query += ", (SELECT SUM(documento_h_producto.precio) FROM documento_h_producto";
    str_query += " WHERE documento.id = documento_h_producto.documento_id) AS total";
    str_query += ", IFNULL((SELECT SUM(d_h_prod_flete.precio_aux) FROM documento d";
    str_query += " JOIN documento_h_documento d_h_d_flete ON d.id = d_h_d_flete.documento_id1";
    str_query += " JOIN documento d_flete ON (d_h_d_flete.documento_id = d_flete.id AND d_flete.tipo_documento_id = "+QString().setNum(tipo_documento::FLETE)+")";
    str_query += " JOIN documento_h_producto d_h_prod_flete ON (d_h_prod_flete.producto_id = "+producto_id+" AND d_h_prod_flete.documento_id = d_flete.id)";
    str_query += " WHERE d.id = documento.id), 0) AS precio_flete";
    str_query += ", IFNULL((SELECT SUM(d_h_prod_nc_desc.precio_aux) FROM documento d";
    str_query += " JOIN documento_h_documento d_h_d_nc ON d.id = d_h_d_nc.documento_id1";
    str_query += " JOIN documento d_nc ON (d_h_d_nc.documento_id = d_nc.id AND d_nc.tipo_documento_id = "+QString().setNum(tipo_documento::NOTA_CREDITO)+")";
    str_query += " JOIN documento_h_producto d_h_prod_nc_desc ON (d_h_prod_nc_desc.producto_id = "+producto_id+" AND d_h_prod_nc_desc.documento_id = d_nc.id)";
    str_query += " WHERE d.id = documento.id), 0) AS desc_nc";
    str_query += ", IFNULL((SELECT SUM(nc.monto) FROM documento d";
    str_query += " JOIN documento_h_documento d_h_d_nc ON d.id = d_h_d_nc.documento_id1";
    str_query += " JOIN documento d_nc ON (d_h_d_nc.documento_id = d_nc.id AND d_nc.tipo_documento_id = "+QString().setNum(tipo_documento::NOTA_CREDITO)+")";
    str_query += " JOIN nota_credito nc ON d_nc.id = nc.comprobante_documento_id";
    str_query += " WHERE d.id = documento.id), 0) AS nc_monto";
    str_query += ", juridica.ruc AS ruc";
    str_query += ", juridica.razon_social AS razon_social";
    str_query += " FROM producto";
    str_query += " JOIN documento_h_producto ON producto.id = documento_h_producto.producto_id";

    str_query += " JOIN documento ON ((documento.tipo_documento_id = "+QString().setNum(tipo_documento::SALDO);
    str_query += " OR documento.tipo_documento_id = "+QString().setNum(tipo_documento::BOLETA);
    str_query += " OR documento.tipo_documento_id = "+QString().setNum(tipo_documento::FACTURA)+")";
    str_query += " AND documento.id = documento_h_producto.documento_id)";
    str_query += " JOIN comprobante ON (comprobante.operacion_id = "+QString().setNum(operacion_items::COMPRA)+" AND documento.id = comprobante.documento_id)";

    str_query += " LEFT JOIN saldo ON saldo.comprobante_documento_id = documento.id";
    str_query += " LEFT JOIN boleta ON boleta.comprobante_documento_id = documento.id";
    str_query += " LEFT JOIN factura ON factura.comprobante_documento_id = documento.id";

    str_query += " JOIN anexo ON documento.id = anexo.documento_id";
    str_query += " LEFT JOIN documento_h_persona ON documento.id = documento_h_persona.documento_id";
    str_query += " LEFT JOIN persona ON persona.id = documento_h_persona.persona_id";
    str_query += " LEFT JOIN juridica ON persona.id = juridica.persona_id";
    str_query += " LEFT JOIN proveedor ON persona.id = proveedor.juridica_persona_id";
    str_query += " WHERE producto.id = "+producto_id;
    str_query += " AND YEAR(anexo.fecha_emision) <= YEAR(curdate())";
    str_query += " AND YEAR(anexo.fecha_emision) >= YEAR(curdate())-4";
    str_query += " GROUP BY anexo.fecha_emision";
    str_query += " ORDER BY anexo.fecha_emision DESC) AS t";

    qDebug()<<str_query<<endl;
    QSqlQuery query;
    if(query.exec(str_query)){
        while(query.next()){
            int year = query.value(0).toInt();
            qreal precio_min = query.value(1).toReal();
            qreal precio_max = query.value(2).toReal();

            int cur_year = QDate::currentDate().year();
            values_min[year%(cur_year-4)] = precio_min;
            values_max[year%(cur_year-4)] = precio_max;
        }
        SYSTEM->table_resize_to_contents(0, ui->tableWidget);
    }else{

    }
}

void VentaChartPrecio::on_year_up()
{
    ui->dateEdit->setDate(QDate(ui->dateEdit->date().year()+1, 1, 1));
    int rowCount = ui->tableWidget->rowCount();
    for(int i=0; i < rowCount; i++)
        ui->tableWidget->removeRow(0);
    pos = 0;
    llenar_tabla();
}
void VentaChartPrecio::on_year_down()
{
    ui->dateEdit->setDate(QDate(ui->dateEdit->date().year()-1, 1, 1));
    int rowCount = ui->tableWidget->rowCount();
    for(int i=0; i < rowCount; i++)
        ui->tableWidget->removeRow(0);
    pos = 0;
    llenar_tabla();
}
void VentaChartPrecio::showEvent(QShowEvent *se)
{
    se->accept();
    ui->dateEdit->setFocus(Qt::TabFocusReason);
}

bool VentaChartPrecio::eventFilter(QObject *watched, QEvent *event)
{
    QWidget* w_temp;
    w_temp = this;
    if(watched == w_temp){
        if(event->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)event;
            switch(KeyEvent->key())
            {
            case Qt::Key_Escape:{
                SYSTEM->change_center_w(this, widget_previous);
                return true;
            }break;
            case Qt::Key_Up: {
                if (this->focusWidget() != ui->tableWidget) {
                    ui->tableWidget->setFocus(Qt::TabFocusReason);
                    if (ui->tableWidget->currentItem())
                        ui->tableWidget->currentItem()->setSelected(true);
                }
            }break;
            case Qt::Key_Down: {
                if (this->focusWidget() != ui->tableWidget) {
                    ui->tableWidget->setFocus(Qt::TabFocusReason);
                    if (ui->tableWidget->currentItem())
                        ui->tableWidget->currentItem()->setSelected(true);
                }
            }break;
            case Qt::Key_F4: {
                ui->pushButton_modificar->setFocus(Qt::TabFocusReason);
                ui->pushButton_modificar->click();
                return true;
            }break;
            }
        }
        return false;
    }
    w_temp = ui->tableWidget;
    if(w_temp == watched){
        if(event->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)event;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:
                //ui->pushButton_ok->click();
                return true;
            case Qt::Key_Down: {
                int index = ui->tableWidget->currentRow();
                if (index == ui->tableWidget->rowCount() - 1) {
                    llenar_tabla();
                }
            }break;
            }

        }else{

        }
        return false;
    }
    w_temp = ui->pushButton_ok;
    if(w_temp == watched){
        if(event->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)event;

            switch(KeyEvent->key())
            {
            case Qt::Key_Return:
                ui->pushButton_ok->click();
                return true;
            }

        }else{

        }
        return false;
    }
    w_temp = ui->pushButton_salir;
    if(w_temp == watched){
        if(event->type() == QEvent::KeyPress){
            QKeyEvent *KeyEvent = (QKeyEvent*)event;

            switch(KeyEvent->key())
            {
            case Qt::Key_Tab:
                this->setFocus();
            case Qt::Key_Return:
                ui->pushButton_salir->click();
                return true;
            }

        }else{

        }
        return false;
    }

    return eventFilter(watched, event);
}

void VentaChartPrecio::on_pushButton_ok_clicked()
{
    QTableWidget* tb = ui->tableWidget;
    QTableWidgetItem* item = tb->currentItem();

    if(!item){
        return;
    }
    int row = item->row();

    /*
    id = tb->item(row, 0)->text();
    persona_id = tb->item(row, 1)->text();
    fecha_emision = tb->item(row, 2)->text();
    serie = tb->item(row, 3)->text();
    numero = tb->item(row, 4)->text();
    codigo = tb->item(row, 5)->text();
    nombre = tb->item(row, 6)->text();
    */

    if(widget_previous){
        this->setAttribute(Qt::WA_DeleteOnClose);
        SYSTEM->change_center_w(this, widget_previous);
    }else{
        SYSTEM->clear_center_w(this);
    }
}

void VentaChartPrecio::on_pushButton_salir_clicked()
{
    if(widget_previous){
        this->setAttribute(Qt::WA_DeleteOnClose);
        SYSTEM->change_center_w(this, widget_previous);
    }else{
        SYSTEM->clear_center_w(this);
    }
}

void VentaChartPrecio::on_dateEdit_dateChanged(const QDate &date)
{    
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    pos = 0;

    llenar_tabla();
}

void VentaChartPrecio::on_pushButton_modificar_clicked()
{
    QTableWidget* tb = ui->tableWidget;
    QTableWidgetItem* item = tb->currentItem();

    if(!item){
        return;
    }

    int ret = QMessageBox::warning(this, "Advertencia", "¿Desea MODIFICAR los datos de esa COMPRA?", "Si", "No");
    switch(ret){
    case 0:{
        int tipo = ui->tableWidget->item(item->row(), 1)->text().toInt();
        switch(tipo)
        {
        case tipo_documento::FACTURA:{
            VentaFactura* w = new VentaFactura;
            w->set_widget_previous(this);
            QString id = tb->item(item->row(), 0)->text();
            QString persona_id = tb->item(item->row(), 12)->text();
            QString fecha_emision = tb->item(item->row(), 3)->text();
            QString serie = tb->item(item->row(), 10)->text();
            QString numero = tb->item(item->row(), 11)->text();
            QString codigo = tb->item(item->row(), 13)->text();
            QString nombre = tb->item(item->row(), 14)->text();
            QString direccion = tb->item(item->row(), 15)->text();

            //connect(w, SIGNAL(closing()), this, SLOT(on_compra_closing()));
            w->select(id, QString().setNum(tipo), persona_id, fecha_emision, serie, numero, codigo, nombre, direccion);
            SYSTEM->change_center_w(this, w);
        }break;
        case tipo_documento::BOLETA:{
            VentaBoleta* w = new VentaBoleta;
            w->set_widget_previous(this);
            QString id = tb->item(item->row(), 0)->text();
            QString persona_id = tb->item(item->row(), 12)->text();
            QString fecha_emision = tb->item(item->row(), 3)->text();
            QString serie = tb->item(item->row(), 10)->text();
            QString numero = tb->item(item->row(), 11)->text();
            QString codigo = tb->item(item->row(), 13)->text();
            QString nombre = tb->item(item->row(), 14)->text();
            QString direccion = tb->item(item->row(), 15)->text();

            //connect(w, SIGNAL(closing()), this, SLOT(on_compra_closing()));
            w->select(id, QString().setNum(tipo), persona_id, fecha_emision, serie, numero, codigo, nombre, direccion);
            SYSTEM->change_center_w(this, w);
        }break;
        }
    }break;
    case 1:{

    }break;
    }
}
