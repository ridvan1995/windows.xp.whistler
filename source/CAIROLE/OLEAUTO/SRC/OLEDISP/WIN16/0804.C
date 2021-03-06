/***
*0804.c
*
*  Copyright (C) 2000-93, Microsoft Corporation.  All Rights Reserved.
*  Information Contained Herein Is Proprietary and Confidential.
*
*Purpose:
*
*  China, Chinese Simplified (People's Republic of China [PRC])
*
*  LCID = 0x0804
*
*
*Generated: 9/22/93 - by hand from lcid_chs.txt
*
*
*****************************************************************************/

#include "oledisp.h"
#include "nlsintrn.h"
#include "nlsdbcs.h"


static SORTWEIGHT NLSALLOC(0804) rgsortweight[] = {
      { 0x0000, 0x0200, 0x00, 0x00 }
    , { 0x0020, 0x1020, 0x00, 0x02 }
    , { 0x0030, 0x5030, 0x00, 0x02 }
    , { 0x003A, 0x103A, 0x00, 0x02 }
    , { 0x0041, 0x6041, 0x00, 0x02 }
    , { 0x005B, 0x105B, 0x00, 0x02 }
    , { 0x0061, 0x6041, 0x02, 0x02 }
    , { 0x007B, 0x107B, 0x00, 0x02 }
    , { 0x007F, 0xFFFF, 0x00, 0xFF }
    , { 0xA1A1, 0x1020, 0x01, 0x02 }
    , { 0xA1A2, 0x20A2, 0x00, 0x00 }
    , { 0xA3A1, 0x1021, 0x01, 0x02 }
    , { 0xA3B0, 0x5030, 0x01, 0x02 }
    , { 0xA3BA, 0x103A, 0x01, 0x02 }
    , { 0xA3C1, 0x6041, 0x01, 0x02 }
    , { 0xA3DB, 0x105B, 0x01, 0x02 }
    , { 0xA3E1, 0x6041, 0x03, 0x02 }
    , { 0xA3FB, 0x107B, 0x01, 0x02 }
    , { 0xA4A1, 0x30A1, 0x00, 0x00 }
    , { 0xA6A1, 0x32A1, 0x01, 0x02 }
    , { 0xA6C1, 0x32A1, 0x03, 0x02 }
    , { 0xA7A1, 0x33A1, 0x01, 0x02 }
    , { 0xA7D1, 0x33A1, 0x03, 0x02 }
    , { 0xA8A1, 0x34A1, 0x00, 0x00 }
    , { 0xB0A1, 0xB0A1, 0x00, 0x00 }
};
static MAPTABLE NLSALLOC(0804) rgmaptable[] = {
      { 0x1020, 0x0001, 0x0020, 0xA1A1, 0x0020, 0xA1A1 }
    , { 0x1021, 0x005F, 0x0021, 0xA3A1, 0x0021, 0xA3A1 }
    , { 0x32A1, 0x0018, 0xA6A1, 0xA6A1, 0xA6C1, 0xA6C1 }
    , { 0x33A1, 0x0021, 0xA7A1, 0xA7A1, 0xA7D1, 0xA7D1 }
    , { 0x5030, 0x000A, 0x0030, 0xA3B0, 0x0030, 0xA3B0 }
    , { 0x6041, 0x001A, 0x0041, 0xA3C1, 0x0061, 0xA3E1 }
};
static TYPETABLE NLSALLOC(0804) rgtypetable[] = {
      { 0x0000, 0x0020, 0x00, 0x00 }
    , { 0x0009, 0x0068, 0x09, 0x00 }
    , { 0x000A, 0x0028, 0x00, 0x00 }
    , { 0x000E, 0x0020, 0x00, 0x00 }
    , { 0x0020, 0x0048, 0x0A, 0x00 }
    , { 0x0021, 0x0010, 0x0B, 0x08 }
    , { 0x0024, 0x0010, 0x05, 0x08 }
    , { 0x0026, 0x0010, 0x01, 0x08 }
    , { 0x002B, 0x0010, 0x05, 0x08 }
    , { 0x002C, 0x0010, 0x07, 0x08 }
    , { 0x002D, 0x0010, 0x05, 0x08 }
    , { 0x002E, 0x0010, 0x03, 0x08 }
    , { 0x0030, 0x0084, 0x03, 0x00 }
    , { 0x003A, 0x0010, 0x07, 0x08 }
    , { 0x003B, 0x0010, 0x0B, 0x08 }
    , { 0x0040, 0x0010, 0x01, 0x08 }
    , { 0x0041, 0x0181, 0x01, 0x00 }
    , { 0x0047, 0x0101, 0x01, 0x00 }
    , { 0x005B, 0x0010, 0x0B, 0x08 }
    , { 0x0061, 0x0182, 0x01, 0x00 }
    , { 0x0067, 0x0102, 0x01, 0x00 }
    , { 0x007B, 0x0010, 0x0B, 0x08 }
    , { 0x007F, 0x0020, 0x00, 0x00 }
    , { 0x0080, 0x0000, 0x0B, 0x08 }
    , { 0xA1A1, 0x0048, 0x0A, 0x00 }
    , { 0xA1A2, 0x0000, 0x0B, 0x08 }
    , { 0xA3A1, 0x0010, 0x0B, 0x08 }
    , { 0xA3A4, 0x0010, 0x05, 0x08 }
    , { 0xA3A6, 0x0010, 0x01, 0x08 }
    , { 0xA3AB, 0x0010, 0x05, 0x08 }
    , { 0xA3AC, 0x0010, 0x07, 0x08 }
    , { 0xA3AD, 0x0010, 0x05, 0x08 }
    , { 0xA3AE, 0x0010, 0x03, 0x08 }
    , { 0xA3B0, 0x0084, 0x03, 0x00 }
    , { 0xA3BA, 0x0010, 0x07, 0x08 }
    , { 0xA3BB, 0x0010, 0x0B, 0x08 }
    , { 0xA3C0, 0x0010, 0x01, 0x08 }
    , { 0xA3C1, 0x0181, 0x01, 0x00 }
    , { 0xA3C7, 0x0101, 0x01, 0x00 }
    , { 0xA3DB, 0x0010, 0x0B, 0x08 }
    , { 0xA3E1, 0x0182, 0x01, 0x00 }
    , { 0xA3E7, 0x0102, 0x01, 0x00 }
    , { 0xA3FB, 0x0010, 0x0B, 0x08 }
    , { 0xA4A1, 0x0010, 0x0B, 0x08 }
    , { 0xB0A1, 0x0100, 0x01, 0x00 }
};


static unsigned char NLSALLOC(0804) rgbSENGCOUNTRY[] = { 
    'P', 'e', 'o', 'p', 'l', 'e', 's', '\'', ' ',
    'R', 'e', 'p', 'u', 'b', 'l', 'i', 'c', ' ',
    'o', 'f', ' ',
    'C', 'h', 'i', 'n', 'a'
};

static unsigned char NLSALLOC(0804) rgbSENGLANGUAGE[] = { 
    'C', 'h', 'i', 'n', 'e', 's', 'e'
};

static unsigned char NLSALLOC(0804) rgbILANGUAGE[] = { 
    '0', '8', '0', '4'
};

static unsigned char NLSALLOC(0804) rgbSLANGUAGE[] = {	
    0xd6, 0xd0, 0xce, 0xc4, '(', 'C', 'h', 'i', 'n', 'e', 's', 'e', ' ', 'S', 'i', 'm', 'p', 'l', 'i', 'f', 'i', 'e', 'd', ')'
};

static unsigned char NLSALLOC(0804) rgbSABBREVLANGNAME[] = {
    'C', 'H', 'S'
};

static unsigned char NLSALLOC(0804) rgbSNATIVELANGNAME[] = {
    0xd6, 0xd0, 0xce, 0xc4
};

static unsigned char NLSALLOC(0804) rgbICOUNTRY[] = { /* "86" */
    '8', '6'
};

static unsigned char NLSALLOC(0804) rgbSCOUNTRY[] = {
    0xd6, 0xd0, 0xbb, 0xaa, 0xc8, 0xcb, 0xc3, 0xf1, 
    0xb9, 0xb2, 0xba, 0xcd, 0xb9, 0xfa,
    '(', 'C', 'h', 'i', 'n', 'a', ')'
};

static unsigned char NLSALLOC(0804) rgbSABBREVCTRYNAME[] = { 
    'C', 'H', 'N'
};

static unsigned char NLSALLOC(0804) rgbSNATIVECTRYNAME[] = {
    0xd6, 0xd0, 0xbb, 0xaa, 0xc8, 0xcb, 0xc3, 0xf1, 
    0xb9, 0xb2, 0xba, 0xcd, 0xb9, 0xfa
};

static unsigned char NLSALLOC(0804) rgbIDEFAULTLANGUAGE[] = { 
    '0', '8', '0', '4'
};

static unsigned char NLSALLOC(0804) rgbIDEFAULTCOUNTRY[] = { 
    '0', '8', '6'
};

static unsigned char NLSALLOC(0804) rgbIDEFAULTCODEPAGE[] = { 
    '9', '3', '6'
};

static unsigned char NLSALLOC(0804) rgbSLIST[] = { 
    ','
};

static unsigned char NLSALLOC(0804) rgbIMEASURE[] = { 
    '0'
};

static unsigned char NLSALLOC(0804) rgbSDECIMAL[] = { 
    '.'
};

static unsigned char NLSALLOC(0804) rgbSTHOUSAND[] = { 
    ','
};

static unsigned char NLSALLOC(0804) rgbSGROUPING[] = { 
    '3', ';', '0'
};

static unsigned char NLSALLOC(0804) rgbIDIGITS[] = { 
    '2'
};

static unsigned char NLSALLOC(0804) rgbILZERO[] = { 
    '1'
};

static unsigned char NLSALLOC(0804) rgbSNATIVEDIGITS[] = { 
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
};

static unsigned char NLSALLOC(0804) rgbSCURRENCY[] = { 
    0xa3, 0xa4
};

static unsigned char NLSALLOC(0804) rgbSINTLSYMBOL[] = { 
    'C', 'N', 'Y'
};

static unsigned char NLSALLOC(0804) rgbSMONDECIMALSEP[] = { 
    '.'
};

static unsigned char NLSALLOC(0804) rgbSMONTHOUSANDSEP[] = { 
    ','
};

static unsigned char NLSALLOC(0804) rgbSMONGROUPING[] = { 
    '3', ';', '0'
};

static unsigned char NLSALLOC(0804) rgbICURRDIGITS[] = { 
    '2'
};

static unsigned char NLSALLOC(0804) rgbIINTLCURRDIGITS[] = { 
    '2'
};

static unsigned char NLSALLOC(0804) rgbICURRENCY[] = { 
    '0'
};

static unsigned char NLSALLOC(0804) rgbINEGCURR[] = { 
    '0'
};

static unsigned char NLSALLOC(0804) rgbSDATE[] = { 
    '/'
};

static unsigned char NLSALLOC(0804) rgbSTIME[] = { 
    ':'
};

static unsigned char NLSALLOC(0804) rgbSSHORTDATE[] = { 
    'y', 'y', 'y', 'y', '/', 'M', '/', 'd'
};

static unsigned char NLSALLOC(0804) rgbSLONGDATE[] = { 
    'd', 'd', 'd', 'd', ',', ' ', 
    'M', 'M', 'M', 'M', ' ',
    'd', ',', ' ',
    'y', 'y', 'y', 'y'
};

static unsigned char NLSALLOC(0804) rgbIDATE[] = { 
    '2'
};

static unsigned char NLSALLOC(0804) rgbILDATE[] = { 
    '2'
};

static unsigned char NLSALLOC(0804) rgbITIME[] = { 
    '0'
};

static unsigned char NLSALLOC(0804) rgbICENTURY[] = { 
    '0'
};

static unsigned char NLSALLOC(0804) rgbITLZERO[] = { 
    '0'
};

static unsigned char NLSALLOC(0804) rgbIDAYLZERO[] = { 
    '0'
};

static unsigned char NLSALLOC(0804) rgbIMONLZERO[] = { 
    '0'
};

static unsigned char NLSALLOC(0804) rgbS1159[] = { 
    0xc9, 0xcf, 0xce, 0xe7
};

static unsigned char NLSALLOC(0804) rgbS2359[] = { 
    0xcf, 0xc2, 0xce, 0xe7
};

static unsigned char NLSALLOC(0804) rgbSDAYNAME1[] = {
    0xd0, 0xc7, 0xc6, 0xda, 0xd2, 0xbb
};

static unsigned char NLSALLOC(0804) rgbSDAYNAME2[] = {
    0xd0, 0xc7, 0xc6, 0xda, 0xb6, 0xfe
};

static unsigned char NLSALLOC(0804) rgbSDAYNAME3[] = {
    0xd0, 0xc7, 0xc6, 0xda, 0xc8, 0xfd
};

static unsigned char NLSALLOC(0804) rgbSDAYNAME4[] = {
    0xd0, 0xc7, 0xc6, 0xda, 0xcb, 0xc4
};

static unsigned char NLSALLOC(0804) rgbSDAYNAME5[] = {
    0xd0, 0xc7, 0xc6, 0xda, 0xce, 0xe5
};

static unsigned char NLSALLOC(0804) rgbSDAYNAME6[] = {
    0xd0, 0xc7, 0xc6, 0xda, 0xc1, 0xf9
};

static unsigned char NLSALLOC(0804) rgbSDAYNAME7[] = {
    0xd0, 0xc7, 0xc6, 0xda, 0xc8, 0xd5
};

static unsigned char NLSALLOC(0804) rgbSABBREVDAYNAME1[] = {
    0xd0, 0xc7, 0xc6, 0xda, 0xd2, 0xbb
};

static unsigned char NLSALLOC(0804) rgbSABBREVDAYNAME2[] = {
    0xd0, 0xc7, 0xc6, 0xda, 0xb6, 0xfe
};

static unsigned char NLSALLOC(0804) rgbSABBREVDAYNAME3[] = {
    0xd0, 0xc7, 0xc6, 0xda, 0xc8, 0xfd
};

static unsigned char NLSALLOC(0804) rgbSABBREVDAYNAME4[] = {
    0xd0, 0xc7, 0xc6, 0xda, 0xcb, 0xc4
};

static unsigned char NLSALLOC(0804) rgbSABBREVDAYNAME5[] = {
    0xd0, 0xc7, 0xc6, 0xda, 0xce, 0xe5
};

static unsigned char NLSALLOC(0804) rgbSABBREVDAYNAME6[] = {
    0xd0, 0xc7, 0xc6, 0xda, 0xc1, 0xf9
};

static unsigned char NLSALLOC(0804) rgbSABBREVDAYNAME7[] = {
    0xd0, 0xc7, 0xc6, 0xda, 0xc8, 0xd5
};

static unsigned char NLSALLOC(0804) rgbSMONTHNAME1[] = {
    0xd2, 0xbb, 0xd4, 0xc2
};

static unsigned char NLSALLOC(0804) rgbSMONTHNAME2[] = {
    0xb6, 0xfe, 0xd4, 0xc2
};

static unsigned char NLSALLOC(0804) rgbSMONTHNAME3[] = {
    0xc8, 0xfd, 0xd4, 0xc2
};

static unsigned char NLSALLOC(0804) rgbSMONTHNAME4[] = {
    0xcb, 0xc4, 0xd4, 0xc2
};

static unsigned char NLSALLOC(0804) rgbSMONTHNAME5[] = {
    0xce, 0xe5, 0xd4, 0xc2
};

static unsigned char NLSALLOC(0804) rgbSMONTHNAME6[] = {
    0xc1, 0xf9, 0xd4, 0xc2
};

static unsigned char NLSALLOC(0804) rgbSMONTHNAME7[] = {
    0xc6, 0xdf, 0xd4, 0xc2
};

static unsigned char NLSALLOC(0804) rgbSMONTHNAME8[] = {
    0xb0, 0xcb, 0xd4, 0xc2
};

static unsigned char NLSALLOC(0804) rgbSMONTHNAME9[] = {
    0xbe, 0xc5, 0xd4, 0xc2
};

static unsigned char NLSALLOC(0804) rgbSMONTHNAME10[] = {
    0xca, 0xae, 0xd4, 0xc2
};

static unsigned char NLSALLOC(0804) rgbSMONTHNAME11[] = {
    0xca, 0xae, 0xd2, 0xbb, 0xd4, 0xc2
};

static unsigned char NLSALLOC(0804) rgbSMONTHNAME12[] = {
    0xca, 0xae, 0xb6, 0xfe, 0xd4, 0xc2
};

static unsigned char NLSALLOC(0804) rgbSABBREVMONTHNAME1[] = {
    0xd2, 0xbb, 0xd4, 0xc2
};

static unsigned char NLSALLOC(0804) rgbSABBREVMONTHNAME2[] = {
    0xb6, 0xfe, 0xd4, 0xc2
};

static unsigned char NLSALLOC(0804) rgbSABBREVMONTHNAME3[] = {
    0xc8, 0xfd, 0xd4, 0xc2
};

static unsigned char NLSALLOC(0804) rgbSABBREVMONTHNAME4[] = {
    0xcb, 0xc4, 0xd4, 0xc2
};

static unsigned char NLSALLOC(0804) rgbSABBREVMONTHNAME5[] = {
    0xce, 0xe5, 0xd4, 0xc2
};
 
static unsigned char NLSALLOC(0804) rgbSABBREVMONTHNAME6[] = {
    0xc1, 0xf9, 0xd4, 0xc2
};

static unsigned char NLSALLOC(0804) rgbSABBREVMONTHNAME7[] = {
    0xc6, 0xdf, 0xd4, 0xc2
};

static unsigned char NLSALLOC(0804) rgbSABBREVMONTHNAME8[] = {
    0xb0, 0xcb, 0xd4, 0xc2
};

static unsigned char NLSALLOC(0804) rgbSABBREVMONTHNAME9[] = {
    0xbe, 0xc5, 0xd4, 0xc2
};

static unsigned char NLSALLOC(0804) rgbSABBREVMONTHNAME10[] = {
    0xca, 0xae, 0xd4, 0xc2
};

static unsigned char NLSALLOC(0804) rgbSABBREVMONTHNAME11[] = {
    0xca, 0xae, 0xd2, 0xbb, 0xd4, 0xc2
};

static unsigned char NLSALLOC(0804) rgbSABBREVMONTHNAME12[] = {
    0xca, 0xae, 0xb6, 0xfe, 0xd4, 0xc2
};

static unsigned char NLSALLOC(0804) rgbSNEGATIVESIGN[] = { 
    '-'
};

static unsigned char NLSALLOC(0804) rgbIPOSSIGNPOSN[] = { 
    '3'
};

static unsigned char NLSALLOC(0804) rgbINEGSIGNPOSN[] = { 
    '3'
};

static unsigned char NLSALLOC(0804) rgbIPOSSYMPRECEDES[] = { 
    '1'
};

static unsigned char NLSALLOC(0804) rgbIPOSSEPBYSPACE[] = { 
    '0'
};

static unsigned char NLSALLOC(0804) rgbINEGSYMPRECEDES[] = { 
    '1'
};

static unsigned char NLSALLOC(0804) rgbINEGSEPBYSPACE[] = { 
    '0'
};


#define LCINFODAT(X) { DIM(X), (X) }
LCINFO NLSALLOC(0804) g_rglcinfo0804[] = {
      { 0, NULL}
    , LCINFODAT(rgbILANGUAGE)
    , LCINFODAT(rgbSLANGUAGE)
    , LCINFODAT(rgbSABBREVLANGNAME)
    , LCINFODAT(rgbSNATIVELANGNAME)
    , LCINFODAT(rgbICOUNTRY)
    , LCINFODAT(rgbSCOUNTRY)
    , LCINFODAT(rgbSABBREVCTRYNAME)
    , LCINFODAT(rgbSNATIVECTRYNAME)
    , LCINFODAT(rgbIDEFAULTLANGUAGE)
    , LCINFODAT(rgbIDEFAULTCOUNTRY)
    , LCINFODAT(rgbIDEFAULTCODEPAGE)
    , LCINFODAT(rgbSLIST)
    , LCINFODAT(rgbIMEASURE)
    , LCINFODAT(rgbSDECIMAL)
    , LCINFODAT(rgbSTHOUSAND)
    , LCINFODAT(rgbSGROUPING)
    , LCINFODAT(rgbIDIGITS)
    , LCINFODAT(rgbILZERO)
    , LCINFODAT(rgbSNATIVEDIGITS)
    , LCINFODAT(rgbSCURRENCY)
    , LCINFODAT(rgbSINTLSYMBOL)
    , LCINFODAT(rgbSMONDECIMALSEP)
    , LCINFODAT(rgbSMONTHOUSANDSEP)
    , LCINFODAT(rgbSMONGROUPING)
    , LCINFODAT(rgbICURRDIGITS)
    , LCINFODAT(rgbIINTLCURRDIGITS)
    , LCINFODAT(rgbICURRENCY)
    , LCINFODAT(rgbINEGCURR)
    , LCINFODAT(rgbSDATE)
    , LCINFODAT(rgbSTIME)
    , LCINFODAT(rgbSSHORTDATE)
    , LCINFODAT(rgbSLONGDATE)
    , LCINFODAT(rgbIDATE)
    , LCINFODAT(rgbILDATE)
    , LCINFODAT(rgbITIME)
    , LCINFODAT(rgbICENTURY)
    , LCINFODAT(rgbITLZERO)
    , LCINFODAT(rgbIDAYLZERO)
    , LCINFODAT(rgbIMONLZERO)
    , LCINFODAT(rgbS1159)
    , LCINFODAT(rgbS2359)
    , LCINFODAT(rgbSDAYNAME1)
    , LCINFODAT(rgbSDAYNAME2)
    , LCINFODAT(rgbSDAYNAME3)
    , LCINFODAT(rgbSDAYNAME4)
    , LCINFODAT(rgbSDAYNAME5)
    , LCINFODAT(rgbSDAYNAME6)
    , LCINFODAT(rgbSDAYNAME7)
    , LCINFODAT(rgbSABBREVDAYNAME1)
    , LCINFODAT(rgbSABBREVDAYNAME2)
    , LCINFODAT(rgbSABBREVDAYNAME3)
    , LCINFODAT(rgbSABBREVDAYNAME4)
    , LCINFODAT(rgbSABBREVDAYNAME5)
    , LCINFODAT(rgbSABBREVDAYNAME6)
    , LCINFODAT(rgbSABBREVDAYNAME7)
    , LCINFODAT(rgbSMONTHNAME1)
    , LCINFODAT(rgbSMONTHNAME2)
    , LCINFODAT(rgbSMONTHNAME3)
    , LCINFODAT(rgbSMONTHNAME4)
    , LCINFODAT(rgbSMONTHNAME5)
    , LCINFODAT(rgbSMONTHNAME6)
    , LCINFODAT(rgbSMONTHNAME7)
    , LCINFODAT(rgbSMONTHNAME8)
    , LCINFODAT(rgbSMONTHNAME9)
    , LCINFODAT(rgbSMONTHNAME10)
    , LCINFODAT(rgbSMONTHNAME11)
    , LCINFODAT(rgbSMONTHNAME12)
    , LCINFODAT(rgbSABBREVMONTHNAME1)
    , LCINFODAT(rgbSABBREVMONTHNAME2)
    , LCINFODAT(rgbSABBREVMONTHNAME3)
    , LCINFODAT(rgbSABBREVMONTHNAME4)
    , LCINFODAT(rgbSABBREVMONTHNAME5)
    , LCINFODAT(rgbSABBREVMONTHNAME6)
    , LCINFODAT(rgbSABBREVMONTHNAME7)
    , LCINFODAT(rgbSABBREVMONTHNAME8)
    , LCINFODAT(rgbSABBREVMONTHNAME9)
    , LCINFODAT(rgbSABBREVMONTHNAME10)
    , LCINFODAT(rgbSABBREVMONTHNAME11)
    , LCINFODAT(rgbSABBREVMONTHNAME12)
    , { 0, NULL }
    , LCINFODAT(rgbSNEGATIVESIGN)
    , LCINFODAT(rgbIPOSSIGNPOSN)
    , LCINFODAT(rgbINEGSIGNPOSN)
    , LCINFODAT(rgbIPOSSYMPRECEDES)
    , LCINFODAT(rgbIPOSSEPBYSPACE)
    , LCINFODAT(rgbINEGSYMPRECEDES)
    , LCINFODAT(rgbINEGSEPBYSPACE)
    , LCINFODAT(rgbSENGCOUNTRY)
    , LCINFODAT(rgbSENGLANGUAGE)
};
#undef LCINFODAT


STRINFO_KTP NLSALLOC(0804) g_strinfo0804 = {
      rgsortweight
    , rgmaptable
    , rgtypetable
    , DIM(rgsortweight)
    , DIM(rgmaptable)
    , DIM(rgtypetable)
};
