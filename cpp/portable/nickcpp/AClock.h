#ifndef ALIB_ACLOCK_H
#define ALIB_ACLOCK_H


class AClock {
public:
  unsigned long read();
  unsigned long getTick();
  unsigned long getMilli();
  unsigned long getFreq();
private:
};


#endif // ALIB_ACLOCK_H

