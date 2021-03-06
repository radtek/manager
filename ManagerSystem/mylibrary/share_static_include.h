#ifndef SHARE_STATIC_INCLUDE_H
#define SHARE_STATIC_INCLUDE_H

//INCLUDES

#include <QMainWindow>
#include <QWidget>
#include <QApplication>

#include <QStatusBar>
#include <QWebEngineView>
#include <QWebEnginePage>
#include <QWebEngineProfile>
#include <QWebEngineCookieStore>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QSharedData>
#include <QDebug>
#include <QSqlError>
#include <QShortcut>
#include <QtGui>
#include <QMenu>
#include <QPixmap>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>
#include <QVariant>
#include <QSqlRecord>
#include <QTableWidget>
#include <QDebug>
#include <QHeaderView>
#include <QSqlQueryModel>
#include <QSqlRelationalTableModel>
#include <QModelIndex>
#include <QSqlRelationalTableModel>
#include <QTableView>
#include <QDateTimeEdit>
#include <QStandardItemModel>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QEvent>
#include <QKeyEvent>
#include <QList>
#include <QStandardItemModel>
#include <QTreeWidget>
#include <QListWidget>
#include <QStringList>
#include <QTreeWidget>
#include <QTextEdit>
#include <QTextDocument>
#include <QTimer>
#include <QCloseEvent>
#include <QHideEvent>
#include <QShowEvent>
#include <QMouseEvent>
#include <QFormLayout>
#include <QDate>
#include <QScrollBar>
#include <QHeaderView>
#include <QCompleter>
#include <QFrame>
#include <QTime>
#include <QTimer>
#include <QObject>
#include <QPrinter>
#include <QPrinterInfo>
#include <QPrintDialog>
#include <QPainter>
#include <QFile>
#include <QFileDialog>
#include <QLabel>
#include <QItemDelegate>
#include <QCheckBox>
#include <QStylePainter>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QGuiApplication>
#include <QDoubleSpinBox>

#define STR_GUIA "guia"
#define STR_FACTURA "factura"
#define STR_FLETE "flete"
#define STR_PAGOS "pagos"
#define STR_N_CREDITO "n_credito"
#define STR_N_DEBITO "n_debito"

//TYPEDEFS

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

namespace operacion_items{
    enum{COMPRA=1, VENTA=2};
}

namespace moneda_items{
    enum{SOL=1, DOLAR=2, EURO=3};
}

enum opciones_categoria{articulo,proveedor,banco,tienda,comprobante,general,base_datos};
enum opciones_articulo{marca,medida,grupo};
enum tipos_comprobante{boleta=1,factura,proforma,cotizacion};


//enum type_link{PUSHBUTTON};
enum table_marks{MANAGER_MARK=-1, MARCA_MARK, MEDIDA_MARK, ARTICULO_MARK, PROVEEDOR_MARK, FACTURA_MARK};
// 5

enum query_types{IS_SUBQUERY=0, IS_NULL, IS_INT, IS_BOOL, IS_TEXT};

enum transactions{INGRESAR=0, MODIFICAR, ELIMINAR, BUSCAR, CONFIGURAR, MAS, SALIR};

enum sub_modules{BUSCAR_COMPRA= 1, BUSCAR_VENTA_FACTURA= 2, BUSCAR_TYPE= 3, BUSCAR_MARCA= 4, BUSCAR_MEDIDA= 5
                 , BUSCAR_PRODUCTO= 6, BUSCAR_PROVEEDOR= 7
                 , BUSCAR_TRANSPORTISTA= 8, BUSCAR_CLIENTE_CON_RUC= 9, BUSCAR_USUARIO= 10
                 , BUSCAR_VENTA= 11, CONFIGURACION= 12, ELIMINAR_COMPRA= 13, ELIMINAR_MARCA= 15, ELIMINAR_MEDIDA= 16
                 , ELIMINAR_PRODUCTO= 17, ELIMINAR_PROVEEDOR= 18, ELIMINAR_TRANSPORTISTA= 19
                 , ELIMINAR_VENTA= 20, INGRESAR_COMPRA= 21
                 , INGRESAR_MARCA= 22, INGRESAR_MEDIDA= 23, INGRESAR_PRODUCTO= 24
                 , INGRESAR_PROVEEDOR= 25, INGRESAR_TRANSPORTISTA= 26
                 , INGRESAR_VENTA= 27, MODIFICAR_COMPRA= 28, MODIFICAR_MARCA= 29
                 , MODIFICAR_MEDIDA= 30, MODIFICAR_PRODUCTO= 31
                 , MODIFICAR_PROVEEDOR= 32, MODIFICAR_TRANSPORTISTA= 33, MODIFICAR_VENTA= 34
                 , BUSCAR_VENTA_BOLETA= 35, BUSCAR_CLIENTE_CON_DNI= 36, BUSCAR_VENTA_PROFORMA= 37
                , BUSCAR_VENTA_NOTA_PEDIDO= 38, BUSCAR_VENTA_COTIZACION= 39
                , BUSCAR_VENTA_DOC_CLIENTE= 40};
enum sub_modules_extra{BUSCAR_PRODUCTO_COMPRADO= 100, BUSCAR_PRODUCTO_VENTA= 101};

namespace ns_group_of_items{
	enum group_of_items {
		DEPARTAMENTO = 8, DOCUMENTO = 3, MARCA = 5, MEDIDA = 6, MONEDA = 9, PAIS = 7
		, PERSONA = 2, PRODUCTO = 1, TIPO = 4
	};

}

namespace items{
	enum items {
		ACTIVO = 90, AMAZONAS = 97, ANCASH = 98, ANULADO = 92, APURIMAC = 99, AREQUIPA = 100
		, AYACUCHO = 101, BANCO = 9, BOLETA = 79, B_COMPRA = 64, B_CONSIGNACION = 68
		, B_DEVOLUCION = 62, B_EXPORTACION = 69, B_IMPORTACION = 63, B_OTROS = 70, B_TRANSFORMACION = 67
		, B_TRASLADO_EMISOR_PAGO = 65, B_VENTA = 66, CAJAMARCA = 102, CALLAO = 103, CANTIDAD = 37
		, CLIENTE_CON_DNI = 11, CLIENTE_CON_RUC = 10, CODIGO = 28, CODIGO_VEHICULO = 72, COMENTARIO = 22
		, COMPRA = 77, COSTO_MINIMO_TRASLADO = 60, COTIZACION = 89, CUZCO = 104, DECLARACION = 30
		, DESCRIPCION_OPC = 39, DESCUENTO = 47, DESCUENTO_CANTIDAD = 52, DESCUENTO_PRECIO_TOTAL = 54, DESCUENTO_PRECIO_UNITARIO = 53
		, DESCUENTO_VALOR = 55, DIRECCION = 16, DOLAR = 123, DOLAR_CAMBIO = 94, D_DE_LLEGADA = 59
		, D_DE_PARTIDA = 58, EMAIL = 21, EMBALAJE = 48, ESTADO = 46, EURO = 124
		, FACTURA = 80, FAX = 18, FECHA_INICIO_TRASLADO = 74, FLETE = 81, FLETE_VALOR = 49
		, GUIA_REMISION_REMITENTE = 82, HUANCAVELICA = 105, HUANUCO = 106, ICA = 107, JUNIN = 108
		, LAMBAYEQUE = 110, LA_LIBERTAD = 109, LICENCIA_CONDUCIR = 73, LIMA = 111, LORETO = 112
		, MADRE_DE_DIOS = 113, MARCA = 2, MARCA_PLACA = 71, MEDIDA_OPC = 38, MODALIDAD = 33
		, MOQUEGUA = 114, MOTIVO = 75, NOMBRE = 29, NOMBRE_COMERCIAL = 13, NOTA_CREDITO = 85
		, NOTA_DEBITO = 86, NOTA_PEDIDO = 84, NO_ACTIVO = 93, NUEVO_SOL = 122, NUMERO = 32
		, NUMERO_FACTURA = 57, OBSERVACION = 76, ORDEN = 83, PAGINA_WEB = 20, PASCO = 115
		, PASS = 26, PENDIENTE = 91, PERCEPCION = 36, PERCEPCION_OPCION = 35, PERU = 96
		, PER_D_CONDICION = 15, PER_D_ESTADO = 14, PESO_TOTAL = 61, PIURA = 116, PRECIO_TOTAL = 41
		, PRECIO_TOTAL_2 = 43, PRECIO_TOTAL_NETO = 45, PRECIO_T_FLETE = 51, PRECIO_UNITARIO = 40, PRECIO_UNITARIO_2 = 42
		, PRECIO_UNITARIO_NETO = 44, PRECIO_U_FLETE = 50, PROD_APODO = 4, PROD_UBICACION = 5, PROFORMA = 88
		, PROVEEDOR = 7, PUNO = 117, REGISTRO_SIN_DOCUMENTO = 95, REPRESENTANTE = 19, SAN_MARTIN = 118
		, SERIE = 31, SERIE_FACTURA = 56, TACNA = 119, TELEFONO = 17, TICKET = 87
		, TIPO = 1, TIPO_CONTRIBUYENTE = 12, TIPO_MONEDA = 34, TIPO_USUARIO = 27, TRANSPORTISTA = 8
		, TUMBES = 120, UCAYALI = 121, UNIDAD = 3, USUARIO = 6, VENDEDOR_NOMBRE = 23
		, VENDEDOR_RPC = 24, VENDEDOR_RPM = 25, VENTA = 78
	};
}

enum lw_colab{OPC_PROVEEDOR=0, OPC_TRANSPORTISTA, OPC_CLIENTE_CON_RUC, OPC_CLIENTE_CON_DNI, OPC_USUARIO};
enum lw_doc_sel{OPC_GUIA=0, OPC_FACTURA, OPC_FLETE, OPC_LETRAS, OPC_N_CREDITO, OPC_N_DEBITO};
//#include "include/share_typedef.h"
enum widget_types{PUSHBUTTON, TABLEWIDGET};

#endif // SHARE_STATIC_INCLUDE_H
