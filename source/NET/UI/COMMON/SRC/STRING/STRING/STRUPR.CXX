/**********************************************************************/
/**			  Microsoft Windows/NT			     **/
/**		   Copyright(c) Microsoft Corp., 2000		     **/
/**********************************************************************/

/*
    strupr.cxx
    NLS/DBCS-aware string class: strupr method

    This file contains the implementation of the strupr method
    for the STRING class.  It is separate so that clients of STRING which
    do not use this operator need not link to it.

    FILE HISTORY:
	beng	    18-Jan-2000 Separated from original monolithic .cxx
	beng	    07-Feb-2000 Uses lmui.hxx
*/

#include "pchstr.hxx"  // Precompiled header


/*******************************************************************

    NAME:	NLS_STR::strupr

    SYNOPSIS:	Convert *this lower case letters to upper case

    ENTRY:	String is of indeterminate case

    EXIT:	String is all uppercase

    NOTES:

    HISTORY:
	johnl	    26-Nov-1990     Written
	beng	    23-Jul-2000     Allow on erroneous string

********************************************************************/

NLS_STR& NLS_STR::strupr()
{
    if (!QueryError())
	::struprf( _pchData );

    return *this;
}
