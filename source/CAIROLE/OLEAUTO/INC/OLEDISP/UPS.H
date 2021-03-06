/*** 
*ups.h - The Universal Proxy/Stub class definitions
*
*  Copyright (C) 2000-94, Microsoft Corporation.  All Rights Reserved.
*  Information Contained Herein Is Proprietary and Confidential.
*
*Purpose:
*  This file describes the TypeInfo-driven, Universal Proxy Stub classes
*
*  CProxUniv  --  The Universal Proxy class
*  CStubUniv  --  The Universal Stub class
*
*Revision History:
*
* [00]	21-Jun-94 bradlo: Created.
*
*Implementation Notes:
*
*****************************************************************************/


// forward declarations
class FAR CProxUniv;
class FAR CStubUniv;


// CProxUniv - 'prox'
//
// The Universal proxy class
//
class FAR CProxUniv
{
    void *m_pvtbl;

public:

static HRESULT Create(IUnknown FAR* punkOuter,
		      REFIID riid,
		      IUnknown FAR* FAR* pprox);

    HRESULT ProxyMethod(int iMeth, va_list args);

    CProxUniv(IUnknown FAR* punkOuter);
    ~CProxUniv();

    class FAR CPriv : public IPROXY
    {
    public:
      STDMETHOD(QueryInterface)(REFIID riid, void FAR* FAR* ppv);
      STDMETHOD_(unsigned long, AddRef)(void);
      STDMETHOD_(unsigned long, Release)(void);

      STDMETHOD(Connect)(ICHANNEL FAR* plrpc);
      STDMETHOD_(void, Disconnect)(void);

      inline CProxUniv FAR* PProx();
    };
    friend CPriv;
    CPriv m_priv;

    HRESULT PSInit(void);

    unsigned long  m_cRefs;
    ICHANNEL FAR*  m_plrpc;
    IUnknown FAR*  m_punkOuter;
    SYSKIND        m_syskindStub;

    BOOL           m_fIsDual;   // Is this a dual interface?
    IID            m_iid;	// the IID for which this instance is a proxy
    int            m_cFuncs;    // count of functions on the custom interface
    ITypeInfo FAR* m_ptinfo;	// the typeinfo describing the interface for
				// which this instance is a proxy
};


// CStubUniv - 'stub'
//
// The Universal stub class.
//
class FAR CStubUniv : public ISTUB
{
public:

static HRESULT Create(IUnknown FAR* punkServer,
		      REFIID riid,
		      ISTUB FAR* FAR* ppstub);
	
    // IUnknown methods
    //
    STDMETHOD(QueryInterface)(REFIID riid, void FAR* FAR* ppv);
    STDMETHOD_(unsigned long, AddRef)(void);
    STDMETHOD_(unsigned long, Release)(void);
	
    // IRpcStub methods
    //
#if ((OE_WIN32 && defined (OA_UNICODE)) || defined (WOW))
    STDMETHOD(Connect)(IUnknown FAR* pUnk);
    STDMETHOD_(void, Disconnect)(void);
    STDMETHOD(Invoke)(RPCOLEMESSAGE FAR* pRpcMsg, 
		      IRpcChannelBuffer FAR* pRpcChannel);
    STDMETHOD_(IRpcStubBuffer*, IsIIDSupported)(REFIID riid);
    STDMETHOD_(ULONG, CountRefs)(void);
    STDMETHOD(DebugServerQueryInterface)(void FAR* FAR* ppv);
    STDMETHOD_(void, DebugServerRelease)(void FAR* pv);
#else 
    STDMETHOD(Connect)(IUnknown FAR* punkObject);
    STDMETHOD_(void, Disconnect)(void);              
    STDMETHOD(Invoke)(REFIID riid,
		      int imeth,
		      IStream FAR* pstm,
		      unsigned long dwDestCtx,
		      void FAR* pvDestCtx);
    STDMETHOD_(OLEBOOL, IsIIDSupported)(REFIID riid);
    STDMETHOD_(unsigned long, CountRefs)(void);
#endif 

    HRESULT PSInit(void);
    HRESULT DispatchMethod(int iMeth);

private:	
    CStubUniv();
    ~CStubUniv();

    unsigned long  m_cRefs;
    IUnknown FAR*  m_punk;
    IStream  FAR*  m_pstm;
    SYSKIND        m_syskindProxy;
    BOOL	   m_fIsDual;		// Is this a dual interface?
    IID            m_iid;		// the IID of the custom interface
    IUnknown FAR*  m_punkCustom;
};


enum IMETH_UNIVERSAL {
    IMETH_UNIVERSAL_QueryInterface = 0,
    IMETH_UNIVERSAL_AddRef,
    IMETH_UNIVERSAL_Release,

    IMETH_UNIVERSAL_GetTypeInfoCount,
    IMETH_UNIVERSAL_GetTypeInfo,
    IMETH_UNIVERSAL_GetIDsOfNames,
    IMETH_UNIVERSAL_Invoke,

    IMETH_UNIVERSAL_PSInit = 32000	// something much larger than the
					// max possible vtable index
};

