#ifndef ELIB_EBYTECODEOBJECT_H
#define ELIB_EBYTECODEOBJECT_H





// The actual use of each field is defined by the child classes
class ECPUByteCodeObject {
public:
  unsigned int type;
  unsigned int len;
  unsigned int val;
  unsigned int name;
protected:
};

#endif
