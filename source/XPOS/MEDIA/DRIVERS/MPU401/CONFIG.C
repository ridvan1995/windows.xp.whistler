/****************************************************************************
 *
 *   config.c
 *
 *   Copyright (c) 1996-2000 Microsoft Corporation.  All Rights Reserved.
 *
 ***************************************************************************/

#include <windows.h>
#include <mmsystem.h>
#include "registry.h"
#include "mpu401.h"

/*****************************************************************************

    internal function prototypes

 ****************************************************************************/
 void Configure(HWND hDlg);

static int PortToId(DWORD wPort)
{
    switch(wPort) {
        case 0x200:  return 0;
        case 0x210:  return 1;
        case 0x220:  return 2;
        case 0x230:  return 3;
        case 0x240:  return 4;
        case 0x250:  return 4;
        case 0x260:  return 6;
        case 0x270:  return 7;

        case 0x300:  return 8;
        case 0x310:  return 9;
        case 0x320:  return 10;
        case 0x330:  return 11;
        case 0x340:  return 12;
        case 0x350:  return 13;
        case 0x360:  return 14;
        case 0x370:  return 15;

        default:     return -1;
    }
}

static DWORD IdToPort(int id)
{
    switch(id) {
        case 0:  return 0x200;
        case 1:  return 0x210;
        case 2:  return 0x220;
        case 3:  return 0x230;
        case 4:  return 0x240;
        case 5:  return 0x250;
        case 6:  return 0x260;
        case 7:  return 0x270;

        case 8:  return 0x300;
        case 9:  return 0x310;
        case 10: return 0x320;
        case 11: return 0x330;
        case 12: return 0x340;
        case 13: return 0x350;
        case 14: return 0x360;
        case 15: return 0x370;

        default:       return (DWORD)-1;
    }
}

static int IntToId(DWORD Int)
{
    switch(Int) {
        case 2:  return IDC_2;
        case 9:  return IDC_2;
        case 3:  return IDC_3;
        case 5:  return IDC_5;
        case 7:  return IDC_7;
        default: return (DWORD)-1;
    }
}

static DWORD IdToInt(int id)
{
    switch(id) {
        case IDC_2:  return 9;
        case IDC_3:  return 3;
        case IDC_5:  return 5;
        case IDC_7:  return 7;
        default:     return (DWORD)-1;
    }
}

/***************************************************************************/

void ConfigErrorMsgBox(HWND hDlg, UINT StringId)
{
WCHAR szErrorBuffer[MAX_ERR_STRING];    /* buffer for error messages */

    LoadString(ghModule, StringId, szErrorBuffer, sizeof(szErrorBuffer));
    MessageBox(hDlg, szErrorBuffer, STR_PRODUCTNAME, MB_OK|MB_ICONEXCLAMATION);
}

/***************************************************************************/

LRESULT ConfigRemove(HWND hDlg)
{

    LRESULT rc;

    //
    // Remove the soundblaster driver entry from the registry
    //

    rc = DrvRemoveDriver(&RegAccess);

    if (rc == DRVCNF_CANCEL) {

       /*
        *  Tell the user there's a problem
        */
        ConfigErrorMsgBox(hDlg, IDS_FAILREMOVE);

    }

    return rc;
}

/****************************************************************************
 * @doc INTERNAL
 *
 * @api int | Config | This puts up the configuration dialog box.
 *
 * @parm HWND | hWnd | Our Window handle.
 *
 * @parm HANDLE | hInstance | Our instance handle.
 *
 * @rdesc Returns whatever was returned from the dialog box procedure.
 ***************************************************************************/
int Config(HWND hWnd, HANDLE hInstance)
{
    return DialogBox(hInstance,
                     MAKEINTATOM(DLG_CONFIG),
                     hWnd,
                     (DLGPROC)ConfigDlgProc);
}

/****************************************************************************
 * @doc INTERNAL
 *
 * @api BOOL | SetDriverConfig | Callback to set config info in the registry
 *         does not write uninitialized values (-1)
 *
 * @parm PVOID | Context | Our context.
 *
 * @rdesc Returns TRUE if success, FALSE otherwise.
 ***************************************************************************/
BOOL SetDriverConfig(PVOID Context)
{
    MPU_CONFIG *Config;

    Config = (MPU_CONFIG *)Context;

    /* We set the IO port and interrupt values      */
    /* and set the returned version to 0            */
    /*                                              */
    /* If any of these calls fail then give up      */

    if (Config->Port != (DWORD)-1 &&
        DrvSetDeviceParameter(
            &RegAccess,
            SOUND_REG_PORT,
            Config->Port) != ERROR_SUCCESS ||
        Config->Int != (DWORD)-1 &&
        DrvSetDeviceParameter(
            &RegAccess,
            SOUND_REG_INTERRUPT,
            Config->Int) != ERROR_SUCCESS) {

        return FALSE;
    } else {
        return TRUE;
    }
}

/****************************************************************************
 * @doc INTERNAL
 *
 * @api void | GetPortAndInt | Determines which port and interrupt settings
 *     the user has chosen in the configuration dialog box.
 *
 * @parm HWND | hDlg | Handle to the configuration dialog box.
 *
 * @rdesc Structure containing new port and interrupt
 ***************************************************************************/
MPU_CONFIG GetPortAndInt(HWND hDlg)
{
    MPU_CONFIG NewConfig;
    int  id, nIndex;

    NewConfig.Port = (DWORD)-1;
    NewConfig.Int = (DWORD)-1;

    // get the port - drude
    nIndex = SendDlgItemMessage(hDlg, IDC_PORTS, CB_GETCURSEL, 0, 0);
    NewConfig.Port = IdToPort(nIndex);

    // get the int - drude
    for (id = IDC_FIRSTINT; id <= IDC_LASTINT; id++)
        if (IsDlgButtonChecked(hDlg, id)) {
            NewConfig.Int = IdToInt(id);
            break;
        }

    return NewConfig;
}

/****************************************************************************
 * @doc INTERNAL
 *
 * @api int | ConfigDlgProc | Dialog proc for the configuration dialog box.
 *
 * @parm HWND | hDlg | Handle to the configuration dialog box.
 *
 * @parm WORD | msg | Message sent to the dialog box.
 *
 * @parm WORD | wParam | Message dependent parameter.
 *
 * @parm LONG | lParam | Message dependent parameter.
 *
 * @rdesc Returns DRV_RESTART if the user has changed settings, which will
 *     cause the drivers applet which launched this to give the user a
 *     message about having to restart Windows for the changes to take
 *     effect.  If the user clicks on "Cancel" or if no settings have changed,
 *     DRV_CANCEL is returned.
 ***************************************************************************/
int ConfigDlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
    int     id;
    static  MPU_CONFIG  StartConfig;  /* Initial port */
    TCHAR szData[10];  // loadstring data

    switch (msg) {
        case WM_INITDIALOG:
            StartConfig.Int = ConfigGetIRQ();
            StartConfig.Port = ConfigGetPortBase();

            // load the list box string - drude
            LoadString(ghModule, IDS_200, szData, sizeof(szData));
            SendDlgItemMessage(hDlg, IDC_PORTS, CB_ADDSTRING, 0, (LPARAM)(LPTSTR)szData);

            LoadString(ghModule, IDS_210, szData, sizeof(szData));
            SendDlgItemMessage(hDlg, IDC_PORTS, CB_ADDSTRING, 0, (LPARAM)(LPTSTR)szData);

            LoadString(ghModule, IDS_220, szData, sizeof(szData));
            SendDlgItemMessage(hDlg, IDC_PORTS, CB_ADDSTRING, 0, (LPARAM)(LPTSTR)szData);

            LoadString(ghModule, IDS_230, szData, sizeof(szData));
            SendDlgItemMessage(hDlg, IDC_PORTS, CB_ADDSTRING, 0, (LPARAM)(LPTSTR)szData);

            LoadString(ghModule, IDS_240, szData, sizeof(szData));
            SendDlgItemMessage(hDlg, IDC_PORTS, CB_ADDSTRING, 0, (LPARAM)(LPTSTR)szData);

            LoadString(ghModule, IDS_250, szData, sizeof(szData));
            SendDlgItemMessage(hDlg, IDC_PORTS, CB_ADDSTRING, 0, (LPARAM)(LPTSTR)szData);

            LoadString(ghModule, IDS_260, szData, sizeof(szData));
            SendDlgItemMessage(hDlg, IDC_PORTS, CB_ADDSTRING, 0, (LPARAM)(LPTSTR)szData);

            LoadString(ghModule, IDS_270, szData, sizeof(szData));
            SendDlgItemMessage(hDlg, IDC_PORTS, CB_ADDSTRING, 0, (LPARAM)(LPTSTR)szData);

            LoadString(ghModule, IDS_300, szData, sizeof(szData));
            SendDlgItemMessage(hDlg, IDC_PORTS, CB_ADDSTRING, 0, (LPARAM)(LPTSTR)szData);

            LoadString(ghModule, IDS_310, szData, sizeof(szData));
            SendDlgItemMessage(hDlg, IDC_PORTS, CB_ADDSTRING, 0, (LPARAM)(LPTSTR)szData);

            LoadString(ghModule, IDS_320, szData, sizeof(szData));
            SendDlgItemMessage(hDlg, IDC_PORTS, CB_ADDSTRING, 0, (LPARAM)(LPTSTR)szData);

            LoadString(ghModule, IDS_330, szData, sizeof(szData));
            SendDlgItemMessage(hDlg, IDC_PORTS, CB_ADDSTRING, 0, (LPARAM)(LPTSTR)szData);

            LoadString(ghModule, IDS_340, szData, sizeof(szData));
            SendDlgItemMessage(hDlg, IDC_PORTS, CB_ADDSTRING, 0, (LPARAM)(LPTSTR)szData);

            LoadString(ghModule, IDS_350, szData, sizeof(szData));
            SendDlgItemMessage(hDlg, IDC_PORTS, CB_ADDSTRING, 0, (LPARAM)(LPTSTR)szData);

            LoadString(ghModule, IDS_360, szData, sizeof(szData));
            SendDlgItemMessage(hDlg, IDC_PORTS, CB_ADDSTRING, 0, (LPARAM)(LPTSTR)szData);

            LoadString(ghModule, IDS_370, szData, sizeof(szData));
            SendDlgItemMessage(hDlg, IDC_PORTS, CB_ADDSTRING, 0, (LPARAM)(LPTSTR)szData);


            // select the proper combo item - drude
            if ((id = PortToId(StartConfig.Port)) != -1)
                SendDlgItemMessage(hDlg, IDC_PORTS, CB_SETCURSEL, id, 0);

            if ((id = IntToId(StartConfig.Int)) != -1)
                CheckRadioButton(hDlg, IDC_FIRSTINT, IDC_LASTINT, id);
            break;

        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case IDOK:
                    Configure(hDlg);
                    break;

                case IDCANCEL:

                   /*
                    *  Restore to state on entry to dialog if we
                    *  possibly can
                    */

                    if (bInstall) {
                        DrvRemoveDriver(&RegAccess);
                    } else {

                        DrvConfigureDriver(&RegAccess,
                                           STR_DRIVERNAME,
                                           SoundDriverTypeNormal,
                                           SetDriverConfig,
                                           &StartConfig);
                    }

                    EndDialog(hDlg, DRVCNF_CANCEL);
                    break;


                case IDC_2:
                case IDC_3:
                case IDC_5:
                case IDC_7:
                    CheckRadioButton(hDlg, IDC_FIRSTINT, IDC_LASTINT, wParam);
                    break;

                default:
                    break;
            }
            break;

        default:
            return FALSE;
    }

    return TRUE;
}



/**************************************************************************
 *
 * Function : Configure
 *
 * Arguments :
 *
 *    NewPort
 *    NewInterrupt
 *    NewChannel
 *
 * Description
 *
 *    The user has selected a (new) configuration.  There are a number
 *    of (binary) state varibles to consider :
 *
 *    1) Is this a new install?
 *
 *    2) Was the driver previously loaded?
 *
 *    3) Has the configuration changed?
 *
 *    4) If the driver was previously loaded could it be unloaded?
 *
 *    5) Could the driver be loaded ?
 *
 *    Possible actions are :
 *
 *    a. Try unload
 *
 *    b. Try reload
 *
 *    c. Warn user parameters are not accepted by driver
 *
 *    d. Put up reboot menu
 *
 *    e. Update registry with new config data
 *
 *    f. Check version and issue necessary warnings
 *
 * Notes :
 *    A - 1 & 2 should not happen
 *
 *    B - ~2 => 4 irrelevant
 *
 *    C - configuration change irrelevant for new install
 *
 *    There's never and point in unloading !
 *
 *    If the configuration changes we have to put it in the registry to
 *    test it.
 *
 *    We must correctly detect the 'not loaded' state via SC manager
 *
 *    Don't warn them about the version if it's not an install (how
 *    do we know ?)
 *
 *
 * The logic is :
 *
 * 1 2 3 4 5 |    a b c d e f     Comments
 * ----------|-------------------------------------------------------------
 * 0 0 0 0 0 |                    Config not changed, not new so don't load
 * 0 0 0 0 1 |                    Config not changed, not new so don't load
 * 0 0 0 1 0 |                    B
 * 0 0 0 1 1 |                    B
 * 0 0 1 0 0 |      X X   X       Assume load failed due to config failure
 * 0 0 1 0 1 |      X     X       Was not loaded, config changed, now loaded OK
 * 0 0 1 1 0 |                    B
 * 0 0 1 1 1 |                    B
 * 0 1 0 0 0 |                    Config not changed, driver loaded so OK
 * 0 1 0 0 1 |                    Config not changed, driver loaded so OK
 * 0 1 0 1 0 |                    Config not changed, driver loaded so OK
 * 0 1 0 1 1 |                    Config not changed, driver loaded so OK
 * 0 1 1 0 0 |          X X       Driver was running OK with old config
 * 0 1 1 0 1 |          X X       Driver was running OK with old config
 * 0 1 1 1 0 |          X X       Driver was running OK with old config
 * 0 1 1 1 1 |          X X       Driver was running OK with old config
 * 1 0 0 0 0 |      X X   X       Assume load failed due to config failure
 * 1 0 0 0 1 |      X     X X     Good install and load
 * 1 0 0 1 0 |                    B
 * 1 0 0 1 1 |                    B
 * 1 0 1 0 0 |                    C
 * 1 0 1 0 1 |                    C
 * 1 0 1 1 0 |                    B
 * 1 0 1 1 1 |                    B
 * 1 1 0 0 0 |                    A
 * 1 1 0 0 1 |                    A
 * 1 1 0 1 0 |                    A
 * 1 1 0 1 1 |                    A
 * 1 1 1 0 0 |                    A
 * 1 1 1 0 1 |                    A
 * 1 1 1 1 0 |                    A
 * 1 1 1 1 1 |                    A
 *
 **************************************************************************/

 void Configure(HWND hDlg)
 {
     MPU_CONFIG NewConfig;        /* New port and int chosen in config box */
     BOOL Success;

    /*
     *  Get the new configuration which the user entered
     */

     NewConfig = GetPortAndInt(hDlg);

    /*
     *     NOTE - even if the configuration has not changed the driver
     *     may now load in some cases - so even in this case try it.
     *
     *     Put the new config in the registry
     *
     *     unload the driver if needed (note this may fail but we've
     *     no way of telling whether it was  loaded in the first place).
     *
     *     if the driver is not loaded
     *
     *         try loading the driver
     *
     *         if the load failed put up a message
     *
     *         if the load succeeded put up a warning if we don't like
     *         the version
     *
     *     if the driver is already loaded ask them if they would like
     *     to reboot (ie return DRVCNF_RESTART (note that if the driver
     *     is currently loaded then changing the config will stop it
     *     working !).
     *
     */


    /*
     *  We have a new config - Configure the driver for this configuration
     */

     Success = DrvConfigureDriver(&RegAccess,
                                  STR_DRIVERNAME,
                                  SoundDriverTypeNormal,
                                  SetDriverConfig,
                                  &NewConfig);
    /*
     *  Find out what we got back
     */


     if (!Success) {

         ConfigErrorMsgBox(hDlg, IDS_ERRBADCONFIG);

     } else {

        /*
         *  If we're an install then check out the version
         */

         if (bInstall) {


             bInstall = FALSE;
         }
         EndDialog(hDlg, DRVCNF_RESTART);
         return;
     }
 }

