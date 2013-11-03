#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <stdlib.h>
#include "Fiber.h"
#include "RecoModule.h"
#include "Track.h"

int main()
{
  
  
  //Reco, generated recodata and I want to be able to read out
  //the angles, hit pins, 
  RecoModule *mm = new RecoModule();
  mm->getfiles("event_list2.txt","pins.csv");
  mm->init_module();
  
  std::map<int, std::vector<int> >::iterator itr;
  int evt=1;
  std::map<int, std::vector<int> > eventdata = mm->get_event_data();
  for(itr=eventdata.begin();itr!=eventdata.end();++itr){
    if(evt%100==0){
      std::cout << "-------------------" << std::endl;
      std::cout << "Event: " << evt << std::endl;
    }
    mm -> find_hit_fibers((*itr).second);
    mm -> fill_fibers();
    mm -> clusterize();
    mm -> attach();
    mm -> choose_angles();
    mm -> write_out((*itr).second);
    mm -> clear();   
    evt++;
  }
  mm->fill_root();
 
  //Event Display needs to take recoroot data.


  return 0;  
}
