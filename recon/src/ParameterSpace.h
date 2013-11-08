#ifndef PARAMETERSPACE_H
#define PARAMETERSPACE_H

#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <algorithm>
#include "Fiber.h"
#include "Geometry.h"
#include "Track.h"

class ParameterSpace {
  
private:
  Geometry g;
  double fMaxSlope;
  double fMinSlope;
  double fMaxYinter;
  double fMinYinter;
  int fNSlopeDivisions;
  int fNYinterDivisions;
  std::vector<std::pair<double,double> > fRecoPoints;
  double fSlopeStep;
  double fYinterStep;
  
  //this hold all parameterspace
  //key   : slope,y_inter
  //value : vector of (x,y) locations of intersected fibers
  std::map<std::pair<double,double>,std::vector<std::pair<double,double>> > fPSpace;
  
public:
  ParameterSpace();
  ~ParameterSpace();
  

  
  void Fill_Space();

  bool Intersection(std::pair<double,double> params, double Slope, double Yint);
  
  std::map<int,std::vector<double> > CreateSpace();
  
  void TrackOpener(std::vector<Track> tracks);
  void set_NSlopeDivisions(int a);
  void set_NYinterDivisions(int a);
  
  double get_MaxSlope(){return fMaxSlope;}
  double get_MinSlope(){return fMinSlope;}
  double get_MaxYinter(){return fMaxYinter;}
  double get_MinYinter(){return fMinYinter;}
};

#endif
