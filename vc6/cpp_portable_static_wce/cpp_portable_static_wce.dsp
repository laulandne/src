# Microsoft Developer Studio Project File - Name="cpp_portable_static_wce" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (WCE x86) Static Library" 0x8304
# TARGTYPE "Win32 (WCE SH4) Static Library" 0x8604
# TARGTYPE "Win32 (WCE MIPS) Static Library" 0x8204
# TARGTYPE "Win32 (WCE x86em) Static Library" 0x7f04
# TARGTYPE "Win32 (WCE MIPSFP) Static Library" 0x8704
# TARGTYPE "Win32 (WCE ARM) Static Library" 0x8504
# TARGTYPE "Win32 (WCE SH3) Static Library" 0x8104
# TARGTYPE "Win32 (WCE PPC) Static Library" 0x8404

CFG=cpp_portable_static_wce - Win32 (WCE MIPS) Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "cpp_portable_static_wce.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "cpp_portable_static_wce.mak" CFG="cpp_portable_static_wce - Win32 (WCE MIPS) Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "cpp_portable_static_wce - Win32 (WCE MIPS) Release" (based on "Win32 (WCE MIPS) Static Library")
!MESSAGE "cpp_portable_static_wce - Win32 (WCE MIPS) Debug" (based on "Win32 (WCE MIPS) Static Library")
!MESSAGE "cpp_portable_static_wce - Win32 (WCE SH4) Release" (based on "Win32 (WCE SH4) Static Library")
!MESSAGE "cpp_portable_static_wce - Win32 (WCE SH4) Debug" (based on "Win32 (WCE SH4) Static Library")
!MESSAGE "cpp_portable_static_wce - Win32 (WCE SH3) Release" (based on "Win32 (WCE SH3) Static Library")
!MESSAGE "cpp_portable_static_wce - Win32 (WCE SH3) Debug" (based on "Win32 (WCE SH3) Static Library")
!MESSAGE "cpp_portable_static_wce - Win32 (WCE ARM) Release" (based on "Win32 (WCE ARM) Static Library")
!MESSAGE "cpp_portable_static_wce - Win32 (WCE ARM) Debug" (based on "Win32 (WCE ARM) Static Library")
!MESSAGE "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release" (based on "Win32 (WCE MIPSFP) Static Library")
!MESSAGE "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug" (based on "Win32 (WCE MIPSFP) Static Library")
!MESSAGE "cpp_portable_static_wce - Win32 (WCE x86) Release" (based on "Win32 (WCE x86) Static Library")
!MESSAGE "cpp_portable_static_wce - Win32 (WCE x86) Debug" (based on "Win32 (WCE x86) Static Library")
!MESSAGE "cpp_portable_static_wce - Win32 (WCE x86em) Release" (based on "Win32 (WCE x86em) Static Library")
!MESSAGE "cpp_portable_static_wce - Win32 (WCE x86em) Debug" (based on "Win32 (WCE x86em) Static Library")
!MESSAGE "cpp_portable_static_wce - Win32 (WCE PPC) Release" (based on "Win32 (WCE PPC) Static Library")
!MESSAGE "cpp_portable_static_wce - Win32 (WCE PPC) Debug" (based on "Win32 (WCE PPC) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath "Palm-size PC 2.01"
# PROP WCE_FormatVersion "6.0"

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "WMIPSRel"
# PROP BASE Intermediate_Dir "WMIPSRel"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "WMIPSRel"
# PROP Intermediate_Dir "WMIPSRel"
# PROP Target_Dir ""
CPP=clmips.exe
# ADD BASE CPP /nologo /M$(CECrt) /W3 /GX- /O2 /D _WIN32_WCE=$(CEVersion) /D "$(CEConfigName)" /D "NDEBUG" /D "MIPS" /D "_MIPS_" /D UNDER_CE=$(CEVersion) /D "UNICODE" /D "_MBCS" /D "_LIB" /YX /QMRWCE   /c
# ADD CPP /nologo /M$(CECrt) /W3 /GX- /O2 /I "e:\src" /D "NDEBUG" /D "MIPS" /D "_MIPS_" /D _WIN32_WCE=$(CEVersion) /D "$(CEConfigName)" /D UNDER_CE=$(CEVersion) /D "UNICODE" /D "_MBCS" /D "_LIB" /D "ASYS_WIN32" /YX /QMRWCE   /c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "WMIPSDbg"
# PROP BASE Intermediate_Dir "WMIPSDbg"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "WMIPSDbg"
# PROP Intermediate_Dir "WMIPSDbg"
# PROP Target_Dir ""
CPP=clmips.exe
# ADD BASE CPP /nologo /M$(CECrtDebug) /W3 /GX- /Zi /Od /D _WIN32_WCE=$(CEVersion) /D "$(CEConfigName)" /D "DEBUG" /D "MIPS" /D "_MIPS_" /D UNDER_CE=$(CEVersion) /D "UNICODE" /D "_MBCS" /D "_LIB" /YX /QMRWCE   /c
# ADD CPP /nologo /M$(CECrtDebug) /W3 /GX- /Zi /Od /I "e:\src" /D "DEBUG" /D "MIPS" /D "_MIPS_" /D _WIN32_WCE=$(CEVersion) /D "$(CEConfigName)" /D UNDER_CE=$(CEVersion) /D "UNICODE" /D "_MBCS" /D "_LIB" /D "ASYS_WIN32" /YX /QMRWCE   /c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "WCESH4Rel"
# PROP BASE Intermediate_Dir "WCESH4Rel"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "WCESH4Rel"
# PROP Intermediate_Dir "WCESH4Rel"
# PROP Target_Dir ""
CPP=shcl.exe
# ADD BASE CPP /nologo /Qsh4 /MC /W3 /GX- /O2 /D _WIN32_WCE=$(CEVersion) /D "$(CEConfigName)" /D "NDEBUG" /D "SHx" /D "SH4" /D "_SH4_" /D UNDER_CE=$(CEVersion) /D "UNICODE" /D "_MBCS" /D "_LIB" /YX /c
# ADD CPP /nologo /Qsh4 /MC /W3 /GX- /O2 /I "e:\src" /D "NDEBUG" /D "SHx" /D "SH4" /D "_SH4_" /D _WIN32_WCE=$(CEVersion) /D "$(CEConfigName)" /D UNDER_CE=$(CEVersion) /D "UNICODE" /D "_MBCS" /D "_LIB" /D "ASYS_WIN32" /YX /c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "WCESH4Dbg"
# PROP BASE Intermediate_Dir "WCESH4Dbg"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "WCESH4Dbg"
# PROP Intermediate_Dir "WCESH4Dbg"
# PROP Target_Dir ""
CPP=shcl.exe
# ADD BASE CPP /nologo /Qsh4 /MC /W3 /GX- /Zi /Od /D _WIN32_WCE=$(CEVersion) /D "$(CEConfigName)" /D "DEBUG" /D "SHx" /D "SH4" /D "_SH4_" /D UNDER_CE=$(CEVersion) /D "UNICODE" /D "_MBCS" /D "_LIB" /YX /c
# ADD CPP /nologo /Qsh4 /MC /W3 /GX- /Zi /Od /I "e:\src" /D "DEBUG" /D "SHx" /D "SH4" /D "_SH4_" /D _WIN32_WCE=$(CEVersion) /D "$(CEConfigName)" /D UNDER_CE=$(CEVersion) /D "UNICODE" /D "_MBCS" /D "_LIB" /D "ASYS_WIN32" /YX /c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "WCESH3Rel"
# PROP BASE Intermediate_Dir "WCESH3Rel"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "WCESH3Rel"
# PROP Intermediate_Dir "WCESH3Rel"
# PROP Target_Dir ""
CPP=shcl.exe
# ADD BASE CPP /nologo /M$(CECrt) /W3 /GX- /O2 /D _WIN32_WCE=$(CEVersion) /D "$(CEConfigName)" /D "NDEBUG" /D "SHx" /D "SH3" /D "_SH3_" /D UNDER_CE=$(CEVersion) /D "UNICODE" /D "_MBCS" /D "_LIB" /YX /c
# ADD CPP /nologo /M$(CECrt) /W3 /GX- /O2 /I "e:\src" /D "NDEBUG" /D "SHx" /D "SH3" /D "_SH3_" /D _WIN32_WCE=$(CEVersion) /D "$(CEConfigName)" /D UNDER_CE=$(CEVersion) /D "UNICODE" /D "_MBCS" /D "_LIB" /D "ASYS_WIN32" /YX /c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "WCESH3Dbg"
# PROP BASE Intermediate_Dir "WCESH3Dbg"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "WCESH3Dbg"
# PROP Intermediate_Dir "WCESH3Dbg"
# PROP Target_Dir ""
CPP=shcl.exe
# ADD BASE CPP /nologo /M$(CECrtDebug) /W3 /GX- /Zi /Od /D _WIN32_WCE=$(CEVersion) /D "$(CEConfigName)" /D "DEBUG" /D "SHx" /D "SH3" /D "_SH3_" /D UNDER_CE=$(CEVersion) /D "UNICODE" /D "_MBCS" /D "_LIB" /YX /c
# ADD CPP /nologo /M$(CECrtDebug) /W3 /GX- /Zi /Od /I "e:\src" /D "DEBUG" /D "SHx" /D "SH3" /D "_SH3_" /D _WIN32_WCE=$(CEVersion) /D "$(CEConfigName)" /D UNDER_CE=$(CEVersion) /D "UNICODE" /D "_MBCS" /D "_LIB" /D "ASYS_WIN32" /YX /c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "WCEARMRel"
# PROP BASE Intermediate_Dir "WCEARMRel"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "WCEARMRel"
# PROP Intermediate_Dir "WCEARMRel"
# PROP Target_Dir ""
CPP=clarm.exe
# ADD BASE CPP /nologo /MC /W3 /GX- /O2 /D _WIN32_WCE=$(CEVersion) /D "$(CEConfigName)" /D "NDEBUG" /D "ARM" /D "_ARM_" /D UNDER_CE=$(CEVersion) /D "UNICODE" /D "_MBCS" /D "_LIB" /YX /c
# ADD CPP /nologo /MC /W3 /GX- /O2 /I "e:\src" /D "NDEBUG" /D "ARM" /D "_ARM_" /D _WIN32_WCE=$(CEVersion) /D "$(CEConfigName)" /D UNDER_CE=$(CEVersion) /D "UNICODE" /D "_MBCS" /D "_LIB" /D "ASYS_WIN32" /YX /c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "WCEARMDbg"
# PROP BASE Intermediate_Dir "WCEARMDbg"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "WCEARMDbg"
# PROP Intermediate_Dir "WCEARMDbg"
# PROP Target_Dir ""
CPP=clarm.exe
# ADD BASE CPP /nologo /MC /W3 /GX- /Zi /Od /D _WIN32_WCE=$(CEVersion) /D "$(CEConfigName)" /D "DEBUG" /D "ARM" /D "_ARM_" /D UNDER_CE=$(CEVersion) /D "UNICODE" /D "_MBCS" /D "_LIB" /YX /c
# ADD CPP /nologo /MC /W3 /GX- /Zi /Od /I "e:\src" /D "DEBUG" /D "ARM" /D "_ARM_" /D _WIN32_WCE=$(CEVersion) /D "$(CEConfigName)" /D UNDER_CE=$(CEVersion) /D "UNICODE" /D "_MBCS" /D "_LIB" /D "ASYS_WIN32" /YX /c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "WMIPSFPRel"
# PROP BASE Intermediate_Dir "WMIPSFPRel"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "WMIPSFPRel"
# PROP Intermediate_Dir "WMIPSFPRel"
# PROP Target_Dir ""
CPP=clmips.exe
# ADD BASE CPP /nologo /QMFWCE /MC /W3 /GX- /O2 /D _WIN32_WCE=$(CEVersion) /D "$(CEConfigName)" /D "NDEBUG" /D "MIPS" /D "_MIPS_" /D UNDER_CE=$(CEVersion) /D "UNICODE" /D "_MBCS" /D "_LIB" /YX /c
# ADD CPP /nologo /QMFWCE /MC /W3 /GX- /O2 /I "e:\src" /D "NDEBUG" /D "MIPS" /D "_MIPS_" /D _WIN32_WCE=$(CEVersion) /D "$(CEConfigName)" /D UNDER_CE=$(CEVersion) /D "UNICODE" /D "_MBCS" /D "_LIB" /D "ASYS_WIN32" /YX /c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "WMIPSFPDbg"
# PROP BASE Intermediate_Dir "WMIPSFPDbg"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "WMIPSFPDbg"
# PROP Intermediate_Dir "WMIPSFPDbg"
# PROP Target_Dir ""
CPP=clmips.exe
# ADD BASE CPP /nologo /QMFWCE /MC /W3 /GX- /Zi /Od /D _WIN32_WCE=$(CEVersion) /D "$(CEConfigName)" /D "DEBUG" /D "MIPS" /D "_MIPS_" /D UNDER_CE=$(CEVersion) /D "UNICODE" /D "_MBCS" /D "_LIB" /YX /c
# ADD CPP /nologo /QMFWCE /MC /W3 /GX- /Zi /Od /I "e:\src" /D "DEBUG" /D "MIPS" /D "_MIPS_" /D _WIN32_WCE=$(CEVersion) /D "$(CEConfigName)" /D UNDER_CE=$(CEVersion) /D "UNICODE" /D "_MBCS" /D "_LIB" /D "ASYS_WIN32" /YX /c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "WCEX86Rel"
# PROP BASE Intermediate_Dir "WCEX86Rel"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "WCEX86Rel"
# PROP Intermediate_Dir "WCEX86Rel"
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /ML /W3 /GX- /O2 /D "x86" /D "_i386_" /D "_x86_" /D _WIN32_WCE=$(CEVersion) /D "$(CEConfigName)" /D UNDER_CE=$(CEVersion) /D "UNICODE" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Gs8192 /GF   /c
# ADD CPP /nologo /ML /W3 /GX- /O2 /I "e:\src" /D "x86" /D "_i386_" /D "_x86_" /D "NDEBUG" /D _WIN32_WCE=$(CEVersion) /D "$(CEConfigName)" /D UNDER_CE=$(CEVersion) /D "UNICODE" /D "_MBCS" /D "_LIB" /D "ASYS_WIN32" /Gs8192 /GF   /c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "WCEX86Dbg"
# PROP BASE Intermediate_Dir "WCEX86Dbg"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "WCEX86Dbg"
# PROP Intermediate_Dir "WCEX86Dbg"
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /MLd /W3 /GX- /Zi /Od /D "x86" /D "_i386_" /D "_x86_" /D _WIN32_WCE=$(CEVersion) /D "$(CEConfigName)" /D UNDER_CE=$(CEVersion) /D "UNICODE" /D "DEBUG" /D "_MBCS" /D "_LIB" /Gs8192 /GF   /c
# ADD CPP /nologo /MLd /W3 /GX- /Zi /Od /I "e:\src" /D "x86" /D "_i386_" /D "_x86_" /D "DEBUG" /D _WIN32_WCE=$(CEVersion) /D "$(CEConfigName)" /D UNDER_CE=$(CEVersion) /D "UNICODE" /D "_MBCS" /D "_LIB" /D "ASYS_WIN32" /Gs8192 /GF   /c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "x86emRel"
# PROP BASE Intermediate_Dir "x86emRel"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "x86emRel"
# PROP Intermediate_Dir "x86emRel"
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /ML /W3 /GX- /O2 /D UNDER_CE=$(CEVersion) /D "UNICODE" /D "_UNICODE" /D "WIN32" /D "STRICT" /D _WIN32_WCE=$(CEVersion) /D "$(CEConfigName)" /D "_WIN32_WCE_EMULATION" /D "INTERNATIONAL" /D "USA" /D "INTLMSG_CODEPAGE" /D "NDEBUG" /D "x86" /D "i486" /D "_x86_" /D "_MBCS" /D "_LIB" /YX /c
# ADD CPP /nologo /ML /W3 /GX- /O2 /I "e:\src" /D "_UNICODE" /D "WIN32" /D "STRICT" /D "_WIN32_WCE_EMULATION" /D "INTERNATIONAL" /D "USA" /D "INTLMSG_CODEPAGE" /D "NDEBUG" /D "x86" /D "i486" /D "_x86_" /D _WIN32_WCE=$(CEVersion) /D "$(CEConfigName)" /D UNDER_CE=$(CEVersion) /D "UNICODE" /D "_MBCS" /D "_LIB" /D "ASYS_WIN32" /YX /c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "x86emDbg"
# PROP BASE Intermediate_Dir "x86emDbg"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "x86emDbg"
# PROP Intermediate_Dir "x86emDbg"
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /MLd /W3 /GX- /Z7 /Od /D UNDER_CE=$(CEVersion) /D "UNICODE" /D "_UNICODE" /D "WIN32" /D "STRICT" /D _WIN32_WCE=$(CEVersion) /D "$(CEConfigName)" /D "_WIN32_WCE_EMULATION" /D "INTERNATIONAL" /D "USA" /D "INTLMSG_CODEPAGE" /D "_DEBUG" /D "x86" /D "i486" /D "_x86_" /D "_MBCS" /D "_LIB" /YX /c
# ADD CPP /nologo /MLd /W3 /GX- /Z7 /Od /I "e:\src" /D "_UNICODE" /D "WIN32" /D "STRICT" /D "_WIN32_WCE_EMULATION" /D "INTERNATIONAL" /D "USA" /D "INTLMSG_CODEPAGE" /D "_DEBUG" /D "x86" /D "i486" /D "_x86_" /D _WIN32_WCE=$(CEVersion) /D "$(CEConfigName)" /D UNDER_CE=$(CEVersion) /D "UNICODE" /D "_MBCS" /D "_LIB" /D "ASYS_WIN32" /YX /c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "WCEPPCRel"
# PROP BASE Intermediate_Dir "WCEPPCRel"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "WCEPPCRel"
# PROP Intermediate_Dir "WCEPPCRel"
# PROP Target_Dir ""
CPP=clppc.exe
# ADD BASE CPP /nologo /M$(CECrt) /W3 /GX- /O2 /D _WIN32_WCE=$(CEVersion) /D "$(CEConfigName)" /D "NDEBUG" /D "PPC" /D "_PPC_" /D UNDER_CE=$(CEVersion) /D "UNICODE" /D "_MBCS" /D "_LIB" /YX /c
# ADD CPP /nologo /M$(CECrt) /W3 /GX- /O2 /I "e:\src" /D "NDEBUG" /D "PPC" /D "_PPC_" /D _WIN32_WCE=$(CEVersion) /D "$(CEConfigName)" /D UNDER_CE=$(CEVersion) /D "UNICODE" /D "_MBCS" /D "_LIB" /D "ASYS_WIN32" /YX /c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "WCEPPCDbg"
# PROP BASE Intermediate_Dir "WCEPPCDbg"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "WCEPPCDbg"
# PROP Intermediate_Dir "WCEPPCDbg"
# PROP Target_Dir ""
CPP=clppc.exe
# ADD BASE CPP /nologo /M$(CECrtDebug) /W3 /GX- /Zi /Od /D _WIN32_WCE=$(CEVersion) /D "$(CEConfigName)" /D "DEBUG" /D "PPC" /D "_PPC_" /D UNDER_CE=$(CEVersion) /D "UNICODE" /D "_MBCS" /D "_LIB" /YX /c
# ADD CPP /nologo /M$(CECrtDebug) /W3 /GX- /Zi /Od /I "e:\src" /D "DEBUG" /D "PPC" /D "_PPC_" /D _WIN32_WCE=$(CEVersion) /D "$(CEConfigName)" /D UNDER_CE=$(CEVersion) /D "UNICODE" /D "_MBCS" /D "_LIB" /D "ASYS_WIN32" /YX /c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "cpp_portable_static_wce - Win32 (WCE MIPS) Release"
# Name "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"
# Name "cpp_portable_static_wce - Win32 (WCE SH4) Release"
# Name "cpp_portable_static_wce - Win32 (WCE SH4) Debug"
# Name "cpp_portable_static_wce - Win32 (WCE SH3) Release"
# Name "cpp_portable_static_wce - Win32 (WCE SH3) Debug"
# Name "cpp_portable_static_wce - Win32 (WCE ARM) Release"
# Name "cpp_portable_static_wce - Win32 (WCE ARM) Debug"
# Name "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"
# Name "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"
# Name "cpp_portable_static_wce - Win32 (WCE x86) Release"
# Name "cpp_portable_static_wce - Win32 (WCE x86) Debug"
# Name "cpp_portable_static_wce - Win32 (WCE x86em) Release"
# Name "cpp_portable_static_wce - Win32 (WCE x86em) Debug"
# Name "cpp_portable_static_wce - Win32 (WCE PPC) Release"
# Name "cpp_portable_static_wce - Win32 (WCE PPC) Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\A3DDecoder.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_A3DDE=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_A3DDE=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_A3DDE=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_A3DDE=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_A3DDE=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_A3DDE=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_A3DDE=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_A3DDE=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_A3DDE=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_A3DDE=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_A3DDE=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_A3DDE=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_A3DDE=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_A3DDE=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_A3DDE=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_A3DDE=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\A3DSDecoder.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_A3DSD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\A3DSDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_A3DSD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\A3DSDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_A3DSD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\A3DSDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_A3DSD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\A3DSDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_A3DSD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\A3DSDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_A3DSD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\A3DSDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_A3DSD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\A3DSDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_A3DSD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\A3DSDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_A3DSD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\A3DSDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_A3DSD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\A3DSDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_A3DSD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\A3DSDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_A3DSD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\A3DSDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_A3DSD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\A3DSDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_A3DSD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\A3DSDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_A3DSD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\A3DSDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_A3DSD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\A3DSDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\soundlib\A8SVXDecoder.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_A8SVX=\
	"..\..\cpp\portable\soundlib\A8SVXDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_A8SVX=\
	"..\..\cpp\portable\soundlib\A8SVXDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_A8SVX=\
	"..\..\cpp\portable\soundlib\A8SVXDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_A8SVX=\
	"..\..\cpp\portable\soundlib\A8SVXDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_A8SVX=\
	"..\..\cpp\portable\soundlib\A8SVXDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_A8SVX=\
	"..\..\cpp\portable\soundlib\A8SVXDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_A8SVX=\
	"..\..\cpp\portable\soundlib\A8SVXDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_A8SVX=\
	"..\..\cpp\portable\soundlib\A8SVXDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_A8SVX=\
	"..\..\cpp\portable\soundlib\A8SVXDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_A8SVX=\
	"..\..\cpp\portable\soundlib\A8SVXDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_A8SVX=\
	"..\..\cpp\portable\soundlib\A8SVXDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_A8SVX=\
	"..\..\cpp\portable\soundlib\A8SVXDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_A8SVX=\
	"..\..\cpp\portable\soundlib\A8SVXDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_A8SVX=\
	"..\..\cpp\portable\soundlib\A8SVXDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_A8SVX=\
	"..\..\cpp\portable\soundlib\A8SVXDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_A8SVX=\
	"..\..\cpp\portable\soundlib\A8SVXDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nickcpp\AAbsRect.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_AABSR=\
	"..\..\cpp\portable\nickcpp\AAbsRect.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_AABSR=\
	"..\..\cpp\portable\nickcpp\AAbsRect.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_AABSR=\
	"..\..\cpp\portable\nickcpp\AAbsRect.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_AABSR=\
	"..\..\cpp\portable\nickcpp\AAbsRect.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_AABSR=\
	"..\..\cpp\portable\nickcpp\AAbsRect.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_AABSR=\
	"..\..\cpp\portable\nickcpp\AAbsRect.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_AABSR=\
	"..\..\cpp\portable\nickcpp\AAbsRect.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_AABSR=\
	"..\..\cpp\portable\nickcpp\AAbsRect.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_AABSR=\
	"..\..\cpp\portable\nickcpp\AAbsRect.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_AABSR=\
	"..\..\cpp\portable\nickcpp\AAbsRect.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_AABSR=\
	"..\..\cpp\portable\nickcpp\AAbsRect.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_AABSR=\
	"..\..\cpp\portable\nickcpp\AAbsRect.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_AABSR=\
	"..\..\cpp\portable\nickcpp\AAbsRect.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_AABSR=\
	"..\..\cpp\portable\nickcpp\AAbsRect.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_AABSR=\
	"..\..\cpp\portable\nickcpp\AAbsRect.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_AABSR=\
	"..\..\cpp\portable\nickcpp\AAbsRect.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\soundlib\AAUDecoder.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_AAUDE=\
	"..\..\cpp\portable\soundlib\AAUDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_AAUDE=\
	"..\..\cpp\portable\soundlib\AAUDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_AAUDE=\
	"..\..\cpp\portable\soundlib\AAUDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_AAUDE=\
	"..\..\cpp\portable\soundlib\AAUDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_AAUDE=\
	"..\..\cpp\portable\soundlib\AAUDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_AAUDE=\
	"..\..\cpp\portable\soundlib\AAUDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_AAUDE=\
	"..\..\cpp\portable\soundlib\AAUDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_AAUDE=\
	"..\..\cpp\portable\soundlib\AAUDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_AAUDE=\
	"..\..\cpp\portable\soundlib\AAUDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_AAUDE=\
	"..\..\cpp\portable\soundlib\AAUDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_AAUDE=\
	"..\..\cpp\portable\soundlib\AAUDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_AAUDE=\
	"..\..\cpp\portable\soundlib\AAUDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_AAUDE=\
	"..\..\cpp\portable\soundlib\AAUDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_AAUDE=\
	"..\..\cpp\portable\soundlib\AAUDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_AAUDE=\
	"..\..\cpp\portable\soundlib\AAUDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_AAUDE=\
	"..\..\cpp\portable\soundlib\AAUDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\soundlib\AAudioDevice.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_AAUDI=\
	"..\..\cpp\portable\soundlib\AAudioDevice.h"\
	"..\..\cpp\portable\soundlib\ASong.h"\
	"..\..\cpp\portable\soundlib\ASound.h"\
	"..\..\cpp\portable\soundlib\ASoundAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_AAUDI=\
	"..\..\cpp\portable\soundlib\AAudioDevice.h"\
	"..\..\cpp\portable\soundlib\ASong.h"\
	"..\..\cpp\portable\soundlib\ASound.h"\
	"..\..\cpp\portable\soundlib\ASoundAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_AAUDI=\
	"..\..\cpp\portable\soundlib\AAudioDevice.h"\
	"..\..\cpp\portable\soundlib\ASong.h"\
	"..\..\cpp\portable\soundlib\ASound.h"\
	"..\..\cpp\portable\soundlib\ASoundAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_AAUDI=\
	"..\..\cpp\portable\soundlib\AAudioDevice.h"\
	"..\..\cpp\portable\soundlib\ASong.h"\
	"..\..\cpp\portable\soundlib\ASound.h"\
	"..\..\cpp\portable\soundlib\ASoundAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_AAUDI=\
	"..\..\cpp\portable\soundlib\AAudioDevice.h"\
	"..\..\cpp\portable\soundlib\ASong.h"\
	"..\..\cpp\portable\soundlib\ASound.h"\
	"..\..\cpp\portable\soundlib\ASoundAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_AAUDI=\
	"..\..\cpp\portable\soundlib\AAudioDevice.h"\
	"..\..\cpp\portable\soundlib\ASong.h"\
	"..\..\cpp\portable\soundlib\ASound.h"\
	"..\..\cpp\portable\soundlib\ASoundAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_AAUDI=\
	"..\..\cpp\portable\soundlib\AAudioDevice.h"\
	"..\..\cpp\portable\soundlib\ASong.h"\
	"..\..\cpp\portable\soundlib\ASound.h"\
	"..\..\cpp\portable\soundlib\ASoundAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_AAUDI=\
	"..\..\cpp\portable\soundlib\AAudioDevice.h"\
	"..\..\cpp\portable\soundlib\ASong.h"\
	"..\..\cpp\portable\soundlib\ASound.h"\
	"..\..\cpp\portable\soundlib\ASoundAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_AAUDI=\
	"..\..\cpp\portable\soundlib\AAudioDevice.h"\
	"..\..\cpp\portable\soundlib\ASong.h"\
	"..\..\cpp\portable\soundlib\ASound.h"\
	"..\..\cpp\portable\soundlib\ASoundAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_AAUDI=\
	"..\..\cpp\portable\soundlib\AAudioDevice.h"\
	"..\..\cpp\portable\soundlib\ASong.h"\
	"..\..\cpp\portable\soundlib\ASound.h"\
	"..\..\cpp\portable\soundlib\ASoundAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_AAUDI=\
	"..\..\cpp\portable\soundlib\AAudioDevice.h"\
	"..\..\cpp\portable\soundlib\ASong.h"\
	"..\..\cpp\portable\soundlib\ASound.h"\
	"..\..\cpp\portable\soundlib\ASoundAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_AAUDI=\
	"..\..\cpp\portable\soundlib\AAudioDevice.h"\
	"..\..\cpp\portable\soundlib\ASong.h"\
	"..\..\cpp\portable\soundlib\ASound.h"\
	"..\..\cpp\portable\soundlib\ASoundAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_AAUDI=\
	"..\..\cpp\portable\soundlib\AAudioDevice.h"\
	"..\..\cpp\portable\soundlib\ASong.h"\
	"..\..\cpp\portable\soundlib\ASound.h"\
	"..\..\cpp\portable\soundlib\ASoundAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_AAUDI=\
	"..\..\cpp\portable\soundlib\AAudioDevice.h"\
	"..\..\cpp\portable\soundlib\ASong.h"\
	"..\..\cpp\portable\soundlib\ASound.h"\
	"..\..\cpp\portable\soundlib\ASoundAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_AAUDI=\
	"..\..\cpp\portable\soundlib\AAudioDevice.h"\
	"..\..\cpp\portable\soundlib\ASong.h"\
	"..\..\cpp\portable\soundlib\ASound.h"\
	"..\..\cpp\portable\soundlib\ASoundAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_AAUDI=\
	"..\..\cpp\portable\soundlib\AAudioDevice.h"\
	"..\..\cpp\portable\soundlib\ASong.h"\
	"..\..\cpp\portable\soundlib\ASound.h"\
	"..\..\cpp\portable\soundlib\ASoundAbstract.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nickcpp\ABitmap.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_ABITM=\
	"..\..\cpp\portable\nickcpp\ABitmap.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_ABITM=\
	"..\..\cpp\portable\nickcpp\ABitmap.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_ABITM=\
	"..\..\cpp\portable\nickcpp\ABitmap.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_ABITM=\
	"..\..\cpp\portable\nickcpp\ABitmap.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_ABITM=\
	"..\..\cpp\portable\nickcpp\ABitmap.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_ABITM=\
	"..\..\cpp\portable\nickcpp\ABitmap.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_ABITM=\
	"..\..\cpp\portable\nickcpp\ABitmap.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_ABITM=\
	"..\..\cpp\portable\nickcpp\ABitmap.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_ABITM=\
	"..\..\cpp\portable\nickcpp\ABitmap.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_ABITM=\
	"..\..\cpp\portable\nickcpp\ABitmap.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_ABITM=\
	"..\..\cpp\portable\nickcpp\ABitmap.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_ABITM=\
	"..\..\cpp\portable\nickcpp\ABitmap.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_ABITM=\
	"..\..\cpp\portable\nickcpp\ABitmap.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_ABITM=\
	"..\..\cpp\portable\nickcpp\ABitmap.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_ABITM=\
	"..\..\cpp\portable\nickcpp\ABitmap.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_ABITM=\
	"..\..\cpp\portable\nickcpp\ABitmap.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nickcpp\ABitmapAbstract.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_ABITMA=\
	"..\..\cpp\portable\nickcpp\ABitmapAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_ABITMA=\
	"..\..\cpp\portable\nickcpp\ABitmapAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_ABITMA=\
	"..\..\cpp\portable\nickcpp\ABitmapAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_ABITMA=\
	"..\..\cpp\portable\nickcpp\ABitmapAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_ABITMA=\
	"..\..\cpp\portable\nickcpp\ABitmapAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_ABITMA=\
	"..\..\cpp\portable\nickcpp\ABitmapAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_ABITMA=\
	"..\..\cpp\portable\nickcpp\ABitmapAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_ABITMA=\
	"..\..\cpp\portable\nickcpp\ABitmapAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_ABITMA=\
	"..\..\cpp\portable\nickcpp\ABitmapAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_ABITMA=\
	"..\..\cpp\portable\nickcpp\ABitmapAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_ABITMA=\
	"..\..\cpp\portable\nickcpp\ABitmapAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_ABITMA=\
	"..\..\cpp\portable\nickcpp\ABitmapAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_ABITMA=\
	"..\..\cpp\portable\nickcpp\ABitmapAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_ABITMA=\
	"..\..\cpp\portable\nickcpp\ABitmapAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_ABITMA=\
	"..\..\cpp\portable\nickcpp\ABitmapAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_ABITMA=\
	"..\..\cpp\portable\nickcpp\ABitmapAbstract.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\ndecoder\ABMPDecoder.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_ABMPD=\
	"..\..\cpp\portable\ndecoder\ABMPDecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_ABMPD=\
	"..\..\cpp\portable\ndecoder\ABMPDecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_ABMPD=\
	"..\..\cpp\portable\ndecoder\ABMPDecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_ABMPD=\
	"..\..\cpp\portable\ndecoder\ABMPDecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_ABMPD=\
	"..\..\cpp\portable\ndecoder\ABMPDecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_ABMPD=\
	"..\..\cpp\portable\ndecoder\ABMPDecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_ABMPD=\
	"..\..\cpp\portable\ndecoder\ABMPDecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_ABMPD=\
	"..\..\cpp\portable\ndecoder\ABMPDecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_ABMPD=\
	"..\..\cpp\portable\ndecoder\ABMPDecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_ABMPD=\
	"..\..\cpp\portable\ndecoder\ABMPDecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_ABMPD=\
	"..\..\cpp\portable\ndecoder\ABMPDecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_ABMPD=\
	"..\..\cpp\portable\ndecoder\ABMPDecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_ABMPD=\
	"..\..\cpp\portable\ndecoder\ABMPDecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_ABMPD=\
	"..\..\cpp\portable\ndecoder\ABMPDecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_ABMPD=\
	"..\..\cpp\portable\ndecoder\ABMPDecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_ABMPD=\
	"..\..\cpp\portable\ndecoder\ABMPDecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\ABspDecoder.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_ABSPD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ABspDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_ABSPD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ABspDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_ABSPD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ABspDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_ABSPD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ABspDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_ABSPD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ABspDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_ABSPD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ABspDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_ABSPD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ABspDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_ABSPD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ABspDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_ABSPD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ABspDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_ABSPD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ABspDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_ABSPD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ABspDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_ABSPD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ABspDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_ABSPD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ABspDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_ABSPD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ABspDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_ABSPD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ABspDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_ABSPD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ABspDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\ACamera3D.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_ACAME=\
	"..\..\cpp\portable\nick3d\ACamera3D.h"\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_ACAME=\
	"..\..\cpp\portable\nick3d\ACamera3D.h"\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_ACAME=\
	"..\..\cpp\portable\nick3d\ACamera3D.h"\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_ACAME=\
	"..\..\cpp\portable\nick3d\ACamera3D.h"\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_ACAME=\
	"..\..\cpp\portable\nick3d\ACamera3D.h"\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_ACAME=\
	"..\..\cpp\portable\nick3d\ACamera3D.h"\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_ACAME=\
	"..\..\cpp\portable\nick3d\ACamera3D.h"\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_ACAME=\
	"..\..\cpp\portable\nick3d\ACamera3D.h"\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_ACAME=\
	"..\..\cpp\portable\nick3d\ACamera3D.h"\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_ACAME=\
	"..\..\cpp\portable\nick3d\ACamera3D.h"\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_ACAME=\
	"..\..\cpp\portable\nick3d\ACamera3D.h"\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_ACAME=\
	"..\..\cpp\portable\nick3d\ACamera3D.h"\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_ACAME=\
	"..\..\cpp\portable\nick3d\ACamera3D.h"\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_ACAME=\
	"..\..\cpp\portable\nick3d\ACamera3D.h"\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_ACAME=\
	"..\..\cpp\portable\nick3d\ACamera3D.h"\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_ACAME=\
	"..\..\cpp\portable\nick3d\ACamera3D.h"\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nickcpp\ACell.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_ACELL=\
	"..\..\cpp\portable\nickcpp\ACell.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_ACELL=\
	"..\..\cpp\portable\nickcpp\ACell.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_ACELL=\
	"..\..\cpp\portable\nickcpp\ACell.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_ACELL=\
	"..\..\cpp\portable\nickcpp\ACell.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_ACELL=\
	"..\..\cpp\portable\nickcpp\ACell.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_ACELL=\
	"..\..\cpp\portable\nickcpp\ACell.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_ACELL=\
	"..\..\cpp\portable\nickcpp\ACell.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_ACELL=\
	"..\..\cpp\portable\nickcpp\ACell.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_ACELL=\
	"..\..\cpp\portable\nickcpp\ACell.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_ACELL=\
	"..\..\cpp\portable\nickcpp\ACell.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_ACELL=\
	"..\..\cpp\portable\nickcpp\ACell.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_ACELL=\
	"..\..\cpp\portable\nickcpp\ACell.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_ACELL=\
	"..\..\cpp\portable\nickcpp\ACell.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_ACELL=\
	"..\..\cpp\portable\nickcpp\ACell.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_ACELL=\
	"..\..\cpp\portable\nickcpp\ACell.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_ACELL=\
	"..\..\cpp\portable\nickcpp\ACell.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nickcpp\AClock.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_ACLOC=\
	"..\..\cpp\portable\nickcpp\AClock.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_ACLOC=\
	"..\..\cpp\portable\nickcpp\AClock.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_ACLOC=\
	"..\..\cpp\portable\nickcpp\AClock.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_ACLOC=\
	"..\..\cpp\portable\nickcpp\AClock.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_ACLOC=\
	"..\..\cpp\portable\nickcpp\AClock.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_ACLOC=\
	"..\..\cpp\portable\nickcpp\AClock.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_ACLOC=\
	"..\..\cpp\portable\nickcpp\AClock.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_ACLOC=\
	"..\..\cpp\portable\nickcpp\AClock.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_ACLOC=\
	"..\..\cpp\portable\nickcpp\AClock.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_ACLOC=\
	"..\..\cpp\portable\nickcpp\AClock.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_ACLOC=\
	"..\..\cpp\portable\nickcpp\AClock.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_ACLOC=\
	"..\..\cpp\portable\nickcpp\AClock.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_ACLOC=\
	"..\..\cpp\portable\nickcpp\AClock.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_ACLOC=\
	"..\..\cpp\portable\nickcpp\AClock.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_ACLOC=\
	"..\..\cpp\portable\nickcpp\AClock.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_ACLOC=\
	"..\..\cpp\portable\nickcpp\AClock.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nickcpp\AColor.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_ACOLO=\
	"..\..\cpp\portable\nickcpp\AColor.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_ACOLO=\
	"..\..\cpp\portable\nickcpp\AColor.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_ACOLO=\
	"..\..\cpp\portable\nickcpp\AColor.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_ACOLO=\
	"..\..\cpp\portable\nickcpp\AColor.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_ACOLO=\
	"..\..\cpp\portable\nickcpp\AColor.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_ACOLO=\
	"..\..\cpp\portable\nickcpp\AColor.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_ACOLO=\
	"..\..\cpp\portable\nickcpp\AColor.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_ACOLO=\
	"..\..\cpp\portable\nickcpp\AColor.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_ACOLO=\
	"..\..\cpp\portable\nickcpp\AColor.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_ACOLO=\
	"..\..\cpp\portable\nickcpp\AColor.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_ACOLO=\
	"..\..\cpp\portable\nickcpp\AColor.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_ACOLO=\
	"..\..\cpp\portable\nickcpp\AColor.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_ACOLO=\
	"..\..\cpp\portable\nickcpp\AColor.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_ACOLO=\
	"..\..\cpp\portable\nickcpp\AColor.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_ACOLO=\
	"..\..\cpp\portable\nickcpp\AColor.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_ACOLO=\
	"..\..\cpp\portable\nickcpp\AColor.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nickcpp\AConvertingBitLine.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_ACONV=\
	"..\..\cpp\portable\nickcpp\AConvertingBitLine.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_ACONV=\
	"..\..\cpp\portable\nickcpp\AConvertingBitLine.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_ACONV=\
	"..\..\cpp\portable\nickcpp\AConvertingBitLine.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_ACONV=\
	"..\..\cpp\portable\nickcpp\AConvertingBitLine.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_ACONV=\
	"..\..\cpp\portable\nickcpp\AConvertingBitLine.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_ACONV=\
	"..\..\cpp\portable\nickcpp\AConvertingBitLine.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_ACONV=\
	"..\..\cpp\portable\nickcpp\AConvertingBitLine.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_ACONV=\
	"..\..\cpp\portable\nickcpp\AConvertingBitLine.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_ACONV=\
	"..\..\cpp\portable\nickcpp\AConvertingBitLine.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_ACONV=\
	"..\..\cpp\portable\nickcpp\AConvertingBitLine.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_ACONV=\
	"..\..\cpp\portable\nickcpp\AConvertingBitLine.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_ACONV=\
	"..\..\cpp\portable\nickcpp\AConvertingBitLine.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_ACONV=\
	"..\..\cpp\portable\nickcpp\AConvertingBitLine.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_ACONV=\
	"..\..\cpp\portable\nickcpp\AConvertingBitLine.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_ACONV=\
	"..\..\cpp\portable\nickcpp\AConvertingBitLine.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_ACONV=\
	"..\..\cpp\portable\nickcpp\AConvertingBitLine.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nickcpp\ADataSourceAbstract.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_ADATA=\
	"..\..\cpp\portable\nickcpp\ADataSourceAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_ADATA=\
	"..\..\cpp\portable\nickcpp\ADataSourceAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_ADATA=\
	"..\..\cpp\portable\nickcpp\ADataSourceAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_ADATA=\
	"..\..\cpp\portable\nickcpp\ADataSourceAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_ADATA=\
	"..\..\cpp\portable\nickcpp\ADataSourceAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_ADATA=\
	"..\..\cpp\portable\nickcpp\ADataSourceAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_ADATA=\
	"..\..\cpp\portable\nickcpp\ADataSourceAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_ADATA=\
	"..\..\cpp\portable\nickcpp\ADataSourceAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_ADATA=\
	"..\..\cpp\portable\nickcpp\ADataSourceAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_ADATA=\
	"..\..\cpp\portable\nickcpp\ADataSourceAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_ADATA=\
	"..\..\cpp\portable\nickcpp\ADataSourceAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_ADATA=\
	"..\..\cpp\portable\nickcpp\ADataSourceAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_ADATA=\
	"..\..\cpp\portable\nickcpp\ADataSourceAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_ADATA=\
	"..\..\cpp\portable\nickcpp\ADataSourceAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_ADATA=\
	"..\..\cpp\portable\nickcpp\ADataSourceAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_ADATA=\
	"..\..\cpp\portable\nickcpp\ADataSourceAbstract.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\ndecoder\ADecoder.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_ADECO=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_ADECO=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_ADECO=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_ADECO=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_ADECO=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_ADECO=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_ADECO=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_ADECO=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_ADECO=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_ADECO=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_ADECO=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_ADECO=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_ADECO=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_ADECO=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_ADECO=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_ADECO=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\ndecoder\ADecoderHelper.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_ADECOD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_ADECOD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_ADECOD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_ADECOD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_ADECOD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_ADECOD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_ADECOD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_ADECOD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_ADECOD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_ADECOD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_ADECOD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_ADECOD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_ADECOD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_ADECOD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_ADECOD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_ADECOD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nickcpp\ADrawableAbstract.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\ADXFDecoder.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_ADXFD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ADXFDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_ADXFD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ADXFDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_ADXFD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ADXFDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_ADXFD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ADXFDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_ADXFD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ADXFDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_ADXFD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ADXFDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_ADXFD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ADXFDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_ADXFD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ADXFDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_ADXFD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ADXFDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_ADXFD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ADXFDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_ADXFD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ADXFDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_ADXFD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ADXFDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_ADXFD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ADXFDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_ADXFD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ADXFDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_ADXFD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ADXFDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_ADXFD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ADXFDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nickcpp\AError.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_AERRO=\
	"..\..\cpp\portable\nickcpp\AError.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_AERRO=\
	"..\..\cpp\portable\nickcpp\AError.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_AERRO=\
	"..\..\cpp\portable\nickcpp\AError.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_AERRO=\
	"..\..\cpp\portable\nickcpp\AError.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_AERRO=\
	"..\..\cpp\portable\nickcpp\AError.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_AERRO=\
	"..\..\cpp\portable\nickcpp\AError.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_AERRO=\
	"..\..\cpp\portable\nickcpp\AError.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_AERRO=\
	"..\..\cpp\portable\nickcpp\AError.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_AERRO=\
	"..\..\cpp\portable\nickcpp\AError.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_AERRO=\
	"..\..\cpp\portable\nickcpp\AError.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_AERRO=\
	"..\..\cpp\portable\nickcpp\AError.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_AERRO=\
	"..\..\cpp\portable\nickcpp\AError.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_AERRO=\
	"..\..\cpp\portable\nickcpp\AError.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_AERRO=\
	"..\..\cpp\portable\nickcpp\AError.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_AERRO=\
	"..\..\cpp\portable\nickcpp\AError.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_AERRO=\
	"..\..\cpp\portable\nickcpp\AError.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\AFace3D.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_AFACE=\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_AFACE=\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_AFACE=\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_AFACE=\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_AFACE=\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_AFACE=\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_AFACE=\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_AFACE=\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_AFACE=\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_AFACE=\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_AFACE=\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_AFACE=\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_AFACE=\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_AFACE=\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_AFACE=\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_AFACE=\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nickcpp\AFileInputStream.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_AFILE=\
	"..\..\cpp\portable\nickcpp\AFileInputStream.h"\
	"..\..\cpp\portable\nickcpp\AInputStream.h"\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_AFILE=\
	"..\..\cpp\portable\nickcpp\AFileInputStream.h"\
	"..\..\cpp\portable\nickcpp\AInputStream.h"\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_AFILE=\
	"..\..\cpp\portable\nickcpp\AFileInputStream.h"\
	"..\..\cpp\portable\nickcpp\AInputStream.h"\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_AFILE=\
	"..\..\cpp\portable\nickcpp\AFileInputStream.h"\
	"..\..\cpp\portable\nickcpp\AInputStream.h"\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_AFILE=\
	"..\..\cpp\portable\nickcpp\AFileInputStream.h"\
	"..\..\cpp\portable\nickcpp\AInputStream.h"\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_AFILE=\
	"..\..\cpp\portable\nickcpp\AFileInputStream.h"\
	"..\..\cpp\portable\nickcpp\AInputStream.h"\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_AFILE=\
	"..\..\cpp\portable\nickcpp\AFileInputStream.h"\
	"..\..\cpp\portable\nickcpp\AInputStream.h"\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_AFILE=\
	"..\..\cpp\portable\nickcpp\AFileInputStream.h"\
	"..\..\cpp\portable\nickcpp\AInputStream.h"\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_AFILE=\
	"..\..\cpp\portable\nickcpp\AFileInputStream.h"\
	"..\..\cpp\portable\nickcpp\AInputStream.h"\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_AFILE=\
	"..\..\cpp\portable\nickcpp\AFileInputStream.h"\
	"..\..\cpp\portable\nickcpp\AInputStream.h"\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_AFILE=\
	"..\..\cpp\portable\nickcpp\AFileInputStream.h"\
	"..\..\cpp\portable\nickcpp\AInputStream.h"\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_AFILE=\
	"..\..\cpp\portable\nickcpp\AFileInputStream.h"\
	"..\..\cpp\portable\nickcpp\AInputStream.h"\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_AFILE=\
	"..\..\cpp\portable\nickcpp\AFileInputStream.h"\
	"..\..\cpp\portable\nickcpp\AInputStream.h"\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_AFILE=\
	"..\..\cpp\portable\nickcpp\AFileInputStream.h"\
	"..\..\cpp\portable\nickcpp\AInputStream.h"\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_AFILE=\
	"..\..\cpp\portable\nickcpp\AFileInputStream.h"\
	"..\..\cpp\portable\nickcpp\AInputStream.h"\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_AFILE=\
	"..\..\cpp\portable\nickcpp\AFileInputStream.h"\
	"..\..\cpp\portable\nickcpp\AInputStream.h"\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nickcpp\AFileOutputStream.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_AFILEO=\
	"..\..\cpp\portable\nickcpp\AFileOutputStream.h"\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	"..\..\cpp\portable\nickcpp\AOutputStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_AFILEO=\
	"..\..\cpp\portable\nickcpp\AFileOutputStream.h"\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	"..\..\cpp\portable\nickcpp\AOutputStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_AFILEO=\
	"..\..\cpp\portable\nickcpp\AFileOutputStream.h"\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	"..\..\cpp\portable\nickcpp\AOutputStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_AFILEO=\
	"..\..\cpp\portable\nickcpp\AFileOutputStream.h"\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	"..\..\cpp\portable\nickcpp\AOutputStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_AFILEO=\
	"..\..\cpp\portable\nickcpp\AFileOutputStream.h"\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	"..\..\cpp\portable\nickcpp\AOutputStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_AFILEO=\
	"..\..\cpp\portable\nickcpp\AFileOutputStream.h"\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	"..\..\cpp\portable\nickcpp\AOutputStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_AFILEO=\
	"..\..\cpp\portable\nickcpp\AFileOutputStream.h"\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	"..\..\cpp\portable\nickcpp\AOutputStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_AFILEO=\
	"..\..\cpp\portable\nickcpp\AFileOutputStream.h"\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	"..\..\cpp\portable\nickcpp\AOutputStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_AFILEO=\
	"..\..\cpp\portable\nickcpp\AFileOutputStream.h"\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	"..\..\cpp\portable\nickcpp\AOutputStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_AFILEO=\
	"..\..\cpp\portable\nickcpp\AFileOutputStream.h"\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	"..\..\cpp\portable\nickcpp\AOutputStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_AFILEO=\
	"..\..\cpp\portable\nickcpp\AFileOutputStream.h"\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	"..\..\cpp\portable\nickcpp\AOutputStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_AFILEO=\
	"..\..\cpp\portable\nickcpp\AFileOutputStream.h"\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	"..\..\cpp\portable\nickcpp\AOutputStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_AFILEO=\
	"..\..\cpp\portable\nickcpp\AFileOutputStream.h"\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	"..\..\cpp\portable\nickcpp\AOutputStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_AFILEO=\
	"..\..\cpp\portable\nickcpp\AFileOutputStream.h"\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	"..\..\cpp\portable\nickcpp\AOutputStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_AFILEO=\
	"..\..\cpp\portable\nickcpp\AFileOutputStream.h"\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	"..\..\cpp\portable\nickcpp\AOutputStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_AFILEO=\
	"..\..\cpp\portable\nickcpp\AFileOutputStream.h"\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	"..\..\cpp\portable\nickcpp\AOutputStream.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nickcpp\AFrameAbstract.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_AFRAM=\
	"..\..\cpp\portable\nickcpp\AError.h"\
	"..\..\cpp\portable\nickcpp\AFrameAbstract.h"\
	"..\..\cpp\portable\nickcpp\APoint.h"\
	"..\..\cpp\portable\nickcpp\ARect.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_AFRAM=\
	"..\..\cpp\portable\nickcpp\AError.h"\
	"..\..\cpp\portable\nickcpp\AFrameAbstract.h"\
	"..\..\cpp\portable\nickcpp\APoint.h"\
	"..\..\cpp\portable\nickcpp\ARect.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_AFRAM=\
	"..\..\cpp\portable\nickcpp\AError.h"\
	"..\..\cpp\portable\nickcpp\AFrameAbstract.h"\
	"..\..\cpp\portable\nickcpp\APoint.h"\
	"..\..\cpp\portable\nickcpp\ARect.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_AFRAM=\
	"..\..\cpp\portable\nickcpp\AError.h"\
	"..\..\cpp\portable\nickcpp\AFrameAbstract.h"\
	"..\..\cpp\portable\nickcpp\APoint.h"\
	"..\..\cpp\portable\nickcpp\ARect.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_AFRAM=\
	"..\..\cpp\portable\nickcpp\AError.h"\
	"..\..\cpp\portable\nickcpp\AFrameAbstract.h"\
	"..\..\cpp\portable\nickcpp\APoint.h"\
	"..\..\cpp\portable\nickcpp\ARect.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_AFRAM=\
	"..\..\cpp\portable\nickcpp\AError.h"\
	"..\..\cpp\portable\nickcpp\AFrameAbstract.h"\
	"..\..\cpp\portable\nickcpp\APoint.h"\
	"..\..\cpp\portable\nickcpp\ARect.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_AFRAM=\
	"..\..\cpp\portable\nickcpp\AError.h"\
	"..\..\cpp\portable\nickcpp\AFrameAbstract.h"\
	"..\..\cpp\portable\nickcpp\APoint.h"\
	"..\..\cpp\portable\nickcpp\ARect.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_AFRAM=\
	"..\..\cpp\portable\nickcpp\AError.h"\
	"..\..\cpp\portable\nickcpp\AFrameAbstract.h"\
	"..\..\cpp\portable\nickcpp\APoint.h"\
	"..\..\cpp\portable\nickcpp\ARect.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_AFRAM=\
	"..\..\cpp\portable\nickcpp\AError.h"\
	"..\..\cpp\portable\nickcpp\AFrameAbstract.h"\
	"..\..\cpp\portable\nickcpp\APoint.h"\
	"..\..\cpp\portable\nickcpp\ARect.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_AFRAM=\
	"..\..\cpp\portable\nickcpp\AError.h"\
	"..\..\cpp\portable\nickcpp\AFrameAbstract.h"\
	"..\..\cpp\portable\nickcpp\APoint.h"\
	"..\..\cpp\portable\nickcpp\ARect.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_AFRAM=\
	"..\..\cpp\portable\nickcpp\AError.h"\
	"..\..\cpp\portable\nickcpp\AFrameAbstract.h"\
	"..\..\cpp\portable\nickcpp\APoint.h"\
	"..\..\cpp\portable\nickcpp\ARect.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_AFRAM=\
	"..\..\cpp\portable\nickcpp\AError.h"\
	"..\..\cpp\portable\nickcpp\AFrameAbstract.h"\
	"..\..\cpp\portable\nickcpp\APoint.h"\
	"..\..\cpp\portable\nickcpp\ARect.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_AFRAM=\
	"..\..\cpp\portable\nickcpp\AError.h"\
	"..\..\cpp\portable\nickcpp\AFrameAbstract.h"\
	"..\..\cpp\portable\nickcpp\APoint.h"\
	"..\..\cpp\portable\nickcpp\ARect.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_AFRAM=\
	"..\..\cpp\portable\nickcpp\AError.h"\
	"..\..\cpp\portable\nickcpp\AFrameAbstract.h"\
	"..\..\cpp\portable\nickcpp\APoint.h"\
	"..\..\cpp\portable\nickcpp\ARect.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_AFRAM=\
	"..\..\cpp\portable\nickcpp\AError.h"\
	"..\..\cpp\portable\nickcpp\AFrameAbstract.h"\
	"..\..\cpp\portable\nickcpp\APoint.h"\
	"..\..\cpp\portable\nickcpp\ARect.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_AFRAM=\
	"..\..\cpp\portable\nickcpp\AError.h"\
	"..\..\cpp\portable\nickcpp\AFrameAbstract.h"\
	"..\..\cpp\portable\nickcpp\APoint.h"\
	"..\..\cpp\portable\nickcpp\ARect.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nickcpp\AGenericNode.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_AGENE=\
	"..\..\cpp\portable\nickcpp\AGenericNode.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_AGENE=\
	"..\..\cpp\portable\nickcpp\AGenericNode.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_AGENE=\
	"..\..\cpp\portable\nickcpp\AGenericNode.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_AGENE=\
	"..\..\cpp\portable\nickcpp\AGenericNode.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_AGENE=\
	"..\..\cpp\portable\nickcpp\AGenericNode.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_AGENE=\
	"..\..\cpp\portable\nickcpp\AGenericNode.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_AGENE=\
	"..\..\cpp\portable\nickcpp\AGenericNode.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_AGENE=\
	"..\..\cpp\portable\nickcpp\AGenericNode.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_AGENE=\
	"..\..\cpp\portable\nickcpp\AGenericNode.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_AGENE=\
	"..\..\cpp\portable\nickcpp\AGenericNode.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_AGENE=\
	"..\..\cpp\portable\nickcpp\AGenericNode.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_AGENE=\
	"..\..\cpp\portable\nickcpp\AGenericNode.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_AGENE=\
	"..\..\cpp\portable\nickcpp\AGenericNode.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_AGENE=\
	"..\..\cpp\portable\nickcpp\AGenericNode.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_AGENE=\
	"..\..\cpp\portable\nickcpp\AGenericNode.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_AGENE=\
	"..\..\cpp\portable\nickcpp\AGenericNode.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\ndecoder\AGIFDecoder.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_AGIFD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AGIFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_AGIFD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AGIFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_AGIFD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AGIFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_AGIFD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AGIFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_AGIFD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AGIFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_AGIFD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AGIFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_AGIFD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AGIFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_AGIFD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AGIFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_AGIFD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AGIFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_AGIFD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AGIFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_AGIFD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AGIFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_AGIFD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AGIFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_AGIFD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AGIFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_AGIFD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AGIFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_AGIFD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AGIFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_AGIFD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AGIFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\AHalfLifeDecoder.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_AHALF=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AHalfLifeDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_AHALF=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AHalfLifeDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_AHALF=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AHalfLifeDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_AHALF=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AHalfLifeDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_AHALF=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AHalfLifeDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_AHALF=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AHalfLifeDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_AHALF=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AHalfLifeDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_AHALF=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AHalfLifeDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_AHALF=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AHalfLifeDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_AHALF=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AHalfLifeDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_AHALF=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AHalfLifeDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_AHALF=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AHalfLifeDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_AHALF=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AHalfLifeDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_AHALF=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AHalfLifeDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_AHALF=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AHalfLifeDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_AHALF=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AHalfLifeDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\netlib\AHost.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_AHOST=\
	"..\..\cpp\portable\netlib\AHost.h"\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	"..\..\cpp\portable\netlib\ANetwork.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_AHOST=\
	"..\..\cpp\portable\netlib\AHost.h"\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	"..\..\cpp\portable\netlib\ANetwork.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_AHOST=\
	"..\..\cpp\portable\netlib\AHost.h"\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	"..\..\cpp\portable\netlib\ANetwork.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_AHOST=\
	"..\..\cpp\portable\netlib\AHost.h"\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	"..\..\cpp\portable\netlib\ANetwork.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_AHOST=\
	"..\..\cpp\portable\netlib\AHost.h"\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	"..\..\cpp\portable\netlib\ANetwork.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_AHOST=\
	"..\..\cpp\portable\netlib\AHost.h"\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	"..\..\cpp\portable\netlib\ANetwork.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_AHOST=\
	"..\..\cpp\portable\netlib\AHost.h"\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	"..\..\cpp\portable\netlib\ANetwork.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_AHOST=\
	"..\..\cpp\portable\netlib\AHost.h"\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	"..\..\cpp\portable\netlib\ANetwork.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_AHOST=\
	"..\..\cpp\portable\netlib\AHost.h"\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	"..\..\cpp\portable\netlib\ANetwork.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_AHOST=\
	"..\..\cpp\portable\netlib\AHost.h"\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	"..\..\cpp\portable\netlib\ANetwork.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_AHOST=\
	"..\..\cpp\portable\netlib\AHost.h"\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	"..\..\cpp\portable\netlib\ANetwork.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_AHOST=\
	"..\..\cpp\portable\netlib\AHost.h"\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	"..\..\cpp\portable\netlib\ANetwork.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_AHOST=\
	"..\..\cpp\portable\netlib\AHost.h"\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	"..\..\cpp\portable\netlib\ANetwork.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_AHOST=\
	"..\..\cpp\portable\netlib\AHost.h"\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	"..\..\cpp\portable\netlib\ANetwork.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_AHOST=\
	"..\..\cpp\portable\netlib\AHost.h"\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	"..\..\cpp\portable\netlib\ANetwork.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_AHOST=\
	"..\..\cpp\portable\netlib\AHost.h"\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	"..\..\cpp\portable\netlib\ANetwork.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\ndecoder\AIFFDecoder.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_AIFFD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AIFFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_AIFFD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AIFFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_AIFFD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AIFFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_AIFFD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AIFFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_AIFFD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AIFFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_AIFFD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AIFFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_AIFFD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AIFFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_AIFFD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AIFFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_AIFFD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AIFFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_AIFFD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AIFFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_AIFFD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AIFFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_AIFFD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AIFFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_AIFFD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AIFFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_AIFFD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AIFFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_AIFFD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AIFFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_AIFFD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AIFFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\ndecoder\AImageDecoder.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_AIMAG=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_AIMAG=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_AIMAG=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_AIMAG=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_AIMAG=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_AIMAG=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_AIMAG=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_AIMAG=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_AIMAG=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_AIMAG=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_AIMAG=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_AIMAG=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_AIMAG=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_AIMAG=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_AIMAG=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_AIMAG=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nickcpp\AInputStream.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_AINPU=\
	"..\..\cpp\portable\nickcpp\AInputStream.h"\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_AINPU=\
	"..\..\cpp\portable\nickcpp\AInputStream.h"\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_AINPU=\
	"..\..\cpp\portable\nickcpp\AInputStream.h"\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_AINPU=\
	"..\..\cpp\portable\nickcpp\AInputStream.h"\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_AINPU=\
	"..\..\cpp\portable\nickcpp\AInputStream.h"\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_AINPU=\
	"..\..\cpp\portable\nickcpp\AInputStream.h"\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_AINPU=\
	"..\..\cpp\portable\nickcpp\AInputStream.h"\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_AINPU=\
	"..\..\cpp\portable\nickcpp\AInputStream.h"\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_AINPU=\
	"..\..\cpp\portable\nickcpp\AInputStream.h"\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_AINPU=\
	"..\..\cpp\portable\nickcpp\AInputStream.h"\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_AINPU=\
	"..\..\cpp\portable\nickcpp\AInputStream.h"\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_AINPU=\
	"..\..\cpp\portable\nickcpp\AInputStream.h"\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_AINPU=\
	"..\..\cpp\portable\nickcpp\AInputStream.h"\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_AINPU=\
	"..\..\cpp\portable\nickcpp\AInputStream.h"\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_AINPU=\
	"..\..\cpp\portable\nickcpp\AInputStream.h"\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_AINPU=\
	"..\..\cpp\portable\nickcpp\AInputStream.h"\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\ndecoder\AJFIFDecoder.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_AJFIF=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AJFIFDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_AJFIF=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AJFIFDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_AJFIF=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AJFIFDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_AJFIF=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AJFIFDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_AJFIF=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AJFIFDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_AJFIF=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AJFIFDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_AJFIF=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AJFIFDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_AJFIF=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AJFIFDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_AJFIF=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AJFIFDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_AJFIF=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AJFIFDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_AJFIF=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AJFIFDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_AJFIF=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AJFIFDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_AJFIF=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AJFIFDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_AJFIF=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AJFIFDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_AJFIF=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AJFIFDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_AJFIF=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AJFIFDecoder.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\ALight3D.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_ALIGH=\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\ALight3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_ALIGH=\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\ALight3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_ALIGH=\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\ALight3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_ALIGH=\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\ALight3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_ALIGH=\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\ALight3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_ALIGH=\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\ALight3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_ALIGH=\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\ALight3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_ALIGH=\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\ALight3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_ALIGH=\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\ALight3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_ALIGH=\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\ALight3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_ALIGH=\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\ALight3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_ALIGH=\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\ALight3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_ALIGH=\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\ALight3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_ALIGH=\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\ALight3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_ALIGH=\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\ALight3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_ALIGH=\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\ALight3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\ALightwaveDecoder.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_ALIGHT=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ALightwaveDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_ALIGHT=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ALightwaveDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_ALIGHT=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ALightwaveDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_ALIGHT=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ALightwaveDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_ALIGHT=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ALightwaveDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_ALIGHT=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ALightwaveDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_ALIGHT=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ALightwaveDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_ALIGHT=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ALightwaveDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_ALIGHT=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ALightwaveDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_ALIGHT=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ALightwaveDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_ALIGHT=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ALightwaveDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_ALIGHT=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ALightwaveDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_ALIGHT=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ALightwaveDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_ALIGHT=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ALightwaveDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_ALIGHT=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ALightwaveDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_ALIGHT=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ALightwaveDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nickcpp\AList.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_ALIST=\
	"..\..\cpp\portable\nickcpp\ACell.h"\
	"..\..\cpp\portable\nickcpp\ADataSourceAbstract.h"\
	"..\..\cpp\portable\nickcpp\AList.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_ALIST=\
	"..\..\cpp\portable\nickcpp\ACell.h"\
	"..\..\cpp\portable\nickcpp\ADataSourceAbstract.h"\
	"..\..\cpp\portable\nickcpp\AList.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_ALIST=\
	"..\..\cpp\portable\nickcpp\ACell.h"\
	"..\..\cpp\portable\nickcpp\ADataSourceAbstract.h"\
	"..\..\cpp\portable\nickcpp\AList.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_ALIST=\
	"..\..\cpp\portable\nickcpp\ACell.h"\
	"..\..\cpp\portable\nickcpp\ADataSourceAbstract.h"\
	"..\..\cpp\portable\nickcpp\AList.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_ALIST=\
	"..\..\cpp\portable\nickcpp\ACell.h"\
	"..\..\cpp\portable\nickcpp\ADataSourceAbstract.h"\
	"..\..\cpp\portable\nickcpp\AList.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_ALIST=\
	"..\..\cpp\portable\nickcpp\ACell.h"\
	"..\..\cpp\portable\nickcpp\ADataSourceAbstract.h"\
	"..\..\cpp\portable\nickcpp\AList.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_ALIST=\
	"..\..\cpp\portable\nickcpp\ACell.h"\
	"..\..\cpp\portable\nickcpp\ADataSourceAbstract.h"\
	"..\..\cpp\portable\nickcpp\AList.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_ALIST=\
	"..\..\cpp\portable\nickcpp\ACell.h"\
	"..\..\cpp\portable\nickcpp\ADataSourceAbstract.h"\
	"..\..\cpp\portable\nickcpp\AList.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_ALIST=\
	"..\..\cpp\portable\nickcpp\ACell.h"\
	"..\..\cpp\portable\nickcpp\ADataSourceAbstract.h"\
	"..\..\cpp\portable\nickcpp\AList.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_ALIST=\
	"..\..\cpp\portable\nickcpp\ACell.h"\
	"..\..\cpp\portable\nickcpp\ADataSourceAbstract.h"\
	"..\..\cpp\portable\nickcpp\AList.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_ALIST=\
	"..\..\cpp\portable\nickcpp\ACell.h"\
	"..\..\cpp\portable\nickcpp\ADataSourceAbstract.h"\
	"..\..\cpp\portable\nickcpp\AList.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_ALIST=\
	"..\..\cpp\portable\nickcpp\ACell.h"\
	"..\..\cpp\portable\nickcpp\ADataSourceAbstract.h"\
	"..\..\cpp\portable\nickcpp\AList.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_ALIST=\
	"..\..\cpp\portable\nickcpp\ACell.h"\
	"..\..\cpp\portable\nickcpp\ADataSourceAbstract.h"\
	"..\..\cpp\portable\nickcpp\AList.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_ALIST=\
	"..\..\cpp\portable\nickcpp\ACell.h"\
	"..\..\cpp\portable\nickcpp\ADataSourceAbstract.h"\
	"..\..\cpp\portable\nickcpp\AList.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_ALIST=\
	"..\..\cpp\portable\nickcpp\ACell.h"\
	"..\..\cpp\portable\nickcpp\ADataSourceAbstract.h"\
	"..\..\cpp\portable\nickcpp\AList.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_ALIST=\
	"..\..\cpp\portable\nickcpp\ACell.h"\
	"..\..\cpp\portable\nickcpp\ADataSourceAbstract.h"\
	"..\..\cpp\portable\nickcpp\AList.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\AMapDecoder.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_AMAPD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMapDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_AMAPD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMapDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_AMAPD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMapDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_AMAPD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMapDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_AMAPD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMapDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_AMAPD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMapDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_AMAPD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMapDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_AMAPD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMapDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_AMAPD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMapDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_AMAPD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMapDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_AMAPD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMapDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_AMAPD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMapDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_AMAPD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMapDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_AMAPD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMapDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_AMAPD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMapDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_AMAPD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMapDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\AMatrix3D.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_AMATR=\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_AMATR=\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_AMATR=\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_AMATR=\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_AMATR=\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_AMATR=\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_AMATR=\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_AMATR=\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_AMATR=\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_AMATR=\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_AMATR=\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_AMATR=\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_AMATR=\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_AMATR=\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_AMATR=\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_AMATR=\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\AMayaDecoder.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_AMAYA=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMayaDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_AMAYA=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMayaDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_AMAYA=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMayaDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_AMAYA=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMayaDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_AMAYA=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMayaDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_AMAYA=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMayaDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_AMAYA=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMayaDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_AMAYA=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMayaDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_AMAYA=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMayaDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_AMAYA=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMayaDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_AMAYA=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMayaDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_AMAYA=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMayaDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_AMAYA=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMayaDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_AMAYA=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMayaDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_AMAYA=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMayaDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_AMAYA=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMayaDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\AMD2Decoder.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_AMD2D=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMD2Decoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_AMD2D=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMD2Decoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_AMD2D=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMD2Decoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_AMD2D=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMD2Decoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_AMD2D=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMD2Decoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_AMD2D=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMD2Decoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_AMD2D=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMD2Decoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_AMD2D=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMD2Decoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_AMD2D=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMD2Decoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_AMD2D=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMD2Decoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_AMD2D=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMD2Decoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_AMD2D=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMD2Decoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_AMD2D=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMD2Decoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_AMD2D=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMD2Decoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_AMD2D=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMD2Decoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_AMD2D=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMD2Decoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\AMD3Decoder.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_AMD3D=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMD3Decoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_AMD3D=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMD3Decoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_AMD3D=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMD3Decoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_AMD3D=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMD3Decoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_AMD3D=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMD3Decoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_AMD3D=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMD3Decoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_AMD3D=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMD3Decoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_AMD3D=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMD3Decoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_AMD3D=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMD3Decoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_AMD3D=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMD3Decoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_AMD3D=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMD3Decoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_AMD3D=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMD3Decoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_AMD3D=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMD3Decoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_AMD3D=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMD3Decoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_AMD3D=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMD3Decoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_AMD3D=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMD3Decoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\AMD5Decoder.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_AMD5D=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMD5Decoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_AMD5D=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMD5Decoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_AMD5D=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMD5Decoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_AMD5D=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMD5Decoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_AMD5D=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMD5Decoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_AMD5D=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMD5Decoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_AMD5D=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMD5Decoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_AMD5D=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMD5Decoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_AMD5D=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMD5Decoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_AMD5D=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMD5Decoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_AMD5D=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMD5Decoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_AMD5D=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMD5Decoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_AMD5D=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMD5Decoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_AMD5D=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMD5Decoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_AMD5D=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMD5Decoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_AMD5D=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMD5Decoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\AMDLDecoder.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_AMDLD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMDLDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_AMDLD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMDLDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_AMDLD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMDLDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_AMDLD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMDLDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_AMDLD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMDLDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_AMDLD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMDLDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_AMDLD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMDLDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_AMDLD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMDLDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_AMDLD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMDLDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_AMDLD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMDLDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_AMDLD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMDLDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_AMDLD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMDLDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_AMDLD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMDLDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_AMDLD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMDLDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_AMDLD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMDLDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_AMDLD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMDLDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\AMesh3D.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_AMESH=\
	"..\..\cpp\portable\nick3d\ACamera3D.h"\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AMesh3D.h"\
	"..\..\cpp\portable\nick3d\APart3D.h"\
	"..\..\cpp\portable\nick3d\ATextCoord3D.h"\
	"..\..\cpp\portable\nick3d\ATexture3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_AMESH=\
	"..\..\cpp\portable\nick3d\ACamera3D.h"\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AMesh3D.h"\
	"..\..\cpp\portable\nick3d\APart3D.h"\
	"..\..\cpp\portable\nick3d\ATextCoord3D.h"\
	"..\..\cpp\portable\nick3d\ATexture3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_AMESH=\
	"..\..\cpp\portable\nick3d\ACamera3D.h"\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AMesh3D.h"\
	"..\..\cpp\portable\nick3d\APart3D.h"\
	"..\..\cpp\portable\nick3d\ATextCoord3D.h"\
	"..\..\cpp\portable\nick3d\ATexture3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_AMESH=\
	"..\..\cpp\portable\nick3d\ACamera3D.h"\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AMesh3D.h"\
	"..\..\cpp\portable\nick3d\APart3D.h"\
	"..\..\cpp\portable\nick3d\ATextCoord3D.h"\
	"..\..\cpp\portable\nick3d\ATexture3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_AMESH=\
	"..\..\cpp\portable\nick3d\ACamera3D.h"\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AMesh3D.h"\
	"..\..\cpp\portable\nick3d\APart3D.h"\
	"..\..\cpp\portable\nick3d\ATextCoord3D.h"\
	"..\..\cpp\portable\nick3d\ATexture3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_AMESH=\
	"..\..\cpp\portable\nick3d\ACamera3D.h"\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AMesh3D.h"\
	"..\..\cpp\portable\nick3d\APart3D.h"\
	"..\..\cpp\portable\nick3d\ATextCoord3D.h"\
	"..\..\cpp\portable\nick3d\ATexture3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_AMESH=\
	"..\..\cpp\portable\nick3d\ACamera3D.h"\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AMesh3D.h"\
	"..\..\cpp\portable\nick3d\APart3D.h"\
	"..\..\cpp\portable\nick3d\ATextCoord3D.h"\
	"..\..\cpp\portable\nick3d\ATexture3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_AMESH=\
	"..\..\cpp\portable\nick3d\ACamera3D.h"\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AMesh3D.h"\
	"..\..\cpp\portable\nick3d\APart3D.h"\
	"..\..\cpp\portable\nick3d\ATextCoord3D.h"\
	"..\..\cpp\portable\nick3d\ATexture3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_AMESH=\
	"..\..\cpp\portable\nick3d\ACamera3D.h"\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AMesh3D.h"\
	"..\..\cpp\portable\nick3d\APart3D.h"\
	"..\..\cpp\portable\nick3d\ATextCoord3D.h"\
	"..\..\cpp\portable\nick3d\ATexture3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_AMESH=\
	"..\..\cpp\portable\nick3d\ACamera3D.h"\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AMesh3D.h"\
	"..\..\cpp\portable\nick3d\APart3D.h"\
	"..\..\cpp\portable\nick3d\ATextCoord3D.h"\
	"..\..\cpp\portable\nick3d\ATexture3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_AMESH=\
	"..\..\cpp\portable\nick3d\ACamera3D.h"\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AMesh3D.h"\
	"..\..\cpp\portable\nick3d\APart3D.h"\
	"..\..\cpp\portable\nick3d\ATextCoord3D.h"\
	"..\..\cpp\portable\nick3d\ATexture3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_AMESH=\
	"..\..\cpp\portable\nick3d\ACamera3D.h"\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AMesh3D.h"\
	"..\..\cpp\portable\nick3d\APart3D.h"\
	"..\..\cpp\portable\nick3d\ATextCoord3D.h"\
	"..\..\cpp\portable\nick3d\ATexture3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_AMESH=\
	"..\..\cpp\portable\nick3d\ACamera3D.h"\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AMesh3D.h"\
	"..\..\cpp\portable\nick3d\APart3D.h"\
	"..\..\cpp\portable\nick3d\ATextCoord3D.h"\
	"..\..\cpp\portable\nick3d\ATexture3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_AMESH=\
	"..\..\cpp\portable\nick3d\ACamera3D.h"\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AMesh3D.h"\
	"..\..\cpp\portable\nick3d\APart3D.h"\
	"..\..\cpp\portable\nick3d\ATextCoord3D.h"\
	"..\..\cpp\portable\nick3d\ATexture3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_AMESH=\
	"..\..\cpp\portable\nick3d\ACamera3D.h"\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AMesh3D.h"\
	"..\..\cpp\portable\nick3d\APart3D.h"\
	"..\..\cpp\portable\nick3d\ATextCoord3D.h"\
	"..\..\cpp\portable\nick3d\ATexture3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_AMESH=\
	"..\..\cpp\portable\nick3d\ACamera3D.h"\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AMesh3D.h"\
	"..\..\cpp\portable\nick3d\APart3D.h"\
	"..\..\cpp\portable\nick3d\ATextCoord3D.h"\
	"..\..\cpp\portable\nick3d\ATexture3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\AMeshDecoder.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_AMESHD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_AMESHD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_AMESHD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_AMESHD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_AMESHD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_AMESHD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_AMESHD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_AMESHD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_AMESHD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_AMESHD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_AMESHD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_AMESHD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_AMESHD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_AMESHD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_AMESHD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_AMESHD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\soundlib\AMidiDecoder.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_AMIDI=\
	"..\..\cpp\portable\soundlib\AMidiDecoder.h"\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_AMIDI=\
	"..\..\cpp\portable\soundlib\AMidiDecoder.h"\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_AMIDI=\
	"..\..\cpp\portable\soundlib\AMidiDecoder.h"\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_AMIDI=\
	"..\..\cpp\portable\soundlib\AMidiDecoder.h"\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_AMIDI=\
	"..\..\cpp\portable\soundlib\AMidiDecoder.h"\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_AMIDI=\
	"..\..\cpp\portable\soundlib\AMidiDecoder.h"\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_AMIDI=\
	"..\..\cpp\portable\soundlib\AMidiDecoder.h"\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_AMIDI=\
	"..\..\cpp\portable\soundlib\AMidiDecoder.h"\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_AMIDI=\
	"..\..\cpp\portable\soundlib\AMidiDecoder.h"\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_AMIDI=\
	"..\..\cpp\portable\soundlib\AMidiDecoder.h"\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_AMIDI=\
	"..\..\cpp\portable\soundlib\AMidiDecoder.h"\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_AMIDI=\
	"..\..\cpp\portable\soundlib\AMidiDecoder.h"\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_AMIDI=\
	"..\..\cpp\portable\soundlib\AMidiDecoder.h"\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_AMIDI=\
	"..\..\cpp\portable\soundlib\AMidiDecoder.h"\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_AMIDI=\
	"..\..\cpp\portable\soundlib\AMidiDecoder.h"\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_AMIDI=\
	"..\..\cpp\portable\soundlib\AMidiDecoder.h"\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nickcpp\AMovie.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_AMOVI=\
	"..\..\cpp\portable\nickcpp\AError.h"\
	"..\..\cpp\portable\nickcpp\AFrameAbstract.h"\
	"..\..\cpp\portable\nickcpp\AMovie.h"\
	"..\..\cpp\portable\nickcpp\APoint.h"\
	"..\..\cpp\portable\nickcpp\ARect.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_AMOVI=\
	"..\..\cpp\portable\nickcpp\AError.h"\
	"..\..\cpp\portable\nickcpp\AFrameAbstract.h"\
	"..\..\cpp\portable\nickcpp\AMovie.h"\
	"..\..\cpp\portable\nickcpp\APoint.h"\
	"..\..\cpp\portable\nickcpp\ARect.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_AMOVI=\
	"..\..\cpp\portable\nickcpp\AError.h"\
	"..\..\cpp\portable\nickcpp\AFrameAbstract.h"\
	"..\..\cpp\portable\nickcpp\AMovie.h"\
	"..\..\cpp\portable\nickcpp\APoint.h"\
	"..\..\cpp\portable\nickcpp\ARect.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_AMOVI=\
	"..\..\cpp\portable\nickcpp\AError.h"\
	"..\..\cpp\portable\nickcpp\AFrameAbstract.h"\
	"..\..\cpp\portable\nickcpp\AMovie.h"\
	"..\..\cpp\portable\nickcpp\APoint.h"\
	"..\..\cpp\portable\nickcpp\ARect.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_AMOVI=\
	"..\..\cpp\portable\nickcpp\AError.h"\
	"..\..\cpp\portable\nickcpp\AFrameAbstract.h"\
	"..\..\cpp\portable\nickcpp\AMovie.h"\
	"..\..\cpp\portable\nickcpp\APoint.h"\
	"..\..\cpp\portable\nickcpp\ARect.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_AMOVI=\
	"..\..\cpp\portable\nickcpp\AError.h"\
	"..\..\cpp\portable\nickcpp\AFrameAbstract.h"\
	"..\..\cpp\portable\nickcpp\AMovie.h"\
	"..\..\cpp\portable\nickcpp\APoint.h"\
	"..\..\cpp\portable\nickcpp\ARect.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_AMOVI=\
	"..\..\cpp\portable\nickcpp\AError.h"\
	"..\..\cpp\portable\nickcpp\AFrameAbstract.h"\
	"..\..\cpp\portable\nickcpp\AMovie.h"\
	"..\..\cpp\portable\nickcpp\APoint.h"\
	"..\..\cpp\portable\nickcpp\ARect.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_AMOVI=\
	"..\..\cpp\portable\nickcpp\AError.h"\
	"..\..\cpp\portable\nickcpp\AFrameAbstract.h"\
	"..\..\cpp\portable\nickcpp\AMovie.h"\
	"..\..\cpp\portable\nickcpp\APoint.h"\
	"..\..\cpp\portable\nickcpp\ARect.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_AMOVI=\
	"..\..\cpp\portable\nickcpp\AError.h"\
	"..\..\cpp\portable\nickcpp\AFrameAbstract.h"\
	"..\..\cpp\portable\nickcpp\AMovie.h"\
	"..\..\cpp\portable\nickcpp\APoint.h"\
	"..\..\cpp\portable\nickcpp\ARect.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_AMOVI=\
	"..\..\cpp\portable\nickcpp\AError.h"\
	"..\..\cpp\portable\nickcpp\AFrameAbstract.h"\
	"..\..\cpp\portable\nickcpp\AMovie.h"\
	"..\..\cpp\portable\nickcpp\APoint.h"\
	"..\..\cpp\portable\nickcpp\ARect.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_AMOVI=\
	"..\..\cpp\portable\nickcpp\AError.h"\
	"..\..\cpp\portable\nickcpp\AFrameAbstract.h"\
	"..\..\cpp\portable\nickcpp\AMovie.h"\
	"..\..\cpp\portable\nickcpp\APoint.h"\
	"..\..\cpp\portable\nickcpp\ARect.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_AMOVI=\
	"..\..\cpp\portable\nickcpp\AError.h"\
	"..\..\cpp\portable\nickcpp\AFrameAbstract.h"\
	"..\..\cpp\portable\nickcpp\AMovie.h"\
	"..\..\cpp\portable\nickcpp\APoint.h"\
	"..\..\cpp\portable\nickcpp\ARect.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_AMOVI=\
	"..\..\cpp\portable\nickcpp\AError.h"\
	"..\..\cpp\portable\nickcpp\AFrameAbstract.h"\
	"..\..\cpp\portable\nickcpp\AMovie.h"\
	"..\..\cpp\portable\nickcpp\APoint.h"\
	"..\..\cpp\portable\nickcpp\ARect.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_AMOVI=\
	"..\..\cpp\portable\nickcpp\AError.h"\
	"..\..\cpp\portable\nickcpp\AFrameAbstract.h"\
	"..\..\cpp\portable\nickcpp\AMovie.h"\
	"..\..\cpp\portable\nickcpp\APoint.h"\
	"..\..\cpp\portable\nickcpp\ARect.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_AMOVI=\
	"..\..\cpp\portable\nickcpp\AError.h"\
	"..\..\cpp\portable\nickcpp\AFrameAbstract.h"\
	"..\..\cpp\portable\nickcpp\AMovie.h"\
	"..\..\cpp\portable\nickcpp\APoint.h"\
	"..\..\cpp\portable\nickcpp\ARect.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_AMOVI=\
	"..\..\cpp\portable\nickcpp\AError.h"\
	"..\..\cpp\portable\nickcpp\AFrameAbstract.h"\
	"..\..\cpp\portable\nickcpp\AMovie.h"\
	"..\..\cpp\portable\nickcpp\APoint.h"\
	"..\..\cpp\portable\nickcpp\ARect.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\ndecoder\AMovieDecoder.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_AMOVIE=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_AMOVIE=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_AMOVIE=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_AMOVIE=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_AMOVIE=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_AMOVIE=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_AMOVIE=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_AMOVIE=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_AMOVIE=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_AMOVIE=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_AMOVIE=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_AMOVIE=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_AMOVIE=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_AMOVIE=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_AMOVIE=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_AMOVIE=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\soundlib\AMP3Decoder.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_AMP3D=\
	"..\..\cpp\portable\soundlib\AMP3Decoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_AMP3D=\
	"..\..\cpp\portable\soundlib\AMP3Decoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_AMP3D=\
	"..\..\cpp\portable\soundlib\AMP3Decoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_AMP3D=\
	"..\..\cpp\portable\soundlib\AMP3Decoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_AMP3D=\
	"..\..\cpp\portable\soundlib\AMP3Decoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_AMP3D=\
	"..\..\cpp\portable\soundlib\AMP3Decoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_AMP3D=\
	"..\..\cpp\portable\soundlib\AMP3Decoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_AMP3D=\
	"..\..\cpp\portable\soundlib\AMP3Decoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_AMP3D=\
	"..\..\cpp\portable\soundlib\AMP3Decoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_AMP3D=\
	"..\..\cpp\portable\soundlib\AMP3Decoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_AMP3D=\
	"..\..\cpp\portable\soundlib\AMP3Decoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_AMP3D=\
	"..\..\cpp\portable\soundlib\AMP3Decoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_AMP3D=\
	"..\..\cpp\portable\soundlib\AMP3Decoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_AMP3D=\
	"..\..\cpp\portable\soundlib\AMP3Decoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_AMP3D=\
	"..\..\cpp\portable\soundlib\AMP3Decoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_AMP3D=\
	"..\..\cpp\portable\soundlib\AMP3Decoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\ndecoder\AMultiImageDecoder.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_AMULT=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMultiImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_AMULT=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMultiImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_AMULT=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMultiImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_AMULT=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMultiImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_AMULT=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMultiImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_AMULT=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMultiImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_AMULT=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMultiImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_AMULT=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMultiImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_AMULT=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMultiImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_AMULT=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMultiImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_AMULT=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMultiImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_AMULT=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMultiImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_AMULT=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMultiImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_AMULT=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMultiImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_AMULT=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMultiImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_AMULT=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMultiImageDecoder.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\ndecoder\AMultiMovieDecoder.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_AMULTI=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMultiMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_AMULTI=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMultiMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_AMULTI=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMultiMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_AMULTI=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMultiMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_AMULTI=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMultiMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_AMULTI=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMultiMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_AMULTI=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMultiMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_AMULTI=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMultiMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_AMULTI=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMultiMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_AMULTI=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMultiMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_AMULTI=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMultiMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_AMULTI=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMultiMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_AMULTI=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMultiMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_AMULTI=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMultiMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_AMULTI=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMultiMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_AMULTI=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMultiMovieDecoder.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\soundlib\AMultiSongDecoder.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_AMULTIS=\
	"..\..\cpp\portable\soundlib\AMultiSongDecoder.h"\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_AMULTIS=\
	"..\..\cpp\portable\soundlib\AMultiSongDecoder.h"\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_AMULTIS=\
	"..\..\cpp\portable\soundlib\AMultiSongDecoder.h"\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_AMULTIS=\
	"..\..\cpp\portable\soundlib\AMultiSongDecoder.h"\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_AMULTIS=\
	"..\..\cpp\portable\soundlib\AMultiSongDecoder.h"\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_AMULTIS=\
	"..\..\cpp\portable\soundlib\AMultiSongDecoder.h"\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_AMULTIS=\
	"..\..\cpp\portable\soundlib\AMultiSongDecoder.h"\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_AMULTIS=\
	"..\..\cpp\portable\soundlib\AMultiSongDecoder.h"\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_AMULTIS=\
	"..\..\cpp\portable\soundlib\AMultiSongDecoder.h"\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_AMULTIS=\
	"..\..\cpp\portable\soundlib\AMultiSongDecoder.h"\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_AMULTIS=\
	"..\..\cpp\portable\soundlib\AMultiSongDecoder.h"\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_AMULTIS=\
	"..\..\cpp\portable\soundlib\AMultiSongDecoder.h"\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_AMULTIS=\
	"..\..\cpp\portable\soundlib\AMultiSongDecoder.h"\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_AMULTIS=\
	"..\..\cpp\portable\soundlib\AMultiSongDecoder.h"\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_AMULTIS=\
	"..\..\cpp\portable\soundlib\AMultiSongDecoder.h"\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_AMULTIS=\
	"..\..\cpp\portable\soundlib\AMultiSongDecoder.h"\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\soundlib\AMultiSoundDecoder.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_AMULTISO=\
	"..\..\cpp\portable\soundlib\AMultiSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_AMULTISO=\
	"..\..\cpp\portable\soundlib\AMultiSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_AMULTISO=\
	"..\..\cpp\portable\soundlib\AMultiSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_AMULTISO=\
	"..\..\cpp\portable\soundlib\AMultiSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_AMULTISO=\
	"..\..\cpp\portable\soundlib\AMultiSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_AMULTISO=\
	"..\..\cpp\portable\soundlib\AMultiSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_AMULTISO=\
	"..\..\cpp\portable\soundlib\AMultiSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_AMULTISO=\
	"..\..\cpp\portable\soundlib\AMultiSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_AMULTISO=\
	"..\..\cpp\portable\soundlib\AMultiSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_AMULTISO=\
	"..\..\cpp\portable\soundlib\AMultiSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_AMULTISO=\
	"..\..\cpp\portable\soundlib\AMultiSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_AMULTISO=\
	"..\..\cpp\portable\soundlib\AMultiSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_AMULTISO=\
	"..\..\cpp\portable\soundlib\AMultiSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_AMULTISO=\
	"..\..\cpp\portable\soundlib\AMultiSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_AMULTISO=\
	"..\..\cpp\portable\soundlib\AMultiSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_AMULTISO=\
	"..\..\cpp\portable\soundlib\AMultiSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\netlib\ANetAbstract.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_ANETA=\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_ANETA=\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_ANETA=\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_ANETA=\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_ANETA=\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_ANETA=\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_ANETA=\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_ANETA=\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_ANETA=\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_ANETA=\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_ANETA=\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_ANETA=\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_ANETA=\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_ANETA=\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_ANETA=\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_ANETA=\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\netlib\ANetwork.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_ANETW=\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	"..\..\cpp\portable\netlib\ANetwork.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_ANETW=\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	"..\..\cpp\portable\netlib\ANetwork.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_ANETW=\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	"..\..\cpp\portable\netlib\ANetwork.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_ANETW=\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	"..\..\cpp\portable\netlib\ANetwork.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_ANETW=\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	"..\..\cpp\portable\netlib\ANetwork.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_ANETW=\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	"..\..\cpp\portable\netlib\ANetwork.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_ANETW=\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	"..\..\cpp\portable\netlib\ANetwork.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_ANETW=\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	"..\..\cpp\portable\netlib\ANetwork.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_ANETW=\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	"..\..\cpp\portable\netlib\ANetwork.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_ANETW=\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	"..\..\cpp\portable\netlib\ANetwork.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_ANETW=\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	"..\..\cpp\portable\netlib\ANetwork.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_ANETW=\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	"..\..\cpp\portable\netlib\ANetwork.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_ANETW=\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	"..\..\cpp\portable\netlib\ANetwork.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_ANETW=\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	"..\..\cpp\portable\netlib\ANetwork.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_ANETW=\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	"..\..\cpp\portable\netlib\ANetwork.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_ANETW=\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	"..\..\cpp\portable\netlib\ANetwork.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nickcpp\ANullStream.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_ANULL=\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_ANULL=\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_ANULL=\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_ANULL=\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_ANULL=\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_ANULL=\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_ANULL=\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_ANULL=\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_ANULL=\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_ANULL=\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_ANULL=\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_ANULL=\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_ANULL=\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_ANULL=\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_ANULL=\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_ANULL=\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nickcpp\AOutputStream.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_AOUTP=\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	"..\..\cpp\portable\nickcpp\AOutputStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_AOUTP=\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	"..\..\cpp\portable\nickcpp\AOutputStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_AOUTP=\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	"..\..\cpp\portable\nickcpp\AOutputStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_AOUTP=\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	"..\..\cpp\portable\nickcpp\AOutputStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_AOUTP=\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	"..\..\cpp\portable\nickcpp\AOutputStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_AOUTP=\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	"..\..\cpp\portable\nickcpp\AOutputStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_AOUTP=\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	"..\..\cpp\portable\nickcpp\AOutputStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_AOUTP=\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	"..\..\cpp\portable\nickcpp\AOutputStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_AOUTP=\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	"..\..\cpp\portable\nickcpp\AOutputStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_AOUTP=\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	"..\..\cpp\portable\nickcpp\AOutputStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_AOUTP=\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	"..\..\cpp\portable\nickcpp\AOutputStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_AOUTP=\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	"..\..\cpp\portable\nickcpp\AOutputStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_AOUTP=\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	"..\..\cpp\portable\nickcpp\AOutputStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_AOUTP=\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	"..\..\cpp\portable\nickcpp\AOutputStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_AOUTP=\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	"..\..\cpp\portable\nickcpp\AOutputStream.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_AOUTP=\
	"..\..\cpp\portable\nickcpp\ANullStream.h"\
	"..\..\cpp\portable\nickcpp\AOutputStream.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nickcpp\APalette.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_APALE=\
	"..\..\cpp\portable\nickcpp\AColor.h"\
	"..\..\cpp\portable\nickcpp\APalette.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_APALE=\
	"..\..\cpp\portable\nickcpp\AColor.h"\
	"..\..\cpp\portable\nickcpp\APalette.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_APALE=\
	"..\..\cpp\portable\nickcpp\AColor.h"\
	"..\..\cpp\portable\nickcpp\APalette.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_APALE=\
	"..\..\cpp\portable\nickcpp\AColor.h"\
	"..\..\cpp\portable\nickcpp\APalette.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_APALE=\
	"..\..\cpp\portable\nickcpp\AColor.h"\
	"..\..\cpp\portable\nickcpp\APalette.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_APALE=\
	"..\..\cpp\portable\nickcpp\AColor.h"\
	"..\..\cpp\portable\nickcpp\APalette.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_APALE=\
	"..\..\cpp\portable\nickcpp\AColor.h"\
	"..\..\cpp\portable\nickcpp\APalette.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_APALE=\
	"..\..\cpp\portable\nickcpp\AColor.h"\
	"..\..\cpp\portable\nickcpp\APalette.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_APALE=\
	"..\..\cpp\portable\nickcpp\AColor.h"\
	"..\..\cpp\portable\nickcpp\APalette.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_APALE=\
	"..\..\cpp\portable\nickcpp\AColor.h"\
	"..\..\cpp\portable\nickcpp\APalette.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_APALE=\
	"..\..\cpp\portable\nickcpp\AColor.h"\
	"..\..\cpp\portable\nickcpp\APalette.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_APALE=\
	"..\..\cpp\portable\nickcpp\AColor.h"\
	"..\..\cpp\portable\nickcpp\APalette.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_APALE=\
	"..\..\cpp\portable\nickcpp\AColor.h"\
	"..\..\cpp\portable\nickcpp\APalette.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_APALE=\
	"..\..\cpp\portable\nickcpp\AColor.h"\
	"..\..\cpp\portable\nickcpp\APalette.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_APALE=\
	"..\..\cpp\portable\nickcpp\AColor.h"\
	"..\..\cpp\portable\nickcpp\APalette.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_APALE=\
	"..\..\cpp\portable\nickcpp\AColor.h"\
	"..\..\cpp\portable\nickcpp\APalette.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\APart3D.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_APART=\
	"..\..\cpp\portable\nick3d\APart3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_APART=\
	"..\..\cpp\portable\nick3d\APart3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_APART=\
	"..\..\cpp\portable\nick3d\APart3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_APART=\
	"..\..\cpp\portable\nick3d\APart3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_APART=\
	"..\..\cpp\portable\nick3d\APart3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_APART=\
	"..\..\cpp\portable\nick3d\APart3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_APART=\
	"..\..\cpp\portable\nick3d\APart3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_APART=\
	"..\..\cpp\portable\nick3d\APart3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_APART=\
	"..\..\cpp\portable\nick3d\APart3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_APART=\
	"..\..\cpp\portable\nick3d\APart3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_APART=\
	"..\..\cpp\portable\nick3d\APart3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_APART=\
	"..\..\cpp\portable\nick3d\APart3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_APART=\
	"..\..\cpp\portable\nick3d\APart3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_APART=\
	"..\..\cpp\portable\nick3d\APart3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_APART=\
	"..\..\cpp\portable\nick3d\APart3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_APART=\
	"..\..\cpp\portable\nick3d\APart3D.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\ndecoder\APCXDecoder.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_APCXD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\APCXDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_APCXD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\APCXDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_APCXD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\APCXDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_APCXD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\APCXDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_APCXD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\APCXDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_APCXD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\APCXDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_APCXD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\APCXDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_APCXD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\APCXDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_APCXD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\APCXDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_APCXD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\APCXDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_APCXD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\APCXDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_APCXD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\APCXDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_APCXD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\APCXDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_APCXD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\APCXDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_APCXD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\APCXDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_APCXD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\APCXDecoder.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\APLGDecoder.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_APLGD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\APLGDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_APLGD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\APLGDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_APLGD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\APLGDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_APLGD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\APLGDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_APLGD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\APLGDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_APLGD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\APLGDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_APLGD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\APLGDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_APLGD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\APLGDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_APLGD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\APLGDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_APLGD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\APLGDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_APLGD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\APLGDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_APLGD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\APLGDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_APLGD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\APLGDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_APLGD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\APLGDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_APLGD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\APLGDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_APLGD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\APLGDecoder.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nickcpp\APoint.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_APOIN=\
	"..\..\cpp\portable\nickcpp\APoint.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_APOIN=\
	"..\..\cpp\portable\nickcpp\APoint.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_APOIN=\
	"..\..\cpp\portable\nickcpp\APoint.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_APOIN=\
	"..\..\cpp\portable\nickcpp\APoint.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_APOIN=\
	"..\..\cpp\portable\nickcpp\APoint.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_APOIN=\
	"..\..\cpp\portable\nickcpp\APoint.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_APOIN=\
	"..\..\cpp\portable\nickcpp\APoint.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_APOIN=\
	"..\..\cpp\portable\nickcpp\APoint.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_APOIN=\
	"..\..\cpp\portable\nickcpp\APoint.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_APOIN=\
	"..\..\cpp\portable\nickcpp\APoint.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_APOIN=\
	"..\..\cpp\portable\nickcpp\APoint.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_APOIN=\
	"..\..\cpp\portable\nickcpp\APoint.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_APOIN=\
	"..\..\cpp\portable\nickcpp\APoint.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_APOIN=\
	"..\..\cpp\portable\nickcpp\APoint.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_APOIN=\
	"..\..\cpp\portable\nickcpp\APoint.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_APOIN=\
	"..\..\cpp\portable\nickcpp\APoint.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\ndecoder\AQTDecoderHelper.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_AQTDE=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_AQTDE=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_AQTDE=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_AQTDE=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_AQTDE=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_AQTDE=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_AQTDE=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_AQTDE=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_AQTDE=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_AQTDE=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_AQTDE=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_AQTDE=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_AQTDE=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_AQTDE=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_AQTDE=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_AQTDE=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\ndecoder\AQTImageDecoder.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_AQTIM=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AQTImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_AQTIM=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AQTImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_AQTIM=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AQTImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_AQTIM=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AQTImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_AQTIM=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AQTImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_AQTIM=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AQTImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_AQTIM=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AQTImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_AQTIM=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AQTImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_AQTIM=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AQTImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_AQTIM=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AQTImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_AQTIM=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AQTImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_AQTIM=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AQTImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_AQTIM=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AQTImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_AQTIM=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AQTImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_AQTIM=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AQTImageDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_AQTIM=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AQTImageDecoder.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\ndecoder\AQTMovieDecoder.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_AQTMO=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AQTMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_AQTMO=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AQTMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_AQTMO=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AQTMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_AQTMO=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AQTMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_AQTMO=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AQTMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_AQTMO=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AQTMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_AQTMO=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AQTMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_AQTMO=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AQTMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_AQTMO=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AQTMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_AQTMO=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AQTMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_AQTMO=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AQTMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_AQTMO=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AQTMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_AQTMO=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AQTMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_AQTMO=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AQTMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_AQTMO=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AQTMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_AQTMO=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AQTMovieDecoder.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\soundlib\AQTSongDecoder.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_AQTSO=\
	"..\..\cpp\portable\soundlib\AQTSongDecoder.h"\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_AQTSO=\
	"..\..\cpp\portable\soundlib\AQTSongDecoder.h"\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_AQTSO=\
	"..\..\cpp\portable\soundlib\AQTSongDecoder.h"\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_AQTSO=\
	"..\..\cpp\portable\soundlib\AQTSongDecoder.h"\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_AQTSO=\
	"..\..\cpp\portable\soundlib\AQTSongDecoder.h"\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_AQTSO=\
	"..\..\cpp\portable\soundlib\AQTSongDecoder.h"\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_AQTSO=\
	"..\..\cpp\portable\soundlib\AQTSongDecoder.h"\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_AQTSO=\
	"..\..\cpp\portable\soundlib\AQTSongDecoder.h"\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_AQTSO=\
	"..\..\cpp\portable\soundlib\AQTSongDecoder.h"\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_AQTSO=\
	"..\..\cpp\portable\soundlib\AQTSongDecoder.h"\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_AQTSO=\
	"..\..\cpp\portable\soundlib\AQTSongDecoder.h"\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_AQTSO=\
	"..\..\cpp\portable\soundlib\AQTSongDecoder.h"\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_AQTSO=\
	"..\..\cpp\portable\soundlib\AQTSongDecoder.h"\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_AQTSO=\
	"..\..\cpp\portable\soundlib\AQTSongDecoder.h"\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_AQTSO=\
	"..\..\cpp\portable\soundlib\AQTSongDecoder.h"\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_AQTSO=\
	"..\..\cpp\portable\soundlib\AQTSongDecoder.h"\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\soundlib\AQTSoundDecoder.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_AQTSOU=\
	"..\..\cpp\portable\soundlib\AQTSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_AQTSOU=\
	"..\..\cpp\portable\soundlib\AQTSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_AQTSOU=\
	"..\..\cpp\portable\soundlib\AQTSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_AQTSOU=\
	"..\..\cpp\portable\soundlib\AQTSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_AQTSOU=\
	"..\..\cpp\portable\soundlib\AQTSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_AQTSOU=\
	"..\..\cpp\portable\soundlib\AQTSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_AQTSOU=\
	"..\..\cpp\portable\soundlib\AQTSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_AQTSOU=\
	"..\..\cpp\portable\soundlib\AQTSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_AQTSOU=\
	"..\..\cpp\portable\soundlib\AQTSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_AQTSOU=\
	"..\..\cpp\portable\soundlib\AQTSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_AQTSOU=\
	"..\..\cpp\portable\soundlib\AQTSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_AQTSOU=\
	"..\..\cpp\portable\soundlib\AQTSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_AQTSOU=\
	"..\..\cpp\portable\soundlib\AQTSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_AQTSOU=\
	"..\..\cpp\portable\soundlib\AQTSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_AQTSOU=\
	"..\..\cpp\portable\soundlib\AQTSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_AQTSOU=\
	"..\..\cpp\portable\soundlib\AQTSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nickcpp\ARect.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_ARECT=\
	"..\..\cpp\portable\nickcpp\ARect.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_ARECT=\
	"..\..\cpp\portable\nickcpp\ARect.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_ARECT=\
	"..\..\cpp\portable\nickcpp\ARect.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_ARECT=\
	"..\..\cpp\portable\nickcpp\ARect.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_ARECT=\
	"..\..\cpp\portable\nickcpp\ARect.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_ARECT=\
	"..\..\cpp\portable\nickcpp\ARect.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_ARECT=\
	"..\..\cpp\portable\nickcpp\ARect.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_ARECT=\
	"..\..\cpp\portable\nickcpp\ARect.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_ARECT=\
	"..\..\cpp\portable\nickcpp\ARect.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_ARECT=\
	"..\..\cpp\portable\nickcpp\ARect.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_ARECT=\
	"..\..\cpp\portable\nickcpp\ARect.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_ARECT=\
	"..\..\cpp\portable\nickcpp\ARect.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_ARECT=\
	"..\..\cpp\portable\nickcpp\ARect.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_ARECT=\
	"..\..\cpp\portable\nickcpp\ARect.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_ARECT=\
	"..\..\cpp\portable\nickcpp\ARect.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_ARECT=\
	"..\..\cpp\portable\nickcpp\ARect.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\ARendererAbstract3D.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_AREND=\
	"..\..\cpp\portable\nick3d\ACamera3D.h"\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\ALight3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AMesh3D.h"\
	"..\..\cpp\portable\nick3d\APart3D.h"\
	"..\..\cpp\portable\nick3d\ARendererAbstract3D.h"\
	"..\..\cpp\portable\nick3d\AScene3D.h"\
	"..\..\cpp\portable\nick3d\ATextCoord3D.h"\
	"..\..\cpp\portable\nick3d\ATexture3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_AREND=\
	"..\..\cpp\portable\nick3d\ACamera3D.h"\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\ALight3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AMesh3D.h"\
	"..\..\cpp\portable\nick3d\APart3D.h"\
	"..\..\cpp\portable\nick3d\ARendererAbstract3D.h"\
	"..\..\cpp\portable\nick3d\AScene3D.h"\
	"..\..\cpp\portable\nick3d\ATextCoord3D.h"\
	"..\..\cpp\portable\nick3d\ATexture3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_AREND=\
	"..\..\cpp\portable\nick3d\ACamera3D.h"\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\ALight3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AMesh3D.h"\
	"..\..\cpp\portable\nick3d\APart3D.h"\
	"..\..\cpp\portable\nick3d\ARendererAbstract3D.h"\
	"..\..\cpp\portable\nick3d\AScene3D.h"\
	"..\..\cpp\portable\nick3d\ATextCoord3D.h"\
	"..\..\cpp\portable\nick3d\ATexture3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_AREND=\
	"..\..\cpp\portable\nick3d\ACamera3D.h"\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\ALight3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AMesh3D.h"\
	"..\..\cpp\portable\nick3d\APart3D.h"\
	"..\..\cpp\portable\nick3d\ARendererAbstract3D.h"\
	"..\..\cpp\portable\nick3d\AScene3D.h"\
	"..\..\cpp\portable\nick3d\ATextCoord3D.h"\
	"..\..\cpp\portable\nick3d\ATexture3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_AREND=\
	"..\..\cpp\portable\nick3d\ACamera3D.h"\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\ALight3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AMesh3D.h"\
	"..\..\cpp\portable\nick3d\APart3D.h"\
	"..\..\cpp\portable\nick3d\ARendererAbstract3D.h"\
	"..\..\cpp\portable\nick3d\AScene3D.h"\
	"..\..\cpp\portable\nick3d\ATextCoord3D.h"\
	"..\..\cpp\portable\nick3d\ATexture3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_AREND=\
	"..\..\cpp\portable\nick3d\ACamera3D.h"\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\ALight3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AMesh3D.h"\
	"..\..\cpp\portable\nick3d\APart3D.h"\
	"..\..\cpp\portable\nick3d\ARendererAbstract3D.h"\
	"..\..\cpp\portable\nick3d\AScene3D.h"\
	"..\..\cpp\portable\nick3d\ATextCoord3D.h"\
	"..\..\cpp\portable\nick3d\ATexture3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_AREND=\
	"..\..\cpp\portable\nick3d\ACamera3D.h"\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\ALight3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AMesh3D.h"\
	"..\..\cpp\portable\nick3d\APart3D.h"\
	"..\..\cpp\portable\nick3d\ARendererAbstract3D.h"\
	"..\..\cpp\portable\nick3d\AScene3D.h"\
	"..\..\cpp\portable\nick3d\ATextCoord3D.h"\
	"..\..\cpp\portable\nick3d\ATexture3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_AREND=\
	"..\..\cpp\portable\nick3d\ACamera3D.h"\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\ALight3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AMesh3D.h"\
	"..\..\cpp\portable\nick3d\APart3D.h"\
	"..\..\cpp\portable\nick3d\ARendererAbstract3D.h"\
	"..\..\cpp\portable\nick3d\AScene3D.h"\
	"..\..\cpp\portable\nick3d\ATextCoord3D.h"\
	"..\..\cpp\portable\nick3d\ATexture3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_AREND=\
	"..\..\cpp\portable\nick3d\ACamera3D.h"\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\ALight3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AMesh3D.h"\
	"..\..\cpp\portable\nick3d\APart3D.h"\
	"..\..\cpp\portable\nick3d\ARendererAbstract3D.h"\
	"..\..\cpp\portable\nick3d\AScene3D.h"\
	"..\..\cpp\portable\nick3d\ATextCoord3D.h"\
	"..\..\cpp\portable\nick3d\ATexture3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_AREND=\
	"..\..\cpp\portable\nick3d\ACamera3D.h"\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\ALight3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AMesh3D.h"\
	"..\..\cpp\portable\nick3d\APart3D.h"\
	"..\..\cpp\portable\nick3d\ARendererAbstract3D.h"\
	"..\..\cpp\portable\nick3d\AScene3D.h"\
	"..\..\cpp\portable\nick3d\ATextCoord3D.h"\
	"..\..\cpp\portable\nick3d\ATexture3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_AREND=\
	"..\..\cpp\portable\nick3d\ACamera3D.h"\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\ALight3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AMesh3D.h"\
	"..\..\cpp\portable\nick3d\APart3D.h"\
	"..\..\cpp\portable\nick3d\ARendererAbstract3D.h"\
	"..\..\cpp\portable\nick3d\AScene3D.h"\
	"..\..\cpp\portable\nick3d\ATextCoord3D.h"\
	"..\..\cpp\portable\nick3d\ATexture3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_AREND=\
	"..\..\cpp\portable\nick3d\ACamera3D.h"\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\ALight3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AMesh3D.h"\
	"..\..\cpp\portable\nick3d\APart3D.h"\
	"..\..\cpp\portable\nick3d\ARendererAbstract3D.h"\
	"..\..\cpp\portable\nick3d\AScene3D.h"\
	"..\..\cpp\portable\nick3d\ATextCoord3D.h"\
	"..\..\cpp\portable\nick3d\ATexture3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_AREND=\
	"..\..\cpp\portable\nick3d\ACamera3D.h"\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\ALight3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AMesh3D.h"\
	"..\..\cpp\portable\nick3d\APart3D.h"\
	"..\..\cpp\portable\nick3d\ARendererAbstract3D.h"\
	"..\..\cpp\portable\nick3d\AScene3D.h"\
	"..\..\cpp\portable\nick3d\ATextCoord3D.h"\
	"..\..\cpp\portable\nick3d\ATexture3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_AREND=\
	"..\..\cpp\portable\nick3d\ACamera3D.h"\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\ALight3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AMesh3D.h"\
	"..\..\cpp\portable\nick3d\APart3D.h"\
	"..\..\cpp\portable\nick3d\ARendererAbstract3D.h"\
	"..\..\cpp\portable\nick3d\AScene3D.h"\
	"..\..\cpp\portable\nick3d\ATextCoord3D.h"\
	"..\..\cpp\portable\nick3d\ATexture3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_AREND=\
	"..\..\cpp\portable\nick3d\ACamera3D.h"\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\ALight3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AMesh3D.h"\
	"..\..\cpp\portable\nick3d\APart3D.h"\
	"..\..\cpp\portable\nick3d\ARendererAbstract3D.h"\
	"..\..\cpp\portable\nick3d\AScene3D.h"\
	"..\..\cpp\portable\nick3d\ATextCoord3D.h"\
	"..\..\cpp\portable\nick3d\ATexture3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_AREND=\
	"..\..\cpp\portable\nick3d\ACamera3D.h"\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\ALight3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AMesh3D.h"\
	"..\..\cpp\portable\nick3d\APart3D.h"\
	"..\..\cpp\portable\nick3d\ARendererAbstract3D.h"\
	"..\..\cpp\portable\nick3d\AScene3D.h"\
	"..\..\cpp\portable\nick3d\ATextCoord3D.h"\
	"..\..\cpp\portable\nick3d\ATexture3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\AScene3D.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_ASCEN=\
	"..\..\cpp\portable\nick3d\ACamera3D.h"\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\ALight3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AMesh3D.h"\
	"..\..\cpp\portable\nick3d\APart3D.h"\
	"..\..\cpp\portable\nick3d\AScene3D.h"\
	"..\..\cpp\portable\nick3d\ATextCoord3D.h"\
	"..\..\cpp\portable\nick3d\ATexture3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_ASCEN=\
	"..\..\cpp\portable\nick3d\ACamera3D.h"\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\ALight3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AMesh3D.h"\
	"..\..\cpp\portable\nick3d\APart3D.h"\
	"..\..\cpp\portable\nick3d\AScene3D.h"\
	"..\..\cpp\portable\nick3d\ATextCoord3D.h"\
	"..\..\cpp\portable\nick3d\ATexture3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_ASCEN=\
	"..\..\cpp\portable\nick3d\ACamera3D.h"\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\ALight3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AMesh3D.h"\
	"..\..\cpp\portable\nick3d\APart3D.h"\
	"..\..\cpp\portable\nick3d\AScene3D.h"\
	"..\..\cpp\portable\nick3d\ATextCoord3D.h"\
	"..\..\cpp\portable\nick3d\ATexture3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_ASCEN=\
	"..\..\cpp\portable\nick3d\ACamera3D.h"\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\ALight3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AMesh3D.h"\
	"..\..\cpp\portable\nick3d\APart3D.h"\
	"..\..\cpp\portable\nick3d\AScene3D.h"\
	"..\..\cpp\portable\nick3d\ATextCoord3D.h"\
	"..\..\cpp\portable\nick3d\ATexture3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_ASCEN=\
	"..\..\cpp\portable\nick3d\ACamera3D.h"\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\ALight3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AMesh3D.h"\
	"..\..\cpp\portable\nick3d\APart3D.h"\
	"..\..\cpp\portable\nick3d\AScene3D.h"\
	"..\..\cpp\portable\nick3d\ATextCoord3D.h"\
	"..\..\cpp\portable\nick3d\ATexture3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_ASCEN=\
	"..\..\cpp\portable\nick3d\ACamera3D.h"\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\ALight3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AMesh3D.h"\
	"..\..\cpp\portable\nick3d\APart3D.h"\
	"..\..\cpp\portable\nick3d\AScene3D.h"\
	"..\..\cpp\portable\nick3d\ATextCoord3D.h"\
	"..\..\cpp\portable\nick3d\ATexture3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_ASCEN=\
	"..\..\cpp\portable\nick3d\ACamera3D.h"\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\ALight3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AMesh3D.h"\
	"..\..\cpp\portable\nick3d\APart3D.h"\
	"..\..\cpp\portable\nick3d\AScene3D.h"\
	"..\..\cpp\portable\nick3d\ATextCoord3D.h"\
	"..\..\cpp\portable\nick3d\ATexture3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_ASCEN=\
	"..\..\cpp\portable\nick3d\ACamera3D.h"\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\ALight3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AMesh3D.h"\
	"..\..\cpp\portable\nick3d\APart3D.h"\
	"..\..\cpp\portable\nick3d\AScene3D.h"\
	"..\..\cpp\portable\nick3d\ATextCoord3D.h"\
	"..\..\cpp\portable\nick3d\ATexture3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_ASCEN=\
	"..\..\cpp\portable\nick3d\ACamera3D.h"\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\ALight3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AMesh3D.h"\
	"..\..\cpp\portable\nick3d\APart3D.h"\
	"..\..\cpp\portable\nick3d\AScene3D.h"\
	"..\..\cpp\portable\nick3d\ATextCoord3D.h"\
	"..\..\cpp\portable\nick3d\ATexture3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_ASCEN=\
	"..\..\cpp\portable\nick3d\ACamera3D.h"\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\ALight3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AMesh3D.h"\
	"..\..\cpp\portable\nick3d\APart3D.h"\
	"..\..\cpp\portable\nick3d\AScene3D.h"\
	"..\..\cpp\portable\nick3d\ATextCoord3D.h"\
	"..\..\cpp\portable\nick3d\ATexture3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_ASCEN=\
	"..\..\cpp\portable\nick3d\ACamera3D.h"\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\ALight3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AMesh3D.h"\
	"..\..\cpp\portable\nick3d\APart3D.h"\
	"..\..\cpp\portable\nick3d\AScene3D.h"\
	"..\..\cpp\portable\nick3d\ATextCoord3D.h"\
	"..\..\cpp\portable\nick3d\ATexture3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_ASCEN=\
	"..\..\cpp\portable\nick3d\ACamera3D.h"\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\ALight3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AMesh3D.h"\
	"..\..\cpp\portable\nick3d\APart3D.h"\
	"..\..\cpp\portable\nick3d\AScene3D.h"\
	"..\..\cpp\portable\nick3d\ATextCoord3D.h"\
	"..\..\cpp\portable\nick3d\ATexture3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_ASCEN=\
	"..\..\cpp\portable\nick3d\ACamera3D.h"\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\ALight3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AMesh3D.h"\
	"..\..\cpp\portable\nick3d\APart3D.h"\
	"..\..\cpp\portable\nick3d\AScene3D.h"\
	"..\..\cpp\portable\nick3d\ATextCoord3D.h"\
	"..\..\cpp\portable\nick3d\ATexture3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_ASCEN=\
	"..\..\cpp\portable\nick3d\ACamera3D.h"\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\ALight3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AMesh3D.h"\
	"..\..\cpp\portable\nick3d\APart3D.h"\
	"..\..\cpp\portable\nick3d\AScene3D.h"\
	"..\..\cpp\portable\nick3d\ATextCoord3D.h"\
	"..\..\cpp\portable\nick3d\ATexture3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_ASCEN=\
	"..\..\cpp\portable\nick3d\ACamera3D.h"\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\ALight3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AMesh3D.h"\
	"..\..\cpp\portable\nick3d\APart3D.h"\
	"..\..\cpp\portable\nick3d\AScene3D.h"\
	"..\..\cpp\portable\nick3d\ATextCoord3D.h"\
	"..\..\cpp\portable\nick3d\ATexture3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_ASCEN=\
	"..\..\cpp\portable\nick3d\ACamera3D.h"\
	"..\..\cpp\portable\nick3d\AFace3D.h"\
	"..\..\cpp\portable\nick3d\ALight3D.h"\
	"..\..\cpp\portable\nick3d\AMatrix3D.h"\
	"..\..\cpp\portable\nick3d\AMesh3D.h"\
	"..\..\cpp\portable\nick3d\APart3D.h"\
	"..\..\cpp\portable\nick3d\AScene3D.h"\
	"..\..\cpp\portable\nick3d\ATextCoord3D.h"\
	"..\..\cpp\portable\nick3d\ATexture3D.h"\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\ASceneDecoder.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_ASCENE=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_ASCENE=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_ASCENE=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_ASCENE=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_ASCENE=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_ASCENE=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_ASCENE=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_ASCENE=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_ASCENE=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_ASCENE=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_ASCENE=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_ASCENE=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_ASCENE=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_ASCENE=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_ASCENE=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_ASCENE=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\ndecoder\ASmartDecoder.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_ASMAR=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_ASMAR=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_ASMAR=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_ASMAR=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_ASMAR=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_ASMAR=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_ASMAR=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_ASMAR=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_ASMAR=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_ASMAR=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_ASMAR=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_ASMAR=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_ASMAR=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_ASMAR=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_ASMAR=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_ASMAR=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartMovieDecoder.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\ndecoder\ASmartImageDecoder.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_ASMART=\
	"..\..\cpp\portable\ndecoder\ABMPDecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AGIFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AIFFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AJFIFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMultiImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\APCXDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AQTImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\ATGADecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_ASMART=\
	"..\..\cpp\portable\ndecoder\ABMPDecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AGIFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AIFFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AJFIFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMultiImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\APCXDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AQTImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\ATGADecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_ASMART=\
	"..\..\cpp\portable\ndecoder\ABMPDecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AGIFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AIFFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AJFIFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMultiImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\APCXDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AQTImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\ATGADecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_ASMART=\
	"..\..\cpp\portable\ndecoder\ABMPDecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AGIFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AIFFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AJFIFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMultiImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\APCXDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AQTImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\ATGADecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_ASMART=\
	"..\..\cpp\portable\ndecoder\ABMPDecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AGIFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AIFFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AJFIFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMultiImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\APCXDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AQTImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\ATGADecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_ASMART=\
	"..\..\cpp\portable\ndecoder\ABMPDecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AGIFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AIFFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AJFIFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMultiImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\APCXDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AQTImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\ATGADecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_ASMART=\
	"..\..\cpp\portable\ndecoder\ABMPDecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AGIFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AIFFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AJFIFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMultiImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\APCXDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AQTImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\ATGADecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_ASMART=\
	"..\..\cpp\portable\ndecoder\ABMPDecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AGIFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AIFFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AJFIFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMultiImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\APCXDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AQTImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\ATGADecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_ASMART=\
	"..\..\cpp\portable\ndecoder\ABMPDecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AGIFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AIFFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AJFIFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMultiImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\APCXDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AQTImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\ATGADecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_ASMART=\
	"..\..\cpp\portable\ndecoder\ABMPDecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AGIFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AIFFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AJFIFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMultiImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\APCXDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AQTImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\ATGADecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_ASMART=\
	"..\..\cpp\portable\ndecoder\ABMPDecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AGIFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AIFFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AJFIFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMultiImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\APCXDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AQTImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\ATGADecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_ASMART=\
	"..\..\cpp\portable\ndecoder\ABMPDecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AGIFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AIFFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AJFIFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMultiImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\APCXDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AQTImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\ATGADecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_ASMART=\
	"..\..\cpp\portable\ndecoder\ABMPDecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AGIFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AIFFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AJFIFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMultiImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\APCXDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AQTImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\ATGADecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_ASMART=\
	"..\..\cpp\portable\ndecoder\ABMPDecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AGIFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AIFFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AJFIFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMultiImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\APCXDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AQTImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\ATGADecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_ASMART=\
	"..\..\cpp\portable\ndecoder\ABMPDecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AGIFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AIFFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AJFIFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMultiImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\APCXDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AQTImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\ATGADecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_ASMART=\
	"..\..\cpp\portable\ndecoder\ABMPDecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AGIFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AIFFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AJFIFDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMultiImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\APCXDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AQTImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\ATGADecoder.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\ASmartMeshDecoder.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_ASMARTM=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\A3DSDecoder.h"\
	"..\..\cpp\portable\nick3d\AHalfLifeDecoder.h"\
	"..\..\cpp\portable\nick3d\AMD2Decoder.h"\
	"..\..\cpp\portable\nick3d\AMD3Decoder.h"\
	"..\..\cpp\portable\nick3d\AMD5Decoder.h"\
	"..\..\cpp\portable\nick3d\AMDLDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\APLGDecoder.h"\
	"..\..\cpp\portable\nick3d\ASmartMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\AWavefrontDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_ASMARTM=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\A3DSDecoder.h"\
	"..\..\cpp\portable\nick3d\AHalfLifeDecoder.h"\
	"..\..\cpp\portable\nick3d\AMD2Decoder.h"\
	"..\..\cpp\portable\nick3d\AMD3Decoder.h"\
	"..\..\cpp\portable\nick3d\AMD5Decoder.h"\
	"..\..\cpp\portable\nick3d\AMDLDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\APLGDecoder.h"\
	"..\..\cpp\portable\nick3d\ASmartMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\AWavefrontDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_ASMARTM=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\A3DSDecoder.h"\
	"..\..\cpp\portable\nick3d\AHalfLifeDecoder.h"\
	"..\..\cpp\portable\nick3d\AMD2Decoder.h"\
	"..\..\cpp\portable\nick3d\AMD3Decoder.h"\
	"..\..\cpp\portable\nick3d\AMD5Decoder.h"\
	"..\..\cpp\portable\nick3d\AMDLDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\APLGDecoder.h"\
	"..\..\cpp\portable\nick3d\ASmartMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\AWavefrontDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_ASMARTM=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\A3DSDecoder.h"\
	"..\..\cpp\portable\nick3d\AHalfLifeDecoder.h"\
	"..\..\cpp\portable\nick3d\AMD2Decoder.h"\
	"..\..\cpp\portable\nick3d\AMD3Decoder.h"\
	"..\..\cpp\portable\nick3d\AMD5Decoder.h"\
	"..\..\cpp\portable\nick3d\AMDLDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\APLGDecoder.h"\
	"..\..\cpp\portable\nick3d\ASmartMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\AWavefrontDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_ASMARTM=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\A3DSDecoder.h"\
	"..\..\cpp\portable\nick3d\AHalfLifeDecoder.h"\
	"..\..\cpp\portable\nick3d\AMD2Decoder.h"\
	"..\..\cpp\portable\nick3d\AMD3Decoder.h"\
	"..\..\cpp\portable\nick3d\AMD5Decoder.h"\
	"..\..\cpp\portable\nick3d\AMDLDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\APLGDecoder.h"\
	"..\..\cpp\portable\nick3d\ASmartMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\AWavefrontDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_ASMARTM=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\A3DSDecoder.h"\
	"..\..\cpp\portable\nick3d\AHalfLifeDecoder.h"\
	"..\..\cpp\portable\nick3d\AMD2Decoder.h"\
	"..\..\cpp\portable\nick3d\AMD3Decoder.h"\
	"..\..\cpp\portable\nick3d\AMD5Decoder.h"\
	"..\..\cpp\portable\nick3d\AMDLDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\APLGDecoder.h"\
	"..\..\cpp\portable\nick3d\ASmartMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\AWavefrontDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_ASMARTM=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\A3DSDecoder.h"\
	"..\..\cpp\portable\nick3d\AHalfLifeDecoder.h"\
	"..\..\cpp\portable\nick3d\AMD2Decoder.h"\
	"..\..\cpp\portable\nick3d\AMD3Decoder.h"\
	"..\..\cpp\portable\nick3d\AMD5Decoder.h"\
	"..\..\cpp\portable\nick3d\AMDLDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\APLGDecoder.h"\
	"..\..\cpp\portable\nick3d\ASmartMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\AWavefrontDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_ASMARTM=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\A3DSDecoder.h"\
	"..\..\cpp\portable\nick3d\AHalfLifeDecoder.h"\
	"..\..\cpp\portable\nick3d\AMD2Decoder.h"\
	"..\..\cpp\portable\nick3d\AMD3Decoder.h"\
	"..\..\cpp\portable\nick3d\AMD5Decoder.h"\
	"..\..\cpp\portable\nick3d\AMDLDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\APLGDecoder.h"\
	"..\..\cpp\portable\nick3d\ASmartMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\AWavefrontDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_ASMARTM=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\A3DSDecoder.h"\
	"..\..\cpp\portable\nick3d\AHalfLifeDecoder.h"\
	"..\..\cpp\portable\nick3d\AMD2Decoder.h"\
	"..\..\cpp\portable\nick3d\AMD3Decoder.h"\
	"..\..\cpp\portable\nick3d\AMD5Decoder.h"\
	"..\..\cpp\portable\nick3d\AMDLDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\APLGDecoder.h"\
	"..\..\cpp\portable\nick3d\ASmartMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\AWavefrontDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_ASMARTM=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\A3DSDecoder.h"\
	"..\..\cpp\portable\nick3d\AHalfLifeDecoder.h"\
	"..\..\cpp\portable\nick3d\AMD2Decoder.h"\
	"..\..\cpp\portable\nick3d\AMD3Decoder.h"\
	"..\..\cpp\portable\nick3d\AMD5Decoder.h"\
	"..\..\cpp\portable\nick3d\AMDLDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\APLGDecoder.h"\
	"..\..\cpp\portable\nick3d\ASmartMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\AWavefrontDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_ASMARTM=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\A3DSDecoder.h"\
	"..\..\cpp\portable\nick3d\AHalfLifeDecoder.h"\
	"..\..\cpp\portable\nick3d\AMD2Decoder.h"\
	"..\..\cpp\portable\nick3d\AMD3Decoder.h"\
	"..\..\cpp\portable\nick3d\AMD5Decoder.h"\
	"..\..\cpp\portable\nick3d\AMDLDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\APLGDecoder.h"\
	"..\..\cpp\portable\nick3d\ASmartMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\AWavefrontDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_ASMARTM=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\A3DSDecoder.h"\
	"..\..\cpp\portable\nick3d\AHalfLifeDecoder.h"\
	"..\..\cpp\portable\nick3d\AMD2Decoder.h"\
	"..\..\cpp\portable\nick3d\AMD3Decoder.h"\
	"..\..\cpp\portable\nick3d\AMD5Decoder.h"\
	"..\..\cpp\portable\nick3d\AMDLDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\APLGDecoder.h"\
	"..\..\cpp\portable\nick3d\ASmartMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\AWavefrontDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_ASMARTM=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\A3DSDecoder.h"\
	"..\..\cpp\portable\nick3d\AHalfLifeDecoder.h"\
	"..\..\cpp\portable\nick3d\AMD2Decoder.h"\
	"..\..\cpp\portable\nick3d\AMD3Decoder.h"\
	"..\..\cpp\portable\nick3d\AMD5Decoder.h"\
	"..\..\cpp\portable\nick3d\AMDLDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\APLGDecoder.h"\
	"..\..\cpp\portable\nick3d\ASmartMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\AWavefrontDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_ASMARTM=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\A3DSDecoder.h"\
	"..\..\cpp\portable\nick3d\AHalfLifeDecoder.h"\
	"..\..\cpp\portable\nick3d\AMD2Decoder.h"\
	"..\..\cpp\portable\nick3d\AMD3Decoder.h"\
	"..\..\cpp\portable\nick3d\AMD5Decoder.h"\
	"..\..\cpp\portable\nick3d\AMDLDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\APLGDecoder.h"\
	"..\..\cpp\portable\nick3d\ASmartMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\AWavefrontDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_ASMARTM=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\A3DSDecoder.h"\
	"..\..\cpp\portable\nick3d\AHalfLifeDecoder.h"\
	"..\..\cpp\portable\nick3d\AMD2Decoder.h"\
	"..\..\cpp\portable\nick3d\AMD3Decoder.h"\
	"..\..\cpp\portable\nick3d\AMD5Decoder.h"\
	"..\..\cpp\portable\nick3d\AMDLDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\APLGDecoder.h"\
	"..\..\cpp\portable\nick3d\ASmartMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\AWavefrontDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_ASMARTM=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\A3DSDecoder.h"\
	"..\..\cpp\portable\nick3d\AHalfLifeDecoder.h"\
	"..\..\cpp\portable\nick3d\AMD2Decoder.h"\
	"..\..\cpp\portable\nick3d\AMD3Decoder.h"\
	"..\..\cpp\portable\nick3d\AMD5Decoder.h"\
	"..\..\cpp\portable\nick3d\AMDLDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\APLGDecoder.h"\
	"..\..\cpp\portable\nick3d\ASmartMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\AWavefrontDecoder.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\ndecoder\ASmartMovieDecoder.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_ASMARTMO=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMultiMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AQTMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_ASMARTMO=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMultiMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AQTMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_ASMARTMO=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMultiMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AQTMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_ASMARTMO=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMultiMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AQTMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_ASMARTMO=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMultiMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AQTMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_ASMARTMO=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMultiMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AQTMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_ASMARTMO=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMultiMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AQTMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_ASMARTMO=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMultiMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AQTMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_ASMARTMO=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMultiMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AQTMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_ASMARTMO=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMultiMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AQTMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_ASMARTMO=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMultiMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AQTMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_ASMARTMO=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMultiMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AQTMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_ASMARTMO=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMultiMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AQTMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_ASMARTMO=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMultiMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AQTMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_ASMARTMO=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMultiMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AQTMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartMovieDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_ASMARTMO=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\AMultiMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\AQTDecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AQTMovieDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartDecoder.h"\
	"..\..\cpp\portable\ndecoder\ASmartMovieDecoder.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\ASmartSceneDecoder.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_ASMARTS=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\A3DSDecoder.h"\
	"..\..\cpp\portable\nick3d\ABspDecoder.h"\
	"..\..\cpp\portable\nick3d\ADXFDecoder.h"\
	"..\..\cpp\portable\nick3d\ALightwaveDecoder.h"\
	"..\..\cpp\portable\nick3d\AMapDecoder.h"\
	"..\..\cpp\portable\nick3d\AMayaDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\ASmartSceneDecoder.h"\
	"..\..\cpp\portable\nick3d\AVRMLDecoder.h"\
	"..\..\cpp\portable\nick3d\AWADDecoder.h"\
	"..\..\cpp\portable\nick3d\AWLDDecoder.h"\
	"..\..\cpp\portable\nick3d\AWolf3dDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_ASMARTS=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\A3DSDecoder.h"\
	"..\..\cpp\portable\nick3d\ABspDecoder.h"\
	"..\..\cpp\portable\nick3d\ADXFDecoder.h"\
	"..\..\cpp\portable\nick3d\ALightwaveDecoder.h"\
	"..\..\cpp\portable\nick3d\AMapDecoder.h"\
	"..\..\cpp\portable\nick3d\AMayaDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\ASmartSceneDecoder.h"\
	"..\..\cpp\portable\nick3d\AVRMLDecoder.h"\
	"..\..\cpp\portable\nick3d\AWADDecoder.h"\
	"..\..\cpp\portable\nick3d\AWLDDecoder.h"\
	"..\..\cpp\portable\nick3d\AWolf3dDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_ASMARTS=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\A3DSDecoder.h"\
	"..\..\cpp\portable\nick3d\ABspDecoder.h"\
	"..\..\cpp\portable\nick3d\ADXFDecoder.h"\
	"..\..\cpp\portable\nick3d\ALightwaveDecoder.h"\
	"..\..\cpp\portable\nick3d\AMapDecoder.h"\
	"..\..\cpp\portable\nick3d\AMayaDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\ASmartSceneDecoder.h"\
	"..\..\cpp\portable\nick3d\AVRMLDecoder.h"\
	"..\..\cpp\portable\nick3d\AWADDecoder.h"\
	"..\..\cpp\portable\nick3d\AWLDDecoder.h"\
	"..\..\cpp\portable\nick3d\AWolf3dDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_ASMARTS=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\A3DSDecoder.h"\
	"..\..\cpp\portable\nick3d\ABspDecoder.h"\
	"..\..\cpp\portable\nick3d\ADXFDecoder.h"\
	"..\..\cpp\portable\nick3d\ALightwaveDecoder.h"\
	"..\..\cpp\portable\nick3d\AMapDecoder.h"\
	"..\..\cpp\portable\nick3d\AMayaDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\ASmartSceneDecoder.h"\
	"..\..\cpp\portable\nick3d\AVRMLDecoder.h"\
	"..\..\cpp\portable\nick3d\AWADDecoder.h"\
	"..\..\cpp\portable\nick3d\AWLDDecoder.h"\
	"..\..\cpp\portable\nick3d\AWolf3dDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_ASMARTS=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\A3DSDecoder.h"\
	"..\..\cpp\portable\nick3d\ABspDecoder.h"\
	"..\..\cpp\portable\nick3d\ADXFDecoder.h"\
	"..\..\cpp\portable\nick3d\ALightwaveDecoder.h"\
	"..\..\cpp\portable\nick3d\AMapDecoder.h"\
	"..\..\cpp\portable\nick3d\AMayaDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\ASmartSceneDecoder.h"\
	"..\..\cpp\portable\nick3d\AVRMLDecoder.h"\
	"..\..\cpp\portable\nick3d\AWADDecoder.h"\
	"..\..\cpp\portable\nick3d\AWLDDecoder.h"\
	"..\..\cpp\portable\nick3d\AWolf3dDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_ASMARTS=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\A3DSDecoder.h"\
	"..\..\cpp\portable\nick3d\ABspDecoder.h"\
	"..\..\cpp\portable\nick3d\ADXFDecoder.h"\
	"..\..\cpp\portable\nick3d\ALightwaveDecoder.h"\
	"..\..\cpp\portable\nick3d\AMapDecoder.h"\
	"..\..\cpp\portable\nick3d\AMayaDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\ASmartSceneDecoder.h"\
	"..\..\cpp\portable\nick3d\AVRMLDecoder.h"\
	"..\..\cpp\portable\nick3d\AWADDecoder.h"\
	"..\..\cpp\portable\nick3d\AWLDDecoder.h"\
	"..\..\cpp\portable\nick3d\AWolf3dDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_ASMARTS=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\A3DSDecoder.h"\
	"..\..\cpp\portable\nick3d\ABspDecoder.h"\
	"..\..\cpp\portable\nick3d\ADXFDecoder.h"\
	"..\..\cpp\portable\nick3d\ALightwaveDecoder.h"\
	"..\..\cpp\portable\nick3d\AMapDecoder.h"\
	"..\..\cpp\portable\nick3d\AMayaDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\ASmartSceneDecoder.h"\
	"..\..\cpp\portable\nick3d\AVRMLDecoder.h"\
	"..\..\cpp\portable\nick3d\AWADDecoder.h"\
	"..\..\cpp\portable\nick3d\AWLDDecoder.h"\
	"..\..\cpp\portable\nick3d\AWolf3dDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_ASMARTS=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\A3DSDecoder.h"\
	"..\..\cpp\portable\nick3d\ABspDecoder.h"\
	"..\..\cpp\portable\nick3d\ADXFDecoder.h"\
	"..\..\cpp\portable\nick3d\ALightwaveDecoder.h"\
	"..\..\cpp\portable\nick3d\AMapDecoder.h"\
	"..\..\cpp\portable\nick3d\AMayaDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\ASmartSceneDecoder.h"\
	"..\..\cpp\portable\nick3d\AVRMLDecoder.h"\
	"..\..\cpp\portable\nick3d\AWADDecoder.h"\
	"..\..\cpp\portable\nick3d\AWLDDecoder.h"\
	"..\..\cpp\portable\nick3d\AWolf3dDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_ASMARTS=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\A3DSDecoder.h"\
	"..\..\cpp\portable\nick3d\ABspDecoder.h"\
	"..\..\cpp\portable\nick3d\ADXFDecoder.h"\
	"..\..\cpp\portable\nick3d\ALightwaveDecoder.h"\
	"..\..\cpp\portable\nick3d\AMapDecoder.h"\
	"..\..\cpp\portable\nick3d\AMayaDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\ASmartSceneDecoder.h"\
	"..\..\cpp\portable\nick3d\AVRMLDecoder.h"\
	"..\..\cpp\portable\nick3d\AWADDecoder.h"\
	"..\..\cpp\portable\nick3d\AWLDDecoder.h"\
	"..\..\cpp\portable\nick3d\AWolf3dDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_ASMARTS=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\A3DSDecoder.h"\
	"..\..\cpp\portable\nick3d\ABspDecoder.h"\
	"..\..\cpp\portable\nick3d\ADXFDecoder.h"\
	"..\..\cpp\portable\nick3d\ALightwaveDecoder.h"\
	"..\..\cpp\portable\nick3d\AMapDecoder.h"\
	"..\..\cpp\portable\nick3d\AMayaDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\ASmartSceneDecoder.h"\
	"..\..\cpp\portable\nick3d\AVRMLDecoder.h"\
	"..\..\cpp\portable\nick3d\AWADDecoder.h"\
	"..\..\cpp\portable\nick3d\AWLDDecoder.h"\
	"..\..\cpp\portable\nick3d\AWolf3dDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_ASMARTS=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\A3DSDecoder.h"\
	"..\..\cpp\portable\nick3d\ABspDecoder.h"\
	"..\..\cpp\portable\nick3d\ADXFDecoder.h"\
	"..\..\cpp\portable\nick3d\ALightwaveDecoder.h"\
	"..\..\cpp\portable\nick3d\AMapDecoder.h"\
	"..\..\cpp\portable\nick3d\AMayaDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\ASmartSceneDecoder.h"\
	"..\..\cpp\portable\nick3d\AVRMLDecoder.h"\
	"..\..\cpp\portable\nick3d\AWADDecoder.h"\
	"..\..\cpp\portable\nick3d\AWLDDecoder.h"\
	"..\..\cpp\portable\nick3d\AWolf3dDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_ASMARTS=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\A3DSDecoder.h"\
	"..\..\cpp\portable\nick3d\ABspDecoder.h"\
	"..\..\cpp\portable\nick3d\ADXFDecoder.h"\
	"..\..\cpp\portable\nick3d\ALightwaveDecoder.h"\
	"..\..\cpp\portable\nick3d\AMapDecoder.h"\
	"..\..\cpp\portable\nick3d\AMayaDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\ASmartSceneDecoder.h"\
	"..\..\cpp\portable\nick3d\AVRMLDecoder.h"\
	"..\..\cpp\portable\nick3d\AWADDecoder.h"\
	"..\..\cpp\portable\nick3d\AWLDDecoder.h"\
	"..\..\cpp\portable\nick3d\AWolf3dDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_ASMARTS=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\A3DSDecoder.h"\
	"..\..\cpp\portable\nick3d\ABspDecoder.h"\
	"..\..\cpp\portable\nick3d\ADXFDecoder.h"\
	"..\..\cpp\portable\nick3d\ALightwaveDecoder.h"\
	"..\..\cpp\portable\nick3d\AMapDecoder.h"\
	"..\..\cpp\portable\nick3d\AMayaDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\ASmartSceneDecoder.h"\
	"..\..\cpp\portable\nick3d\AVRMLDecoder.h"\
	"..\..\cpp\portable\nick3d\AWADDecoder.h"\
	"..\..\cpp\portable\nick3d\AWLDDecoder.h"\
	"..\..\cpp\portable\nick3d\AWolf3dDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_ASMARTS=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\A3DSDecoder.h"\
	"..\..\cpp\portable\nick3d\ABspDecoder.h"\
	"..\..\cpp\portable\nick3d\ADXFDecoder.h"\
	"..\..\cpp\portable\nick3d\ALightwaveDecoder.h"\
	"..\..\cpp\portable\nick3d\AMapDecoder.h"\
	"..\..\cpp\portable\nick3d\AMayaDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\ASmartSceneDecoder.h"\
	"..\..\cpp\portable\nick3d\AVRMLDecoder.h"\
	"..\..\cpp\portable\nick3d\AWADDecoder.h"\
	"..\..\cpp\portable\nick3d\AWLDDecoder.h"\
	"..\..\cpp\portable\nick3d\AWolf3dDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_ASMARTS=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\A3DSDecoder.h"\
	"..\..\cpp\portable\nick3d\ABspDecoder.h"\
	"..\..\cpp\portable\nick3d\ADXFDecoder.h"\
	"..\..\cpp\portable\nick3d\ALightwaveDecoder.h"\
	"..\..\cpp\portable\nick3d\AMapDecoder.h"\
	"..\..\cpp\portable\nick3d\AMayaDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\ASmartSceneDecoder.h"\
	"..\..\cpp\portable\nick3d\AVRMLDecoder.h"\
	"..\..\cpp\portable\nick3d\AWADDecoder.h"\
	"..\..\cpp\portable\nick3d\AWLDDecoder.h"\
	"..\..\cpp\portable\nick3d\AWolf3dDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_ASMARTS=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\A3DSDecoder.h"\
	"..\..\cpp\portable\nick3d\ABspDecoder.h"\
	"..\..\cpp\portable\nick3d\ADXFDecoder.h"\
	"..\..\cpp\portable\nick3d\ALightwaveDecoder.h"\
	"..\..\cpp\portable\nick3d\AMapDecoder.h"\
	"..\..\cpp\portable\nick3d\AMayaDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\ASmartSceneDecoder.h"\
	"..\..\cpp\portable\nick3d\AVRMLDecoder.h"\
	"..\..\cpp\portable\nick3d\AWADDecoder.h"\
	"..\..\cpp\portable\nick3d\AWLDDecoder.h"\
	"..\..\cpp\portable\nick3d\AWolf3dDecoder.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\soundlib\ASmartSongDecoder.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_ASMARTSO=\
	"..\..\cpp\portable\soundlib\AMidiDecoder.h"\
	"..\..\cpp\portable\soundlib\AMultiSongDecoder.h"\
	"..\..\cpp\portable\soundlib\AQTSongDecoder.h"\
	"..\..\cpp\portable\soundlib\ASmartSongDecoder.h"\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_ASMARTSO=\
	"..\..\cpp\portable\soundlib\AMidiDecoder.h"\
	"..\..\cpp\portable\soundlib\AMultiSongDecoder.h"\
	"..\..\cpp\portable\soundlib\AQTSongDecoder.h"\
	"..\..\cpp\portable\soundlib\ASmartSongDecoder.h"\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_ASMARTSO=\
	"..\..\cpp\portable\soundlib\AMidiDecoder.h"\
	"..\..\cpp\portable\soundlib\AMultiSongDecoder.h"\
	"..\..\cpp\portable\soundlib\AQTSongDecoder.h"\
	"..\..\cpp\portable\soundlib\ASmartSongDecoder.h"\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_ASMARTSO=\
	"..\..\cpp\portable\soundlib\AMidiDecoder.h"\
	"..\..\cpp\portable\soundlib\AMultiSongDecoder.h"\
	"..\..\cpp\portable\soundlib\AQTSongDecoder.h"\
	"..\..\cpp\portable\soundlib\ASmartSongDecoder.h"\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_ASMARTSO=\
	"..\..\cpp\portable\soundlib\AMidiDecoder.h"\
	"..\..\cpp\portable\soundlib\AMultiSongDecoder.h"\
	"..\..\cpp\portable\soundlib\AQTSongDecoder.h"\
	"..\..\cpp\portable\soundlib\ASmartSongDecoder.h"\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_ASMARTSO=\
	"..\..\cpp\portable\soundlib\AMidiDecoder.h"\
	"..\..\cpp\portable\soundlib\AMultiSongDecoder.h"\
	"..\..\cpp\portable\soundlib\AQTSongDecoder.h"\
	"..\..\cpp\portable\soundlib\ASmartSongDecoder.h"\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_ASMARTSO=\
	"..\..\cpp\portable\soundlib\AMidiDecoder.h"\
	"..\..\cpp\portable\soundlib\AMultiSongDecoder.h"\
	"..\..\cpp\portable\soundlib\AQTSongDecoder.h"\
	"..\..\cpp\portable\soundlib\ASmartSongDecoder.h"\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_ASMARTSO=\
	"..\..\cpp\portable\soundlib\AMidiDecoder.h"\
	"..\..\cpp\portable\soundlib\AMultiSongDecoder.h"\
	"..\..\cpp\portable\soundlib\AQTSongDecoder.h"\
	"..\..\cpp\portable\soundlib\ASmartSongDecoder.h"\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_ASMARTSO=\
	"..\..\cpp\portable\soundlib\AMidiDecoder.h"\
	"..\..\cpp\portable\soundlib\AMultiSongDecoder.h"\
	"..\..\cpp\portable\soundlib\AQTSongDecoder.h"\
	"..\..\cpp\portable\soundlib\ASmartSongDecoder.h"\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_ASMARTSO=\
	"..\..\cpp\portable\soundlib\AMidiDecoder.h"\
	"..\..\cpp\portable\soundlib\AMultiSongDecoder.h"\
	"..\..\cpp\portable\soundlib\AQTSongDecoder.h"\
	"..\..\cpp\portable\soundlib\ASmartSongDecoder.h"\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_ASMARTSO=\
	"..\..\cpp\portable\soundlib\AMidiDecoder.h"\
	"..\..\cpp\portable\soundlib\AMultiSongDecoder.h"\
	"..\..\cpp\portable\soundlib\AQTSongDecoder.h"\
	"..\..\cpp\portable\soundlib\ASmartSongDecoder.h"\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_ASMARTSO=\
	"..\..\cpp\portable\soundlib\AMidiDecoder.h"\
	"..\..\cpp\portable\soundlib\AMultiSongDecoder.h"\
	"..\..\cpp\portable\soundlib\AQTSongDecoder.h"\
	"..\..\cpp\portable\soundlib\ASmartSongDecoder.h"\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_ASMARTSO=\
	"..\..\cpp\portable\soundlib\AMidiDecoder.h"\
	"..\..\cpp\portable\soundlib\AMultiSongDecoder.h"\
	"..\..\cpp\portable\soundlib\AQTSongDecoder.h"\
	"..\..\cpp\portable\soundlib\ASmartSongDecoder.h"\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_ASMARTSO=\
	"..\..\cpp\portable\soundlib\AMidiDecoder.h"\
	"..\..\cpp\portable\soundlib\AMultiSongDecoder.h"\
	"..\..\cpp\portable\soundlib\AQTSongDecoder.h"\
	"..\..\cpp\portable\soundlib\ASmartSongDecoder.h"\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_ASMARTSO=\
	"..\..\cpp\portable\soundlib\AMidiDecoder.h"\
	"..\..\cpp\portable\soundlib\AMultiSongDecoder.h"\
	"..\..\cpp\portable\soundlib\AQTSongDecoder.h"\
	"..\..\cpp\portable\soundlib\ASmartSongDecoder.h"\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_ASMARTSO=\
	"..\..\cpp\portable\soundlib\AMidiDecoder.h"\
	"..\..\cpp\portable\soundlib\AMultiSongDecoder.h"\
	"..\..\cpp\portable\soundlib\AQTSongDecoder.h"\
	"..\..\cpp\portable\soundlib\ASmartSongDecoder.h"\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\soundlib\ASmartSoundDecoder.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_ASMARTSOU=\
	"..\..\cpp\portable\soundlib\A8SVXDecoder.h"\
	"..\..\cpp\portable\soundlib\AAUDecoder.h"\
	"..\..\cpp\portable\soundlib\AMP3Decoder.h"\
	"..\..\cpp\portable\soundlib\AMultiSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\AQTSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\ASmartSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	"..\..\cpp\portable\soundlib\AWAVDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_ASMARTSOU=\
	"..\..\cpp\portable\soundlib\A8SVXDecoder.h"\
	"..\..\cpp\portable\soundlib\AAUDecoder.h"\
	"..\..\cpp\portable\soundlib\AMP3Decoder.h"\
	"..\..\cpp\portable\soundlib\AMultiSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\AQTSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\ASmartSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	"..\..\cpp\portable\soundlib\AWAVDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_ASMARTSOU=\
	"..\..\cpp\portable\soundlib\A8SVXDecoder.h"\
	"..\..\cpp\portable\soundlib\AAUDecoder.h"\
	"..\..\cpp\portable\soundlib\AMP3Decoder.h"\
	"..\..\cpp\portable\soundlib\AMultiSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\AQTSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\ASmartSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	"..\..\cpp\portable\soundlib\AWAVDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_ASMARTSOU=\
	"..\..\cpp\portable\soundlib\A8SVXDecoder.h"\
	"..\..\cpp\portable\soundlib\AAUDecoder.h"\
	"..\..\cpp\portable\soundlib\AMP3Decoder.h"\
	"..\..\cpp\portable\soundlib\AMultiSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\AQTSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\ASmartSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	"..\..\cpp\portable\soundlib\AWAVDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_ASMARTSOU=\
	"..\..\cpp\portable\soundlib\A8SVXDecoder.h"\
	"..\..\cpp\portable\soundlib\AAUDecoder.h"\
	"..\..\cpp\portable\soundlib\AMP3Decoder.h"\
	"..\..\cpp\portable\soundlib\AMultiSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\AQTSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\ASmartSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	"..\..\cpp\portable\soundlib\AWAVDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_ASMARTSOU=\
	"..\..\cpp\portable\soundlib\A8SVXDecoder.h"\
	"..\..\cpp\portable\soundlib\AAUDecoder.h"\
	"..\..\cpp\portable\soundlib\AMP3Decoder.h"\
	"..\..\cpp\portable\soundlib\AMultiSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\AQTSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\ASmartSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	"..\..\cpp\portable\soundlib\AWAVDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_ASMARTSOU=\
	"..\..\cpp\portable\soundlib\A8SVXDecoder.h"\
	"..\..\cpp\portable\soundlib\AAUDecoder.h"\
	"..\..\cpp\portable\soundlib\AMP3Decoder.h"\
	"..\..\cpp\portable\soundlib\AMultiSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\AQTSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\ASmartSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	"..\..\cpp\portable\soundlib\AWAVDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_ASMARTSOU=\
	"..\..\cpp\portable\soundlib\A8SVXDecoder.h"\
	"..\..\cpp\portable\soundlib\AAUDecoder.h"\
	"..\..\cpp\portable\soundlib\AMP3Decoder.h"\
	"..\..\cpp\portable\soundlib\AMultiSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\AQTSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\ASmartSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	"..\..\cpp\portable\soundlib\AWAVDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_ASMARTSOU=\
	"..\..\cpp\portable\soundlib\A8SVXDecoder.h"\
	"..\..\cpp\portable\soundlib\AAUDecoder.h"\
	"..\..\cpp\portable\soundlib\AMP3Decoder.h"\
	"..\..\cpp\portable\soundlib\AMultiSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\AQTSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\ASmartSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	"..\..\cpp\portable\soundlib\AWAVDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_ASMARTSOU=\
	"..\..\cpp\portable\soundlib\A8SVXDecoder.h"\
	"..\..\cpp\portable\soundlib\AAUDecoder.h"\
	"..\..\cpp\portable\soundlib\AMP3Decoder.h"\
	"..\..\cpp\portable\soundlib\AMultiSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\AQTSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\ASmartSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	"..\..\cpp\portable\soundlib\AWAVDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_ASMARTSOU=\
	"..\..\cpp\portable\soundlib\A8SVXDecoder.h"\
	"..\..\cpp\portable\soundlib\AAUDecoder.h"\
	"..\..\cpp\portable\soundlib\AMP3Decoder.h"\
	"..\..\cpp\portable\soundlib\AMultiSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\AQTSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\ASmartSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	"..\..\cpp\portable\soundlib\AWAVDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_ASMARTSOU=\
	"..\..\cpp\portable\soundlib\A8SVXDecoder.h"\
	"..\..\cpp\portable\soundlib\AAUDecoder.h"\
	"..\..\cpp\portable\soundlib\AMP3Decoder.h"\
	"..\..\cpp\portable\soundlib\AMultiSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\AQTSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\ASmartSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	"..\..\cpp\portable\soundlib\AWAVDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_ASMARTSOU=\
	"..\..\cpp\portable\soundlib\A8SVXDecoder.h"\
	"..\..\cpp\portable\soundlib\AAUDecoder.h"\
	"..\..\cpp\portable\soundlib\AMP3Decoder.h"\
	"..\..\cpp\portable\soundlib\AMultiSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\AQTSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\ASmartSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	"..\..\cpp\portable\soundlib\AWAVDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_ASMARTSOU=\
	"..\..\cpp\portable\soundlib\A8SVXDecoder.h"\
	"..\..\cpp\portable\soundlib\AAUDecoder.h"\
	"..\..\cpp\portable\soundlib\AMP3Decoder.h"\
	"..\..\cpp\portable\soundlib\AMultiSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\AQTSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\ASmartSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	"..\..\cpp\portable\soundlib\AWAVDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_ASMARTSOU=\
	"..\..\cpp\portable\soundlib\A8SVXDecoder.h"\
	"..\..\cpp\portable\soundlib\AAUDecoder.h"\
	"..\..\cpp\portable\soundlib\AMP3Decoder.h"\
	"..\..\cpp\portable\soundlib\AMultiSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\AQTSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\ASmartSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	"..\..\cpp\portable\soundlib\AWAVDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_ASMARTSOU=\
	"..\..\cpp\portable\soundlib\A8SVXDecoder.h"\
	"..\..\cpp\portable\soundlib\AAUDecoder.h"\
	"..\..\cpp\portable\soundlib\AMP3Decoder.h"\
	"..\..\cpp\portable\soundlib\AMultiSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\AQTSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\ASmartSoundDecoder.h"\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	"..\..\cpp\portable\soundlib\AWAVDecoder.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\netlib\ASocket.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_ASOCK=\
	"..\..\cpp\portable\netlib\AHost.h"\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	"..\..\cpp\portable\netlib\ASocket.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_ASOCK=\
	"..\..\cpp\portable\netlib\AHost.h"\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	"..\..\cpp\portable\netlib\ASocket.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_ASOCK=\
	"..\..\cpp\portable\netlib\AHost.h"\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	"..\..\cpp\portable\netlib\ASocket.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_ASOCK=\
	"..\..\cpp\portable\netlib\AHost.h"\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	"..\..\cpp\portable\netlib\ASocket.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_ASOCK=\
	"..\..\cpp\portable\netlib\AHost.h"\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	"..\..\cpp\portable\netlib\ASocket.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_ASOCK=\
	"..\..\cpp\portable\netlib\AHost.h"\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	"..\..\cpp\portable\netlib\ASocket.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_ASOCK=\
	"..\..\cpp\portable\netlib\AHost.h"\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	"..\..\cpp\portable\netlib\ASocket.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_ASOCK=\
	"..\..\cpp\portable\netlib\AHost.h"\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	"..\..\cpp\portable\netlib\ASocket.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_ASOCK=\
	"..\..\cpp\portable\netlib\AHost.h"\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	"..\..\cpp\portable\netlib\ASocket.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_ASOCK=\
	"..\..\cpp\portable\netlib\AHost.h"\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	"..\..\cpp\portable\netlib\ASocket.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_ASOCK=\
	"..\..\cpp\portable\netlib\AHost.h"\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	"..\..\cpp\portable\netlib\ASocket.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_ASOCK=\
	"..\..\cpp\portable\netlib\AHost.h"\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	"..\..\cpp\portable\netlib\ASocket.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_ASOCK=\
	"..\..\cpp\portable\netlib\AHost.h"\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	"..\..\cpp\portable\netlib\ASocket.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_ASOCK=\
	"..\..\cpp\portable\netlib\AHost.h"\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	"..\..\cpp\portable\netlib\ASocket.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_ASOCK=\
	"..\..\cpp\portable\netlib\AHost.h"\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	"..\..\cpp\portable\netlib\ASocket.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_ASOCK=\
	"..\..\cpp\portable\netlib\AHost.h"\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	"..\..\cpp\portable\netlib\ASocket.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\netlib\ASocketNonBlocking.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_ASOCKE=\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	"..\..\cpp\portable\netlib\ASocket.h"\
	"..\..\cpp\portable\netlib\ASocketNonBlocking.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_ASOCKE=\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	"..\..\cpp\portable\netlib\ASocket.h"\
	"..\..\cpp\portable\netlib\ASocketNonBlocking.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_ASOCKE=\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	"..\..\cpp\portable\netlib\ASocket.h"\
	"..\..\cpp\portable\netlib\ASocketNonBlocking.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_ASOCKE=\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	"..\..\cpp\portable\netlib\ASocket.h"\
	"..\..\cpp\portable\netlib\ASocketNonBlocking.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_ASOCKE=\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	"..\..\cpp\portable\netlib\ASocket.h"\
	"..\..\cpp\portable\netlib\ASocketNonBlocking.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_ASOCKE=\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	"..\..\cpp\portable\netlib\ASocket.h"\
	"..\..\cpp\portable\netlib\ASocketNonBlocking.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_ASOCKE=\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	"..\..\cpp\portable\netlib\ASocket.h"\
	"..\..\cpp\portable\netlib\ASocketNonBlocking.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_ASOCKE=\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	"..\..\cpp\portable\netlib\ASocket.h"\
	"..\..\cpp\portable\netlib\ASocketNonBlocking.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_ASOCKE=\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	"..\..\cpp\portable\netlib\ASocket.h"\
	"..\..\cpp\portable\netlib\ASocketNonBlocking.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_ASOCKE=\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	"..\..\cpp\portable\netlib\ASocket.h"\
	"..\..\cpp\portable\netlib\ASocketNonBlocking.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_ASOCKE=\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	"..\..\cpp\portable\netlib\ASocket.h"\
	"..\..\cpp\portable\netlib\ASocketNonBlocking.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_ASOCKE=\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	"..\..\cpp\portable\netlib\ASocket.h"\
	"..\..\cpp\portable\netlib\ASocketNonBlocking.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_ASOCKE=\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	"..\..\cpp\portable\netlib\ASocket.h"\
	"..\..\cpp\portable\netlib\ASocketNonBlocking.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_ASOCKE=\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	"..\..\cpp\portable\netlib\ASocket.h"\
	"..\..\cpp\portable\netlib\ASocketNonBlocking.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_ASOCKE=\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	"..\..\cpp\portable\netlib\ASocket.h"\
	"..\..\cpp\portable\netlib\ASocketNonBlocking.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_ASOCKE=\
	"..\..\cpp\portable\netlib\ANetAbstract.h"\
	"..\..\cpp\portable\netlib\ASocket.h"\
	"..\..\cpp\portable\netlib\ASocketNonBlocking.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\soundlib\ASong.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_ASONG=\
	"..\..\cpp\portable\soundlib\AAudioDevice.h"\
	"..\..\cpp\portable\soundlib\ASong.h"\
	"..\..\cpp\portable\soundlib\ASoundAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_ASONG=\
	"..\..\cpp\portable\soundlib\AAudioDevice.h"\
	"..\..\cpp\portable\soundlib\ASong.h"\
	"..\..\cpp\portable\soundlib\ASoundAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_ASONG=\
	"..\..\cpp\portable\soundlib\AAudioDevice.h"\
	"..\..\cpp\portable\soundlib\ASong.h"\
	"..\..\cpp\portable\soundlib\ASoundAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_ASONG=\
	"..\..\cpp\portable\soundlib\AAudioDevice.h"\
	"..\..\cpp\portable\soundlib\ASong.h"\
	"..\..\cpp\portable\soundlib\ASoundAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_ASONG=\
	"..\..\cpp\portable\soundlib\AAudioDevice.h"\
	"..\..\cpp\portable\soundlib\ASong.h"\
	"..\..\cpp\portable\soundlib\ASoundAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_ASONG=\
	"..\..\cpp\portable\soundlib\AAudioDevice.h"\
	"..\..\cpp\portable\soundlib\ASong.h"\
	"..\..\cpp\portable\soundlib\ASoundAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_ASONG=\
	"..\..\cpp\portable\soundlib\AAudioDevice.h"\
	"..\..\cpp\portable\soundlib\ASong.h"\
	"..\..\cpp\portable\soundlib\ASoundAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_ASONG=\
	"..\..\cpp\portable\soundlib\AAudioDevice.h"\
	"..\..\cpp\portable\soundlib\ASong.h"\
	"..\..\cpp\portable\soundlib\ASoundAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_ASONG=\
	"..\..\cpp\portable\soundlib\AAudioDevice.h"\
	"..\..\cpp\portable\soundlib\ASong.h"\
	"..\..\cpp\portable\soundlib\ASoundAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_ASONG=\
	"..\..\cpp\portable\soundlib\AAudioDevice.h"\
	"..\..\cpp\portable\soundlib\ASong.h"\
	"..\..\cpp\portable\soundlib\ASoundAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_ASONG=\
	"..\..\cpp\portable\soundlib\AAudioDevice.h"\
	"..\..\cpp\portable\soundlib\ASong.h"\
	"..\..\cpp\portable\soundlib\ASoundAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_ASONG=\
	"..\..\cpp\portable\soundlib\AAudioDevice.h"\
	"..\..\cpp\portable\soundlib\ASong.h"\
	"..\..\cpp\portable\soundlib\ASoundAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_ASONG=\
	"..\..\cpp\portable\soundlib\AAudioDevice.h"\
	"..\..\cpp\portable\soundlib\ASong.h"\
	"..\..\cpp\portable\soundlib\ASoundAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_ASONG=\
	"..\..\cpp\portable\soundlib\AAudioDevice.h"\
	"..\..\cpp\portable\soundlib\ASong.h"\
	"..\..\cpp\portable\soundlib\ASoundAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_ASONG=\
	"..\..\cpp\portable\soundlib\AAudioDevice.h"\
	"..\..\cpp\portable\soundlib\ASong.h"\
	"..\..\cpp\portable\soundlib\ASoundAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_ASONG=\
	"..\..\cpp\portable\soundlib\AAudioDevice.h"\
	"..\..\cpp\portable\soundlib\ASong.h"\
	"..\..\cpp\portable\soundlib\ASoundAbstract.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\soundlib\ASongDecoder.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_ASONGD=\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_ASONGD=\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_ASONGD=\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_ASONGD=\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_ASONGD=\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_ASONGD=\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_ASONGD=\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_ASONGD=\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_ASONGD=\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_ASONGD=\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_ASONGD=\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_ASONGD=\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_ASONGD=\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_ASONGD=\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_ASONGD=\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_ASONGD=\
	"..\..\cpp\portable\soundlib\ASongDecoder.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\soundlib\ASound.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_ASOUN=\
	"..\..\cpp\portable\soundlib\AAudioDevice.h"\
	"..\..\cpp\portable\soundlib\ASound.h"\
	"..\..\cpp\portable\soundlib\ASoundAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_ASOUN=\
	"..\..\cpp\portable\soundlib\AAudioDevice.h"\
	"..\..\cpp\portable\soundlib\ASound.h"\
	"..\..\cpp\portable\soundlib\ASoundAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_ASOUN=\
	"..\..\cpp\portable\soundlib\AAudioDevice.h"\
	"..\..\cpp\portable\soundlib\ASound.h"\
	"..\..\cpp\portable\soundlib\ASoundAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_ASOUN=\
	"..\..\cpp\portable\soundlib\AAudioDevice.h"\
	"..\..\cpp\portable\soundlib\ASound.h"\
	"..\..\cpp\portable\soundlib\ASoundAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_ASOUN=\
	"..\..\cpp\portable\soundlib\AAudioDevice.h"\
	"..\..\cpp\portable\soundlib\ASound.h"\
	"..\..\cpp\portable\soundlib\ASoundAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_ASOUN=\
	"..\..\cpp\portable\soundlib\AAudioDevice.h"\
	"..\..\cpp\portable\soundlib\ASound.h"\
	"..\..\cpp\portable\soundlib\ASoundAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_ASOUN=\
	"..\..\cpp\portable\soundlib\AAudioDevice.h"\
	"..\..\cpp\portable\soundlib\ASound.h"\
	"..\..\cpp\portable\soundlib\ASoundAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_ASOUN=\
	"..\..\cpp\portable\soundlib\AAudioDevice.h"\
	"..\..\cpp\portable\soundlib\ASound.h"\
	"..\..\cpp\portable\soundlib\ASoundAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_ASOUN=\
	"..\..\cpp\portable\soundlib\AAudioDevice.h"\
	"..\..\cpp\portable\soundlib\ASound.h"\
	"..\..\cpp\portable\soundlib\ASoundAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_ASOUN=\
	"..\..\cpp\portable\soundlib\AAudioDevice.h"\
	"..\..\cpp\portable\soundlib\ASound.h"\
	"..\..\cpp\portable\soundlib\ASoundAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_ASOUN=\
	"..\..\cpp\portable\soundlib\AAudioDevice.h"\
	"..\..\cpp\portable\soundlib\ASound.h"\
	"..\..\cpp\portable\soundlib\ASoundAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_ASOUN=\
	"..\..\cpp\portable\soundlib\AAudioDevice.h"\
	"..\..\cpp\portable\soundlib\ASound.h"\
	"..\..\cpp\portable\soundlib\ASoundAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_ASOUN=\
	"..\..\cpp\portable\soundlib\AAudioDevice.h"\
	"..\..\cpp\portable\soundlib\ASound.h"\
	"..\..\cpp\portable\soundlib\ASoundAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_ASOUN=\
	"..\..\cpp\portable\soundlib\AAudioDevice.h"\
	"..\..\cpp\portable\soundlib\ASound.h"\
	"..\..\cpp\portable\soundlib\ASoundAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_ASOUN=\
	"..\..\cpp\portable\soundlib\AAudioDevice.h"\
	"..\..\cpp\portable\soundlib\ASound.h"\
	"..\..\cpp\portable\soundlib\ASoundAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_ASOUN=\
	"..\..\cpp\portable\soundlib\AAudioDevice.h"\
	"..\..\cpp\portable\soundlib\ASound.h"\
	"..\..\cpp\portable\soundlib\ASoundAbstract.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\soundlib\ASoundAbstract.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_ASOUND=\
	"..\..\cpp\portable\soundlib\ASong.h"\
	"..\..\cpp\portable\soundlib\ASound.h"\
	"..\..\cpp\portable\soundlib\ASoundAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_ASOUND=\
	"..\..\cpp\portable\soundlib\ASong.h"\
	"..\..\cpp\portable\soundlib\ASound.h"\
	"..\..\cpp\portable\soundlib\ASoundAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_ASOUND=\
	"..\..\cpp\portable\soundlib\ASong.h"\
	"..\..\cpp\portable\soundlib\ASound.h"\
	"..\..\cpp\portable\soundlib\ASoundAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_ASOUND=\
	"..\..\cpp\portable\soundlib\ASong.h"\
	"..\..\cpp\portable\soundlib\ASound.h"\
	"..\..\cpp\portable\soundlib\ASoundAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_ASOUND=\
	"..\..\cpp\portable\soundlib\ASong.h"\
	"..\..\cpp\portable\soundlib\ASound.h"\
	"..\..\cpp\portable\soundlib\ASoundAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_ASOUND=\
	"..\..\cpp\portable\soundlib\ASong.h"\
	"..\..\cpp\portable\soundlib\ASound.h"\
	"..\..\cpp\portable\soundlib\ASoundAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_ASOUND=\
	"..\..\cpp\portable\soundlib\ASong.h"\
	"..\..\cpp\portable\soundlib\ASound.h"\
	"..\..\cpp\portable\soundlib\ASoundAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_ASOUND=\
	"..\..\cpp\portable\soundlib\ASong.h"\
	"..\..\cpp\portable\soundlib\ASound.h"\
	"..\..\cpp\portable\soundlib\ASoundAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_ASOUND=\
	"..\..\cpp\portable\soundlib\ASong.h"\
	"..\..\cpp\portable\soundlib\ASound.h"\
	"..\..\cpp\portable\soundlib\ASoundAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_ASOUND=\
	"..\..\cpp\portable\soundlib\ASong.h"\
	"..\..\cpp\portable\soundlib\ASound.h"\
	"..\..\cpp\portable\soundlib\ASoundAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_ASOUND=\
	"..\..\cpp\portable\soundlib\ASong.h"\
	"..\..\cpp\portable\soundlib\ASound.h"\
	"..\..\cpp\portable\soundlib\ASoundAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_ASOUND=\
	"..\..\cpp\portable\soundlib\ASong.h"\
	"..\..\cpp\portable\soundlib\ASound.h"\
	"..\..\cpp\portable\soundlib\ASoundAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_ASOUND=\
	"..\..\cpp\portable\soundlib\ASong.h"\
	"..\..\cpp\portable\soundlib\ASound.h"\
	"..\..\cpp\portable\soundlib\ASoundAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_ASOUND=\
	"..\..\cpp\portable\soundlib\ASong.h"\
	"..\..\cpp\portable\soundlib\ASound.h"\
	"..\..\cpp\portable\soundlib\ASoundAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_ASOUND=\
	"..\..\cpp\portable\soundlib\ASong.h"\
	"..\..\cpp\portable\soundlib\ASound.h"\
	"..\..\cpp\portable\soundlib\ASoundAbstract.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_ASOUND=\
	"..\..\cpp\portable\soundlib\ASong.h"\
	"..\..\cpp\portable\soundlib\ASound.h"\
	"..\..\cpp\portable\soundlib\ASoundAbstract.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\soundlib\ASoundDecoder.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_ASOUNDD=\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_ASOUNDD=\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_ASOUNDD=\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_ASOUNDD=\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_ASOUNDD=\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_ASOUNDD=\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_ASOUNDD=\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_ASOUNDD=\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_ASOUNDD=\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_ASOUNDD=\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_ASOUNDD=\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_ASOUNDD=\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_ASOUNDD=\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_ASOUNDD=\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_ASOUNDD=\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_ASOUNDD=\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nickcpp\AStringHelper.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_ASTRI=\
	"..\..\cpp\portable\nickcpp\AStringHelper.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_ASTRI=\
	"..\..\cpp\portable\nickcpp\AStringHelper.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_ASTRI=\
	"..\..\cpp\portable\nickcpp\AStringHelper.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_ASTRI=\
	"..\..\cpp\portable\nickcpp\AStringHelper.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_ASTRI=\
	"..\..\cpp\portable\nickcpp\AStringHelper.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_ASTRI=\
	"..\..\cpp\portable\nickcpp\AStringHelper.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_ASTRI=\
	"..\..\cpp\portable\nickcpp\AStringHelper.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_ASTRI=\
	"..\..\cpp\portable\nickcpp\AStringHelper.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_ASTRI=\
	"..\..\cpp\portable\nickcpp\AStringHelper.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_ASTRI=\
	"..\..\cpp\portable\nickcpp\AStringHelper.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_ASTRI=\
	"..\..\cpp\portable\nickcpp\AStringHelper.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_ASTRI=\
	"..\..\cpp\portable\nickcpp\AStringHelper.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_ASTRI=\
	"..\..\cpp\portable\nickcpp\AStringHelper.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_ASTRI=\
	"..\..\cpp\portable\nickcpp\AStringHelper.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_ASTRI=\
	"..\..\cpp\portable\nickcpp\AStringHelper.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_ASTRI=\
	"..\..\cpp\portable\nickcpp\AStringHelper.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\ATextCoord3D.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_ATEXT=\
	"..\..\cpp\portable\nick3d\ATextCoord3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_ATEXT=\
	"..\..\cpp\portable\nick3d\ATextCoord3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_ATEXT=\
	"..\..\cpp\portable\nick3d\ATextCoord3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_ATEXT=\
	"..\..\cpp\portable\nick3d\ATextCoord3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_ATEXT=\
	"..\..\cpp\portable\nick3d\ATextCoord3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_ATEXT=\
	"..\..\cpp\portable\nick3d\ATextCoord3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_ATEXT=\
	"..\..\cpp\portable\nick3d\ATextCoord3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_ATEXT=\
	"..\..\cpp\portable\nick3d\ATextCoord3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_ATEXT=\
	"..\..\cpp\portable\nick3d\ATextCoord3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_ATEXT=\
	"..\..\cpp\portable\nick3d\ATextCoord3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_ATEXT=\
	"..\..\cpp\portable\nick3d\ATextCoord3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_ATEXT=\
	"..\..\cpp\portable\nick3d\ATextCoord3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_ATEXT=\
	"..\..\cpp\portable\nick3d\ATextCoord3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_ATEXT=\
	"..\..\cpp\portable\nick3d\ATextCoord3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_ATEXT=\
	"..\..\cpp\portable\nick3d\ATextCoord3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_ATEXT=\
	"..\..\cpp\portable\nick3d\ATextCoord3D.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\ATexture3D.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_ATEXTU=\
	"..\..\cpp\portable\nick3d\ATexture3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_ATEXTU=\
	"..\..\cpp\portable\nick3d\ATexture3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_ATEXTU=\
	"..\..\cpp\portable\nick3d\ATexture3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_ATEXTU=\
	"..\..\cpp\portable\nick3d\ATexture3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_ATEXTU=\
	"..\..\cpp\portable\nick3d\ATexture3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_ATEXTU=\
	"..\..\cpp\portable\nick3d\ATexture3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_ATEXTU=\
	"..\..\cpp\portable\nick3d\ATexture3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_ATEXTU=\
	"..\..\cpp\portable\nick3d\ATexture3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_ATEXTU=\
	"..\..\cpp\portable\nick3d\ATexture3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_ATEXTU=\
	"..\..\cpp\portable\nick3d\ATexture3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_ATEXTU=\
	"..\..\cpp\portable\nick3d\ATexture3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_ATEXTU=\
	"..\..\cpp\portable\nick3d\ATexture3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_ATEXTU=\
	"..\..\cpp\portable\nick3d\ATexture3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_ATEXTU=\
	"..\..\cpp\portable\nick3d\ATexture3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_ATEXTU=\
	"..\..\cpp\portable\nick3d\ATexture3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_ATEXTU=\
	"..\..\cpp\portable\nick3d\ATexture3D.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\ndecoder\ATGADecoder.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_ATGAD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\ATGADecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_ATGAD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\ATGADecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_ATGAD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\ATGADecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_ATGAD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\ATGADecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_ATGAD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\ATGADecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_ATGAD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\ATGADecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_ATGAD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\ATGADecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_ATGAD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\ATGADecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_ATGAD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\ATGADecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_ATGAD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\ATGADecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_ATGAD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\ATGADecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_ATGAD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\ATGADecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_ATGAD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\ATGADecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_ATGAD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\ATGADecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_ATGAD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\ATGADecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_ATGAD=\
	"..\..\cpp\portable\ndecoder\ADecoder.h"\
	"..\..\cpp\portable\ndecoder\ADecoderHelper.h"\
	"..\..\cpp\portable\ndecoder\AImageDecoder.h"\
	"..\..\cpp\portable\ndecoder\ATGADecoder.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\AVertex3D.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_AVERT=\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_AVERT=\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_AVERT=\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_AVERT=\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_AVERT=\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_AVERT=\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_AVERT=\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_AVERT=\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_AVERT=\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_AVERT=\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_AVERT=\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_AVERT=\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_AVERT=\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_AVERT=\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_AVERT=\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_AVERT=\
	"..\..\cpp\portable\nick3d\AVertex3D.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\AVRMLDecoder.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_AVRML=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\AVRMLDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_AVRML=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\AVRMLDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_AVRML=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\AVRMLDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_AVRML=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\AVRMLDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_AVRML=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\AVRMLDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_AVRML=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\AVRMLDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_AVRML=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\AVRMLDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_AVRML=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\AVRMLDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_AVRML=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\AVRMLDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_AVRML=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\AVRMLDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_AVRML=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\AVRMLDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_AVRML=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\AVRMLDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_AVRML=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\AVRMLDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_AVRML=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\AVRMLDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_AVRML=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\AVRMLDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_AVRML=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\AVRMLDecoder.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\AWADDecoder.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_AWADD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\AWADDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_AWADD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\AWADDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_AWADD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\AWADDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_AWADD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\AWADDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_AWADD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\AWADDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_AWADD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\AWADDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_AWADD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\AWADDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_AWADD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\AWADDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_AWADD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\AWADDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_AWADD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\AWADDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_AWADD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\AWADDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_AWADD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\AWADDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_AWADD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\AWADDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_AWADD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\AWADDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_AWADD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\AWADDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_AWADD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\AWADDecoder.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\soundlib\AWAVDecoder.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_AWAVD=\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	"..\..\cpp\portable\soundlib\AWAVDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_AWAVD=\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	"..\..\cpp\portable\soundlib\AWAVDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_AWAVD=\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	"..\..\cpp\portable\soundlib\AWAVDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_AWAVD=\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	"..\..\cpp\portable\soundlib\AWAVDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_AWAVD=\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	"..\..\cpp\portable\soundlib\AWAVDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_AWAVD=\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	"..\..\cpp\portable\soundlib\AWAVDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_AWAVD=\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	"..\..\cpp\portable\soundlib\AWAVDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_AWAVD=\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	"..\..\cpp\portable\soundlib\AWAVDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_AWAVD=\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	"..\..\cpp\portable\soundlib\AWAVDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_AWAVD=\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	"..\..\cpp\portable\soundlib\AWAVDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_AWAVD=\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	"..\..\cpp\portable\soundlib\AWAVDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_AWAVD=\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	"..\..\cpp\portable\soundlib\AWAVDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_AWAVD=\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	"..\..\cpp\portable\soundlib\AWAVDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_AWAVD=\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	"..\..\cpp\portable\soundlib\AWAVDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_AWAVD=\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	"..\..\cpp\portable\soundlib\AWAVDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_AWAVD=\
	"..\..\cpp\portable\soundlib\ASoundDecoder.h"\
	"..\..\cpp\portable\soundlib\AWAVDecoder.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\AWavefrontDecoder.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_AWAVE=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\AWavefrontDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_AWAVE=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\AWavefrontDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_AWAVE=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\AWavefrontDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_AWAVE=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\AWavefrontDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_AWAVE=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\AWavefrontDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_AWAVE=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\AWavefrontDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_AWAVE=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\AWavefrontDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_AWAVE=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\AWavefrontDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_AWAVE=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\AWavefrontDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_AWAVE=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\AWavefrontDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_AWAVE=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\AWavefrontDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_AWAVE=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\AWavefrontDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_AWAVE=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\AWavefrontDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_AWAVE=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\AWavefrontDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_AWAVE=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\AWavefrontDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_AWAVE=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\AWavefrontDecoder.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\AWLDDecoder.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_AWLDD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\ASmartMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\AWLDDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_AWLDD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\ASmartMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\AWLDDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_AWLDD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\ASmartMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\AWLDDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_AWLDD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\ASmartMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\AWLDDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_AWLDD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\ASmartMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\AWLDDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_AWLDD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\ASmartMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\AWLDDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_AWLDD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\ASmartMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\AWLDDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_AWLDD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\ASmartMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\AWLDDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_AWLDD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\ASmartMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\AWLDDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_AWLDD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\ASmartMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\AWLDDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_AWLDD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\ASmartMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\AWLDDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_AWLDD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\ASmartMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\AWLDDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_AWLDD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\ASmartMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\AWLDDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_AWLDD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\ASmartMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\AWLDDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_AWLDD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\ASmartMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\AWLDDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_AWLDD=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\AMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\ASmartMeshDecoder.h"\
	"..\..\cpp\portable\nick3d\AWLDDecoder.h"\
	

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\AWolf3dDecoder.cpp

!IF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Release"

DEP_CPP_AWOLF=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\AWolf3dDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPS) Debug"

DEP_CPP_AWOLF=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\AWolf3dDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Release"

DEP_CPP_AWOLF=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\AWolf3dDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH4) Debug"

DEP_CPP_AWOLF=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\AWolf3dDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Release"

DEP_CPP_AWOLF=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\AWolf3dDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE SH3) Debug"

DEP_CPP_AWOLF=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\AWolf3dDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Release"

DEP_CPP_AWOLF=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\AWolf3dDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE ARM) Debug"

DEP_CPP_AWOLF=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\AWolf3dDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Release"

DEP_CPP_AWOLF=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\AWolf3dDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE MIPSFP) Debug"

DEP_CPP_AWOLF=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\AWolf3dDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Release"

DEP_CPP_AWOLF=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\AWolf3dDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86) Debug"

DEP_CPP_AWOLF=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\AWolf3dDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Release"

DEP_CPP_AWOLF=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\AWolf3dDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE x86em) Debug"

DEP_CPP_AWOLF=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\AWolf3dDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Release"

DEP_CPP_AWOLF=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\AWolf3dDecoder.h"\
	

!ELSEIF  "$(CFG)" == "cpp_portable_static_wce - Win32 (WCE PPC) Debug"

DEP_CPP_AWOLF=\
	"..\..\cpp\portable\nick3d\A3DDecoder.h"\
	"..\..\cpp\portable\nick3d\ASceneDecoder.h"\
	"..\..\cpp\portable\nick3d\AWolf3dDecoder.h"\
	

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\A3DDecoder.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\A3DSDecoder.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\soundlib\A8SVXDecoder.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nickcpp\AAbsRect.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\soundlib\AAUDecoder.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\soundlib\AAudioDevice.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nickcpp\ABitmap.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nickcpp\ABitmapAbstract.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\ndecoder\ABMPDecoder.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\ABspDecoder.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\ACamera3D.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nickcpp\ACell.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nickcpp\AClock.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nickcpp\AColor.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nickcpp\AConvertingBitLine.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nickcpp\ADataSourceAbstract.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\ndecoder\ADecoder.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\ndecoder\ADecoderHelper.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nickcpp\ADirection2d.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nickcpp\ADrawableAbstract.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\ADXFDecoder.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nickcpp\AError.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\AFace3D.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nickcpp\AFileInputStream.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nickcpp\AFileOutputStream.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nickcpp\AFrameAbstract.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nickcpp\AGenericNode.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\ndecoder\AGIFDecoder.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\AHalfLifeDecoder.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\netlib\AHost.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\ndecoder\AIFFDecoder.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\ndecoder\AImageDecoder.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nickcpp\AInputStream.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\ndecoder\AJFIFDecoder.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\ALight3D.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\ALightwaveDecoder.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nickcpp\AList.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\AMapDecoder.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\AMatrix3D.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\AMayaDecoder.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\AMD2Decoder.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\AMD3Decoder.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\AMD5Decoder.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\AMDLDecoder.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\AMesh3D.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\AMeshDecoder.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\soundlib\AMidiDecoder.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nickcpp\AMovie.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\ndecoder\AMovieDecoder.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\soundlib\AMP3Decoder.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\ndecoder\AMultiImageDecoder.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\ndecoder\AMultiMovieDecoder.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\soundlib\AMultiSongDecoder.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\soundlib\AMultiSoundDecoder.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\netlib\ANetAbstract.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\netlib\ANetwork.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nickcpp\ANullStream.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nickcpp\AOutputStream.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nickcpp\APalette.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\APart3D.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\ndecoder\APCXDecoder.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\APLGDecoder.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nickcpp\APoint.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\ndecoder\AQTDecoderHelper.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\ndecoder\AQTImageDecoder.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\ndecoder\AQTMovieDecoder.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\soundlib\AQTSongDecoder.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\soundlib\AQTSoundDecoder.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nickcpp\ARect.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\ARendererAbstract3D.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\AScene3D.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\ASceneDecoder.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\ndecoder\ASmartDecoder.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\ndecoder\ASmartImageDecoder.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\ASmartMeshDecoder.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\ndecoder\ASmartMovieDecoder.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\ASmartSceneDecoder.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\soundlib\ASmartSongDecoder.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\soundlib\ASmartSoundDecoder.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\netlib\ASocket.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\netlib\ASocketNonBlocking.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\soundlib\ASong.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\soundlib\ASongDecoder.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\soundlib\ASound.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\soundlib\ASoundAbstract.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\soundlib\ASoundDecoder.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nickcpp\AStringHelper.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\ATextCoord3D.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\ATexture3D.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\ndecoder\ATGADecoder.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\AVertex3D.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\AVRMLDecoder.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\AWADDecoder.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\soundlib\AWAVDecoder.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\AWavefrontDecoder.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\AWLDDecoder.h
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\AWolf3dDecoder.h
# End Source File
# End Group
# End Target
# End Project
