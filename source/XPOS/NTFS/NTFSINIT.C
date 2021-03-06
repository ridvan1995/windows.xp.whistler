/*++

Copyright (c) 2000  Microsoft Corporation

Module Name:

    NtfsInit.c

Abstract:

    This module implements the DRIVER_INITIALIZATION routine for Ntfs

Author:

    Gary Kimura     [GaryKi]        21-May-2000

Revision History:

--*/

#include "NtfsProc.h"

NTSTATUS
DriverEntry (
    IN PDRIVER_OBJECT DriverObject,
    IN PUNICODE_STRING RegistryPath
    );

NTSTATUS
NtfsGet8dot3NameStatus (
    IN PUNICODE_STRING ValueName,
    IN OUT PULONG Value
    );

#ifdef ALLOC_PRAGMA
#pragma alloc_text(INIT, DriverEntry)
#pragma alloc_text(INIT, NtfsGet8dot3NameStatus)
#endif

#define COMPATIBILITY_MODE_KEY_NAME L"\\Registry\\Machine\\System\\CurrentControlSet\\Control\\FileSystem"
#define COMPATIBILITY_MODE_VALUE_NAME L"NtfsDisable8dot3NameCreation"

#define KEY_WORK_AREA ((sizeof(KEY_VALUE_FULL_INFORMATION) + \
                        sizeof(ULONG)) + 64)


NTSTATUS
DriverEntry(
    IN PDRIVER_OBJECT DriverObject,
    IN PUNICODE_STRING RegistryPath
    )

/*++

Routine Description:

    This is the initialization routine for the Ntfs file system
    device driver.  This routine creates the device object for the FileSystem
    device and performs all other driver initialization.

Arguments:

    DriverObject - Pointer to driver object created by the system.

Return Value:

    NTSTATUS - The function value is the final status from the initialization
        operation.

--*/

{
    NTSTATUS Status;
    UNICODE_STRING UnicodeString;
    PDEVICE_OBJECT DeviceObject;

    UNICODE_STRING ValueName;
    ULONG Value;

    PAGED_CODE();

    //
    //  Compute the last access increment.  We convert the number of
    //  minutes to number of 1/100 of nanoseconds.  We have to be careful
    //  not to overrun 32 bits for any multiplier.
    //
    //  To reach 1/100 of nanoseconds per minute we take
    //
    //      1/100 nanoseconds * 10      = 1 microsecond
    //                        * 1000    = 1 millesecond
    //                        * 1000    = 1 second
    //                        * 60      = 1 minute
    //
    //  Then multiply this by the last access increment in minutes.
    //

    NtfsLastAccess = ( 10 * 1000 * 1000 * 60 );

    NtfsLastAccess = ( NtfsLastAccess * LAST_ACCESS_INCREMENT_MINUTES );

    //
    // Create the device object.
    //

    RtlInitUnicodeString( &UnicodeString, L"\\Ntfs" );

    Status = IoCreateDevice( DriverObject,
                             0,
                             &UnicodeString,
                             FILE_DEVICE_DISK_FILE_SYSTEM,
                             0,
                             FALSE,
                             &DeviceObject );

    if (!NT_SUCCESS( Status )) {

        return Status;
    }

    //
    //  Note that because of the way data caching is done, we set neither
    //  the Direct I/O or Buffered I/O bit in DeviceObject->Flags.  If
    //  data is not in the cache, or the request is not buffered, we may,
    //  set up for Direct I/O by hand.
    //

    //
    // Initialize the driver object with this driver's entry points.
    //

    DriverObject->MajorFunction[IRP_MJ_CREATE]                   = (PDRIVER_DISPATCH)NtfsFsdCreate;
    DriverObject->MajorFunction[IRP_MJ_CLOSE]                    = (PDRIVER_DISPATCH)NtfsFsdClose;
    DriverObject->MajorFunction[IRP_MJ_READ]                     = (PDRIVER_DISPATCH)NtfsFsdRead;
    DriverObject->MajorFunction[IRP_MJ_WRITE]                    = (PDRIVER_DISPATCH)NtfsFsdWrite;
    DriverObject->MajorFunction[IRP_MJ_QUERY_INFORMATION]        = (PDRIVER_DISPATCH)NtfsFsdQueryInformation;
    DriverObject->MajorFunction[IRP_MJ_SET_INFORMATION]          = (PDRIVER_DISPATCH)NtfsFsdSetInformation;
    DriverObject->MajorFunction[IRP_MJ_QUERY_EA]                 = (PDRIVER_DISPATCH)NtfsFsdQueryEa;
    DriverObject->MajorFunction[IRP_MJ_SET_EA]                   = (PDRIVER_DISPATCH)NtfsFsdSetEa;
    DriverObject->MajorFunction[IRP_MJ_FLUSH_BUFFERS]            = (PDRIVER_DISPATCH)NtfsFsdFlushBuffers;
    DriverObject->MajorFunction[IRP_MJ_QUERY_VOLUME_INFORMATION] = (PDRIVER_DISPATCH)NtfsFsdQueryVolumeInformation;
    DriverObject->MajorFunction[IRP_MJ_SET_VOLUME_INFORMATION]   = (PDRIVER_DISPATCH)NtfsFsdSetVolumeInformation;
    DriverObject->MajorFunction[IRP_MJ_DIRECTORY_CONTROL]        = (PDRIVER_DISPATCH)NtfsFsdDirectoryControl;
    DriverObject->MajorFunction[IRP_MJ_FILE_SYSTEM_CONTROL]      = (PDRIVER_DISPATCH)NtfsFsdFileSystemControl;
    DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL]           = (PDRIVER_DISPATCH)NtfsFsdDeviceControl;
    DriverObject->MajorFunction[IRP_MJ_LOCK_CONTROL]             = (PDRIVER_DISPATCH)NtfsFsdLockControl;
    DriverObject->MajorFunction[IRP_MJ_CLEANUP]                  = (PDRIVER_DISPATCH)NtfsFsdCleanup;
    DriverObject->MajorFunction[IRP_MJ_QUERY_SECURITY]           = (PDRIVER_DISPATCH)NtfsFsdQuerySecurityInfo;
    DriverObject->MajorFunction[IRP_MJ_SET_SECURITY]             = (PDRIVER_DISPATCH)NtfsFsdSetSecurityInfo;
    DriverObject->MajorFunction[IRP_MJ_SHUTDOWN]                 = (PDRIVER_DISPATCH)NtfsFsdShutdown;

    DriverObject->FastIoDispatch = &NtfsFastIoDispatch;

    NtfsFastIoDispatch.SizeOfFastIoDispatch =    sizeof(FAST_IO_DISPATCH);
    NtfsFastIoDispatch.FastIoCheckIfPossible =   NtfsFastIoCheckIfPossible;  //  CheckForFastIo
    NtfsFastIoDispatch.FastIoRead =              FsRtlCopyRead;              //  Read
    NtfsFastIoDispatch.FastIoWrite =             FsRtlCopyWrite;             //  Write
    NtfsFastIoDispatch.FastIoQueryBasicInfo =    NtfsFastQueryBasicInfo;     //  QueryBasicInfo
    NtfsFastIoDispatch.FastIoQueryStandardInfo = NtfsFastQueryStdInfo;       //  QueryStandardInfo
    NtfsFastIoDispatch.FastIoLock =              NtfsFastLock;               //  Lock
    NtfsFastIoDispatch.FastIoUnlockSingle =      NtfsFastUnlockSingle;       //  UnlockSingle
    NtfsFastIoDispatch.FastIoUnlockAll =         NtfsFastUnlockAll;          //  UnlockAll
    NtfsFastIoDispatch.FastIoUnlockAllByKey =    NtfsFastUnlockAllByKey;     //  UnlockAllByKey
    NtfsFastIoDispatch.FastIoDeviceControl =     NULL;                       //  IoDeviceControl

    //
    //  Initialize the global ntfs data structure
    //

    NtfsInitializeNtfsData( DriverObject );

    ExInitializeFastMutex( &StreamFileCreationFastMutex );

    //
    //
    //  Register the file system with the I/O system
    //

    IoRegisterFileSystem(DeviceObject);

    //
    //  Initialize logging.
    //

    NtfsInitializeLogging();

    //
    //  Initialize global variables.  (ntfsdata.c assumes 2-digit value for
    //  $FILE_NAME)
    //

    ASSERT(($FILE_NAME >= 0x10) && ($FILE_NAME < 0x100));

    RtlInitUnicodeString( &NtfsFileNameIndex, NtfsFileNameIndexName );

    //
    //  Read the registry to determine if we are to create short names.
    //

    ValueName.Buffer = COMPATIBILITY_MODE_VALUE_NAME;
    ValueName.Length = sizeof( COMPATIBILITY_MODE_VALUE_NAME ) - sizeof( WCHAR );
    ValueName.MaximumLength = sizeof( COMPATIBILITY_MODE_VALUE_NAME );

    Status = NtfsGet8dot3NameStatus( &ValueName, &Value );

    //
    //  If we didn't find the value or the value is zero then create the 8.3
    //  names.
    //

    if (!NT_SUCCESS( Status ) || Value == 0) {

        SetFlag( NtfsData.Flags, NTFS_FLAGS_CREATE_8DOT3_NAMES );
    }

    //
    //  Setup the CheckPointAllVolumes callback item, timer, dpc, and
    //  spinlock.
    //

    ExInitializeWorkItem( &NtfsData.VolumeCheckpointItem,
                          NtfsCheckpointAllVolumes,
                          (PVOID)NULL );

    KeInitializeTimer( &NtfsData.VolumeCheckpointTimer );

    KeInitializeDpc( &NtfsData.VolumeCheckpointDpc,
                     NtfsVolumeCheckpointDpc,
                     NULL );

    KeInitializeSpinLock( &NtfsData.VolumeCheckpointSpinLock );

    //
    //  And return to our caller
    //

    return( STATUS_SUCCESS );
}


//
//  Local Support routine
//

NTSTATUS
NtfsGet8dot3NameStatus (
    IN PUNICODE_STRING ValueName,
    IN OUT PULONG Value
    )

/*++

Routine Description:

    Given a unicode value name this routine will go into the registry
    location for the 8dot3 name generation information and get the
    value.

Arguments:

    ValueName - the unicode name for the registry value located in the
                double space configuration location of the registry.
    Value   - a pointer to the ULONG for the result.

Return Value:

    NTSTATUS

    If STATUS_SUCCESSFUL is returned, the location *Value will be
    updated with the DWORD value from the registry.  If any failing
    status is returned, this value is untouched.

--*/

{
    HANDLE Handle;
    NTSTATUS Status;
    ULONG RequestLength;
    ULONG ResultLength;
    UCHAR Buffer[KEY_WORK_AREA];
    UNICODE_STRING KeyName;
    OBJECT_ATTRIBUTES ObjectAttributes;
    PKEY_VALUE_FULL_INFORMATION KeyValueInformation;

    KeyName.Buffer = COMPATIBILITY_MODE_KEY_NAME;
    KeyName.Length = sizeof( COMPATIBILITY_MODE_KEY_NAME ) - sizeof( WCHAR );
    KeyName.MaximumLength = sizeof( COMPATIBILITY_MODE_KEY_NAME );

    InitializeObjectAttributes( &ObjectAttributes,
                                &KeyName,
                                OBJ_CASE_INSENSITIVE,
                                NULL,
                                NULL);

    Status = ZwOpenKey( &Handle,
                        KEY_READ,
                        &ObjectAttributes);

    if (!NT_SUCCESS( Status )) {

        return Status;
    }

    RequestLength = KEY_WORK_AREA;

    KeyValueInformation = (PKEY_VALUE_FULL_INFORMATION)Buffer;

    while (TRUE) {

        Status = ZwQueryValueKey( Handle,
                                  ValueName,
                                  KeyValueFullInformation,
                                  KeyValueInformation,
                                  RequestLength,
                                  &ResultLength);

        ASSERT( Status != STATUS_BUFFER_OVERFLOW );

        if (Status == STATUS_BUFFER_OVERFLOW) {

            //
            // Try to get a buffer big enough.
            //

            if (KeyValueInformation != (PKEY_VALUE_FULL_INFORMATION)Buffer) {

                ExFreePool( KeyValueInformation );
            }

            RequestLength += 256;

            KeyValueInformation = (PKEY_VALUE_FULL_INFORMATION)
                                  ExAllocatePoolWithTag( PagedPool,
                                                         RequestLength,
                                                         'sftN');

            if (!KeyValueInformation) {
                return STATUS_NO_MEMORY;
            }

        } else {

            break;
        }
    }

    ZwClose(Handle);

    if (NT_SUCCESS(Status)) {

        if (KeyValueInformation->DataLength != 0) {

            PULONG DataPtr;

            //
            // Return contents to the caller.
            //

            DataPtr = (PULONG)
              ((PUCHAR)KeyValueInformation + KeyValueInformation->DataOffset);
            *Value = *DataPtr;

        } else {

            //
            // Treat as if no value was found
            //

            Status = STATUS_OBJECT_NAME_NOT_FOUND;
        }
    }

    if (KeyValueInformation != (PKEY_VALUE_FULL_INFORMATION)Buffer) {

        ExFreePool(KeyValueInformation);
    }

    return Status;
}
