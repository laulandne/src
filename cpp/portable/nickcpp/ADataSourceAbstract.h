#ifndef ALIB_ADATASOURCEABSTRACT_H
#define ALIB_ADATASOURCEABSTRACT_H


#include <stdio.h>


class ADataSourceAbstract
{
public:
  ADataSourceAbstract();
  ADataSourceAbstract(char *name,char *table,char *host=NULL,char *user=NULL,char *password=NULL);
  virtual ~ADataSourceAbstract();
  virtual void kill();
  virtual void sort();
  virtual void reset();
  virtual void insert(void *n);
  virtual void append(void *n);
  virtual void *remove();
  virtual void push(void *n) { append(n); }
  virtual void *pop() { return remove(); }
  virtual void *info();
  virtual void *get() { return info(); }
  virtual void advance();
  virtual void next() { advance(); }
  virtual bool placeBookmark();
  virtual bool gotoBookmark();
  virtual bool atEnd();
  virtual bool isEmpty();
  virtual bool isSorted();
  virtual bool isConnected();
protected:
  void init();
  bool connected;
  char *name;
  char *table;
  char *host;
  char *user;
};


#endif // ALIB_ADATASOURCEABSTRACT_H
