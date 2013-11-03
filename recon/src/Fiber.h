#ifndef FIBER_H
#define FIBER_H

#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <algorithm>


class Fiber {

private:
  int fid;
  double fx;
  double fy;
  std::pair<double,double> fCoords;
  int fpixel;
  int fpin;  
  
public:
  Fiber();
  ~Fiber();
  
  void set_id(int a);
  void set_x(double a);
  void set_y(double a);
  void set_pin(int a);
  void set_pixel(int a);
  
  int id()       {return fid;}
  double x()     {return fx;}
  double y()     {return fy;}
  int pin()   {return fpin;}
  int pixel() {return fpixel;}   
  
  void dump();
  bool near(Fiber b);
  void set_coords(std::pair<double,double> coords);
  std::pair<double,double> coords(){return fCoords;}
};

#endif
