/*++

Copyright (c) 1993  Microsoft Corporation

Module Name:

    vwasync.c

Abstract:

    ntVdm netWare (Vw) IPX/SPX Functions

    Vw: The peoples' network

    Contains Asyncrhonous Event Scheduler (thread)

    Contents:
        VwAesThread
        (CheckPendingIpxRequests)

Author:

    Richard L Firth (rfirth) 30-Sep-1993

Environment:

    User-mode Win32

Revision History:

    30-Sep-1993 rfirth
        Created

--*/

#include "vw.h"
#pragma hdrstop

//
// private routine prototypes
//

PRIVATE
VOID
CheckPendingIpxRequests(
    VOID
    );

//
// global data
//

WORD AesTickCount;

//
// functions
//


DWORD
VwAesThread(
    IN LPVOID Parameters
    )

/*++

Routine Description:

    Provides the functionality of the Asynchronous Event Scheduler (AES) in the
    Netware world:

        - updates the tick count
        - completes any matured timer events
        - checks any pending requests and schedules the next action

    This thread wakes up every PC tick (1/18 second)

Arguments:

    Parameters  - unused

Return Value:

    DWORD
        0

--*/

{
    BOOL fOperationPerformed = FALSE ;
    UNREFERENCED_PARAMETER(Parameters);

    while (TRUE) 
    {
        if (!fOperationPerformed)
            Sleep(ONE_TICK);
        else
            Sleep(0) ;

        ++AesTickCount;
        ScanTimerList();
        CheckPendingIpxRequests();
        CheckPendingSpxRequests(&fOperationPerformed);
    }

    return 0;   // compiler-pacifier
}


PRIVATE
VOID
CheckPendingIpxRequests(
    VOID
    )

/*++

Routine Description:

    Polls the opened, active non-blocking IPX sockets to see if there is anything
    to do (data to receive, availability to send, timeouts)

Arguments:

    None.

Return Value:

    None.

--*/

{
    LPSOCKET_INFO pActiveSocket = NULL;

    //
    // search SOCKET_INFO structures for something to do. Could do select()
    // but we have most of the info anyway. We use the BFI filter mechanism
    //

    while (pActiveSocket = FindActiveSocket(pActiveSocket)) {
        if (pActiveSocket->Flags & SOCKET_FLAG_SENDING) {
            IpxSendNext(pActiveSocket);
        }
        if (pActiveSocket->Flags & SOCKET_FLAG_LISTENING) {
            IpxReceiveNext(pActiveSocket);
        }
    }
}
