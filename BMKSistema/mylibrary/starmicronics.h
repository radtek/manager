#ifndef STARMICRONICS_H
#define STARMICRONICS_H

#include "mylibrary_global.h"

#ifdef WIN32

#include <wchar.h>
#include <stdio.h>
#include <starmicronics/Platform.h>
#include <starmicronics/platform/Win32.h>
#include <starmicronics/StarIOPort.h>

// Unicode
#define CONSOLE_OUT wprintf
#define CONSOLE_IN wscanf
#define CONSOLE_IN_CHAR getwchar

#define STR_CMP wcscmp
#define STR_TO_INT _wtoi

#define STR_ELEM wchar_t
#define STR(x) L ## x

const char g_portName[100 + 1] = "tcp:192.168.1.36";
const char g_portSettings[100 + 1] = "";
const UINT32 g_portTimeout = 10 * 1000;

#else

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>

// ansi
#define CONSOLE_OUT printf
#define CONSOLE_IN scanf
#define CONSOLE_IN_CHAR getchar

#define STR_CMP strcmp
#define STR_TO_INT atoi

#define STR_ELEM char
#define STR(x) x

const char g_portName[100 + 1] = "tcp:192.168.1.36";
const char g_portSettings[100 + 1] = "";
const UINT32 g_portTimeout = 10 * 1000;

#endif

class MYLIBRARYSHARED_EXPORT StarMicronics
{
public:
    StarMicronics();

    void * openPortHelper();
    INT64 WritePortHelper(void * port, UCHAR const * writeBuffer, UINT32 writeBufferSize);
    void printSampleReceipt();
    void printSampleReceiptInJapanese();
    void printSampleReceiptErrorRecovery();
    void checkedBlock();
    void oneDBarcodes();
    void twoDBarcodes();
    void kickCashDrawer();
    void getParsedStatus();
    void codePages();
    void fonts();
    void feed();
    void autoCutter();
    void resetPrinter();

};

#endif // STARMICRONICS_H
