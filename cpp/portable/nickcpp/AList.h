#ifndef ALIB_ALIST_H
#define ALIB_ALIST_H


#include "ADataSourceAbstract.h"
#include "ACell.h"


// Simple linked list implementation
class AList : public ADataSourceAbstract
{
public:
  AList();
  virtual ~AList();
  virtual void kill();
  virtual void insert(void *n); // insert before current position
  virtual void append(void *n); // append after current position
  virtual void *remove(); // remove current position
  virtual void *info(); // info of current element
  virtual void advance(); // advance current position
  virtual void sort() { /* list is always unsorted */ }
  virtual void reset();
  virtual bool placeBookmark();
  virtual bool gotoBookmark();
  virtual bool atEnd();
  virtual bool isEmpty() { return !head; }
  virtual bool isSorted() { return false; }
protected:
  void init();
  ACell *head;
  ACell *tail;
  ACell *cur; // current position
  ACell *bookmark;
};


#endif // ALIB_ALIST_H
