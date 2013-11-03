#ifndef TRACK_H
#define TRACK_H

#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <algorithm>
#include "Fiber.h"
#include "TGraphErrors.h"
#include "TF1.h"
#include "math.h"

class Track {
  
private:
  std::vector<Fiber> fFibers;
  TGraphErrors *fTG;
  TF1 *fFit;
  /*std::pair<double,double> fSlope;
  std::pair<double,double> fYinter;
  std::pair<double,double> fAngle;*/
  double fSlope;
  double fYinter;
  double fAngle;
  double fCosAngle;
  double fChi;
  double fNdf;
  double fPvalue;
  int fid;
  
public:
  Track();
  ~Track();
  
  void add_fiber(Fiber a);
  void set_id(int a);
  int id() {return fid;}
  bool contains(Fiber a);
  int size(){return fFibers.size();}
  std::vector<Fiber> fibers(){return fFibers;}
  void dump();
  void fit();
  void calculate_angle();
  void reconstruct();
  
  
  
  double slope(){return fSlope;}
  double yinter(){return fYinter;}
  double angle(){return fAngle;}
  double chi(){return fChi;}
  double ndf(){return fNdf;}
  double pvalue(){return fPvalue;}
  double cosangle(){return fCosAngle;}
  
};

#endif
