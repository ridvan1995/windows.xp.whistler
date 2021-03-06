/*++

Copyright (c) 1993  Microsoft Corporation

Module Name:

    nwxchg.h

Abstract:

    Header for generic NCP calling routine.

Author:

    Rita Wong      (ritaw)      11-Mar-1993

Environment:


Revision History:

--*/

#ifndef _NW_XCHG_INCLUDED_
#define _NW_XCHG_INCLUDED_

NTSTATUS
NwlibMakeNcp(
    IN HANDLE DeviceHandle,
    IN ULONG FsControlCode,
    IN ULONG RequestBufferSize,
    IN ULONG ResponseBufferSize,
    IN PCHAR FormatString,
    ...                           // Arguments to FormatString
    );

#endif // _NW_XCHG_INCLUDED_
