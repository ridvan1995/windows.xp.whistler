/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 Copyright (c) 2000 Microsoft Corporation

 Module Name:
    
    paddict.cxx

 Abstract:

    Implements a dictionary for handling padding expressions for unknown
    represent as data types.

    Implements a dictionary for handling sizing macro for unknown
    represent as data types.

    Implements a dictionary for registering all type names for both
    transmit_as and represent_as.

 Notes:


 History:

     Jan 25, 1994        RyszardK        Created

 ----------------------------------------------------------------------------*/

#include "becls.hxx"
#pragma hdrstop

#include "typecls.hxx"


void 
RepAsPadExprDict::Register(
    unsigned long   Offset,
    node_skl *      pStructType,
    char *          pFieldName,
    node_skl *      pPrevFieldType
    )
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Routine Description:

    Adds a padding expression description to the dictionary

Arguments:

    Offset          - offset of the padding field
    pPaddingExpr    - text of the expression to be printed out
    
----------------------------------------------------------------------------*/
{
    Dict_Status    Status;

    REP_AS_PAD_EXPR_DESC * pOldEntry;
    REP_AS_PAD_EXPR_DESC * pEntry = new REP_AS_PAD_EXPR_DESC;

    pEntry->KeyOffset = Offset;
    pEntry->pStructType = pStructType;
    pEntry->pFieldName =  pFieldName;
    pEntry->pPrevFieldType =  pPrevFieldType;

    Status    = Dict_Find( pEntry );

    switch( Status )
        {
        case EMPTY_DICTIONARY:
        case ITEM_NOT_FOUND:

            Dict_Insert( pEntry );
            EntryCount++;
            break;

        default:
            // The only reason for an entry (offset) to be used already
            // would be that the otimization has shrunk the format string.
            // This means that the old entry should be deleted.

            pOldEntry = (REP_AS_PAD_EXPR_DESC *)Dict_Curr_Item();

            Dict_Delete( (pUserType *) &pOldEntry );
            Dict_Insert( pEntry );
            break;
        }
    return;
}


int
RepAsPadExprDict::Compare(
    pUserType   pE1,
    pUserType   pE2
    )
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

 Routine Description:

     Compare pad expr descriptors.
     KeyOffset is the key that orders the entries.

 Arguments:

 Return Value:
    
----------------------------------------------------------------------------*/
{
    if( ((REP_AS_PAD_EXPR_DESC *)pE1)->KeyOffset <
                                ((REP_AS_PAD_EXPR_DESC *)pE2)->KeyOffset )
        return( -1 );
    else
    if( ((REP_AS_PAD_EXPR_DESC *)pE1)->KeyOffset >
                                ((REP_AS_PAD_EXPR_DESC *)pE2)->KeyOffset )
        return( -1 );
    else
        return( 0 );
}

REP_AS_PAD_EXPR_DESC *
RepAsPadExprDict::GetFirst()
{
    Dict_Status             Status;
    REP_AS_PAD_EXPR_DESC *  pFirst = 0;

    Status = Dict_Next( 0 );

    if ( Status == SUCCESS )
        pFirst = (REP_AS_PAD_EXPR_DESC *)Dict_Curr_Item();

    return( pFirst );
}

REP_AS_PAD_EXPR_DESC *
RepAsPadExprDict::GetNext()
{
    Dict_Status             Status;
    REP_AS_PAD_EXPR_DESC    *pCurr, *pNext = 0;

    pCurr = (REP_AS_PAD_EXPR_DESC *)Dict_Curr_Item();

    if ( pCurr )
        {
        Status = Dict_Next( pCurr );
        if ( Status == SUCCESS )
            pNext = (REP_AS_PAD_EXPR_DESC *)Dict_Curr_Item();
        }

    return( pNext );
}

void
RepAsPadExprDict::WriteCurrentPadDesc(
    ISTREAM * pStream
    )
/*++

Routine description:

    Writes out the following string:

        (unsigned char)(NdrFieldPad(pSN,pFN,pPFN,pPFT))

Arguments:

    pStream -   stream to write to.

--*/
{
    REP_AS_PAD_EXPR_DESC    *pCurr = (REP_AS_PAD_EXPR_DESC *)Dict_Curr_Item();

    if ( pCurr  &&  pCurr->pPrevFieldType )
        {
        pStream->Write( "(unsigned char)("FC_FIELD_PAD_MACRO"(" );
        pCurr->pStructType->PrintType( PRT_TYPE_SPECIFIER, pStream );
        pStream->Write( "," );
        pStream->Write( pCurr->pFieldName );
        pStream->Write( "," );
        pStream->Write( pCurr->pPrevFieldType->GetSymName() );
        pStream->Write( "," );
        pCurr->pPrevFieldType->GetChild()->
                                PrintType( PRT_TYPE_SPECIFIER, pStream );
        pStream->Write( "))," );
        }
    else
        pStream->Write( "0," );
}


//========================================================================


void 
RepAsSizeDict::Register(
    unsigned long   Offset,
    char *          pTypeName
    )
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Routine Description:

    Adds a padding expression description to the dictionary

Arguments:

    Offset          - offset of the padding field
    pPaddingExpr    - text of the expression to be printed out
    
----------------------------------------------------------------------------*/
{
    Dict_Status    Status;

    REP_AS_SIZE_DESC * pOldEntry;
    REP_AS_SIZE_DESC * pEntry = new REP_AS_SIZE_DESC;

    pEntry->KeyOffset = Offset;
    pEntry->pTypeName =  pTypeName;

    Status    = Dict_Find( pEntry );

    switch( Status )
        {
        case EMPTY_DICTIONARY:
        case ITEM_NOT_FOUND:

            Dict_Insert( pEntry );
            EntryCount++;
            break;

        default:
            // The only reason for an entry (offset) to be used already
            // would be that the otimization has shrunk the format string.
            // This means that the old entry should be deleted.

            pOldEntry = (REP_AS_SIZE_DESC *)Dict_Curr_Item();

            Dict_Delete( (pUserType *) &pOldEntry );
            Dict_Insert( pEntry );
            break;
        }
    return;
}


int
RepAsSizeDict::Compare(
    pUserType   pE1,
    pUserType   pE2
    )
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

 Routine Description:

     Compare pad expr descriptors.
     KeyOffset is the key that orders the entries.

 Arguments:

 Return Value:
    
----------------------------------------------------------------------------*/
{
    if( ((REP_AS_SIZE_DESC *)pE1)->KeyOffset <
                                ((REP_AS_SIZE_DESC *)pE2)->KeyOffset )
        return( -1 );
    else
    if( ((REP_AS_SIZE_DESC *)pE1)->KeyOffset >
                                ((REP_AS_SIZE_DESC *)pE2)->KeyOffset )
        return( -1 );
    else
        return( 0 );
}

REP_AS_SIZE_DESC *
RepAsSizeDict::GetFirst()
{
    Dict_Status         Status;
    REP_AS_SIZE_DESC *  pFirst = 0;

    Status = Dict_Next( 0 );

    if ( Status == SUCCESS )
        pFirst = (REP_AS_SIZE_DESC *)Dict_Curr_Item();

    return( pFirst );
}

REP_AS_SIZE_DESC *
RepAsSizeDict::GetNext()
{
    Dict_Status         Status;
    REP_AS_SIZE_DESC    *pCurr, *pNext = 0;

    pCurr = (REP_AS_SIZE_DESC *)Dict_Curr_Item();

    if ( pCurr )
        {
        Status = Dict_Next( pCurr );
        if ( Status == SUCCESS )
            pNext = (REP_AS_SIZE_DESC *)Dict_Curr_Item();
        }

    return( pNext );
}

void
RepAsSizeDict::WriteCurrentSizeDesc(
    ISTREAM * pStream
    )
/*++

Routine description:

    Writes out the following string:

        NdrFcShort( sizeof(<type>) 

Arguments:

    pStream -   stream to write to.

--*/
{
    REP_AS_SIZE_DESC    *pCurr = (REP_AS_SIZE_DESC *)Dict_Curr_Item();

    if ( pCurr )
        {
        pStream->Write( "NdrFcShort( sizeof(" );
        pStream->Write( pCurr->pTypeName );
        pStream->Write( "))," );
        }
    else
        pStream->Write( "0," );
}


//========================================================================


BOOL 
QuintupleDict::Add(
    void *          pContext
    )
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Routine Description:

    Adds a padding expression description to the dictionary

Arguments:

Returns:

    TRUE    - when a new entry has been registered,
    FALSE   - otherwise

    Index field gets set to appropriate index.

    
----------------------------------------------------------------------------*/
{
    Dict_Status         Status;
    XMIT_AS_CONTEXT  *  pEntry = (XMIT_AS_CONTEXT *) pContext;

    Status = Dict_Find( pEntry );

    if( Status == EMPTY_DICTIONARY  ||  Status == ITEM_NOT_FOUND )
        {
        pEntry->Index = CurrentIndex;
        Dict_Insert( pEntry );
        CurrentIndex++;
        return TRUE;
        }
    else
        {
        pEntry->Index = ((XMIT_AS_CONTEXT *)Dict_Curr_Item())->Index;
        return FALSE;
        }
}


int
QuintupleDict::Compare(
    pUserType   pE1,
    pUserType   pE2
    )
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

 Routine Description:

     Compare two quintuple types.
     Both fXmit and the name have to match.

     fXmit allows for comparing xmit_as with xmit_as and rep_as with rep_as.
     For xmit_as we compare the presented types.
     For rep_as we compare the wire types (not the local types).

 Arguments:

 Return Value:
    
----------------------------------------------------------------------------*/
{
    if( ((XMIT_AS_CONTEXT *)pE1)->fXmit == ((XMIT_AS_CONTEXT *)pE2)->fXmit )
        if ( ((XMIT_AS_CONTEXT *)pE1)->fXmit )
            return( strcmp( ((XMIT_AS_CONTEXT *)pE1)->pTypeName,
                            ((XMIT_AS_CONTEXT *)pE2)->pTypeName ) );
        else
            return( strcmp(
                ((CG_REPRESENT_AS *)((XMIT_AS_CONTEXT *)pE1)->pXmitNode)->
                            GetTransmittedType()->GetSymName(),
                ((CG_REPRESENT_AS *)((XMIT_AS_CONTEXT *)pE2)->pXmitNode)->
                            GetTransmittedType()->GetSymName()
                           ) );
    else
    if( ((XMIT_AS_CONTEXT *)pE1)->fXmit )
        return( -1 );
    else
        return( 1 );
}

void *
QuintupleDict::GetFirst()
{
    Dict_Status             Status;
    XMIT_AS_CONTEXT *  pFirst = 0;

    Status = Dict_Next( 0 );

    if ( Status == SUCCESS )
        pFirst = (XMIT_AS_CONTEXT *)Dict_Curr_Item();

    return( pFirst );
}

void *
QuintupleDict::GetNext()
{
    Dict_Status             Status;
    XMIT_AS_CONTEXT    *pCurr, *pNext = 0;

    pCurr = (XMIT_AS_CONTEXT *)Dict_Curr_Item();

    if ( pCurr )
        {
        Status = Dict_Next( pCurr );
        if ( Status == SUCCESS )
            pNext = (XMIT_AS_CONTEXT *)Dict_Curr_Item();
        }

    return( pNext );
}


