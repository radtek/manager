#include "comprachartcosto.h"
#include "ui_comprachartcosto.h"

CompraChartCosto::CompraChartCosto(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CompraChartCosto)
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

    QAction *action_up = new QAction(this);
    QAction *action_down = new QAction(this);
    action_up->setShortcut(Qt::ALT + Qt::Key_Up);
    action_down->setShortcut(Qt::ALT + Qt::Key_Down);

    connect(action_up, SIGNAL(triggered(bool)), this, SLOT(on_year_up()));
    connect(action_down, SIGNAL(triggered(bool)), this, SLOT(on_year_down()));

    this->addAction(action_up);
    this->addAction(action_down);

    ui->tableWidget->hideColumn(0);

    this->installEventFilter(this);
    ui->tableWidget->installEventFilter(this);
    ui->pushButton_ok->installEventFilter(this);
    ui->pushButton_salir->installEventFilter(this);
}

CompraChartCosto::~CompraChartCosto()
{
    qDebug()<<"delete compra chart costo"<<endl;
    delete ui;
}
void CompraChartCosto::set_widget_previous(QWidget *widget_previous)
{
    this->widget_previous = widget_previous;
}
void CompraChartCosto::set_producto(QString producto_id
                                    , QString unidad, QString descripcion)
{
    ui->label_unidad->setText(unidad);
    ui->label_producto->setText(descripcion);

    llenar_datos(producto_id);
}
void CompraChartCosto::set_value_min(QVector<qreal> v)
{
    this->values_min = v;
}
void CompraChartCosto::set_value_max(QVector<qreal> v)
{
    this->values_max = v;
}
void CompraChartCosto::set_tw()
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
void CompraChartCosto::set_chart()
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
void CompraChartCosto::llenar_datos(QString producto_id)
{
    this->producto_id = producto_id;

    llenar_tabla();

    llenar_grafica();

    set_tw();

    set_chart();

}
void CompraChartCosto::llenar_tabla()
{
    QString str_query;
    str_query += "SELECT t.id, t.tipo_documento_id, t.fecha_emision, t.cantidad_bruto";
    str_query += ", t.cantidad_bruto - t.cantidad_nc";
    str_query += ", t.precio / t.cantidad_bruto AS precio_bruto";
    str_query += ", t.precio_flete / t.cantidad_bruto AS flete_unit";
    str_query += ", IF((t.cantidad_bruto - t.cantidad_nc) <> 0, t.desc_nc / (t.cantidad_bruto - t.cantidad_nc), 0) AS desc_nc_unit";
    str_query += ", IF((t.cantidad_bruto - t.cantidad_nc) <> 0, IF(t.total <> 0, (t.precio / t.total), 0) * t.nc_monto / (t.cantidad_bruto - t.cantidad_nc), 0) AS desc_nc_monto_unit";
    //str_query += ", t.precio_bruto + t.flete_unit -  desc_nc_unit - desc_monto_unit AS precio_neto";
    str_query += ", t.ruc AS ruc, t.razon_social AS razon_social FROM";
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
            if(tipo_documento_id == tipo_documento::SALDO){
                documento = "SALDO";
            }
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

            double flete = query.value(6).toDouble();
            QString precio_flete = QString().setNum(flete, ' ', 3);

            double desc_nc = query.value(7).toDouble();
            QString descuento_nc = QString().setNum(desc_nc, ' ', 3);

            double desc_nc_monto = query.value(8).toDouble();
            QString descuento_nc_monto = QString().setNum(desc_nc_monto, ' ', 3);

            double precio_neto_val = precio + flete - desc_nc - desc_nc_monto;
            QString precio_neto = QString().setNum(precio_neto_val, ' ', 3);

            QString ruc = query.value(9).toString();
            QString razon_social = query.value(10).toString();

            ui->tableWidget->setItem(rowCount, 0, new QTableWidgetItem(id));
            ui->tableWidget->setItem(rowCount, 1, new QTableWidgetItem(documento));
            ui->tableWidget->setItem(rowCount, 2, new QTableWidgetItem(fecha_emision));
            ui->tableWidget->setItem(rowCount, 3, new QTableWidgetItem(cantidad));
            ui->tableWidget->setItem(rowCount, 4, new QTableWidgetItem(cantidad_final));
            ui->tableWidget->setItem(rowCount, 5, new QTableWidgetItem(precio_bruto));
            ui->tableWidget->setItem(rowCount, 6, new QTableWidgetItem(precio_flete));
            ui->tableWidget->setItem(rowCount, 7, new QTableWidgetItem(descuento_nc));
            ui->tableWidget->setItem(rowCount, 8, new QTableWidgetItem(descuento_nc_monto));
            ui->tableWidget->setItem(rowCount, 9, new QTableWidgetItem(precio_neto));
            ui->tableWidget->setItem(rowCount, 10, new QTableWidgetItem(ruc));
            ui->tableWidget->setItem(rowCount, 11, new QTableWidgetItem(razon_social));

            pos++;
        }
        SYSTEM->table_resize_to_contents(0, ui->tableWidget);
    }else{

    }
}
void CompraChartCosto::llenar_grafica()
{
    QString str_query;
    str_query += "SELECT YEAR(anexo.fecha_emision)";
    str_query += ", MIN(documento_h_producto.precio/documento_h_producto.cantidad)";
    str_query += ", MAX(documento_h_producto.precio/documento_h_producto.cantidad)";
    str_query += " FROM producto";
    str_query += " JOIN documento_h_producto ON producto.id = documento_h_producto.producto_id";
    str_query += " JOIN documento ON documento.id = documento_h_producto.documento_id";
    str_query += " JOIN comprobante ON (comprobante.operacion_id = "+QString().setNum(operacion_items::COMPRA)+" AND documento.id = comprobante.documento_id)";
    str_query += " LEFT JOIN boleta ON boleta.comprobante_documento_id = documento.id";
    str_query += " LEFT JOIN factura ON factura.comprobante_documento_id = documento.id";
    str_query += " JOIN anexo ON documento.id = anexo.documento_id";
    str_query += " JOIN documento_h_persona ON documento.id = documento_h_persona.documento_id";
    str_query += " JOIN persona ON persona.id = documento_h_persona.persona_id";
    str_query += " JOIN juridica ON persona.id = juridica.persona_id";
    str_query += " JOIN proveedor ON persona.id = proveedor.juridica_persona_id";
    str_query += " WHERE producto.id = "+producto_id;
    str_query += " AND YEAR(anexo.fecha_emision) <= YEAR(curdate())";
    str_query += " AND YEAR(anexo.fecha_emision) >= YEAR(curdate())-4";
    str_query += " GROUP BY anexo.fecha_emision";
    str_query += " ORDER BY anexo.fecha_emision DESC";
    //str_query += " LIMIT "+QString().setNum(pos)+", "+QString().setNum(size_query);

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

void CompraChartCosto::on_year_up()
{
    ui->dateEdit->setDate(QDate(ui->dateEdit->date().year()+1, 1, 1));
    int rowCount = ui->tableWidget->rowCount();
    for(int i=0; i < rowCount; i++)
        ui->tableWidget->removeRow(0);
    pos = 0;
    llenar_tabla();
}
void CompraChartCosto::on_year_down()
{
    ui->dateEdit->setDate(QDate(ui->dateEdit->date().year()-1, 1, 1));
    int rowCount = ui->tableWidget->rowCount();
    for(int i=0; i < rowCount; i++)
        ui->tableWidget->removeRow(0);
    pos = 0;
    llenar_tabla();
}
void CompraChartCosto::showEvent(QShowEvent *se)
{
    se->accept();
    ui->dateEdit->setFocus(Qt::TabFocusReason);
}

bool CompraChartCosto::eventFilter(QObject *watched, QEvent *event)
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
            }
            case Qt::Key_Down: {
                if (this->focusWidget() != ui->tableWidget) {
                    ui->tableWidget->setFocus(Qt::TabFocusReason);
                    if (ui->tableWidget->currentItem())
                        ui->tableWidget->currentItem()->setSelected(true);
                }
            }
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

void CompraChartCosto::on_pushButton_ok_clicked()
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

void CompraChartCosto::on_pushButton_salir_clicked()
{
    if(widget_previous){
        this->setAttribute(Qt::WA_DeleteOnClose);
        SYSTEM->change_center_w(this, widget_previous);
    }else{
        SYSTEM->clear_center_w(this);
    }
}
