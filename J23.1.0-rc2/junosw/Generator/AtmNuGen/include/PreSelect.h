#ifndef PRESELECT_hh
#define PRESELECT_hh

#include "Params.h"

//#include "AtmEvent.h"

class AtmEvent;
class PreSelect{

  public:
     PreSelect(Params & a);
    ~PreSelect();
  private:
     Params params;
     bool isselect;
  public:
     void Execute(AtmEvent & event );
     bool GetIsSelect(){return  isselect;}

}; 
#endif
