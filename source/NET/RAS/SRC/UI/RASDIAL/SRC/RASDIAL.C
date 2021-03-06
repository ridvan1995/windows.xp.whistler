/*****************************************************************************/
/**                         Microsoft LAN Manager                           **/
/**                   Copyright (C) 1993 Microsoft Corp.                    **/
/*****************************************************************************/

//***
//    File Name:
//       RASDIAL.C
//
//    Function:
//        Command line interface for making Remote Access connections,
//        as well as disconnecting from and enumerating these connections.
//
//    History:
//        03/18/93 - Michael Salamone (MikeSa) - Original Version 1.0
//***


#ifdef UNICODE
#error This program is built ANSI-only so it will run, as is, on Chicago.
#undef UNICODE
#endif

#include <windows.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <io.h>

#include <lmcons.h>
#include <lmerr.h>

#include <ras.h>
#include <raserror.h>

#include "rasdial.h"
#include "rasdial.rch"


char g_progname[MAX_PATH + 1];
char g_username[UNLEN + 1];
char g_password[PWLEN + 1];
char g_domain[DNLEN + 1];
char g_entryname[RAS_MaxEntryName + 1];
char g_phone_num[RAS_MaxPhoneNumber + 1];
char g_callback_num[RAS_MaxCallbackNumber + 1];
char g_phone_book[MAX_PATH];

HANDLE g_hEvent;
DWORD g_exitcode;
BOOL g_fHangupCalled = FALSE;
PBYTE g_Args[9];


HRASCONN g_hRasConn = NULL;
DWORD g_dbg = 0;

void _cdecl main(int argc, char *argv[])
{
    WORD len;
    UCHAR term;
    DWORD Action;
    BYTE ErrorMsg[1024];

    g_exitcode = 0L;

    Action = ParseCmdLine(argc, argv);

    switch (Action)
    {
        case HELP:
            Usage();
            break;


        case DIAL:
            //
            // Was username specified on command line?  If not, prompt for it.
            //
            if (!strcmp(g_username, "*"))
            {
                PrintMessage(DIAL_USERNAME_PROMPT, NULL);
                GetString(g_username, UNLEN + 1, &len, &term);
            }

            //
            // Was password specified on command line?  If not, prompt for it.
            //
            if (!strcmp(g_password, "*"))
            {
                PrintMessage(DIAL_PASSWORD_PROMPT, NULL);
                GetPasswdStr(g_password, PWLEN + 1, &len);
            }

            Dial();
            break;


        case DISCONNECT:
            Disconnect();
            break;


        case ENUMERATE_CONNECTIONS:
            EnumerateConnections();
            break;
    }


    if (g_exitcode)
    {
        if ((g_exitcode >= RASBASE) && (g_exitcode <= RASBASEEND))
        {
            BYTE str[10];

            g_Args[0] = itoa(g_exitcode, str, 10);
            g_Args[1] = NULL;
            PrintMessage(DIAL_ERROR_PREFIX, g_Args);

            RasGetErrorStringA(g_exitcode, ErrorMsg, 1024L);

            CharToOemA(ErrorMsg, ErrorMsg);
            fprintf(stdout, ErrorMsg);

            PrintMessage(DIAL_MORE_HELP, g_Args);
        }
        else
        {
            FormatMessageA(FORMAT_MESSAGE_IGNORE_INSERTS |
                    FORMAT_MESSAGE_MAX_WIDTH_MASK | FORMAT_MESSAGE_FROM_SYSTEM,
                    GetModuleHandle(NULL), g_exitcode, 0, ErrorMsg, 1024, NULL);

            CharToOemA(ErrorMsg, ErrorMsg);
            fprintf(stdout, ErrorMsg);
        }
    }
    else
    {
        PrintMessage(DIAL_COMMAND_SUCCESS, NULL);
    }

    exit(g_exitcode);
}


VOID Dial(VOID)
{
    DWORD rc;
    LPSTR pPhoneFile = NULL;
    RASDIALPARAMSA DialParms;
    DWORD NumEntries;
    RASCONNA *RasConn = NULL;
    RASCONNA *SaveRasConn = NULL;


    //
    // This just gets us an array of RASCONN structs
    //
    if (Enumerate(&RasConn, &NumEntries))
    {
        return;
    }

    SaveRasConn = RasConn;


    while (NumEntries--)
    {
        if (!strcmp(g_entryname, RasConn->szEntryName))
        {
            g_Args[0] = RasConn->szEntryName;
            g_Args[1] = NULL;
            PrintMessage(DIAL_ALREADY_CONNECTED, g_Args);

            GlobalFree(SaveRasConn);
            return;
        }

        RasConn++;
    }

    GlobalFree(SaveRasConn);


    //
    // This is the structure we pass to RasDial
    //
    DialParms.dwSize = sizeof(RASDIALPARAMSA);

    strcpy(DialParms.szUserName, g_username);
    strcpy(DialParms.szPassword, g_password);

    strcpy(DialParms.szEntryName, g_entryname);
    strcpy(DialParms.szDomain, g_domain);
    strcpy(DialParms.szPhoneNumber, g_phone_num);
    strcpy(DialParms.szCallbackNumber, g_callback_num);


    //
    // This event will get signaled in the RasDialCallback routine
    // once dial has completed (either successfully or because of error.
    //
    g_hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (!g_hEvent)
    {
        g_exitcode = GetLastError();

#if DBG
        if (g_dbg)
            printf("Error creating event - rc=%li\n", g_exitcode);
#endif

        return;
    }


    //
    // We need a routine to handle CTRL-C, CTRL-BREAK, etc.
    //
    if (!SetConsoleCtrlHandler(DialControlSignalHandler, TRUE))
    {
#if DBG
        printf("SetConsoleCtrlHandler returned %li\n");
#endif
    }


    if (g_phone_book[0])
    {
        pPhoneFile = g_phone_book;
    }

    //
    // Now dial
    //
    if (rc = RasDialA(NULL, pPhoneFile, &DialParms, 0, RasDialCallback,
            &g_hRasConn))
    {
        g_exitcode = rc;

#if DBG
        if (g_dbg)
            printf("Error from RasDial = %li\n", rc);
#endif

        return;
    }


    //
    // Now we just print "." every second until dial has completed.
    //
    while (1)
    {
        rc = WaitForSingleObject(g_hEvent, 1000L);
        if (rc == WAIT_TIMEOUT)
        {
            PrintMessage(DIAL_DOT, NULL);
        }
        else
        {
            break;
        }
    }

    if (g_fHangupCalled)
    {
        WaitForRasCompletion();
    }


    return;
}


VOID EnumerateConnections(VOID)
{
    DWORD NumEntries;
    RASCONNA *RasConn, *SaveRasConn;


    //
    // This just gets us an array of RASCONN structs
    //
    if (Enumerate(&RasConn, &NumEntries))
    {
        return;
    }

    SaveRasConn = RasConn;


    //
    // Now, go thru array of RASCONN structs and print out each connection.
    //
    if (!NumEntries)
    {
        PrintMessage(DIAL_NO_CONNECTIONS, NULL);
    }
    else
    {
        PrintMessage(DIAL_ENUM_HEADER, NULL);

        while (NumEntries--)
        {
            g_Args[0] = RasConn->szEntryName;
            g_Args[1] = NULL;
            PrintMessage(DIAL_ENUM_ENTRY, g_Args);

            RasConn++;
        }
    }


    //
    // This was allocated for us by the Enumerate call above
    //
    GlobalFree(SaveRasConn);

    return;
}


VOID Disconnect(VOID)
{
    DWORD rc;
    DWORD NumEntries;
    RASCONNA *RasConn, *SaveRasConn;
    BOOL fFoundEntry = FALSE;


    //
    // This just gets us an array of RASCONN structs
    //
    if (Enumerate(&RasConn, &NumEntries))
    {
        return;
    }

    SaveRasConn = RasConn;


    //
    // Now, go thru array of RASCONN structs searching for the
    // right entry to disconnect.
    //
    // Also, If no entryname specified on cmd line AND there
    // is only one connection, we'll set the entryname to that
    // one (thus having the effect of disconnecting that one).
    // If no entryname given AND more than one connection, we
    // won't disconnect anything - we'll enumerate the connections
    // and give the user an error message.
    //
    if (!g_entryname[0] & (NumEntries > 1))
    {
        PrintMessage(DIAL_DISCONNECT_ERROR, NULL);
        EnumerateConnections();

        goto Done;
    }


    if (!NumEntries)
    {
        PrintMessage(DIAL_NO_CONNECTIONS, NULL);

        goto Done;
    }


    if (!g_entryname[0] & (NumEntries == 1))
    {
        strcpy(g_entryname, RasConn->szEntryName);
    }


    while (NumEntries-- && !fFoundEntry)
    {
        if (!strcmpi(g_entryname, RasConn->szEntryName))
        {
            fFoundEntry = TRUE;

            if (!SetConsoleCtrlHandler(DisconnectControlSignalHandler, TRUE))
            {
#if DBG
                printf("SetConsoleCtrlHandler returned %li\n");
#endif
            }

            if (rc = RasHangUpA(RasConn->hrasconn))
            {
                g_exitcode = rc;

#if DBG
                if (g_dbg)
                    printf("Error from RasHangUp = %li\n", rc);
#endif

            }

            WaitForRasCompletion();

            break;
        }

        RasConn++;
    }


    if (!fFoundEntry)
    {
        g_Args[0] = g_entryname;
        g_Args[1] = NULL;
        PrintMessage(DIAL_NOT_CONNECTED, g_Args);
    }


Done:

    //
    // This was allocated for us by the Enumerate call above
    //
    GlobalFree(SaveRasConn);


    return;
}


//
// To get array of RASCONN structures
//
DWORD Enumerate(RASCONNA **RasConn, PDWORD NumEntries)
{
    DWORD rc;
    DWORD EnumSize = 0L;

    *NumEntries = 0;

    *RasConn = (RASCONNA *) GlobalAlloc(GMEM_FIXED, sizeof(RASCONNA));
    if (!*RasConn)
    {
        g_exitcode = GetLastError();

#if DBG
        if (g_dbg)
            printf("No memory for enumerating connections!\n");
#endif

        *NumEntries = 0;
        return (1L);
    }

    (*RasConn)->dwSize = sizeof(RASCONNA);


    //
    // This first call will tell us how much space we need to
    // fit in all the structures.
    //
    rc = RasEnumConnectionsA(*RasConn, &EnumSize, NumEntries);
    if (!rc && !*NumEntries)
    {
        return (0L);
    }


    if (NumEntries && (rc != ERROR_BUFFER_TOO_SMALL))
    {
        g_exitcode = rc;

#if DBG
        if (g_dbg)
            printf("Error from RasEnumConnectionsA = %li!\n", rc);
#endif

        *NumEntries = 0;
        return (1L);
    }

    //
    // Now we get memory for the structures.
    //
    GlobalFree(*RasConn);
    *RasConn = (RASCONNA *) GlobalAlloc(GMEM_FIXED, EnumSize);

    if (!*RasConn)
    {
        g_exitcode = GetLastError();

#if DBG
        if (g_dbg)
            printf("No memory for enumerating connections!\n");
#endif

        *NumEntries = 0;
        return (1L);
    }


    (*RasConn)->dwSize = sizeof(RASCONNA);

    //
    // This second call will now fill up our buffer with the
    // RASCONN structures.
    //
    if (rc = RasEnumConnectionsA(*RasConn, &EnumSize, NumEntries))
    {
        g_exitcode = rc;

#if DBG
        if (g_dbg)
            printf("Error from RasEnumConnectionsA = %li!\n", rc);
#endif

        *NumEntries = 0;
        GlobalFree(*RasConn);

        return (1L);
    }

    return (0L);
}


VOID Usage(VOID)
{
    g_Args[0] = g_progname;
    g_Args[1] = NULL;
    PrintMessage(DIAL_USAGE, g_Args);

    return;
}


DWORD ParseCmdLine(int argc, char *argv[])
{
    int i;
    BYTE CmdLineSwitch[80];
    PCHAR pColon;


    strcpy(g_progname, argv[0]);

    //
    // Set up defaults for these, in case switch isn't given on the
    // command line for them.
    //
    g_username[0] = '\0';       // means use name user is logged on with
    g_password[0] = '\0';       // means use password user is logged on with
    strcpy(g_domain, "*");      // means use domain stored in phonebook
    g_phone_num[0] = '\0';      // means use phone number stored in phonebook
    g_phone_book[0] = '\0';     // means use default phone book file
    g_callback_num[0] = '\0';   // means don't callback if user-specified


    if (argc == 1)
    {
        //
        // In this case, only the name of the program was specified,
        // which means all we have to do is enumerate connections.
        //
        return (ENUMERATE_CONNECTIONS);
    }


    //
    // see if an entryname is present (must be 1st argument if it is)
    //
    if (is_valid_entryname(argv[1]))
    {
        //
        // We have a valid entryname - user either wants to dial to
        // it or disconnect from it.
        //

        strcpy(g_entryname, argv[1]);
        strupr(g_entryname);

        LoadStringA(GetModuleHandle(NULL), DIAL_DISCONNECT_SWITCH,
                CmdLineSwitch, 80);
        if ((argc == 3) && (argv[2][0] == '/') && (strlen(&argv[2][1])) &&
                match(&argv[2][1], CmdLineSwitch))
        {
            return (DISCONNECT);
        }
        else
        {
            if ((argc > 3) && (argv[2][0] == '/') && (strlen(&argv[2][1])) &&
                    match(&argv[2][1], CmdLineSwitch))
            {
                return (HELP);
            }
        }


        //
        // User wants to connect - get username, password, and options
        //

        //
        // Username specified?  If next arg doesn't start with "/", then
        // YEA!.  If it does, then neither username or password are
        // specified.
        //
        if ((argc > 2) && (argv[2][0] != '/'))
        {
            if (strlen(argv[2]) > UNLEN)
            {
                return (HELP);
            }

            strcpy(g_username, argv[2]);


            //
            // Password specified?  If next arg doesn't start with "/", then
            // YEA!.
            //
            if ((argc > 3) && (argv[3][0] != '/'))
            {
                if (strlen(argv[3]) > PWLEN)
                {
                    return (HELP);
                }

                strcpy(g_password, argv[3]);
                i = 4;
            }
            else
            {
                i = 3;
            }
        }
        else
        {
            //
            // No username or password specified
            //
            i = 2;
        }


        //
        // Now get any other options.  If any cmd line switch is
        // invalid, or is specified more than once, we'll bail
        // out.
        //
        for (; i<argc; i++)
        {
            BOOL fDomainSpecified = FALSE;
            BOOL fCallbackSpecified = FALSE;


            //
            // Command line switched must be designated by '/'!
            if (argv[i][0] != '/')
            {
                return (HELP);
            }


            LoadStringA(GetModuleHandle(NULL), DIAL_DOMAIN_SWITCH,
                    CmdLineSwitch, 80);
            if (match(&argv[i][1], CmdLineSwitch))
            {
                //
                // Switch previously specified?
                //
                if (fDomainSpecified)
                {
                    return (HELP);
                }

                fDomainSpecified = TRUE;

                pColon = strchr(argv[i], ':');
                if (pColon)
                {
                    strncpy(g_domain, pColon+1, DNLEN);
                    g_domain[DNLEN] = '\0';
                    strupr(g_domain);
                }
                else
                {
                    return (HELP);
                }

                continue;
            }


            LoadStringA(GetModuleHandle(NULL), DIAL_PHONE_NO_SWITCH,
                    CmdLineSwitch, 80);
            if (match(&argv[i][1], CmdLineSwitch))
            {
                //
                // Switch previously specified?
                //
                if (g_phone_num[0])
                {
                    return (HELP);
                }

                pColon = strchr(argv[i], ':');
                if (pColon && strlen(pColon+1))
                {
                    strncpy(g_phone_num, pColon+1, RAS_MaxPhoneNumber);
                    g_phone_num[RAS_MaxPhoneNumber] = '\0';
                }
                else
                {
                    return (HELP);
                }

                continue;
            }


            LoadStringA(GetModuleHandle(NULL), DIAL_PHONE_BOOK_SWITCH,
                    CmdLineSwitch, 80);
            if (match(&argv[i][1], CmdLineSwitch))
            {
                OFSTRUCT of_struct;

                //
                // Switch previously specified?
                //
                if (g_phone_book[0])
                {
                    return (HELP);
                }


                //
                // This is the default path for the phone book file.
                // Our method is, if the phone book switch is supplied,
                // we will append it to this string and check for file
                // existence.  If it does not exist, we will test for
                // existence of the literal value supplied.  If that
                // still does not exist, we give a help message and exit.
                //
                ExpandEnvironmentStringsA("%windir%\\system32\\ras\\",
                        g_phone_book, MAX_PATH);

                pColon = strchr(argv[i], ':');
                if (pColon && strlen(pColon+1))
                {
                    if ((strlen(pColon+1) + strlen(g_phone_book)) > MAX_PATH-1)
                    {
                        //
                        // The catenated string would exceed MAX_PATH, so
                        // forget it - just use the string supplied.
                        //
                        strncpy(g_phone_book, pColon+1, MAX_PATH);
                        g_phone_book[MAX_PATH] = '\0';
                    }
                    else
                    {
                        strcat(g_phone_book, pColon+1);
                        if (OpenFile(g_phone_book, &of_struct, OF_EXIST) ==
                                HFILE_ERROR)
                        {
                            //
                            // The file doesn't exist in the default directory,
                            // so we'll use the value supplied straight away.
                            //
                            strncpy(g_phone_book, pColon+1, MAX_PATH);
                            g_phone_book[MAX_PATH] = '\0';
                        }
                    }

                    if (OpenFile(g_phone_book, &of_struct, OF_EXIST) ==
                            HFILE_ERROR)
                    {
                        PrintMessage(DIAL_FILE_DOESNT_EXIST, NULL);
                        return (HELP);
                    }
                }
                else
                {
                    return (HELP);
                }

                continue;
            }


            LoadStringA(GetModuleHandle(NULL), DIAL_CALLBACK_NO_SWITCH,
                    CmdLineSwitch, 80);
            if (match(&argv[i][1], CmdLineSwitch))
            {
                //
                // Switch previously specified?
                //
                if (fCallbackSpecified)
                {
                    return (HELP);
                }

                fCallbackSpecified = TRUE;

                pColon = strchr(argv[i], ':');
                if (pColon && strlen(pColon+1))
                {
                    strncpy(g_callback_num, pColon+1, RAS_MaxPhoneNumber);
                    g_callback_num[RAS_MaxPhoneNumber] = '\0';
                }
                else
                {
                    return (HELP);
                }

                continue;
            }


            //
            // Invalid switch, so we're out of here
            //
            return (HELP);
        }

        return (DIAL);
    }
    else
    {
        //
        // since no entryname was specified, there are 2 possibilities:
        //    1. user wants help
        //    2. user wants to disconnect
        //
        LoadStringA(GetModuleHandle(NULL), DIAL_HELP_SWITCH,
                CmdLineSwitch, 80);
        if (match(&argv[1][1], CmdLineSwitch))
        {
            return (HELP);
        }

        LoadStringA(GetModuleHandle(NULL), DIAL_DISCONNECT_SWITCH,
                CmdLineSwitch, 80);
        if (match(&argv[1][1], CmdLineSwitch))
        {
            //
            // Ok, user wants to disconnect, but we don't know what the
            // entryname is.  We'll just put in blank for now.
            //
            g_entryname[0] = '\0';

            if (argc == 2)
            {
                return (DISCONNECT);
            }
            else
            {
                return (HELP);
            }
        }


        //
        // Invalid command line if we get here
        //
        return (HELP);
    }
}


VOID WINAPI RasDialCallback(
    UINT MsgId,
    RASCONNSTATE conn_state,
    DWORD error
    )
{
#if DBG
    if (g_dbg)
        printf("RasDial conn state is %li\n", conn_state);
#endif

    if (error)
    {
#if DBG
        if (g_dbg)
            printf("Error from RasDial = %li\n", error);
#endif

        g_exitcode = error;

        RasHangUpA(g_hRasConn);
        g_fHangupCalled = TRUE;

        SetEvent(g_hEvent);

        return;
    }


    switch (conn_state)
    {
        case RASCS_OpenPort:
            g_Args[0] = g_entryname;
            g_Args[1] = NULL;
            PrintMessage(DIAL_CONNECTING, g_Args);
            break;

        case RASCS_PortOpened:
        case RASCS_ConnectDevice:
        case RASCS_DeviceConnected:
        case RASCS_AllDevicesConnected:
            break;

        case RASCS_Authenticate:
            PrintMessage(DIAL_AUTHENTICATING, NULL);
            break;

        case RASCS_ReAuthenticate:
            PrintMessage(DIAL_REAUTHENTICATING, NULL);
            break;

        case RASCS_AuthNotify:
        case RASCS_AuthCallback:
        case RASCS_AuthAck:
        case RASCS_AuthChangePassword:
        case RASCS_AuthRetry:
            break;

        case RASCS_AuthProject:
            PrintMessage(DIAL_PROJECTING, NULL);
            break;

        case RASCS_AuthLinkSpeed:
            PrintMessage(DIAL_LINK_SPEED, NULL);
            break;

        case RASCS_Authenticated:
            PrintMessage(DIAL_NEWLINE, NULL);
            break;

        case RASCS_PrepareForCallback:
            PrintMessage(DIAL_CALLBACK, NULL);
            break;

        case RASCS_WaitForModemReset:
        case RASCS_WaitForCallback:
            break;

        case RASCS_Interactive:
        case RASCS_RetryAuthentication:
        case RASCS_CallbackSetByCaller:
        case RASCS_PasswordExpired:
        {
            BYTE str[8];

            g_Args[0] = itoa(conn_state, str, 10);
            g_Args[1] = NULL;
            PrintMessage(DIAL_AUTH_ERROR, g_Args);

            RasHangUpA(g_hRasConn);
            g_fHangupCalled = TRUE;

            SetEvent(g_hEvent);
            break;
        }

        case RASCS_Connected:
            g_Args[0] = g_entryname;
            g_Args[1] = NULL;
            PrintMessage(DIAL_CONNECT_SUCCESS, g_Args);
            SetEvent(g_hEvent);

            break;

        case RASCS_Disconnected:
            PrintMessage(DIAL_DISCONNECTED, NULL);
            SetEvent(g_hEvent);
            break;
    }


    return;
}


BOOL DialControlSignalHandler(DWORD ControlType)
{
    //
    // Do we have a handle back from Rasdial call?
    //
    if (g_hRasConn)
    {
        RasHangUpA(g_hRasConn);
    }

    WaitForRasCompletion();

    PrintMessage(DIAL_CONTROL_C, NULL);

    exit(1L);

    return (TRUE);    // have to satisfy the compiler, you know.
}


BOOL DisconnectControlSignalHandler(DWORD ControlType)
{
    return (TRUE);
}


VOID WaitForRasCompletion(VOID)
{
    RASCONNSTATUSA Status;

    Status.dwSize = sizeof(RASCONNSTATUSA);

    while (RasGetConnectStatusA(g_hRasConn, &Status) != ERROR_INVALID_HANDLE)
    {
        Sleep(125L);
    }
}


BOOL is_valid_entryname(char *candidate)
{
    if (strlen(candidate) > RAS_MaxEntryName)
    {
        return (FALSE);
    }

    if (candidate[0] == '/')
    {
        return (FALSE);
    }

    return (TRUE);
}


//
// Returns TRUE if str1 is a substr of str2, starting at the beginning
// of str2 and ignoring case.  I.e. "Mike" will match "MIKESA".  "MIKESA"
// will not match "Mike"
//
BOOL match(
    char *str1,
    char *str2
    )
{
    BOOL retval;
    char *tstr1;
    char *tstr2;
    char *pcolon;

    tstr1 = (char *) GlobalAlloc(GMEM_FIXED, strlen(str1) + 1);
    if (!tstr1)
    {
        return (FALSE);
    }

    tstr2 = (char *) GlobalAlloc(GMEM_FIXED, strlen(str2) + 1);
    if (!tstr2)
    {
        GlobalFree(tstr1);
        return (FALSE);
    }


    strcpy(tstr1, str1);
    strcpy(tstr2, str2);

    strupr(tstr1);
    strupr(tstr2);

    pcolon = strchr(tstr1, ':');
    if (pcolon)
    {
        *pcolon = '\0';
    }

    if (strstr(tstr2, tstr1) == tstr2)
    {
        retval = TRUE;
    }
    else
    {
        retval = FALSE;
    }

    GlobalFree(tstr1);
    GlobalFree(tstr2);

    return (retval);
}


/***    GetPasswdStr -- read in password string
 *
 *      USHORT LUI_GetPasswdStr(char far *, USHORT);
 *
 *      ENTRY:  buf             buffer to put string in
 *              buflen          size of buffer
 *              &len            address of USHORT to place length in
 *
 *      RETURNS:
 *              0 or NERR_BufTooSmall if user typed too much.  Buffer
 *              contents are only valid on 0 return.
 *
 *      History:
 *              who     when    what
 *              erichn  5/10/89 initial code
 *              dannygl 5/28/89 modified DBCS usage
 *              erichn  7/04/89 handles backspaces
 *              danhi   4/16/91 32 bit version for NT
 */
#define CR              0xD
#define BACKSPACE       0x8

USHORT GetPasswdStr(
    UCHAR *buf,
    USHORT buflen,
    USHORT *len
    )
{
    USHORT ch;
    CHAR *bufPtr = buf;

    buflen -= 1;    // make space for null terminator
    *len = 0;       // GP fault probe (a la API's)

    while (TRUE)
    {
        ch = LOWORD(getch());                   // grab char silently
        if ((ch == CR) || (ch == 0xFFFF))       // end of the line
        {
            break;
        }

        if (ch == BACKSPACE)    // back up one or two
        {
            //
            // IF bufPtr == buf then the next two lines are
            // a no op.
            //
            if (bufPtr != buf)
            {
                bufPtr--;
                *len--;
            }
            continue;           // bail out, start loop over
        }

        *bufPtr = (UCHAR) ch;

        bufPtr += (*len < buflen) ? 1 : 0;   // don't overflow buf
        (*len)++;               // always increment len
    }

    *bufPtr = '\0';             // null terminate the string

    putchar('\n');

    return((*len <= buflen) ? (USHORT) 0 : (USHORT) NERR_BufTooSmall);
}


#define MAX_ARGS 9

VOID PrintMessage(
    DWORD MsgId,
    PBYTE *pArgs
    )
{
    DWORD NumArgs;
    DWORD BufSize;
    BOOL BufAllocated = FALSE;
    PBYTE Buf;
    PBYTE *pTmpArgs;
    PBYTE pSub;
    BYTE MsgBuf[256];
    PBYTE pMsgBuf = MsgBuf;

    if (!LoadStringA(GetModuleHandle(NULL), MsgId, MsgBuf, sizeof(MsgBuf)))
    {
        return;
    }

    if (pArgs)
    {
        //
        // Find out how many arguments were passed in.  We do this to detect
        // if the string requires a parameter that wasn't supplied.  If that
        // happens, we just won't substitute anything.
        //
        for (NumArgs=0, pTmpArgs=pArgs; *pTmpArgs!=NULL; NumArgs++, pTmpArgs++);

        if (NumArgs >= MAX_ARGS)
        {
            return;
        }


        //
        // We'll figure out how large our buffer should be to contain the
        // final output (length of string + sum(length of substitution params)
        //
        BufSize = strlen(MsgBuf) + 1;

        while (pSub = strchr(pMsgBuf, '%'))
        {
            DWORD Num = *(pSub+1) - '0';
            if (Num >=1 && Num <=NumArgs)
            {
                BufSize += strlen(pArgs[Num-1]) - 2;

                pMsgBuf = pSub+2;
            }
            else
            {
                pMsgBuf = pSub+1;
            }
        }


        //
        // Get space for our buffer (we multiply by 2 because we want buf to
        // be big enough for Oem character set.
        //
        Buf = GlobalAlloc(GMEM_FIXED, BufSize * 2);
        if (!Buf)
        {
            return;
        }

        BufAllocated = TRUE;


        Buf[0] = '\0';
        pMsgBuf = MsgBuf;


        //
        // Now make our final output buffer.  Strategy is to strcat
        // the first part of the string up to where the 1st substitution
        // goes, then strcat the substitution param.  Do this until no
        // more substitutions.
        //
        while (pSub = strchr(pMsgBuf, '%'))
        {
            DWORD Num = *(pSub+1) - '0';
            if (Num >=1 && Num <=NumArgs)
            {
                *pSub = '\0';

                strcat(Buf, pMsgBuf);
                strcat(Buf, pArgs[Num-1]);

                pMsgBuf = pSub+2;
            }
            else
            {
                strcat(Buf, pMsgBuf);
                strcat(Buf, pArgs[Num-1]);

                pMsgBuf = pSub+1;
            }
        }

        //
        // Now get everything after the last substitution.
        //
        if (*pMsgBuf)
        {
            strcat(Buf, pMsgBuf);
        }
    }
    else
    {
        Buf = MsgBuf;
    }

    CharToOemA(Buf, Buf);

    fprintf(stdout, Buf);

    if (BufAllocated)
    {
        GlobalFree(Buf);
    }

    return;
}


/***    GetString -- read in string with echo
 *
 *      USHORT LUI_GetString(char far *, USHORT, USHORT far *, char far *);
 *
 *      ENTRY:  buf             buffer to put string in
 *              buflen          size of buffer
 *              &len            address of USHORT to place length in
 *              &terminator     holds the char used to terminate the string
 *
 *      RETURNS:
 *              0 or NERR_BufTooSmall if user typed too much.  Buffer
 *              contents are only valid on 0 return.  Len is ALWAYS valid.
 *
 *      OTHER EFFECTS:
 *              len is set to hold number of bytes typed, regardless of
 *              buffer length.  Terminator (Arnold) is set to hold the
 *              terminating character (newline or EOF) that the user typed.
 *
 *      Read in a string a character at a time.  Is aware of DBCS.
 *
 *      History:
 *              who     when    what
 *              erichn  5/11/89 initial code
 *              dannygl 5/28/89 modified DBCS usage
 *              danhi   3/20/91 ported to 32 bits
 */

USHORT GetString(
    register UCHAR *buf,
    register USHORT buflen,
    register USHORT *len,
    register UCHAR *terminator
    )
{
    buflen -= 1;                        // make space for null terminator
    *len = 0;                           // GP fault probe (a la API's)

    while (TRUE)
    {
        *buf = (UCHAR) getchar();
        if (*buf == '\n' || *buf == (UCHAR) EOF)
        {
            break;
        }

        buf += (*len < buflen) ? 1 : 0; // don't overflow buf
        (*len)++;                       // always increment len
    }

    *terminator = *buf;                 // set terminator
    *buf = '\0';                        // null terminate the string

    return ((*len <= buflen) ? (USHORT) 0 : (USHORT) NERR_BufTooSmall);
}
