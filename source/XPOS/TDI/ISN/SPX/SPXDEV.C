/*++

Copyright (c) 2000-1993  Microsoft Corporation

Module Name:

    spxdev.c

Abstract:

    This module contains code which implements the DEVICE_CONTEXT object.
    Routines are provided to reference, and dereference transport device
    context objects.

    The transport device context object is a structure which contains a
    system-defined DEVICE_OBJECT followed by information which is maintained
    by the transport provider, called the context.

Author:

    Nikhil Kamkolkar (nikhilk) 11-November-1993

Environment:

    Kernel mode

Revision History:

--*/

#include "precomp.h"
#pragma hdrstop

//	Define module number for event logging entries
#define	FILENUM		SPXDEV

#ifdef ALLOC_PRAGMA
#pragma alloc_text(INIT, SpxInitCreateDevice)
#pragma alloc_text(PAGE, SpxDestroyDevice)
#endif




VOID
SpxDerefDevice(
    IN PDEVICE Device
    )

/*++

Routine Description:

    This routine dereferences a device context by decrementing the
    reference count contained in the structure.  Currently, we don't
    do anything special when the reference count drops to zero, but
    we could dynamically unload stuff then.

Arguments:

    Device - Pointer to a transport device context object.

Return Value:

    none.

--*/

{
    INTERLOCKED_RESULT result;

    result = ExInterlockedDecrementLong (
                 &Device->dev_RefCount,
                 &Device->dev_Interlock);

    CTEAssert (result != ResultNegative);

    if (result == ResultZero)
	{
		//	Close binding to IPX
		SpxUnbindFromIpx();
	
		//	Set unload event.
		KeSetEvent(&SpxUnloadEvent, IO_NETWORK_INCREMENT, FALSE);
    }

} // SpxDerefDevice




NTSTATUS
SpxInitCreateDevice(
    IN PDRIVER_OBJECT 	DriverObject,
    IN PUNICODE_STRING 	DeviceName,
    IN OUT PDEVICE *	DevicePtr
    )

/*++

Routine Description:

    This routine creates and initializes a device context structure.

Arguments:


    DriverObject - pointer to the IO subsystem supplied driver object.

    Device - Pointer to a pointer to a transport device context object.

    DeviceName - pointer to the name of the device this device object points to.

Return Value:

    STATUS_SUCCESS if all is well; STATUS_INSUFFICIENT_RESOURCES otherwise.

--*/

{
    NTSTATUS        status;
    PDEVICE_OBJECT  deviceObject;
    PDEVICE         Device;
    ULONG           DeviceSize;
    ULONG           DeviceNameOffset;


    DBGPRINT(DEVICE, INFO,
			("SpxInitCreateDevice - Create device %ws\n", DeviceName->Buffer));

    // Create the device object for the sample transport, allowing
    // room at the end for the device name to be stored (for use
    // in logging errors).
    DeviceSize = sizeof(DEVICE) - sizeof(DEVICE_OBJECT) +
                 DeviceName->Length + sizeof(UNICODE_NULL);

    status = IoCreateDevice(
                 DriverObject,
                 DeviceSize,
                 DeviceName,
                 FILE_DEVICE_TRANSPORT,
                 0,
                 FALSE,
                 &deviceObject);

    if (!NT_SUCCESS(status)) {
        DBGPRINT(DEVICE, ERR, ("IoCreateDevice failed\n"));
        return status;
    }

    deviceObject->Flags |= DO_DIRECT_IO;
    Device 				 = (PDEVICE)deviceObject;

    DBGPRINT(DEVICE, INFO, ("IoCreateDevice succeeded %lx\n", Device));

    // Initialize our part of the device context.
    RtlZeroMemory(
        ((PUCHAR)Device) + sizeof(DEVICE_OBJECT),
        sizeof(DEVICE) - sizeof(DEVICE_OBJECT));

    DeviceNameOffset = sizeof(DEVICE);

    // Copy over the device name.
    Device->dev_DeviceNameLen   = DeviceName->Length + sizeof(WCHAR);
    Device->dev_DeviceName      = (PWCHAR)(((PUCHAR)Device) + DeviceNameOffset);

    RtlCopyMemory(
        Device->dev_DeviceName,
        DeviceName->Buffer,
        DeviceName->Length);

    Device->dev_DeviceName[DeviceName->Length/sizeof(WCHAR)] = UNICODE_NULL;

    // Initialize the reference count.
    Device->dev_RefCount = 1;

#if DBG
    Device->dev_RefTypes[DREF_CREATE] = 1;
#endif

#if DBG
    RtlCopyMemory(Device->dev_Signature1, "IDC1", 4);
    RtlCopyMemory(Device->dev_Signature2, "IDC2", 4);
#endif

	//	Set next conn id to be used.
	Device->dev_NextConnId							= (USHORT)SpxRandomNumber();
	if (Device->dev_NextConnId == 0xFFFF)
	{
		Device->dev_NextConnId	= 1;
	}

	DBGPRINT(DEVICE, ERR,
			("SpxInitCreateDevice: Start Conn Id %lx\n", Device->dev_NextConnId));

    // Initialize the resource that guards address ACLs.
    ExInitializeResource (&Device->dev_AddrResource);

    // initialize the various fields in the device context
    CTEInitLock (&Device->dev_Interlock);
    CTEInitLock (&Device->dev_Lock);
    KeInitializeSpinLock (&Device->dev_StatInterlock);
    KeInitializeSpinLock (&Device->dev_StatSpinLock);

    Device->dev_State       = DEVICE_STATE_CLOSED;
    Device->dev_Type        = SPX_DEVICE_SIGNATURE;
    Device->dev_Size        = sizeof (DEVICE);

    Device->dev_Stat.Version = 0x100;

    *DevicePtr = Device;
    return STATUS_SUCCESS;

}   // SpxCreateDevice




VOID
SpxDestroyDevice(
    IN PDEVICE Device
    )

/*++

Routine Description:

    This routine destroys a device context structure.

Arguments:

    Device - Pointer to a pointer to a transport device context object.

Return Value:

    None.

--*/

{
    ExDeleteResource (&Device->dev_AddrResource);
    IoDeleteDevice ((PDEVICE_OBJECT)Device);

}   // SpxDestroyDevice
