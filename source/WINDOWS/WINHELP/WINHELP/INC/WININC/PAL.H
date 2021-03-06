/*****************************************************************************
*                                                                            *
*  PAL.H                                                                     *
*                                                                            *
*  Copyright (C) Microsoft Corporation 1990.                                 *
*  All Rights reserved.                                                      *
*                                                                            *
******************************************************************************
*                                                                            *
*  Module Intent                                                             *
*                                                                            *
*  Exports palette getting routine.                                          *
*                                                                            *
******************************************************************************
*                                                                            *
*  Testing Notes                                                             *
*                                                                            *
******************************************************************************
*                                                                            *
*  Current Owner: Dann
*                                                                            *
******************************************************************************
*                                                                            *
*  Released by Development:                                                  *
*                                                                            *
*****************************************************************************/

/*****************************************************************************
*
*  Revision History:  Created by KevynCT
*
*  11/02/90  RobertBu Removed all routines but HpalGetBestPalette() which
*                     was changed to take an HDE rather than a QDE.
*
*****************************************************************************/


/*****************************************************************************
*                                                                            *
*                               Prototypes                                   *
*                                                                            *
*****************************************************************************/


HPAL HpalGetBestPalette(HDE);
