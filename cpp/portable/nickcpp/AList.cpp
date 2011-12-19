
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT *aNullStream


#include "AList.h"


////////////////////////////////////////////////////////////////////////////////
//  AList Class
////////////////////////////////////////////////////////////////////////////////

AList::AList()
{
  init();
}


void AList::init()
{
  ADataSourceAbstract::init();
  head=(ACell *)NULL;
  tail=(ACell *)NULL;
  cur=(ACell *)NULL;
  bookmark=(ACell *)NULL;
}


AList::~AList()
{
  kill();
}


void AList::kill()
{
  if(head) {
  ACell *p=head;
    while(p) {
      ACell *q=p;
      if(p) p=p->next;
      if(q) delete q;
    }
  }
  head=(ACell *)NULL;
  tail=(ACell *)NULL;
  cur=(ACell *)NULL;
}


void AList::insert( void *n )
{
  ACell *p=new ACell;
  if(!p) return;
  p->info=n;
  p->next=cur;
  if(cur) p->prev=cur->prev;
  if(p->next) p->next->prev=p;
  if(p->prev) p->prev->next=p;
  cur=p;
  if(!p->prev) head=p;
  if(!p->next) tail=p;
}


void AList::append(void *n)
{
  if(!cur) cur=tail;
  ACell *p=new ACell;
  if(!p) return;
  p->info=n;
  p->prev=cur;
  if(cur) p->next=cur->next;
  if(p->next) p->next->prev=p;
  if(p->prev) p->prev->next=p;
  cur=p;
  if(!p->prev) head=p;
  if(!p->next) tail=p;
}


// FIXME: I'm still a little iffy on this one...
void *AList::remove()
{
  if(!head) return NULL;
  if(!tail) return NULL;
  if(!cur) cur=tail;
  ACell *p=cur;
  void *r=cur->info;
  if(p->next) p->next->prev=p->prev;
  if(p->prev) p->prev->next=p->next;
  if(p->next) cur=p->next;
  else cur=p->prev;
  //if(!head->prev) head=p->next;
  //if(!tail->next) tail=p->prev;
  if(!p->next) tail=p->prev;
  if(!p->prev) head=p->next;
  if(!cur) head=(ACell *)NULL;
  if(!cur) tail=(ACell *)NULL;
  delete p;
  return r;
}


void *AList::info()
{
  void *ret=NULL;
  if(cur) ret=cur->info;
  return ret;
}


void AList::advance()
{
  if(cur) {
     cur=cur->next;
  }
}


void AList::reset()
{
  cur=head;
}


bool AList::atEnd()
{
  bool ret=false;
  if(!cur) {
    ret=true;
  }
  return ret;
}


bool AList::placeBookmark()
{
  bookmark=cur;
  return true;
}


bool AList::gotoBookmark()
{
  cur=bookmark;
  return true;
}




