# Microsoft Developer Studio Project File - Name="GdiLeakDetector" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=GdiLeakDetector - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "GdiLeakDetector.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "GdiLeakDetector.mak" CFG="GdiLeakDetector - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "GdiLeakDetector - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "GdiLeakDetector - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "GdiLeakDetector - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 psapi.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "GdiLeakDetector - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 psapi.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "GdiLeakDetector - Win32 Release"
# Name "GdiLeakDetector - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BitmapTabCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\CategoryStatisticsPage.cpp
# End Source File
# Begin Source File

SOURCE=.\CodeShow.cpp
# End Source File
# Begin Source File

SOURCE=.\CpuPage.cpp
# End Source File
# Begin Source File

SOURCE=.\CurveSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\CurveView.cpp
# End Source File
# Begin Source File

SOURCE=.\Gdi.cpp
# End Source File
# Begin Source File

SOURCE=.\GdiLeakDetector.cpp
# End Source File
# Begin Source File

SOURCE=.\GdiLeakDetector.rc
# End Source File
# Begin Source File

SOURCE=.\GdiLeakDetectorDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\GdiLeakDetectorView.cpp
# End Source File
# Begin Source File

SOURCE=.\GdiNumberPage.cpp
# End Source File
# Begin Source File

SOURCE=.\GdiObjWatchPage.cpp
# End Source File
# Begin Source File

SOURCE=.\InfoView.cpp
# End Source File
# Begin Source File

SOURCE=.\Inject.cpp
# End Source File
# Begin Source File

SOURCE=.\LeftView.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\Markup.cpp
# End Source File
# Begin Source File

SOURCE=.\MemoryPage.cpp
# End Source File
# Begin Source File

SOURCE=.\OutputPage.cpp
# End Source File
# Begin Source File

SOURCE=.\RightView2.cpp
# End Source File
# Begin Source File

SOURCE=.\SplitterWndEx.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\ThreadPage.cpp
# End Source File
# Begin Source File

SOURCE=.\XMLOperator.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BitmapTabCtrl.h
# End Source File
# Begin Source File

SOURCE=.\CategoryStatisticsPage.h
# End Source File
# Begin Source File

SOURCE=.\CodeShow.h
# End Source File
# Begin Source File

SOURCE=.\CpuPage.h
# End Source File
# Begin Source File

SOURCE=.\CurveSheet.h
# End Source File
# Begin Source File

SOURCE=.\CurveView.h
# End Source File
# Begin Source File

SOURCE=.\Gdi.h
# End Source File
# Begin Source File

SOURCE=.\GdiLeakDetector.h
# End Source File
# Begin Source File

SOURCE=.\GdiLeakDetectorDoc.h
# End Source File
# Begin Source File

SOURCE=.\GdiLeakDetectorView.h
# End Source File
# Begin Source File

SOURCE=.\GdiNumberPage.h
# End Source File
# Begin Source File

SOURCE=.\GdiObjWatchPage.h
# End Source File
# Begin Source File

SOURCE=.\InfoView.h
# End Source File
# Begin Source File

SOURCE=.\Inject.h
# End Source File
# Begin Source File

SOURCE=.\LeftView.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\Markup.h
# End Source File
# Begin Source File

SOURCE=.\MemoryPage.h
# End Source File
# Begin Source File

SOURCE=.\OutputPage.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\RightView2.h
# End Source File
# Begin Source File

SOURCE=.\SplitterWndEx.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Struct_Define.h
# End Source File
# Begin Source File

SOURCE=.\ThreadPage.h
# End Source File
# Begin Source File

SOURCE=.\XMLOperator.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap5.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap6.bmp
# End Source File
# Begin Source File

SOURCE=.\res\GdiLeakDetector.ico
# End Source File
# Begin Source File

SOURCE=.\res\GdiLeakDetector.rc2
# End Source File
# Begin Source File

SOURCE=.\res\GdiLeakDetectorDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\White.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
