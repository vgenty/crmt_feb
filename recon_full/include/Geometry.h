#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <iostream>
#include <string>
#include <stdlib.h>
#include <fstream>

class Geometry {

private:
  std::string fName;
  std::pair<double,double> fLocation[4][64];
  
  double fId;
  double fGap;
  
  double fBottom;
  double fWidth      = 1.04  ;
  double fOffset     = 0.3333;
  double fStopGap    = 0.48  ;
  double fHalfHeight = 1.54  ;


public:
  Geometry(const double &id,const double &gap);
  ~Geometry();
  
  void set_coordinates();
  void dump();
  std::pair<double,double> location(int a, int b){return fLocation[a][b];}

  const double get_HalfHeight(){return fHalfHeight;}
  const double get_Width(){return fWidth;}
  
};

#endif

