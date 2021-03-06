/*++

Copyright (c) 2000  Microsoft Corporation

Module Name:

    Startexec.c

Abstract:

    This module contains routines for switching to and from application
    mode in a vdm

Author:

    Dave Hastings (daveh) 24-Apr-2000

Notes:

    This code started out in ke\i386\vdm.c

Revision History:

    23-Sep-2000 sudeepb Formed W_VDMEndExecution from VDMEndExecution
			for performance.

    18-Dec-2000 sudeepb Tuned all the routines for performance

    12-Oct-1993 Jonle , removed unneeded endexecution worker functions

--*/
#include "vdmp.h"

#ifdef ALLOC_PRAGMA
#pragma alloc_text(PAGE, VdmpStartExecution)
#pragma alloc_text(PAGE, VdmEndExecution)
#endif

NTSTATUS
VdmpStartExecution(
    VOID
    )
/*++

Routine Description:

    This routine causes execution of dos application code to begin.  The
    Dos application executes on the thread.  The Vdms context is loaded
    from the VDM_TIB for the thread.  The 32 bit context is stored into
    the MonitorContext.  Execution in the VDM context will continue until
    an event occurs that the monitor needs to service.  At that point,
    the information will be put into the VDM_TIB, and the call will
    return.

Arguments:

Return Value:

    TrapFrame->Eax for application mode, required for system sevices
    exit.


--*/
{
    PVDM_TIB VdmTib;
    PKTRAP_FRAME TrapFrame;
    PETHREAD Thread;
    KIRQL    OldIrql;
    BOOLEAN  IntsEnabled;

    PAGED_CODE();


    KeRaiseIrql(APC_LEVEL, &OldIrql);

    //
    // Form a pointer to the trap frame for the current thread
    //
    Thread    = PsGetCurrentThread();
    TrapFrame = VdmGetTrapFrame(&Thread->Tcb);


    // Sudeepb 01-Dec-2000 - There was a try except here which i have
    // taken out, because we come here from SystemServe Entry which
    // already has a top level try except, so no one can blow NT
    // even if we take a fault here becuase user memory was'nt allocated..

    //
    // Get the VdmTib,
    //
    VdmTib = NtCurrentTeb()->Vdm;


    //
    // Determine if interrupts are on or off
    //
    IntsEnabled = VdmTib->VdmContext.EFlags & EFLAGS_INTERRUPT_MASK
                   ? TRUE : FALSE;

    //
    // check for timer ints to dispatch, However if interrupts are disabled
    // or there are hardware ints pending we postpone dispatching the timer
    // interrupt until interrupts are enabled.
    //
    if (*pNtVDMState & VDM_INT_TIMER &&
        IntsEnabled && !(*pNtVDMState & VDM_INT_HARDWARE))
    {
        VdmTib->EventInfo.Event = VdmIntAck;
        VdmTib->EventInfo.InstructionSize = 0;
        VdmTib->EventInfo.IntAckInfo = 0;
        KeLowerIrql(OldIrql);
        return STATUS_SUCCESS;
    }

    //
    // Perform IF to VIF translation
    //

    //
    // If the processor supports IF virtualization
    //
    if (((KeI386VirtualIntExtensions & V86_VIRTUAL_INT_EXTENSIONS) &&
        (VdmTib->VdmContext.EFlags & EFLAGS_V86_MASK)) ||
        ((KeI386VirtualIntExtensions & PM_VIRTUAL_INT_EXTENSIONS) &&
        !(VdmTib->VdmContext.EFlags & EFLAGS_V86_MASK)))
    {
        //
        // Translate IF to VIF
        //

        if (IntsEnabled) {
            VdmTib->VdmContext.EFlags |= EFLAGS_VIF;

        } else {
            VdmTib->VdmContext.EFlags &= ~EFLAGS_VIF;
            VdmTib->VdmContext.EFlags |= EFLAGS_INTERRUPT_MASK;
        }

        if (*pNtVDMState & VDM_INT_HARDWARE)
            VdmTib->VdmContext.EFlags |= EFLAGS_VIP;
        else
            VdmTib->VdmContext.EFlags &= ~EFLAGS_VIP;


    //
    // Else if we are not running in v86 mode, or not using IOPL in
    // v86 mode
    //
    } else if (!(KeI386VdmIoplAllowed) ||
        !(VdmTib->VdmContext.EFlags & EFLAGS_V86_MASK))
    {
        //
        // Translate the real interrupt flag in the VdmContext to the virtual
        // interrupt flag in the VdmTib, and force real interrupts enabled.
        //

        ASSERT(VDM_VIRTUAL_INTERRUPTS == EFLAGS_INTERRUPT_MASK);

        if (VdmTib->VdmContext.EFlags & EFLAGS_INTERRUPT_MASK) {
            _asm {
                 mov  eax,FIXED_NTVDMSTATE_LINEAR     ; get pointer to VDM State
                 lock or dword ptr [eax], dword ptr VDM_VIRTUAL_INTERRUPTS
            }
        } else {
            _asm {
                 mov  eax,FIXED_NTVDMSTATE_LINEAR   ; get pointer to VDM State
                 lock and dword ptr [eax], NOT VDM_VIRTUAL_INTERRUPTS
            }
        }

        //
        // Insure that real interrupts are always enabled.
        //
        VdmTib->VdmContext.EFlags |= EFLAGS_INTERRUPT_MASK;
    }

    //
    // Switch from MonitorContext to VdmContext
    //
    VdmSwapContexts(
        TrapFrame,
        &(VdmTib->MonitorContext),
        &(VdmTib->VdmContext)
        );


    //
    // Check for pending interrupts
    //
    if (IntsEnabled && (*pNtVDMState & VDM_INT_HARDWARE)) {
        VdmDispatchInterrupts(TrapFrame, VdmTib);
    }

    KeLowerIrql(OldIrql);

    return (NTSTATUS) TrapFrame->Eax;
}

VOID
VdmEndExecution(
    PKTRAP_FRAME TrapFrame,
    PVDM_TIB VdmTib
    )
/*++

Routine Description:

    This routine does the core work to end the execution

Arguments:

    None

Return Value:

--*/
{
    PAGED_CODE();

    ASSERT((TrapFrame->EFlags & EFLAGS_V86_MASK) ||
           (TrapFrame->SegCs != (KGDT_R3_CODE | RPL_MASK)) );


    // Sudeepb 01-Dec-2000 - There was a try except here which i have
    // taken out, because we come here with a top level exception frame,
    // so no one can blow NT even if we take a fault here becuase user
    // memory was'nt allocated.


    // The return value must be put into the Monitorcontext, and set,
    // since we are probably returning to user mode via EXIT_ALL, and
    // the volatile registers will be restored.
    VdmTib->MonitorContext.Eax = STATUS_SUCCESS;

    //
    // Switch from MonitorContext to VdmContext
    //
    VdmSwapContexts(
        TrapFrame,
        &(VdmTib->VdmContext),
        &(VdmTib->MonitorContext)
        );

    //
    // Perform IF to VIF translation
    //

    //
    // If the processor supports IF virtualization
    //
    if (((KeI386VirtualIntExtensions & V86_VIRTUAL_INT_EXTENSIONS) &&
        (VdmTib->VdmContext.EFlags & EFLAGS_V86_MASK)) ||
        ((KeI386VirtualIntExtensions & PM_VIRTUAL_INT_EXTENSIONS) &&
        !(VdmTib->VdmContext.EFlags & EFLAGS_V86_MASK)))
    {
        //
        // Translate VIF to IF
        //
        if (VdmTib->VdmContext.EFlags & EFLAGS_VIF) {
            VdmTib->VdmContext.EFlags |= EFLAGS_INTERRUPT_MASK;
        } else {
            VdmTib->VdmContext.EFlags &= ~EFLAGS_INTERRUPT_MASK;
        }

        //
        // Turn off VIP and VIF to insure that nothing strange happens
        //
        TrapFrame->EFlags &= ~(EFLAGS_VIP | EFLAGS_VIF);
    //
    // Else if we are not running in v86 mode, or not using IOPL in
    // v86 mode
    //
    } else if (!(KeI386VdmIoplAllowed) ||
        !(VdmTib->VdmContext.EFlags & EFLAGS_V86_MASK))
    {
        //
        // Translate the virtual interrupt flag from the VdmTib back to the
        // real interrupt flag in the VdmContext
        //

        VdmTib->VdmContext.EFlags =
	    (VdmTib->VdmContext.EFlags & ~EFLAGS_INTERRUPT_MASK)
	        | (*(PULONG)pNtVDMState & VDM_VIRTUAL_INTERRUPTS);
    }

    return;
}
