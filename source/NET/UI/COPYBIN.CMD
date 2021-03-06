@echo off
setlocal

if     "%BASEDIR%%_XPDRIVE%==" goto envvarerr
if     "%BASEDIR%==" set BASEDIR=%_XPDRIVE%\nt

if     "%1==" set source_dir=\\DAVIDHOV1\NTSRC
if not "%1==" set source_dir=%1
if     "%2==" set dest_dir=%BASEDIR%
if not "%2==" set dest_dir=%2
if     "%3==" set TC_flags=/irtA

rem Jon Newman, 23 November 2000
rem From Rustan/Chuck's qqchk.cmd

rem usage:  copybin [source_dir [dest_dir [TC_flags]]]
rem         %0       %1          %2        %3

set TC_flags=
:top_loop
if not "%3==" set TC_flags=%TC_flags% %3 && shift && goto :top_loop

@echo Copying from directory :  %source_dir%
@echo Copying to directory   :  %dest_dir%
@echo TC.EXE Flags           :  %TC_flags%
@echo Wanna carry on? (Control-C if no)
pause

tc %TC_flags% %source_dir%\public %dest_dir%\public
if not errorlevel 0 goto copyerr
tc %TC_flags% %source_dir%\neptune\net\winnet %dest_dir%\neptune\net\winnet
if not errorlevel 0 goto copyerr
tc %TC_flags% %source_dir%\neptune\net\api %dest_dir%\neptune\net\api
if not errorlevel 0 goto copyerr
tc %TC_flags% %source_dir%\neptune\net\inc %dest_dir%\neptune\net\inc
if not errorlevel 0 goto copyerr
tc %TC_flags% %source_dir%\neptune\eventlog %dest_dir%\neptune\eventlog
if not errorlevel 0 goto copyerr

goto :exit

:copyerr
@echo Treecopy failed
goto :exit

:envvarerr
@echo Either _NTDRIVE or BASEDIR must be defined
goto :exit

:syntax
@echo usage:  copybin [source_dir [dest_dir [TC_flags]]]

:exit
