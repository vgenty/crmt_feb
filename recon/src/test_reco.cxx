#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <stdlib.h>
#include "Fiber.h"
#include "RecoModule.h"
#include "Track.h"
#include "FileManager.h"
int main()
{
  
  
  //Reco, generated recodata and I want to be able to read out
  //the angles, hit pins, 
  RecoModule  *mm = new RecoModule();
  FileManager *fm = new FileManager();
  
  mm->getfiles("event_list2.txt","pins.csv");
  mm->init_module(); 
  fm->open_file("recodata.root");
  
  
  std::map<int, std::vector<int> >::iterator itr;
  int evt=1;
  std::map<int, std::vector<int> > eventdata = mm->get_event_data();
  for(itr=eventdata.begin();itr!=eventdata.end();++itr){
    if(evt%100==0){
      std::cout << "-------------------" << std::endl;
      std::cout << "Event: " << evt << std::endl;
    }
    //hit pins: (*itr).second
    mm -> find_hit_fibers((*itr).second);
    mm -> fill_fibers();
    mm -> clusterize();
    mm -> attach();
    mm -> choose_angles();
    if (mm->conditions_are_met()){
      fm -> fill_event_tree(evt,
			    mm->get_Slope(),mm->get_YInter(),
			    mm->get_Chi(),mm->get_Ndf(),
			    mm->get_Pvalue(),mm->get_Angle(),
			    mm->get_CosAngle(),(*itr).second,
			    mm->get_Tracks());
      evt++;    
    }
    mm -> clear();   
  }
  fm -> finish();
  
  //Event Display needs to take recoroot data.


  return 0;  
}
