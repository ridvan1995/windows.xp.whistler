!IF 0

Copyright (c) 2000  Microsoft Corporation

Module Name:

    makefile.inc

Abstract:

    This file tells nmake the correct dependency relationships
    for objects created from c files in this directory which are
    wrappers for c files in another directory.


Author:

    Wesley Witt (wesw) 10-Aug-1993


!ENDIF

SRCDIR=..

!INCLUDE $(SRCDIR)\common.inc

OBJECTS = $(OBJECTS) $(BASEDIR)\neptune\xpos\rtl\user\obj\$(TARGET_DIRECTORY)\splay.obj
