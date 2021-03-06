/****************************** Module Header ******************************\
* Module Name: wintext.h
*
* Copyright (c) 2000, Microsoft Corporation
*
* wintext's header file
*
* History:
* 11-21-91 KentD      Created.
\***************************************************************************/

#include <windows.h>
#include <string.h>

/*
 * Resource defines
 */
#define WINDOWMENU  1

#ifdef RC_INVOKED
#define ID(id) id
#else
#define ID(id) MAKEINTRESOURCE(id)
#endif

/*
 * Menu ID's
 */
#define MM_NEW          8001
#define MM_EXIT         8002
#define MM_ABOUT        8003
#define IDM_STARTTESTS  8004
#define IDM_STOP	8005
#define IDM_0		8010
#define IDM_1		8011
#define IDM_2		8012
#define IDM_3		8013
#define IDM_4		8014
#define IDM_5		8015
#define IDM_6		8016
#define IDM_7		8017
#define IDM_8		8018
#define IDM_9		8019

#define IDM_PERF	8020
#define IDM_TEST1	8021
#define IDM_TEST2	8022
#define IDM_TEST3	8023
#define IDM_DIM1	8024
#define IDM_DIM2	8025
#define IDM_DIM3	8026
#define IDM_DIM4	8027
#define IDM_DIM5	8028

#define IDM_SCL1	8029
#define IDM_SCL2	8030
#define IDM_SCL3	8031
#define IDM_SCL4	8032
#define IDM_SCL5	8033
#define IDM_SCL6	8034
#define IDM_SCL7	8035
#define IDM_ROT1	8036
#define IDM_ROT2	8037
#define IDM_ROT3	8038
