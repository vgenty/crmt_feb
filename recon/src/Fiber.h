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

public:
  Fiber();
  ~Fiber();

  void set_id(int a);
  void set_x(double a);
  void set_y(double a);
  
  int id()   {return fid;}
  double x() {return fx;}
  double y() {return fy;}
  void dump();
  bool near(Fiber b);

};

#endif
