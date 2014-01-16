#ifndef DETECTOR_H 
#define DETECTOR_H

#include <utility>

#include "RecoModule.h"
#include "Line.h"


class Detector {
  
private:
  
  std::vector<RecoModule*> fModules;
  double fGap;
  std::pair<Line,Line> fLines = std::make_pair(Line(true),Line(false));

public:
  Detector(double gap);
  ~Detector();
  
  void init_modules();
  std::pair<Line,Line> recon_event(std::map<int, std::vector<int> >& eventdata,bool& good); 
    
  void clean_event();
  
};

#endif
