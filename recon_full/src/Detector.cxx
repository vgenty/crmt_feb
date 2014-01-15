#include "Detector.h"

Detector::Detector(const double &gap){
  fGap = gap;
  init_modules();

}


void Detector::init_modules() {

  for (int i = 0; i<4; ++i)
    fModules.push_back(new RecoModule(i,fGap));

}

void Detector::recon_event(std::map<int, std::vector<int> >& eventdata,int &good_ones) {  

  bool good = false;
  for(auto module : eventdata){
    if(fModules[module.first]->check_event(module.second)) { //dont even try if not enough pins, or no identifier row
      //      std::cout << "good module: " << module.first << "\n";
      fModules[module.first] -> find_hit_fibers(module.second);
      fModules[module.first] -> fill_fibers();
      fModules[module.first] -> clusterize();
      fModules[module.first] -> attach();
      good = true;
    } else {
      //std::cout << "false on module.first: " << module.first << std::endl;
      good  = false;
      break;
    }
  }
  
  if(good){
    //std::cout << "doing good\n";
    fLines.first  -> do_tracks(fModules[1]->get_Tracks(),
			       fModules[3]->get_Tracks()); //XZ
    
    fLines.second -> do_tracks(fModules[0]->get_Tracks(),
			       fModules[2]->get_Tracks()); //YZ
    good_ones++;
  }
  
  //cleaning
  clean_event();
}

void Detector::clean_event(){
  
  for(auto module : fModules)
    module->clear();

  fLines.first ->clear_lines();
  fLines.second->clear_lines();
}
