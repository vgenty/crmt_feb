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
  Fiber(){
    fid=0;
    fx=0;
    fy=0;
  };
  ~Fiber(){};

  void set_id(int a){fid=a;}
  void set_x(double a){fx=a;}
  void set_y(double a){fy=a;}

  int id(){return fid;}
  double x(){return fx;}
  double y(){return fy;}
  void dump(){
    std::cout << " id " << id() << " x " << x() << " y " << y() << std::endl;
  }
  bool near(Fiber b);

};

#endif
