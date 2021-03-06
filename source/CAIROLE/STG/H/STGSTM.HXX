//+---------------------------------------------------------------------------
//
//  Microsoft Windows
//  Copyright (C) Microsoft Corporation, 2000 - 1993.
//
//  File:	stgstm.hxx
//
//  Contents:	Common header file for debug and global definitions
//
//  History:	29-Jun-93	DrewB	Created
//
//----------------------------------------------------------------------------

#ifndef __STGSTM_HXX__
#define __STGSTM_HXX__

#define CONTENTS_STREAM L"CONTENTS"

typedef LPSECURITY_ATTRIBUTES LPSTGSECURITY;

#define aMsg(s) ((char *)(s) != NULL)
#define ssResult(sc) ((HRESULT)(sc))

#if !defined(ErrJmp)
#if DBG == 1


#define ErrJmp(comp, label, errval, var) \
{\
    comp##DebugOut((DEB_IERROR, "Error %lX at %s:%d\n",\
		    (unsigned long)(errval), __FILE__, __LINE__));\
    var = errval;\
    goto label;\
}

#else

#define ErrJmp(comp, label, errval, var) \
{\
    var = errval;\
    goto label;\
}

#endif
#endif

#define ssErr(l, e) ErrJmp(ss, l, e, sc)
#define ssChkTo(l, e) if (FAILED(sc = (e))) ssErr(l, sc) else 1
#define ssHChkTo(l, e) if (FAILED(sc = GetScode(e))) ssErr(l, sc) else 1
#define ssChk(e) ssChkTo(EH_Err, e)
#define ssHChk(e) ssHChkTo(EH_Err, e)
#define ssMemTo(l, e) \
    if ((e) == NULL) ssErr(l, STG_E_INSUFFICIENTMEMORY) else 1
#define ssMem(e) ssMemTo(EH_Err, e)

#if DBG == 1
DECLARE_DEBUG(ss);
#endif

#if DBG == 1

#define ssDebugOut(args) ssInlineDebugOut args

#define ssAssert(e) Win4Assert(e)
#define ssAssSucc(e) Win4Assert(SUCCEEDED(e))
#define ssHAssSucc(e) Win4Assert(SUCCEEDED(e))

#define ssVerify(e) Win4Assert(e)
#define ssVerSucc(e) Win4Assert(SUCCEEDED(e))
#define ssHVerSucc(e) Win4Assert(SUCCEEDED(e))

#else

#define ssDebugOut(args)

#define ssAssert(e)
#define ssAssSucc(e)
#define ssHAssSucc(e)

#define ssVerify(e) (e)
#define ssVerSucc(e) (e)
#define ssHVerSucc(e) (e)

#endif

// Generic flags for various routines
enum CREATEOPEN
{
    CO_CREATE,
    CO_OPEN
};

enum FILEDIR
{
    FD_DEFAULT,             //  Indicates openning whatever is stored

    FD_FILE,
    FD_DIR,
    FD_STORAGE,
    FD_STREAM,
    FD_EMBEDDING,
};

// BUGBUG - Remove when available
STDAPI CoMemAlloc(DWORD cb, void **ppv);
STDAPI CoMemFree(void *pv);

#endif // #ifndef __STGSTM_HXX__
