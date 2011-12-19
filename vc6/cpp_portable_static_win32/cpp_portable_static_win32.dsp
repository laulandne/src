# Microsoft Developer Studio Project File - Name="cpp_portable_static_win32" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=cpp_portable_static_win32 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "cpp_portable_static_win32.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "cpp_portable_static_win32.mak" CFG="cpp_portable_static_win32 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "cpp_portable_static_win32 - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "cpp_portable_static_win32 - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath "Desktop"
# PROP WCE_FormatVersion "6.0"
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "cpp_portable_static_win32 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "e:\src\cpp" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "ASYS_WIN32" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "cpp_portable_static_win32 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ  /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "e:\src\cpp" /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "ASYS_WIN32" /YX /FD /GZ  /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "cpp_portable_static_win32 - Win32 Release"
# Name "cpp_portable_static_win32 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\A3DDecoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\A3DSDecoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\soundlib\A8SVXDecoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nickcpp\AAbsRect.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\soundlib\AAUDecoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\soundlib\AAudioDevice.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nickcpp\ABitmap.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nickcpp\ABitmapAbstract.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\ndecoder\ABMPDecoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\ABspDecoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\ACamera3D.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nickcpp\ACell.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nickcpp\AClock.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nickcpp\AColor.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nickcpp\AConvertingBitLine.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nickcpp\ADataSourceAbstract.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\ndecoder\ADecoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\ndecoder\ADecoderHelper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nickcpp\ADrawableAbstract.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\ADXFDecoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nickcpp\AError.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\AFace3D.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nickcpp\AFileInputStream.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nickcpp\AFileOutputStream.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nickcpp\AFrameAbstract.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nickcpp\AGenericNode.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\ndecoder\AGIFDecoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\AHalfLifeDecoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\netlib\AHost.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\ndecoder\AIFFDecoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\ndecoder\AImageDecoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nickcpp\AInputStream.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\ndecoder\AJFIFDecoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\ALight3D.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\ALightwaveDecoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nickcpp\AList.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\AMapDecoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\AMatrix3D.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\AMayaDecoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\AMD2Decoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\AMD3Decoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\AMD5Decoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\AMDLDecoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\AMesh3D.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\AMeshDecoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\soundlib\AMidiDecoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nickcpp\AMovie.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\ndecoder\AMovieDecoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\soundlib\AMP3Decoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\ndecoder\AMultiImageDecoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\ndecoder\AMultiMovieDecoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\soundlib\AMultiSongDecoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\soundlib\AMultiSoundDecoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\netlib\ANetAbstract.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\netlib\ANetwork.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nickcpp\ANullStream.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nickcpp\AOutputStream.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nickcpp\APalette.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\APart3D.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\ndecoder\APCXDecoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\APLGDecoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nickcpp\APoint.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\ndecoder\AQTDecoderHelper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\ndecoder\AQTImageDecoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\ndecoder\AQTMovieDecoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\soundlib\AQTSongDecoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\soundlib\AQTSoundDecoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nickcpp\ARect.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\ARendererAbstract3D.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\AScene3D.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\ASceneDecoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\ndecoder\ASmartDecoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\ndecoder\ASmartImageDecoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\ASmartMeshDecoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\ndecoder\ASmartMovieDecoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\ASmartSceneDecoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\soundlib\ASmartSongDecoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\soundlib\ASmartSoundDecoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\netlib\ASocket.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\netlib\ASocketNonBlocking.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\soundlib\ASong.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\soundlib\ASongDecoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\soundlib\ASound.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\soundlib\ASoundAbstract.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\soundlib\ASoundDecoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nickcpp\AStringHelper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\ATextCoord3D.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\ATexture3D.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\ndecoder\ATGADecoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\AVertex3D.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\AVRMLDecoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\AWADDecoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\soundlib\AWAVDecoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\AWavefrontDecoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\AWLDDecoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpp\portable\nick3d\AWolf3dDecoder.cpp
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
