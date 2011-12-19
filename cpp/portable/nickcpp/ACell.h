#ifndef ALIB_ACELL_H
#define ALIB_ACELL_H


// This is used internally by AList to store nodes...
class ACell
{
friend class AList;
public:
  ACell();
protected:
  void *info;  // could be a pointer to AGenericNode or some such
  ACell *prev;
  ACell *next;
};


#endif // ALIB_ACELL_H
