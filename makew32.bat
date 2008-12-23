@echo off
REM echo %QMAKESPEC%
IF NOT DEFINED QMAKESPEC GOTO NOQT

IF DEFINED BUILDENV GOTO MAKE
SET BUILDENV=1
SET ISC="C:\Program Files\Inno Setup 4"
SET PATH=%QTDIR%\bin;%QTDIR%\plugins\designer;%ISC%;%PATH%
:MAKE
IF %QMAKESPEC%==win32-borland GOTO BCC
IF %QMAKESPEC%==win32-msvc.net GOTO VCC
goto OTHER
exit

:BCC
echo Build using Borlanc C++ compiler.
SET MAKE=make
REM SET BCC=c:\Borland\bcc55
REM SET PATH=%bcc%\bin;%PATH%

make -f Makefile.w32 %1
goto COMMON

:VCC
echo Build using Microsoft Visual C++ compiler.
SET MAKE=nmake
nmake -f Makefile.w32 %1
goto COMMON

:OTHER
echo Build impossible: unknown compiler.
exit

:COMMON

exit

:NOQT
echo Build impossible: QT Library not installed.
exit