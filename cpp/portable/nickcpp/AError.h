#ifndef ALIB_AERROR_H
#define ALIB_AERROR_H


// NOTE: You can't reset the flag, on purpose, all errors are considered fatal.

class AError
{
public:
  AError();
  bool error();
  void set();
protected:
  bool errorFlag;
};


#endif // ALIB_AERROR_H
