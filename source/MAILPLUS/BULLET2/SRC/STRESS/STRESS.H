


//
//  Typedef the MAPI API's that we will be using.
//
typedef ULONG (FAR PASCAL *PFNMAPIADDRESS)(LHANDLE, ULONG, LPSTR, ULONG, LPSTR, ULONG, lpMapiRecipDesc, ULONG, ULONG, PULONG, lpMapiRecipDesc *);
typedef ULONG (FAR PASCAL *PFNMAPILOGOFF)(LHANDLE, ULONG, FLAGS, ULONG);
typedef ULONG (FAR PASCAL *PFNMAPILOGON)(ULONG, LPSTR, LPSTR, FLAGS, ULONG, LPLHANDLE);
typedef ULONG (FAR PASCAL *PFNMAPISENDMAIL)(LHANDLE, ULONG, lpMapiMessage, FLAGS, ULONG);
typedef ULONG (FAR PASCAL *PFNMAPISENDDOCUMENTS)(ULONG, LPSTR, LPSTR, LPSTR, ULONG);
