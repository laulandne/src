#ifndef PLATFORM_ALIB_PALM_H
#define PLATFORM_ALIB_PALM_H

/*

    ALib Project - A Platform-Independent General Purpose Class Library
    Copyright (C) 2006 Nicholas Lauland

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

    -------------------------------------------------------------------


*/

#include <alib/atypedefs.h>

#include <alib/aplatform.h>


#ifdef __PALMOS_H__
#ifndef ALIB_PALMOS6
/*******************************************************************
 * Copyright (c) 1994-1999 3Com Corporation or its subsidiaries.
 * All rights reserved.
 *

 *-------------------------------------------------------------------
 * FileName:  
 *		PilotStdio.h
 *
 * Description:
 *		Pilot standard IO equivalents.
 *
 * History:
 *		11/24/97	vmk		- Created by Vitaly Kruglikov 
 *
 *******************************************************************/

#ifndef __PILOTSTDIO_H__
#define __PILOTSTDIO_H__

#include <FileStream.h>


#define FILE		_opaque
#define size_t		ULong

#ifndef EOF
	#define EOF		(-1)
#endif

#define SEEK_SET	fileOriginBeginning
#define SEEK_CUR	fileOriginCurrent
#define SEEK_END	fileOriginEnd

#define fopenHelper(cardNo, filename, mode)																\
	FileOpen((cardNo)/*cardNo*/, (filename), 0/*def type*/, 0/*def creator*/,					\
	(((mode)[0] == 'r' && (mode)[1] == '+') ?	(fileModeUpdate		| fileModeExclusive) :	\
	((mode)[0] == 'r') ?								(fileModeReadOnly) :									\
	((mode)[0] == 'w') ?								(fileModeReadWrite	| fileModeExclusive) :	\
	((mode)[0] == 'a') ?								(fileModeAppend		| fileModeExclusive) :	\
	0) | fileModeAnyTypeCreator,																				\
	NULL)

#define fopen(filename, mode)	fopenHelper(0/*cardNo*/, (filename), (mode))
#define fclose(stream)			(FileClose(stream) ? EOF : 0)
#define fread(buffer, objsize, count, stream)	FileRead(stream, buffer, objsize, count, NULL)
#define fwrite(buffer, objsize, count, stream)	FileWrite(stream, buffer, objsize, count, NULL)
#define ferror(stream)			FileError(stream)
#define feof(stream)				FileEOF(stream)
#define ftell(stream)			FileTell(stream, NULL, NULL)
#define clearerr(stream)		FileClearerr(stream)
#define fseek(stream, offset, whence)	FileSeek(stream, offset, whence)
#define rewind(stream)			FileRewind(stream)
#define fflush(stream)			(FileFlush(stream) ? EOF : 0)
#define unlink(filename)		(FileDelete(0, (filename)) ? -1 : 0)


#define assert(condition)		ErrFatalDisplayIf(!(condition), "assertion failed: " # condition );

#endif
#endif // __PALMOS_H__
#endif // ALIB_PALMOS6


#ifdef ASYS_PALM
#ifndef ALIB_PALMOS6
#define stdout (FILE *)NULL
#define stdin (FILE *)NULL
#define stderr (FILE *)NULL
#endif // ALIB_PALMOS6
#endif // ASYS_PALM


#ifdef ASYS_PALM
struct stat {
  int st_size;
};
#endif // ASYS_PALM


#ifdef ASYS_PALM
extern FormPtr alibPalmFormP;
#endif // ASYS_PALM


#ifdef ASYS_PALM
extern "C" {
void palmMainFormInit(FormPtr frmP);
Boolean palmMainFormDoCommand(Word command);
Err palmRomVersionCompatible(DWord requiredVersion, Word launchFlags);
Err palmAppStart(void);
void palmAppStop(void);
DWord palmStarterPalmMain(Word cmd, Ptr cmdPBP, Word launchFlags);
Boolean palmAppHandleEvent( EventPtr eventP);
void palmAppEventLoopOnce(void);
VoidPtr palmGetObjectPtr(Word objectID);
//
int fputc(int c,FILE *fd);
int fputs(char *str,FILE *fd);
int fgetc(FILE *fd);
char *fgets(char *str,unsigned int len,FILE *fd);
int stat(char *filename,struct stat *s);
int fstat(int fd,struct stat *s);
char *getcwd(char *dir,unsigned int len);
int chdir(char *dir);
unsigned int wcslen(wchar_t *s);
#define strdup(x) strcpy((char *)malloc(strlen(x)+1),x)
#ifdef  __MWERKS__
// TODO: Use MathLib for these
int sin(float f);
int cos(float f);
#endif // __MWERKS__
};
int sscanf(char *s,char *f,unsigned int *val);
int fscanf(FILE *fd,char *f,char *val);
#endif // ASYS_PALM


#ifdef ASYS_PALM
extern "C" {
int AAppPalmMain(int argc,char *argv[]);
};
#endif // ASYS_PALM


#ifdef ASYS_PALM
#define appFileCreator  'NIK8'
#define appVersionNum  0x01
#define appPrefID  0x00
#define appPrefVersionNum  0x01
#define version20  0x02000000
#endif // ASYS_PALM


#ifdef ASYS_PALM
typedef struct 
{
  Byte replaceme;
} StarterPreferenceType;
#endif // ASYS_PALM


#ifdef ASYS_PALM
typedef struct 
{
  Byte replaceme;
} StarterAppInfoType;
typedef StarterAppInfoType* StarterAppInfoPtr;
#endif // ASYS_PALM


#ifdef ASYS_PALM
Boolean palmMainFormHandleEvent(EventPtr eventP);
#endif // ASYS_PALM


#endif // PLATFORM_ALIB_PALM_H
