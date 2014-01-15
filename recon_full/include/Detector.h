#ifndef DETECTOR_H 
#define DETECTOR_H

#include "RecoModule.h"
#include "Line.h"
#include "Geometry.h"

class Detector {
  
private:
  //std::pair<std::vector<Track>,std::vector<Track> > fAngles;
  std::vector<RecoModule*> fModules;
  std::vector<RecoModule*>::iterator fModIter;
  double fGap;
  
public:
  Detector(const double &gap);
  ~Detector();

  void init_modules();
  void recon_event(const std::map<int, std::vector<int> >& eventdata);
  void clean_event();

};

#endif
