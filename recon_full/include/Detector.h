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
  std::pair<Line*,Line*> fLines = std::make_pair(new Line(true),new Line(false)); //XZ then YZ lines

public:
  Detector(const double &gap);
  ~Detector();

  void init_modules();
  void recon_event(std::map<int, std::vector<int> >& eventdata,int &good_ones);
  void clean_event();

};

#endif
