/*++

Copyright (c) 1993  Microsoft Corporation

Module Name:

    kiinit.c

Abstract:

    This module implements architecture independent kernel initialization.

Author:

    David N. Cutler 11-May-1993

Environment:

    Kernel mode only.

Revision History:

--*/

#include "ki.h"

//
// Put all code for kernel initialization in the INIT section. It will be
// deallocated by memory management when phase 1 initialization is completed.
//

#if defined(ALLOC_PRAGMA)

#pragma alloc_text(INIT, KiInitSystem)
#pragma alloc_text(INIT, KiComputeReciprocal)

#endif

VOID
KiInitSystem (
    VOID
    )

/*++

Routine Description:

    This function initializes architecture independent kernel structures.

Arguments:

    None.

Return Value:

    None.

--*/

{

    ULONG Index;

    //
    // Initialize active matrix.
    //

    for (Index = 0; Index < MAXIMUM_PRIORITY; Index += 1) {
        KiActiveMatrix[Index] = 0;
    }

    //
    // Initialize dispatcher ready queue listheads.
    //

    for (Index = 0; Index < MAXIMUM_PRIORITY; Index += 1) {
        InitializeListHead(&KiDispatcherReadyListHead[Index]);
    }

    //
    // Initialize bug check callback listhead and spinlock.
    //

    InitializeListHead(&KeBugCheckCallbackListHead);
    KeInitializeSpinLock(&KeBugCheckCallbackLock);

    //
    // Initialize the timer expiration DPC object.
    //

    KeInitializeDpc(&KiTimerExpireDpc,
                    (PKDEFERRED_ROUTINE)KiTimerExpiration, NIL);

    //
    // Initialize the profile listhead.
    //

    InitializeListHead(&KiProfileListHead);

    //
    // Initialize the timer table, the timer completion listhead, and the
    // timer completion DPC.
    //

    for (Index = 0; Index < TIMER_TABLE_SIZE; Index += 1) {
        InitializeListHead(&KiTimerTableListHead[Index]);
    }

    //
    // Initialize the swap event, the process inswap listhead, the
    // process outswap listhead, the kernel stack inswap listhead,
    // the wait in listhead, and the wait out listhead.
    //

    KeInitializeEvent(&KiSwapEvent,
                      SynchronizationEvent,
                      FALSE);

    InitializeListHead(&KiProcessInSwapListHead);
    InitializeListHead(&KiProcessOutSwapListHead);
    InitializeListHead(&KiStackInSwapListHead);
    InitializeListHead(&KiWaitInListHead);
    InitializeListHead(&KiWaitOutListHead);

    //
    // Initialize call performance data structures.
    //

#if defined(_COLLECT_FLUSH_SINGLE_CALLDATA_)

    ExInitializeCallData(&KiFlushSingleCallData);

#endif

#if defined(_COLLECT_SET_EVENT_CALLDATA_)

    ExInitializeCallData(&KiSetEventCallData);

#endif

#if defined(_COLLECT_WAIT_SINGLE_CALLDATA_)

    ExInitializeCallData(&KiWaitSingleCallData);

#endif

    return;
}

LARGE_INTEGER
KiComputeReciprocal (
    IN LONG Divisor,
    OUT PCCHAR Shift
    )

/*++

Routine Description:

    This function computes the large integer reciprocal of the specified
    value.

Arguments:

    Divisor - Supplies the value for which the large integer reciprocal is
        computed.

    Shift - Supplies a pointer to a variable that receives the computed
        shift count.

Return Value:

    The large integer reciprocal is returned as the fucntion value.

--*/

{

    LARGE_INTEGER Fraction;
    LONG NumberBits;
    LONG Remainder;

    //
    // Compute the large integer reciprocal of the specified value.
    //

    NumberBits = 0;
    Remainder = 1;
    Fraction.LowPart = 0;
    Fraction.HighPart = 0;
    while (Fraction.HighPart >= 0) {
        NumberBits += 1;
        Fraction.HighPart = (Fraction.HighPart << 1) | (Fraction.LowPart >> 31);
        Fraction.LowPart <<= 1;
        Remainder <<= 1;
        if (Remainder >= Divisor) {
            Remainder -= Divisor;
            Fraction.LowPart |= 1;
        }
    }

    if (Remainder != 0) {
        if ((Fraction.LowPart == 0xffffffff) && (Fraction.HighPart == 0xffffffff)) {
            Fraction.LowPart = 0;
            Fraction.HighPart = 0x80000000;
            NumberBits -= 1;

        } else {
            if (Fraction.LowPart == 0xffffffff) {
                Fraction.LowPart = 0;
                Fraction.HighPart += 1;

            } else {
                Fraction.LowPart += 1;
            }
        }
    }

    //
    // Compute the shift count value and return the reciprocal fraction.
    //

    *Shift = NumberBits - 64;
    return Fraction;
}
