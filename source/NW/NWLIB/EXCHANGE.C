/*++

Copyright (c) 1993  Microsoft Corporation

Module Name:

    exchange.c

Abstract:

    Contains routine which packages the request buffer, makes
    the NCP request, and unpackages the response buffer.

Author:

    Hans Hurvig     (hanshu)       Aug-2000  Created
    Colin Watson    (colinw)    19-Dec-2000
    Rita Wong       (ritaw)     11-Mar-1993  FSCtl version

Environment:


Revision History:


--*/


#include <procs.h>

NTSTATUS
NwlibMakeNcp(
    IN HANDLE DeviceHandle,
    IN ULONG FsControlCode,
    IN ULONG RequestBufferSize,
    IN ULONG ResponseBufferSize,
    IN PCHAR FormatString,
    ...                           // Arguments to FormatString
    )
/*++

Routine Description:

    This function converts the input arguments into an NCP request buffer
    based on the format specified in FormatString (e.g. takes a word
    and writes it in hi-lo format in the request buffer as required by
    an NCP).

    It then makes the NCP call via the NtFsControlFile API.

    The FormatString also specifies how to convert the fields in the
    response buffer from the completed NCP call into the output
    arguments.

    The FormatString takes the form of "xxxx|yyyy" where each 'x'
    indicates an input argument to convert from, and each 'y' indicates
    an output argument to convert into.  The '|' character separates
    the input format from the output format specifications.

Arguments:

    DeviceHandle - Supplies a handle to the network file system driver
        which will be making the network request.  This function
        assumes that the handle was opened for synchronouse I/O access.

    FsControlCode - Supplies the control code which determines the
        NCP.

    RequestBufferSize - Supplies the size of the request buffer in
        bytes to be allocated by this routine.

    ResponseBufferSize - Supplies the size of the response buffer in
        bytes to be allocated by this routine.

    FormatString - Supplies an ANSI string which describes how to
       convert from the input arguments into NCP request fields, and
       from the NCP response fields into the output arguments.

         Field types, request/response:

            'b'      byte              ( byte   /  byte* )
            'w'      hi-lo word        ( word   /  word* )
            'd'      hi-lo dword       ( dword  /  dword* )
            '-'      zero/skip byte    ( void )
            '='      zero/skip word    ( void )
            ._.      zero/skip string  ( word )
            'p'      pstring           ( char* )
            'c'      cstring           ( char* )
            'C'      cstring followed skip word ( char*, word )
            'r'      raw bytes         ( byte*, word )
            'u'      p unicode string  ( UNICODE_STRING * )
            'U'      p uppercase string( UNICODE_STRING * )
            'W'      word n followed by an array of word[n] ( word, word* )

Return Value:

    Return code from the NCP call.

--*/
{
    NTSTATUS status;

    va_list Arguments;
    PCHAR z;
    WORD t = 1;
    ULONG data_size;

    LPBYTE RequestBuffer = NULL;
    LPBYTE ResponseBuffer;

    IO_STATUS_BLOCK IoStatusBlock;
    DWORD ReturnedDataSize;


    //
    // Allocate memory for request and response buffers.
    //
    RequestBuffer = LocalAlloc(
                        LMEM_ZEROINIT,
                        RequestBufferSize + ResponseBufferSize
                        );

    if (RequestBuffer == NULL) {
        KdPrint(("NWLIB: NwlibMakeNcp LocalAlloc failed %lu\n", GetLastError()));
        return STATUS_NO_MEMORY;
    }

    ResponseBuffer = (LPBYTE) ((ULONG) RequestBuffer + RequestBufferSize);


    va_start( Arguments, FormatString );

    //
    // Convert the input arguments into request packet.
    //
    z = FormatString;

    data_size = 0;

    while ( *z && *z != '|')
    {
        switch ( *z )
        {
        case '=':
            RequestBuffer[data_size++] = 0;
        case '-':
            RequestBuffer[data_size++] = 0;
            break;

        case '_':
        {
            WORD l = va_arg ( Arguments, WORD );
            if (data_size + l > RequestBufferSize)
            {
                KdPrint(("NWLIB: NwlibMakeNcp case '_' request buffer too small\n"));
                status = STATUS_BUFFER_TOO_SMALL;
                goto CleanExit;
            }
            while ( l-- )
                RequestBuffer[data_size++] = 0;
            break;
        }

        case 'b':
            RequestBuffer[data_size++] = va_arg ( Arguments, BYTE );
            break;

        case 'w':
        {
            WORD w = va_arg ( Arguments, WORD );
            RequestBuffer[data_size++] = (BYTE) (w >> 8);
            RequestBuffer[data_size++] = (BYTE) (w >> 0);
            break;
        }

        case 'd':
        {
            DWORD d = va_arg ( Arguments, DWORD );
            RequestBuffer[data_size++] = (BYTE) (d >> 24);
            RequestBuffer[data_size++] = (BYTE) (d >> 16);
            RequestBuffer[data_size++] = (BYTE) (d >>  8);
            RequestBuffer[data_size++] = (BYTE) (d >>  0);
            break;
        }

        case 'c':
        {
            char* c = va_arg ( Arguments, char* );
            WORD  l = strlen( c );
            if (data_size + l > RequestBufferSize)
            {
                KdPrint(("NWLIB: NwlibMakeNcp case 'c' request buffer too small\n"));
                status = STATUS_BUFFER_TOO_SMALL;
                goto CleanExit;
            }
            RtlCopyMemory( &RequestBuffer[data_size], c, l+1 );
            data_size += l + 1;
            break;
        }

        case 'C':
        {
            char* c = va_arg ( Arguments, char* );
            WORD l = va_arg ( Arguments, WORD );
            WORD len = strlen( c ) + 1;
            if (data_size + l > RequestBufferSize)
            {
                KdPrint(("NWLIB: NwlibMakeNcp case 'C' request buffer too small\n"));
                status = STATUS_BUFFER_TOO_SMALL;
                goto CleanExit;
            }

            RtlCopyMemory( &RequestBuffer[data_size], c, len > l? l : len);
            data_size += l;
            RequestBuffer[data_size-1] = 0;
            break;
        }

        case 'p':
        {
            char* c = va_arg ( Arguments, char* );
            BYTE  l = strlen( c );
            if (data_size + l > RequestBufferSize)
            {
                KdPrint(("NWLIB: NwlibMakeNcp case 'p' request buffer too small\n"));
                status = STATUS_BUFFER_TOO_SMALL;
                goto CleanExit;
            }
            RequestBuffer[data_size++] = l;
            RtlCopyMemory( &RequestBuffer[data_size], c, l );
            data_size += l;
            break;
        }

        case 'u':
        {
            PUNICODE_STRING pUString = va_arg ( Arguments, PUNICODE_STRING );
            OEM_STRING OemString;
            ULONG Length;

            //
            //  Calculate required string length, excluding trailing NUL.
            //

            Length = RtlUnicodeStringToOemSize( pUString ) - 1;
            ASSERT( Length < 0x100 );

            if ( data_size + Length > RequestBufferSize ) {
                KdPrint(("NWLIB: NwlibMakeNcp case 'u' request buffer too small\n"));
                status = STATUS_BUFFER_TOO_SMALL;
                goto CleanExit;
            }

            RequestBuffer[data_size++] = (UCHAR)Length;
            OemString.Buffer = &RequestBuffer[data_size];
            OemString.MaximumLength = (USHORT)Length + 1;

            //
            // BUGBUG: The following should call RtlUnicodeStringToCountedOemString
            // but this routine is not exported by ntdll.dll so we can't get to it.
            //
            status = RtlUnicodeStringToOemString( &OemString, pUString, FALSE );
            ASSERT( NT_SUCCESS( status ));
            data_size += (USHORT)Length;
            break;
        }

        case 'U':
        {
            //
            //  UPPERCASE the string, copy it from unicode to the packet
            //

            PUNICODE_STRING pUString = va_arg ( Arguments, PUNICODE_STRING );
            UNICODE_STRING UUppercaseString;
            OEM_STRING OemString;
            ULONG Length;

            RtlUpcaseUnicodeString( &UUppercaseString, pUString, TRUE );
            pUString = &UUppercaseString;

            //
            //  Calculate required string length, excluding trailing NUL.
            //

            Length = RtlUnicodeStringToOemSize( pUString ) - 1;
            ASSERT( Length < 0x100 );

            if ( data_size + Length > RequestBufferSize ) {
                KdPrint(("NWLIB: NwlibMakeNcp case 'U' request buffer too small\n"));
                status = STATUS_BUFFER_TOO_SMALL;
                goto CleanExit;
            }

            RequestBuffer[data_size++] = (UCHAR)Length;
            OemString.Buffer = &RequestBuffer[data_size];
            OemString.MaximumLength = (USHORT)Length + 1;

            //
            // BUGBUG: The following should call RtlUnicodeStringToCountedOemString
            // but this routine is not exported by ntdll.dll so we can't get to it.
            //
            status = RtlUnicodeStringToOemString( &OemString, pUString, FALSE );
            ASSERT( NT_SUCCESS( status ));

            RtlFreeUnicodeString( &UUppercaseString );

            data_size += (USHORT)Length;
            break;
        }

        case 'r':
        {
            BYTE* b = va_arg ( Arguments, BYTE* );
            WORD  l = va_arg ( Arguments, WORD );
            if ( data_size + l > RequestBufferSize )
            {
                KdPrint(("NWLIB: NwlibMakeNcp case 'r' request buffer too small\n"));
                status = STATUS_BUFFER_TOO_SMALL;
                goto CleanExit;
            }
            RtlCopyMemory( &RequestBuffer[data_size], b, l );
            data_size += l;
            break;
        }

        default:
            KdPrint(("NWLIB: NwlibMakeNcp invalid request field, %x\n", *z));
            ASSERT(FALSE);
        }

        if ( data_size > RequestBufferSize )
        {
            KdPrint(("NWLIB: NwlibMakeNcp too much request data\n"));
            status = STATUS_BUFFER_TOO_SMALL;
            goto CleanExit;
        }


        z++;
    }


    //
    // Make the NCP request
    //
    status = NtFsControlFile(
                 DeviceHandle,
                 NULL,
                 NULL,
                 NULL,
                 &IoStatusBlock,
                 FsControlCode,
                 (PVOID) RequestBuffer,
                 RequestBufferSize,
                 (PVOID) ResponseBuffer,
                 ResponseBufferSize
                 );

    if (status == STATUS_SUCCESS) {
        status = IoStatusBlock.Status;
    }

    if (status != STATUS_SUCCESS) {

#if 0
        if (! NT_SUCCESS(status)) {
            KdPrint(("NWLIB: NwlibMakeNcp: NtFsControlFile returns x%08lx\n", status));
        }
#endif

        goto CleanExit;
    }


    ReturnedDataSize = IoStatusBlock.Information; // Number of bytes returned
                                                  //    in ResponseBuffer


    //
    // Convert the response packet into output arguments.
    //

    data_size = 0;

    if (*z && *z == '|') {
        z++;
    }

    while ( *z )
    {
        switch ( *z )
        {
        case '-':
            data_size += 1;
            break;

        case '=':
            data_size += 2;
            break;

        case '_':
        {
            WORD l = va_arg ( Arguments, WORD );
            data_size += l;
            break;
        }

        case 'b':
        {
            BYTE* b = va_arg ( Arguments, BYTE* );
            *b = ResponseBuffer[data_size++];
            break;
        }

        case 'w':
        {
            BYTE* b = va_arg ( Arguments, BYTE* );
            b[1] = ResponseBuffer[data_size++];
            b[0] = ResponseBuffer[data_size++];
            break;
        }

        case 'd':
        {
            BYTE* b = va_arg ( Arguments, BYTE* );
            b[3] = ResponseBuffer[data_size++];
            b[2] = ResponseBuffer[data_size++];
            b[1] = ResponseBuffer[data_size++];
            b[0] = ResponseBuffer[data_size++];
            break;
        }

        case 'c':
        {
            char* c = va_arg ( Arguments, char* );
            WORD  l = strlen( &ResponseBuffer[data_size] );
            if ( data_size+l+1 < ReturnedDataSize ) {
                RtlCopyMemory( c, &ResponseBuffer[data_size], l+1 );
            }
            data_size += l+1;
            break;
        }

        case 'C':
        {
            char* c = va_arg ( Arguments, char* );
            WORD l = va_arg ( Arguments, WORD );
            WORD len = strlen( &ResponseBuffer[data_size] ) + 1;

            if ( data_size + l < ReturnedDataSize ) {
                RtlCopyMemory( c, &ResponseBuffer[data_size], len > l ? l :len);
            }
            c[l-1] = 0;
            data_size += l;
            break;

        }

        case 'p':
        {
            char* c = va_arg ( Arguments, char* );
            BYTE  l = ResponseBuffer[data_size++];
            if ( data_size+l <= ReturnedDataSize ) {
                RtlCopyMemory( c, &ResponseBuffer[data_size], l );
                c[l] = 0;
            }
            data_size += l;
            break;
        }

        case 'r':
        {
            BYTE* b = va_arg ( Arguments, BYTE* );
            WORD  l = va_arg ( Arguments, WORD );
            RtlCopyMemory( b, &ResponseBuffer[data_size], l );
            data_size += l;
            break;
        }

        case 'W':
        {
            BYTE* b = va_arg ( Arguments, BYTE* );
            BYTE* w = va_arg ( Arguments, BYTE* );
            WORD  i;

            b[1] = ResponseBuffer[data_size++];
            b[0] = ResponseBuffer[data_size++];

            for ( i = 0; i < ((WORD) *b); i++, w += sizeof(WORD) )
            {
                w[1] = ResponseBuffer[data_size++];
                w[0] = ResponseBuffer[data_size++];
            }
            break;
        }

        default:
            KdPrint(("NWLIB: NwlibMakeNcp invalid response field, %x\n", *z));
            ASSERT(FALSE);
        }

        if ( data_size > ReturnedDataSize )
        {
            KdPrint(("NWLIB: NwlibMakeNcp not enough response data\n"));
            status = STATUS_UNSUCCESSFUL;
            goto CleanExit;
        }

        z++;
    }

    status = STATUS_SUCCESS;

CleanExit:
    if (RequestBuffer != NULL) {
        (void) LocalFree((HLOCAL) RequestBuffer);
    }

    va_end( Arguments );

    return status;
}

