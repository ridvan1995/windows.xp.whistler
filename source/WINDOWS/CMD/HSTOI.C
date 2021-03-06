#include "cmd.h"
#include "cmdproto.h"

/***	hstoi - convert a hex string to an integer
 *
 *  Conversion stops when the first non-hex character is found.  If the first
 *  character is not a hex character, 0 is returned.
 *
 *  Eric K. Evans, Microsoft
 */

hstoi(s)
TCHAR *s ;
{
	int result = 0 ;
	int digit ;

	for( ; s && _istspace(*s) ; s++)
	;

	for ( ; s && _istxdigit(*s) ; s++) {
	digit = (int) (*s <= TEXT('9')) ? (int)*s - (int)'0' : (int)_totlower(*s)-(int)'W' ;
	result = (result << 4)+digit ;
	} ;

	return(result) ;
}
