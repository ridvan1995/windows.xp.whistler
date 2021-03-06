/*++

Copyright (c) 1993  Microsoft Corporation

Module Name:

    biavst.h

Abstract:

    This header file is used to cause the correct machine/platform specific
    data structures to be used when compiling for a non-hosted platform.

Author:

    Wesley Witt (wesw) 2-Aug-1993

Environment:

    User Mode

--*/

#include "heap.h"


#undef i386
#undef _X86_
#undef MIPS
#undef _MIPS_
#undef ALPHA
#undef _ALPHA_

#define _NTSYSTEM_

//-----------------------------------------------------------------------------------------
//
// mips r4000
//
//-----------------------------------------------------------------------------------------
#if defined(TARGET_MIPS)

#pragma message( "Compiling for target = mips" )

#define _MIPS_

#if defined(HOST_MIPS)
#define MIPS
#endif

#if defined(HOST_i386)
#define __unaligned
#endif

#include <nt.h>
#include <ntrtl.h>
#include <nturtl.h>
#include <ntdbg.h>
#include <xpos.h>
#include <iop.h>
#include <windows.h>

#if !defined(HOST_MIPS)
#undef MIPS
#undef _MIPS_
#endif

#if defined(HOST_i386)
#undef _cdecl
#undef UNALIGNED
#define UNALIGNED
#endif

//-----------------------------------------------------------------------------------------
//
// intel x86
//
//-----------------------------------------------------------------------------------------
#elif defined(TARGET_i386)

#pragma message( "Compiling for target = x86" )

#define _X86_

#if defined(HOST_MIPS)
#define MIPS
#endif

#include <nt.h>
#include <ntrtl.h>
#include <nturtl.h>
#include <ntdbg.h>
#include <xpos.h>
#include <iop.h>
#include <windows.h>

#if defined(HOST_MIPS)
#undef _cdecl
#define _cdecl
#endif

#if defined(HOST_ALPHA)
#undef _cdecl
#define _cdecl
#endif

#if !defined(HOST_i386)
#undef _X86_
#endif

//-----------------------------------------------------------------------------------------
//
// alpha axp
//
//-----------------------------------------------------------------------------------------
#elif defined(TARGET_ALPHA)

#pragma message( "Compiling for target = alpha" )

#define _ALPHA_ 1

#if defined(HOST_i386)
#define __unaligned
#endif

#if defined(HOST_MIPS)
#define MIPS
#endif

#include <nt.h>
#include <ntrtl.h>
#include <nturtl.h>
#include <ntdbg.h>
#include <xpos.h>
#include <iop.h>
#include <windows.h>

#if defined(HOST_MIPS)
#undef _cdecl
#define _cdecl
#endif

#if defined(HOST_i386)
#undef UNALIGNED
#define UNALIGNED
#endif

#if !defined(HOST_ALPHA)
#undef _ALPHA_
#endif


#else

//-----------------------------------------------------------------------------------------
//
// unknown platform
//
//-----------------------------------------------------------------------------------------
#error "Unsupported target CPU"

#endif
