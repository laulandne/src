#ifndef DLIB_DNATIVEFILESYSTEM_H
#define DLIB_DNATIVEFILESYSTEM_H


#include <portable/nickcpp/AList.h>
#include <portable/nickcpp/ANativeFilesystem.h>

#include <disklib/DDiskAbstract.h>


class DNativeFilesystem : public DDiskAbstract
{
public:
  DNativeFilesystem(DVfs *parent);
  virtual ~DNativeFilesystem();
  //
  virtual const char *Getcwd(char *buf,unsigned int len);
  virtual bool Chdir(const char *dname);
  virtual void *Fopen(const char *filename,const char *mode);
  virtual unsigned int Fclose(void *fp);
  virtual ULONG Fread(void *ptr,ULONG sizelem,ULONG n,void *fp);
  virtual ULONG Fwrite(void *ptr,ULONG sizelem,ULONG n,void *fp);
  virtual ULONG Fseek(void *fp,long offset,int origin);
  virtual ULONG Ftell(void *fp);
  virtual int Fstat(int fd,struct awstat_struct *st);
  virtual int Stat(const char *filename,struct awstat_struct *st);
  virtual void *Open(const char *filename,unsigned int mode);
  virtual unsigned int Close(void *fp);
  virtual ULONG Read(void *fp,void *ptr,ULONG n);
  virtual ULONG Write(void *fp,void *ptr,ULONG n);
  virtual ULONG Lseek(void *fp,long offset,int origin);
  virtual bool CopyFile(const char *oldname,const char *newname);
  virtual bool Rename(const char *oldname,const char *newname);
  // I wanted to call this "delete"...C++, bah!
  virtual bool Remove(const char *filename);
  virtual void refresh();
  virtual const char *getShortName(const char *longName);
  //
  virtual const char *which() { return "DNativeFilesystem"; }
  //
protected:
  //
  virtual void constructorHelper();
  virtual void readDirectory();
  //
#ifdef USE_FINDER_DAT
  void readFinderDat(FILE *f);
  bool gotFinderDat;
  AList finderDat;
#endif // USE_FINDER_DAT
  //
  ANativeFilesystem native;
};


#endif

