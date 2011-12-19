#ifndef ALIB_AGENERICNODE_H
#define ALIB_AGENERICNODE_H


// values returned by comparision functions
#define A_COMPARE_UNKNOWN  42
#define A_COMPARE_SAME     0
#define A_COMPARE_BEFORE   -1
#define A_COMPARE_AFTER    1


// The idea is that this is what *info points to...in
// practice this class is never used...
class AGenericNode
{
public:
  virtual int compare(AGenericNode *other);
protected:
};


#endif // ALIB_AGENERICNODE_H
