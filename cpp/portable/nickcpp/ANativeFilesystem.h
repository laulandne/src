#ifndef ALIB_ANATIVEFILESYSTEM_H
#define ALIB_ANATIVEFILESYSTEM_H


#define ALIB_HAVE_UNISTD_H 1
#define ALIB_HAVE_SYS_STAT_H 1
//#define ALIB_HAVE_STAT_H 1
#define ALIB_HAVE_DIRENT_H 1
//#define ALIB_HAVE_DIRECT_H 1
#define ALIB_HAVE_SYS_DIR_H 1


#ifdef ASYS_WIN32
// TODO: define and undef headers we have here different from the norm
#endif // ASYS_WIN32


#ifdef ALIB_HAVE_UNISTD_H
#include <unistd.h>
#endif // ALIB_HAVE_UNISTD_H

#ifdef ALIB_HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif

#ifdef ALIB_HAVE_STAT_H
#include <stat.h>
#endif

#ifdef ALIB_HAVE_DIRENT_H
#include <dirent.h>
#endif // ALIB_HAVE_DIRENT_H

#ifdef ALIB_HAVE_DIRECT_H
#include <direct.h>
#endif // ALIB_HAVE_DIRECT_H

#ifdef ALIB_HAVE_SYS_DIR_H
#include <sys/dir.h>
#endif // ALIB_HAVE_SYS_DIR_H


class ANativeFilesystem {
public:
  ANativeFilesystem();
private:
#ifdef ALIB_HAVE_DIRENT_H
   dirent **theDirents;
#endif
#ifdef ALIB_HAVE_DIRECT_H
   direct **theDirents;
#endif
};


#endif // ALIB_ANATIVEFILESYSTEM_H

