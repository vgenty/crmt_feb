#include "Detector.h"

Detector::Detector(double gap){
  fGap = gap;
  init_modules();
}

Detector::~Detector(){}

void Detector::init_modules() {
 
  for (int i = 0; i<4; ++i)
    fModules.push_back(new RecoModule(i,fGap));
  
}

std::pair<Line,Line> Detector::recon_event(std::map<int, std::vector<int> >& eventdata,bool& good) {  
  //Clear reco events before new
  clean_event();
  
  good = false;
  for(auto module : eventdata){
    if(fModules[module.first]->check_event(module.second)) {
      fModules[module.first]->find_hit_fibers(module.second);
      fModules[module.first]->fill_fibers();
      fModules[module.first]->clusterize();
      fModules[module.first]->attach();
      good = true;
    
    } else {
      good  = false;
      break;
    }
  }
  
  if(good){
    fLines.first.do_track_reco(fModules[1]->get_Tracks(),
			       fModules[3]->get_Tracks()); //XZ
    fLines.second.do_track_reco(fModules[0]->get_Tracks(),
				fModules[2]->get_Tracks()); //YZ
  }
  
  return fLines;

}

void Detector::clean_event(){
  
  for(auto module : fModules)
    module->clear();
  
  fLines.first.clear_lines();
  fLines.second.clear_lines();
}
