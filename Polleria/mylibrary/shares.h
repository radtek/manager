#ifndef SHARES

#define SHARES

#include "configuracion.h"
#include "login.h"
#include "managersystem.h"
#include "toolbar.h"

#define CONFIGURACION SINGLETON(Configuracion)

#define APP_LOGIN SINGLETON(Login)

#define APP_MAINWINDOW SINGLETON(ManagerSystem)

#define APP_TOOLBAR SINGLETON(ToolBar)

//#define APP_TAB_WIDGET SINGLETON(MyTabWidget)


#endif // SHARES

