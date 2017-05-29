#ifndef SHARE_TYPEDEF_H
#define SHARE_TYPEDEF_H

// STRUCTS
#include "singleton.h"
#include "sistema.h"
//MACROS DEL SISTEMA

    //#define APP_SHOW_MAXIMIZED SINGLETON(MainWindow)->showMaximized()

    //#define APP_STATUS_BAR(X) SINGLETON(ui_main)->statusBar()->showMessage(X);
    //#define APP_STATUS_BAR_TIME(X,Y) SINGLETON(ui_main)->statusBar()->showMessage(X,Y)

    //LOG
/*
    #define SYS_LOG_FILE(X) SINGLETON(system_log)->mf_set_file_name_log(X)
    #define SYS_LOGX_FILE(X) SINGLETON(system_log)->mf_set_file_name_log_error(X)

    #define SYS_APP(X) SINGLETON(system_log)->mf_set_name_app(X)
    #define SYS_CLASS(X) SINGLETON(system_log)->mf_set_name_class(X)
    #define SYS_FILE(X) SINGLETON(system_log)->mf_set_name_file(X)
    #define SYS_FUNCTION(X) SINGLETON(system_log)->mf_set_name_function(X)
    #define SYS_STATE(X) SINGLETON(system_log)->mf_set_state(X)
    #define SYS_COMMENT(X) SINGLETON(system_log)->mf_set_comment(X)

    #define SYS_WRITE SINGLETON(system_log)->write()
    #define SYS_WRITE_COMMENT(X) SINGLETON(system_log)->write(X)
	*/

    //states de el LOG

    #define STATE_STAR_APP "STAR_APP"
    #define STATE_END_APP "END_APP"

    #define STATE_STAR "STAR"
    #define STATE_END "END"
    #define STATE_OK "OK"
    #define STATE_ERROR "ERROR"
    #define STATE_FAILED "FAILED"

    //BASE DE DATOS

    #define DATABASE SINGLETON(database_connection)
    #define DATABASE_CONFIG(X) SINGLETON(database_connection)->mf_load_configuration(X)
    #define DATABASE_CONNECT SINGLETON(database_connection)->mf_connect()
    //#define DATABASE_SET_CONFIG SINGLETON(database_connection)->mf_connect()

    //SESSION
/*
    #define USER_GET_NAME SINGLETON(session)->mf_get_name()
    #define USER_GET_NICK_NAME SINGLETON(session)->mf_get_nick_name()
    #define USER_GET_PERMISSIONS SINGLETON(session)->mf_get_permissions()
    #define USER_GET_DNI SINGLETON(session)->mf_get_code()

    #define USER_SET_NAME(X) SINGLETON(session)->mf_set_name(X)
    #define USER_SET_NICK_NAME(X) SINGLETON(session)->mf_set_nick_name(X)
    #define USER_SET_PERMISSIONS(X) SINGLETON(session)->mf_set_permissions(X)
    #define USER_SET_DNI(X) SINGLETON(session)->mf_set_code(X)
	*/
    //Sistema

    //static Sistema* singleton_sistema = singleton<Sistema>::instance();
    #define SYSTEM SINGLETON(Sistema)


    //Mensajes de sistema

    #define MESSAGE_ALERT(X,Y) SYSTEM->messageAlert(X,Y)
    #define MESSAGE_INFORMATION(X,Y) SYSTEM->messageInformation(X,Y)
    #define MESSAGE_QUESTION(X,Y) SYSTEM->messageQuestion(X,Y)
    #define MESSAGE_CRITICAL(X,Y) SYSTEM->messageCritical(X,Y)


//CONSTANTES DEL SISTEMA

    //sistema en general

    #define C_ABOUT_MANAGERM_TITLE "Acerca de ManagerM"
    #define C_ABOUT_MANAGERM_DESCRIPCION "ManagerM 1.0\n\nCopyright 2013 - Todos los Derechos reservados.\n\nDesarrolladores\n\nCondori Fernandez Milton.\nLuna Choquecota Alexander. \nJose Luis Sotomayor Malqui. \nJan Franco LLerena Quenaya."

    #define C_APP_NAME "MANAGERM"

    #define C_LOG_FILE_NAME "managerm"
    #define C_LOG_FILE_ERROR_NAME "managermx"

    //toolbar

    #define C_TOOLBAR_SIZE 80

    //base de datos

    #define C_DB_FILE_CONFIG_NAME "config"
    #define C_DB_FILE_DEFAULT_CONFIG_NAME "config.cfg"

    //login

    #define C_LOGIN_UNLOGGED "Sesion no iniciada." //pensar en otro mensaje :/
    //#define C_LOGIN_LOGGED_NAME "Bienvenido" + USER_GET_NAME + "." // revisar esto
    #define C_LOGIN_LOGGED "Bienvenido."

    #define C_ERROR_REGISTRO_ARTICULO "Ocurrio un error al momento de registrar, intente de nuevo"
    #define C_ERROR_ELIMINAR_ARTICULO "Ocurrio un error al momento de eliminar, intente de nuevo"
    #define C_CORRECTO_REGISTRO_ARTICULO "Los articulos fueron registrados correctamente"

    #define C_HABILITADO "1"
    #define C_NO_HABILITADO  "0"

    #define C_HEADER_CANTIDAD "Cantidad"

    //MENSAJES QMessageBox
    #define C_CAMPOS_VACIOS "Campos Vacios"
    #define C_CAMPOS_VACIOS_TEXT "Los campos se encuentran vacios"
    #define C_CAMBIO_INCORRECTO "Cambio incorrecto"
    #define C_CAMBIO_CORRECTO "Cambio correcto de informacion personal"
    #define C_CAMBIO_CORRECTO_TEXT "Los cambios fueron exitosos"
    #define C_WRONG_PASSWORD "Password incorrecta"
    #define C_LENGTH_PASSWORD "El pasword debe tener como minimo 6 caracteres"
    #define C_CAMBIAR_TEXT "Seguro que desea cambiar sus datos?"
    #define C_CAMBIAR_TITLE "Cambiar datos"
    #define C_ERROR "Error"
    #define C_NO_STOCK "No se puede realizar la operacion. No hay suficiente stock"
    #define C_WRONG_COMPROBANTE "Error en almacenamiento"
    #define C_WRONG_COMPROBANTE_TEXT "No se pudo guardar el comprobante. Vuelva a intentar"
    #define C_COMPROBANTE "Comprobante guardado"
    #define C_COMPROBANTE_TEXT "El registro de comprobante fue exitoso"

namespace tipo_documento {
    enum{REGISTRO_SIN_DOCUMENTO=1, BOLETA, FACTURA, NOTA_PEDIDO, GUIA_REMISION_REMITENTE
         , COTIZACION, NOTA_CREDITO, NOTA_DEBITO, FLETE, ORDEN, SALDO};
}
namespace tipo_persona {
    enum{PROVEEDOR=1, TRANSPORTISTA, CLIENTE_RUC, CLIENTE_DNI, USUARIO};
}
// FUNCIONES DEL SISTEMA

//CONSULTAS A LA DB

//HEADERS
//ENUM ARTICULOS
//enum Art{COD,GRUPO,MARCA,MEDIDA,DESCRIPCION,PRECIO,STOCK,HABILITADO,CANTIDAD};

//ENUM COLABORADORES
//enum col{COL_DNI,COL_TIENDA,COL_NOMBRES,COL_APAT,COL_AMAT,COL_DIRECCION,COL_FNAC,COL_SEXO,COL_ESTCIVIL,
//     COL_CELULAR,COL_TLFONO,COL_EMAIL,COL_COMMENT,COL_HABILITADO};

//ENUM PERSONA_JURIDICA
//enum pj{PJ_RUC,PJ_REGION,PJ_NOMBRE,PJ_DIRECCION,PJ_TLFONO,PJ_FAX,PJ_REPRESENTANTE,PJ_EMAIL,PJ_PAGWEB,
 //   PJ_COMENTARIO,PJ_HABILITADO};




#endif // SHARE_TYPEDEF_H
