/***
*clutil.hxx - Class Lib component-wide utility functions.
*
*  Copyright (C) 2000, Microsoft Corporation.  All Rights Reserved.
*  Information Contained Herein Is Proprietary and Confidential.
*
*Purpose:
*  typelib utility functions
*
*Revision History:
*   [00] 24-Jan-93 RajivK:	Created
*
*Implementation Notes:
*
*****************************************************************************/

#ifndef TLIBUTIL_HXX_INCLUDED
#define TLIBUTIL_HXX_INCLUDED

#include "typesx.h"

#if ID_DEBUG
#undef SZ_FILE_NAME
ASSERTNAME(g_szTLIBUTIL_HXX)
#define SZ_FILE_NAME g_szTLIBUTIL_HXX
#endif 

// This table is not used on MAC.  On Mac all characters greater than
// 128 is mapped to 128
// Define an array for mapping characters > 128 (for WIN)
//
extern BYTE g_rgbHashTable[];



#endif 
