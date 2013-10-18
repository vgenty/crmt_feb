#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <iostream>
#include <string>
#include <stdlib.h>

class Geometry {

private:
  std::string fName;
  std::pair<double,double> fLocation[4][64];
  double fHalfHeight = 1.54;
  double fWidth      = 1.04;
  double fOffset     = 0.33;
  double fStopGap    = 0.48;
public:
  Geometry();
  ~Geometry();
  
  void set_coordinates();
  void dump();
  
};

#endif
