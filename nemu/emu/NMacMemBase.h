#ifndef EMU_NMACMEMBASE_H
#define EMU_NMACMEMBASE_H


// This'll be expanded at some point...
class NMacMemBase
{
public:
  ULONG loc;
  unsigned int len;
  void init() { loc=0;  len=0; }
  unsigned int getMin() { return loc; }
  unsigned int getMax() { return len+loc; }
  //void print(AWindow *w) { *w<<" from "<<getMin()<<" to "<<getMax()<<"\n"; }
protected:
};


#endif
