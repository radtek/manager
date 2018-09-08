#ifndef MYLIBRARY_GLOBAL_H
#define MYLIBRARY_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(MYLIBRARY_LIBRARY)
#  define MYLIBRARYSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MYLIBRARYSHARED_EXPORT Q_DECL_IMPORT
#endif

#include "share_static_include.h"
#include "share_typedef.h"

#include "snackbarinfo.h"
#include "lineeditdelegate.h"
#include "lineeditnumberdelegate.h"
#include "doublespinboxdelegate.h"

#define DECIMALS_CANTIDAD 4
#define DECIMALS_PRECIO_UNITARIO 4
#define DECIMALS_PRECIO_TOTAL 3

#endif // MYLIBRARY_GLOBAL_H
