#include "PreSelect.h"
#include "AtmEvent.h"

PreSelect::PreSelect(Params & a){
   
     isselect = false ;
     params = a;
}

PreSelect::~PreSelect(){
  
}
 
/**PreSelect::Execute
 * it will select the event we want based on our parameters we set in 
 * the  command line
 *
 *
*/


void PreSelect::Execute(AtmEvent & event){
   
    if(params.interType.size()){
         std::vector<int>::iterator it; 
         it=std::find(params.interType.begin(),params.interType.end(),event.data.interType);
         if(it == params.interType.end())
         {
           isselect = false ;
           return;
         }        
      }

   if(params.TransEMax or params.TransEMin){
   
      double tfinalnuE = 0.;
      double entrans = 0.;
      if (event.data.ctag == 1) {
           for (int j = 0; j < event.data.Npars; j++) {
              if (event.data.pdg[j] == 12 || event.data.pdg[j] == -12 || event.data.pdg[j] == 14 || event.data.pdg[j] == -14) {
                tfinalnuE = sqrt(event.data.px[j] * event.data.px[j] + event.data.py[j] * event.data.py[j] + event.data.pz[j] * event.data.pz[j]);
                }
            }
             entrans = event.data.pEn - tfinalnuE;
         } else {
          entrans = event.data.pEn;
        }
    
      if( (entrans > params.TransEMax) or (entrans < params.TransEMin)){
          isselect = false;
          return;
        }
     }

      if(params.currentTag){

           if(params.currentTag != event.data.ctag ){
               isselect = false;
               return;
           }
         }
      if(params.flavorTag.size())
      {
    	  std::vector<int>::iterator it;
    	  it = std::find(params.flavorTag.begin(),params.flavorTag.end(),event.data.pPdg);
    	  if(it == params.flavorTag.end())
    	  {
    		  isselect = false;
    		  return;
    	  }
      }
      isselect = true;
      return;
}
