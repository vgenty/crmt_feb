#include "Detector.h"

Detector::Detector(const double &gap){
  fGap = gap;
  init_modules();

}


void Detector::init_modules() {

  for (int i = 0; i<4; ++i)
    fModules.push_back(new RecoModule(i,fGap));

}

void Detector::recon_event(const std::map<int, std::vector<int> >& eventdata) {  
 
  for(auto module : eventdata){
    if(fModules[module.first]->check_event(module.second)) { //dont even try if not enough pins
     
      fModules[module.first] -> find_hit_fibers(module.second);
      fModules[module.first] -> fill_fibers();
      fModules[module.first] -> clusterize();
      fModules[module.first] -> attach();
      fModules[module.first] -> print_tracks();
    }
  }
  
  //Need to now pairwise reconstruct angle and create the Lines.

  
  //cleaning
  clean_event();
}

void Detector::clean_event(){
  
  for(const auto& module : fModules)
    module->clear();
  
}
