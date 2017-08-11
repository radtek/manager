#include "starmicronics.h"

StarMicronics::StarMicronics()
{

}
//By calling this function, it will use the user's input to connect to the printer, returns null if something went wrong.
void * StarMicronics::openPortHelper()
{
    //void * port = OpenPort((const char*)L"TCP:192.168.1.36", (const char*)L"", g_portTimeout);
    void * port = NULL;
    /*
    if(port == NULL)
    {
        CONSOLE_OUT(STR("Failed when trying to open port! Check your connection settings.\n"));
        return NULL;
    }*/
    while ((port = OpenPort((const char*)L"tcp:192.168.1.36", (const char*)L"9100,n,8,1,h", g_portTimeout)) == 0)
    {
        if (GetLastError() == ERROR_ALREADY_EXISTS) // ALREADY_EXISTS indicates another process's usage - try again
        {
            Sleep(100); // suspend the thread - save the earth

            continue; // try again
        }
        else
        {
            return port; // this is a different kind of failure - dead
        }
    }
    return port;
}

//This function will let you send bytes to the printer. Arguments include the port handle, the buffered data, and the size of the data.
INT64 StarMicronics::WritePortHelper(void * port, UCHAR const * writeBuffer, UINT32 writeBufferSize)
{
    UINT zeroProgressOccurances = 0;
    UINT32 totalSizeCommunicated = 0;
    while ((totalSizeCommunicated < writeBufferSize) && (zeroProgressOccurances < 2)) // adjust zeroProgressOccurances threshold as required
    {
        UINT32 sizeCommunicated = 0;
        if(WritePort(port, writeBuffer + totalSizeCommunicated, writeBufferSize - totalSizeCommunicated, &sizeCommunicated) == 0)
        {
            return -1;
        }

        if (sizeCommunicated == 0)
        {
            zeroProgressOccurances++;
        }
        else
        {
            totalSizeCommunicated += sizeCommunicated;
            zeroProgressOccurances = 0;
        }
    }

    return (INT64)totalSizeCommunicated;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|
//     Sample Receipt (in English)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|
// This function will print a sample receipt to your Star Printer.
// In this code block, we will using WritePortHelper to write out data to the printer.
// For command codes and examples on how to use special functions in the printer, look at the sample receipt string.
// You can find these command codes and more in the Star Line Mode Thermal Printer Command Manual.
// To access this documentation, open the SDK Readme for links on where to get it.
void StarMicronics::printSampleReceipt()
{
    void * port = OpenPort((const char*)"tcp:192.168.1.36", (const char*)"9100", 5*1000);
    if(port == NULL)
    {
        qDebug() << "Failed to open port! Check your connection settings.\n";
        return;
    }

    //Set an array to push our bytes into for our receipt
    UCHAR command[801];
    UCHAR* temportaryString;
    size_t commandSize = 0;

    memcpy(command, "\x1b\x1d\x61\x01", 4); //Center Alignment - Refer to Pg. 3-29
    commandSize += 4;

    temportaryString = (UCHAR*)"[If Loaded.. Logo1 goes here]\r\n";
    memcpy(command + commandSize, temportaryString, strlen((CHAR*)temportaryString));
    commandSize += strlen((CHAR*)temportaryString);

    memcpy(command + commandSize, "\x1b\x1c\x70\x01\x00\r\n", 7); //Stored Logo Printing - Refer to Pg. 3-38
    commandSize += 7;

    temportaryString = (UCHAR*)"Star Clothing Boutique\r\n";
    memcpy(command + commandSize, temportaryString, strlen((CHAR*)temportaryString));
    commandSize += strlen((CHAR*)temportaryString);

    temportaryString = (UCHAR*)"1150 King Georges Post Rd.\r\nEdison, NJ, 08837\r\n\r\n";
    memcpy(command + commandSize, temportaryString, strlen((CHAR*)temportaryString));
    commandSize += strlen((CHAR*)temportaryString);

    memcpy(command + commandSize, "\x1b\x1d\x61\x00", 4); //Left Alignment - Refer to Pg. 3-29
    commandSize += 4;

    memcpy(command + commandSize, "\x1b\x44\x02\x10\x22\x00", 6);  //Setting Horizontal Tab - Pg. 3-27
    commandSize += 6;

    temportaryString = (UCHAR*)"Date: 5/25/2011";
    memcpy(command + commandSize, temportaryString, strlen((CHAR*)temportaryString));
    commandSize += strlen((CHAR*)temportaryString);

    memcpy(command + commandSize, " \x09 ", 3);  //Moving Horizontal Tab - Pg. 3-26
    commandSize += 3;

    temportaryString = (UCHAR*)"Time: 9:10 AM\r\n------------------------------------------------\r\n\r\n";
    memcpy(command + commandSize, temportaryString, strlen((CHAR*)temportaryString));
    commandSize += strlen((CHAR*)temportaryString);

    memcpy(command + commandSize, "\x1b\x45", 2);
    commandSize += 2;

    temportaryString = (UCHAR*)"SALE \r\n";
    memcpy(command + commandSize, temportaryString, strlen((CHAR*)temportaryString));
    commandSize += strlen((CHAR*)temportaryString);

    memcpy(command + commandSize, "\x1b\x46", 2);
    commandSize += 2;

    temportaryString = (UCHAR*)"SKU \x09";
    memcpy(command + commandSize, temportaryString, strlen((CHAR*)temportaryString));
    commandSize += strlen((CHAR*)temportaryString);

    temportaryString = (UCHAR*)" Description   \x09         Total\r\n";
    memcpy(command + commandSize, temportaryString, strlen((CHAR*)temportaryString));
    commandSize += strlen((CHAR*)temportaryString);

    temportaryString = (UCHAR*)"300678566 \x09  PLAN T-SHIRT\x09         10.99\r\n";
    memcpy(command + commandSize, temportaryString, strlen((CHAR*)temportaryString));
    commandSize += strlen((CHAR*)temportaryString);

    temportaryString = (UCHAR*)"300692003 \x09  BLACK DENIM\x09         29.99\r\n";
    memcpy(command + commandSize, temportaryString, strlen((CHAR*)temportaryString));
    commandSize += strlen((CHAR*)temportaryString);

    temportaryString = (UCHAR*)"300651148 \x09  BLUE DENIM\x09         29.99\r\n";
    memcpy(command + commandSize, temportaryString, strlen((CHAR*)temportaryString));
    commandSize += strlen((CHAR*)temportaryString);

    temportaryString = (UCHAR*)"300642980 \x09  STRIPE DRESS\x09         49.99\r\n";
    memcpy(command + commandSize, temportaryString, strlen((CHAR*)temportaryString));
    commandSize += strlen((CHAR*)temportaryString);

    temportaryString = (UCHAR*)"300638471 \x09  BLACK BOOT\x09         35.99\r\n\r\n";
    memcpy(command + commandSize, temportaryString, strlen((CHAR*)temportaryString));
    commandSize += strlen((CHAR*)temportaryString);

    temportaryString = (UCHAR*)"Subtotal \x09\x09        156.95\r\n";
    memcpy(command + commandSize, temportaryString, strlen((CHAR*)temportaryString));
    commandSize += strlen((CHAR*)temportaryString);

    temportaryString = (UCHAR*)"Tax \x09\x09         00.00\r\n";
    memcpy(command + commandSize, temportaryString, strlen((CHAR*)temportaryString));
    commandSize += strlen((CHAR*)temportaryString);

    temportaryString = (UCHAR*)"------------------------------------------------\r\n";
    memcpy(command + commandSize, temportaryString, strlen((CHAR*)temportaryString));
    commandSize += strlen((CHAR*)temportaryString);

    temportaryString = (UCHAR*)"Total";
    memcpy(command + commandSize, temportaryString, strlen((CHAR*)temportaryString));
    commandSize += strlen((CHAR*)temportaryString);

    memcpy(command + commandSize, "\x06\x09\x1b\x69\x01\x01", 6);
    commandSize += 6;

    temportaryString = (UCHAR*)"        $156.95\r\n";
    memcpy(command + commandSize, temportaryString, strlen((CHAR*)temportaryString));
    commandSize += strlen((CHAR*)temportaryString);

    memcpy(command + commandSize, "\x1b\x69\x00\x00", 4);  //Cancel Character Expansion - Pg. 3-10
    commandSize += 4;

    temportaryString = (UCHAR*)"------------------------------------------------\r\n\r\n";
    memcpy(command + commandSize, temportaryString, strlen((CHAR*)temportaryString));
    commandSize += strlen((CHAR*)temportaryString);

    temportaryString = (UCHAR*)"Charge\r\n159.95\r\n";
    memcpy(command + commandSize, temportaryString, strlen((CHAR*)temportaryString));
    commandSize += strlen((CHAR*)temportaryString);

    temportaryString = (UCHAR*)"Visa XXXX-XXXX-XXXX-0123\r\n\r\n";
    memcpy(command + commandSize, temportaryString, strlen((CHAR*)temportaryString));
    commandSize += strlen((CHAR*)temportaryString);

    temportaryString = (UCHAR*)"\x1b\x34Regimds and Exchanges\x1b\x35\r\n";  //Specify/Cencel White/Black Invert - Pg. 3-16
    memcpy(command + commandSize, temportaryString, strlen((CHAR*)temportaryString));
    commandSize += strlen((CHAR*)temportaryString);

    temportaryString = (UCHAR*)"Within \x1b\x2d\x01";
    memcpy(command + commandSize, temportaryString, strlen((CHAR*)temportaryString));
    commandSize += strlen((CHAR*)temportaryString);

    temportaryString = (UCHAR*)"30 days";
    memcpy(command + commandSize, temportaryString, strlen((CHAR*)temportaryString));
    commandSize += strlen((CHAR*)temportaryString);

    memcpy(command + commandSize, "\x1b\x2d\x00", 3);  //Specify/Cancel Underline Printing - Pg. 3-15
    commandSize += 3;

    temportaryString = (UCHAR*)" with receipt\r\n";
    memcpy(command + commandSize, temportaryString, strlen((CHAR*)temportaryString));
    commandSize += strlen((CHAR*)temportaryString);

    temportaryString = (UCHAR*)"And tags attached\r\n\r\n";
    memcpy(command + commandSize, temportaryString, strlen((CHAR*)temportaryString));
    commandSize += strlen((CHAR*)temportaryString);

    memcpy(command + commandSize, "\x1b\x1d\x61\x01", 4);
    commandSize += 4;

    memcpy(command + commandSize, "\x1b\x62\x06\x02\x02", 5);
    commandSize += 5;

    temportaryString = (UCHAR*)" 12ab34cd56\x1e\r\n";
    memcpy(command + commandSize, temportaryString, strlen((CHAR*)temportaryString));
    commandSize += strlen((CHAR*)temportaryString);

    memcpy(command + commandSize, "\x1b\x64\x02", 3);  //Cut  - Pg. 3-41
    commandSize += 3;

    INT64 totalSizeCommunicated = WritePortHelper(port, command, commandSize);
    if (totalSizeCommunicated == -1)
    {
        qDebug() << "Failure - WritePort.\n";
        ClosePort(port);
        return;
    }
    else if (totalSizeCommunicated != (INT64)commandSize)
    {
        qDebug() << "Error - not all data could be written out.\n";
        ClosePort(port);
        return;
    }

    ClosePort(port);

    qDebug() << "Success\n";
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|
//     Sample Receipt (in Japanese)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|
// This function will print a sample receipt in Japanese to your Star Printer.
// Note:
//     The printer should have the Japanese font in it.
//     The printer should be set to DBCS (MSW0-4 = 1) and Shift JIS Kanji mode (MSW0-5 = 0).
//
void StarMicronics::printSampleReceiptInJapanese()
{
    void * port = OpenPort(g_portName, g_portSettings, g_portTimeout);
    if(port == NULL)
    {
        CONSOLE_OUT(STR("Failed to open port! Check your connection settings.\n"));
        return;
    }

    //Set an array to push our bytes into for our receipt
    UCHAR command[801];
    UCHAR* temportaryString;
    size_t commandSize = 0;

    memcpy(command + commandSize, "\x1b\x40\x0d\x0a", 4); // <ESC> @ <CR> <LF>
    commandSize += 4;

    temportaryString = (UCHAR*)"        ============= ";
    memcpy(command + commandSize, temportaryString, strlen((CHAR*)temportaryString));
    commandSize += strlen((CHAR*)temportaryString);

    memcpy(command + commandSize, "\x1b\x24\x31", 3);       // <ESC> $ 1 ... Shift JIS Kanji mode (on)
    commandSize += 3;

    memcpy(command + commandSize, "\x1b\x45", 2); // <ESC> E ... emphasis (on)
    commandSize += 2;

    memcpy(command + commandSize, "\x94\x5b\x95\x69\x8f\x91", 6); // Japanese text
    commandSize += 6;

    memcpy(command + commandSize, "\x1b\x46", 2); // <ESC> F ... emphasis (off)
    commandSize += 2;

    temportaryString = (UCHAR*)" =============";
    memcpy(command + commandSize, temportaryString, strlen((CHAR*)temportaryString));
    commandSize += strlen((CHAR*)temportaryString);

    memcpy(command + commandSize, "\x0d\x0a", 2); // <CR><LF>
    commandSize += 2;

    memcpy(command + commandSize, "\x93\x60\x95\x5b\x93\xfa\x95\x74\x81\x46", 10); // Japanese text
    commandSize += 10;

    temportaryString = (UCHAR*)"2011/12/15";
    memcpy(command + commandSize, temportaryString, strlen((CHAR*)temportaryString));
    commandSize += strlen((CHAR*)temportaryString);

    memcpy(command + commandSize, "\x0d\x0a", 2); // <CR><LF>
    commandSize += 2;

    memcpy(command + commandSize, "\x93\x60\x95\x5b\x20\x4e\x6f\x2e\x81\x46\x30\x39\x38\x37\x36\x35\x34\x33\x32\x31", 20); // Japanese text
    commandSize += 20;

    memcpy(command + commandSize, "\x0d\x0a", 2); // <CR><LF>
    commandSize += 2;

    memcpy(command + commandSize, "\x93\xbe\x88\xd3\x90\xe6\x43\x44\x81\x46\x31\x32\x33\x34\x35\x36\x37\x38\x39\x30", 20); // Japanese text
    commandSize += 20;

    memcpy(command + commandSize, "\x0d\x0a\x0d\x0a", 4); // <CR><LF><CR><LF>
    commandSize += 4;

    memcpy(command + commandSize, "\x1b\x69\x31\x31", 4); // <ESC> i 1 1 ... magnification (x2)
    commandSize += 4;

    memcpy(command + commandSize, "\x1b\x2d\x31", 3); // <ESC> - 1 ... underline (on)
    commandSize += 3;

    temportaryString = (UCHAR*)"*                    ";
    memcpy(command + commandSize, temportaryString, strlen((CHAR*)temportaryString));
    commandSize += strlen((CHAR*)temportaryString);

    memcpy(command + commandSize, "\x97\x6c", 2); // Japanese text
    commandSize += 2;

    memcpy(command + commandSize, "\x1b\x2d\x30", 3); // <ESC> - 0 ... underline (off)
    commandSize += 3;

    memcpy(command + commandSize, "\x1b\x69\x30\x30", 4); // <ESC> i 0 0 ... magnification (x1)
    commandSize += 4;

    memcpy(command + commandSize, "\x0d\x0a", 2); // <CR><LF>
    commandSize += 2;

    memcpy(command + commandSize, "\x1b\x2d\x31", 3); // <ESC> - 1 ... underline (on)
    commandSize += 3;

    memcpy(command + commandSize, "\x20\x4e\x6f\x2e\x20\x95\x69\x96\xbc\x20\x20\x20\x20\x90\x94\x97\xca\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20", 28); // Japanese text
    commandSize += 28;

    memcpy(command + commandSize, "\x92\x50\x89\xbf\x20\x20\x20\x20\x20\x20\x20\x20\x20", 13); // Japanese text
    commandSize += 13;

    memcpy(command + commandSize, "\x8b\xe0\x8a\x7a", 4); // Japanese text
    commandSize += 4;

    memcpy(command + commandSize, "\x1b\x2d\x30", 3); // <ESC> - 0 ... underline (off)
    commandSize += 3;

    memcpy(command + commandSize, "\x0d\x0a", 2); // <CR><LF>
    commandSize += 2;

    memcpy(command + commandSize, "\x20\x31\x2e\x20\x20\x97\xce\x92\x83\x20\x35\x30\x30\x6d\x6c", 15); // Japanese text
    commandSize += 15;

    memcpy(command + commandSize, "\x0d\x0a", 2); // <CR><LF>
    commandSize += 2;

    temportaryString = (UCHAR*)"            4 case         2,304         9,216";
    memcpy(command + commandSize, temportaryString, strlen((CHAR*)temportaryString));
    commandSize += strlen((CHAR*)temportaryString);

    memcpy(command + commandSize, "\x0d\x0a\x0d", 3); // <CR><LF><CR>
    commandSize += 3;

    memcpy(command + commandSize, "\x20\x32\x2e\x20\x20\x97\xce\x92\x83\x20\x33\x35\x30\x6d\x6c", 15); // Japanese text
    commandSize += 15;

    memcpy(command + commandSize, "\x0d\x0a", 2); // <CR><LF>
    commandSize += 2;

    temportaryString = (UCHAR*)"            4 case         2,088         8,352";
    memcpy(command + commandSize, temportaryString, strlen((CHAR*)temportaryString));
    commandSize += strlen((CHAR*)temportaryString);

    memcpy(command + commandSize, "\x0d\x0a\x0d", 3); // <CR><LF><CR>
    commandSize += 3;

    memcpy(command + commandSize, "\x20\x33\x2e\x20\x20\x8d\x67\x92\x83\x20\x35\x30\x30\x6d\x6c", 15); // Japanese text
    commandSize += 15;

    memcpy(command + commandSize, "\x0d\x0a", 2); // <CR><LF>
    commandSize += 2;

    temportaryString = (UCHAR*)"            4 case         2,304         9,216";
    memcpy(command + commandSize, temportaryString, strlen((CHAR*)temportaryString));
    commandSize += strlen((CHAR*)temportaryString);

    memcpy(command + commandSize, "\x0d\x0a\x0d", 3); // <CR><LF><CR>
    commandSize += 3;

    memcpy(command + commandSize, "\x20\x34\x2e\x20\x20\x89\x47\x97\xb4\x92\x83\x20\x35\x30\x30\x6d\x6c", 17); // Japanese text
    commandSize += 17;

    memcpy(command + commandSize, "\x0d\x0a", 2); // <CR><LF>
    commandSize += 2;

    temportaryString = (UCHAR*)"            4 case         2,304         9,216";
    memcpy(command + commandSize, temportaryString, strlen((CHAR*)temportaryString));
    commandSize += strlen((CHAR*)temportaryString);

    memcpy(command + commandSize, "\x0d\x0a\x0d\x0a", 4); // <CR><LF><CR><LF>
    commandSize += 4;

    memcpy(command + commandSize, "\x1b\x2d\x31", 3); // <ESC> - 1 ... underline (on)
    commandSize += 3;

    memcpy(command + commandSize, "\x20\x20\x20\x20\x8d\x87\x8c\x76\x20\x20\x34\x95\x69\x96\xda\x20\x20\x20", 18); // Japanese text
    commandSize += 18;

    memcpy(command + commandSize, "\x31\x36\x20\x63\x61\x73\x65\x20\x20\x30\x20\x96\x7b\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20", 28); // Japanese text
    commandSize += 28;

    memcpy(command + commandSize, "\x1b\x2d\x30", 3); // <ESC> - 0 ... underline (off)
    commandSize += 3;

    memcpy(command + commandSize, "\x0d\x0a", 2); // <CR><LF>
    commandSize += 2;

    memcpy(command + commandSize, "\x8d\xa1\x89\xf1\x94\x5b\x95\x69\x8d\x87\x8c\x76", 12); // Japanese text
    commandSize += 12;

    temportaryString = (UCHAR*)"                         36,000";
    memcpy(command + commandSize, temportaryString, strlen((CHAR*)temportaryString));
    commandSize += strlen((CHAR*)temportaryString);

    memcpy(command + commandSize, "\x89\x7e\x0a\x0d", 4); // Japanese text <CR><LF>
    commandSize += 4;

    temportaryString = (UCHAR*)"  5.00%";
    memcpy(command + commandSize, temportaryString, strlen((CHAR*)temportaryString));
    commandSize += strlen((CHAR*)temportaryString);

    memcpy(command + commandSize, "\x92\x6c\x88\xf8\x82\xab", 6); // Japanese text
    commandSize += 6;

    temportaryString = (UCHAR*)"                          1,800";
    memcpy(command + commandSize, temportaryString, strlen((CHAR*)temportaryString));
    commandSize += strlen((CHAR*)temportaryString);

    memcpy(command + commandSize, "\x89\x7e\x0a\x0d\x0a\x0d", 6); // Japanese text <CR><LF><CR><LF>
    commandSize += 6;

    memcpy(command + commandSize, "\x1b\x2d\x31", 3); // <ESC> - 1 ... underline (on)
    commandSize += 3;

    memcpy(command + commandSize, "\x8f\xac\x8c\x76", 4); // Japanese text
    commandSize += 4;

    temportaryString = (UCHAR*)"                                  34,200";
    memcpy(command + commandSize, temportaryString, strlen((CHAR*)temportaryString));
    commandSize += strlen((CHAR*)temportaryString);

    memcpy(command + commandSize, "\x89\x7e", 2); // Japanese text
    commandSize += 2;

    memcpy(command + commandSize, "\x1b\x2d\x30", 3); // <ESC> - 0 ... underline (off)
    commandSize += 3;

    memcpy(command + commandSize, "\x0d\x0a", 2); // <CR><LF>
    commandSize += 2;

    memcpy(command + commandSize, "\x81\xa6\x8f\xe3\x8b\x4c\x8b\xe0\x8a\x7a\x82\xc9\x82\xcd\x8f\xc1\x94\xef\x90\xc5\x82\xf0\x8a\xdc\x82\xf1\x82\xc5\x82\xa2\x82\xdc\x82\xb7\x81\x42", 36); // Japanese text
    commandSize += 36;

    memcpy(command + commandSize, "\x0d\x0a\x0d\x0a", 4); // <CR><LF><CR><LF>
    commandSize += 4;

    memcpy(command + commandSize, "\x8f\x8a\x91\xae\x81\x46\x88\xc1\x8c\xb4\x8f\x6f\x92\xa3\x8f\x8a", 16); // Japanese text
    commandSize += 16;

    memcpy(command + commandSize, "\x0d\x0a", 2); // <CR><LF>
    commandSize += 2;

    memcpy(command + commandSize, "\x92\x53\x93\x96\x81\x46\x90\xaf\x20\x91\xbe\x98\x59", 13); // Japanese text
    commandSize += 13;

    temportaryString = (UCHAR*)"             Tel";
    memcpy(command + commandSize, temportaryString, strlen((CHAR*)temportaryString));
    commandSize += strlen((CHAR*)temportaryString);

    memcpy(command + commandSize, "\x81\x46", 2);
    commandSize += 2;

    temportaryString = (UCHAR*)"012-345-6789";
    memcpy(command + commandSize, temportaryString, strlen((CHAR*)temportaryString));
    commandSize += strlen((CHAR*)temportaryString);

    memcpy(command + commandSize, "\x0d\x0a", 2); // <CR><LF>
    commandSize += 2;

    memcpy(command + commandSize, "\x1b\x40", 2); // <ESC>@
    commandSize += 2;

    memcpy(command + commandSize, "\x0d\x0a\x0d\x0a\x0d\x0a", 6); // <CR><LF><CR><LF><CR><LF>
    commandSize += 6;

    memcpy(command + commandSize, "\x1b\x64\x33", 2); // <ESC> d 3 ... cut command
    commandSize += 3;

    INT64 totalSizeCommunicated = WritePortHelper(port, command, commandSize);
    if (totalSizeCommunicated == -1)
    {
        CONSOLE_OUT(STR("Failure - WritePort.\n"));
        ClosePort(port);
        return;
    }
    else if (totalSizeCommunicated != (INT64)commandSize)
    {
        CONSOLE_OUT(STR("Error - not all data could be written out.\n"));
        ClosePort(port);
        return;
    }

    ClosePort(port);

    CONSOLE_OUT(STR("Success\n"));
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|
//   Print a sample receipt (+ Error Recovery)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|
// This function will print a sample receipt to your Star Printer.
// In this code block, we will using WritePortHelper to write out data to the printer.
// For command codes and examples on how to use special functions in the printer, look at the sample receipt string.
// You can find these command codes and more in the Star Line Mode Thermal Printer Command Manual.
// To access this documentation, open the SDK Readme for links on where to get it.
// Enables the printer to completely recover from a failed print job due to an error occurring
// in the middle of printing.
void StarMicronics::printSampleReceiptErrorRecovery()
{
    void * port = OpenPort(g_portName, g_portSettings, g_portTimeout);
    if(port == NULL)
    {
        CONSOLE_OUT(STR("Failed to open port! Check your connection settings.\n"));
        return;
    }

    //Set an array to push our bytes into for our receipt
    UCHAR command[801 + 6 * 3];
    UCHAR* temportaryString;
    size_t commandSize = 0;

    // Command A : <ESC><GS><ETX><EOT><NUL><NUL><ESC><GS><ETX><ETX><NUL><NUL>
    memcpy(command + commandSize, "\x1b\x1d\x03\x04\x0\x0\x1b\x1d\x03\x03\x0\x0", 12);
    commandSize += 12;

    memcpy(command + commandSize, "\x1b\x1d\x61\x01", 4); //Center Alignment - Refer to Pg. 3-29
    commandSize += 4;

    temportaryString = (UCHAR*)"[If Loaded.. Logo1 goes here]\r\n";
    memcpy(command + commandSize, temportaryString, strlen((CHAR*)temportaryString));
    commandSize += strlen((CHAR*)temportaryString);

    memcpy(command + commandSize, "\x1b\x1c\x70\x01\x00\r\n", 7); //Stored Logo Printing - Refer to Pg. 3-38
    commandSize += 7;

    temportaryString = (UCHAR*)"Star Clothing Boutique\r\n";
    memcpy(command + commandSize, temportaryString, strlen((CHAR*)temportaryString));
    commandSize += strlen((CHAR*)temportaryString);

    temportaryString = (UCHAR*)"1150 King Georges Post Rd.\r\nEdison, NJ, 08837\r\n\r\n";
    memcpy(command + commandSize, temportaryString, strlen((CHAR*)temportaryString));
    commandSize += strlen((CHAR*)temportaryString);

    memcpy(command + commandSize, "\x1b\x1d\x61\x00", 4); //Left Alignment - Refer to Pg. 3-29
    commandSize += 4;

    memcpy(command + commandSize, "\x1b\x44\x02\x10\x22\x00", 6);  //Setting Horizontal Tab - Pg. 3-27
    commandSize += 6;

    temportaryString = (UCHAR*)"Date:12/14/2011";
    memcpy(command + commandSize, temportaryString, strlen((CHAR*)temportaryString));
    commandSize += strlen((CHAR*)temportaryString);

    memcpy(command + commandSize, " \x09 ", 3);  //Moving Horizontal Tab - Pg. 3-26
    commandSize += 3;

    temportaryString = (UCHAR*)"Time: 9:10 AM\r\n------------------------------------------------\r\n\r\n";
    memcpy(command + commandSize, temportaryString, strlen((CHAR*)temportaryString));
    commandSize += strlen((CHAR*)temportaryString);

    memcpy(command + commandSize, "\x1b\x45", 2);
    commandSize += 2;

    temportaryString = (UCHAR*)"SALE \r\n";
    memcpy(command + commandSize, temportaryString, strlen((CHAR*)temportaryString));
    commandSize += strlen((CHAR*)temportaryString);

    memcpy(command + commandSize, "\x1b\x46", 2);
    commandSize += 2;

    temportaryString = (UCHAR*)"SKU \x09";
    memcpy(command + commandSize, temportaryString, strlen((CHAR*)temportaryString));
    commandSize += strlen((CHAR*)temportaryString);

    temportaryString = (UCHAR*)" Description   \x09         Total\r\n";
    memcpy(command + commandSize, temportaryString, strlen((CHAR*)temportaryString));
    commandSize += strlen((CHAR*)temportaryString);

    temportaryString = (UCHAR*)"300678566 \x09  PLAN T-SHIRT\x09         10.99\r\n";
    memcpy(command + commandSize, temportaryString, strlen((CHAR*)temportaryString));
    commandSize += strlen((CHAR*)temportaryString);

    temportaryString = (UCHAR*)"300692003 \x09  BLACK DENIM\x09         29.99\r\n";
    memcpy(command + commandSize, temportaryString, strlen((CHAR*)temportaryString));
    commandSize += strlen((CHAR*)temportaryString);

    temportaryString = (UCHAR*)"300651148 \x09  BLUE DENIM\x09         29.99\r\n";
    memcpy(command + commandSize, temportaryString, strlen((CHAR*)temportaryString));
    commandSize += strlen((CHAR*)temportaryString);

    temportaryString = (UCHAR*)"300642980 \x09  STRIPE DRESS\x09         49.99\r\n";
    memcpy(command + commandSize, temportaryString, strlen((CHAR*)temportaryString));
    commandSize += strlen((CHAR*)temportaryString);

    temportaryString = (UCHAR*)"300638471 \x09  BLACK BOOT\x09         35.99\r\n\r\n";
    memcpy(command + commandSize, temportaryString, strlen((CHAR*)temportaryString));
    commandSize += strlen((CHAR*)temportaryString);

    temportaryString = (UCHAR*)"Subtotal \x09\x09        156.95\r\n";
    memcpy(command + commandSize, temportaryString, strlen((CHAR*)temportaryString));
    commandSize += strlen((CHAR*)temportaryString);

    temportaryString = (UCHAR*)"Tax \x09\x09         00.00\r\n";
    memcpy(command + commandSize, temportaryString, strlen((CHAR*)temportaryString));
    commandSize += strlen((CHAR*)temportaryString);

    temportaryString = (UCHAR*)"------------------------------------------------\r\n";
    memcpy(command + commandSize, temportaryString, strlen((CHAR*)temportaryString));
    commandSize += strlen((CHAR*)temportaryString);

    temportaryString = (UCHAR*)"Total";
    memcpy(command + commandSize, temportaryString, strlen((CHAR*)temportaryString));
    commandSize += strlen((CHAR*)temportaryString);

    memcpy(command + commandSize, "\x06\x09\x1b\x69\x01\x01", 6);
    commandSize += 6;

    temportaryString = (UCHAR*)"        $156.95\r\n";
    memcpy(command + commandSize, temportaryString, strlen((CHAR*)temportaryString));
    commandSize += strlen((CHAR*)temportaryString);

    memcpy(command + commandSize, "\x1b\x69\x00\x00", 4);  //Cancel Character Expansion - Pg. 3-10
    commandSize += 4;

    temportaryString = (UCHAR*)"------------------------------------------------\r\n\r\n";
    memcpy(command + commandSize, temportaryString, strlen((CHAR*)temportaryString));
    commandSize += strlen((CHAR*)temportaryString);

    temportaryString = (UCHAR*)"Charge\r\n159.95\r\n";
    memcpy(command + commandSize, temportaryString, strlen((CHAR*)temportaryString));
    commandSize += strlen((CHAR*)temportaryString);

    temportaryString = (UCHAR*)"Visa XXXX-XXXX-XXXX-0123\r\n\r\n";
    memcpy(command + commandSize, temportaryString, strlen((CHAR*)temportaryString));
    commandSize += strlen((CHAR*)temportaryString);

    temportaryString = (UCHAR*)"\x1b\x34Regimds and Exchanges\x1b\x35\r\n";  //Specify/Cencel White/Black Invert - Pg. 3-16
    memcpy(command + commandSize, temportaryString, strlen((CHAR*)temportaryString));
    commandSize += strlen((CHAR*)temportaryString);

    temportaryString = (UCHAR*)"Within \x1b\x2d\x01";
    memcpy(command + commandSize, temportaryString, strlen((CHAR*)temportaryString));
    commandSize += strlen((CHAR*)temportaryString);

    temportaryString = (UCHAR*)"30 days";
    memcpy(command + commandSize, temportaryString, strlen((CHAR*)temportaryString));
    commandSize += strlen((CHAR*)temportaryString);

    memcpy(command + commandSize, "\x1b\x2d\x00", 3);  //Specify/Cancel Underline Printing - Pg. 3-15
    commandSize += 3;

    temportaryString = (UCHAR*)" with receipt\r\n";
    memcpy(command + commandSize, temportaryString, strlen((CHAR*)temportaryString));
    commandSize += strlen((CHAR*)temportaryString);

    temportaryString = (UCHAR*)"And tags attached\r\n\r\n";
    memcpy(command + commandSize, temportaryString, strlen((CHAR*)temportaryString));
    commandSize += strlen((CHAR*)temportaryString);

    memcpy(command + commandSize, "\x1b\x1d\x61\x01", 4);
    commandSize += 4;

    memcpy(command + commandSize, "\x1b\x62\x06\x02\x02", 5);
    commandSize += 5;

    temportaryString = (UCHAR*)" 12ab34cd56\x1e\r\n";
    memcpy(command + commandSize, temportaryString, strlen((CHAR*)temportaryString));
    commandSize += strlen((CHAR*)temportaryString);

    memcpy(command + commandSize, "\x1b\x64\x02", 3);  //Cut  - Pg. 3-41
    commandSize += 3;

    // Command B : <ESC><GS><ETX><EOT><NUL><NUL>
    memcpy(command + commandSize, "\x1b\x1d\x03\x04\x0\x0", 6);
    commandSize += 6;

    INT64 totalSizeCommunicated = WritePortHelper(port, command, commandSize);
    if (totalSizeCommunicated == -1)
    {
        CONSOLE_OUT(STR("Failure - WritePort.\n"));
        ClosePort(port);
        return;
    }
    else if (totalSizeCommunicated != (INT64)commandSize)
    {
        CONSOLE_OUT(STR("Error - not all data could be written out.\n"));
        ClosePort(port);
        return;
    }

    ClosePort(port);

    CONSOLE_OUT(STR("Success\n"));
}

//Get status using StarIO to retreive our status bytes and parse it for us to offer a readable status via Boolean Logic.
void StarMicronics::getParsedStatus()
{
    CONSOLE_OUT(STR("Current status of the Star Printer:\n\n"));

    void * port = OpenPort(g_portName, g_portSettings, g_portTimeout);
    if (port == NULL)
    {
        CONSOLE_OUT(STR("Failure - OpenPort\n"));
        return;
    }

    //Create status structure and get parsed status with StarIO
    StarPrinterStatus_2 starPrinterStatus;
    if (GetParsedStatusEx(port, &starPrinterStatus, 2) == 0)
    {
        CONSOLE_OUT(STR("Failure - GetParsedStatusEx\n"));
        ClosePort(port);
        return;
    }

    //Use our status structure to check booleans to see if certain statuses are true or not
    if (starPrinterStatus.offline == SM_TRUE)
        CONSOLE_OUT(STR("Offline.\n"));
    else if (starPrinterStatus.offline == SM_FALSE)
        CONSOLE_OUT(STR("Online.\n"));
    if (starPrinterStatus.coverOpen == SM_TRUE)
        CONSOLE_OUT(STR("Cover Open.\n"));
    if (starPrinterStatus.receiptPaperNearEmptyInner == SM_TRUE)
        CONSOLE_OUT(STR("Paper Near Empty.\n"));
    if (starPrinterStatus.receiptPaperEmpty == SM_TRUE)
        CONSOLE_OUT(STR("Paper Empty.\n"));
    if (starPrinterStatus.compulsionSwitch == SM_FALSE)
        CONSOLE_OUT(STR("Drawer Closed.\n"));
    else if (starPrinterStatus.compulsionSwitch == SM_TRUE)
        CONSOLE_OUT(STR("Drawer Open.\n"));

    ClosePort(port);
            CONSOLE_OUT(STR("\n"));
}


//ETB Checked Block - Verifies the data has been received by the printer
void StarMicronics::checkedBlock()
{
    CONSOLE_OUT(STR("\nStarting Checked Block (ETB)...\n"));

    void * port = OpenPort(g_portName, g_portSettings, g_portTimeout);
    if (port == NULL)
    {
        CONSOLE_OUT(STR("Failure - OpenPort\n"));
        return;
    }

    StarPrinterStatus_2 starPrinterStatus;

    if (BeginCheckedBlockEx(port, &starPrinterStatus, 2) == SM_FALSE)
    {
        CONSOLE_OUT(STR("Failure - BeginCheckedBlockEx\n"));
        ClosePort(port);
        return;
    }
    if(starPrinterStatus.offline == SM_TRUE)
    {
        CONSOLE_OUT(STR("Error - Cannot proceed with printing because the printer is offline.\n"));
        ClosePort(port);
        return;
    }

    UCHAR const * command = (UCHAR const *)"Hello World!\n";
    UINT32 const commandSize = sizeof("Hello World!\n") - 1;

    for (int count = 0; count < 50; count++)
    {
        INT64 totalSizeCommunicated = WritePortHelper(port, command, commandSize);
        if (totalSizeCommunicated == -1)
        {
            CONSOLE_OUT(STR("Failure - WritePort.\n"));
            ClosePort(port);
            return;
        }
        else if (totalSizeCommunicated != (INT64)commandSize)
        {
            CONSOLE_OUT(STR("Error - not all data could be written out.\n"));
            ClosePort(port);
            return;
        }
    }

    if (EndCheckedBlockEx(port, &starPrinterStatus, 2) == SM_FALSE)
    {
        CONSOLE_OUT(STR("Failure - EndCheckedBlockEx\n"));
        ClosePort(port);
        return;
    }
    if(starPrinterStatus.offline == SM_TRUE)
    {
        CONSOLE_OUT(STR("Error - The printer is offline indicating that the printing failed.\n"));
        ClosePort(port);
        return;
    }

    ClosePort(port);

    CONSOLE_OUT(STR("End Transmission Block Successful.\n"));
}

//1D Barcodes (Code 39, I2of5, Code 93, Code 128)
void StarMicronics::oneDBarcodes()
{
    bool showMenu = true;
    //Display what command is being sent.
    CONSOLE_OUT(STR("\n1D BARCODES\n ASCII: ESC b n1 n2 n3 n4 d1 ... dk RS\n HEX:   1B 62 n1 n2 n3 n4 d1 ... dk 1E\n\n n1 = Barcode type\n n2 = Layout\n n3 = Barcode size selection\n n4 = Barcode Height (In Dots)\n\n"));
    //1D Barcode While Loop
    while(showMenu)
    {
        bool barcodeSelected = false;
        //Use our helper function to open the port to the printer
        void * port = openPortHelper();
        if(port == NULL) //If the port is NULL then the port was not successfully opened
            return;

        CONSOLE_OUT(STR("1D BARCODES MENU\n"));
        CONSOLE_OUT(STR(" [1] Code 39\n"));
        CONSOLE_OUT(STR(" [2] Interleaved 2 of 5\n"));
        CONSOLE_OUT(STR(" [3] Code 93\n"));
        CONSOLE_OUT(STR(" [4] Code 128\n"));
        CONSOLE_OUT(STR(" [m] Back to Main Menu\n"));

        STR_ELEM answerString[10];

        CONSOLE_OUT(STR("Please choose a barcode to print: "));
        CONSOLE_IN(STR("%s"), answerString); CONSOLE_IN_CHAR(); //read input

        if(STR_CMP(answerString, STR("m")) == 0)
        {
        ClosePort(port);
            showMenu = false;
            return;
        }
        int answer = STR_TO_INT(answerString);

        //Notice the 1, 2, 3, 4 which is the "data" to create a barcode for, before that you need to say how many chars there are.
        UCHAR printBarcode[16] = { 0x1b, (UCHAR)'b', (UCHAR)'4', 0x32, 0x31, 0x50, (UCHAR)'1',(UCHAR)'2',(UCHAR)'3',(UCHAR)'4', 0x1e};
        //Main switch for reading the input as a digit choice
        switch(answer)
        {
        case 1:
            CONSOLE_OUT(STR("Code 39:\n HEX: 1B 62 34 32 31 50 31 32 33 34 1E\n Note these are the bytes that were sent to produce the barcode\n"));
            barcodeSelected = true;
            printBarcode[2] = (UCHAR)'4';
            break;
        case 2:
            CONSOLE_OUT(STR("Interleaved 2 of 5:\n HEX: 1B 62 35 32 31 50 31 32 33 34 1E\n Note these are the bytes that were sent to produce the barcode\n"));
            barcodeSelected = true;
            printBarcode[2] = (UCHAR)'5';
            break;
        case 3:
            CONSOLE_OUT(STR("Code 93:\n HEX: 1B 62 37 32 31 50 31 32 33 34 1E\n Note these are the bytes that were sent to produce the barcode\n"));
            barcodeSelected = true;
            printBarcode[2] = (UCHAR)'7';
            break;
        case 4:
            CONSOLE_OUT(STR("Code 128:\n HEX: 1B 62 36 32 31 50 31 32 33 34 1E\n Note these are the bytes that were sent to produce the barcode\n"));
            barcodeSelected = true;
            printBarcode[2] = (UCHAR)'6';
            break;
        default:
            CONSOLE_OUT(STR("\nThat is not a valid choice! Try again...\n"));
            break;
        }
        if(barcodeSelected)
        {
        //Get total bytes successfully communicated to the printer
        INT64 totalSizeCommunicated = WritePortHelper(port, printBarcode, sizeof(printBarcode));
        if (totalSizeCommunicated == -1)
        {
            CONSOLE_OUT(STR("Failed on sending data.\n"));
            ClosePort(port);
            return;
        }
        else if (totalSizeCommunicated != (INT64)sizeof(printBarcode))
        {
            CONSOLE_OUT(STR("Failure to send the entire command.\n Did the printer go offline?\n"));
            ClosePort(port);
            return;
        }
        }
        ClosePort(port);
    }
}

//2D Barcodes and steps to print a perfect QR or PDF417 2D Barcode
void StarMicronics::twoDBarcodes()
{
    bool showMenu = true;
    //2D Barcode While Loop
    while(showMenu)
    {
        bool barcodeSelected = false;
        //Use our helper function to open the port to the printer
        void * port = openPortHelper();
        if(port == NULL) //If the port is NULL then the port was not successfully opened
            return;

        CONSOLE_OUT(STR("2D BARCODES MENU\n"));
        CONSOLE_OUT(STR("[1] QR Code ~ Step 1 ~ Set Model\n"));
        CONSOLE_OUT(STR("[2] QR Code ~ Step 2 ~ Set Correction Level\n"));
        CONSOLE_OUT(STR("[3] QR Code ~ Step 3 ~ Set Cell Size\n"));
        CONSOLE_OUT(STR("[4] QR Code ~ Step 4 ~ Set Barcode Data\n"));
        CONSOLE_OUT(STR("[5] QR Code ~ Step 5 ~ Print QR Code\n"));
        CONSOLE_OUT(STR("[6] QR Code ~ Do All Steps\n"));
        CONSOLE_OUT(STR("[7]  PDF 417 ~ Step 1 ~ Set Size\n"));
        CONSOLE_OUT(STR("[8]  PDF 417 ~ Step 2 ~ Set ECC (Security Level)\n"));
        CONSOLE_OUT(STR("[9]  PDF 417 ~ Step 3 ~ Set X-Dimensions\n"));
        CONSOLE_OUT(STR("[10] PDF 417 ~ Step 4 ~ Set Aspect Ratio\n"));
        CONSOLE_OUT(STR("[11] PDF 417 ~ Step 5 ~ Set Data\n"));
        CONSOLE_OUT(STR("[12] PDF 417 ~ Step 6 ~ Print PDF 417\n"));
        CONSOLE_OUT(STR("[13] PDF 417 ~ Do All Steps\n"));
        CONSOLE_OUT(STR("[m] Back to Main Menu\n"));

        STR_ELEM answerString[10];

        CONSOLE_OUT(STR("Please choose a 2D Barcode Command: "));
        CONSOLE_IN(STR("%s"), answerString); CONSOLE_IN_CHAR(); //read input

        if(STR_CMP(answerString, STR("m")) == 0)
        {
        ClosePort(port);
            showMenu = false;
            return;
        }
        int answer = STR_TO_INT(answerString);
        INT64 totalSizeCommunicated;
        INT64 commandSize;

        //Main switch for reading the input as a digit choice

        if(answer == 1)
        {	//QR - Set Model
                CONSOLE_OUT(STR("\n Set QR Code Model:\n ASCII: ESC GS y  S  0  n\n HEX:   1B  1D 79 53 30 n \n\n n = 1 or 2\n\n"));
                UCHAR command0[6] = {0x1B,0x1D,0x79,0x53,0x30,0x01};
                commandSize = (INT64)sizeof(command0);
                totalSizeCommunicated = WritePortHelper(port, command0, sizeof(command0));//Get total bytes successfully communicated to the printer
                barcodeSelected = true;
        }
        else if(answer == 2)
        {	//QR - Set Correction Level
                CONSOLE_OUT(STR("\n Set QR Code Correction Level:\n ASCII: ESC GS y  S  1  n\n HEX:   1B  1D 79 53 31 n \n\n n = 0,1,2 or 3 \n\n"));
                UCHAR command1[6] = {0x1B,0x1D, 0x79,0x53,0x31,0x02};
                commandSize = (INT64)sizeof(command1);
                totalSizeCommunicated = WritePortHelper(port, command1, sizeof(command1));//Get total bytes successfully communicated to the printer
                barcodeSelected = true;
        }
        else if(answer == 3)
        {	//QR - Set Cell Size
                CONSOLE_OUT(STR("\n Set QR Code Cell Size:\n ASCII: ESC GS y  S  2  n\n HEX:   1B  1D 79 53 32 n \n\n n = 1 through 8\n\n"));
                UCHAR command2[6] = {0x1B,0x1D,0x79,0x53,0x32,0x04};
                commandSize = (INT64)sizeof(command2);
                totalSizeCommunicated = WritePortHelper(port, command2, sizeof(command2));//Get total bytes successfully communicated to the printer
                barcodeSelected = true;
        }
        else if(answer == 4)
        {	//QR - Set Data
                CONSOLE_OUT(STR("\n Set QR Code Data:\n ASCII: ESC GS y  D  1  0  n  0 data LF\n HEX:   1B  1D 79 44 31 00 n 00 data 0A\n\n n = data length\n\n"));
                UCHAR command3[13] = {0x1B,0x1D,0x79,0x44,0x31,0x00,0x04,0x00,0x31,0x32,0x33,0x34,0x0A};
                commandSize = (INT64)sizeof(command3);
                totalSizeCommunicated = WritePortHelper(port, command3, sizeof(command3));//Get total bytes successfully communicated to the printer
                barcodeSelected = true;
        }
        else if(answer == 5)
        {	//QR - Print
            CONSOLE_OUT(STR("\n Print QR Code :\n ASCII: ESC GS y  P\n HEX:   1B  1D 79 50\n\n"));
                UCHAR command4[4] = {0x1B,0x1D,0x79,0x50};
                commandSize = (INT64)sizeof(command4);
                totalSizeCommunicated = WritePortHelper(port, command4, sizeof(command4));//Get total bytes successfully communicated to the printer
                barcodeSelected = true;
        }
        else	if(answer == 6)
        {	//QR - Do All
                CONSOLE_OUT(STR("\nQR Code ~ DO ALL\n\n"));
                UCHAR command5[35] = {0x1B,0x1D,0x79,0x53,0x30,0x01,0x1B,0x1D,0x79,0x53,0x31,0x02,0x1B,0x1D,0x79,0x53,0x32,0x04,0x1B,0x1D,0x79,0x44,0x31,0x00,0x04,0x00,0x31,0x32,0x33,0x34,0x0A,0x1B,0x1D,0x79,0x50};
                commandSize = (INT64)sizeof(command5);
                totalSizeCommunicated = WritePortHelper(port, command5, sizeof(command5));//Get total bytes successfully communicated to the printer
                barcodeSelected = true;
        }
        else	if(answer == 7)
        {	//PDF 417 - Size setting
                CONSOLE_OUT(STR("\n Set PDF417 Size:\n ASCII: ESC GS x  S  0  n1 n2\n HEX:   1B  1D 78 53 00 n1 n2 \n\n n1 = height     n2 = width\n\n"));
                UCHAR command6[8] = {0x1B,0x1D,0x78,0x53,0x30,0x00,0x02,0x03};
                commandSize = (INT64)sizeof(command6);
                totalSizeCommunicated = WritePortHelper(port, command6, sizeof(command6));//Get total bytes successfully communicated to the printer
                barcodeSelected = true;
        }
        else	if(answer == 8)
        {	//PDF 417 - ECC
                CONSOLE_OUT(STR("\n Set PDF417 ECC Security Level:\n ASCII: ESC GS x  S  1  n\n HEX:   1B  1D 78 53 01 n\n\n n = ECC level\n\n"));
                UCHAR command7[6] = {0x1B,0x1D,0x78,0x53,0x31,0x02};
                commandSize = (INT64)sizeof(command7);
                totalSizeCommunicated = WritePortHelper(port, command7, sizeof(command7));//Get total bytes successfully communicated to the printer
                barcodeSelected = true;
        }
        else	if(answer == 9)
        {	//PDF 417 - X-Dim
                CONSOLE_OUT(STR("\n Set PDF417 X-Dimensions Security Level:\n ASCII: ESC GS x  S  2  n\n HEX:   1B  1D 78 53 02 n\n\n n = Module X size\n\n"));
                UCHAR command8[6] = {0x1B,0x1D,0x78,0x53,0x32,0x03};
                commandSize = (INT64)sizeof(command8);
                totalSizeCommunicated = WritePortHelper(port, command8, sizeof(command8));//Get total bytes successfully communicated to the printer
                barcodeSelected = true;
        }
        else	if(answer == 10)
        {	//PDF 417 - Aspect Ratio
                CONSOLE_OUT(STR("\n Set PDF417 Module Aspect Ratio:\n ASCII: ESC GS x  S  3  n\n HEX:   1B  1D 78 53 03 n\n\n n = Module aspect ratio\n\n"));
                UCHAR command9[6] = {0x1B,0x1D,0x78,0x53,0x33,0x02};
                commandSize = (INT64)sizeof(command9);
                totalSizeCommunicated = WritePortHelper(port, command9, sizeof(command9));//Get total bytes successfully communicated to the printer
                barcodeSelected = true;
        }
        else	if(answer == 11)
        {	//PDF 417 - Set Data
                CONSOLE_OUT(STR("\n Set PDF417 Data Setting:\n ASCII: ESC GS x  D  nL nH d1 d2 ... dk\n HEX:   1B  1D 78 44 nL nH d1 d2 ... dk\n\n n = Module aspect ratio\n\n"));
                UCHAR command10[10] = {0x1B,0x1D,0x78,0x44,0x04,0x00,0x31,0x32,0x33,0x34};
                commandSize = (INT64)sizeof(command10);
                totalSizeCommunicated = WritePortHelper(port, command10, sizeof(command10));//Get total bytes successfully communicated to the printer
                barcodeSelected = true;
        }
        else	if(answer == 12)
        {	//PDF 417 - Print Barcode
                CONSOLE_OUT(STR("\n Print PDF417 Barcode:\n ASCII: ESC GS x  P\n HEX:   1B  1D 78 50\n\n"));
                UCHAR command11[4] = {0x1B,0x1D,0x78,0x50};
                commandSize = (INT64)sizeof(command11);
                totalSizeCommunicated = WritePortHelper(port, command11, sizeof(command11));//Get total bytes successfully communicated to the printer
                barcodeSelected = true;
        }
        else	if(answer == 13)
        {	//PDF 417 - Print Barcode
                CONSOLE_OUT(STR("\n PDF417 ~ DO ALL\n\n"));
                UCHAR command12[41] = {0x1B,0x1D,0x78,0x53,0x30,0x00,0x02,0x03,0x1B,0x1D,0x78,0x53,0x31,0x02,0x1B,0x1D,0x78,0x53,0x32,0x03,0x1B,0x1D,0x78,0x53,0x33,0x02,0x1B,0x1D,0x78,0x44,0x04,0x00,0x31,0x32,0x33,0x34,0x0A,0x1B,0x1D,0x78,0x50};
                commandSize = (INT64)sizeof(command12);
                totalSizeCommunicated = WritePortHelper(port, command12, sizeof(command12));//Get total bytes successfully communicated to the printer
                barcodeSelected = true;
        }
        else
        {
                CONSOLE_OUT(STR("\nThat is not a valid choice! Try again...\n"));
        }

        if(barcodeSelected)
        {
            if (totalSizeCommunicated == -1)
            {
                CONSOLE_OUT(STR("Failed on sending data.\n"));
                ClosePort(port);
                return;
            }
            else if (totalSizeCommunicated != commandSize)
            {
                CONSOLE_OUT(STR("Failure to send the entire command.\n Did the printer go offline?\n"));
                ClosePort(port);
                return;
            }
        }
        ClosePort(port);
    }

}

//Code Page Function, This function is large because there is a menu item for every code page.
void StarMicronics::codePages()
{
    bool showMenu = true;
    //Code Page Menu Loop
    while(showMenu)
    {
        STR_ELEM answerString[10];
        bool codePageSelected = false;
        //Use our helper function to open the port to the printer
        void * port = openPortHelper();
        if(port == NULL) //If the port is NULL then the port was not successfully opened
            return;

        CONSOLE_OUT(STR("CODE PAGES: \n"));
        CONSOLE_OUT(STR("[1]   Normal\n"));
        CONSOLE_OUT(STR("[2]   437 (USA, Std. Europe) \n"));
        CONSOLE_OUT(STR("[3]   Katakana \n"));
        CONSOLE_OUT(STR("[4]   437 (USA, Std. Europe) \n"));
        CONSOLE_OUT(STR("[5]   858 (Multilingual) \n"));
        CONSOLE_OUT(STR("[6]   852 (Latin-2) \n"));
        CONSOLE_OUT(STR("[7]   860 (Portuguese)\n"));
        CONSOLE_OUT(STR("[8]   861 (Icelandic) \n"));
        CONSOLE_OUT(STR("[9]   863 (Canadian French)\n"));
        CONSOLE_OUT(STR("[10]  865 (Nordic) \n"));
        CONSOLE_OUT(STR("[11]  866 (Cyrillic Russian)  \n"));
        CONSOLE_OUT(STR("[12]  855 (Cyrillic Bulgarian) \n"));
        CONSOLE_OUT(STR("[13]  857 (Turkey) \n"));
        CONSOLE_OUT(STR("[14]  862 (Israel (Hebrew))\n"));
        CONSOLE_OUT(STR("[15]  864 (Arabic)\n"));
        CONSOLE_OUT(STR("[16]  737 (Greek)\n"));
        CONSOLE_OUT(STR("[17]  851 (Greek)\n"));
        CONSOLE_OUT(STR("[18]  869 (Greek)\n"));
        CONSOLE_OUT(STR("[19]  928 (Greek)\n"));
        CONSOLE_OUT(STR("[20]  772 (Lithuanian)\n"));
        CONSOLE_OUT(STR("[21]  774 (Lithuanian)\n"));
        CONSOLE_OUT(STR("[22]  874 (Thai)\n"));
        CONSOLE_OUT(STR("[23]  1252 (Windows Latin-1)\n"));
        CONSOLE_OUT(STR("[24]  1250 (Windows Latin-2)\n"));
        CONSOLE_OUT(STR("[25]  1251 (Windows Cyrillic)\n"));
        CONSOLE_OUT(STR("[26]  3840 (IBM-Russian)\n"));
        CONSOLE_OUT(STR("[27]  3841 (Gost)\n"));
        CONSOLE_OUT(STR("[28]  3843 (Polish)\n"));
        CONSOLE_OUT(STR("[29]  3844 (CS2)\n"));
        CONSOLE_OUT(STR("[30]  3845 (Hungarian)\n"));
        CONSOLE_OUT(STR("[31]  3846 (Turkish)\n"));
        CONSOLE_OUT(STR("[32]  3847 (Brazil-ABNT)\n"));
        CONSOLE_OUT(STR("[33]  3848 (Brazil-ABICOMP)\n"));
        CONSOLE_OUT(STR("[34]  1001 (Arabic)\n"));
        CONSOLE_OUT(STR("[35]  2001 (Lithuanian-KBL)\n"));
        CONSOLE_OUT(STR("[36]  3001 (Estonian-1)\n"));
        CONSOLE_OUT(STR("[37]  3002 (Estonian-2)\n"));
        CONSOLE_OUT(STR("[38]  3011 (Latvian-1)\n"));
        CONSOLE_OUT(STR("[39]  3012 (Latvian-2)\n"));
        CONSOLE_OUT(STR("[40]  3021 (Bulgarian)\n"));
        CONSOLE_OUT(STR("[41]  3041 (Maltese)\n"));

        CONSOLE_OUT(STR("[m] Back to Main Menu\n"));


        CONSOLE_OUT(STR("Please choose a Code Page to Print Character Map: "));
        CONSOLE_IN(STR("%s"), answerString); CONSOLE_IN_CHAR(); //read input

        if(STR_CMP(answerString, STR("m")) == 0)
        {
        ClosePort(port);
            showMenu = false;
            return;
        }
        int answer = STR_TO_INT(answerString);
        UCHAR command[4] = {0x00,0x00,0x00,0x00};
        switch(answer){
            case 1:
                CONSOLE_OUT(STR("\n Set Normal Code Page\n 0x1B 0x1D 0x74 0x00 \n\n"));
                command[0] = 0x1B; command[1] = 0x1D; command[2] = 0x74; command[3] = 0x00;
                codePageSelected = true;
            break;
            case 2:
                CONSOLE_OUT(STR("\n Set 437 (USA, Std. Europe) Code Page\n 0x1B 0x1D 0x74 0x01 \n\n"));
                command[0] = 0x1B; command[1] = 0x1D; command[2] = 0x74; command[3] = 0x01;
                codePageSelected = true;
            break;
            case 3:
                CONSOLE_OUT(STR("\n Set Katakana Code Page\n 0x1B 0x1D 0x74 0x02 \n\n"));
                command[0] = 0x1B; command[1] = 0x1D; command[2] = 0x74; command[3] = 0x02;
                codePageSelected = true;
            break;
            case 4:
                CONSOLE_OUT(STR("\n Set 437 (USA, Std. Europe) Code Page\n 0x1B 0x1D 0x74 0x03 \n\n"));
                command[0] = 0x1B; command[1] = 0x1D; command[2] = 0x74; command[3] = 0x03;
                codePageSelected = true;
            break;
            case 5:
                CONSOLE_OUT(STR("\n Set 858 (Multilingual) Code Page\n 0x1B 0x1D 0x74 0x04 \n\n"));
                command[0] = 0x1B; command[1] = 0x1D; command[2] = 0x74; command[3] = 0x04;
                codePageSelected = true;
            break;
            case 6:
                CONSOLE_OUT(STR("\n Set 852 (Latin-2) Code Page\n 0x1B 0x1D 0x74 0x05 \n\n"));
                command[0] = 0x1B; command[1] = 0x1D; command[2] = 0x74; command[3] = 0x05;
                codePageSelected = true;
            break;
            case 7:
                CONSOLE_OUT(STR("\n Set 860 (Portuguese) Code Page\n 0x1B 0x1D 0x74 0x06 \n\n"));
                command[0] = 0x1B; command[1] = 0x1D; command[2] = 0x74; command[3] = 0x06;
                codePageSelected = true;
            break;
            case 8:
                CONSOLE_OUT(STR("\n Set 861 (Icelandic) Code Page\n 0x1B 0x1D 0x74 0x07 \n\n"));
                command[0] = 0x1B; command[1] = 0x1D; command[2] = 0x74; command[3] = 0x07;
                codePageSelected = true;
            break;
            case 9:
                CONSOLE_OUT(STR("\n Set 863 (Canadian French) Code Page\n 0x1B 0x1D 0x74 0x08 \n\n"));
                command[0] = 0x1B; command[1] = 0x1D; command[2] = 0x74; command[3] = 0x08;
                codePageSelected = true;
            break;
            case 10:
                CONSOLE_OUT(STR("\n Set 865 (Nordic) Code Page\n 0x1B 0x1D 0x74 0x09 \n\n"));
                command[0] = 0x1B; command[1] = 0x1D; command[2] = 0x74; command[3] = 0x09;
                codePageSelected = true;
            break;
            case 11:
                CONSOLE_OUT(STR("\n Set 866 (Cyrillic Russian) Code Page\n 0x1B 0x1D 0x74 0x0A \n\n"));
                command[0] = 0x1B; command[1] = 0x1D; command[2] = 0x74; command[3] = 0x0A;
                codePageSelected = true;
            break;
            case 12:
                CONSOLE_OUT(STR("\n Set 855 (Cyrillic Bulgarian) Code Page\n 0x1B 0x1D 0x74 0x0B \n\n"));
                command[0] = 0x1B; command[1] = 0x1D; command[2] = 0x74; command[3] = 0x0B;
                codePageSelected = true;
            break;
            case 13:
                CONSOLE_OUT(STR("\n Set 857 (Turkey) Code Page\n 0x1B 0x1D 0x74 0x0C \n\n"));
                command[0] = 0x1B; command[1] = 0x1D; command[2] = 0x74; command[3] = 0x0C;
                codePageSelected = true;
            break;
            case 14:
                CONSOLE_OUT(STR("\n Set 862 (Israel (Hebrew) Code Page\n 0x1B 0x1D 0x74 0x0D \n\n"));
                command[0] = 0x1B; command[1] = 0x1D; command[2] = 0x74; command[3] = 0x0D;
                codePageSelected = true;
            break;
            case 15:
                CONSOLE_OUT(STR("\n Set 864 (Arabic) Code Page\n 0x1B 0x1D 0x74 0x0E\n\n"));
                command[0] = 0x1B; command[1] = 0x1D; command[2] = 0x74; command[3] = 0x0E;
                codePageSelected = true;
            break;
            case 16:
                CONSOLE_OUT(STR("\n Set 737 (Greek) Code Page\n 0x1B 0x1D 0x74 0x0F \n\n"));
                command[0] = 0x1B; command[1] = 0x1D; command[2] = 0x74; command[3] = 0x0F;
                codePageSelected = true;
            break;
            case 17:
                CONSOLE_OUT(STR("\n Set 851 (Greek) Code Page\n 0x1B 0x1D 0x74 0x10 \n\n"));
                command[0] = 0x1B; command[1] = 0x1D; command[2] = 0x74; command[3] = 0x10;
                codePageSelected = true;
            break;
            case 18:
                CONSOLE_OUT(STR("\n Set 869 (Greek) Code Page\n 0x1B 0x1D 0x74 0x11 \n\n"));
                command[0] = 0x1B; command[1] = 0x1D; command[2] = 0x74; command[3] = 0x11;
                codePageSelected = true;
            break;
            case 19:
                CONSOLE_OUT(STR("\n Set 928 (Greek) Code Page\n 0x1B 0x1D 0x74 0x12 \n\n"));
                command[0] = 0x1B; command[1] = 0x1D; command[2] = 0x74; command[3] = 0x12;
                codePageSelected = true;
            break;
            case 20:
                CONSOLE_OUT(STR("\n Set 772 (Lithuanian) Code Page\n 0x1B 0x1D 0x74 0x13 \n\n"));
                command[0] = 0x1B; command[1] = 0x1D; command[2] = 0x74; command[3] = 0x13;
                codePageSelected = true;
            break;
            case 21:
                CONSOLE_OUT(STR("\n Set 774 (Lithuanian) Code Page\n 0x1B 0x1D 0x74 0x14 \n\n"));
                command[0] = 0x1B; command[1] = 0x1D; command[2] = 0x74; command[3] = 0x14;
                codePageSelected = true;
            break;
            case 22:
                CONSOLE_OUT(STR("\n Set 874 (Thai) Code Page\n 0x1B 0x1D 0x74 0x15 \n\n"));
                command[0] = 0x1B; command[1] = 0x1D; command[2] = 0x74; command[3] = 0x15;
                codePageSelected = true;
            break;
            case 23:
                CONSOLE_OUT(STR("\n Set 1252 (Windows Latin-1) Code Page\n 0x1B 0x1D 0x74 0x20 \n\n"));
                command[0] = 0x1B; command[1] = 0x1D; command[2] = 0x74; command[3] = 0x20;
                codePageSelected = true;
            break;
            case 24:
                CONSOLE_OUT(STR("\n Set 1250 (Windows Latin-2) Code Page\n 0x1B 0x1D 0x74 0x21 \n\n"));
                command[0] = 0x1B; command[1] = 0x1D; command[2] = 0x74; command[3] = 0x21;
                codePageSelected = true;
            break;
            case 25:
                CONSOLE_OUT(STR("\n Set 1251 (Windows Cyrillic) Code Page\n 0x1B 0x1D 0x74 0x22 \n\n"));
                command[0] = 0x1B; command[1] = 0x1D; command[2] = 0x74; command[3] = 0x22;
                codePageSelected = true;
            break;
            case 26:
                CONSOLE_OUT(STR("\n Set 3840 (IBM-Russian) Code Page\n 0x1B 0x1D 0x74 0x40 \n\n"));
                command[0] = 0x1B; command[1] = 0x1D; command[2] = 0x74; command[3] = 0x40;
                codePageSelected = true;
            break;
            case 27:
                CONSOLE_OUT(STR("\n Set 3841 (Gost) Code Page\n 0x1B 0x1D 0x74 0x41 \n\n"));
                command[0] = 0x1B; command[1] = 0x1D; command[2] = 0x74; command[3] = 0x41;
                codePageSelected = true;
            break;
            case 28:
                CONSOLE_OUT(STR("\n Set 3843 (Polish) Code Page\n 0x1B 0x1D 0x74 0x42 \n\n"));
                command[0] = 0x1B; command[1] = 0x1D; command[2] = 0x74; command[3] = 0x42;
                codePageSelected = true;
            break;
            case 29:
                CONSOLE_OUT(STR("\n Set 3844 (CS2) Code Page\n 0x1B 0x1D 0x74 0x43 \n\n"));
                command[0] = 0x1B; command[1] = 0x1D; command[2] = 0x74; command[3] = 0x43;
                codePageSelected = true;
            break;
            case 30:
                CONSOLE_OUT(STR("\n Set 3845 (Hungarian) Code Page\n 0x1B 0x1D 0x74 0x44 \n\n"));
                command[0] = 0x1B; command[1] = 0x1D; command[2] = 0x74; command[3] = 0x44;
                codePageSelected = true;
            break;
            case 31:
                CONSOLE_OUT(STR("\n Set 3846 (Turkish) Code Page\n 0x1B 0x1D 0x74 0x45 \n\n"));
                command[0] = 0x1B; command[1] = 0x1D; command[2] = 0x74; command[3] = 0x45;
                codePageSelected = true;
            break;
            case 32:
                CONSOLE_OUT(STR("\n Set 3847 (Brazil-ABNT) Code Page\n 0x1B 0x1D 0x74 0x46 \n\n"));
                command[0] = 0x1B; command[1] = 0x1D; command[2] = 0x74; command[3] = 0x46;
                codePageSelected = true;
            break;
            case 33:
                CONSOLE_OUT(STR("\n Set 3848 (Brazil-ABICOMP) Code Page\n 0x1B 0x1D 0x74 0x47 \n\n"));
                command[0] = 0x1B; command[1] = 0x1D; command[2] = 0x74; command[3] = 0x47;
                codePageSelected = true;
            break;
            case 34:
                CONSOLE_OUT(STR("\n Set 1001 (Arabic) Code Page\n 0x1B 0x1D 0x74 0x48 \n\n"));
                command[0] = 0x1B; command[1] = 0x1D; command[2] = 0x74; command[3] = 0x48;
                codePageSelected = true;
            break;
            case 35:
                CONSOLE_OUT(STR("\n Set 2001 (Lithuanian-KBL) Code Page\n 0x1B 0x1D 0x74 0x49 \n\n"));
                command[0] = 0x1B; command[1] = 0x1D; command[2] = 0x74; command[3] = 0x49;
                codePageSelected = true;
            break;
            case 36:
                CONSOLE_OUT(STR("\n Set 3001 (Estonian-1) Code Page\n 0x1B 0x1D 0x74 0x4A \n\n"));
                command[0] = 0x1B; command[1] = 0x1D; command[2] = 0x74; command[3] = 0x4A;
                codePageSelected = true;
            break;
            case 37:
                CONSOLE_OUT(STR("\n Set 3002 (Estonian-2) Code Page\n 0x1B 0x1D 0x74 0x4B \n\n"));
                command[0] = 0x1B; command[1] = 0x1D; command[2] = 0x74; command[3] = 0x4B;
                codePageSelected = true;
            break;
            case 38:
                CONSOLE_OUT(STR("\n Set 3011 (Latvian-1) Code Page\n 0x1B 0x1D 0x74 0x4C \n\n"));
                command[0] = 0x1B; command[1] = 0x1D; command[2] = 0x74; command[3] = 0x4C;
                codePageSelected = true;
            break;
            case 39:
                CONSOLE_OUT(STR("\n Set 3012 (Latvian-2) Code Page\n 0x1B 0x1D 0x74 0x4D \n\n"));
                command[0] = 0x1B; command[1] = 0x1D; command[2] = 0x74; command[3] = 0x4D;
                codePageSelected = true;
            break;
            case 40:
                CONSOLE_OUT(STR("\n Set 3021 (Bulgarian) Code Page\n 0x1B 0x1D 0x74 0x4E \n\n"));
                command[0] = 0x1B; command[1] = 0x1D; command[2] = 0x74; command[3] = 0x4E;
                codePageSelected = true;
            break;
            case 41:
                CONSOLE_OUT(STR("\n Set 3041 (Maltese) Code Page\n 0x1B 0x1D 0x74 0x4F \n\n"));
                command[0] = 0x1B; command[1] = 0x1D; command[2] = 0x74; command[3] = 0x4F;
                codePageSelected = true;
            break;
            default:
                CONSOLE_OUT(STR("\nThat is not a valid choice! Try again...\n"));
            break;
        }

        if(codePageSelected)
        {
            INT64 totalSizeCommunicated = WritePortHelper(port, command, sizeof(command));//Get total bytes successfully communicated to the printer
            if (totalSizeCommunicated == -1)
            {
                CONSOLE_OUT(STR("Failed on sending data.\n"));
                ClosePort(port);
                return;
            }
            else if (totalSizeCommunicated != (INT64)sizeof(command))
            {
                CONSOLE_OUT(STR("Failure to send the entire command.\n Did the printer go offline?\n"));
                ClosePort(port);
                return;
            }

            UCHAR printTheseBytes[256];
            for(INT64 i = 32; i < 255; i++){
            printTheseBytes[i] = (UCHAR)i;
            }

            totalSizeCommunicated = WritePortHelper(port, printTheseBytes, sizeof(printTheseBytes));//Get total bytes successfully communicated to the printer
            if (totalSizeCommunicated == -1)
            {
                CONSOLE_OUT(STR("Failed on sending data.\n"));
                ClosePort(port);
                return;
            }
            else if (totalSizeCommunicated != (INT64)sizeof(printTheseBytes))
            {
                CONSOLE_OUT(STR("Failure to send the entire command.\n Did the printer go offline?\n"));
                ClosePort(port);
                return;
            }
        }
        ClosePort(port);

        CONSOLE_OUT(STR("\n\nPlease press enter to return to Code Page Menu..."));
        CONSOLE_IN_CHAR(); //pause
    }
}

//Fonts (A, B and OCR-B)
void StarMicronics::fonts()
{
    bool showMenu = true;
    //Code Page Menu Loop
    while(showMenu)
    {
        STR_ELEM answerString[10];
        bool fontSelected = false;
        //Use our helper function to open the port to the printer
        void * port = openPortHelper();
        if(port == NULL) //If the port is NULL then the port was not successfully opened
            return;

        CONSOLE_OUT(STR("FONTS: \n"));
        CONSOLE_OUT(STR("[1]   Set Font A\n"));
        CONSOLE_OUT(STR("[2]   Set Font B\n"));
        CONSOLE_OUT(STR("[3]   Set Font OCR-B\n"));
        CONSOLE_OUT(STR("[m] Back to Main Menu\n"));
        CONSOLE_OUT(STR("Please choose a font to set the printer to: "));
        CONSOLE_IN(STR("%s"), answerString); CONSOLE_IN_CHAR(); //read input

        if(STR_CMP(answerString, STR("m")) == 0)
        {
        ClosePort(port);
            showMenu = false;
            return;
        }
                int answer = STR_TO_INT(answerString);
        UCHAR command[4] = {0x00,0x00,0x00,0x00};
        switch(answer){
            case 1:
                CONSOLE_OUT(STR("\n Set Font A\n 0x1B 0x1E 0x46 0x00 \n\n"));
                command[0] = 0x1B; command[1] = 0x1E; command[2] = 0x46; command[3] = 0x00;
                fontSelected  = true;
            break;
            case 2:
                CONSOLE_OUT(STR("\n Set Font B\n 0x1B 0x1E 0x46 0x01 \n\n"));
                command[0] = 0x1B; command[1] = 0x1E; command[2] = 0x46; command[3] = 0x01;
                fontSelected  = true;
            break;
            case 3:
                CONSOLE_OUT(STR("\n Set Font OCR-B\n 0x1B 0x1E 0x46 0x10 \n\n"));
                command[0] = 0x1B; command[1] = 0x1E; command[2] = 0x46; command[3] = 0x10;
                fontSelected  = true;
            break;
            default:
                CONSOLE_OUT(STR("\nThat is not a valid choice! Try again...\n"));
            break;
        }

        if(fontSelected )
        {
            INT64 totalSizeCommunicated = WritePortHelper(port, command, sizeof(command));//Get total bytes successfully communicated to the printer
            if (totalSizeCommunicated == -1)
            {
                CONSOLE_OUT(STR("Failed on sending data.\n"));
                ClosePort(port);
                return;
            }
            else if (totalSizeCommunicated != (INT64)sizeof(command))
            {
                CONSOLE_OUT(STR("Failure to send the entire command.\n Did the printer go offline?\n"));
                ClosePort(port);
                return;
            }

            UCHAR printTheseBytes[256];
            for(INT64 i = 32; i < 255; i++){
            printTheseBytes[i] = (UCHAR)i;
            }

            totalSizeCommunicated = WritePortHelper(port, printTheseBytes, sizeof(printTheseBytes));//Get total bytes successfully communicated to the printer
            if (totalSizeCommunicated == -1)
            {
                CONSOLE_OUT(STR("Failed on sending data.\n"));
                ClosePort(port);
                return;
            }
            else if (totalSizeCommunicated != (INT64)sizeof(printTheseBytes))
            {
                CONSOLE_OUT(STR("Failure to send the entire command.\n Did the printer go offline?\n"));
                ClosePort(port);
                return;
            }
        }
        ClosePort(port);
    }
}


//Feed Commands
void StarMicronics::feed()
{

    bool showMenu = true;
    //Code Page Menu Loop
    while(showMenu)
    {
        STR_ELEM answerString[10];
        bool feedSelected = false;
        //Use our helper function to open the port to the printer
        void * port = openPortHelper();
        if(port == NULL) //If the port is NULL then the port was not successfully opened
            return;

        CONSOLE_OUT(STR("\nFEED: \n"));
        CONSOLE_OUT(STR("[1] Line Feed A\n"));
        CONSOLE_OUT(STR("[2] Set Line Feed to 4mm\n"));
        CONSOLE_OUT(STR("[3] Set Line Feed to 3mm\n"));
        CONSOLE_OUT(STR("[4] Multi Line Feed\n"));
        CONSOLE_OUT(STR("[5] Set Line Spacing to 3mm\n"));
        CONSOLE_OUT(STR("[6] Feed 4mm Multi Lines\n"));
        CONSOLE_OUT(STR("[7] Feed 8mm Multi Lines\n"));
        CONSOLE_OUT(STR("[8] Form Feed\n"));
        CONSOLE_OUT(STR("[m] Back to Main Menu\n"));
        CONSOLE_OUT(STR("Please choose a feed command to use: "));
        CONSOLE_IN(STR("%s"), answerString); CONSOLE_IN_CHAR(); //read input

        if(STR_CMP(answerString, STR("m")) == 0)
        {
            ClosePort(port);
            showMenu = false;
            return;
        }

        int answer = STR_TO_INT(answerString);
        UCHAR command[4] = {0x00,0x00,0x00,0x00};
        switch(answer){
            case 1:
                CONSOLE_OUT(STR("\n Line Feed\n 0x0A \n\n"));
                command[0] = 0x0A; command[1] = 0x00; command[2] = 0x00; command[3] = 0x00;
                feedSelected  = true;
            break;
            case 2:
                CONSOLE_OUT(STR("\n Set Line Feed to 4mm\n 0x1B 0x7A 0x01 \n\n"));
                command[0] = 0x1B; command[1] = 0x7A; command[2] = 0x01; command[3] = 0x00;
                feedSelected  = true;
            break;
            case 3:
                CONSOLE_OUT(STR("\n Set Line Feed to 3mm\n 0x1B 0x7A 0x00 \n\n"));
                command[0] = 0x1B; command[1] = 0x7A; command[2] = 0x00; command[3] = 0x00;
                feedSelected  = true;
            break;
            case 4:
                CONSOLE_OUT(STR("\n Multi Line Feed\n 0x1B 0x61 n \n\n"));
                command[0] = 0x1B; command[1] = 0x61; command[2] = 0x04; command[3] = 0x00;
                feedSelected  = true;
            break;
            case 5:
                CONSOLE_OUT(STR("\n Set Line Spacing to 3mm\n 0x1B 0x30 \n\n"));
                command[0] = 0x1B; command[1] = 0x30; command[2] = 0x00; command[3] = 0x00;
                feedSelected  = true;
            break;
            case 6:
                CONSOLE_OUT(STR("\n Feed 4mm Multi Lines\n 0x1B 0x4A n \n\n"));
                command[0] = 0x1B; command[1] = 0x4A; command[2] = 0x04; command[3] = 0x00;
                feedSelected  = true;
            break;
            case 7:
                CONSOLE_OUT(STR("\n Feed 8mm Multi Lines\n 0x1B 0x49 n \n\n"));
                command[0] = 0x1B; command[1] = 0x49; command[2] = 0x04; command[3] = 0x00;
                feedSelected  = true;
            break;
            case 8:
                CONSOLE_OUT(STR("\n Form Feed\n 0x0C\n\n"));
                command[0] = 0x0C; command[1] = 0x00; command[2] = 0x00; command[3] = 0x00;
                feedSelected  = true;
            break;
            default:
                CONSOLE_OUT(STR("\nThat is not a valid choice! Try again...\n"));
            break;
        }

        if(feedSelected)
        {
            INT64 totalSizeCommunicated = WritePortHelper(port, command, sizeof(command));//Get total bytes successfully communicated to the printer
            if (totalSizeCommunicated == -1)
            {
                CONSOLE_OUT(STR("Failed on sending data.\n"));
                ClosePort(port);
                return;
            }
            else if (totalSizeCommunicated != (INT64)sizeof(command))
            {
                CONSOLE_OUT(STR("Failure to send the entire command.\n Did the printer go offline?\n"));
                ClosePort(port);
                return;
            }
        }
        ClosePort(port);
    }
}

//Auto-Cutter Commands
void StarMicronics::autoCutter()
{
    bool showMenu = true;
    //Auto-cutter Menu Loop
    while(showMenu)
    {
        STR_ELEM answerString[10];
        bool cutSelected = false;
        //Use our helper function to open the port to the printer
        void * port = openPortHelper();
        if(port == NULL) //If the port is NULL then the port was not successfully opened
            return;

        CONSOLE_OUT(STR("\nAUTOCUTTER: \n"));
        CONSOLE_OUT(STR("[1] Full Cut\n"));
        CONSOLE_OUT(STR("[2] Partial Cut\n"));
        CONSOLE_OUT(STR("[3] Feed and Full Cut\n"));
        CONSOLE_OUT(STR("[4] Feed and Partial Cut\n"));
        CONSOLE_OUT(STR("[m] Back to Main Menu\n"));
        CONSOLE_OUT(STR("Please choose a cut command to execute: "));
        CONSOLE_IN(STR("%s"), answerString); CONSOLE_IN_CHAR(); //read input

        if(STR_CMP(answerString, STR("m")) == 0)
        {
        ClosePort(port);
            showMenu = false;
            return;
        }
                int answer = STR_TO_INT(answerString);

                //
                UCHAR command[3] = {0x00,0x00,0x00};
        switch(answer){
            case 1:
                CONSOLE_OUT(STR("\n  Full Cut\n  0x1B 0x64 0x00 \n\n"));
                command[0] = 0x1B; command[1] = 0x64; command[2] = 0x00;
                cutSelected  = true;
            break;
            case 2:
                CONSOLE_OUT(STR("\n  Partial Cut\n  0x1B 0x64 0x01 \n\n"));
                command[0] = 0x1B; command[1] = 0x64; command[2] = 0x01;
                cutSelected  = true;
            break;
            case 3:
                CONSOLE_OUT(STR("\n  Feed and Full Cut\n  0x1B 0x64 0x02 \n\n"));
                command[0] = 0x1B; command[1] = 0x64; command[2] = 0x02;
                cutSelected  = true;
            break;
            case 4:
                CONSOLE_OUT(STR("\n  Feed and Partial Cut\n  0x1B 0x64 0x03 \n\n"));
                command[0] = 0x1B; command[1] = 0x64; command[2] = 0x03;
                cutSelected  = true;
            break;
            default:
                CONSOLE_OUT(STR("\nThat is not a valid choice! Try again...\n"));
            break;
        }

        if(cutSelected)
        {
            INT64 totalSizeCommunicated = WritePortHelper(port, command, sizeof(command));//Get total bytes successfully communicated to the printer
            if (totalSizeCommunicated == -1)
            {
                CONSOLE_OUT(STR("Failed on sending data.\n"));
                ClosePort(port);
                return;
            }
            else if (totalSizeCommunicated != (INT64)sizeof(command))
            {
                CONSOLE_OUT(STR("Failure to send the entire command.\n Did the printer go offline?\n"));
                ClosePort(port);
                return;
            }
        }
        ClosePort(port);
    }
}

//Kick the cash drawer
void StarMicronics::kickCashDrawer()
{
    //Display what command is being sent.
    CONSOLE_OUT(STR("\nKICK CASH DRAWER\nASCII: BEL\nHEX:    0x07\n"));

    //Use our helper function to open the port to the printer
    void * port = openPortHelper();
    if(port == NULL) //If the port is NULL then the port was not successfully opened
        return;

    //Kick Cash Drawer Command
    UCHAR const * command = (UCHAR const *)"\x07";
    UINT32 const commandSize = sizeof("\x07") - 1;

    //Get total bytes successfully communicated to the printer
    INT64 totalSizeCommunicated = WritePortHelper(port, command, commandSize);
    if (totalSizeCommunicated == -1)
    {
        CONSOLE_OUT(STR("Failed on sending data.\n"));
        ClosePort(port);
        return;
    }
    else if (totalSizeCommunicated != (INT64)commandSize)
    {
        CONSOLE_OUT(STR("Failure to send the entire command.\n Did the printer go offline?\n"));
        ClosePort(port);
        return;
    }

    ClosePort(port);

    CONSOLE_OUT(STR("Success\n"));
}

//Resets the printers hardware
void StarMicronics::resetPrinter()
{
    void * port = OpenPort(g_portName, g_portSettings, g_portTimeout);
    if(port == NULL)
    {
        CONSOLE_OUT(STR("Error cannot open port\n"));
        return;
    }

    if(ResetDevice(port) == 0)
    {
        CONSOLE_OUT(STR("Error cannot reset device\n"));
    }

    ClosePort(port);
        CONSOLE_OUT(STR("Successfully Sent Printer Reset\n"));
}
