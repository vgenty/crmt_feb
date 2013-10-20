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

  RecoModule *mm = new RecoModule();
  mm->getfiles("event_list.txt","pins.csv");
  mm->init_module();

  std::map<int, std::vector<int> >::iterator itr;
  int evt=1;
  
  std::map<int, std::vector<int> > eventdata = mm->get_event_data();
  for(itr=eventdata.begin();itr!=eventdata.end();++itr){
   std::cout << "-------------------" << std::endl;
   std::cout << "Event: " << evt << std::endl;
   mm->find_hit_fibers((*itr).second);
   for (int oo=0;oo<((*itr).second).size();oo++)
     std::cout << "have: " << ((*itr).second).at(oo) << " ";
   
   std::cout << std::endl;
   mm -> fill_fibers();
   mm -> clusterize();
   mm -> attach();
   mm -> choose_angles();
   mm -> clear();   
   evt++;
  }
  mm->fill_root();
  return 0;
}

